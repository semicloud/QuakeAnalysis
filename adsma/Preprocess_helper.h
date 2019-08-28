#pragma once

#include <boost/format.hpp>
#include <string>


namespace adsma
{
	/**
	 * \brief ��ȡԤ���������Ŀ¼�����֣�
	 * \param product_type ��Ʒ���ͣ�MOD��MYD
	 * \param product_name ��Ʒ���ƣ�BT��MV��AOD��LST
	 * \return Ԥ���������Ŀ¼�����֣�
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