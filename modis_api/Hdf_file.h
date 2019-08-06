#pragma once
#pragma warning( disable : 4251 )

#include <filesystem>
#include <string>
#include "Modis_type.h"

namespace  modis_api
{
	/**
	 * \brief Hdf�ļ���
	 */
	class __declspec(dllexport) Hdf_file
	{
	public:
		Hdf_file(const std::string&);

		~Hdf_file();

		std::filesystem::path get_archive_path(const std::filesystem::path& workspace_dir) const;

		std::string get_file_path() const;
		/**
		 * \brief ��ȡhdf�ļ���������չ����
		 * \return
		 */
		std::string get_real_file_name() const;

		/**
		 * \brief �����ƣ����ļ�����һ����.��ǰ��Ĳ���
		 * \return
		 */
		std::string get_main_name() const;

		/**
		 * \brief ���ڼ�ʱ���ַ��������ڶ�λͬ��HDF�ļ�
		 * \return
		 */
		std::string get_date_time_str() const;

		/**
		 * \brief ���ļ������ڣ�����Ayyyymmdd������������
		 * \return
		 */
		std::string get_year() const;

		/**
		 * \brief ���ļ��Ĺ۲���
		 * \return
		 */
		std::string get_day_of_year() const;

		/**
		 * \brief ���ಿ�֣�����Ҫ
		 * \return
		 */
		std::string get_remainder() const;

		/**
		 * \brief ���ݼ����ͣ�MOD��MYD
		 * \return
		 */
		Modis_type get_modis_type() const;

	private:
		std::string m_file_path;
		std::string m_real_file_name;
		std::string m_main_name;
		std::string m_date_time_str;
		std::string m_year;
		std::string m_day_of_year;
		std::string m_remainder;
		Modis_type m_modis_type;
	};
}


