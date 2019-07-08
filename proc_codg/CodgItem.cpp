#include "pch.h"
#include "CodgItem.h"
#include "codg_parser.h"
#include "collections.h"
#include "strings.h"
#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>
#include <filesystem>
#include <iostream>

CodgItem::CodgItem(const std::vector<std::string>& dataSec)
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
	m_year = stoi(epochs[0]);
	m_month = stoi(epochs[1]);
	m_day = stoi(epochs[2]);
	m_hour = stoi(epochs[3]);
	BOOST_LOG_TRIVIAL(debug) << "m_year:" << m_year << ", m_month:"
		<< m_month << ", m_day:" << m_day << ", m_hour:" << m_hour;
	m_mat = parse_fmat(dataSec);
}

CodgItem::~CodgItem()
{
}

const std::string TEC_MAP_START = "START OF TEC MAP";
const std::string TEC_MAP_END = "END OF TEC MAP";

std::vector<CodgItem> CodgItem::load_items(const std::string& file_path_str)
{
	using namespace arma;
	using namespace std;
	using namespace std::filesystem;

	vector<CodgItem> items;
	const path codg_file_path(file_path_str);
	assert(exists(codg_file_path));
	auto LoadFile = [](const string& file_path)
	{
		using namespace std;
		vector<string> ans;
		ifstream stream(file_path);
		string line;
		while (getline(stream, line))
			ans.push_back(line);
		cout << ans.size() << " lines loaded from " << file_path << endl;
		return ans;
	};
	const vector<string> lines = LoadFile(codg_file_path.string());
	vector<size_t> indexes_start;
	vector<size_t> indexes_end;
	for (vector<string>::const_iterator it = lines.cbegin();
		it != lines.cend(); ++it)
	{
		if (it->find(TEC_MAP_START) != string::npos)
			indexes_start.push_back(distance(lines.cbegin(), it));
		if (it->find(TEC_MAP_END) != string::npos)
			indexes_end.push_back(distance(lines.cbegin(), it));
	}
	assert(!indexes_start.empty());
	assert(indexes_start.size() == indexes_end.size());
	for (size_t i = 0; i != indexes_start.size(); ++i)
	{
		vector<string> data_section = slice(lines, indexes_start[i], indexes_end[i]);
		items.emplace_back(data_section);
	}
	return items;
}
