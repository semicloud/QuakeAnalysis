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

	/**
	 * \brief ������䱳��������yml�ļ�
	 * \param workspace_path �����ռ�·��
	 * \param tmp_path tmpĿ¼
	 * \param product_name ��Ʒ����
	 * \param product_type ��Ʒ����
	 * \param start_date ��ʼ����
	 * \param end_date ��������
	 * \param fig_extent ͼ����Χ
	 * \param shp_bound_path �߽�shpfile
	 * \param shp_fault_path �ϲ�shpfile
	 * \param shp_city_path ����shpfile
	 * \param quake_record_path ����Ŀ¼�ļ�·��
	 * \param yml_folder_path ���yml�ļ���·��
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
	 * \brief ��������쳣����yml�ļ�
	 * \param workspace_path �����ռ�·��
	 * \param tmp_path tmpĿ¼
	 * \param product_name ��Ʒ����
	 * \param product_type ��Ʒ����
	 * \param start_date ��ʼ����
	 * \param end_date ��������
	 * \param fig_extent ͼ����Χ
	 * \param shp_bound_path �߽�shpfile
	 * \param shp_fault_path �ϲ�shpfile
	 * \param shp_city_path ����shpfile
	 * \param quake_record_path ����Ŀ¼�ļ�·��
	 * \param yml_folder_path ���yml�ļ���·��
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
	 * \brief ��ȡ����쳣��ͼyml�ַ���
	 * \param ano_tif_path ���tif�ļ�·��
	 * \param ano_fig_path ����쳣����ͼ���·��
	 * \param fig_title ͼ������
	 * \param bar_title Bar����
	 * \param fig_extent ͼ��չʾ����Χ
	 * \param shp_bound_path �߽�shpfile
	 * \param shp_fault_path �ϲ�shpfile
	 * \param shp_city_path ����shpfile
	 * \param quake_record_path ����Ŀ¼�ļ�·��
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