#pragma warning(disable: 4251)

#include "../modis_api/Logger_setting.h"
#include "globals.h"
#include "Input_file.h"
#include "yamlArgs.h"
#include <armadillo>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <gdal_priv.h>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <filesystem>
#include "mxd021km_conversion.h"

const std::string VERSION = "1.0";
const std::string PROGRAM = "proc_MxD021km";

bool global_is_debug;

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace filesystem;
	using namespace modis_api;
	using namespace YAML;

	init_console_logger();
	init_file_logger("logs\\", PROGRAM);
	set_logger_severity(boost::log::trivial::info);

	string yml_path;
	Node node;
	boost::program_options::variables_map vm;
	boost::program_options::options_description desc("Usage:");
	desc.add_options()
		("help,h", "显示帮助信息")
		("version,v", "show version information")
		("yml,y", boost::program_options::value(&yml_path), "the path of .yml configuration file")
		("debug,d", "run this program in debug mode");
	try
	{
		store(boost::program_options::command_line_parser(argc, argv).options(desc).run(), vm);
		notify(vm);
	}
	catch (std::exception& e)
	{
		cerr << e.what();
		return EXIT_FAILURE;
	}

	if (vm.count("help"))
	{
		cout << "\nBT ARCHIVE and PREPROCESS PROGRAM v" << VERSION << "\n" << endl;
		cout << desc << endl;
		return EXIT_SUCCESS;
	}

	if (vm.count("version"))
	{
		cout << "\nBT ARCHIVE and PREPROCESS PROGRAM v" << VERSION << "\n" << endl;
		return EXIT_SUCCESS;
	}

	if (exists(yml_path))
	{
		try
		{
			BOOST_LOG_TRIVIAL(info) << "YAML PATH:" << yml_path;
			node = YAML::LoadFile(yml_path);
			if (vm.count("debug"))
			{
				set_logger_severity(boost::log::trivial::debug);
				global_is_debug = true;
			}
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
		BOOST_LOG_TRIVIAL(error) << "找不到Yml文件" << yml_path;
		return EXIT_FAILURE;
	}

	adsma::yamlArgs options(node);
	if(options.band() == 30)
	{
		cout << "本程序不支持30波段数据的预处理.." << endl;
		exit(EXIT_SUCCESS);
	}
	adsma::mxd021km_conversion::preprocess(options);
}



