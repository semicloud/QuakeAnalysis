#include "pch.h"
#include "Adsma_settings.h"
#include "Common_input.h"
#include "Interpreter_helper.h"
#include "yml_intepreter.h"
#include "Preprocess_bt_intepreter.h"
#include "Preprocess_lst_interpreter.h"
#include "Preprocess_aod_wv_interpreter.h"
#include "Eddyfield_interpreter.h"
#include "General_ano_interpreter.h"
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
	using namespace adsma::settings::yaml;

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
	using namespace adsma::settings;
	using namespace adsma::settings::yaml;
	using namespace adsma::settings::yaml::preprocess;
	using namespace adsma::settings::yaml::eddy;
	using namespace adsma::settings::yaml::ga;

	const path yml_path(yml_path_str);

	if (std::optional<Node> node_optional = load_yml(yml_path.string()))
	{
		const Node node = node_optional.value();
		const Common_input common_input(yml_path);

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
		int ret = adsma::split_lonlat_str(pp_extent, pp_min_lon, pp_max_lon, pp_min_lat, pp_max_lat);
		BOOST_ASSERT(ret == EXIT_SUCCESS);

		// plot global setting
		const path shp_boundary_path(node["ShpBoundary"].as<string>());
		const path shp_fault_path(node["ShpFault"].as<string>());
		const path shp_city_path(node["ShpCity"].as<string>());
		const path quake_record_path(node["QuakeRecord"].as<string>());
		const string plot_extent = node["PlotExtent"].as<string>();

		for (const string& product_str : products)
		{
			// MOD or MYD
			string product_type = product_str.substr(0, 3);

			// this product_str is MOD03, MOD11A1, MOD05, MOD04
			if (product_str.find(BT_CODE) != string::npos)
			{
				if (node[PREPROCESS].IsDefined() && node[PREPROCESS][BT_NAME].IsDefined())
				{
					preprocess_bt(Preprocess_bt_input(node, product_type));
				}

				if (node[EDDY_FIELD].IsDefined() && node[EDDY_FIELD][BT_NAME].IsDefined())
				{
					eddy_field(Eddy_field_input(node, BT_NAME, product_type));
				}

				if (node[GENERAL_ANO].IsDefined() && node[GENERAL_ANO][BT_NAME].IsDefined())
				{
					general_ano(General_ano_input(node, BT_NAME, product_type));
				}
			}
			else if (product_str.find(AOD_CODE) != string::npos)
			{
				if (node[PREPROCESS].IsDefined() && node[PREPROCESS][AOD_NAME].IsDefined())
				{
					preprocess_aod(Preprocess_aod_wv_input(node, AOD_NAME, product_type));
				}
			}
			else if (product_str.find(WV_CODE) != string::npos)
			{
				if (node[PREPROCESS].IsDefined() && node[PREPROCESS][WV_NAME].IsDefined())
				{
					preprocess_wv(Preprocess_aod_wv_input(node, WV_NAME, product_type));
				}
			}
			else if (product_str.find(LST_CODE) != string::npos)
			{
				if (node[PREPROCESS].IsDefined() && node[PREPROCESS][LST_NAME].IsDefined())
				{
					preprocess_lst(Preprocess_lst_input(node, product_type));
				}

				if (node[EDDY_FIELD].IsDefined() && node[EDDY_FIELD][LST_NAME].IsDefined())
				{
					eddy_field(Eddy_field_input(node, LST_NAME, product_type));
				}

				if (node[GENERAL_ANO].IsDefined() && node[GENERAL_ANO][LST_NAME].IsDefined())
				{
					general_ano(General_ano_input(node, LST_NAME, product_type));
				}
			}
		}

		BOOST_LOG_TRIVIAL(debug) << "Done!";
	}

	return EXIT_SUCCESS;
}

int preprocess_bt(const Preprocess_bt_input& p)
{
	const int ans = adsma::generate_pp_bt_yml_hdflist_files(p.workspace(),
		p.tmp_dir(), p.start_date(), p.end_date(),
		p.prodcut_type(), p.min_lon(), p.max_lon(),
		p.min_lat(), p.max_lat(), p.band(), p.mrt_kernel_type(),
		p.mrt_projection_type(), p.mrt_projection_args(), p.mrt_pixel_size(),
		p.yml_folder());
	return ans;
}

int preprocess_aod(const Preprocess_aod_wv_input& p)
{
	const std::string product_str = p.prodcut_type() + adsma::settings::AOD_CODE;
	const int ans = adsma::generate_pp_aod_or_wv_yml_hdflist_files(
		p.workspace(), p.tmp_dir(), p.start_date(), p.end_date(), product_str, p.min_lon(),
		p.max_lon(), p.min_lat(), p.max_lat(), p.resampling_type(),
		p.output_projection_type(), p.output_projection_parameters(), p.yml_folder());
	return ans;
}

int preprocess_wv(const Preprocess_aod_wv_input& p)
{
	const std::string product_str = p.prodcut_type() + adsma::settings::WV_CODE;
	const int ans = adsma::generate_pp_aod_or_wv_yml_hdflist_files(
		p.workspace(), p.tmp_dir(), p.start_date(), p.end_date(), product_str,
		p.min_lon(), p.max_lon(), p.min_lat(), p.max_lat(),
		p.resampling_type(), p.output_projection_type(), p.output_projection_parameters(),
		p.yml_folder());
	return ans;
}

int preprocess_lst(const Preprocess_lst_input& p)
{
	const std::string product_str = p.prodcut_type() + adsma::settings::LST_CODE;
	const int ans = adsma::generate_pp_lst_yml_hdflist_files(
		p.workspace(), p.tmp_dir(), p.start_date(), p.end_date(), p.prodcut_type(),
		p.min_lon(), p.max_lon(), p.min_lat(), p.max_lat(), p.resampling_type(),
		p.output_projection_type(), p.output_projection_parameters(),
		p.output_pixel_size(), p.yml_folder());
	return ans;
}

/**
 * \brief 进行涡度处理
 * \param p
 * \return
 */
int eddy_field(const Eddy_field_input& p)
{
	// 这就要开始算涡度了！
	adsma::generate_eddyfield_yml_hdflist_files(p.workspace(), p.tmp_dir(),
		p.product_name(), p.product_type(), p.start_date(), p.end_date(),
		p.is_calc_ref(), p.is_calc_ref(), p.ano_method(), p.yml_folder());
	if (p.is_plot_ref())
	{
		adsma::generate_plot_eddyfield_ref_yml_files(p.workspace(),
			p.tmp_dir(), p.product_name(), p.product_type(), p.start_date(), p.end_date(),
			p.is_calc_ref(), p.is_calc_ano(), p.ano_method(), p.plot_extent(),
			p.shp_boundary(), p.shp_fault(), p.shp_city(), p.quake_record(), p.yml_folder());
	}
	if (p.is_plot_ano())
	{
		adsma::generate_plot_eddyfield_ano_yml_files(p.workspace(),
			p.tmp_dir(), p.product_name(), p.product_type(), p.start_date(), p.end_date(),
			p.is_calc_ref(), p.is_calc_ano(), p.ano_method(), p.plot_extent(),
			p.shp_boundary(), p.shp_fault(), p.shp_city(), p.quake_record(), p.yml_folder());
	}

	return 0;
}

/**
 * \brief 进行年变处理
 * \return
 */
int general_ano(const General_ano_input& p)
{
	adsma::generate_general_ano_yml_hdflist_files(p.workspace(), p.tmp_dir(),
		p.product_name(), p.product_type(),
		p.start_date(), p.end_date(), p.yml_folder());
	if (p.is_plot_ref())
	{

	}
	if (p.is_plot_ano())
	{
		adsma::generate_plot_general_ano_yml_files(
			p.workspace(), p.tmp_dir(), p.product_name(), p.product_type(),
			p.start_date(), p.end_date(), p.plot_extent(), p.shp_boundary(),
			p.shp_fault(), p.shp_city(), p.quake_record(), p.yml_folder()
		);
	}

	return 0;
}
