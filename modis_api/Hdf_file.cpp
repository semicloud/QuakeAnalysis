#include "stdafx.h"

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "Hdf_file.h"
#include <boost/log/trivial.hpp>
#include <boost/format/free_funcs.hpp>


modis_api::Hdf_file::Hdf_file(const std::string &file_path)
{
	_file_path = file_path;
	_real_file_name = boost::filesystem::path(file_path).filename().string();
	BOOST_LOG_TRIVIAL(debug) << "real_file_name: " << _real_file_name;

	const std::string file_name_without_extension
		= boost::filesystem::change_extension(_real_file_name, "").string();
	BOOST_LOG_TRIVIAL(debug) << "file_name_without_extension: " << file_name_without_extension;

	std::vector<std::string> strings;
	boost::split(strings, file_name_without_extension, [](char c) {return c == '.'; });
	_main_name = strings[0];
	_date_time_str = strings[1] + "." + strings[2];
	_remainder = strings[3] + strings[4];
	BOOST_LOG_TRIVIAL(debug) << "main_name:" << _main_name << ", date_time_str:" << _date_time_str << ", remainder:" << _remainder;

	std::string date_str = strings[1];
	date_str = date_str.substr(1, date_str.length() - 1);
	_year = date_str.substr(0, 4);
	_day_of_year = date_str.substr(4, 3);
	BOOST_LOG_TRIVIAL(debug) << "date_str: " << date_str << ", year: " << _year << ", day_of_year: " << _day_of_year;

	if (_file_path.find("MOD") != std::string::npos)
	{
		_modis_type = Modis_type::MOD;
		BOOST_LOG_TRIVIAL(debug) << "modis_type: MOD";
	}
	else if (_file_path.find("MYD") != std::string::npos)
	{
		_modis_type = Modis_type::MYD;
		BOOST_LOG_TRIVIAL(debug) << "modis_type: MYD";
	}
	else
	{
		_modis_type = Modis_type::ILLEGAL;
		BOOST_LOG_TRIVIAL(error) << "ILLEGAL MODIS FILE TYPE!";
	}
}


modis_api::Hdf_file::~Hdf_file()
{
}

/**
 * \brief 获取归档路径
 * \param workspace_dir MODIS工作空间目录
 * \return
 */
std::string modis_api::Hdf_file::get_archive_path(const std::string& workspace_dir) const
{
	std::string a_path = str(boost::format("%1%%2%\\%3%\\%4%\\%5%") % workspace_dir % _main_name % _year % _day_of_year % _real_file_name);
	return a_path;
}

std::string modis_api::Hdf_file::get_file_path() const
{
	return _file_path;
}


std::string modis_api::Hdf_file::get_real_file_name() const
{
	return _real_file_name;
}

std::string modis_api::Hdf_file::get_main_name() const
{
	return _main_name;
}

std::string modis_api::Hdf_file::get_date_time_str() const
{
	return _date_time_str;
}

std::string modis_api::Hdf_file::get_year() const
{
	return _year;
}

std::string modis_api::Hdf_file::get_day_of_year() const
{
	return _day_of_year;
}

std::string modis_api::Hdf_file::get_remainder() const
{
	return _remainder;
}

modis_api::Modis_type modis_api::Hdf_file::get_modis_type() const
{
	return _modis_type;
}
