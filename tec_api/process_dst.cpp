#include "stdafx.h"
#include "process_dst.h"

const size_t DATE_IDX = 2;
const size_t DATA_IDX = 6;

std::string proc_dst::load_file(boost::filesystem::path const& file_path)
{
	std::string content{};
	boost::filesystem::load_string_file(file_path, content);
	return content;
}

std::vector<std::string> proc_dst::str2vec(std::string const& content)
{
	std::vector<std::string> vec;
	boost::algorithm::split(vec, content, boost::is_any_of("\n"));
	return vec;
}

int proc_dst::index_of_str(std::vector<std::string>& vec, std::string const& str)
{
	std::vector<std::string>::const_iterator it =
		std::find_if(vec.cbegin(), vec.cend(),
			[&str](std::string const& p) { return boost::algorithm::contains(p, str); });
	return it != vec.cend() ? static_cast<int>(std::distance(vec.cbegin(), it)) : -1;
}

std::tuple<int, int> proc_dst::parse_year_month(std::string const& str)
{
	std::string s = boost::trim_copy(str);
	std::vector<std::string> vec;
	boost::split(vec, s, boost::is_any_of(" "), boost::algorithm::token_compress_on);
	const std::string dstr = (boost::format("1-%1%-%2%") % vec.at(0) % vec.at(1)).str();
	const boost::gregorian::date date{ boost::gregorian::from_uk_string(dstr) };
	return std::tuple<int, int>{date.year(), date.month()};
}

std::vector<size_t> proc_dst::get_day_vec(std::string const& content)
{
	std::vector<std::string> vec = str2vec(content);
	std::vector<std::string> vec2;
	std::remove_copy_if(vec.begin(), vec.end(), std::back_inserter(vec2),
		[](std::string const& line) { return boost::trim_copy(line).empty(); });
	std::vector<size_t> ans;
	for (size_t i = DATA_IDX; i != vec2.size(); ++i)
	{
		std::string s = vec2.at(i).substr(0, 2);
		boost::trim(s);
		ans.push_back(boost::lexical_cast<size_t>(s));
	}
	return ans;
}

std::vector<int> proc_dst::get_value_vec(std::string const& content)
{
	std::vector<std::string> vec = str2vec(content);
	std::vector<std::string> vec2;
	std::remove_copy_if(vec.begin(), vec.end(), std::back_inserter(vec2),
		[](std::string const& line) { return boost::trim_copy(line).empty(); });
	std::vector<int> ans;
	for (size_t i = DATA_IDX; i != vec2.size(); ++i)
	{
		std::string s = boost::trim_copy(vec2.at(i).substr(2));
		std::vector<std::string> nums;
		boost::split(nums, s, boost::is_any_of(" "), boost::token_compress_on);
		std::transform(nums.cbegin(), nums.cend(), std::back_inserter(ans),
			[](std::string const& p) {return std::stoi(p); });
	}
	return ans;
}

std::vector<std::string> proc_dst::get_time_vec(int year, int month, std::vector<size_t> const& days)
{
	std::vector<std::string> ans;
	for (size_t day : days)
	{
		for (size_t hour = 1; hour != 25; ++hour)
		{
			ans.push_back((boost::format("%1%%2$02d%3$02d%4$02d") % year % month % day % hour).str());
		}
	}
	return ans;
}

std::string proc_dst::process_dst(std::string const& content)
{
	std::vector<std::string> vec{ str2vec(content) };
	std::tuple<int, int> t = parse_year_month(vec.at(DATE_IDX));
	const int year = std::get<0>(t);
	const int month = std::get<1>(t);
	const std::vector<size_t> days = get_day_vec(content);
	const std::vector<std::string> times = get_time_vec(year, month, days);
	const std::vector<int> values = get_value_vec(content);
	assert(times.size() == values.size());
	std::stringstream ss;
	for (size_t i = 0; i != times.size(); ++i)
		ss << times.at(i) << " " << values.at(i) << "\n";
	return boost::trim_copy(ss.str());
}

