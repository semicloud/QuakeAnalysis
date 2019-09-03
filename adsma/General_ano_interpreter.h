#pragma once
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time.hpp>
#include <filesystem>
#include <string>

namespace adsma
{
	/**
	 * \brief 获取年变yml字符串
	 * \param month_list_file
	 * \param ref_list_file
	 * \param output_image_file
	 * \param tmp_path
	 * \return yml_str
	 */
	std::string get_general_ano_yml_str(
		const std::filesystem::path& month_list_file,
		const std::filesystem::path& ref_list_file,
		const std::filesystem::path& output_image_file,
		const std::filesystem::path& tmp_path);

	/**
	 * \brief 获取计算年变当前月份的hdf列表文件和背景场hdf列表文件，这两个文件可以通过一个循环作出来
	 * \param workspace_path
	 * \param tmp_path
	 * \param product_name
	 * \param product_type
	 * \param year
	 * \param month
	 * \param month_hdflist_str
	 * \param ref_hdflist_str
	 * \return
	 */
	int get_general_ano_month_and_ref_hdflist_str(
		const std::filesystem::path& workspace_path,
		const std::filesystem::path& tmp_path,
		const std::string & product_name,
		const std::string & product_type,
		unsigned year, unsigned month,
		std::string& month_hdflist_str, std::string& ref_hdflist_str);

	/**
	 * \brief 生成年变的yml文件和hdflist文本文件
	 * \param workspace_path
	 * \param tmp_path
	 * \param product_name
	 * \param product_type
	 * \param start_date
	 * \param end_date
	 * \param yml_folder_path
	 * \return
	 */
	int generate_general_ano_yml_hdflist_files(
		const std::filesystem::path& workspace_path,
		const std::filesystem::path& tmp_path,
		const std::string & product_name,
		const std::string & product_type,
		const boost::gregorian::date& start_date,
		const boost::gregorian::date& end_date,
		const std::filesystem::path& yml_folder_path
	);

	/**
	 * \brief 生成年变背景场绘制yml文件
	 * \param workspace_path 工作空间路径
	 * \param tmp_path tmp目录
	 * \param product_name 产品名称
	 * \param product_type 产品类型
	 * \param start_date 开始日期
	 * \param end_date 结束日期
	 * \param fig_extent 图件范围
	 * \param shp_bound_path 边界shpfile
	 * \param shp_fault_path 断层shpfile
	 * \param shp_city_path 城市shpfile
	 * \param quake_record_path 地震目录文件路径
	 * \param yml_folder_path 存放yml文件的路径
	 * \return
	 */
	int generate_plot_general_ref_yml_files(
		const std::filesystem::path& workspace_path,
		const std::filesystem::path& tmp_path,
		const std::string& product_name,
		const std::string& product_type,
		const boost::gregorian::date& start_date,
		const boost::gregorian::date& end_date,
		const std::string& fig_extent,
		const std::filesystem::path& shp_bound_path,
		const std::filesystem::path& shp_fault_path,
		const std::filesystem::path& shp_city_path,
		const std::filesystem::path& quake_record_path,
		const std::filesystem::path& yml_folder_path);


	/**
	 * \brief 生成年变异常绘制yml文件
	 * \param workspace_path 工作空间路径
	 * \param tmp_path tmp目录
	 * \param product_name 产品名称
	 * \param product_type 产品类型
	 * \param start_date 开始日期
	 * \param end_date 结束日期
	 * \param fig_extent 图件范围
	 * \param shp_bound_path 边界shpfile
	 * \param shp_fault_path 断层shpfile
	 * \param shp_city_path 城市shpfile
	 * \param quake_record_path 地震目录文件路径
	 * \param yml_folder_path 存放yml文件的路径
	 * \return
	 */
	int generate_plot_general_ano_yml_files(
		const std::filesystem::path& workspace_path,
		const std::filesystem::path& tmp_path,
		const std::string& product_name,
		const std::string& product_type,
		const boost::gregorian::date& start_date,
		const boost::gregorian::date& end_date,
		const std::string& fig_extent,
		const std::filesystem::path& shp_bound_path,
		const std::filesystem::path& shp_fault_path,
		const std::filesystem::path& shp_city_path,
		const std::filesystem::path& quake_record_path,
		const std::filesystem::path& yml_folder_path);

	/**
	 * \brief 获取年变异常出图yml字符串
	 * \param ano_tif_path 年变tif文件路径
	 * \param ano_fig_path 年变异常主题图输出路径
	 * \param fig_title 图件标题
	 * \param bar_title Bar标题
	 * \param fig_extent 图件展示地理范围
	 * \param shp_bound_path 边界shpfile
	 * \param shp_fault_path 断层shpfile
	 * \param shp_city_path 城市shpfile
	 * \param quake_record_path 地震目录文件路径
	 * \return 
	 */
	std::string get_plot_generala_ano_yml_str(
		std::filesystem::path const & ano_tif_path,
		std::filesystem::path const & ano_fig_path,
		std::string const & fig_title,
		std::string const & bar_title,
		std::string const & fig_extent,
		std::filesystem::path const & shp_bound_path,
		std::filesystem::path const & shp_fault_path,
		std::filesystem::path const & shp_city_path,
		std::filesystem::path const & quake_record_path);

}