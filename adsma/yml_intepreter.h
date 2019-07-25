#pragma once
#include <yaml-cpp/yaml.h>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/format.hpp>
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

#pragma region 涡度相关

/**
 * \brief 生成涡度Yml文件以及背景场计算需要的HdfList文件
 * \param workspace_path
 * \param tmp_path
 * \param product
 * \param product_type
 * \param start_date
 * \param end_date
 * \param calc_ref
 * \param calc_ano
 * \param ano_method
 * \param yml_folder_path
 * \return
 */
int generate_eddyfield_yml_hdflist_files(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product,
	const std::string& product_type,
	const boost::gregorian::date& start_date,
	const boost::gregorian::date& end_date,
	bool calc_ref, bool calc_ano, int ano_method,
	const std::filesystem::path& yml_folder_path
);

/**
 * \brief 生成涡度Yml字符串
 * \param workspace_path MODIS工作空间目录
 * \param tmp_path
 * \param calc_ref 是否计算背景场
 * \param calc_ano 是否计算涡度
 * \param ano_method 方法类别，1或者2
 * \param input_image_file_path 输入Tif文件，即要进行涡度计算的Tif文件
 * \param ref_list_file_path 用于生成背景场的Tif列表文件
 * \param ref_image_file_path 背景场文件路径，如果生成背景场，则将生成的背景场保存到这个文件，如果不生成背景场，则用该文件作为背景场
 * \param output_ano_file_path 生成的涡度文件路径
 * \return
 */
std::string get_eddyfield_yml_str(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	bool calc_ref, bool calc_ano, int ano_method,
	const std::filesystem::path& input_image_file_path,
	const std::filesystem::path& ref_list_file_path,
	const std::filesystem::path& ref_image_file_path,
	const std::filesystem::path& output_ano_file_path
);

/**
 * \brief 获取涡度背景场hdflist字符串
 * \param workspace_path
 * \param tmp_path
 * \param product
 * \param product_type
 * \param ano_method
 * \param date
 * \param years 用于生成涡度背景场的年份，即date的年份之前的年份
 * \return 涡度背景场hdflist字符串
 */
std::string get_eddyfield_ref_hdflist_str(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product,
	const std::string& product_type,
	const boost::gregorian::date& date,
	const std::vector<unsigned>& years,
	int ano_method
);

std::string get_plot_eddyfield_ref_yml_str();

std::string get_plot_eddyfield_ano_yml_str();

#pragma endregion 涡度相关

