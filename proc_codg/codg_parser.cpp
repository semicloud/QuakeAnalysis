#include "pch.h"
#include "codg_parser.h"
#include "strings.h"
#include "collections.h"
#include <boost/log/trivial.hpp>
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

int parse_epoch(const std::vector<std::string>& dataSec, int& year, int& month, int& day, int& hour)
{
	using namespace std;
	BOOST_ASSERT(!dataSec.empty());
	vector<string>::const_iterator it = find_if(dataSec.cbegin(), dataSec.cend(),
		[](const string& str) {
		return str.find("EPOCH OF CURRENT MAP") != string::npos;
	});
	if (it == dataSec.cend())
	{
		BOOST_LOG_TRIVIAL(error) << "No EPOCH OF CURRENT MAP found!";
		cerr << "ERROR! No EPOCH OF CURRENT MAP found!" << endl;
	}
	vector<string> epochs;
	string epoch = boost::trim_copy(it->substr(0, 40));
	BOOST_LOG_TRIVIAL(debug) << "Epoch: " << epoch;
	string trimmed_epoch = trim_inner_copy(epoch);

	split(epochs, trimmed_epoch, boost::is_any_of(" "));
	BOOST_ASSERT(epochs.size() == 6);
	year = stoi(epochs[0]);
	month = stoi(epochs[1]);
	day = stoi(epochs[2]);
	hour = stoi(epochs[3]);
	return EXIT_SUCCESS;
}
