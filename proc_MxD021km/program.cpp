#pragma warning(disable: 4251)

#include "globals.h"
#include "Options_yaml.h"
#include "Preprocess_bt.h"
#include <armadillo>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <gdal_priv.h>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include "../modis_api/Logger_setting.h"
#include "Input_file.h"

using namespace std;
using namespace arma;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

const string VERSION = "1.0";
const string PROGRAM = "proc_MxD021km";

bool global_is_debug;

int main(int argc, char* argv[])
{
	modis_api::init_console_logger();
	modis_api::init_file_logger("logs\\", PROGRAM);
	modis_api::set_logger_severity(boost::log::trivial::info);

	// auto files = proc_MxD021km::Input_file::load("D:\\proc_MxD021km_tif_list.txt");
	// cout << files.size() << endl;
	// for (const auto& file : files)
	// {
	// 	cout << file.bt_hdf_file() << endl;
	// 	cout << file.sza_hdf_file() << endl;
	// 	cout << file.cm_hdf_file() << endl;
	// 	cout << endl;
	// }

	std::string yml_path;
	YAML::Node node;
	po::variables_map vm;
	po::options_description desc("Usage:");
	desc.add_options()
		("help,h", "显示帮助信息")
		("version,v", "show version information")
		("yml,y", po::value(&yml_path), "the path of .yml configuration file")
		("debug,d", "run this program in debug mode");
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
		cout << "\nBT ARCHIVE and PREPROCESS PROGRAM v" << VERSION << "\n" << endl;
		cout << desc << endl;
		return EXIT_SUCCESS;
	}

	if (vm.count("version"))
	{
		cout << "\nBT ARCHIVE and PREPROCESS PROGRAM v" << VERSION << "\n" << endl;
		return EXIT_SUCCESS;
	}

	if (fs::exists(yml_path))
	{
		try
		{
			BOOST_LOG_TRIVIAL(info) << "YAML PATH:" << yml_path;
			node = YAML::LoadFile(yml_path);
			if (vm.count("debug"))
			{
				modis_api::set_logger_severity(boost::log::trivial::debug);
				global_is_debug = true;
			}
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
		BOOST_LOG_TRIVIAL(error) << "找不到Yml文件" << yml_path;
		return EXIT_FAILURE;
	}

	//proc_MxD021km::Preprocess_bt::get_sds_bt(31);
	//proc_MxD021km::Preprocess_bt::get_suffix_bt(31);
	proc_MxD021km::Options_yaml options(node);
	if(stoi(options.band()) == 30)
	{
		cout << "本程序不支持30波段数据的预处理.." << endl;
		exit(EXIT_SUCCESS);
	}
	proc_MxD021km::Preprocess_bt::preprocess(options);

	// if (options.date().length() != 7 && options.date().length() != 8)
	// {
	// 	cout << "Error Date format in " << yml_path << endl;
	// 	cout << "Date is a 7 or 8 string, such as 2018010 or 20180323" << endl;
	// 	return EXIT_FAILURE;
	// }
	//
	// if (!fs::exists(options.workspace_path()) || !fs::is_directory(options.workspace_path()))
	// {
	// 	cout << "错误! 工作空间目录: " << options.workspace_path()
	// 		<< " 不存在或不是一个合法目录.." << endl;
	// 	return EXIT_FAILURE;
	// }
	//
	// // 如归档才检查数据目录
	// if (options.need_archive())
	// {
	// 	if (!fs::exists(options.modis_data_path()) || !fs::is_directory(options.modis_data_path()))
	// 	{
	// 		cout << "错误! MODIS数据目录: " << options.modis_data_path()
	// 			<< " 不存在或不是一个合法目录.." << endl;
	// 		return -1;
	// 	}
	// }

}



