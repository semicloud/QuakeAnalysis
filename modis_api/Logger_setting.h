#pragma once

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp> 
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>


namespace expr = boost::log::expressions;

namespace modis_api
{
	inline boost::log::formatter get_formatter()
	{
		// return expr::stream << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
		// 	<< " [" << boost::log::trivial::severity << "]" << " " << expr::format_named_scope("Scope", keywd::format = "%n :%f")
		// 	<< " " << expr::smessage;
		return expr::stream << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
			<< " [" << boost::log::trivial::severity << "]" << " " << expr::smessage;
	}

	/**
	 * \brief 初始化console logger
	 */
	void init_console_logger()
	{
		auto console_sink = boost::log::add_console_log(std::cout);
		console_sink->set_formatter(get_formatter());

		//<< expr::format_named_scope("Scope", boost::log::keywords::format = "%n (%f : %l)")

		// console_sink->set_formatter(
		// 	boost::log::expressions::stream <<
		// 	boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
		// 	<< " [" << boost::log::trivial::severity << "] [" << boost::log::expressions::format_named_scope("Scope", "%f")
		// 	<< boost::log::expressions::format_named_scope("Scope", "%n") << "]" << boost::log::expressions::smessage
		// );

		console_sink->imbue(console_sink->getloc());
		boost::log::add_common_attributes();
		//boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());
	}

	/**
	 * \brief 初始化文件logger，文件名file_prefix_YYMMDD.log
	 * \brief 每天一个文件
	 * \param log_dir 日志所在的目录
	 * \param file_prefix 文件名前缀，目前的设计是一个exe每天一个文件
	 */
	void init_file_logger(const std::string& log_dir, const std::string& file_prefix)
	{
		auto file_sink = boost::log::add_file_log(
			boost::log::keywords::file_name = log_dir + file_prefix + "_%Y%m%d.log",
			boost::log::keywords::auto_flush = true,
			boost::log::keywords::open_mode = std::ios::app
		);

		// file_sink->set_formatter(
		// 	boost::log::expressions::stream <<
		// 	boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
		// 	<< " [" << boost::log::trivial::severity << "]" << " " << boost::log::expressions::smessage
		// );

		file_sink->set_formatter(get_formatter());
		file_sink->imbue(file_sink->getloc());
		boost::log::add_common_attributes();
		//boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());
	}

	/**
	 * \brief 设置日志输出级别
	 * \param level 日志输出级别
	 */
	inline  void set_logger_severity(boost::log::trivial::severity_level level)
	{
		boost::log::core::get()->set_filter(boost::log::trivial::severity >= level);
	}
}



