#include "stdafx.h"
#include "process_kp_ap.h"


std::string proc_kp_ap::process_kp_ap(std::string const& content)
{
	std::vector<std::string> vec;
	boost::split(vec, content, boost::is_any_of("\n"), boost::token_compress_on);
	std::vector<std::string> vec2;
	std::remove_copy_if(vec.begin(), vec.end(), std::back_inserter(vec2),
		[](std::string const& p) {return boost::trim_copy(p).empty(); });
	std::stringstream ss;
	ss << "Year   Month   day   hour   kp   ap\n";
	for (std::string const& line : vec2)
	{
		std::string year = line.substr(0, 2);
		std::string month = line.substr(2, 2);
		std::string day = line.substr(4, 2);

		std::string kp00 = line.substr(12, 2);
		std::string kp03 = line.substr(14, 2);
		std::string kp06 = line.substr(16, 2);
		std::string kp09 = line.substr(18, 2);
		std::string kp12 = line.substr(20, 2);
		std::string kp15 = line.substr(22, 2);
		std::string kp18 = line.substr(24, 2);
		std::string kp21 = line.substr(26, 2);
		boost::trim(kp00);
		boost::trim(kp03);
		boost::trim(kp06);
		boost::trim(kp09);
		boost::trim(kp12);
		boost::trim(kp15);
		boost::trim(kp18);
		boost::trim(kp21);

		std::string ap00 = line.substr(31, 3);
		std::string ap03 = line.substr(34, 3);
		std::string ap06 = line.substr(37, 3);
		std::string ap09 = line.substr(40, 3);
		std::string ap12 = line.substr(43, 3);
		std::string ap15 = line.substr(46, 3);
		std::string ap18 = line.substr(49, 3);
		std::string ap21 = line.substr(52, 3);

		boost::trim(ap00);
		boost::trim(ap03);
		boost::trim(ap06);
		boost::trim(ap09);
		boost::trim(ap12);
		boost::trim(ap15);
		boost::trim(ap18);
		boost::trim(ap21);

		ss << year << "   " << month << "   " << day << "   " << "00" << "   " << kp00 << "   " << ap00 << "\n";
		ss << year << "   " << month << "   " << day << "   " << "03" << "   " << kp03 << "   " << ap03 << "\n";
		ss << year << "   " << month << "   " << day << "   " << "06" << "   " << kp06 << "   " << ap06 << "\n";
		ss << year << "   " << month << "   " << day << "   " << "09" << "   " << kp09 << "   " << ap09 << "\n";
		ss << year << "   " << month << "   " << day << "   " << "12" << "   " << kp12 << "   " << ap12 << "\n";
		ss << year << "   " << month << "   " << day << "   " << "15" << "   " << kp15 << "   " << ap15 << "\n";
		ss << year << "   " << month << "   " << day << "   " << "18" << "   " << kp18 << "   " << ap18 << "\n";
		ss << year << "   " << month << "   " << day << "   " << "21" << "   " << kp21 << "   " << ap21 << "\n";
	}

	return boost::trim_copy(ss.str());
}


