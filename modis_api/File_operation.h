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
		 * \brief ��ȡ�ļ��е������е�vector<string>��
		 * \return
		 */
		static std::vector<std::string> read_file_all_lines(const std::string&);
		static std::vector<std::string> read_file_all_lines_2(const std::string& file_path);

		/**
		 * \brief ���ַ���д���ļ�
		 * \param file_path_str �ļ�·��
		 * \param content Ҫд���ļ����ַ���
		 * \return �ɹ�����EXIT_SUCCESS��ʧ�ܷ���EXIT_FAILTURE
		 */
		static int write_to_file(const std::string& file_path_str, const std::string& content);

		static void process_dir(std::string& d)
		{
			if (d[d.size() - 1] != '\\')
				d.push_back('\\');
		}

		static std::filesystem::path get_tmp_file(std::filesystem::path const& base_path,
			std::filesystem::path const& tmp_folder, std::string const& tmp_appender, std::string const& ext = "tif");

		static void prepare_file_output_envir(std::filesystem::path const& out_file);
	};
}



