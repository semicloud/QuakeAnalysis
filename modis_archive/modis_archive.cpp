// modis_archive.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"
#include "../modis_api/Hdf_file.h"
#include "../modis_api/Logger_setting.h"
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace fs = boost::filesystem;
namespace po = boost::program_options;

const string PROGRAM = "modis_archive";
const string VERSION = "1.0";
const int PRINT_PATH_THRESHOLD = 20;

inline void init_logger_setting()
{
	modis_api::init_console_logger();
	modis_api::init_file_logger("logs\\", PROGRAM);
	modis_api::set_logger_severity(boost::log::trivial::info);
}
void check_arg(string& workspace_dir, string& data_dir);
void process_arg(int argc, char** argv, string& workspace_dir, string& data_dir, bool& is_copy, bool& is_debug);
void archive(const string& workspace_dir, const string& data_dir, bool is_copy, bool is_debug);

int main(int argc, char** argv)
{
	init_logger_setting();
	string workspace_dir, data_dir;
	bool is_copy = false, is_debug = false;
	process_arg(argc, argv, workspace_dir, data_dir, is_copy, is_debug);
	BOOST_LOG_TRIVIAL(debug) << "解析命令参数：\n" <<
		"\tWorkspace Dir:" << workspace_dir <<
		"\n\tData Dir:" << data_dir <<
		"\n\tis_copy:" << is_copy <<
		"\n\tis_debug:" << is_debug;
	check_arg(workspace_dir, data_dir);
	BOOST_LOG_TRIVIAL(debug) << "Check命令参数：\n" <<
		"\tWorkspace Dir:" << workspace_dir <<
		"\n\tData Dir:" << data_dir;
	archive(workspace_dir, data_dir, is_copy, is_debug);
}

void check_arg(string& workspace_dir, string& data_dir)
{
	if (workspace_dir.at(workspace_dir.size() - 1) != '\\')
	{
		workspace_dir.push_back('\\');
		BOOST_LOG_TRIVIAL(debug) << "MODIS工作目录修改为" << workspace_dir;
	}

	if (!fs::exists(workspace_dir))
	{
		cerr << "错误！找不到MODIS工作目录" << workspace_dir;
		exit(EXIT_FAILURE);
	}

	if (data_dir.at(data_dir.size() - 1) != '\\')
	{
		data_dir.push_back('\\');
		BOOST_LOG_TRIVIAL(debug) << "MODIS数据目录修改为" << data_dir;
	}

	if (!fs::exists(data_dir))
	{
		cerr << "错误！找不到MODIS数据目录" << data_dir;
		exit(EXIT_FAILURE);
	}
}

void process_arg(int argc, char** argv, string& workspace_dir, string& data_dir, bool& is_copy, bool& is_debug)
{
	boost::program_options::options_description desc(PROGRAM + " " + VERSION + "\nUsage");
	desc.add_options()
		("help,h", "显示本程序帮助信息")
		("version,v", "显示本程序版本信息")
		("workspace-dir,w", po::value<string>(&workspace_dir)->required(), "MODIS工作空间目录")
		("data-dir,a", po::value<string>(&data_dir)->required(), "MODIS数据目录")
		("copy,c", "如使用该参数，程序将以复制模式进行归档")
		("debug,d", "如使用该参数，程序将以Debug模式运行");
	po::variables_map vm;

	try
	{
		store(boost::program_options::command_line_parser(argc, argv).options(desc).run(), vm);
		if (vm.count("version"))
		{
			cout << endl;
			cout << PROGRAM << " " << VERSION << endl;
			cout << endl;
			exit(EXIT_SUCCESS);
		}
		if (vm.count("help"))
		{
			cout << endl;
			cout << desc << endl;
			exit(EXIT_SUCCESS);
		}
		// notify函数会报出参数解析异常
		po::notify(vm);
		is_copy = vm.count("copy");
		is_debug = vm.count("debug");
	}
	catch (exception& e)
	{
		cerr << e.what();
		exit(EXIT_FAILURE);
	}
}

void archive(const string& workspace_dir, const string& data_dir, bool is_copy, bool is_debug)
{
	if (is_debug) modis_api::set_logger_severity(boost::log::trivial::debug);
	BOOST_LOG_TRIVIAL(info) << "开始准备归档..";
	BOOST_LOG_TRIVIAL(debug) << "MODIS数据目录：" << data_dir;
	BOOST_LOG_TRIVIAL(debug) << "MODIS工作目录：" << workspace_dir;
	vector<string> hdf_paths;
	for (auto it = fs::recursive_directory_iterator(fs::path(data_dir)); it != fs::recursive_directory_iterator(); ++it)
	{
		if (it->path().filename().extension().string() == ".hdf") hdf_paths.push_back(it->path().string());
	}
	BOOST_LOG_TRIVIAL(info) << "数据目录下共找到" << hdf_paths.size() << "个待归档文件..";
	if (hdf_paths.empty())
	{
		BOOST_LOG_TRIVIAL(info) << "没有要归档的文件，结束..";
		return;
	}
	for (auto it = hdf_paths.cbegin(); it != hdf_paths.cend(); ++it)
	{
		modis_api::Hdf_file hdf_file(*it);
		const string source = *it, dest = hdf_file.get_archive_path(workspace_dir);
		if (fs::exists(dest)) fs::remove(dest);
		fs::path dest_path(dest);
		if (!fs::exists(dest_path.parent_path())) create_directories(dest_path.parent_path());
		fs::copy_file(source, dest);
		if (!is_copy)
			fs::remove(source);
		const int current = static_cast<int>(it - hdf_paths.cbegin()) + 1;
		BOOST_LOG_TRIVIAL(debug) << "源文件：" << source;
		BOOST_LOG_TRIVIAL(debug) << "目标文件：" << dest;
		BOOST_LOG_TRIVIAL(info) << dest_path.filename().string() << "已归档";
		/*if (source.size() >= PRINT_PATH_THRESHOLD && dest.size() >= PRINT_PATH_THRESHOLD)
			BOOST_LOG_TRIVIAL(info) << "[" << source.substr(0, PRINT_PATH_THRESHOLD) << "...] --> [..."
			<< dest.substr(dest.size() - PRINT_PATH_THRESHOLD, dest.size() - 1) << "] "
			<< current << "/" << hdf_paths.size();
		else
			BOOST_LOG_TRIVIAL(info) << "[" << source << "] --> ["
			<< dest << "] " << current << "/" << hdf_paths.size();*/
	}
	BOOST_LOG_TRIVIAL(info) << "归档结束..";
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
