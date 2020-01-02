#pragma once
#include "pch.h"
namespace logger
{
	inline boost::log::formatter get_formatter()
	{
		return boost::log::expressions::stream <<
			boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S") <<
			" [" << boost::log::trivial::severity << "]" << " " <<
			boost::log::expressions::smessage;
	}

	inline void init_console_logger()
	{
		auto console_sink = boost::log::add_console_log(std::cout);
		console_sink->set_formatter(get_formatter());
		console_sink->imbue(console_sink->getloc());
		boost::log::add_common_attributes();
	}

	inline void init_file_logger(std::string const& dir, std::string const& prefix)
	{
		auto file_sink = boost::log::add_file_log(
			boost::log::keywords::file_name = dir + prefix + "_%Y%m%d.log",
			boost::log::keywords::auto_flush = true,
			boost::log::keywords::open_mode = std::ios::app
		);
		file_sink->set_formatter(get_formatter());
		file_sink->imbue(file_sink->getloc());
		boost::log::add_common_attributes();
	}

	inline void set_severity(boost::log::trivial::severity_level level)
	{
		boost::log::core::get()->set_filter(boost::log::trivial::severity >= level);
	}
}