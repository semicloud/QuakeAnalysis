#include "stdafx.h"
#include "HrtecItem.h"
#include "hrtec_parser.h"
#include "../commons/collections.h"
#include <boost/log/trivial.hpp>
#include <cassert>
#include <filesystem>

proc_hrtec::HrtecItem::HrtecItem(const std::vector<std::string>& dataSec)
{
	const int i = parse_epoch(dataSec, m_year, m_month, m_day, m_hour);
	assert(i == EXIT_SUCCESS);
	BOOST_LOG_TRIVIAL(debug) << "m_year:" << m_year << ", m_month:"
		<< m_month << ", m_day:" << m_day << ", m_hour:" << m_hour;
	m_mat = parse_mat(dataSec);
}

proc_hrtec::HrtecItem::~HrtecItem()
{
}

const std::string TEC_MAP_START = "START OF TEC MAP";
const std::string TEC_MAP_END = "END OF TEC MAP";

std::vector<proc_hrtec::HrtecItem> proc_hrtec::HrtecItem::load_items(const std::string& file_path_str)
{
	using namespace arma;
	using namespace std;
	using namespace std::filesystem;

	vector<HrtecItem> items;
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
		vector<string> data_section = commons::collections::slice(lines, indexes_start[i], indexes_end[i]);
		items.emplace_back(data_section);
	}
	return items;
}