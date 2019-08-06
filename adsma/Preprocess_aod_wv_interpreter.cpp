#include "pch.h"
#include "Adsma_settings.h"
#include "Interpreter_helper.h"
#include "Preprocess_aod_wv_interpreter.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Date_utils.h"
#include <boost/assert.hpp>
#include <boost/date_time.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <yaml-cpp/yaml.h>
#include "Preprocess_helper.h"


int adsma::generate_pp_aod_or_wv_yml_hdflist_files(
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
	using namespace settings;

	const string product_type = m_product.substr(0, 3);
	const string product_code = m_product.substr(3, 2);
	BOOST_ASSERT_MSG(product_code ==  AOD_CODE || product_code == WV_CODE, "wrong product code!");
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

std::string adsma::get_preprocess_aod_wv_yml_str(
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


std::string adsma::get_preprocess_aod_wv_hdf_list_str(const std::filesystem::path& workspace_path,
	const std::string& product_code, const std::string& product_type,
	const boost::gregorian::date& date)
{
	using namespace std;
	using namespace filesystem;
	using namespace boost;
	using namespace gregorian;
	using namespace modis_api;
	using namespace settings;
	BOOST_ASSERT_MSG(product_code == AOD_CODE || product_code ==WV_CODE, "wrong prodcut code!");
	BOOST_ASSERT_MSG(product_type == PROD_TYPE_MOD || product_type ==PROD_TYPE_MYD, "wrong product type!");

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