// proc_MxD11A1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Logger_setting.h"
#include "Preprocess_st.h"
#include "yaml-cpp/yaml.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

using namespace std;

const string PROGRAM = "proc_MxD11A1";
const string VERSION = "1.0";

inline void init_logger_setting()
{
	modis_api::init_console_logger();
	modis_api::init_file_logger("logs\\", PROGRAM);
	modis_api::set_logger_severity(boost::log::trivial::info);
}

int main(int argc, char** argv)
{
	init_logger_setting();

	string yml_path;
	YAML::Node node;
	boost::program_options::variables_map vm;
	boost::program_options::options_description desc("Usage:");
	desc.add_options()
		("help,h", "显示帮助信息")
		("version,v", "显示版本信息")
		("yml,y", boost::program_options::value(&yml_path), ".yml文件路径")
		("debug,d", "使用debug模式运行该程序");
	try
	{
		store(boost::program_options::command_line_parser(argc, argv).options(desc).run(), vm);
		notify(vm);
	}
	catch (exception& e)
	{
		cerr << e.what();
		return EXIT_FAILURE;
	}

	if (vm.count("help"))
	{
		cout << "\nMODIS地表温度(MxD11A1)预处理程序 v" << VERSION << "\n" << endl;
		cout << desc << endl;
		return EXIT_SUCCESS;
	}

	if (vm.count("version"))
	{
		cout << "\nMODIS地表温度(MxD11A1)预处理程序 v" << VERSION << "\n" << endl;
		return EXIT_SUCCESS;
	}

	if (vm.count("yml") && boost::filesystem::exists(yml_path))
	{
		if (vm.count("debug"))
		{
			modis_api::set_logger_severity(boost::log::trivial::debug);
			BOOST_LOG_TRIVIAL(debug) << "本程序将以debug模式运行";
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

	proc_MxD11A1::Preprocess_st::preprocess(yml_path, node);
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
