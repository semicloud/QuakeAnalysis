#include "stdafx.h"
#include "Date_utils.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/date_time/local_time/dst_transition_day_rules.hpp>
#include <boost/log/trivial.hpp>
#include <sstream>


modis_api::Date_utils::Date_utils()
= default;


modis_api::Date_utils::~Date_utils()
= default;

std::string modis_api::Date_utils::get_doy_year(const std::string& doy)
{
	return doy.substr(0, 4);
}

std::string modis_api::Date_utils::get_doy_year_underline(const std::string& doy)
{
	std::vector<std::string> svec;
	svec.reserve(2);
	boost::split(svec, doy, boost::is_any_of("_"));
	std::string ans = svec[0];
	//BOOST_LOG_TRIVIAL(debug) << "Get year from " << doy << ", result: " << ans;
	return ans;
}

std::string modis_api::Date_utils::get_doy_day(const std::string& doy)
{
	return doy.substr(4, 3);
}

std::string modis_api::Date_utils::get_doy_day_underline(const std::string& doy)
{
	std::vector<std::string> svec;
	svec.reserve(2);
	boost::split(svec, doy, boost::is_any_of("_"));
	std::string ans = svec[1];
	//BOOST_LOG_TRIVIAL(debug) << "Get day from " << doy << ", result: " << ans;
	return ans;
}

/**
 * \brief
 * \param d1 001
 * \param d2 030
 * \return 001 002 003 004 ... 029 030
 */
std::vector<std::string> modis_api::Date_utils::get_doy_day_span(const std::string& d1, const std::string& d2)
{
	const int start = boost::lexical_cast<int>(trim_left_copy_if(d1, boost::is_any_of("0")));
	const int end = boost::lexical_cast<int>(trim_left_copy_if(d2, boost::is_any_of("0")));
	BOOST_LOG_TRIVIAL(debug) << "doy -> int: " << d1 << "->" << start << ", " << d2 << "->" << end;
	std::vector<std::string> ret;
	std::ostringstream os;
	for (int i = start; i != end + 1; ++i)
	{
		const auto doy = i_to_doy(i);
		os << doy << ",";
		ret.push_back(doy);
	}
	BOOST_LOG_TRIVIAL(debug) << "生成的DOY：" << os.str();
	return ret;
}

std::string modis_api::Date_utils::i_to_doy(const long i)
{
	std::string doy;
	if (i < 10)
		doy = "00" + std::to_string(i);
	else if (i >= 10 && i <= 99)
		doy = "0" + std::to_string(i);
	else
		doy = std::to_string(i);
	return doy;
}

/**
 * \brief 获得日期的doy_str形式
 * \return
 */
std::string modis_api::Date_utils::get_doy_str(const boost::gregorian::date& date)
{
	const boost::gregorian::date first_day(date.year(), 1, 1);
	const auto day_span = (date - first_day).days() + 1;
	//std::string doy;
	/*if (day_span < 10)
		doy = "00" + std::to_string(day_span);
	else if (day_span >= 10 && day_span <= 99)
		doy = "0" + std::to_string(day_span);
	else
		doy = std::to_string(day_span);*/
	const std::string ans = boost::lexical_cast<std::string>(date.year()) + i_to_doy(day_span);
	BOOST_LOG_TRIVIAL(debug) << "date:" << date << ", doy:" << ans;
	return ans;
}

std::string modis_api::Date_utils::
get_doy_str_underline(const boost::gregorian::date& date)
{
	const boost::gregorian::date first_day(date.year(), 1, 1);
	const auto day_span = (date - first_day).days() + 1;
	const std::string ans = boost::lexical_cast<std::string>(date.year())
		+ "_" + i_to_doy(day_span);
	//BOOST_LOG_TRIVIAL(debug) << "Convert date " << date << " to doy: " <<
		//ans;
	return ans;
}

/**
 * \brief 将doy_str解析为date对象
 * \return
 */
boost::gregorian::date modis_api::Date_utils::get_date_from_doy_str(const std::string& date_str)
{
	//BOOST_LOG_TRIVIAL(debug) << "convert date_str " << date_str << " to date..";
	if (date_str.length() != 7 && date_str.length() != 8)
	{
		BOOST_LOG_TRIVIAL(error) << "Unparsed date_str:" << date_str;
		exit(EXIT_FAILURE);
	}
	const auto year = boost::lexical_cast<int>(date_str.substr(0, 4));
	boost::gregorian::date ans;
	if (date_str.length() == 7)
	{
		const auto doy = boost::lexical_cast<int>(trim_left_copy_if(date_str.substr(4, 3), boost::is_any_of("0")));
		//BOOST_LOG_TRIVIAL(debug) << "doy: " << doy;
		ans = boost::gregorian::date(year, 1, 1) + boost::gregorian::days(doy) + boost::gregorian::days(-1);
	}

	if (date_str.length() == 8)
	{
		const auto month = boost::lexical_cast<int>(trim_left_copy_if(date_str.substr(4, 2), boost::is_any_of("0")));
		const auto day = boost::lexical_cast<int>(trim_left_copy_if(date_str.substr(6, 2), boost::is_any_of("0")));
		//BOOST_LOG_TRIVIAL(debug) << "month: " << month << ", day: " << day;
		ans = boost::gregorian::date(year, month, day);
	}
	BOOST_LOG_TRIVIAL(debug) << "date_str " << date_str << " to date object " << to_iso_string(ans);
	return ans;
}


/**
 * \brief 从字符串(日期以下划线分隔)中解析出date
 * \param year_doy
 * \return
 * \remark 支持两种形式的doy, yyyy_doy或yyyy_mm_dd
 */
boost::gregorian::date
modis_api::Date_utils::get_date_from_doy_str_underline(const std::string& year_doy)
{
	const auto sep_number = std::count(year_doy.begin(), year_doy.end(), '_');
	boost::gregorian::date ans;
	std::vector<std::string> svec;
	boost::algorithm::split(svec, year_doy, boost::algorithm::is_any_of("_"));
	if (sep_number == 1)
	{
		const auto year = boost::lexical_cast<unsigned>(svec[0]);
		const auto day = boost::lexical_cast<unsigned>(trim_left_copy_if(svec[1],
			boost::algorithm::is_any_of("0")));
		ans = boost::gregorian::date(year, 1, 1)
			+ boost::gregorian::days(day)
			+ boost::gregorian::days(-1);
	}
	if (sep_number == 2) {
		const auto year = boost::lexical_cast<unsigned>(svec[0]);
		const auto month = boost::lexical_cast<unsigned>(trim_left_copy_if(svec[1],
			boost::algorithm::is_any_of("0")));
		const auto day = boost::lexical_cast<unsigned>(trim_left_copy_if(svec[2],
			boost::algorithm::is_any_of("0")));
		ans = boost::gregorian::date(year, month, day);
	}

	BOOST_LOG_TRIVIAL(debug) << "Convert " << year_doy << " to Date: " <<
		ans;

	return ans;
}


