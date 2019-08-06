#include "pch.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Date_utils.h"
#include "Adsma_settings.h"
#include "Interpreter_helper.h"
#include "Preprocess_bt_intepreter.h"
#include <yaml-cpp/yaml.h>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include "Preprocess_helper.h"

int adsma::generate_pp_bt_yml_hdflist_files(
	const std::filesystem::path& workspace_path,
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
	using namespace adsma::settings;

	const path new_tmp_path = tmp_path / BT_NAME;
	if (!exists(new_tmp_path)) create_directories(new_tmp_path);
	if (!exists(yml_folder_path)) create_directory(yml_folder_path);

	for (day_iterator it = day_iterator(date_start); it <= date_end; ++it)
	{
		const string year_and_day = Date_utils::get_doy_str(*it);
		const string year = Date_utils::get_doy_year(year_and_day);
		const string day = Date_utils::get_doy_day(year_and_day);
		// generate hdf list file
		const path hdf_list_path = yml_folder_path / str(format("%1%_bt_hdf_list_%2%.txt") % product_type % year_and_day);
		const string hdf_list_str = get_preprocess_bt_hdf_list_str(workspace_path, product_type, *it);
		int ret = File_operation::write_to_file(hdf_list_path.string(), hdf_list_str);
		BOOST_ASSERT_MSG(ret == EXIT_SUCCESS, "generate hdf file list failed!");

		// generate yml
		const path yml_path = yml_folder_path / str(format("pp_bt_%1%_%2%.yml") % product_type % year_and_day);
		const path output_file_path = workspace_path / PP_STANDARD_FOLDER /
			get_pp_folder(product_type, BT_NAME) / year / get_pp_output_file_name(BT_NAME, year, day);
		const string yml_str = get_preprocess_bt_yml_str(hdf_list_path, tmp_path, pp_min_lon, pp_max_lon, pp_min_lat, pp_max_lat,
			band, mrt_kernel_type, mrt_projection_type, mrt_projection_args, mrt_pixel_size, output_file_path);
		ret = File_operation::write_to_file(yml_path.string(), yml_str);
		BOOST_ASSERT_MSG(ret == EXIT_SUCCESS, "generate hdf file list failed!");
	}
	return EXIT_SUCCESS;
}

std::string adsma::get_preprocess_bt_yml_str(
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

std::string adsma::get_preprocess_bt_hdf_list_str(const std::filesystem::path& workspace_path,
	const std::string& product_type, const boost::gregorian::date& date)
{
	using namespace std;
	using namespace filesystem;
	using namespace boost;
	using namespace gregorian;
	using namespace modis_api;
	using namespace adsma::settings;

	ostringstream oss;
	const string year_and_day = Date_utils::get_doy_str(date);
	const string year = Date_utils::get_doy_year(year_and_day);
	const string day_of_year = Date_utils::get_doy_day(year_and_day);

	const path bt_folder_path = workspace_path / (product_type + BT_FOLDER_SUFFIX) / year / day_of_year;
	BOOST_ASSERT_MSG(exists(bt_folder_path) && !filesystem::is_empty(bt_folder_path),
		str(format("bt folder for %1% not found or it is empty!") % year_and_day).c_str());
	const path sza_folder_path = workspace_path / (product_type + SZA_FOLDER_SUFFIX) / year / day_of_year;
	BOOST_ASSERT_MSG(exists(sza_folder_path) && !filesystem::is_empty(sza_folder_path),
		str(format("sza folder for %1% not found or it is empty!") % year_and_day).c_str());
	const path cm_folder_path = workspace_path / (product_type + CM_FOLDER_SUFFIX) / year / day_of_year;
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
