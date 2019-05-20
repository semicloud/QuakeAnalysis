// rst_en.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "../eddy_field/Eddy_field.h"
#include "../modis_api/Date_utils.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Logger_setting.h"
#include "../modis_api/String_utils.h"
#include "Cube_utils.h"
#include "Rst.h"
#include "Rst_checker.h"
#include "Rst_prep.h"
#include "Rst_prep_file_op.h"
#include "Rst_prep_input.h"
#include "Wavelet_utils.h"
#include <algorithm>
#include <armadillo>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

using namespace std;
namespace fs = boost::filesystem;
namespace gr = boost::gregorian;
namespace po = boost::program_options;

typedef modis_api::Date_utils du;
typedef modis_api::File_operation fo;

const string PROGRAM = "rst_en";
const string VERSION = "1.0";

struct Map_key_compare {
	bool operator()(const unsigned k1, const unsigned k2)const {
		return k1 > k2;
	}
};

inline void init_logger_setting()
{
	modis_api::init_console_logger();
	modis_api::init_file_logger("logs\\", "rst_en");
	modis_api::set_logger_severity(boost::log::trivial::info);
}

/**
 * \brief 获取年份范围，给定(2018,2016)，返回[2017,2016]
 * \param start
 * \param end
 * \return
 */
std::vector<unsigned>
get_year_ranges_except_start(const unsigned start, const unsigned end);

/**
 * \brief 获取当前年份及历史年份
 * \param cur_year 开始年份
 * \param min_year 中止年份
 * \return 从开始年份到中止年份，获取年份及该年份的历史年份
 * \remark <2018,[2017,2016]>, <2017,[2016]>, <2016,[]>
 */
std::map<unsigned, std::vector<unsigned>, Map_key_compare>
get_cur_hist_year_pairs(const unsigned cur_year, const unsigned min_year);

/**
 * \brief 获取某年份的cube，并重新调整大小以用于GSL的小波变换
 * \param all_files 全部的文件
 * \param all_doys 全部的doy
 * \param year 年份
 * \return cube
 */
arma::fcube
load_cube_by_year_and_shrink(const std::vector<std::string>& all_files,
	const std::vector<std::string>& all_doys, const unsigned year);

/**
 * \brief 获取某年的文件
 * \param all_files
 * \param all_doys
 * \param year
 * \return
 */
std::vector<std::string>
get_year_files(const std::vector<std::string>& all_files,
	const std::vector<std::string>& all_doys, const unsigned year);

/**
 * \brief 处理输入
 * \param argc
 * \param argv
 * \param out_data_dir 数据目录，输出
 * \param out_start_date_str 开始日期字符串，输出
 * \param out_end_date_str 结束日期字符串，输出
 * \param out_bg_tif_dir 背景值文件目录，输出
 * \param out_output_data_dir 结果文件目录，输出
 * \param out_k 用于填充背景值的参数，输出
 */
void handle_inputs(int argc, char** argv, std::string& out_data_dir,
	std::string& out_start_date_str, std::string& out_end_date_str,
	std::string& out_bg_tif_dir, std::string& out_output_data_dir, unsigned& out_k)
{
	std::string yml_path;
	po::variables_map vm;
	po::options_description desc("Usage:");
	desc.add_options()
		("help,h", "显示帮助信息")
		("version,v", "显示版本信息")
		("yml,y", po::value(&yml_path), ".yml配置文件路径，必须")
		("debug,d", "以Debug模式运行本程序");

	try
	{
		store(po::command_line_parser(argc, argv).options(desc).run(), vm);
		vm.notify();
	}
	catch (exception& e)
	{
		cerr << e.what();
		exit(EXIT_FAILURE);
	}

	if (vm.count("help"))
	{
		std::cout << "\n RST(Robust Satellite Techniques) Enhancement算法 版本："
			<< VERSION << "\n" << std::endl;
		std::cout << desc << std::endl;
		exit(EXIT_SUCCESS);
	}

	if (vm.count("version"))
	{
		std::cout << "\n RST(Robust Satellite Techniques) Enhancement算法 版本："
			<< VERSION << "\n" << std::endl;
		exit(EXIT_SUCCESS);
	}

	if (vm.count("yml") && boost::filesystem::exists(yml_path))
	{
		if (vm.count("debug"))
		{
			modis_api::set_logger_severity(boost::log::trivial::debug);
			BOOST_LOG_TRIVIAL(debug) << "本程序将以Debug模式运行";
		}
		try
		{
			BOOST_LOG_TRIVIAL(info) << "加载.yml配置文件：" << yml_path;

			const YAML::Node node = YAML::LoadFile(yml_path);
			out_data_dir = node["DataDir"].as<std::string>();
			modis_api::File_operation::process_dir(out_data_dir);
			out_start_date_str = node["StartDate"].as<std::string>();
			out_end_date_str = node["EndDate"].as<std::string>();
			out_bg_tif_dir = node["PrepTifDir"].as<std::string>();
			modis_api::File_operation::process_dir(out_bg_tif_dir);
			out_output_data_dir = node["OutputDataDir"].as<std::string>();
			modis_api::File_operation::process_dir(out_output_data_dir);
			out_k = node["PrepSupport"].as<unsigned>();

			BOOST_LOG_TRIVIAL(debug) << "DataDir:" << out_data_dir;
			BOOST_LOG_TRIVIAL(debug) << "Start Date:" << out_start_date_str;
			BOOST_LOG_TRIVIAL(debug) << "End Date:" << out_end_date_str;
			BOOST_LOG_TRIVIAL(debug) << "PrepTifDir:" << out_bg_tif_dir;
			BOOST_LOG_TRIVIAL(debug) << "OutputDataDir:" << out_output_data_dir;
			BOOST_LOG_TRIVIAL(debug) << "PrepSupport:" << out_k;
		}
		catch (exception& ex)
		{
			std::cerr << "解析.yml配置文件：" << yml_path
				<< "失败！";
			std::cerr << ex.what() << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		std::cerr << "必须使用-y或--yml指定.yml配置文件！";
		exit(EXIT_FAILURE);
	}
}

/**
 * \brief 填充背景值
 * \param current_year 要处理的数据的年份
 * \param all_tif_files 全部的数据文件
 * \param generated_doys 按照起止日期生成的DOY
 * \param output_dir 背景值文件输出目录
 * \param k 填充值k值
 */
void fill_background(unsigned current_year,
	const std::vector<std::string>& all_tif_files,
	const std::vector<std::string>& generated_doys,
	const std::string& output_dir, unsigned k)
{
	const auto rst_prep_inputs = Rst_prep_input::load(current_year,
		all_tif_files, generated_doys, output_dir, k);
	Rst_prep::preprocess(rst_prep_inputs);
}

/**
 * \brief 获取原地温度cube序列
 * \param current_year 当前年份
 * \param all_bg_tifs 用于计算的tif文件
 * \param years 年份序列
 * \param generated_doys 生成的DOY
 * \return
 */
std::vector<arma::fcube> get_ydwd_cubes(unsigned current_year,
	const std::vector<std::string>& all_bg_tifs,
	const std::vector<unsigned>& years,
	const std::vector<std::string>& generated_doys)
{
	const unsigned min_year = *std::min_element(years.begin(), years.end());

	std::map<unsigned, std::vector<unsigned>, Map_key_compare> year_info_map =
		get_cur_hist_year_pairs(current_year, min_year);

	// 历年原地温度cubes，第一个cube是第一年的数据，其他的是历史cube
	std::vector<arma::fcube> y_cubes;

	for (std::pair<unsigned, std::vector<unsigned>> year_info : year_info_map)
	{
		const unsigned cur_year = year_info.first;
		const std::vector<unsigned> hist_years = year_info.second;
		if (hist_years.empty())
			continue;

		BOOST_LOG_TRIVIAL(debug) << "当前年份：" << cur_year;
		BOOST_LOG_TRIVIAL(debug) << "历史年份："
			<< boost::algorithm::join(String_utils::conv_vec_str(hist_years), ",");

		arma::fcube cur_cube =
			load_cube_by_year_and_shrink(all_bg_tifs, generated_doys, cur_year);
		BOOST_LOG_TRIVIAL(debug) << cur_year << "年的数据已加载，并已缩减";

		std::vector<arma::fcube> hist_cubes;
		hist_cubes.reserve(hist_years.size());
		for (const unsigned hist_year : hist_years)
		{
			arma::fcube hist_cube = load_cube_by_year_and_shrink(all_bg_tifs,
				generated_doys, hist_year);
			hist_cubes.push_back(hist_cube);
			BOOST_LOG_TRIVIAL(debug) << hist_year << "年的数据已加载，并已缩减";
		}

		arma::fcube background_field_cube =
			Rst::construct_background_field(hist_cubes);

		BOOST_LOG_TRIVIAL(info) << "背景场 [" << hist_years[0]
			<< "-" << hist_years[hist_years.size() - 1] << "] 构建完成";

		arma::fcube residual_cube = cur_cube - background_field_cube;
		arma::fcube wavelet_residual_cube =
			Cube_utils::cube_wavelet2d(residual_cube);

		y_cubes.push_back(wavelet_residual_cube);

		BOOST_LOG_TRIVIAL(info) << "构建" << cur_year << "的残差二维小波变换Cube完成";
	}

	return y_cubes;
}

/**
 * \brief 获取alice_cube
 * \param ydwd_cubes 原地温度cubes
 * \return
 */
arma::fcube get_alice_cube(std::vector<arma::fcube>& ydwd_cubes)
{
	const arma::fcube cur_y_cube = ydwd_cubes.front();
	ydwd_cubes.erase(ydwd_cubes.begin());
	//BOOST_LOG_TRIVIAL(debug) << "y_cubes size: " << ydwd_cubes.size();

	const arma::fcube mean_cube = Cube_utils::mean_cubes(ydwd_cubes);
	const arma::fcube sd_cube = Cube_utils::sd_cubes(ydwd_cubes);
	arma::fcube alice_cube = (cur_y_cube - mean_cube) / sd_cube;
	return alice_cube;
}

/**
 * \brief 保存cube，cube的slice必须与保存的文件列表长度一致
 * \param paths 保存的文件地址
 * \param cube 数据立方
 * \param geo_trans
 * \param proj
 */
void save_cube(const std::vector<std::string>& paths, arma::fcube& cube,
	double* geo_trans, const std::string& proj)
{
	if (paths.size() != cube.n_slices)
	{
		BOOST_LOG_TRIVIAL(error) << "给定的文件存储路径个数与矩阵个数不等！" <<
			"文件存储路径个数：" << paths.size() << "，矩阵个数：" << cube.n_slices;
		return;
	}
	for (arma::uword i = 0; i < cube.n_slices; ++i)
	{
		modis_api::Gdal_operation::
			create_tif(paths[i], geo_trans, proj, cube.slice(i));
	}
}

/**
 * \brief 获取输出文件列表，以输入文件的文件名作为输出文件名
 * \param output_dir 输出目录
 * \param current_year_tifs 输入文件列表
 * \return
 */
std::vector<std::string> get_output_files(const std::string& output_dir,
	const std::vector<std::string>& current_year_tifs)
{
	std::vector<std::string> file_names = fo::get_file_names(current_year_tifs);
	std::vector<std::string> ret(file_names);
	std::transform(file_names.begin(), file_names.end(), ret.begin(),
		[&output_dir](const std::string& f) { return output_dir + f; });
	return ret;
}

/**
 * \brief 执行rst_en算法
 * \param start_date 开始日期
 * \param end_date 结束日期
 * \param in_dir 输入目录
 * \param out_dir 输出目录
 */
void do_rst_en(const gr::date& start_date, const gr::date& end_date,
	const std::string& in_dir, const std::string& out_dir)
{
	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "准备执行RST主算法..";
	BOOST_LOG_TRIVIAL(info) << "输入数据目录：" << in_dir;

	const unsigned current_year = start_date.year();
	std::ostringstream err_os;

	std::vector<std::string> tif_files =
		fo::get_all_files_by_extension(in_dir, ".tif");
	BOOST_LOG_TRIVIAL(debug) << "输入数据目录" << in_dir << "下共找到" <<
		tif_files.size() << "个tif文件";

	std::vector<unsigned> years =
		Rst_prep_file_op::get_file_years(tif_files);
	BOOST_LOG_TRIVIAL(debug) << "输入数据目录" << in_dir << "下找到[" <<
		boost::algorithm::join(String_utils::conv_vec_str(years), ",") <<
		"]年的数据";
	if (!Rst_checker::check_year(years, err_os))
	{
		std::cerr << err_os.str() << std::endl;
		exit(EXIT_FAILURE);
	}
	err_os.clear();

	const unsigned min_year = *std::min_element(years.begin(), years.end());
	BOOST_LOG_TRIVIAL(debug) << "输入数据目录" << in_dir << "下的最小年份为"
		<< min_year;

	std::vector<std::string> generate_doys =
		Rst_prep_file_op::generate_doys(start_date, end_date, min_year);
	BOOST_LOG_TRIVIAL(debug) << "生成的DOY:"
		<< boost::algorithm::join(generate_doys, ",");
	if (!Rst_checker::check_doy_file_ready(tif_files, generate_doys, err_os))
	{
		std::cerr << "根据生成的DOY检查输入数据目录" << in_dir <<
			"下的tif文件，缺少相关tif文件！" << std::endl;
		std::cerr << err_os.str() << std::endl;
		exit(EXIT_FAILURE);
	}
	err_os.clear();

	std::vector<std::string> tif_files_curr_year =
		get_year_files(tif_files, generate_doys, current_year);
	BOOST_LOG_TRIVIAL(debug) << "输入数据目录" << in_dir << "下"
		<< current_year << "年的文件：" <<
		"\n" << boost::algorithm::join(tif_files_curr_year, "\n");

	if (!Rst_checker::check_tif(tif_files, err_os))
	{
		std::cerr << "输入数据目录" << in_dir << "下tif文件格式检查失败！"
			<< std::endl;
		std::cerr << err_os.str() << std::endl;
		exit(EXIT_FAILURE);
	}
	err_os.clear();

	BOOST_LOG_TRIVIAL(info) << "RST算法输入检查结束";

	std::vector<arma::fcube> ydwd_cubes = get_ydwd_cubes(current_year,
		tif_files, years, generate_doys);
	BOOST_LOG_TRIVIAL(info) << "构建原地温度完成..";

	arma::fcube alice_cube = get_alice_cube(ydwd_cubes);
	BOOST_LOG_TRIVIAL(info) << "计算Alice指数结束..";

	std::vector<std::string> output_files =
		get_output_files(out_dir, tif_files_curr_year);
	output_files.resize(Wavelet_utils::get_left_closest_of_pow2(output_files.size()));
	BOOST_LOG_TRIVIAL(info) << "准备将文件输出到..";
	BOOST_LOG_TRIVIAL(debug) << boost::algorithm::join(output_files, "\n");

	std::vector<double> geo_trans;
	std::string proj;

	modis_api::Gdal_operation::
		get_geo_trans_and_proj(tif_files_curr_year.front(), geo_trans, proj);

	save_cube(output_files, alice_cube, geo_trans.data(), proj);
	BOOST_LOG_TRIVIAL(info) << "Alice指数已保存..";
	BOOST_LOG_TRIVIAL(info) << "";
}

/**
 * \brief 执行背景值填充
 * \param start_date 开始日期
 * \param end_date 结束日期
 * \param in_dir 输入数据目录
 * \param out_dir 输出目录
 * \param k 背景值填充k值
 */
void do_bg_interp(const gr::date& start_date, const gr::date& end_date,
	const std::string& in_dir, const std::string& out_dir, const unsigned k)
{
	BOOST_LOG_TRIVIAL(info) << "开始生成背景场文件，输入数据目录："
		<< in_dir << "，输出数据目录：" << out_dir;

	const unsigned current_year = start_date.year();

	std::ostringstream err_os;

	std::vector<std::string> tif_files =
		fo::get_all_files_by_extension(in_dir, ".tif");
	BOOST_LOG_TRIVIAL(debug) << "输入数据目录下共找到：" <<
		tif_files.size() << "个tif文件";

	std::vector<unsigned> years =
		Rst_prep_file_op::get_file_years(tif_files);
	BOOST_LOG_TRIVIAL(debug) << "输入数据目录下共找到[" << in_dir << " : "
		<< boost::algorithm::join(String_utils::conv_vec_str(years), ",")
		<< "]年的数据";
	if (!Rst_checker::check_year(years, err_os))
	{
		std::cerr << err_os.str() << std::endl;
		exit(EXIT_FAILURE);
	}
	err_os.clear();
	const unsigned min_year = *std::min_element(years.begin(), years.end());
	BOOST_LOG_TRIVIAL(debug) << "输入数据目录" << in_dir << "下最早年份为："
		<< min_year;

	std::vector<std::string> generate_doys =
		Rst_prep_file_op::generate_doys(start_date, end_date, min_year);
	BOOST_LOG_TRIVIAL(debug) << "生成的DOY :"
		<< boost::algorithm::join(generate_doys, ",");
	if (!Rst_checker::check_doy_file_ready(tif_files, generate_doys, err_os))
	{
		std::cerr << "根据生成的DOY检查输入数据目录" << in_dir <<
			"下的tif文件，缺少相关tif文件！" << std::endl;
		std::cerr << err_os.str() << std::endl;
		exit(EXIT_FAILURE);
	}
	err_os.clear();

	if (!Rst_checker::check_tif(tif_files, err_os))
	{
		std::cerr << "输入数据目录" << in_dir << "下tif文件格式检查失败！"
			<< std::endl;
		std::cerr << err_os.str() << std::endl;
		exit(EXIT_FAILURE);
	}
	err_os.clear();

	BOOST_LOG_TRIVIAL(info) << "输入检查结束";

	fill_background(current_year, tif_files, generate_doys, out_dir, k);

	std::vector<std::string> bg_tif_files(fo::get_all_files_by_extension(out_dir, ".tif"));
	BOOST_LOG_TRIVIAL(info) << "生成" << bg_tif_files.size()
		<< "个背景场文件，生成结束";
}

int main(int argc, char** argv)
{
	init_logger_setting();
	// 数据目录、开始日期字符串、结束日期字符串、背景值数据目录、输出数据目录
	std::string data_dir, start_date_str, end_date_str,
		bg_tif_dir, output_dir;
	// 背景值填充k值，默认为1 
	unsigned k = 1;
	// 解析上述参数
	handle_inputs(argc, argv, data_dir, start_date_str,
		end_date_str, bg_tif_dir, output_dir, k);

	const gr::date start_date = du::get_date_from_doy_str(start_date_str);
	BOOST_LOG_TRIVIAL(debug) << "开始日期:" << gr::to_iso_string(start_date);

	const gr::date end_date = du::get_date_from_doy_str(end_date_str);
	BOOST_LOG_TRIVIAL(debug) << "结束日期:" << gr::to_iso_string(end_date);

	const unsigned current_year = start_date.year();
	BOOST_LOG_TRIVIAL(debug) << "当前年份:" << current_year;

	std::ostringstream err_os;
	if (!Rst_checker::check_date(start_date, end_date, err_os))
	{
		std::cerr << "错误的日期范围！" << std::endl;
		std::cerr << err_os.str() << std::endl;
		exit(EXIT_FAILURE);
	};

	if (fs::is_empty(bg_tif_dir))
	{
		BOOST_LOG_TRIVIAL(info) << "背景场数据目录下没有文件，程序将生成背景场文件";
		do_bg_interp(start_date, end_date, data_dir, bg_tif_dir, k);
	}

	do_rst_en(start_date, end_date, bg_tif_dir, output_dir);
	BOOST_LOG_TRIVIAL(info) << "完成！";
}

/**
 * \brief 获取当前年份和历史年份列表
 * \param cur_year 起始年份
 * \param min_year 最小年份
 * \return <2017,[2016,...,2004], 2016, [2015,...,2004], ..., 2004, []>
 */
std::map<unsigned, std::vector<unsigned>, Map_key_compare>
get_cur_hist_year_pairs(const unsigned cur_year, const unsigned min_year)
{
	std::map<unsigned, std::vector<unsigned>, Map_key_compare> ret;
	for (unsigned year = cur_year; year >= min_year; --year)
		ret.insert(std::make_pair(year, get_year_ranges_except_start(year, min_year)));
	for (const std::pair<unsigned, std::vector<unsigned>> p : ret)
	{
		std::vector<std::string> svec;
		svec.reserve(p.second.size());
		std::transform(p.second.begin(), p.second.end(), std::back_inserter(svec),
			[](unsigned i) { return std::to_string(i); });
		BOOST_LOG_TRIVIAL(debug) << "年份：" << p.first << "，历史年份："
			<< boost::join(svec, ",");
	}
	return ret;
}

std::vector<unsigned>
get_year_ranges_except_start(const unsigned start, const unsigned end)
{
	std::vector<unsigned> ret;
	if (start == end) return ret;
	for (unsigned i = start - 1; i >= end; --i)
		ret.push_back(i);
	std::vector<std::string> svec;
	std::transform(ret.begin(), ret.end(), std::back_inserter(svec),
		[](unsigned i) { return std::to_string(i); });
	BOOST_LOG_TRIVIAL(debug) << "获取年份范围 (" << start << "," <<
		end << ") = [" << boost::algorithm::join(svec, ",") << "]";
	return ret;
}

/**
 * \brief 加载某年份相关doy的tif至Cube
 * \param all_files 全部tif文件
 * \param all_doys DOY
 * \param year 年份
 * \return Cube
 */
arma::fcube
load_cube_by_year_and_shrink(const std::vector<std::string>& all_files,
	const std::vector<std::string>& all_doys, const unsigned year)
{
	std::vector<std::string> year_files =
		get_year_files(all_files, all_doys, year);
	const size_t shrink_size = Wavelet_utils::
		get_left_closest_of_pow2(year_files.size());
	year_files.resize(shrink_size);

	BOOST_LOG_TRIVIAL(debug) << "缩减后的文件列表：" <<
		boost::algorithm::join(fo::get_file_names(year_files), "\n");

	arma::fcube ret_cube = Cube_utils::load_cube(year_files);
	// Shrink
	Cube_utils::shrink_cube_to_square(ret_cube);

	BOOST_LOG_TRIVIAL(debug) << "Cube(" << ret_cube.n_rows
		<< "," << ret_cube.n_cols << "," << ret_cube.n_slices << ")已加载";
	return ret_cube;
}

/**
 * \brief 获取某年的DOY的文件
 * \param all_files 全部tif文件
 * \param all_doys 全部DOY
 * \param year 年份
 * \return
 */
std::vector<std::string> get_year_files(
	const std::vector<std::string>& all_files,
	const std::vector<std::string>& all_doys,
	const unsigned year)
{
	std::vector<std::string> year_doys =
		Rst_prep_file_op::get_doys_by_year(year, all_doys);
	std::vector<std::string> year_files =
		Rst_prep_file_op::get_files_by_doys(all_files, year_doys);
	return year_files;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

