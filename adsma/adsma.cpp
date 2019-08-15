// adsma.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Logger_setting.h"
#include "Adsma_settings.h"
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

inline void init_logger() {
	modis_api::init_console_logger();
	modis_api::init_file_logger("//logs", "adsma");
	modis_api::set_logger_severity(boost::log::trivial::debug);
}

std::string get_doy_date(const boost::gregorian::date& date);
boost::gregorian::date get_date(const std::string& date_str);

void generate_mxd11a1_ymls();
void generate_txt_for_general_ano();
void prepare_general_ano_tif(const std::string& dir_data, const unsigned year, const unsigned month, std::vector<std::string>& vec_month, std::vector<std::string>& vec_ref);

int run_programs(const std::string& ymls_folder);

int run_programs(const std::string& ymls_folder)
{
	using namespace std;
	using namespace filesystem;
	using namespace adsma::settings;
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
			if (product == boost::to_lower_copy(BT_NAME))
			{
				const string cmd = str(boost::format("%1% -y %2% -d") % "proc_MxD021km.exe" % yml_file_path.string());
				BOOST_LOG_TRIVIAL(debug) << "CMD: " << cmd;
				int i = system(cmd.c_str());
				assert(i == 0);
			}
			else if (product == boost::to_lower_copy(AOD_NAME))
			{
				const string cmd = str(boost::format("%1% -y %2% -d") % "proc_MxD04_3k.exe" % yml_file_path.string());
				BOOST_LOG_TRIVIAL(debug) << "CMD: " << cmd;
				int i = system(cmd.c_str());
				assert(i == 0);
			}
			else if (product == boost::to_lower_copy(WV_NAME))
			{
				const string cmd = str(boost::format("%1% -y %2% -d") % "proc_MxD05_L2.exe" % yml_file_path.string());
				BOOST_LOG_TRIVIAL(debug) << "CMD: " << cmd;
				int i = system(cmd.c_str());
				assert(i == 0);

			}
			else if (product == boost::to_lower_copy(LST_NAME))
			{
				const string cmd = str(boost::format("%1% -y %2% -d") % "proc_MxD11A1.exe" % yml_file_path.string());
				BOOST_LOG_TRIVIAL(debug) << "CMD: " << cmd;
				int i = system(cmd.c_str());
				assert(i == 0);
			}
		}
		else
		{
			// 涡度处理
			if(file_name.substr(0,2) == "ef") 
			{
				const string cmd = (boost::format("%1% -y %2%") % "eddy_field_ano.exe" % yml_file_path.string()).str();
				BOOST_LOG_TRIVIAL(debug) << "CMD: " << cmd;
				int i = system(cmd.c_str());
			}

			if(file_name.substr(0,2) == "pl")
			{
				const string cmd = (boost::format("%1% -y %2%") % "modis_plot.exe" % yml_file_path.string()).str();
				int i = system(cmd.c_str());
			}
		}
	}
	return EXIT_SUCCESS;
}


int main()
{
	using namespace std;
	init_logger();
	/*generate_mxd11a1_ymls();*/
	//generate_txt_for_general_ano();
	const std::string yml = "E:\\CodeWorld\\VSProjects\\QuakeAnalysis\\QuakeAnalysis\\ymlsamples\\adsma.yml";
	process(yml);
	// run_programs("D:\\modis_workspace\\generated_ymls");
	//auto years = get_years_of_product("D:\\modis_workspace", "MOD", "LST");
	/*for (unsigned year : years)
	{
		cout << year << " ";
	}*/
}

void prepare_general_ano_tif(const std::string& dir_data, const unsigned year, const unsigned month, std::vector<std::string>& vec_month, std::vector<std::string>& vec_ref)
{
	using namespace std;
	using namespace filesystem;
	recursive_directory_iterator it(dir_data);
	for (; it != recursive_directory_iterator(); ++it)
	{
		if (it->path().extension() != ".tif") continue;
		std::string file_name = it->path().filename().string();
		unsigned file_year = boost::lexical_cast<unsigned>(file_name.substr(3, 4));
		unsigned file_day = boost::lexical_cast<unsigned>(trim_left_copy_if(file_name.substr(7, 3), boost::is_any_of("0")));
		unsigned file_month = (boost::gregorian::date(file_year, 1, 1) + boost::gregorian::days(file_day - 1)).month().as_number();
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
	using namespace std;
	using namespace filesystem;
	path dir_output("E:\\CodeWorld\\VSProjects\\QuakeAnalysis\\QuakeAnalysis\\ymlsamples\\");
	path path_txt_month_tif = dir_output / "genenal_ano_month_tif_list.txt";
	path path_txt_ref_tif = dir_output / "general_ano_ref_tif_list.txt";

	path dir_workspace("e:\\modis_workspace_test\\");
	path dir_data = dir_workspace / "Standard" / "MOD11A1" / "\\";
	const unsigned year = 2018, month = 1;
	BOOST_LOG_TRIVIAL(debug) << "data dir: " << dir_data.string();
	std::vector<std::string> vec_month, vec_ref;
	prepare_general_ano_tif(dir_data.string(), year, month, vec_month, vec_ref);

	sort(vec_month.begin(), vec_month.end());
	sort(vec_ref.begin(), vec_ref.end());

	std::ostringstream oss;
	for_each(vec_month.begin(), vec_month.end(), [&oss](std::string& p) {oss << p << std::endl; });
	if (exists(path_txt_month_tif)) remove(path_txt_month_tif);
	std::ofstream ofs(path_txt_month_tif.string());
	if (ofs)
	{
		ofs << oss.str();
		ofs.flush();
		ofs.clear();
		ofs.close();
	}

	if (exists(path_txt_ref_tif)) remove(path_txt_ref_tif);
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
	using namespace std;
	using namespace boost::gregorian;
	using namespace std::filesystem;
	using namespace YAML;
	const date start(2019, 4, 20), end(2019, 4, 20);
	const std::string workspace_dir = "e:\\modis_workspace_test\\";
	const std::string temp_dir = "e:\\modis_workspace_test\\tmp\\";
	const double min_lon = 110, max_lon = 140, min_lat = 20, max_lat = 50;
	modis_api::File_operation::clear_directory(temp_dir);
	if (!exists(temp_dir)) create_directories(temp_dir);
	std::vector<std::string> yml_paths;

	for (auto it = day_iterator(start); it <= end; ++it)
	{
		const date d = *it;
		const std::string doy_date = get_doy_date(d);
		const std::string year = doy_date.substr(0, 4), day = doy_date.substr(4, 3);
		BOOST_LOG_TRIVIAL(debug) << "year:" << year << ",day:" << day;
		const path hdf_files_dir = path(workspace_dir) / "MOD11A1" / year / day;
		BOOST_LOG_TRIVIAL(debug) << "hdf files path:" << hdf_files_dir;
		if (!exists(hdf_files_dir))
		{
			BOOST_LOG_TRIVIAL(error) << "目录" << hdf_files_dir.string() << "不存在";
			continue;
		}
		std::vector<std::string> hdf_file_paths;
		for (auto dit = directory_iterator(hdf_files_dir); dit != directory_iterator(); ++dit)
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

		const path hdf_files_txt_path = path(temp_dir) / str(boost::format("MOD11A1_%1%_HDF_LIST.txt") % doy_date);
		BOOST_LOG_TRIVIAL(debug) << "hdf list file:" << hdf_files_txt_path.string();
		if (exists(hdf_files_txt_path)) remove(hdf_files_txt_path);
		std::ofstream ofs(hdf_files_txt_path.string());
		if (ofs)
		{
			ofs << oss.str();
			ofs.flush();
		}
		ofs.clear();
		ofs.close();

		const path doy_temp_dir = path(temp_dir) / doy_date / "\\";
		const path output_image_path = path(workspace_dir) / "SampleData" / "MOD11A1" / str(boost::format("st_%1%.tif") % doy_date);
		BOOST_LOG_TRIVIAL(debug) << "output image path:" << output_image_path.string();
		Emitter emt;
		emt << BeginMap;
		emt << Comment("The text file which contains .hdf files to process") << Newline;
		emt << Key << "InputHdfFile" << Value << hdf_files_txt_path.string() << Newline;
		emt << Comment("The directory to keep temporary files");
		emt << Key << "TempDir" << Value << doy_temp_dir.string() << Newline;
		emt << Comment("minim longitude");
		emt << Key << "MinLon" << Value << min_lon << Newline;
		emt << Comment("maximum longitude");
		emt << Key << "MaxLon" << Value << max_lon << Newline;
		emt << Comment("minim latitude");
		emt << Key << "MinLat" << Value << min_lat << Newline;
		emt << Comment("maximum latitude");
		emt << Key << "MaxLat" << Value << max_lat << Newline;
		emt << Comment("SpatialSubsetType");
		emt << Key << "SpatialSubsetType" << Value << "INPUT_LAT_LONG" << Newline;
		emt << Comment("ResamplingType");
		emt << Key << "ResamplingType" << Value << "NEAREST_NEIGHBOR" << Newline;
		emt << Comment("OutputProjectionType");
		emt << Key << "OutputProjectionType" << Value << "ER" << Newline;
		emt << Comment("OutputProjectionParameters");
		emt << Key << "OutputProjectionParameters" << Value << "6370997.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0" << Newline;
		emt << Comment("Datum");
		emt << Key << "Datum" << Value << "NoDatum" << Newline;
		emt << Comment("OutputPixelSize");
		emt << Key << "OutputPixelSize" << Value << 1000 << Newline;
		emt << Comment("The final result file we want");
		emt << Key << "OutputImageFile" << Value << output_image_path.string();
		emt << EndMap;

		const path yml_path = path(temp_dir) / str(boost::format("MOD11A1_%1%.yml") % doy_date);
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
		const std::string cmd = (current_path() / "proc_MxD11A1.exe").string() + " --yml " + yml_path + " -d";
		std::cout << cmd << std::endl;
		system(cmd.c_str());
		std::cout << yml_path << " is executed." << std::endl;
		Sleep(500);
	}
}


std::string get_doy_date(const boost::gregorian::date& date)
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

boost::gregorian::date get_date(const std::string& date_str)
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
