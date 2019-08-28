#include "pch.h"
#include <boost/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <yaml-cpp/yaml.h>
#include "../modis_api/Date_utils.h"
#include "../modis_api/File_operation.h"
#include "Adsma_settings.h"
#include "Interpreter_helper.h"
#include "Eddyfield_interpreter.h"
#include "Preprocess_helper.h"

std::string adsma::get_eddyfield_yml_str(const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path, bool calc_ref, bool calc_ano, int ano_method,
	const std::filesystem::path& input_image_file_path, const std::filesystem::path& ref_list_file_path,
	const std::filesystem::path& ref_image_file_path, const std::filesystem::path& output_ano_file_path)
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

std::string adsma::get_eddyfield_ref_hdflist_str(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product, const std::string& product_type,
	const boost::gregorian::date& date,
	const std::vector<unsigned>& years, int ano_method)
{
	using namespace std;
	using namespace filesystem;
	ostringstream oss;
	//vector<path> tif_file_paths; 
	const string year_and_day = modis_api::Date_utils::get_doy_str(date);
	const string current_year = modis_api::Date_utils::get_doy_year(year_and_day);
	const string current_day = modis_api::Date_utils::get_doy_day(year_and_day);
	const path pp_data_path = workspace_path / settings::PP_STANDARD_FOLDER / get_pp_folder(product_type, product);
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

/**
 * \brief 生成涡度处理yml文件及hdflist文件
 * \param workspace_path 工作空间路径
 * \param tmp_path tmp路径
 * \param product_name 产品名称
 * \param product_type 产品类型
 * \param start_date 开始日期
 * \param end_date 结束日期
 * \param calc_ref 是否计算背景场
 * \param calc_ano 是否计算异常
 * \param ano_method 异常计算方法
 * \param yml_folder_path 生成的yml文件存放目录
 * \return 
 */
int adsma::generate_eddyfield_yml_hdflist_files(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product_name,
	const std::string& product_type,
	const boost::gregorian::date& start_date,
	const boost::gregorian::date& end_date,
	bool calc_ref, bool calc_ano,
	int ano_method, const std::filesystem::path& yml_folder_path)
{
	using namespace std;
	using namespace std::filesystem;
	using namespace boost::gregorian;

	vector<unsigned> product_years = get_years_of_product(workspace_path,
		product_type, product_name);

	for (day_iterator it(start_date); it <= end_date; ++it)
	{
		const string year_and_day = modis_api::Date_utils::get_doy_str(*it);
		const string year = modis_api::Date_utils::get_doy_year(year_and_day);
		const unsigned year_u = stoul(year);
		const string day = modis_api::Date_utils::get_doy_day(year_and_day);
		const path input_image_path = workspace_path / adsma::settings::PP_STANDARD_FOLDER
			/ get_pp_folder(product_type, product_name) / year
			/ get_pp_output_file_name(product_name, year, day);
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
			tmp_path, product_name, product_type, *it, ref_years, ano_method);
		if (boost::trim_copy(hdf_list_str).empty())
		{
			BOOST_LOG_TRIVIAL(error) << "没有找到可用于生成背景场的文件，无法为" << input_image_path << "生成涡度背景场";
			// TODO continue or exit?
			continue;
		}
		const string hdf_list_name = (boost::format("ef_ref%1%_%2%_%3%_%4%.txt")
			% ano_method % boost::to_lower_copy(product_type) % boost::to_lower_copy(product_name)
			% year_and_day).str();
		const path hdf_list_path = yml_folder_path / hdf_list_name;
		modis_api::File_operation::write_to_file(hdf_list_path.string(), hdf_list_str);

		// 背景场文件存放路径
		const path ref_file_path = workspace_path / "Ref" /
			(boost::format("EF%1%_%2%_%3%") % ano_method % product_type % product_name).str() /
			year / (boost::format("%1%_REF_%2%_%3%.tif") % product_name % year% day).str();
		// 涡度异常文件存放路径
		const path ano_file_path = workspace_path / "Ano" /
			(boost::format("EF%1%_%2%_%3%") % ano_method % product_type % product_name).str() /
			year / (boost::format("%1%_ANO_%2%_%3%.tif") % product_name % year% day).str();
		const string yml_str = get_eddyfield_yml_str(workspace_path, tmp_path, calc_ref, calc_ano, ano_method,
			input_image_path, hdf_list_path, ref_file_path, ano_file_path);
		const string yml_file_name = (boost::format("ef_ano%1%_%2%_%3%_%4%.yml")
			% ano_method % boost::to_lower_copy(product_type) % boost::to_lower_copy(product_name)
			% year_and_day).str();
		const path yml_path = yml_folder_path / yml_file_name;
		modis_api::File_operation::write_to_file(yml_path.string(), yml_str);
	}
	return 0;
}

int adsma::generate_plot_eddyfield_ref_yml_files(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product_name,
	const std::string& product_type,
	const boost::gregorian::date& start_date,
	const boost::gregorian::date& end_date,
	bool calc_ref, bool calc_ano,
	int ano_method,
	const std::string& fig_extent,
	const std::filesystem::path& shp_bound_path,
	const std::filesystem::path& shp_fault_path,
	const std::filesystem::path& shp_city_path,
	const std::filesystem::path& quake_record_path,
	const std::filesystem::path& yml_folder_path)
{
	using namespace std;
	using namespace filesystem;

	for (boost::gregorian::day_iterator it(start_date); it <= end_date; ++it)
	{
		const string year_and_day = modis_api::Date_utils::get_doy_str(*it);
		const string year = modis_api::Date_utils::get_doy_year(year_and_day);
		const string day = modis_api::Date_utils::get_doy_day(year_and_day);
		// Example: EF1_MOD_LST
		const string ref_folder_name = (boost::format("EF%1%_%2%_%3%") % ano_method % product_type % product_name).str();
		BOOST_LOG_TRIVIAL(debug) << "Ref folder name:" << ref_folder_name;

		const string ref_tif_name = (boost::format("%1%_REF_%2%_%3%.tif") % product_name % year % day).str();
		const path ref_tif_path = workspace_path / "Ref" / ref_folder_name / year / ref_tif_name;
		BOOST_LOG_TRIVIAL(debug) << "Ref tif path:" << ref_tif_path;

		const string ref_fig_name = (boost::format("%1%_REF_%2%_%3%.jpg") % product_name % year % day).str();
		const path ref_fig_path = workspace_path / "Ref_Map" / ref_folder_name / year / ref_fig_name;
		BOOST_LOG_TRIVIAL(debug) << "Ref fig path:" << ref_fig_path;

		const string fig_title = (boost::format("%1%%2% in %3%-%4%-%5% (DOY %6%)") % product_type % product_name % year % it->month() % it->day_number() % day).str();
		BOOST_LOG_TRIVIAL(debug) << "Fig title: " << fig_title;

		const string yml_str = adsma::get_plot_eddyfield_yml_str(
			ref_tif_path, ref_fig_path, fig_title, "bar title", fig_extent,
			shp_bound_path, shp_fault_path, shp_city_path, quake_record_path);

		// 生成的yml文件名，例如 pl_ref_ef1_mod_bt_2018_303.yml
		const string yml_file_name = (boost::format("pl_ref_ef%1%_%2%_%3%_%4%_%5%.yml") %
			ano_method % boost::to_lower_copy(product_type) % boost::to_lower_copy(product_name) %
			year % day).str();
		const path yml_path = yml_folder_path / yml_file_name;
		BOOST_LOG_TRIVIAL(debug) << "Yml path: " << yml_path;

		modis_api::File_operation::write_to_file(yml_path.string(), yml_str);
	}

	return EXIT_SUCCESS;
}

/**
 * \brief 生成涡度出图yml文件
 * \param workspace_path 
 * \param tmp_path 
 * \param product_name 
 * \param product_type 
 * \param start_date 
 * \param end_date 
 * \param calc_ref 
 * \param calc_ano 
 * \param ano_method 
 * \param fig_extent 
 * \param shp_bound_path 
 * \param shp_fault_path 
 * \param shp_city_path 
 * \param quake_record_path 
 * \param yml_folder_path 
 * \return 
 */
int adsma::generate_plot_eddyfield_ano_yml_files(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product_name,
	const std::string& product_type,
	const boost::gregorian::date& start_date,
	const boost::gregorian::date& end_date,
	bool calc_ref, bool calc_ano,
	int ano_method, const std::string& fig_extent,
	const std::filesystem::path& shp_bound_path,
	const std::filesystem::path& shp_fault_path,
	const std::filesystem::path& shp_city_path,
	const std::filesystem::path& quake_record_path,
	const std::filesystem::path& yml_folder_path)
{
	using namespace std;
	using namespace filesystem;

	for (boost::gregorian::day_iterator it(start_date); it <= end_date; ++it)
	{
		const string year_and_day = modis_api::Date_utils::get_doy_str(*it);
		const string year = modis_api::Date_utils::get_doy_year(year_and_day);
		const string day = modis_api::Date_utils::get_doy_day(year_and_day);
		// Example: EF1_MOD_LST
		const string ref_folder_name = (boost::format("EF%1%_%2%_%3%") % ano_method % product_type % product_name).str();
		BOOST_LOG_TRIVIAL(debug) << "Ano folder name:" << ref_folder_name;

		const string ref_tif_name = (boost::format("%1%_ANO_%2%_%3%.tif") % product_name % year % day).str();
		const path ref_tif_path = workspace_path / "Ano" / ref_folder_name / year / ref_tif_name;
		BOOST_LOG_TRIVIAL(debug) << "Ano tif path:" << ref_tif_path;

		const string ref_fig_name = (boost::format("%1%_ANO_%2%_%3%.jpg") % product_name % year % day).str();
		const path ref_fig_path = workspace_path / "Ano_Map" / ref_folder_name / year / ref_fig_name;
		BOOST_LOG_TRIVIAL(debug) << "Ano fig path:" << ref_fig_path;

		const string fig_title = (boost::format("%1%%2% in %3%-%4%-%5% (DOY %6%)") % product_type % product_name % year % it->month() % it->day_number() % day).str();
		BOOST_LOG_TRIVIAL(debug) << "Fig title: " << fig_title;

		const string yml_str = adsma::get_plot_eddyfield_yml_str(
			ref_tif_path, ref_fig_path, fig_title, "bar title", fig_extent,
			shp_bound_path, shp_fault_path, shp_city_path, quake_record_path);

		// 生成的yml文件名，例如 pl_ref_ef1_mod_bt_2018_303.yml
		const string yml_file_name = (boost::format("pl_ano_ef%1%_%2%_%3%_%4%_%5%.yml") %
			ano_method % boost::to_lower_copy(product_type) % boost::to_lower_copy(product_name) %
			year % day).str();
		const path yml_path = yml_folder_path / yml_file_name;
		BOOST_LOG_TRIVIAL(debug) << "Yml path: " << yml_path;

		modis_api::File_operation::write_to_file(yml_path.string(), yml_str);
	}

	return EXIT_SUCCESS;
}

std::string adsma::get_plot_eddyfield_yml_str(
	const std::filesystem::path& input_file_path,
	const std::filesystem::path& output_file_path,
	const std::string& fig_title, const std::string& fig_bar_title,
	const std::string& fig_extent, const std::filesystem::path& shp_bound_path,
	const std::filesystem::path& shp_fault_path, const std::filesystem::path& shp_city_path,
	const std::filesystem::path& quake_record_path)
{
	using namespace std;
	using namespace std::filesystem;
	using namespace YAML;
	Emitter emt;
	emt << BeginMap;
	emt << Key << "PlotTitle" << Value << fig_title;
	emt << Key << "PlotTitleSize" << Value << PLOT_TITLE_SIZE;
	emt << Key << "PlotExtent" << Value << fig_extent;
	emt << Key << "BarName" << Value << REF_BAR_NAME;
	emt << Key << "BarTitle" << Value << fig_bar_title;
	emt << Key << "BarTitleSize" << Value << BAR_TITLE_SIZE;
	emt << Key << "ShpBoundary" << Value << shp_bound_path.string();
	emt << Key << "ShpFault" << Value << shp_fault_path.string();
	emt << Key << "ShpCity" << Value << shp_city_path.string();
	emt << Key << "QuakeRecord" << Value << quake_record_path.string();
	emt << Key << "InputFile" << Value << input_file_path.string();
	emt << Key << "OutputFile" << Value << output_file_path.string();
	emt << Key << "OutputDpi" << Value << 600;
	emt << Key << "OutputSize" << Value << "12,8";
	emt << EndMap;
	return emt.c_str();
}




