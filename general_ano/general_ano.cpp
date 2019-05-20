// general_ano.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Logger_setting.h"
#include "../modis_api/Mat_operation.h"
#include <armadillo>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>

using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace m = modis_api;

const string PROGRAM = "general_ano";
const string VERSION = "1.0";

void init_logger()
{
	modis_api::set_logger_severity(boost::log::trivial::info);
	modis_api::init_console_logger();
	modis_api::init_file_logger("logs//", PROGRAM);
}

void check_node(const YAML::Node& node, const string& attr)
{
	if (!node[attr].IsDefined())
	{
		BOOST_LOG_TRIVIAL(error) << str(boost::format("yml文件中找不到%1%配置项") % attr);
		exit(EXIT_FAILURE);
	}
}

void handle_help_version(const po::variables_map& vm, const po::options_description& desc)
{
	if (vm.count("help"))
	{
		cout << "\nMODIS距平处理程序 v" << VERSION << "\n" << endl;
		cout << desc << endl;
		exit(EXIT_SUCCESS);
	}

	if (vm.count("version"))
	{
		cout << "\nMODIS距平处理程序 v" << VERSION << "\n" << endl;
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char** argv)
{
	init_logger();
	string path_yml;
	YAML::Node node;
	po::variables_map vm;
	po::options_description desc("Usage:");
	desc.add_options()
		("help,h", "显示帮助信息")
		("version,v", "显示版本信息")
		("yml,y", po::value(&path_yml), ".yml文件路径")
		("debug,d", "使用debug模式运行该程序");
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

	handle_help_version(vm, desc);

	if (vm.count("yml") && fs::exists(path_yml))
	{
		if (vm.count("debug"))
		{
			modis_api::set_logger_severity(boost::log::trivial::debug);
			BOOST_LOG_TRIVIAL(debug) << "本程序将以debug模式运行";
		}
		try
		{
			BOOST_LOG_TRIVIAL(debug) << "YAML PATH:" << path_yml;
			node = YAML::LoadFile(path_yml);
		}
		catch (exception& ex)
		{
			BOOST_LOG_TRIVIAL(error) << "解析Yml文件" << path_yml << "失败：";
			BOOST_LOG_TRIVIAL(error) << ex.what();
			return EXIT_FAILURE;
		}
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "找不到" << path_yml << "文件，或.yml文件设置出现问题";
		return EXIT_FAILURE;
	}

	check_node(node, "InputMonthTifFile");
	check_node(node, "InputRefTifFile");
	check_node(node, "OutputImageFile");
	check_node(node, "TempDir");

	//最终输出图片路径
	const fs::path path_output_image = node["OutputImageFile"].as<string>();
	//Temp目录路径
	const fs::path dir_temp = node["TempDir"].as<string>();
	//计算月平均的tif文件列表txt文件路径
	const fs::path path_month_tif_file = node["InputMonthTifFile"].as<string>();
	//计算历史月平均的tif文件列表txt文件路径
	const fs::path path_ref_tif_file = node["InputRefTifFile"].as<string>();

	if (!fs::exists(path_month_tif_file))
	{
		BOOST_LOG_TRIVIAL(error) << "找不到文件：" << path_month_tif_file.string();
		return EXIT_FAILURE;
	}

	if (!fs::exists(path_ref_tif_file))
	{
		BOOST_LOG_TRIVIAL(error) << "找不到文件：" << path_ref_tif_file.string();
		return EXIT_FAILURE;
	}

	vector<string> vec_month_tif_files = m::File_operation::read_file_all_lines(path_month_tif_file.string());
	vector<string> vec_ref_tif_files = m::File_operation::read_file_all_lines(path_ref_tif_file.string());

	if (vec_month_tif_files.empty())
	{
		BOOST_LOG_TRIVIAL(error) << "月份tif文件个数为零，无法进行距平计算，月份tif文件列表位于："
			<< path_month_tif_file.string();
		exit(EXIT_FAILURE);
	}

	if (vec_ref_tif_files.empty())
	{
		BOOST_LOG_TRIVIAL(error) << "历史月份tif文件个数为零，无法进行距平计算，历史月份tif文件列表位于：" <<
			path_ref_tif_file.string();
		exit(EXIT_FAILURE);
	}

	vector<arma::fmat> mats_month;
	vector<arma::fmat> mats_ref;

	auto read_tif = [](const string& p) -> arma::fmat { return *m::Gdal_operation::read_tif_to_fmat(p);  };
	transform(vec_month_tif_files.begin(), vec_month_tif_files.end(), back_inserter(mats_month), read_tif);
	transform(vec_ref_tif_files.begin(), vec_ref_tif_files.end(), back_inserter(mats_ref), read_tif);

	arma::fmat mean_mats_month = *modis_api::Mat_operation::mean_mat_by_each_pixel(mats_month);
	BOOST_LOG_TRIVIAL(debug) << "月份平均值计算完毕，共有" << mats_month.size() << "个文件参与计算";

	arma::fmat mean_mats_ref = *modis_api::Mat_operation::mean_mat_by_each_pixel(mats_ref);
	BOOST_LOG_TRIVIAL(debug) << "历史月份平均值计算完毕，共有" << mats_ref.size() << "个文件参与计算";

	arma::fmat result = mean_mats_month - mean_mats_ref;

	if (fs::exists(path_output_image)) fs::remove(path_output_image);
	fs::copy_file(vec_month_tif_files.front(), path_output_image.string());
	modis_api::Gdal_operation::write_fmat_to_tif(path_output_image.string(), result);
	BOOST_LOG_TRIVIAL(info) << "距平计算完成，结果文件为：" << path_output_image.string();
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
