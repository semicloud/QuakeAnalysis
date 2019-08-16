#pragma once

#include <boost/date_time/gregorian/greg_date.hpp>
#include <string>
#include <filesystem>

namespace adsma
{
	const std::string PLOT_TITLE_SIZE = "16";
	const std::string BAR_TITLE_SIZE = "12";
	const std::string REF_BAR_NAME = "nipy_spectral";

	/**
	 * \brief �����ж�Yml�ַ���
	 * \param workspace_path MODIS�����ռ�Ŀ¼
	 * \param tmp_path
	 * \param calc_ref �Ƿ���㱳����
	 * \param calc_ano �Ƿ�����ж�
	 * \param ano_method �������1����2
	 * \param input_image_file_path ����Tif�ļ�����Ҫ�����жȼ����Tif�ļ�
	 * \param ref_list_file_path �������ɱ�������Tif�б��ļ�
	 * \param ref_image_file_path �������ļ�·����������ɱ������������ɵı��������浽����ļ�����������ɱ����������ø��ļ���Ϊ������
	 * \param output_ano_file_path ���ɵ��ж��ļ�·��
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
	 * \brief ��ȡ�жȱ�����hdflist�ַ���
	 * \param workspace_path
	 * \param tmp_path
	 * \param product
	 * \param product_type
	 * \param ano_method
	 * \param date
	 * \param years ���������жȱ���������ݣ���date�����֮ǰ�����
	 * \return �жȱ�����hdflist�ַ���
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

	/**
	 * \brief �����ж�Yml�ļ��Լ�������������Ҫ��HdfList�ļ�
	 * \param workspace_path
	 * \param tmp_path
	 * \param product_name
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
		const std::string& product_name,
		const std::string& product_type,
		const boost::gregorian::date& start_date,
		const boost::gregorian::date& end_date,
		bool calc_ref, bool calc_ano, int ano_method,
		const std::filesystem::path& yml_folder_path
	);

	/**
	 * \brief �����жȱ�������ͼyml�ļ�
	 * \param workspace_path 
	 * \param tmp_path 
	 * \param product_name 
	 * \param product_type 
	 * \param start_date 
	 * \param end_date 
	 * \param calc_ref 
	 * \param calc_ano 
	 * \param ano_method 
	 * \param fig_extent 
	 * \param shp_bound_path 
	 * \param shp_fault_path 
	 * \param shp_city_path 
	 * \param quake_record_path 
	 * \param yml_folder_path 
	 * \return 
	 */
	int generate_plot_eddyfield_ref_yml_files(
		const std::filesystem::path& workspace_path,
		const std::filesystem::path& tmp_path,
		const std::string& product_name,
		const std::string& product_type,
		const boost::gregorian::date& start_date,
		const boost::gregorian::date& end_date,
		bool calc_ref, bool calc_ano, int ano_method,
		const std::string& fig_extent,
		const std::filesystem::path& shp_bound_path,
		const std::filesystem::path& shp_fault_path,
		const std::filesystem::path& shp_city_path,
		const std::filesystem::path& quake_record_path,
		const std::filesystem::path& yml_folder_path);

	/**
	 * \brief ��ȡ�жȳ�ͼyml�ַ���
	 * \param input_file_path 
	 * \param output_file_path 
	 * \param fig_title 
	 * \param fig_bar_title 
	 * \param fig_extent 
	 * \param shp_bound_path 
	 * \param shp_fault_path 
	 * \param shp_city_path 
	 * \param quake_record_path 
	 * \return 
	 */
	std::string get_plot_eddyfield_yml_str(
		const std::filesystem::path& input_file_path,
		const std::filesystem::path& output_file_path,
		const std::string& fig_title,
		const std::string& fig_bar_title,
		const std::string& fig_extent,
		const std::filesystem::path& shp_bound_path,
		const std::filesystem::path& shp_fault_path,
		const std::filesystem::path& shp_city_path,
		const std::filesystem::path& quake_record_path
	);

	/**
	 * \brief �����ж��쳣��ͼyml�ļ�
	 * \param workspace_path 
	 * \param tmp_path 
	 * \param product_name 
	 * \param product_type 
	 * \param start_date 
	 * \param end_date 
	 * \param calc_ref 
	 * \param calc_ano 
	 * \param ano_method 
	 * \param yml_folder_path 
	 * \return 
	 */
	int generate_plot_eddyfield_ano_yml_files(
		const std::filesystem::path& workspace_path,
		const std::filesystem::path& tmp_path,
		const std::string& product_name,
		const std::string& product_type,
		const boost::gregorian::date& start_date,
		const boost::gregorian::date& end_date,
		bool calc_ref, bool calc_ano, int ano_method,
		const std::string& fig_extent,
		const std::filesystem::path& shp_bound_path,
		const std::filesystem::path& shp_fault_path,
		const std::filesystem::path& shp_city_path,
		const std::filesystem::path& quake_record_path,
		const std::filesystem::path& yml_folder_path);



}
