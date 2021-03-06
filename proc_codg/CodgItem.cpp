#include "pch.h"
#include "../commons/collections.h"
#include "CodgItem.h"
#include "codg_parser.h"
#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>
#include <filesystem>
#include <iostream>

CodgItem::CodgItem(const std::vector<std::string>& dataSec)
{
	const int i = proc_codg::parse_epoch(dataSec, m_year, m_month, m_day, m_hour);
	assert(i == EXIT_SUCCESS);
	BOOST_LOG_TRIVIAL(debug) << "m_year:" << m_year << ", m_month:"
		<< m_month << ", m_day:" << m_day << ", m_hour:" << m_hour;
	m_mat = proc_codg::parse_fmat(dataSec);
}

CodgItem::~CodgItem()
{
}

const std::string TEC_MAP_START = "START OF TEC MAP";
const std::string TEC_MAP_END = "END OF TEC MAP";

std::vector<CodgItem> CodgItem::load_items(const std::string& file_path_str)
{
	std::vector<CodgItem> items;
	const std::filesystem::path codg_file_path(file_path_str);
	assert(exists(codg_file_path));
	auto LoadFile = [](const std::string& file_path)
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
	const std::vector<std::string> lines = LoadFile(codg_file_path.string());
	std::vector<size_t> indexes_start;
	std::vector<size_t> indexes_end;
	for (std::vector<std::string>::const_iterator it = lines.cbegin();
		it != lines.cend(); ++it)
	{
		if (it->find(TEC_MAP_START) != std::string::npos)
			indexes_start.push_back(distance(lines.cbegin(), it));
		if (it->find(TEC_MAP_END) != std::string::npos)
			indexes_end.push_back(distance(lines.cbegin(), it));
	}
	assert(!indexes_start.empty());
	assert(indexes_start.size() == indexes_end.size());
	for (size_t i = 0; i != indexes_start.size(); ++i)
	{
		std::vector<std::string> data_section = commons::collections::slice(lines, indexes_start[i], indexes_end[i]);
		items.emplace_back(data_section);
	}
	return items;
}
