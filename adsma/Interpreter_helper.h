#pragma once

#include <boost/format.hpp>
#include <string>
#include <filesystem>

namespace adsma
{
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
	 * \brief 将unordered map转换为yml str
	 * \param umap
	 * \return
	 */
	std::string get_yml_str(const std::unordered_map<std::string, std::string>& umap);

	/**
	 * \brief 相关待预处理数据是否已存在
	 * \param workspace 工作空间目录
	 * \param product 产品，要求使用产品全称，如MOD11A1
	 * \param year_and_day 年份及天数字符串
	 * \return 数据存在返回true，不存在返回false
	 */
	bool is_data_exist(const std::filesystem::path& workspace,
		const std::string& product, const std::string& year_and_day);

	/**
	 * \brief 获取一个产品的预处理数据年份集合
	 * \param workspace_path 工作空间目录
	 * \param product_type 产品类型，MOD或MYD
	 * \param product 产品名称，限于[BT, AOD, WV, LST]
	 * \return 该产品的预处理数据年份集合，升序排序
	 */
	std::vector<unsigned int> get_years_of_product(
		const std::filesystem::path& workspace_path,
		const std::string& product_type,
		const std::string& product
	);
}

