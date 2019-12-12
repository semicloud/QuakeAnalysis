#include "pch.h"
#include "codg_parser.h"
#include "../commons/collections.h"
#include "../commons/strings.h"
#include <boost/log/trivial.hpp>
#include <boost/algorithm/string.hpp>

std::optional<arma::fmat> proc_codg::parse_fmat(const std::vector<std::string>& dataSec)
{
	std::string mat_str;
	for (std::vector<std::string>::const_iterator it = dataSec.cbegin();
		it != dataSec.cend(); ++it)
	{
		if (it->find("LAT/LON1/LON2/DLON/H") != std::string::npos)
		{
			const size_t index = distance(dataSec.cbegin(), it);
			const size_t start_index = index + 1, end_index = index + 5;
			std::vector<std::string> data_lines = commons::collections::slice(dataSec, start_index, end_index);
			std::for_each(data_lines.begin(), data_lines.end(), [](std::string& data_line)
			{boost::trim(data_line); });
			std::string data_line = boost::join(data_lines, "   ");
			data_line.append(";");
			//cout << data_line << endl;
			mat_str.append(data_line);
		}
	}
	const arma::fmat mat(mat_str);
	return std::optional<arma::fmat>(mat);
}

int proc_codg::parse_epoch(const std::vector<std::string>& dataSec, int& year, int& month, int& day, int& hour)
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
	string trimmed_epoch = commons::strings::trim_but_one_inner_copy(epoch);

	split(epochs, trimmed_epoch, boost::is_any_of(" "));
	BOOST_ASSERT(epochs.size() == 6);
	year = stoi(epochs[0]);
	month = stoi(epochs[1]);
	day = stoi(epochs[2]);
	hour = stoi(epochs[3]);
	return EXIT_SUCCESS;
}
