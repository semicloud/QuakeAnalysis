// adsma.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Logger_setting.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "yml_intepreter.h"

namespace gr = boost::gregorian;
namespace fs = boost::filesystem;
namespace m = modis_api;
namespace y = YAML;

inline void init_logger() {
	m::init_console_logger();
	m::init_file_logger("//logs", "adsma");
	m::set_logger_severity(boost::log::trivial::debug);
}
std::string get_doy_date(const gr::date& date);
gr::date get_date(const std::string& date_str);

void generate_mxd11a1_ymls();
void generate_txt_for_general_ano();
void prepare_general_ano_tif(const std::string& dir_data, const unsigned year, const unsigned month, std::vector<std::string>& vec_month, std::vector<std::string>& vec_ref);

int run_programs(const std::string& ymls_folder);

int run_programs(const std::string& ymls_folder)
{
	using namespace std;
	using namespace filesystem;
	path ymls_folder_path(ymls_folder);
	vector<string> yml_files = modis_api::File_operation::get_all_files_by_extension(ymls_folder_path.string(), ".yml");
	for (const string& yml_file : yml_files)
	{
		path yml_file_path(yml_file);
		string file_name = yml_file_path.filename().string();
		BOOST_LOG_TRIVIAL(debug) << "Yml file name: " << file_name;
		vector<string> file_name_components;
		split(file_name_components, file_name, boost::is_any_of("_"));
		const string operation = file_name_components[0];
		const string product = file_name_components[1];
		const string product_type = file_name_components[2];
		if (operation == "pp")
		{
			if (product == "bt")
			{
				const string cmd = str(boost::format("%1% -y %2% -d") % "proc_MxD021km.exe" % yml_file_path.string());
				BOOST_LOG_TRIVIAL(debug) << "CMD: " << cmd;
				int i = system(cmd.c_str());
				assert(i == 0);
			}
			else if (product == "aod")
			{
				const string cmd = str(boost::format("%1% -y %2% -d") % "proc_MxD04_3k.exe" % yml_file_path.string());
				BOOST_LOG_TRIVIAL(debug) << "CMD: " << cmd;
				int i = system(cmd.c_str());
				assert(i == 0);
			}
			else if (product == "wv")
			{
				const string cmd = str(boost::format("%1% -y %2% -d") % "proc_MxD05_L2.exe" % yml_file_path.string());
				BOOST_LOG_TRIVIAL(debug) << "CMD: " << cmd;
				int i = system(cmd.c_str());
				assert(i == 0);

			}
			else if (product == "lst")
			{
				const string cmd = str(boost::format("%1% -y %2% -d") % "proc_MxD11A1.exe" % yml_file_path.string());
				BOOST_LOG_TRIVIAL(debug) << "CMD: " << cmd;
				int i = system(cmd.c_str());
				assert(i == 0);
			}
		}
	}
	return EXIT_SUCCESS;
}


int main()
{
	init_logger();

	/*generate_mxd11a1_ymls();*/
	//generate_txt_for_general_ano();
	const std::string yml = "E:\\CodeWorld\\VSProjects\\QuakeAnalysis\\QuakeAnalysis\\ymlsamples\\adsma.yml";
	process(yml);
	run_programs("D:\\modis_workspace\\generated_ymls");

}

void prepare_general_ano_tif(const std::string& dir_data, const unsigned year, const unsigned month, std::vector<std::string>& vec_month, std::vector<std::string>& vec_ref)
{
	fs::recursive_directory_iterator it(dir_data);
	for (; it != fs::recursive_directory_iterator(); ++it)
	{
		if (it->path().extension() != ".tif") continue;
		std::string file_name = it->path().filename().string();
		unsigned file_year = boost::lexical_cast<unsigned>(file_name.substr(3, 4));
		unsigned file_day = boost::lexical_cast<unsigned>(trim_left_copy_if(file_name.substr(7, 3), boost::is_any_of("0")));
		unsigned file_month = (gr::date(file_year, 1, 1) + gr::days(file_day - 1)).month().as_number();
		BOOST_LOG_TRIVIAL(debug) << "file year:" << file_year << ", file month: " << file_month << " ,file day:" << file_day;
		if (file_year == year && file_month == month)
		{
			vec_month.push_back(it->path().string());
		}
		else if (file_year < year && file_month == month)
		{
			vec_ref.push_back(it->path().string());
		}
	}
}


void generate_txt_for_general_ano()
{
	fs::path dir_output("E:\\CodeWorld\\VSProjects\\QuakeAnalysis\\QuakeAnalysis\\ymlsamples\\");
	fs::path path_txt_month_tif = dir_output / "genenal_ano_month_tif_list.txt";
	fs::path path_txt_ref_tif = dir_output / "general_ano_ref_tif_list.txt";

	fs::path dir_workspace("e:\\modis_workspace_test\\");
	fs::path dir_data = dir_workspace / "Standard" / "MOD11A1" / "\\";
	const unsigned year = 2018, month = 1;
	BOOST_LOG_TRIVIAL(debug) << "data dir: " << dir_data.string();
	std::vector<std::string> vec_month, vec_ref;
	prepare_general_ano_tif(dir_data.string(), year, month, vec_month, vec_ref);

	sort(vec_month.begin(), vec_month.end());
	sort(vec_ref.begin(), vec_ref.end());

	std::ostringstream oss;
	for_each(vec_month.begin(), vec_month.end(), [&oss](std::string& p) {oss << p << std::endl; });
	if (fs::exists(path_txt_month_tif)) fs::remove(path_txt_month_tif);
	std::ofstream ofs(path_txt_month_tif.string());
	if (ofs)
	{
		ofs << oss.str();
		ofs.flush();
		ofs.clear();
		ofs.close();
	}

	if (fs::exists(path_txt_ref_tif)) fs::remove(path_txt_ref_tif);
	oss.clear();
	oss.str(""); // trick
	for_each(vec_ref.begin(), vec_ref.end(), [&oss](std::string& p) {oss << p << std::endl; });
	ofs.open(path_txt_ref_tif.string());
	if (ofs)
	{
		ofs << oss.str();
		ofs.flush();
		ofs.clear();
		ofs.close();
	}
	BOOST_LOG_TRIVIAL(debug) << "txt file geneated..";
}


void generate_mxd11a1_ymls()
{
	const gr::date start(2019, 4, 20), end(2019, 4, 20);
	const std::string workspace_dir = "e:\\modis_workspace_test\\";
	const std::string temp_dir = "e:\\modis_workspace_test\\tmp\\";
	const double min_lon = 110, max_lon = 140, min_lat = 20, max_lat = 50;
	modis_api::File_operation::clear_directory(temp_dir);
	if (!fs::exists(temp_dir)) fs::create_directories(temp_dir);
	std::vector<std::string> yml_paths;

	for (auto it = gr::day_iterator(start); it <= end; ++it)
	{
		const gr::date d = *it;
		const std::string doy_date = get_doy_date(d);
		const std::string year = doy_date.substr(0, 4), day = doy_date.substr(4, 3);
		BOOST_LOG_TRIVIAL(debug) << "year:" << year << ",day:" << day;
		const fs::path hdf_files_dir = fs::path(workspace_dir) / "MOD11A1" / year / day;
		BOOST_LOG_TRIVIAL(debug) << "hdf files path:" << hdf_files_dir;
		if (!exists(hdf_files_dir))
		{
			BOOST_LOG_TRIVIAL(error) << "目录" << hdf_files_dir.string() << "不存在";
			continue;
		}
		std::vector<std::string> hdf_file_paths;
		for (auto dit = fs::directory_iterator(hdf_files_dir); dit != fs::directory_iterator(); ++dit)
		{
			if (dit->path().extension().string() == ".hdf")
				hdf_file_paths.push_back(dit->path().string());
		}
		std::ostringstream oss;
		for (const std::string& hdf_file_path : hdf_file_paths)
		{
			BOOST_LOG_TRIVIAL(debug) << "\t" << hdf_file_path;
			oss << hdf_file_path << std::endl;
		}

		const fs::path hdf_files_txt_path = fs::path(temp_dir) / str(boost::format("MOD11A1_%1%_HDF_LIST.txt") % doy_date);
		BOOST_LOG_TRIVIAL(debug) << "hdf list file:" << hdf_files_txt_path.string();
		if (exists(hdf_files_txt_path)) fs::remove(hdf_files_txt_path);
		std::ofstream ofs(hdf_files_txt_path.string());
		if (ofs)
		{
			ofs << oss.str();
			ofs.flush();
		}
		ofs.clear();
		ofs.close();

		const fs::path doy_temp_dir = fs::path(temp_dir) / doy_date / "\\";
		const fs::path output_image_path = fs::path(workspace_dir) / "SampleData" / "MOD11A1" / str(boost::format("st_%1%.tif") % doy_date);
		BOOST_LOG_TRIVIAL(debug) << "output image path:" << output_image_path.string();
		y::Emitter emt;
		emt << y::BeginMap;
		emt << y::Comment("The text file which contains .hdf files to process") << y::Newline;
		emt << y::Key << "InputHdfFile" << y::Value << hdf_files_txt_path.string() << y::Newline;
		emt << y::Comment("The directory to keep temporary files");
		emt << y::Key << "TempDir" << y::Value << doy_temp_dir.string() << y::Newline;
		emt << y::Comment("minim longitude");
		emt << y::Key << "MinLon" << y::Value << min_lon << y::Newline;
		emt << y::Comment("maximum longitude");
		emt << y::Key << "MaxLon" << y::Value << max_lon << y::Newline;
		emt << y::Comment("minim latitude");
		emt << y::Key << "MinLat" << y::Value << min_lat << y::Newline;
		emt << y::Comment("maximum latitude");
		emt << y::Key << "MaxLat" << y::Value << max_lat << y::Newline;
		emt << y::Comment("SpatialSubsetType");
		emt << y::Key << "SpatialSubsetType" << y::Value << "INPUT_LAT_LONG" << y::Newline;
		emt << y::Comment("ResamplingType");
		emt << y::Key << "ResamplingType" << y::Value << "NEAREST_NEIGHBOR" << y::Newline;
		emt << y::Comment("OutputProjectionType");
		emt << y::Key << "OutputProjectionType" << y::Value << "ER" << y::Newline;
		emt << y::Comment("OutputProjectionParameters");
		emt << y::Key << "OutputProjectionParameters" << y::Value << "6370997.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0" << y::Newline;
		emt << y::Comment("Datum");
		emt << y::Key << "Datum" << y::Value << "NoDatum" << y::Newline;
		emt << y::Comment("OutputPixelSize");
		emt << y::Key << "OutputPixelSize" << y::Value << 1000 << y::Newline;
		emt << y::Comment("The final result file we want");
		emt << y::Key << "OutputImageFile" << y::Value << output_image_path.string();
		emt << y::EndMap;

		const fs::path yml_path = fs::path(temp_dir) / str(boost::format("MOD11A1_%1%.yml") % doy_date);
		ofs.open(yml_path.string());
		if (ofs)
		{
			ofs << emt.c_str();
			ofs.flush();
		}
		ofs.clear();
		ofs.close();

		yml_paths.push_back(yml_path.string());
	}

	std::cout << "I am ready to run proc_MxD11A1.exe..";
	Sleep(5000);

	for (const std::string& yml_path : yml_paths)
	{
		const std::string cmd = (fs::current_path() / "proc_MxD11A1.exe").string() + " --yml " + yml_path + " -d";
		std::cout << cmd << std::endl;
		system(cmd.c_str());
		std::cout << yml_path << " is executed." << std::endl;
		Sleep(500);
	}
}


std::string get_doy_date(const gr::date& date)
{
	const auto first_day = boost::gregorian::date(date.year(), 1, 1);
	const auto day_span = (date - first_day).days() + 1;
	std::string doy = "";
	if (day_span < 10)
		doy = "00" + std::to_string(day_span);
	else if (day_span >= 10 && day_span <= 99)
		doy = "0" + std::to_string(day_span);
	else
		doy = std::to_string(day_span);
	return boost::lexical_cast<std::string>(date.year()) + doy;
}

gr::date get_date(const std::string& date_str)
{
	BOOST_LOG_TRIVIAL(debug) << "convert date_str " << date_str << " to date..";
	const auto year = boost::lexical_cast<int>(date_str.substr(0, 4));
	boost::gregorian::date ans;
	if (date_str.length() == 7)
	{
		const auto doy = boost::lexical_cast<int>(trim_left_copy_if(date_str.substr(4, 3), boost::is_any_of("0")));
		BOOST_LOG_TRIVIAL(debug) << "doy: " << doy;
		ans = boost::gregorian::date(year, 1, 1) + boost::gregorian::days(doy);
	}

	if (date_str.length() == 8)
	{
		const auto month = boost::lexical_cast<int>(trim_left_copy_if(date_str.substr(4, 2), boost::is_any_of("0")));
		const auto day = boost::lexical_cast<int>(trim_left_copy_if(date_str.substr(6, 2), boost::is_any_of("0")));
		BOOST_LOG_TRIVIAL(debug) << "month: " << month << ", day: " << day;
		ans = boost::gregorian::date(year, month, day);
	}
	BOOST_LOG_TRIVIAL(debug) << "date_str " << date_str << " to date object " << to_iso_string(ans);
	return ans;
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
