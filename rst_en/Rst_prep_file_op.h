#pragma once

#include <string>
#include <vector>
#include <boost/date_time/gregorian/greg_date.hpp>

/**
 * \brief Rst算法预处理的相关文件操作，包括生成doy、获取全部文件的起止年份、获取一个文件的相邻数据、历史数据等
 */
class Rst_prep_file_op
{
public:
	Rst_prep_file_op();
	~Rst_prep_file_op();

	static std::vector<std::string> generate_doys(const boost::gregorian::date&,
		const boost::gregorian::date&, const unsigned&);

	static std::vector<std::string> get_doys_by_year(const unsigned,
		const std::vector<std::string>&);

	static std::vector<unsigned> get_file_years(const std::vector<std::string>&);

	static std::vector<std::string> get_adjacent_file_paths(
		const std::vector<std::string>&, const boost::gregorian::date&, int);

	static std::vector<std::string> get_hist_file_paths(
		const std::vector<std::string>&, const boost::gregorian::date&,
		const unsigned);

	static bool exist_file_according_to_doy(const std::vector<std::string>&,
		const std::string&, std::string& out_file_path);

	static std::vector<std::string> get_files_by_doys(
		const std::vector<std::string>&, const std::vector<std::string>&);

	static bool parse_file_year_doy(const std::string& file_path,
		std::string& year, std::string& doy);

	static bool all_data_ready(const std::vector<std::string>&,
		const std::vector<std::string>&, std::vector<std::string>&);

};

