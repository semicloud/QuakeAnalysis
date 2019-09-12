// proc_MxD05_L2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Logger_setting.h"
#include "boost/program_options.hpp"
#include "Preprocess_water.h"
#include <iostream>
#include <filesystem>
#include <string>

const std::string PROGRAM = "proc_MxD05_L2";
const std::string VERSION = "1.0";

void init_logger()
{
	modis_api::set_logger_severity(boost::log::trivial::info);
	modis_api::init_console_logger();
	modis_api::init_file_logger("logs//", PROGRAM);
}

int main(int argc, char** argv)
{
	namespace fs = std::filesystem;
	namespace po = boost::program_options;
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
	catch (std::exception& e)
	{
		std::cerr << e.what();
		return EXIT_FAILURE;
	}

	if (vm.count("help"))
	{
		std::cout << "\nMODIS气溶胶(MxD04)预处理程序 v" << VERSION << "\n" << std::endl;
		std::cout << desc << std::endl;
		return EXIT_SUCCESS;
	}

	if (vm.count("version"))
	{
		std::cout << "\nMODIS气溶胶(MxD04)预处理程序 v" << VERSION << "\n" << std::endl;
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
		catch (std::exception& ex)
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

