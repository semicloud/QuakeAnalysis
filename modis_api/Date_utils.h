#pragma once
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

namespace modis_api
{
	class __declspec(dllexport)  Date_utils
	{
	public:
		Date_utils();
		~Date_utils();

		static std::string get_doy_year(const std::string&);
		static std::string get_doy_year_underline(const std::string&);

		static std::string get_doy_day(const std::string&);
		static std::string get_doy_day_underline(const std::string&);
		static std::vector<std::string> get_doy_day_span(const std::string&, const std::string&);
		static std::string get_doy_str(const boost::gregorian::date&);
		static std::string
			get_doy_str_underline(const boost::gregorian::date&);
		static boost::gregorian::date get_date_from_doy_str(const std::string&);
		static boost::gregorian::date
			get_date_from_doy_str_underline(const std::string& year_doy);

	private:
		static std::string i_to_doy(const long i);
	};

}

