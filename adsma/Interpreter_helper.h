#pragma once

#include <boost/format.hpp>
#include <string>
#include <filesystem>

namespace adsma::interpreter::helper
{
	/**
	 * \brief ������С�����γ��
	 * \param lon_lat_str ��γ���ַ���
	 * \param out_min_lon ��С���ȣ����
	 * \param out_max_lon ��󾭶ȣ����
	 * \param out_min_lat ��Сγ�ȣ����
	 * \param out_max_lat ���γ�ȣ����
	 * \return
	 */
	int split_lonlat_str(const std::string& lon_lat_str, float& out_min_lon,
		float& out_max_lon, float& out_min_lat, float& out_max_lat);

	/**
	 * \brief ��unordered map����Ϊyml str
	 * \param umap
	 * \return
	 */
	std::string get_yml_str(const std::unordered_map<std::string, std::string>& umap);

	/**
	 * \brief ��ش�Ԥ���������Ƿ��Ѵ���
	 * \param workspace �����ռ�Ŀ¼
	 * \param product ��Ʒ��Ҫ��ʹ�ò�Ʒȫ�ƣ���MOD11A1
	 * \param year_and_day ��ݼ������ַ���
	 * \return ���ݴ��ڷ���true�������ڷ���false
	 */
	bool is_data_exist(const std::filesystem::path& workspace,
		const std::string& product, const std::string& year_and_day);

	/**
	 * \brief ��ȡһ����Ʒ��Ԥ����������ݼ���
	 * \param workspace_path �����ռ�Ŀ¼
	 * \param product_type ��Ʒ���ͣ�MOD��MYD
	 * \param product ��Ʒ���ƣ�����[BT, AOD, WV, LST]
	 * \return �ò�Ʒ��Ԥ����������ݼ��ϣ���������
	 */
	std::vector<unsigned int> get_years_of_product(
		const std::filesystem::path& workspace_path,
		const std::string& product_type,
		const std::string& product
	);
}

namespace adsma::interpreter::helper::preprocess
{
	/**
	 * \brief ��ȡԤ����������Ŀ¼�����֣�
	 * \param product_type ��Ʒ���ͣ�MOD��MYD
	 * \param product_name ��Ʒ����
	 * \return Ԥ����������Ŀ¼�����֣�
	 */
	inline std::string get_pp_folder(const std::string& product_type, const std::string& product_name)
	{
		return (boost::format("%1%_%2%") % product_type % product_name).str();
	}

	/**
	 * \brief ��ȡԤ��������ļ���
	 * \param product_name ��Ʒ����
	 * \param year ���
	 * \param day DOY
	 * \return Ԥ��������ļ���
	 */
	inline std::string get_pp_output_file_name(const std::string& product_name, const std::string& year, const std::string& day)
	{
		return (boost::format("%1%_%2%_%3%.tif") % product_name % year % day).str();
	}
}
