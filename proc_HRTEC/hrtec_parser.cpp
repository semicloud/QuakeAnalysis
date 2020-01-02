#include "stdafx.h"
#include "hrtec_parser.h"
#include "../commons/collections.h"
#include "../commons/strings.h"
#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>


std::optional<arma::fmat> proc_hrtec::parse_mat(const std::vector<std::string>& dataSec)
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
			for_each(data_lines.begin(), data_lines.end(), [](std::string& data_line)
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

int proc_hrtec::parse_epoch(const std::vector<std::string>& dataSec, int& year, int& month, int& day, int& hour)
{
	BOOST_ASSERT(!dataSec.empty());
	std::vector<std::string>::const_iterator it = std::find_if(dataSec.cbegin(), dataSec.cend(),
		[](const std::string& str) {
		return str.find("EPOCH OF CURRENT MAP") != std::string::npos;
	});
	if (it == dataSec.cend())
	{
		BOOST_LOG_TRIVIAL(error) << "No EPOCH OF CURRENT MAP found!";
		std::cerr << "ERROR! No EPOCH OF CURRENT MAP found!" << std::endl;
	}
	std::vector<std::string> epochs;
	std::string epoch = boost::trim_copy(it->substr(0, 40));
	BOOST_LOG_TRIVIAL(debug) << "Epoch: " << epoch;
	std::string trimmed_epoch = commons::strings::trim_but_one_inner_copy(epoch);

	split(epochs, trimmed_epoch, boost::is_any_of(" "));
	BOOST_ASSERT(epochs.size() == 6);
	year = stoi(epochs[0]);
	month = stoi(epochs[1]);
	day = stoi(epochs[2]);
	hour = stoi(epochs[3]);
	return EXIT_SUCCESS;
}
