#pragma once

#include <boost/date_time/gregorian/greg_date.hpp>
#include <string>
#include <filesystem>

namespace adsma
{
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

	std::string get_plot_eddyfield_ref_yml_str();

	std::string get_plot_eddyfield_ano_yml_str();

}
