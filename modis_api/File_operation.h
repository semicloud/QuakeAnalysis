#pragma once

#include "stdafx.h"
#include <string>
#include <vector>


namespace modis_api
{
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
		static std::vector<std::string> read_file_all_lines(const std::string&);
		static std::vector<std::string> read_file_all_lines_2(const std::string& file_path);

		/**
		 * \brief 将字符串写入文件
		 * \param file_path_str 文件路径
		 * \param content 要写入文件的字符串
		 * \return 成功返回EXIT_SUCCESS，失败返回EXIT_FAILTURE
		 */
		static int write_to_file(const std::string& file_path_str, const std::string& content);

		static void process_dir(std::string& d)
		{
			if (d[d.size() - 1] != '\\')
				d.push_back('\\');
		}
	};
}



