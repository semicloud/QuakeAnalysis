#pragma once
#pragma warning( disable : 4251 )

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

		std::string get_archive_path(const std::string& workspace_dir) const;

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
		std::string _file_path;
		std::string _real_file_name;
		std::string _main_name;
		std::string _date_time_str;
		std::string _year;
		std::string _day_of_year;
		std::string _remainder;
		Modis_type _modis_type;
	};
}


