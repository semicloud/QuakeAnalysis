#include "..//modis_api//Gdal_operation.h"
#include "..//modis_api//Mat_operation.h"
#include "Eddy_ref.h"
#include "Eddy_field.h"
#include <armadillo>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>
#include <string>
#include <filesystem>
#include <vector>

/**
 * \brief 获取用于生成背景场的Tif文件路径列表，该函数后期可移动到adasm.exe中
 * \brief 目前，通过Eddy_field_options_yaml类的get_ref_tif_file_list成员
 * \brief 获取计算背景场的tif文件列表
 * \return
 */
std::vector<std::string> Eddy_ref::get_tif_file_list_for_ref(const Eddy_field_options_yaml& options, const std::string& type)
{
	throw std::exception("should not call this function..");

	//const auto data_folder = options.get_tif_directory(type);
	//BOOST_LOG_TRIVIAL(debug) << "数据文件检索路径：" << data_folder;

	//if (!fs::exists(data_folder))
	//{
	//	BOOST_LOG_TRIVIAL(error) << "不存在的预处理数据文件夹：" << data_folder;
	//	return std::vector<std::string>();
	//}

	//if (!fs::is_directory(data_folder))
	//{
	//	BOOST_LOG_TRIVIAL(error) << data_folder << "不是合法的目录！";
	//	return std::vector<std::string>();
	//}

	//std::vector<std::string> ans;

	//for (fs::recursive_directory_iterator it(data_folder); it != fs::recursive_directory_iterator(); ++it)
	//{
	//	if (!is_regular_file(*it)) continue;

	//	auto file_name = it->path().filename().string();
	//	// bt_2018100.tif，由中间的_将字符串分为两部分，然后提取第二部分的前4个字符作为年份
	//	std::vector<std::string> parts;
	//	split(parts, file_name, boost::is_any_of("_"));
	//	const auto file_year = boost::lexical_cast<int>(parts[1].substr(0, 4));
	//	const auto options_year = boost::lexical_cast<int>(options.year());
	//	auto file_day = parts[1].substr(4, 3); // day按照字符串进行比较
	//	//spdlog::debug("文件{}为{}年{}日的观测数据", it->path().string(), file_year, file_day);
	//	// 筛选文件
	//	if (file_day == options.day() && file_year <= options_year)
	//	{
	//		ans.push_back(it->path().string());
	//	}
	//}

	//if (!ans.empty())
	//{
	//	BOOST_LOG_TRIVIAL(info) << "在预处理数据文件夹" << data_folder << "下共找到" << ans.size() << "个第" << options.day() << "天的数据文件";
	//	return ans;
	//}

	//BOOST_LOG_TRIVIAL(warning) << "未在预处理数据文件夹" << data_folder << "中找到第" << options.day() << "天的数据！";
	//return std::vector<std::string>();
}

Eddy_ref::Eddy_ref()
{
}


Eddy_ref::~Eddy_ref()
{
}

void Eddy_ref::compute_eddy_ref_m1(Eddy_field_options_yaml& options)
{
	using namespace std;
	using namespace std::filesystem;

	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "============使用方法1生成背景场===============";
	//auto file_path_list = get_tif_file_list_for_ref(options, type);
	const vector<path>  tif_files_for_ref_computing = options.get_tif_files_for_ref_computing();
	if (tif_files_for_ref_computing.empty())
	{
		BOOST_LOG_TRIVIAL(info) << "未找到数据，无法生成背景场！";
		BOOST_LOG_TRIVIAL(info) << "===============使用方法1生成背景场结束===========";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}
	std::vector<arma::fmat> mat_list;
	for (const auto& file_path : tif_files_for_ref_computing)
	{
		auto optional_fmat = modis_api::Gdal_operation::read_tif_to_fmat(file_path.string());
		if (optional_fmat)
			mat_list.push_back(*optional_fmat);
		else
			BOOST_LOG_TRIVIAL(error) << "读取dn值失败，跳过" << file_path << "的处理";
	}

	auto mean_mat = modis_api::Mat_operation::mean_mat_by_each_pixel(mat_list, 0);
	if (!mean_mat)
	{
		BOOST_LOG_TRIVIAL(error) << "矩阵大小不同，无法生成背景场！";
		BOOST_LOG_TRIVIAL(info) << "===============使用方法1生成背景场结束===========";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}

	// 写回tif文件

	/*
	 * copy_file with copy_options is dangerous!
	 * see: https://stackoverflow.com/questions/829468/how-to-perform-boostfilesystem-copy-file-with-overwrite
	 * Beware of boost::copy_file with copy_option::overwrite_if_exists!
	 * If the destination file exists and it is smaller than the source, the function will only overwrite the first size(from_file) bytes in the target file.
	 */
	if (exists(options.ref_image_file()))
		remove(options.ref_image_file());
	// 父目录不存在则创建之
	if (!exists(options.ref_image_file().parent_path()))
		create_directories(options.ref_image_file().parent_path());
	// 然后复制过去
	copy_file(tif_files_for_ref_computing.front(), options.ref_image_file());
	// 写入数据
	BOOST_LOG_TRIVIAL(debug) << "COPY " << tif_files_for_ref_computing.front() << " to " << options.ref_image_file();
	modis_api::Gdal_operation::write_fmat_to_tif(options.ref_image_file().string(), *mean_mat);

	BOOST_LOG_TRIVIAL(info) << "生成的背景场文件为：" << options.ref_image_file();
	BOOST_LOG_TRIVIAL(info) << "===============使用方法1生成背景场结束===========";
	BOOST_LOG_TRIVIAL(info) << "";
}

void Eddy_ref::compute_eddy_ref_m2(Eddy_field_options_yaml& options)
{
	using namespace  std;
	using namespace filesystem;
	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "============使用方法2生成背景场===============";
	//auto file_path_list = get_tif_file_list_for_ref(options, type);
	const vector<path> tif_files_for_ref_computing = options.get_tif_files_for_ref_computing();
	if (tif_files_for_ref_computing.empty())
	{
		BOOST_LOG_TRIVIAL(warning) << "未找到数据，无法生成背景场！";
		BOOST_LOG_TRIVIAL(info) << "===============使用方法2生成背景场结束===========";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}
	vector<arma::fmat> mat_list;
	for (auto file_path : tif_files_for_ref_computing)
	{
		auto optional_fmat = modis_api::Gdal_operation::read_tif_to_fmat(file_path.string());
		if (optional_fmat)
			mat_list.push_back(*optional_fmat);
		else
			BOOST_LOG_TRIVIAL(error) << "读取dn值失败，跳过" << file_path << "的处理";
	}

	// 涡度矩阵列表
	std::vector<arma::fmat> ef_mat_list;
	for (arma::Mat<float>& f_mat : mat_list)
	{
		auto ef_mat = Eddy_field::get_eddy_field_4(f_mat, 0);
		if (!ef_mat)
		{
			BOOST_LOG_TRIVIAL(error) << "求涡度矩阵时出现错误，跳过该矩阵的处理..";
			continue;
		}
		ef_mat_list.push_back(*ef_mat);
	}

	auto ef_mat_list_mean = modis_api::Mat_operation::mean_mat_by_each_pixel(ef_mat_list, 0);
	if (!ef_mat_list_mean)
	{
		BOOST_LOG_TRIVIAL(error) << "涡度矩阵大小不同，无法生成背景场！";
		BOOST_LOG_TRIVIAL(info) << "===============使用方法2生成背景场结束===========";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}

	BOOST_LOG_TRIVIAL(info) << "涡度矩阵的平均值计算完毕..";

	// 写回tif文件
	const path source_data_set = tif_files_for_ref_computing.front();
	const path dest_data_set = options.ref_image_file();
	BOOST_LOG_TRIVIAL(debug) << "目标数据文件：" << dest_data_set;

	if (exists(dest_data_set))
		remove(dest_data_set);
	// 父目录不存在则创建之
	if (!exists(dest_data_set.parent_path()))
		create_directories(dest_data_set.parent_path());
	// 然后复制过去
	copy_file(source_data_set, dest_data_set);
	// 写入数据
	BOOST_LOG_TRIVIAL(debug) << "COPY " << source_data_set << " to " << dest_data_set;
	modis_api::Gdal_operation::write_fmat_to_tif(dest_data_set.string(), *ef_mat_list_mean);

	BOOST_LOG_TRIVIAL(info) << "生成的背景场文件为：" << dest_data_set;
	BOOST_LOG_TRIVIAL(info) << "===============使用方法2生成背景场结束===========";
	BOOST_LOG_TRIVIAL(info) << "";
}
