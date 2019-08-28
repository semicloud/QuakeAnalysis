#pragma once

#include <boost/format.hpp>
#include <string>


namespace adsma
{
	/**
	 * \brief 获取预处理结果输出目录（部分）
	 * \param product_type 产品类型，MOD或MYD
	 * \param product_name 产品名称，BT或MV或AOD或LST
	 * \return 预处理结果输出目录（部分）
	 */
	inline std::string get_pp_folder(const std::string& product_type, const std::string& product_name)
	{
		return (boost::format("%1%_%2%") % product_type % product_name).str();
	}

	/**
	 * \brief 获取预处理输出文件名
	 * \param product_name 产品名称
	 * \param year 年份
	 * \param day DOY
	 * \return 预处理输出文件名
	 */
	inline std::string get_pp_output_file_name(const std::string& product_name, const std::string& year, const std::string& day)
	{
		return (boost::format("%1%_%2%_%3%.tif") % product_name % year % day).str();
	}
}