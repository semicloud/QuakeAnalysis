#pragma once
#include <yaml-cpp/yaml.h>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <map>
#include <optional>
#include <string>
#include <filesystem>

std::optional<YAML::Node> load_yml(const std::string& yml_path_str);

/**
 * \brief 从Node中解析出开始日期和结束日期
 * \param node
 * \return
 */
std::vector<boost::gregorian::date> parse_date(const YAML::Node& node);

int process(const std::string& yml_path_str);

/**
 * \brief 准备地表温度的预处理，生成yml文件和hdflist文件
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
 * \param output_pixel_size 
 * \param yml_folder_path 
 * \return 
 */
int prepare_lst(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const boost::gregorian::date& date_start,
	const boost::gregorian::date& date_end,
	const std::string& product_type,
	float pp_min_lon, float pp_max_lon, float pp_min_lat, float pp_max_lat,
	const std::string& resampling_type,
	const std::string& output_projection_type,
	const std::string& output_projection_parameters,
	const float output_pixel_size,
	const std::filesystem::path& yml_folder_path
);

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
int prepare_aod_or_wv(
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
 * \brief 准备亮温预处理，生成yml文件和hdflist文件
 * \param workspace_path
 * \param tmp_path
 * \param date_start
 * \param date_end
 * \param product_type
 * \param pp_min_lon
 * \param pp_max_lon
 * \param pp_min_lat
 * \param pp_max_lat
 * \param band
 * \param mrt_kernel_type
 * \param mrt_projection_type
 * \param mrt_projection_args
 * \param mrt_pixel_size
 * \param yml_folder_path
 * \return
 */
int prepare_bt(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const boost::gregorian::date& date_start,
	const boost::gregorian::date& date_end,
	const std::string& product_type,
	float pp_min_lon, float pp_max_lon, float pp_min_lat, float pp_max_lat,
	int band,
	const std::string& mrt_kernel_type,
	const std::string& mrt_projection_type,
	const std::string& mrt_projection_args,
	float mrt_pixel_size,
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
std::string generate_preprocess_aod_wv_hdf_list_str(
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
std::string generate_preprocess_aod_wv_yml_str(
	const std::filesystem::path& hdf_list_file_path,
	const std::filesystem::path& tmp_path,
	float min_lon, float max_lon,
	float min_lat, float max_lat,
	const std::string& resampling_type,
	const std::string& output_projection_type,
	const std::string& output_projection_parameters,
	const std::filesystem::path& output_image_path
);

/**
 * \brief 生成地表温度预处理HDF文件列表字符串
 * \param workspace_path 
 * \param product_type 
 * \param date 
 * \return 
 */
std::string generate_preprocess_lst_hdf_list_str(
	const std::filesystem::path& workspace_path,
	const std::string& product_type, const boost::gregorian::date& date
);

/**
 * \brief 生成地表温度的yml
 * \param hdf_list_file_path 
 * \param tmp_path 
 * \param min_lon 
 * \param max_lon 
 * \param min_lat 
 * \param max_lat 
 * \param resampling_type 
 * \param output_projection_type 
 * \param output_projection_parameters 
 * \param output_pixel_size 
 * \param output_image_path 
 * \return 
 */
std::string generate_preprocess_lst_yml_str(
	const std::filesystem::path& hdf_list_file_path,
	const std::filesystem::path& tmp_path,
	float min_lon, float max_lon,
	float min_lat, float max_lat,
	const std::string& resampling_type,
	const std::string& output_projection_type,
	const std::string& output_projection_parameters,
	float output_pixel_size,
	const std::filesystem::path& output_image_path
);

/**
 * \brief 生成亮温预处理HDF文件列表字符串
 * \param workspace_path 工作空间
 * \param product_type 产品类别，MOD或MYD
 * \param date 日期
 * \return 亮温预处理HDF文件列表字符串
 */
std::string generate_preprocess_bt_hdf_list_str(
	const std::filesystem::path& workspace_path,
	const std::string& product_type, const boost::gregorian::date& date);

/**
 * \brief 生成亮温预处理Yml
 * \param hdf_list_file_path HDF列表文件路径
 * \param tmp_path tmp目录路径
 * \param min_lon
 * \param max_lon
 * \param min_lat
 * \param max_lat
 * \param band 波段
 * \param mrt_kernel_type
 * \param mrt_projection_type
 * \param mrt_projection_args
 * \param mrt_pixel_size
 * \param output_image_path
 * \return
 */
std::string generate_preprocess_bt_yml_str(
	const std::filesystem::path& hdf_list_file_path,
	const std::filesystem::path& tmp_path,
	float min_lon, float max_lon,
	float min_lat, float max_lat,
	int band,
	const std::string& mrt_kernel_type,
	const std::string& mrt_projection_type,
	const std::string& mrt_projection_args,
	float mrt_pixel_size,
	const std::filesystem::path& output_image_path
);

/**
 * \brief 解析最小、最大经纬度
 * \param lon_lat_str 经纬度字符串
 * \param out_min_lon 最小经度，输出
 * \param out_max_lon 最大经度，输出
 * \param out_min_lat 最小纬度，输出
 * \param out_max_lat 最大纬度，输出
 * \return
 */
int split_lonlat_str(const std::string& lon_lat_str, float& out_min_lon,
	float& out_max_lon, float& out_min_lat, float& out_max_lat);

/**
 * \brief 将unordered map翻译为yml str
 * \param umap 
 * \return 
 */
std::string get_yml_str(const std::unordered_map<std::string, std::string>& umap);
