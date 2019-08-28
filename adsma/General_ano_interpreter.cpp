#include "pch.h"
#include "General_ano_interpreter.h"
#include "Adsma_settings.h"
#include "Preprocess_helper.h"
#include "../modis_api/File_operation.h"
#include <boost/log/trivial.hpp>
#include <yaml-cpp/yaml.h>
#include <sstream>
#include <iterator>

std::string adsma::get_general_ano_yml_str(
	const std::filesystem::path& month_list_file,
	const std::filesystem::path& ref_list_file,
	const std::filesystem::path& output_image_file,
	const std::filesystem::path& tmp_path)
{
	YAML::Emitter emitter;
	emitter << YAML::BeginMap;
	emitter << YAML::Key << "MonthListFile"  << YAML::Value << month_list_file.string();
	emitter << YAML::Key << "RefListFile" << YAML::Value << ref_list_file.string();
	emitter << YAML::Key << "OutputImageFile" << YAML::Value << output_image_file.string();
	emitter << YAML::Key << "TmpPath" << YAML::Value << tmp_path.string();
	emitter << YAML::EndMap;
	std::string s = emitter.c_str();
	return s;
}

int adsma::get_general_ano_month_and_ref_hdflist_str(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product_name,
	const std::string& product_type,
	unsigned year, unsigned month,
	std::string& month_hdflist_str, std::string& ref_hdflist_str)
{
	const std::filesystem::path dir_data = workspace_path /
		adsma::settings::PP_STANDARD_FOLDER /
		get_pp_folder(product_type, product_name);
	// 当前年份当前月份的tif文件
	std::vector<std::filesystem::path> month_tif_files;
	// 历史年份当前月份的tif文件
	std::vector<std::filesystem::path> ref_tif_files;

	for (std::filesystem::recursive_directory_iterator it(dir_data);
		it != std::filesystem::recursive_directory_iterator(); ++it)
	{
		if (it->path().extension() != ".tif") continue;
		std::string file_name_without_extension =
			it->path().filename().stem().string();
		std::vector<std::string> file_name_components;
		boost::split(file_name_components, file_name_without_extension,
			boost::is_any_of("_"));
		const unsigned file_year =
			boost::lexical_cast<unsigned>(file_name_components[1]);
		std::string file_day_str =
			boost::trim_left_copy_if(file_name_components[2], boost::is_any_of("0"));
		const unsigned file_day = boost::lexical_cast<unsigned>(file_day_str);
		const unsigned file_month = (boost::gregorian::date(file_year, 1, 1)
			+ boost::gregorian::days(file_day - 1)).month().as_number();
		if (file_year == year && file_month == month)
		{
			month_tif_files.push_back(it->path());
		}
		else if (file_year < year && file_month == month)
		{
			ref_tif_files.push_back(it->path());
		}
	}

	if (month_tif_files.empty())
	{
		BOOST_LOG_TRIVIAL(error) << "不存在" << year << "-" << month << "的"
			<< product_type << "_" << product_name << "数据，无法进行年变计算";
		exit(EXIT_FAILURE);
	}

	if (ref_tif_files.empty())
	{
		BOOST_LOG_TRIVIAL(error) << "不存在" << year << "-" << month << "的"
			<< product_type << "_" << product_name << "背景场数据，无法进行年变计算";
		exit(EXIT_FAILURE);
	}

	std::stringstream ss_month_tif_files;
	for (const std::filesystem::path& p : month_tif_files)
		ss_month_tif_files << p.string() << "\n";
	std::stringstream ss_ref_tif_files;
	for (const std::filesystem::path& p : ref_tif_files)
		ss_ref_tif_files << p.string() << "\n";
	month_hdflist_str = ss_month_tif_files.str();
	ref_hdflist_str = ss_ref_tif_files.str();
	return 0;
}

int adsma::generate_general_ano_yml_hdflist_files(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product_name,
	const std::string& product_type,
	const boost::gregorian::date& start_date,
	const boost::gregorian::date& end_date,
	const std::filesystem::path& yml_folder_path)
{
	for (boost::gregorian::month_iterator it(start_date); it <= end_date; ++it)
	{
		const unsigned year = it->year();
		const unsigned month = it->month().as_number();

		std::string  month_tif_files_str, ref_tif_files_str;
		get_general_ano_month_and_ref_hdflist_str(workspace_path, tmp_path,
			product_name, product_type, year, month, month_tif_files_str,
			ref_tif_files_str);
		// ga_mod_bt_2018_1.txt
		const std::string month_tif_file_name =
			(boost::format("ga_%1%_%2%_%3%_%4%.txt")
				% boost::to_lower_copy(product_type)
				% boost::to_lower_copy(product_name) % year % month).str();
		// ga_ref_mon_2018_1.txt
		const std::string ref_tif_file_name =
			(boost::format("ga_ref_%1%_%2%_%3%_%4%.txt")
				% boost::to_lower_copy(product_type)
				% boost::to_lower_copy(product_name) % year % month).str();
		const std::filesystem::path month_tif_file_path
			= yml_folder_path / month_tif_file_name;
		const std::filesystem::path ref_tif_file_path
			= yml_folder_path / ref_tif_file_name;

		const std::filesystem::path output_image_path
			= workspace_path / "Ano" /
			(boost::format("GA_%1%_%2%") % product_type % product_name).str() /
			std::to_string(year) /
			(boost::format("%1%_ANO_%2%_%3%.tif") % product_name % year % month).str();

		const std::string yml_file_str = get_general_ano_yml_str(
			month_tif_file_path, ref_tif_file_path, output_image_path, tmp_path
		);
		const std::filesystem::path yml_file_path = yml_folder_path /
			(boost::format("ga_%1%_%2%_%3%_%4%.yml") %
				boost::to_lower_copy(product_type) %
				boost::to_lower_copy(product_name) % year % month).str();
		modis_api::File_operation::write_to_file(yml_file_path.string(), yml_file_str);
		modis_api::File_operation::write_to_file(month_tif_file_path.string(), month_tif_files_str);
		modis_api::File_operation::write_to_file(ref_tif_file_path.string(), ref_tif_files_str);
	}
	return 0;
}


