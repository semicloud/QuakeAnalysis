#pragma once
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <sstream>
#include <boost/date_time/gregorian/gregorian.hpp>

class Rst_checker
{
public:
	Rst_checker();
	~Rst_checker();

	static bool check_date(const boost::gregorian::date& start,
		const boost::gregorian::date& end, std::ostringstream& os);

	static bool check_year(const std::vector<unsigned>& all_years,
		std::ostringstream& os);

	static bool check_data_dir(const std::string& data_dir, std::ostringstream& os);

	static bool check_file_exists(const std::vector<std::string>& all_files,
		std::ostringstream& os);

	static bool check_doy_file_ready(const std::vector<std::string>& all_files,
		const std::vector<std::string>& doys, std::ostringstream& os);

	static bool check_tif(const std::vector<std::string>&,
		std::ostringstream& os);
};

