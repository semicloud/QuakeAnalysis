#include "Rad2bt.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/log/trivial.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

std::optional<arma::fmat> Rad2bt::load_lut_table(const std::string& lut_path)
{
	if (!std::filesystem::is_regular_file(lut_path))
	{
		BOOST_LOG_TRIVIAL(error) << "LUT表文件" << lut_path << "不是一个合法文件";
		return std::optional<arma::fmat>();
	}
	std::string line;
	std::ifstream ifs(lut_path);
	if (!ifs)
	{
		BOOST_LOG_TRIVIAL(error) << "无法打开LUT表文件" << lut_path;
		return std::optional<arma::fmat>();
	}
	std::stringstream ss;
	while (getline(ifs, line))
	{
		//see http://arma.sourceforge.net/docs.html#Mat
		//When using the mat(string) constructor, the format is elements separated by spaces
		//and rows denoted by semicolons; for example, the 2x2 identity matrix can be created using "1 0; 0 1". 
		//跳过首行
		if (line.find("BT") != std::string::npos) continue;
		ss << line << ";";
	}
	const arma::fmat lut_mat(ss.str());
	//cout << lut_mat;
	BOOST_LOG_TRIVIAL(info) << "已加载LUT表，共" << lut_mat.size() << "条记录，LUT表路径：" << lut_path;
	return std::optional<arma::fmat>(lut_mat);
}

int Rad2bt::get_col_index(const std::string& lut_path, const std::string& col_name)
{
	std::ifstream ifs(lut_path);
	std::string line;
	std::getline(ifs, line);
	if (line.find("BT") == std::string::npos)
	{
		BOOST_LOG_TRIVIAL(error) << "LUT表" << lut_path << "，表头无效，找不到BT列";
		return -1;
	}
	boost::trim(line);
	std::vector<std::string> col_names;
	split(col_names, line, boost::is_any_of(" "), boost::token_compress_on);
	const std::vector<std::string>::iterator it = std::find(col_names.begin(), col_names.end(), col_name);
	if (it == col_names.end())
	{
		BOOST_LOG_TRIVIAL(error) << "LUT表" << lut_path << "，不含数据列" << col_name;
		return -1;
	}
	return static_cast<int>(std::distance(col_names.begin(), it));
}

float Rad2bt::calc_band_bt(const arma::fvec& bt_lut,
	const arma::fvec& rad_lut, const arma::uword line_num, float rad)
{
	int idx_min = -1;
	int idx_max = -1;
	float bt = -1.0;

	for (int idx = 0; idx < line_num; ++idx)
	{
		if (rad_lut[idx] < rad)
		{
			idx_min = idx;
			continue;
		}

		if (rad_lut[idx] > rad)
		{
			idx_max = idx;
			break;
		}

		if (abs(rad_lut[idx] - rad) < 1E-05)
		{
			bt = bt_lut[idx];
			idx_min = -1;
			idx_max = -1;
		}
	}

	if (idx_min == -1 || idx_max == -1)
	{
		return bt;
	}

	float ymin = bt_lut[idx_min];
	float ymax = bt_lut[idx_max];
	float xmin = rad_lut[idx_min];
	float xmax = rad_lut[idx_max];

	bt = ymin + (ymax - ymin) / (xmax - xmin) * (rad - xmin);
	//g.Debug("bt: " + bt);
	return bt;
}

Rad2bt::Rad2bt()
{
}


Rad2bt::~Rad2bt()
{
}

std::optional<arma::fmat> Rad2bt::load_lut_cols(const std::string& lut_path, const int band)
{
	using namespace std;
	using namespace arma;
	const optional<fmat> lut_mat = load_lut_table(lut_path);
	const uword bt_index = get_col_index(lut_path, get_bt_col_name());
	const uword band_index = get_col_index(lut_path, get_rad_col_name(to_string(band)));
	const vector<uword> col_indexes{ bt_index, band_index };
	BOOST_LOG_TRIVIAL(debug) << "bt_index: " << bt_index << ", band_index: " << band_index;
	const fvec bt_col = lut_mat->col(bt_index);
	const fvec band_col = lut_mat->col(band_index);
	assert(bt_col.size() == band_col.size());
	fmat ans(bt_col.size(), 2, fill::zeros);
	ans.col(0) = bt_col;
	ans.col(1) = band_col;
	return optional<fmat>(ans);
}
