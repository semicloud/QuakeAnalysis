#include "stdafx.h"
#include "Heg_utils.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <boost/log/trivial.hpp>
#include <boost/format/free_funcs.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <Windows.h>
#include "Program_operation.h"

std::string modis_api::Heg_utils::load_template_string(const std::string& file_path)
{
	std::ifstream ifs(file_path);
	if (ifs)
	{
		std::stringstream ss;
		ss << ifs.rdbuf();
		ifs.close();
		BOOST_LOG_TRIVIAL(debug) << "�Ѽ���" << file_path << "�ļ�";
		return ss.str();
	}
	throw std::runtime_error("load file " + file_path + " failed!");
}

modis_api::Heg_utils::Heg_utils() = default;


modis_api::Heg_utils::~Heg_utils() = default;

void modis_api::Heg_utils::run_heg(cs input_file_name, cs object_name, cs field_name, int band_number,
	double output_pixel_size_x, double output_pixel_size_y, double min_lon, double max_lon, double min_lat,
	double max_lat, cs resampling_type, cs output_projection_type, cs ellipsoid_code, cs output_projection_parameters,
	cs output_filename, cs output_type, cs temp_dir)
{
	BOOST_LOG_TRIVIAL(debug) << "����Heg��ȡ.tif�ļ��������ļ���" << input_file_name;
	BOOST_LOG_TRIVIAL(debug) << "��ȡ��������" << temp_dir << "Ŀ¼�����";
	if (!boost::filesystem::exists(temp_dir))
	{
		boost::filesystem::create_directories(temp_dir);
		BOOST_LOG_TRIVIAL(debug) << "�Ѵ���Ŀ¼" << temp_dir;
	}

	try
	{
		std::string current_path = boost::filesystem::current_path().string();
		// �ҵ�����Ŀ¼�µ�Heg_prm.tt�ļ������ļ���HEGʹ�õ�PRM�ļ���һ��ģ��
		std::string heg_prm = load_template_string(current_path + "\\templates\\HEG.tt");
		// ��ģ�������������
		std::string prm_str = str(boost::format(heg_prm) % input_file_name % object_name % field_name % band_number
			% output_pixel_size_x % output_pixel_size_y % max_lat % min_lon % min_lat % max_lon % resampling_type
			% output_projection_type % ellipsoid_code % output_projection_parameters % output_filename % output_type);
		//BOOST_LOG_TRIVIAL(info) << prm_str;
		//����prm�ļ�
		const std::string prm_path = temp_dir + boost::filesystem::path(input_file_name).stem().string() + ".prm";
		BOOST_LOG_TRIVIAL(debug) << "Prm�ļ����ݣ�\n" << prm_str;
		std::ofstream ofs(prm_path);
		if (ofs)
		{
			ofs << prm_str;
			ofs.flush();
		}
		ofs.close(); // ���Ҫ���´򿪣������ȹر�
		BOOST_LOG_TRIVIAL(debug) << "Prm�ļ��ѱ�����" << prm_path;

		// hegҪ���Prm�ļ�ΪUnix�ģ����Ե���dos_2_unixת��һ��
		const std::string dos_2_unix = current_path + "\\dos2unix.exe " + prm_path;
		system(dos_2_unix.c_str());
		BOOST_LOG_TRIVIAL(debug) << "�ѵ���dos2unix.exe��" << prm_path << "�ļ�ת��ΪUnix��ʽ";

		const std::string heg_home = current_path + "\\HEG_Win\\";
		//����HEG��bat�ļ�ģ��
		const std::string heg_bat = load_template_string(current_path + "\\templates\\HEG_RUN.tt");
		const std::string bat_str = str(boost::format(heg_bat) % heg_home % prm_path);
		BOOST_LOG_TRIVIAL(debug) << "Bat�ļ����ݣ�\n" << bat_str;

		const std::string bat_path = temp_dir + boost::filesystem::path(input_file_name).stem().string() + ".bat";
		ofs.open(bat_path);
		if (ofs)
		{
			ofs << bat_str;
			ofs.flush();
		}
		BOOST_LOG_TRIVIAL(debug) << "Bat�ļ��ѱ�����" << prm_path;

		//o(�s���t)o HEG����ô��ţ�ƣ�system���в�������ֻ�ܵ���WindowsAPI CreateProcess������
		std::string run_str = str(boost::format("cmd.exe /c %1%") % bat_path);
		Program_operation::run(run_str);
		BOOST_LOG_TRIVIAL(debug) << "������heg�ű�����ȡ��.tif�ļ�*Ԥ��*��λ��" << output_filename;
	}
	catch (std::exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << "����Heg��ȡ.tif�ļ������쳣���쳣��Ϣ��" << e.what();
	}
}
