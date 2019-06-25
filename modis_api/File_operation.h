#pragma once

#include "stdafx.h"
#include <string>
#include <vector>


namespace modis_api
{
	using namespace std;

	class __declspec(dllexport) File_operation
	{
	public:
		File_operation();
		~File_operation();
		static std::string say_hello();
		static std::vector<std::string> get_file_names(
			const std::vector<std::string>&
		);
		static std::vector<std::string> get_all_files_by_extension(
			const std::string&, const std::string&);
		static void clear_directory(const std::string&);
		/**
		 * \brief 读取文件中的所有行到vector<string>中
		 * \return
		 */
		static vector<string> read_file_all_lines(const std::string&);
		static vector<string> read_file_all_lines_2(const std::string& file_path);

		static void process_dir(std::string& d)
		{
			if (d[d.size() - 1] != '\\')
				d.push_back('\\');
		}
	};
}



