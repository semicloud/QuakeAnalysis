// proc_MxD04_3k.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Heg_utils.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Logger_setting.h"
#include "Preprocess_aerosol.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <filesystem>
#include <string>
#include <yaml-cpp/yaml.h>

const std::string PROGRAM = "proc_MxD04_3k";
const std::string VERSION = "1.0";

inline void init_logger_setting()
{
	modis_api::init_console_logger();
	modis_api::init_file_logger("logs\\", PROGRAM);
	modis_api::set_logger_severity(boost::log::trivial::info);
}

int main(int argc, char** argv)
{
	namespace fs = std::filesystem;
	namespace po = boost::program_options;
	// MXD 03 04的数据文件可能不匹配
	init_logger_setting();

	// 测试代码，以后可用于测试，模块稳定后删除
	//std::string path = "D:\\modis_workspace\\MOD04_3K\\2018\\001\\MOD04_3K.A2018001.0315.061.2018003202246.hdf";
	//double a = 0, b = 0, c = 0, d = 0;
	//modis_api::Gdal_operation::read_geo_bound(path, std::filesystem::path(path).parent_path(), a, b, c, d);
	//std::cout << a << std::endl;
	//std::cout << b << std::endl;
	//std::cout << c << std::endl;
	//std::cout << d << std::endl;
	//std::cout << "----------------" << std::endl;
	//modis_api::Gdal_operation::read_geo_bound_py_h5(path, std::filesystem::path(path).parent_path().string(), a, b, c, d);
	//std::cout << a << std::endl;
	//std::cout << b << std::endl;
	//std::cout << c << std::endl;
	//std::cout << d << std::endl;

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
			debug_mode = true;
			BOOST_LOG_TRIVIAL(debug) << "本程序将以debug模式运行";
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

	proc_MxD04_3k::Preprocess_aerosol::preprocess(yml_path, node, debug_mode);
}
