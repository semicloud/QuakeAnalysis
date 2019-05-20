#pragma once

#include <string>
#include <vector>

/**
 * \brief Rst预处理输入对象
 */
class Rst_prep_input
{
public:
	Rst_prep_input(std::string curr_file,
		std::vector<std::string> adj_files,
		std::vector<std::string> hist_files,
		std::string output_file)
		: _curr_file(curr_file), _adj_files(adj_files),
		_hist_files(hist_files), _output_file(output_file)
	{

	}

	~Rst_prep_input() = default;

	std::string get_curr_file() const
	{
		return _curr_file;
	}

	std::string get_output_file() const
	{
		return _output_file;
	}

	std::vector<std::string> get_adjacent_files() const
	{
		return _adj_files;
	}

	std::vector<std::string> get_hist_files() const
	{
		return _hist_files;
	}

	static std::vector<Rst_prep_input>
		load(unsigned current_year, 
			const std::vector<std::string>& all_tif_files,
			const std::vector<std::string>& generated_doys,
			const std::string& output_dir, unsigned k);
private:
	std::string _curr_file;
	std::vector<std::string> _adj_files;
	std::vector<std::string> _hist_files;
	std::string _output_file;
};

