#include "stdafx.h"
#include "Mrt_utils.h"
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "Program_operation.h"

namespace  fs = boost::filesystem;
std::string modis_api::Mrt_utils::load_template_string(const  std::string& file_path)
{
	std::ifstream ifs(file_path);
	if (ifs)
	{
		std::stringstream ss;
		ss << ifs.rdbuf();
		BOOST_LOG_TRIVIAL(debug) << "�Ѽ���" << file_path << "�ļ�";
		return ss.str();
	}
	throw  std::runtime_error("load file " + file_path + " failed!");
}

modis_api::Mrt_utils::Mrt_utils() = default;


modis_api::Mrt_utils::~Mrt_utils() = default;

void modis_api::Mrt_utils::run_mrt(cs input_file_name,
	double min_lon, double max_lon, double min_lat, double max_lat,
	cs output_file_name, cs temp_dir, cs spatial_subset_type, cs resampling_type, cs output_projection_type,
	cs output_projection_parameters, cs datum, double output_pixel_size)
{
	BOOST_LOG_TRIVIAL(debug) << "����MRT��ȡ.tif�ļ��������ļ�Ϊ��" << input_file_name;
	BOOST_LOG_TRIVIAL(debug) << "��ȡ��������" << temp_dir << "Ŀ¼�����";
	if (!boost::filesystem::exists(temp_dir))
	{
		boost::filesystem::create_directories(temp_dir);
		BOOST_LOG_TRIVIAL(debug) << "�Ѵ���Ŀ¼" << temp_dir;
	}

	try
	{
		//fs::path current_path = fs::current_path();
		std::string current_path = fs::current_path().string();
		std::string mrt_prm_template_path = current_path + "//templates//MRT.tt";
		if (!fs::exists(mrt_prm_template_path))
			throw  std::runtime_error(boost::str(boost::format("δ�ҵ�MRT��Prmģ���ļ�%1%") % mrt_prm_template_path));
		std::string mrt_prm_template = load_template_string(mrt_prm_template_path);
		std::string mrt_prm_str = str(boost::format(mrt_prm_template) % input_file_name %spatial_subset_type
			% max_lat % min_lon % min_lat % max_lon % output_file_name
			% resampling_type % output_projection_type % output_projection_parameters % datum % output_pixel_size);
		//MRT .prm�ļ�����·��
		const  std::string mrt_prm_path = temp_dir + fs::path(input_file_name).stem().string() + ".prm";
		BOOST_LOG_TRIVIAL(debug) << "Prm�ļ����ݣ�\n" << mrt_prm_str;
		if (fs::exists(mrt_prm_path)) fs::remove(mrt_prm_path);
		std::ofstream ofs(mrt_prm_path);
		if (ofs)
			ofs << mrt_prm_str;
		ofs.clear();
		ofs.close();
		BOOST_LOG_TRIVIAL(debug) << "Prm�ļ��ѱ�����" << mrt_prm_path;

		std::string mrt_home = current_path + "\\MRT\\";
		std::string mrt_bat_template_path = current_path + "\\templates\\MRT_RUN.tt";
		if (!fs::exists(mrt_bat_template_path))
			throw  std::runtime_error(boost::str(boost::format("δ�ҵ�MRT��Batģ���ļ�%1%") % mrt_bat_template_path));
		std::string mrt_bat_template = load_template_string(mrt_bat_template_path);
		std::string mrt_bat_str = str(boost::format(mrt_bat_template) % mrt_home % mrt_prm_path);
		BOOST_LOG_TRIVIAL(debug) << "Bat�ļ����ݣ�\n" << mrt_bat_str;
		const  std::string mrt_bat_path = temp_dir + fs::path(input_file_name).stem().string() + ".bat";
		if (fs::exists(mrt_bat_path)) fs::remove(mrt_bat_path);

		ofs.open(mrt_bat_path);
		if (ofs)
		{
			ofs << mrt_bat_str;
			ofs.flush();
		}
		BOOST_LOG_TRIVIAL(debug) << "Bat�ļ��ѱ�����" << mrt_bat_path;
		//string run_str = str(boost::format("cmd.exe /c %1%") % mrt_bat_path);

		//modis_api::Program_operation::run(run_str);
		system(mrt_bat_path.c_str());

		std::string generated_tif_file_path = temp_dir + fs::path(input_file_name).stem().string() + "_mrt.LST_Day_1km.tif";
		// ��ȡ��tif�ļ�������MRT�Զ������.LST_Day_1km��ɾ����
		if (!fs::exists(generated_tif_file_path))
		{
			BOOST_LOG_TRIVIAL(error) << "����MRT��ȡ.tif�ļ�ʧ�ܣ�δ�ҵ�" << generated_tif_file_path << "�ļ�";
			return;
		}
		fs::rename(generated_tif_file_path, output_file_name);
		BOOST_LOG_TRIVIAL(debug) << generated_tif_file_path << "�ļ���������Ϊ" << output_file_name;
		BOOST_LOG_TRIVIAL(debug) << "����MRT��ȡtif�ļ�����";
	}
	catch (std::exception& ex)
	{
		BOOST_LOG_TRIVIAL(error) << ex.what();
	}
}
