// proc_MxD05_L2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "../modis_api/Logger_setting.h"
#include <iostream>
#include <string>
#include <vector>
#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"
#include "Preprocess_water.h"

using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

const string PROGRAM = "proc_MxD05_L2";
const string VERSION = "1.0";

void init_logger()
{
	modis_api::set_logger_severity(boost::log::trivial::info);
	modis_api::init_console_logger();
	modis_api::init_file_logger("logs//", PROGRAM);

}

int main(int argc, char** argv)
{
	init_logger();

	std::string yml_path;
	YAML::Node node;
	po::variables_map vm;
	po::options_description desc("Usage:");
	desc.add_options()
		("help,h", "显示帮助信息")
		("version,v", "显示版本信息")
		("yml,y", po::value(&yml_path), ".yml文件路径")
		("debug,d", "使用debug模式运行该程序");
	bool debug_mode = false;
	try
	{
		store(po::command_line_parser(argc, argv).options(desc).run(), vm);
		notify(vm);
	}
	catch (exception& e)
	{
		cerr << e.what();
		return EXIT_FAILURE;
	}

	if (vm.count("help"))
	{
		cout << "\nMODIS气溶胶(MxD04)预处理程序 v" << VERSION << "\n" << endl;
		cout << desc << endl;
		return EXIT_SUCCESS;
	}

	if (vm.count("version"))
	{
		cout << "\nMODIS气溶胶(MxD04)预处理程序 v" << VERSION << "\n" << endl;
		return EXIT_SUCCESS;
	}

	if (vm.count("yml") && fs::exists(yml_path))
	{
		if (vm.count("debug"))
		{
			modis_api::set_logger_severity(boost::log::trivial::debug);
			BOOST_LOG_TRIVIAL(debug) << "本程序将以debug模式运行";
			debug_mode = true;
		}
		try
		{
			BOOST_LOG_TRIVIAL(info) << "YAML PATH:" << yml_path;
			node = YAML::LoadFile(yml_path);
		}
		catch (exception& ex)
		{
			BOOST_LOG_TRIVIAL(error) << "解析Yml文件" << yml_path << "失败：";
			BOOST_LOG_TRIVIAL(error) << ex.what();
			return EXIT_FAILURE;
		}
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "找不到" << yml_path << "文件，或.yml文件设置出现问题";
		return EXIT_FAILURE;
	}

	proc_MxD05_L2::Preprocess_water::preprocess(yml_path, node, debug_mode);
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
