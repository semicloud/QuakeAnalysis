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
			BOOST_LOG_TRIVIAL(warning) << "�������ļ�" << product_path << "���޷�ʹ�ø��ļ������жȱ�����";
			BOOST_LOG_TRIVIAL(warning) << __FUNCDNAME__ << "," << __LINE__;
			continue;
		}
		//tif_file_paths.push_back(product_path);
		oss << product_path.string() << endl;
	}
	return oss.str();
}

int adsma::generate_eddyfield_yml_hdflist_files(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product,
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
		product_type, product);

	for (day_iterator it(start_date); it <= end_date; ++it)
	{
		const string year_and_day = modis_api::Date_utils::get_doy_str(*it);
		const string year = modis_api::Date_utils::get_doy_year(year_and_day);
		const unsigned year_u = stoul(year);
		const string day = modis_api::Date_utils::get_doy_day(year_and_day);
		const path input_image_path = workspace_path / adsma::settings::PP_STANDARD_FOLDER
			/ get_pp_folder(product_type, product) / year
			/ get_pp_output_file_name(product, year, day);
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
