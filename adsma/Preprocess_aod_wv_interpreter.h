#pragma once

#include <boost/date_time.hpp>
#include <filesystem>

namespace adsma
{
	/**
	 * \brief 准备云掩膜（AOD）和水汽（WV）的预处理，生成yml文件和hdflist文件
	 * \param workspace_path
	 * \param tmp_path
	 * \param date_start
	 * \param date_end
	 * \param m_product
	 * \param pp_min_lon
	 * \param pp_max_lon
	 * \param pp_min_lat
	 * \param pp_max_lat
	 * \param resampling_type
	 * \param output_projection_type
	 * \param output_projection_parameters
	 * \param yml_folder_path
	 * \return
	 */
	int generate_pp_aod_or_wv_yml_hdflist_files(
		const std::filesystem::path& workspace_path,
		const std::filesystem::path& tmp_path,
		const boost::gregorian::date& date_start,
		const boost::gregorian::date& date_end,
		const std::string& m_product,
		float pp_min_lon, float pp_max_lon, float pp_min_lat, float pp_max_lat,
		const std::string& resampling_type,
		const std::string& output_projection_type,
		const std::string& output_projection_parameters,
		const std::filesystem::path& yml_folder_path
	);

	/**
	 * \brief 生成云掩膜和水汽的预处理HDF文件列表
	 * \param workspace_path 工作空间
	 * \param product_code 产品代码，只能是04或05
	 * \param product_type 产品类别，只能是MOD或MYD
	 * \param date
	 * \return
	 */
	std::string get_preprocess_aod_wv_hdf_list_str(
		const std::filesystem::path& workspace_path,
		const std::string& product_code,
		const std::string& product_type,
		const boost::gregorian::date& date);

	/**
	 * \brief 生成云掩膜和水汽的预处理Yml
	 * \param hdf_list_file_path
	 * \param tmp_path
	 * \param min_lon
	 * \param max_lon
	 * \param min_lat
	 * \param max_lat
	 * \param resampling_type
	 * \param output_projection_type
	 * \param output_projection_parameters
	 * \param output_image_path
	 * \return
	 */
	std::string get_preprocess_aod_wv_yml_str(
		const std::filesystem::path& hdf_list_file_path,
		const std::filesystem::path& tmp_path,
		float min_lon, float max_lon,
		float min_lat, float max_lat,
		const std::string& resampling_type,
		const std::string& output_projection_type,
		const std::string& output_projection_parameters,
		const std::filesystem::path& output_image_path
	);
}

