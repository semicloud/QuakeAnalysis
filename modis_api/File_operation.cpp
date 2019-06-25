#include "stdafx.h"
#include "File_operation.h"
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <fstream>

namespace fs = boost::filesystem;

modis_api::File_operation::File_operation() = default;
modis_api::File_operation::~File_operation() = default;

std::string modis_api::File_operation::say_hello()
{
	return std::string("hello.");
}

/**
 * \brief 根据路径获取文件名
 * \param paths 路径名集合
 * \return 文件名集合
 */
std::vector<std::string> modis_api::File_operation::get_file_names(
	const std::vector<std::string>& paths)
{
	std::vector<std::string> file_names;
	std::transform(paths.begin(), paths.end(), std::back_inserter(file_names),
		[](const auto& p) { return fs::path(p).filename().string(); });
	return file_names;
}


/**
 * \brief 在dir目录下找到所有扩展名为extension的文件
 * \param dir 目录
 * \param extension 扩展名
 * \return
 */
std::vector<std::string> modis_api::File_operation::get_all_files_by_extension(
	const std::string& dir, const std::string& extension)
{
	std::vector<std::string> ret;
	auto it = boost::filesystem::recursive_directory_iterator(dir);
	for (; it != boost::filesystem::recursive_directory_iterator(); ++it)
	{
		if (it->path().extension() == extension)
		{
			ret.push_back(it->path().string());
		}
	}
	BOOST_LOG_TRIVIAL(debug) << "在" << dir << "目录下找到" << ret.size() << "个" << extension << "文件";
	return ret;
}

void modis_api::File_operation::clear_directory(const std::string& directory)
{
	if (!fs::is_directory(directory))
	{
		BOOST_LOG_TRIVIAL(error) << str(boost::format("%1%不是合法的目录，无法清空") % directory);
		return;
	}
	fs::directory_iterator it(directory);
	for (; it != fs::directory_iterator(); ++it)
	{
		if (is_regular_file(it->path()))
		{
			fs::remove(it->path());
			BOOST_LOG_TRIVIAL(debug) << str(boost::format("删除文件%1%") % it->path().string());
		}
		else if (is_directory(it->path()))
		{
			fs::remove_all(it->path());
			BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("删除目录%1%") % it->path().string());
		}
	}

	BOOST_LOG_TRIVIAL(debug) << str(boost::format("已清空目录%1%") % directory);
}

std::vector<std::string> modis_api::File_operation::read_file_all_lines(const std::string& file_path)
{
	if (!fs::exists(file_path)) throw std::runtime_error(str(boost::format("file %1% not found!") % file_path));
	std::vector<std::string> svec;
	std::ifstream ifs(file_path);
	copy(std::istream_iterator<std::string>(ifs), std::istream_iterator<std::string>(), back_inserter(svec));
	return svec;
}

std::vector<std::string> modis_api::File_operation::read_file_all_lines_2(const std::string& file_path)
{
	if (!fs::exists(file_path)) throw std::runtime_error(str(boost::format("file %1% not found!") % file_path));
	std::vector<std::string> svec;
	std::ifstream ifs(file_path);
	std::string str;
	while (getline(ifs, str))
		svec.push_back(str);
	return svec;
}

