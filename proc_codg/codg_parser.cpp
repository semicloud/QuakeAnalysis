#include "pch.h"
#include "codg_parser.h"
#include "collections.h"
#include <boost/algorithm/string.hpp>

std::optional<arma::fmat> parse_fmat(const std::vector<std::string>& dataSec)
{
	using namespace std;
	string mat_str;
	for (vector<string>::const_iterator it = dataSec.cbegin();
		it != dataSec.cend(); ++it)
	{
		if (it->find("LAT/LON1/LON2/DLON/H") != string::npos)
		{
			const size_t index = distance(dataSec.cbegin(), it);
			const size_t start_index = index + 1, end_index = index + 5;
			vector<string> data_lines = slice(dataSec, start_index, end_index);
			for_each(data_lines.begin(), data_lines.end(), [](string& data_line)
			{boost::trim(data_line); });
			string data_line = boost::join(data_lines, "   ");
			data_line.append(";");
			//cout << data_line << endl;
			mat_str.append(data_line);
		}
	}
	const arma::fmat mat(mat_str);
	return optional<arma::fmat>(mat);
}
