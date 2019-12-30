// proc_jplg.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "../gdal_lib/gdal_lib.h"
#include "../modis_api/Logger_setting.h"
#include "JplgItem.h"
#include "proc_jplg.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>
#include <vector>

const std::string version = "1.0";

int main(int argc, char** argv)
{
	std::string in_file{};
	std::string out_dir{};
	bool is_debug = false;
	bool err = false;
	const bool b = proc_jplg::process_command_line(argc, argv, in_file, out_dir, is_debug, err);
	if (err) return EXIT_FAILURE;
	if (!b) return EXIT_SUCCESS;
	modis_api::init_console_logger();
	modis_api::set_logger_severity(is_debug ? boost::log::trivial::debug : boost::log::trivial::info);
	if (!std::filesystem::exists(in_file))
	{
		std::cerr << "Error. Input file " << in_file << " not found!\n";
		return EXIT_FAILURE;
	}
	if (!std::filesystem::exists(out_dir))
	{
		BOOST_LOG_TRIVIAL(debug) << "Output dir " << out_dir << " does not exist, create it..";
		std::filesystem::create_directories(out_dir);
	}
	const int ans = proc_jplg::process_dplg(in_file, out_dir, is_debug);
	assert(ans == EXIT_SUCCESS);
	return 0;
}

bool proc_jplg::process_command_line(int argc, char** argv,
	std::string& in_file,
	std::string& out_dir, bool& debug, bool& has_err)
{
	try
	{
		boost::program_options::options_description desc("Usage");
		desc.add_options()
			("help,h", "show help message")
			("version,v", "show version")
			("input-file,i", boost::program_options::value<std::string>(&in_file)->required(), "input JPLG file.")
			("output-dir,o", boost::program_options::value<std::string>(&out_dir)->required(), "output DIRECTORY")
			("debug,d", boost::program_options::bool_switch()->default_value(false), "run in debug node");
		boost::program_options::variables_map vm;
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
		if (vm.count("help"))
		{
			std::cout << desc << "\n";
			return false;
		}
		if (vm.count("version"))
		{
			std::cout << "JPLG preprocess program version 1.0\n" << desc << "\n";
			return false;
		}
		if (vm["debug"].as<bool>())
		{
			debug = true;
			BOOST_LOG_TRIVIAL(debug) << "this program will running in DEBUG mode..";
		}
		boost::program_options::notify(vm);
	}
	catch (std::exception& e)
	{
		std::cerr << "Error." << e.what() << "\n";
		has_err = true;
		return false;
	}
	return true;
}

int proc_jplg::process_dplg(const std::string& in_file, const std::string& out_dir, bool is_debug)
{
	const std::filesystem::path dplg_file_path(in_file);
	const std::filesystem::path output_folder_path(out_dir);
	BOOST_LOG_TRIVIAL(debug) << "JplgFile: " << dplg_file_path;
	BOOST_LOG_TRIVIAL(debug) << "OutputPath: " << output_folder_path;
	const std::vector<JplgItem> items = JplgItem::load_items(dplg_file_path.string());
	assert(!items.empty());
	const double ndv = 0.0;
	const size_t bn = 1;
	for (auto p = items.cbegin(); p != items.cend() - 1; ++p)
	{
		const JplgItem&  item = *p;
		std::optional<arma::fmat> optional_mat = item.mat();
		boost::gregorian::date d{ static_cast<unsigned short>(item.year()), static_cast<unsigned short>(item.month()), static_cast<unsigned short>(item.day()) };
		std::string file_name{ (boost::format("JPLG_%1%_%2$03d_%3$02d.tif") % d.year() % d.day_of_year() % item.hour()).str() };
		std::filesystem::path output_file_path = output_folder_path / std::to_string(d.year()) / (boost::format("%1$03d") % d.day_of_year()).str() / file_name;
		if (!std::filesystem::exists(output_file_path.parent_path()))
			std::filesystem::create_directories(output_file_path.parent_path());
		if (std::filesystem::exists(output_file_path))
			std::filesystem::remove(output_file_path);
		arma::u32_mat um = arma::conv_to<arma::u32_mat>::from(*optional_mat);
		std::vector<unsigned> vec = arma::conv_to<std::vector<unsigned>>::from(um.as_row());
		std::unique_ptr<unsigned, std::default_delete<unsigned[]>> arr{ new unsigned[vec.size()], std::default_delete<unsigned[]>() };
		std::copy(vec.begin(), vec.end(), arr.get());
		const auto ans = gdal_lib::create_tif<unsigned>(output_file_path.string(), arr.get(), static_cast<unsigned>(ndv), item.mat()->n_cols, item.mat()->n_rows, bn,
			gdal_lib::get_wgs84_proj(), gdal_lib::get_default_geo_trans().get(), gdal_lib::tif_options_for_grey());
		if (!ans)
			BOOST_LOG_TRIVIAL(info) << "save " << output_file_path.string() << " success";
		else
			BOOST_LOG_TRIVIAL(error) << "save " << output_file_path.string() << " failed";
	}
	return EXIT_SUCCESS;
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