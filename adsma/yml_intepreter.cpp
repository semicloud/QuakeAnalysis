#include "pch.h"
#include "Adsma_settings.h"
#include "Interpreter_helper.h"
#include "yml_intepreter.h"
#include "Preprocess_bt_intepreter.h"
#include "Preprocess_lst_interpreter.h"
#include "Preprocess_aod_wv_interpreter.h"
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
	using namespace adsma::interpreter::helper;

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
					adsma::interpreter::preprocess::bt::generate_pp_bt_yml_hdflist_files(workspace_path, tmp_path, date_start, date_end,
						product_type, pp_min_lon, pp_max_lon,
						pp_min_lat, pp_max_lat, band, mrt_kernel_type,
						mrt_projection_type, mrt_projection_args, mrt_pixel_size, yml_folder_path);
				}

				if (node[EDDY_FIELD].IsDefined() && node[EDDY_FIELD][BT_NAME].IsDefined())
				{
					// ���Ҫ��ʼ���ж��ˣ�
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

					if (subNode[PLOT_BACKGROUND].IsDefined()) // ��������ͼ
					{
						// TODO �жȳ�ͼ
					}

					if (subNode[PLOT_EDDYFIELD].IsDefined()) // �쳣��ͼ
					{
						// TODO �жȳ�ͼ
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
					adsma::interpreter::preprocess::aodwv::generate_pp_aod_or_wv_yml_hdflist_files(workspace_path, tmp_path, date_start, date_end, product, pp_min_lon,
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
					adsma::interpreter::preprocess::aodwv::generate_pp_aod_or_wv_yml_hdflist_files(workspace_path, tmp_path, date_start, date_end, product, pp_min_lon,
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
					adsma::interpreter::preprocess::lst::generate_pp_lst_yml_hdflist_files(workspace_path, tmp_path, date_start, date_end, product_type, pp_min_lon,
						pp_max_lon, pp_min_lat, pp_max_lat, resampling_type,
						output_projection_type, output_projection_parameters, output_pixel_size,
						yml_folder_path);
				}

				if (node[EDDY_FIELD].IsDefined() && node[EDDY_FIELD][LST_NAME].IsDefined())
				{
					// ���Ҫ��ʼ���ж��ˣ�
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

#pragma region Ԥ�������

#pragma  region ����Ԥ����


#pragma endregion 

#pragma  region �ر��¶�Ԥ����


#pragma endregion 

#pragma  region ���ܽ���ˮ��Ԥ����






#pragma endregion 

#pragma endregion 

#pragma region �ж����

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
	const path pp_data_path = workspace_path / adsma::settings::PP_STANDARD_FOLDER / adsma::interpreter::helper::preprocess::get_pp_folder(product_type, product);
	for (const unsigned year : years)
	{
		const path product_path = pp_data_path / to_string(year) /
			adsma::interpreter::helper::preprocess::get_pp_output_file_name(product, to_string(year), current_day);
		if (!exists(product_path))
		{
			BOOST_LOG_TRIVIAL(warning) << "�������ļ�" << product_path << "���޷�ʹ�ø��ļ������жȱ�����";
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

	vector<unsigned> product_years = adsma::interpreter::helper::get_years_of_product(workspace_path,
		product_type, product);

	for (day_iterator it(start_date); it <= end_date; ++it)
	{
		const string year_and_day = modis_api::Date_utils::get_doy_str(*it);
		const string year = modis_api::Date_utils::get_doy_year(year_and_day);
		const unsigned year_u = stoul(year);
		const string day = modis_api::Date_utils::get_doy_day(year_and_day);
		const path input_image_path = workspace_path / adsma::settings::PP_STANDARD_FOLDER
			/ adsma::interpreter::helper::preprocess::get_pp_folder(product_type, product) / year
			/ adsma::interpreter::helper::preprocess::get_pp_output_file_name(product, year, day);
		if (!exists(input_image_path))
		{
			BOOST_LOG_TRIVIAL(error) << "�ļ�" << input_image_path << "�����ڣ��޷����н����жȴ���" << endl;
			BOOST_LOG_TRIVIAL(error) << "�����˳�";
			exit(EXIT_FAILURE);
		}
		// ���㱳������Ҫ��Щ��ݵ�����
		vector<unsigned> ref_years;
		copy_if(product_years.begin(), product_years.end(), back_inserter(ref_years),
			[&year_u](const unsigned& y) {return y < year_u; });
		const string hdf_list_str = get_eddyfield_ref_hdflist_str(workspace_path,
			tmp_path, product, product_type, *it, ref_years, ano_method);
		if (boost::trim_copy(hdf_list_str).empty())
		{
			BOOST_LOG_TRIVIAL(error) << "û���ҵ����������ɱ��������ļ����޷�Ϊ" << input_image_path << "�����жȱ�����";
			// TODO continue or exit?
			continue;
		}
		const string hdf_list_name = (boost::format("ef_ref%1%_%2%_%3%_%4%.txt")
			% ano_method % boost::to_lower_copy(product_type) % boost::to_lower_copy(product)
			% year_and_day).str();
		const path hdf_list_path = yml_folder_path / hdf_list_name;
		modis_api::File_operation::write_to_file(hdf_list_path.string(), hdf_list_str);

		// �������ļ����·��
		const path ref_file_path = workspace_path / "Ref" /
			(boost::format("EF%1%_%2%_%3%") % ano_method % product_type % product).str() /
			year / (boost::format("%1%_REF_%2%_%3%.tif") % product % year% day).str();
		// �ж��쳣�ļ����·��
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
#pragma endregion �ж����



