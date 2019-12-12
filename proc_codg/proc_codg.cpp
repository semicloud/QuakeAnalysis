// proc_codg.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Logger_setting.h"
#include "CodgItem.h"
#include "proc_codg.h"
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>
#include <vector>
#include <yaml-cpp/yaml.h>

const std::string version = "1.0";

int main(int argc, char** argv)
{
	using namespace std;
	using namespace filesystem;
	using namespace boost::program_options;
	using namespace boost::log::trivial;
	modis_api::init_console_logger();
	modis_api::set_logger_severity(debug);
	/*std::cout << "Hello World!\n";
	vector<CodgItem> codg_items = CodgItem::load_items("CODG1950.17I");
	for (CodgItem& item : codg_items)
	{
		cout << item.year() << ", " << item.month() << ", " << item.day() << ", " << item.hour() << endl;
	}*/
	string yml_path;
	bool is_debug = false;
	variables_map vm;
	options_description desc("Usage:");
	desc.add_options()
		("help,h", "显示帮助信息")
		("version,v", "显示版本信息")
		("yml,y", value(&yml_path), "yml文件路径")
		("debug,d", "以Debug模式运行本程序");
	try
	{
		store(command_line_parser(argc, argv).options(desc).run(), vm);
		notify(vm);
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}

	if (vm.count("debug"))
	{
		is_debug = true;
	}

	if (vm.count("help") || vm.count("version"))
	{
		cout << "CODG Preprocess Program, Version: " << version << endl;
		cout << desc << endl;
		return EXIT_SUCCESS;
	}

	if (!exists(yml_path))
	{
		cerr << "None of -y or --yml arguments found!" << endl;
		return EXIT_FAILURE;
	}

	const int ans = proc_codg::process_codg(yml_path, is_debug);

	assert(ans == EXIT_SUCCESS);
	return 0;
}

int proc_codg::process_codg(const std::string& yml_path_str, bool is_debug)
{
	const std::optional<YAML::Node> node = load_variables(yml_path_str);
	if (!node.has_value())
		return EXIT_FAILURE;
	if (!check_variables(*node))
		return EXIT_FAILURE;
	const std::filesystem::path yml_path(yml_path_str);
	const std::filesystem::path tmp_folder_path((*node)["TmpPath"].as<std::string>());
	const std::filesystem::path codg_file_path((*node)["CodgFile"].as<std::string>());
	const std::filesystem::path output_folder_path((*node)["OutputPath"].as<std::string>());
	BOOST_LOG_TRIVIAL(debug) << "TmpPath: " << tmp_folder_path;
	BOOST_LOG_TRIVIAL(debug) << "CodgFilePath: " << codg_file_path;
	BOOST_LOG_TRIVIAL(debug) << "OutputPath: " << output_folder_path;
	if (!std::filesystem::exists(tmp_folder_path))
		std::filesystem::create_directories(tmp_folder_path);
	if (!exists(output_folder_path))
		std::filesystem::create_directories(output_folder_path);
	const std::vector<CodgItem> items = CodgItem::load_items(codg_file_path.string());
	assert(!items.empty());
	std::unique_ptr<double> geo_trans{ new  double[6]{ -179.5, 5.0, 0.0, 89.5, 0.0, -2.5 } };
	const std::string projection = "+proj=longlat +datum=WGS84 +no_defs";
	for (const CodgItem& item : items)
	{
		std::optional<arma::fmat> optional_mat = item.mat();
		boost::gregorian::date d{ static_cast<unsigned short>(item.year()), static_cast<unsigned short>(item.month()), static_cast<unsigned short>(item.day()) };
		std::string file_name{ (boost::format("CODG_%1%_%2$03d_%3$02d.tif") % d.year() % d.day_of_year() % item.hour()).str() };
		std::filesystem::path output_file_path = output_folder_path / std::to_string(d.year()) / (boost::format("%1$03d") % d.day_of_year()).str() / file_name;
		if (!std::filesystem::exists(output_file_path.parent_path()))
			std::filesystem::create_directories(output_file_path.parent_path());
		if (std::filesystem::exists(output_file_path))
			std::filesystem::remove(output_file_path);
		modis_api::Gdal_operation::create_tif(output_file_path.string(), geo_trans.get(), projection, *item.mat());
	}
	return EXIT_SUCCESS;
}

std::optional<YAML::Node> proc_codg::load_variables(const std::string& yml_path)
{
	using namespace std;
	using namespace YAML;
	optional<Node> optional_node;
	try
	{
		optional_node = LoadFile(yml_path);
	}
	catch (exception& e)
	{
		cerr << "解析yml文件出错，错误信息为：" << e.what() << endl;
	}
	return optional_node;
}

bool proc_codg::check_variables(const YAML::Node& node)
{
	using namespace std;
	vector<string> names{ "CodgFile", "TmpPath","OutputPath" };
	vector<string> no_exist_names;
	for_each(names.begin(), names.end(), [&node, &no_exist_names](const string& name)
	{
		if (!node[name].IsDefined()) no_exist_names.push_back(name);
	});
	if (!no_exist_names.empty())
	{
		cerr << "未找到配置项：" << boost::join(no_exist_names, ", ") << endl;
		return false;
	}
	return true;
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
