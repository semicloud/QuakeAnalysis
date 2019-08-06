#include "stdafx.h"

#include "Hdf_file.h"
#include <boost/algorithm/string.hpp>
#include <boost/format/free_funcs.hpp>
#include <boost/log/trivial.hpp>
#include <filesystem>
#include <boost/filesystem.hpp>


modis_api::Hdf_file::Hdf_file(const std::string &file_path)
{
	m_file_path = file_path;
	m_real_file_name = std::filesystem::path(file_path).filename().string();
	BOOST_LOG_TRIVIAL(debug) << "real_file_name: " << m_real_file_name;

	const std::string file_name_without_extension
		= boost::filesystem::change_extension(m_real_file_name, "").string();
	BOOST_LOG_TRIVIAL(debug) << "file_name_without_extension: " << file_name_without_extension;

	std::vector<std::string> strings;
	boost::split(strings, file_name_without_extension, [](char c) {return c == '.'; });
	m_main_name = strings[0];
	m_date_time_str = strings[1] + "." + strings[2];
	m_remainder = strings[3] + strings[4];
	BOOST_LOG_TRIVIAL(debug) << "main_name:" << m_main_name << ", date_time_str:" << m_date_time_str << ", remainder:" << m_remainder;

	std::string date_str = strings[1];
	date_str = date_str.substr(1, date_str.length() - 1);
	m_year = date_str.substr(0, 4);
	m_day_of_year = date_str.substr(4, 3);
	BOOST_LOG_TRIVIAL(debug) << "date_str: " << date_str << ", year: " << m_year << ", day_of_year: " << m_day_of_year;

	if (m_file_path.find("MOD") != std::string::npos)
	{
		m_modis_type = Modis_type::MOD;
		BOOST_LOG_TRIVIAL(debug) << "modis_type: MOD";
	}
	else if (m_file_path.find("MYD") != std::string::npos)
	{
		m_modis_type = Modis_type::MYD;
		BOOST_LOG_TRIVIAL(debug) << "modis_type: MYD";
	}
	else
	{
		m_modis_type = Modis_type::ILLEGAL;
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
std::filesystem::path modis_api::Hdf_file::get_archive_path(const std::filesystem::path& workspace_dir) const
{
	const std::filesystem::path archive_path = workspace_dir / m_main_name / m_year / m_day_of_year / m_real_file_name;
	//std::string a_path = str(boost::format("%1%%2%\\%3%\\%4%\\%5%") % workspace_dir % _main_name % _year % _day_of_year % _real_file_name);
	return archive_path;
}

std::string modis_api::Hdf_file::get_file_path() const
{
	return m_file_path;
}


std::string modis_api::Hdf_file::get_real_file_name() const
{
	return m_real_file_name;
}

std::string modis_api::Hdf_file::get_main_name() const
{
	return m_main_name;
}

std::string modis_api::Hdf_file::get_date_time_str() const
{
	return m_date_time_str;
}

std::string modis_api::Hdf_file::get_year() const
{
	return m_year;
}

std::string modis_api::Hdf_file::get_day_of_year() const
{
	return m_day_of_year;
}

std::string modis_api::Hdf_file::get_remainder() const
{
	return m_remainder;
}

modis_api::Modis_type modis_api::Hdf_file::get_modis_type() const
{
	return m_modis_type;
}
