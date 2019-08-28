#pragma once
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time.hpp>
#include <filesystem>
#include <string>

namespace adsma
{
	/**
	 * \brief ��ȡ���yml�ַ���
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
	 * \brief ��ȡ������䵱ǰ�·ݵ�hdf�б��ļ��ͱ�����hdf�б��ļ����������ļ�����ͨ��һ��ѭ��������
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
	 * \brief ��������yml�ļ���hdflist�ı��ļ�
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

}