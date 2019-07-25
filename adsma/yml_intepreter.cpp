#include "pch.h"
#include "yml_intepreter.h"
#include "../modis_api/Date_utils.h"
#include "../modis_api/File_operation.h"
#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <vector>
#include <optional>
#include <iostream>
#include <filesystem>

std::optional<YAML::Node> load_yml(const std::string& yml_path_str)
{
	using namespace std;
	using namespace std::filesystem;
	using namespace YAML;

	BOOST_ASSERT(exists(yml_path_str));
	try
	{
		Node node = LoadFile(yml_path_str);
		return optional<Node>(node);
	}
	catch (std::exception& e)
	{
		cerr << "parse yml error! message: " << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

std::vector<boost::gregorian::date> parse_date(const YAML::Node& node)
{
	using namespace std;
	using namespace boost::gregorian;
	using namespace modis_api;
	const string date_start_str = node[START_DATE].as<string>();
	const string date_end_str = node[END_DATE].as<string>();
	const date d1 = Date_utils::get_date_from_doy_str(date_start_str);
	const date d2 = Date_utils::get_date_from_doy_str(date_end_str);
	BOOST_ASSERT(d1 <= d2);
	return vector<date>{d1, d2};
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
		const path workspace_path(node[WORKSPACE].as<string>());
		BOOST_ASSERT_MSG(exists(workspace_path) && is_directory(workspace_path), "workspace path is illegal!");
		const path tmp_path(node[TMP_PATH].as<string>());
		BOOST_ASSERT_MSG(exists(tmp_path) && is_directory(tmp_path), "tmp path is illegal!");
		const path yml_folder_path(node[YML_FOLDER_PATH].as<string>());
		if (!exists(yml_folder_path)) create_directories(yml_folder_path);

		const vector<string> products = node[SELECTED_PRODUCTS].as<vector<string>>();
		BOOST_ASSERT_MSG(!products.empty(), "No selected products found!");

		BOOST_LOG_TRIVIAL(debug) << "load " << products.size() << " products";
		BOOST_LOG_TRIVIAL(debug) << join(products, ",");

		vector<date> dates = parse_date(node);
		date date_start = dates[0], date_end = dates[1];
		BOOST_LOG_TRIVIAL(debug) << "Start date: " << date_start;
		BOOST_LOG_TRIVIAL(debug) << "End date: " << date_end;

		const string pp_extent = node[PREPROCESS_EXTENT].as<string>();
		// pp means PreProcess
		float pp_min_lon = 0, pp_max_lon = 0, pp_min_lat = 0, pp_max_lat = 0;
		int ret = split_lonlat_str(pp_extent, pp_min_lon, pp_max_lon, pp_min_lat, pp_max_lat);
		BOOST_ASSERT(ret == EXIT_SUCCESS);

		for (const string& product : products)
		{
			string product_type = product.substr(0, 3);
			if (product.find(BT_CODE) != string::npos)
			{
				if (node[PREPROCESS].IsDefined() && node[PREPROCESS][BT_NAME].IsDefined())
				{
					const Node subNode = node[PREPROCESS][BT_NAME];
					const int band = subNode[BAND].as<int>();
					const string mrt_kernel_type = subNode["MRTProjectionType"].as<string>();
					const string mrt_projection_type = subNode["MRTProjectionType"].as<string>();
					const string mrt_projection_args = subNode["MRTProjectionArgs"].as<string>();
					const float mrt_pixel_size = subNode["MRTPixelSize"].as<float>();
					generate_pp_bt_yml_hdflist_files(workspace_path, tmp_path, date_start, date_end,
						product_type, pp_min_lon, pp_max_lon,
						pp_min_lat, pp_max_lat, band, mrt_kernel_type,
						mrt_projection_type, mrt_projection_args, mrt_pixel_size, yml_folder_path);
				}

				if (node[EDDY_FIELD].IsDefined() && node[EDDY_FIELD][BT_NAME].IsDefined())
				{
					// 这就要开始算涡度了！
					const Node subNode = node[EDDY_FIELD][BT_NAME];
					const bool calc_ref = subNode[CALC_REF].as<bool>();
					const bool calc_ano = subNode[CALC_ANO].as<bool>();
					const int ano_method = subNode[ANO_METHOD].as<int>();
					const Node plot_bg_node = subNode[PLOT_BACKGROUND];
					const string plot_bg_title = plot_bg_node[TITLE].as<string>();
					const string plot_bg_bar_title = plot_bg_node[BAR_TITLE].as<string>();
					const Node plot_ef_node = subNode[PLOT_EDDYFIELD];
					const string plot_ef_title = plot_ef_node[TITLE].as<string>();
					const string plot_ef_bar_title = plot_ef_node[BAR_TITLE].as<string>();
					generate_eddyfield_yml_hdflist_files(workspace_path, tmp_path,
						product, product_type, date_start, date_end,
						calc_ref, calc_ano, ano_method, yml_folder_path);

					if (subNode[PLOT_BACKGROUND].IsDefined()) // 背景场出图
					{
						// TODO 涡度出图
					}

					if (subNode[PLOT_EDDYFIELD].IsDefined()) // 异常出图
					{
						// TODO 涡度出图
					}
				}
			}
			else if (product.find(AOD_CODE) != string::npos)
			{
				if (node[PREPROCESS].IsDefined() && node[PREPROCESS][AOD_NAME].IsDefined())
				{
					const Node subNode = node[PREPROCESS][AOD_NAME];
					const string resampling_type = subNode["ResamplingType"].as<string>();
					const string output_projection_type = subNode["OutputProjectionType"].as<string>();
					const string output_projection_parameters = subNode["OutputProjectionParameters"].as<string>();
					generate_pp_aod_or_wv_yml_hdflist_files(workspace_path, tmp_path, date_start, date_end, product, pp_min_lon,
						pp_max_lon, pp_min_lat, pp_max_lat, resampling_type, output_projection_type, output_projection_parameters,
						yml_folder_path);
				}
			}
			else if (product.find(WV_CODE) != string::npos)
			{
				if (node[PREPROCESS].IsDefined() && node[PREPROCESS][WV_NAME].IsDefined())
				{
					const Node subNode = node[PREPROCESS][WV_NAME];
					const string resampling_type = subNode["ResamplingType"].as<string>();
					const string output_projection_type = subNode["OutputProjectionType"].as<string>();
					const string output_projection_parameters = subNode["OutputProjectionParameters"].as<string>();
					generate_pp_aod_or_wv_yml_hdflist_files(workspace_path, tmp_path, date_start, date_end, product, pp_min_lon,
						pp_max_lon, pp_min_lat, pp_max_lat, resampling_type, output_projection_type, output_projection_parameters,
						yml_folder_path);
				}
			}
			else if (product.find(LST_CODE) != string::npos)
			{
				if (node[PREPROCESS].IsDefined() && node[PREPROCESS][LST_NAME].IsDefined())
				{
					const Node subNode = node[PREPROCESS][LST_NAME];
					const string resampling_type = subNode["ResamplingType"].as<string>();
					const string output_projection_type = subNode["OutputProjectionType"].as<string>();
					const string output_projection_parameters = subNode["OutputProjectionParameters"].as<string>();
					const float output_pixel_size = subNode["OutputPixelSize"].as<float>();
					generate_pp_lst_yml_hdflist_files(workspace_path, tmp_path, date_start, date_end, product_type, pp_min_lon,
						pp_max_lon, pp_min_lat, pp_max_lat, resampling_type,
						output_projection_type, output_projection_parameters, output_pixel_size,
						yml_folder_path);
				}

				if (node[EDDY_FIELD].IsDefined() && node[EDDY_FIELD][LST_NAME].IsDefined())
				{
					// 这就要开始算涡度了！
					const Node subNode = node[EDDY_FIELD][LST_NAME];
					const bool calc_ref = subNode[CALC_REF].as<bool>();
					const bool calc_ano = subNode[CALC_ANO].as<bool>();
					const int ano_method = subNode[ANO_METHOD].as<int>();
					const Node plot_bg_node = subNode[PLOT_BACKGROUND];
					const string plot_bg_title = plot_bg_node[TITLE].as<string>();
					const string plot_bg_bar_title = plot_bg_node[BAR_TITLE].as<string>();
					const Node plot_ef_node = subNode[PLOT_EDDYFIELD];
					const string plot_ef_title = plot_ef_node[TITLE].as<string>();
					const string plot_ef_bar_title = plot_ef_node[BAR_TITLE].as<string>();
					generate_eddyfield_yml_hdflist_files(workspace_path, tmp_path,
						LST_NAME, product_type, date_start, date_end,
						calc_ref, calc_ano, ano_method, yml_folder_path);
				}
			}
		}

		BOOST_LOG_TRIVIAL(debug) << "Done!";
	}

	return EXIT_SUCCESS;
}

#pragma region 预处理相关

#pragma  region 亮温预处理

int generate_pp_bt_yml_hdflist_files(const std::filesystem::path& workspace_path,
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
	using namespace boost::gregorian;
	using namespace YAML;
	using namespace modis_api;

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

std::string get_preprocess_bt_yml_str(
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

std::string get_preprocess_bt_hdf_list_str(const std::filesystem::path& workspace_path,
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

#pragma endregion 

#pragma  region 地表温度预处理

int generate_pp_lst_yml_hdflist_files(const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const boost::gregorian::date& date_start,
	const boost::gregorian::date& date_end, const std::string& product_type,
	float pp_min_lon, float pp_max_lon, float pp_min_lat, float pp_max_lat,
	const std::string& resampling_type,
	const std::string& output_projection_type,
	const std::string& output_projection_parameters,
	const float output_pixel_size,
	const std::filesystem::path& yml_folder_path)
{
	using namespace std;
	using namespace filesystem;
	using namespace boost;
	using namespace gregorian;
	using namespace YAML;
	using namespace modis_api;

	const path new_tmp_path = tmp_path / LST_NAME;
	if (!exists(new_tmp_path)) create_directories(new_tmp_path);
	if (!exists(yml_folder_path)) create_directory(yml_folder_path);

	for (day_iterator it = day_iterator(date_start); it <= date_end; ++it)
	{
		const string year_and_day = Date_utils::get_doy_str(*it);
		const string year = Date_utils::get_doy_year(year_and_day);
		const string day = Date_utils::get_doy_day(year_and_day);
		if (!is_data_exist(workspace_path, str(format("%1%%2%") % product_type % LST_FOLDER_SUFFIX), year_and_day))
			continue;
		// generate hdf list file
		const path hdf_list_path = yml_folder_path / str(format("%1%_lst_hdf_list_%2%.txt") % product_type % year_and_day);
		const string hdf_list_str = get_preprocess_lst_hdf_list_str(workspace_path, product_type, *it);
		int ret = File_operation::write_to_file(hdf_list_path.string(), hdf_list_str);
		BOOST_ASSERT_MSG(ret == EXIT_SUCCESS, "generate hdf file list failed!");

		// generate yml
		const path yml_path = yml_folder_path / str(format("pp_lst_%1%_%2%.yml") % product_type % year_and_day);
		const path output_file_path = workspace_path / PP_STANDARD_FOLDER /
			get_pp_folder(product_type, LST_NAME) / year / get_pp_output_file_name(LST_NAME, year, day);
		const string yml_str = get_preprocess_lst_yml_str(hdf_list_path, tmp_path, pp_min_lon, pp_max_lon, pp_min_lat, pp_max_lat, resampling_type,
			output_projection_type, output_projection_parameters, output_pixel_size, output_file_path.string());
		ret = File_operation::write_to_file(yml_path.string(), yml_str);
		BOOST_ASSERT_MSG(ret == EXIT_SUCCESS, "generate hdf file list failed!");
	}
	return EXIT_SUCCESS;
}

std::string get_preprocess_lst_yml_str(
	const std::filesystem::path& hdf_list_file_path,
	const std::filesystem::path& tmp_path,
	float min_lon, float max_lon, float min_lat, float max_lat,
	const std::string& resampling_type,
	const std::string& output_projection_type,
	const std::string& output_projection_parameters,
	float output_pixel_size,
	const std::filesystem::path& output_image_path)
{
	using namespace std;
	using namespace boost;
	auto s = [](const char* cc) {return string(cc); };
	unordered_map<string, string> umap;
	umap.insert({ string("HDFListFile"),  hdf_list_file_path.string() });
	umap.insert({ string("MinLon"),  lexical_cast<string>(min_lon) });
	umap.insert({ string("MaxLon"),  lexical_cast<string>(max_lon) });
	umap.insert({ string("MinLat"),  lexical_cast<string>(min_lat) });
	umap.insert({ string("MaxLat"),  lexical_cast<string>(max_lat) });
	umap.insert({ string("ResamplingType"),  resampling_type });
	umap.insert({ string("OutputProjectionType"),  output_projection_type });
	umap.insert({ string("OutputProjectionParameters"),  output_projection_parameters });
	umap.insert({ string("OutputPixelSize"),  lexical_cast<string>(output_pixel_size) });
	umap.insert({ string("TmpPath"),  tmp_path.string() });
	umap.insert({ string("OutputImageFile"), output_image_path.string() });
	string yml_str = get_yml_str(umap);
	return yml_str;
}

std::string get_preprocess_lst_hdf_list_str(
	const std::filesystem::path& workspace_path,
	const std::string& product_type,
	const boost::gregorian::date& date)
{
	using namespace std;
	using namespace filesystem;
	using namespace boost;
	using namespace gregorian;
	using namespace modis_api;
	BOOST_ASSERT_MSG(product_type == PROD_TYPE_MOD || product_type == PROD_TYPE_MYD, "wrong product type!");

	ostringstream oss;
	const string year_and_day = Date_utils::get_doy_str(date);
	const string year = Date_utils::get_doy_year(year_and_day);
	const string day_of_year = Date_utils::get_doy_day(year_and_day);
	path product_path = workspace_path / (product_type + LST_FOLDER_SUFFIX) / year / day_of_year;
	BOOST_LOG_TRIVIAL(debug) << "Product path: " << product_path.string();

	vector<string> files = File_operation::get_all_files_by_extension(product_path.string(), ".hdf");
	BOOST_ASSERT_MSG(!files.empty(), "no file found!");

	for_each(files.cbegin(), files.cend(), [&oss](const string& path) { oss << path << "\n"; });

	return oss.str();
}

#pragma endregion 

#pragma  region 气溶胶与水汽预处理

int generate_pp_aod_or_wv_yml_hdflist_files(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const boost::gregorian::date& date_start,
	const boost::gregorian::date& date_end,
	const std::string& m_product,
	float pp_min_lon, float pp_max_lon, float pp_min_lat, float pp_max_lat,
	const std::string& resampling_type,
	const std::string& output_projection_type,
	const std::string& output_projection_parameters,
	const std::filesystem::path& yml_folder_path)
{
	using namespace std;
	using namespace filesystem;
	using namespace boost;
	using namespace gregorian;
	using namespace YAML;
	using namespace modis_api;

	const string product_type = m_product.substr(0, 3);
	const string product_code = m_product.substr(3, 2);
	BOOST_ASSERT_MSG(product_code == AOD_CODE || product_code == WV_CODE, "wrong product code!");
	string product;
	if (product_code == AOD_CODE)
		product = AOD_NAME;
	else if (product_code == WV_CODE)
		product = WV_NAME;
	BOOST_LOG_TRIVIAL(debug) << "product: " << product << ", product type: " << product_type;

	const path new_tmp_path = tmp_path / product;
	if (!exists(new_tmp_path)) create_directories(new_tmp_path);
	if (!exists(yml_folder_path)) create_directories(yml_folder_path);

	for (day_iterator it = day_iterator(date_start); it <= date_end; ++it)
	{
		const string year_and_day = Date_utils::get_doy_str(*it);
		const string year = Date_utils::get_doy_year(year_and_day);
		const string day = Date_utils::get_doy_day(year_and_day);
		// generate hdf list file
		const path hdf_list_path = yml_folder_path / str(format("%1%_%2%_hdf_list_%3%.txt") % product_type % to_lower_copy(product) % year_and_day);
		const string hdf_list_str = get_preprocess_aod_wv_hdf_list_str(workspace_path, product_code, product_type, *it);
		int ret = File_operation::write_to_file(hdf_list_path.string(), hdf_list_str);
		BOOST_ASSERT_MSG(ret == EXIT_SUCCESS, "generate hdf file list failed!");

		// generate yml
		const path yml_path = yml_folder_path / str(format("pp_%1%_%2%_%3%.yml") % to_lower_copy(product) % product_type % year_and_day);
		const path output_file_path = workspace_path / PP_STANDARD_FOLDER /
			get_pp_folder(product_type, product) / year / get_pp_output_file_name(product, year, day);
		const string yml_str = get_preprocess_aod_wv_yml_str(hdf_list_path, tmp_path,
			pp_min_lon, pp_max_lon, pp_min_lat, pp_max_lat, resampling_type,
			output_projection_type, output_projection_parameters, output_file_path);
		ret = File_operation::write_to_file(yml_path.string(), yml_str);
		BOOST_ASSERT_MSG(ret == EXIT_SUCCESS, "generate yml failed!");
	}
	return EXIT_SUCCESS;

}

std::string get_preprocess_aod_wv_hdf_list_str(const std::filesystem::path& workspace_path,
	const std::string& product_code, const std::string& product_type,
	const boost::gregorian::date& date)
{
	using namespace std;
	using namespace filesystem;
	using namespace boost;
	using namespace gregorian;
	using namespace modis_api;
	BOOST_ASSERT_MSG(product_code == AOD_CODE || product_code == WV_CODE, "wrong prodcut code!");
	BOOST_ASSERT_MSG(product_type == PROD_TYPE_MOD || product_type == PROD_TYPE_MYD, "wrong product type!");

	ostringstream oss;
	const string year_and_day = Date_utils::get_doy_str(date);
	const string year = Date_utils::get_doy_year(year_and_day);
	const string day_of_year = Date_utils::get_doy_day(year_and_day);
	path product_path;
	if (product_code == AOD_CODE)
		product_path = workspace_path / (product_type + AOD_FOLDER_SUFFIX) / year / day_of_year;
	else if (product_code == WV_CODE)
		product_path = workspace_path / (product_type + WV_FOLDER_SUFFIX) / year / day_of_year;
	BOOST_LOG_TRIVIAL(debug) << "Product path: " << product_path.string();
	BOOST_ASSERT_MSG(exists(product_path), "wrong product path!");

	vector<string> files = File_operation::get_all_files_by_extension(product_path.string(), ".hdf");
	BOOST_ASSERT_MSG(!files.empty(), "no file found!");

	for_each(files.cbegin(), files.cend(), [&oss](const string& path) { oss << path << "\n"; });

	return oss.str();
}

std::string get_preprocess_aod_wv_yml_str(
	const std::filesystem::path& hdf_list_file_path,
	const std::filesystem::path& tmp_path,
	float min_lon, float max_lon, float min_lat, float max_lat,
	const std::string& resampling_type,
	const std::string& output_projection_type,
	const std::string& output_projection_parameters,
	const std::filesystem::path& output_image_path)
{
	using namespace std;
	using namespace boost;
	auto s = [](const char* cc) {return string(cc); };
	unordered_map<string, string> umap;
	umap.insert({ string("HDFListFile"),  hdf_list_file_path.string() });
	umap.insert({ string("MinLon"),  lexical_cast<string>(min_lon) });
	umap.insert({ string("MaxLon"),  lexical_cast<string>(max_lon) });
	umap.insert({ string("MinLat"),  lexical_cast<string>(min_lat) });
	umap.insert({ string("MaxLat"),  lexical_cast<string>(max_lat) });
	umap.insert({ string("ResamplingType"),  resampling_type });
	umap.insert({ string("OutputProjectionType"),  output_projection_type });
	umap.insert({ string("OutputProjectionParameters"),  output_projection_parameters });
	umap.insert({ string("OutputImageFile"),  output_image_path.string() });
	umap.insert({ string("TmpPath"),  tmp_path.string() });
	string yml_str = get_yml_str(umap);
	return yml_str;
}

#pragma endregion 

#pragma endregion 

#pragma region 涡度相关

std::string get_eddyfield_yml_str(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	bool calc_ref, bool calc_ano, int ano_method,
	const std::filesystem::path& input_image_file_path,
	const std::filesystem::path& ref_list_file_path,
	const std::filesystem::path& ref_image_file_path,
	const std::filesystem::path& output_ano_file_path)
{
	using namespace std;
	using namespace std::filesystem;
	using namespace YAML;
	Emitter emt;
	emt << BeginMap;
	emt << Key << "Workspace" << Value << workspace_path.string();
	emt << Key << "CalcRef" << Value << calc_ref;
	emt << Key << "CalcAno" << Value << calc_ano;
	emt << Key << "AnoMethod" << Value << ano_method;
	emt << Key << "InputImageFile" << Value << input_image_file_path.string();
	emt << Key << "RefListFile" << Value << ref_list_file_path.string();
	emt << Key << "RefImageFile" << Value << ref_image_file_path.string();
	emt << Key << "OutputAnoFile" << Value << output_ano_file_path.string();
	emt << EndMap;
	return emt.c_str();
}

std::string get_eddyfield_ref_hdflist_str(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product,
	const std::string& product_type,
	const boost::gregorian::date& date,
	const std::vector<unsigned>& years,
	int ano_method)
{
	using namespace std;
	using namespace filesystem;
	ostringstream oss;
	//vector<path> tif_file_paths; 
	const string year_and_day = modis_api::Date_utils::get_doy_str(date);
	const string current_year = modis_api::Date_utils::get_doy_year(year_and_day);
	const string current_day = modis_api::Date_utils::get_doy_day(year_and_day);
	const path pp_data_path = workspace_path / PP_STANDARD_FOLDER / get_pp_folder(product_type, product);
	for (const unsigned year : years)
	{
		const path product_path = pp_data_path / to_string(year) /
			get_pp_output_file_name(product, to_string(year), current_day);
		if (!exists(product_path))
		{
			BOOST_LOG_TRIVIAL(warning) << "不存在文件" << product_path << "，无法使用该文件生成涡度背景场";
			BOOST_LOG_TRIVIAL(warning) << __FUNCDNAME__ << "," << __LINE__;
			continue;
		}
		//tif_file_paths.push_back(product_path);
		oss << product_path.string() << endl;
	}
	return oss.str();
}

int generate_eddyfield_yml_hdflist_files(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product,
	const std::string& product_type,
	const boost::gregorian::date& start_date,
	const boost::gregorian::date& end_date,
	bool calc_ref, bool calc_ano, int ano_method,
	const std::filesystem::path& yml_folder_path)
{
	using namespace std;
	using namespace std::filesystem;
	using namespace boost::gregorian;

	vector<unsigned> product_years = get_years_of_product(workspace_path,
		product_type, product);

	for (day_iterator it(start_date); it <= end_date; ++it)
	{
		const string year_and_day = modis_api::Date_utils::get_doy_str(*it);
		const string year = modis_api::Date_utils::get_doy_year(year_and_day);
		const unsigned year_u = stoul(year);
		const string day = modis_api::Date_utils::get_doy_day(year_and_day);
		const path input_image_path = workspace_path / PP_STANDARD_FOLDER
			/ get_pp_folder(product_type, product) / year
			/ get_pp_output_file_name(product, year, day);
		if (!exists(input_image_path))
		{
			BOOST_LOG_TRIVIAL(error) << "文件" << input_image_path << "不存在，无法进行进行涡度处理" << endl;
			BOOST_LOG_TRIVIAL(error) << "程序退出";
			exit(EXIT_FAILURE);
		}
		// 计算背景场需要这些年份的数据
		vector<unsigned> ref_years;
		copy_if(product_years.begin(), product_years.end(), back_inserter(ref_years),
			[&year_u](const unsigned& y) {return y < year_u; });
		const string hdf_list_str = get_eddyfield_ref_hdflist_str(workspace_path,
			tmp_path, product, product_type, *it, ref_years, ano_method);
		if (boost::trim_copy(hdf_list_str).empty())
		{
			BOOST_LOG_TRIVIAL(error) << "没有找到可用于生成背景场的文件，无法为" << input_image_path << "生成涡度背景场";
			// TODO continue or exit?
			continue;
		}
		const string hdf_list_name = (boost::format("ef_ref%1%_%2%_%3%_%4%.txt")
			% ano_method % boost::to_lower_copy(product_type) % boost::to_lower_copy(product)
			% year_and_day).str();
		const path hdf_list_path = yml_folder_path / hdf_list_name;
		modis_api::File_operation::write_to_file(hdf_list_path.string(), hdf_list_str);

		// 背景场文件存放路径
		const path ref_file_path = workspace_path / "Ref" /
			(boost::format("EF%1%_%2%_%3%") % ano_method % product_type % product).str() /
			year / (boost::format("%1%_REF_%2%_%3%.tif") % product % year% day).str();
		// 涡度异常文件存放路径
		const path ano_file_path = workspace_path / "Ano" /
			(boost::format("EF%1%_%2%_%3%") % ano_method % product_type % product).str() /
			year / (boost::format("%1%_ANO_%2%_%3%.tif") % product % year% day).str();
		const string yml_str = get_eddyfield_yml_str(workspace_path, tmp_path, calc_ref, calc_ano, ano_method,
			input_image_path, hdf_list_path, ref_file_path, ano_file_path);
		const string yml_file_name = (boost::format("ef_ano%1%_%2%_%3%_%4%.yml")
			% ano_method % boost::to_lower_copy(product_type) % boost::to_lower_copy(product)
			% year_and_day).str();
		const path yml_path = yml_folder_path / yml_file_name;
		modis_api::File_operation::write_to_file(yml_path.string(), yml_str);
	}
	return 0;
}





#pragma endregion 涡度相关


int split_lonlat_str(const std::string& lon_lat_str, float& out_min_lon, float& out_max_lon, float& out_min_lat,
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

std::string get_yml_str(const std::unordered_map<std::string, std::string>& umap)
{
	using namespace std;
	using namespace YAML;
	BOOST_ASSERT_MSG(!umap.empty(), "empty map!");
	Emitter emt;
	emt << BeginMap;
	for (unordered_map<string, string>::const_iterator it = umap.cbegin();
		it != umap.cend(); ++it)
	{
		emt << Key << it->first << Value << it->second;
	}
	emt << EndMap;
	return emt.c_str();
}

bool is_data_exist(const std::filesystem::path& workspace, const std::string& product, const std::string& year_and_day)
{
	using namespace std;
	using namespace std::filesystem;
	using namespace boost;
	const string year = year_and_day.substr(0, 4);
	const string day = year_and_day.substr(4, 3);
	const path data_folder = workspace / product / year / day;

	if (!exists(data_folder))
	{
		BOOST_LOG_TRIVIAL(error) << "不存在的目录：" << data_folder << "，无法进行预处理";
		return false;
	}
	if (filesystem::is_empty(data_folder))
	{
		BOOST_LOG_TRIVIAL(error) << "目录为空：" << data_folder << "，无法进行预处理";
		return false;
	}
	return true;
}

std::vector<unsigned int> get_years_of_product(
	const std::filesystem::path& workspace_path,
	const std::string& product_type,
	const std::string& product)
{
	using namespace std;
	using namespace filesystem;
	vector<unsigned int> years;
	const path pp_data_path = workspace_path / PP_STANDARD_FOLDER /
		get_pp_folder(product_type, product);
	for (directory_iterator it(pp_data_path); it != directory_iterator(); ++it)
	{
		const string str = it->path().string().substr(it->path().string().size() - 4, 4);
		const unsigned int year = boost::lexical_cast<unsigned int>(str);
		years.push_back(year);
	}
	sort(years.begin(), years.end());
	BOOST_LOG_TRIVIAL(debug) << "Years of " << product_type << product << ": "
		<< join(years | boost::adaptors::transformed([](const unsigned& y) {return to_string(y); }), " ");
	return years;
}
