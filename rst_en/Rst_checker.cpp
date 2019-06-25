#include "pch.h"
#include "Rst_checker.h"
#include "../modis_api/Date_utils.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Gdal_operation.h"
#include "Rst_prep_file_op.h"
#include <armadillo>
#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

namespace gr = boost::gregorian;
namespace fs = boost::filesystem;

typedef modis_api::Date_utils du;
typedef modis_api::Gdal_operation go;

Rst_checker::Rst_checker() = default;
Rst_checker::~Rst_checker() = default;

/**
 * \brief 最小日期区间
 */
const unsigned MIN_DATE_SPAN = 32;

/**
 * \brief 最小年份约束
 */
const unsigned MIN_YEAR_THRESHOLD = 3;

/**
 * \brief 检查数据目录
 * \param data_dir 数据目录
 * \param os
 * \return
 */
bool Rst_checker::check_data_dir(const std::string& data_dir, std::ostringstream& os)
{
	bool is_data_dir_valid = true;

	if (!fs::exists(data_dir))
	{
		os << "数据目录不存在！" << std::endl;
		is_data_dir_valid = false;
	}

	return is_data_dir_valid;
}

/**
 * \brief 检查起止日期是否合法
 * \param start_date 起始日期
 * \param end_date 中止日期
 * \param os 错误消息输出对象
 * \return
 */
bool Rst_checker::check_date(const boost::gregorian::date& start_date,
	const boost::gregorian::date& end_date, std::ostringstream& os)
{
	bool is_date_valid = true;

	if (start_date.year() != end_date.year())
	{
		os << "起止日期必须为同一年！" << std::endl;
		is_date_valid = false;
	}

	if (start_date >= end_date)
	{
		os << "起始日期必须小于终止日期！" << std::endl;
		is_date_valid = false;
	}

	if (static_cast<unsigned>((end_date - start_date).days()) < MIN_DATE_SPAN)
	{
		os << "起止日期间隔须大于或等于" << MIN_DATE_SPAN << "天" << std::endl;
		is_date_valid = false;
	}

	return is_date_valid;
}

/**
 * \brief 检查文件年份是否符合要求
 * \param all_years 全部年份
 * \param os
 * \return
 */
bool Rst_checker::check_year(const std::vector<unsigned>& all_years,
	std::ostringstream& os)
{
	bool is_year_valid = true;

	if (all_years.size() < MIN_YEAR_THRESHOLD)
	{
		os << "至少需要" << MIN_YEAR_THRESHOLD << "年的数据！目前只有" <<
			all_years.size() << "年的数据！";
		is_year_valid = false;
	}

	return is_year_valid;
}

/**
 * \brief 检查tif文件是否存在
 * \param all_files
 * \param os
 * \return
 */
bool Rst_checker::check_file_exists(const std::vector<std::string>& all_files,
	std::ostringstream& os)
{
	bool file_exists = true;
	if (all_files.empty())
	{
		os << "No tif file found!" << std::endl;
		file_exists = false;
	}
	// 检查tif文件是否存在
	std::vector<std::string> files_not_exist;
	std::for_each(all_files.begin(), all_files.end(),
		[&files_not_exist](const std::string& f)
	{
		if (!fs::exists(f)) files_not_exist.push_back(f);
	});
	if (!files_not_exist.empty())
	{
		os << "下列文件不存在： " << std::endl;
		std::for_each(files_not_exist.begin(), files_not_exist.end(),
			[&os](const std::string& f) { os << "\t" << f << std::endl; });
		file_exists = false;
	}
	return file_exists;
}

/**
 * \brief 根据doy检查相关tif文件是否存在
 * \param all_files 全部文件
 * \param required_doys doys
 * \param os
 * \return
 */
bool Rst_checker::check_doy_file_ready(const std::vector<std::string>& all_files,
	const  std::vector<std::string>& required_doys,
	std::ostringstream& os)
{
	bool doy_file_ready = true;

	std::vector<std::string> doys_no_file_exist;
	for (const std::string& required_doy : required_doys)
	{
		auto p = std::find_if(all_files.begin(), all_files.end(),
			[&required_doy](const std::string& f)
		{
			return f.find(required_doy) != std::string::npos;
		});
		if (p == all_files.end())
			doys_no_file_exist.push_back(required_doy);
	}

	if (!doys_no_file_exist.empty())
	{
		os << "下载DOY不存在相关tif文件:" <<
			boost::algorithm::join(doys_no_file_exist, "\n") << std::endl;
		doy_file_ready = false;
	}

	return doy_file_ready;
}

/**
 * \brief 检查tif文件是否合法，检查所有tif文件的大小是否一致，是否全为零
 * \param all_files 全部文件的路径
 * \param os 用于输入错误信息的ostringstream
 * \return 全部tif文件合法返回true，存在不合法的tif文件返回false
 */
bool Rst_checker::check_tif(const std::vector<std::string>& all_files
	, std::ostringstream& os)
{
	const std::string first_tif_path = all_files.front();
	const arma::fmat my_mat = *go::read_tif_to_fmat(first_tif_path);
	const arma::uword n_row = my_mat.n_rows, n_col = my_mat.n_cols;

	std::vector<std::string> tifs_wrong_size;
	std::vector<std::string> tifs_all_zero;

	for (const std::string& tif_file : all_files)
	{
		const arma::fmat mat = *go::read_tif_to_fmat(tif_file);
		if (mat.n_rows != n_row || mat.n_cols != n_col)
			tifs_wrong_size.push_back(tif_file);
		if (all(vectorise(mat) == 0))
			tifs_all_zero.push_back(tif_file);
	}

	if (!tifs_wrong_size.empty())
		os << "tif文件大小不同！" << std::endl;
	if (!tifs_all_zero.empty())
	{
		os << "下列tif文件全零！" << std::endl;
		std::for_each(tifs_all_zero.begin(), tifs_all_zero.end(),
			[&os](const std::string& f) {os << "\t" << f << std::endl; });
	}
	return tifs_wrong_size.empty() && tifs_all_zero.empty();
}