#pragma once
#pragma warning( disable : 4251 )

#include <string>
#include "Modis_type.h"

namespace  modis_api
{
	/**
	 * \brief Hdf文件类
	 */
	class __declspec(dllexport) Hdf_file
	{
	public:
		Hdf_file(const std::string&);

		~Hdf_file();

		std::string get_archive_path(const std::string& workspace_dir) const;

		std::string get_file_path() const;
		/**
		 * \brief 获取hdf文件名（带扩展名）
		 * \return
		 */
		std::string get_real_file_name() const;

		/**
		 * \brief 主名称，即文件名第一个“.”前面的部分
		 * \return
		 */
		std::string get_main_name() const;

		/**
		 * \brief 日期及时间字符串，用于定位同组HDF文件
		 * \return
		 */
		std::string get_date_time_str() const;

		/**
		 * \brief 该文件的日期，就是Ayyyymmdd里面的这个日期
		 * \return
		 */
		std::string get_year() const;

		/**
		 * \brief 该文件的观测日
		 * \return
		 */
		std::string get_day_of_year() const;

		/**
		 * \brief 其余部分，不重要
		 * \return
		 */
		std::string get_remainder() const;

		/**
		 * \brief 数据集类型，MOD或MYD
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


