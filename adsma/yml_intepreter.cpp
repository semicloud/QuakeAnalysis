#include "pch.h"
#include "yml_intepreter.h"
#include "../modis_api/Date_utils.h"
#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <vector>
#include <optional>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "../modis_api/File_operation.h"

std::optional<YAML::Node> load_yml(const std::string& yml_path_str)
{
	using namespace std;
	assert(std::filesystem::exists(yml_path_str));
	try
	{
		YAML::Node node = YAML::LoadFile(yml_path_str);
		return optional<YAML::Node>(node);
	}
	catch (std::exception& e)
	{
		cerr << "parse yml error! message: " << e.what() << endl;
		std::exit(EXIT_FAILURE);
	}
}

int process(const std::string& yml_path_str)
{
	using namespace std;
	using namespace std::filesystem;
	using namespace boost;
	using namespace gregorian;
	using namespace YAML;
	using namespace modis_api;

	const path yml_path(yml_path_str);

	if (std::optional<Node> node_optional = load_yml(yml_path.string()))
	{

		const Node node = node_optional.value();
		const path workspace_path = node["Workspace"].as<string>();
		BOOST_ASSERT_MSG(exists(workspace_path) && is_directory(workspace_path), "workspace path is illegal!");
		const path tmp_path = node["TmpPath"].as<string>();
		BOOST_ASSERT_MSG(exists(tmp_path) && is_directory(tmp_path), "tmp path is illegal!");

		const vector<string> products = node["SelectedProducts"].as<vector<string>>();
		BOOST_ASSERT_MSG(!products.empty(), "No selected products found!");

		BOOST_LOG_TRIVIAL(debug) << "load " << products.size() << " products";
		BOOST_LOG_TRIVIAL(debug) << join(products, ",");

		const string date_start_str = node["StartDate"].as<string>();
		const string date_end_str = node["EndDate"].as<string>();
		date date_start = Date_utils::get_date_from_doy_str(date_start_str);
		date date_end = Date_utils::get_date_from_doy_str(date_end_str);

		const string preprocess_extent = node["PreprocessExtent"].as<string>();
		float preprocess_min_lon = 0, preprocess_max_lon = 0,
			preprocess_min_lat = 0, preprocess_max_lat = 0;
		int ret = parse_lon_lat(preprocess_extent, preprocess_min_lon,
			preprocess_max_lon, preprocess_min_lat, preprocess_max_lat);
		BOOST_ASSERT(ret == EXIT_SUCCESS);

		const path yml_folder_path("D:\\modis_workspace\\generated_ymls");

		for (const string& product : products)
		{
			if (product.find("02") != string::npos)
			{
				if (node["Preprocess"].IsDefined() && node["Preprocess"]["BT"].IsDefined())
				{
					const Node subNode = node["Preprocess"]["BT"];
					const int band = subNode["Band"].as<int>();
					const string mrt_kernel_type = subNode["MRTProjectionType"].as<string>();
					const string mrt_projection_type = subNode["MRTProjectionType"].as<string>();
					const string mrt_projection_args = subNode["MRTProjectionArgs"].as<string>();
					const float mrt_pixel_size = subNode["MRTPixelSize"].as<float>();
					prepare_bt(workspace_path, tmp_path, date_start, date_end,
						product.substr(0, 3), preprocess_min_lon, preprocess_max_lon,
						preprocess_min_lat, preprocess_max_lat, band, mrt_kernel_type,
						mrt_projection_type, mrt_projection_args, mrt_pixel_size, yml_folder_path);
				}
			}
		}
	}

	return EXIT_SUCCESS;
}

int prepare_bt(const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const boost::gregorian::date& date_start,
	const boost::gregorian::date& date_end,
	const std::string& product_type,
	float pp_min_lon, float pp_max_lon, float pp_min_lat, float pp_max_lat, int band,
	const std::string& mrt_kernel_type,
	const std::string& mrt_projection_type,
	const std::string& mrt_projection_args,
	float mrt_pixel_size,
	const std::filesystem::path& yml_folder_path)
{
	using namespace std;
	using namespace filesystem;
	using namespace boost;
	using namespace gregorian;
	using namespace YAML;
	using namespace modis_api;

	const path new_tmp_path = tmp_path / "BT";
	if (!exists(new_tmp_path)) create_directories(new_tmp_path);
	if (!exists(yml_folder_path)) create_directory(yml_folder_path);

	for (day_iterator it = day_iterator(date_start); it <= date_end; ++it)
	{
		const string year_and_day = Date_utils::get_doy_str(*it);
		// generate hdf list file
		const path hdf_list_path = yml_folder_path / str(format("%1%_bt_hdf_list_%2%.txt") % product_type % year_and_day);
		const string hdf_list_str = generate_preprocess_bt_hdf_list_str(workspace_path, product_type, *it);
		int ret = File_operation::write_to_file(hdf_list_path.string(), hdf_list_str);
		BOOST_ASSERT_MSG(ret == EXIT_SUCCESS, "generate hdf file list failed!");

		// generate yml
		const path yml_path = yml_folder_path / str(format("%1%_bt_%2%.yml") % product_type % year_and_day);
		const path output_file_path = workspace_path / "Standard" / "BT" / product_type / (year_and_day + ".tif");
		const string yml_str = generate_preprocess_bt_yml_str(hdf_list_path, tmp_path, pp_min_lon, pp_max_lon, pp_min_lat, pp_max_lat,
			band, mrt_kernel_type, mrt_projection_type, mrt_projection_args, mrt_pixel_size, output_file_path);
		ret = File_operation::write_to_file(yml_path.string(), yml_str);
		BOOST_ASSERT_MSG(ret == EXIT_SUCCESS, "generate hdf file list failed!");
	}
	return EXIT_SUCCESS;
}

std::string generate_preprocess_bt_hdf_list_str(const std::filesystem::path& workspace_path,
	const std::string& product_type, const boost::gregorian::date& date)
{
	using namespace std;
	using namespace filesystem;
	using namespace boost;
	using namespace gregorian;
	using namespace modis_api;

	ostringstream oss;
	const string year_and_day = Date_utils::get_doy_str(date);
	const string year = Date_utils::get_doy_year(year_and_day);
	const string day_of_year = Date_utils::get_doy_day(year_and_day);

	const path bt_folder_path = workspace_path / (product_type + "021KM") / year / day_of_year;
	BOOST_ASSERT_MSG(exists(bt_folder_path) && !filesystem::is_empty(bt_folder_path),
		str(format("bt folder for %1% not found or it is empty!") % year_and_day).c_str());
	const path sza_folder_path = workspace_path / (product_type + "03") / year / day_of_year;
	BOOST_ASSERT_MSG(exists(sza_folder_path) && !filesystem::is_empty(sza_folder_path),
		str(format("sza folder for %1% not found or it is empty!") % year_and_day).c_str());
	const path cm_folder_path = workspace_path / (product_type + "35_L2") / year / day_of_year;
	BOOST_ASSERT_MSG(exists(cm_folder_path) && !filesystem::is_empty(cm_folder_path),
		str(format("cm folder for %1% not found or it is empty!") % year_and_day).c_str());

	vector<string> bt_files = File_operation::get_all_files_by_extension(bt_folder_path.string(), ".hdf");
	vector<string> sza_files = File_operation::get_all_files_by_extension(sza_folder_path.string(), ".hdf");
	vector<string> cm_files = File_operation::get_all_files_by_extension(cm_folder_path.string(), ".hdf");
	BOOST_ASSERT_MSG(bt_files.size() == sza_files.size() && bt_files.size() == cm_files.size(),
		"the files does not matched!");

	for (const string& bt_file : bt_files)
	{
		vector<string> components;
		const string bt_file_name = path(bt_file).filename().string();
		split(components, bt_file_name, is_any_of("."));
		BOOST_ASSERT_MSG(components.size() == 6,
			("Wrong file name: " + bt_file_name).c_str());
		const string filter_str = components[1] + "." + components[2];

		vector<string>::iterator it1 = find_if(sza_files.begin(), sza_files.end(),
			[&filter_str](const string& n)
		{ return n.find(filter_str) != string::npos; });
		if (it1 == sza_files.end())
		{
			BOOST_LOG_TRIVIAL(error) << "亮温文件" << bt_file_name <<
				"未找到对应的太阳天顶角文件。亮温数据目录：" << bt_folder_path <<
				"，太阳天顶角数据目录：" << sza_folder_path;
			continue;
		}
		const string sza_file = *it1;

		it1 = find_if(cm_files.begin(), cm_files.end(), [&filter_str](const string& n)
		{return n.find(filter_str) != string::npos; });
		if (it1 == cm_files.end())
		{
			BOOST_LOG_TRIVIAL(error) << "亮温文件" << bt_file_name <<
				"未找到对应的云掩膜文件。亮温数据目录：" << bt_folder_path <<
				"，云掩膜数据目录：" << cm_folder_path;
			continue;
		}
		const string cm_file = *it1;
		BOOST_LOG_TRIVIAL(debug) << "bt file: " << bt_file;
		BOOST_LOG_TRIVIAL(debug) << "sza file: " << sza_file;
		BOOST_LOG_TRIVIAL(debug) << "cm file: " << cm_file;

		oss << "#\n" << bt_file << "\n" << sza_file << "\n"
			<< cm_file << "\n";
	}

	return oss.str();
}

std::string generate_preprocess_bt_yml_str(
	const std::filesystem::path& hdf_list_file_path,
	const std::filesystem::path& tmp_path,
	float min_lon, float max_lon, float min_lat, float max_lat,
	int band,
	const std::string& mrt_kernel_type,
	const std::string& mrt_projection_type,
	const std::string& mrt_projection_args,
	float mrt_pixel_size,
	const std::filesystem::path& output_image_path)
{
	using namespace YAML;
	Emitter emt;
	emt << BeginMap;
	emt << Key << "HDFListFile" << Value << hdf_list_file_path.string();
	emt << Key << "TmpPath" << Value << tmp_path.string();
	emt << Key << "MinLon" << Value << min_lon;
	emt << Key << "MaxLon" << Value << max_lon;
	emt << Key << "MinLat" << Value << min_lat;
	emt << Key << "MaxLat" << Value << max_lat;
	emt << Key << "Band" << Value << band;
	emt << Key << "MRTKernelType" << Value << mrt_kernel_type;
	emt << Key << "MRTProjectionType" << Value << mrt_projection_type;
	emt << Key << "MRTProjectionArgs" << Value << mrt_projection_args;
	emt << Key << "MRTPixelSize" << Value << mrt_pixel_size;
	emt << Key << "OutputImageFile" << output_image_path.string();
	emt << EndMap;
	return emt.c_str();
}

int parse_lon_lat(const std::string& lon_lat_str, float& out_min_lon, float& out_max_lon, float& out_min_lat,
	float& out_max_lat)
{
	using namespace std;
	vector<string> extent_strs;
	split(extent_strs, lon_lat_str, boost::is_any_of(" "));
	BOOST_ASSERT(extent_strs.size() == 4);
	out_min_lon = stof(extent_strs[0]);
	out_max_lon = stof(extent_strs[1]);
	out_min_lat = stof(extent_strs[2]);
	out_max_lat = stof(extent_strs[3]);
	BOOST_LOG_TRIVIAL(debug) << str(boost::format("Parse lon lat from %1%: %2%, %3%, %4%, %5%")
		% lon_lat_str % out_min_lon % out_max_lon % out_min_lat % out_max_lat);
	return EXIT_SUCCESS;
}
