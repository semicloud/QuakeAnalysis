#include "stdafx.h"
#include "File_operation.h"
#include <algorithm>
#include <boost/format.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <fstream>
#include <filesystem>

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
	using namespace std;
	using namespace filesystem;
	vector<string> file_names;
	transform(paths.begin(), paths.end(), back_inserter(file_names),
		[](const auto& p) { return path(p).filename().string(); });
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
	using namespace std;
	using namespace filesystem;
	vector<string> ret;
	auto it = recursive_directory_iterator(dir);
	for (; it != recursive_directory_iterator(); ++it)
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
	using namespace std;
	using namespace filesystem;
	if (!is_directory(directory))
	{
		BOOST_LOG_TRIVIAL(error) << str(boost::format("%1%不是合法的目录，无法清空") % directory);
		return;
	}
	directory_iterator it(directory);
	for (; it != directory_iterator(); ++it)
	{
		if (is_regular_file(it->path()))
		{
			remove(it->path());
			BOOST_LOG_TRIVIAL(debug) << str(boost::format("删除文件%1%") % it->path().string());
		}
		else if (is_directory(it->path()))
		{
			remove_all(it->path());
			BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("删除目录%1%") % it->path().string());
		}
	}

	BOOST_LOG_TRIVIAL(debug) << str(boost::format("已清空目录%1%") % directory);
}

std::vector<std::string> modis_api::File_operation::read_file_all_lines(const std::string& file_path)
{
	using namespace std;
	using namespace std::filesystem;

	if (!exists(file_path)) throw runtime_error(str(boost::format("file %1% not found!") % file_path));
	vector<string> svec;
	ifstream ifs(file_path);
	copy(istream_iterator<string>(ifs), istream_iterator<string>(), back_inserter(svec));
	return svec;
}

std::vector<std::string> modis_api::File_operation::read_file_all_lines_2(const std::string& file_path)
{
	using namespace std;
	using namespace std::filesystem;

	if (!exists(file_path)) throw runtime_error(str(boost::format("file %1% not found!") % file_path));
	vector<string> svec;
	ifstream ifs(file_path);
	string str;
	while (getline(ifs, str))
		svec.push_back(str);
	return svec;
}

int modis_api::File_operation::write_to_file(const std::string& file_path_str, const std::string& content)
{
	using namespace std;
	using namespace filesystem;
	if (exists(file_path_str))
		remove(file_path_str);
	ofstream ofs(file_path_str);
	ofs << content;
	ofs.flush();
	ofs.close();
	return EXIT_SUCCESS;
}

