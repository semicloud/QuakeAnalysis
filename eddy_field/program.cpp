#include "..//modis_api//File_operation.h"
#include "..//modis_api//Logger_setting.h"
#include "Eddy_field.h"
#include "Eddy_field_options_yaml.h"
#include "Eddy_ref.h"
#include <boost/filesystem.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>


using namespace std;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

const string PROGRAM = "eddy_field";
const string VERSION = "1.0";

void check_data(const Eddy_field_options_yaml&);

int main(int argc, char* argv[])
{
	//BOOST_LOG_NAMED_SCOPE("Scope");
	modis_api::init_console_logger();
	modis_api::init_file_logger("logs\\", PROGRAM);
	modis_api::set_logger_severity(boost::log::trivial::info);

	string yml_path;
	po::options_description desc("Usage: ");
	desc.add_options()("help,h", "展示帮助信息")
		("yml,y", po::value<string>(&yml_path), "指定yml配置文件")
		("debug,d", "以debug模式运行本程序")
		("version,v", "显示版本信息");
	po::variables_map vm;

	try
	{
		store(parse_command_line(argc, argv, desc), vm);
		vm.notify();
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		return EXIT_FAILURE;
	}

	if (vm.count("help"))
	{
		cout << desc << endl;
		return EXIT_SUCCESS;
	}

	if (vm.count("version"))
	{
		cout << PROGRAM + " " + VERSION << endl;
		return EXIT_SUCCESS;
	}

	if (vm.count("yml"))
	{
		yml_path = vm["yml"].as<string>();
		if (!fs::exists(yml_path))
		{
			BOOST_LOG_TRIVIAL(error) << "找不到YML配置文件：" << yml_path;
			return EXIT_FAILURE;
		}
		BOOST_LOG_TRIVIAL(info) << "YML PATH: " << yml_path;
		if (vm.count("debug"))
		{
			modis_api::set_logger_severity(boost::log::trivial::debug);
			BOOST_LOG_TRIVIAL(debug) << "程序将以debug模式运行..";
		}
	}

	auto options_yaml = Eddy_field_options_yaml(yml_path);

	check_data(options_yaml);

	// 方法1
	if (options_yaml.method() == 1)
	{
		// 需要计算背景场
		if (options_yaml.need_compute_ref())
		{
			Eddy_ref::compute_eddy_ref_m1(options_yaml);
		}
		// 需要计算涡度
		if (options_yaml.need_compute_eddy_field())
		{
			Eddy_field::compute_eddy_field_m1(options_yaml);
		}
	}

	// 方法2
	if (options_yaml.method() == 2)
	{
		// 需要计算背景场
		if (options_yaml.need_compute_ref())
		{
			Eddy_ref::compute_eddy_ref_m2(options_yaml);
		}
		// 需要计算涡度
		if (options_yaml.need_compute_eddy_field())
		{
			Eddy_field::compute_eddy_field_m2(options_yaml);
		}
	}


	return 0;
}

/**
 * \brief 检查输入的.yml文件是否合法，不合法则给出提示，直接退出程序
 */
void check_data(const Eddy_field_options_yaml& options)
{
	// 不存在workspacefolder，非法！
	if (!fs::exists(options.modis_workspace_folder()))
	{
		cerr << "MODIS workspace folder " << options.modis_workspace_folder() << " not found!";
		exit(EXIT_FAILURE);
	}
	// 方法不是1或2，非法！
	if (options.method() != 1 && options.method() != 2)
	{
		cerr << "method must be 1 or 2!";
		exit(EXIT_FAILURE);
	}
	if (options.need_compute_eddy_field())
	{
		// 如果计算涡度，则必须给出输入tif文件
		if (!fs::exists(options.input_image_file()))
		{
			cerr << "input image file " << options.input_image_file() << " not found!";
			exit(EXIT_FAILURE);
		}
	}
	if (options.need_compute_ref())
	{
		// 如果生成影响场，support_ref_tif_file必须存在
		if (!fs::exists(options.support_ref_tif_file()))
		{
			cerr << "reference tif list file " << options.support_ref_tif_file() << " not found!";
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		// 如不生成影响场，则ref_image_file必须存在
		if (!fs::exists(options.ref_image_file()))
		{
			cerr << "ref image file " << options.ref_image_file() << " not found!";
			exit(EXIT_FAILURE);
		}
	}
}
