#include "stdafx.h"
#include "Heg_utils.h"
#include "Program_operation.h"
#include <boost/format/free_funcs.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <Windows.h>

std::string modis_api::Heg_utils::get_heg_prm_template()
{
	std::stringstream ss;
	ss << "NUM_RUNS = 1\n\nBEGIN\nINPUT_FILENAME = %1%\nOBJECT_NAME = %2%\n" <<
		"FIELD_NAME = %3%\n" << "BAND_NUMBER = %4%\n" <<
		"OUTPUT_PIXEL_SIZE_X = %5$.1f\n" << "OUTPUT_PIXEL_SIZE_Y = %6$.1f\n" <<
		"SPATIAL_SUBSET_UL_CORNER = ( %7$.6f %8$.6f )\n" << "SPATIAL_SUBSET_LR_CORNER = ( %9$.6f %10$.6f )\n" <<
		"RESAMPLING_TYPE = %11%\n" << "OUTPUT_PROJECTION_TYPE = %12%\n" <<
		"ELLIPSOID_CODE = %13%\n" << "OUTPUT_PROJECTION_PARAMETERS = %14%\n" <<
		"OUTPUT_FILENAME = %15%\n" << "OUTPUT_TYPE = %16%\nEND\n";
	return ss.str();
}

std::string modis_api::Heg_utils::get_heg_bat_template()
{
	std::stringstream ss;
	ss << "cd %1%bin\nset MRTDATADIR=\"%1%data\\\"\n" << "set LD_LIBRARY_PATH=\"%1%bin\\\"\n" << "set MRTBINDIR=\"%1%bin\\\"\n"
		<< "set PGSHOME=\"%1%TOOLKIT_MTD\\\"\n\n" << "%1%bin\\swtif.exe -p %2%";
	return ss.str();
}

modis_api::Heg_utils::Heg_utils() = default;


modis_api::Heg_utils::~Heg_utils() = default;

void modis_api::Heg_utils::run_heg(cs input_file_name, cs object_name, cs field_name, int band_number,
	double output_pixel_size_x, double output_pixel_size_y, double min_lon, double max_lon, double min_lat,
	double max_lat, cs resampling_type, cs output_projection_type, cs ellipsoid_code, cs output_projection_parameters,
	cs output_filename, cs output_type, cs temp_dir)
{
	using namespace std;
	using namespace filesystem;
	path tmp_folder_path(temp_dir);

	BOOST_LOG_TRIVIAL(debug) << "����Heg��ȡ.tif�ļ��������ļ���" << input_file_name;
	BOOST_LOG_TRIVIAL(debug) << "��ȡ��������" << tmp_folder_path.string() << "Ŀ¼�����";

	if (!exists(tmp_folder_path))
	{
		create_directories(tmp_folder_path);
		BOOST_LOG_TRIVIAL(debug) << "�Ѵ���Ŀ¼" << tmp_folder_path;
	}

	try
	{
		std::string current_path = filesystem::current_path().string();
		// �ҵ�����Ŀ¼�µ�Heg_prm.tt�ļ������ļ���HEGʹ�õ�PRM�ļ���һ��ģ��
		//std::string heg_prm = load_template_string(current_path + "\\templates\\HEG.tt");
		std::string heg_prm = get_heg_prm_template();

		boost::format fmter;

		// ��ģ�������������
		std::string prm_str = str(boost::format(heg_prm) % input_file_name % object_name % field_name % band_number
			% output_pixel_size_x % output_pixel_size_y % max_lat % min_lon % min_lat % max_lon % resampling_type
			% output_projection_type % ellipsoid_code % output_projection_parameters % output_filename % output_type);
		BOOST_LOG_TRIVIAL(info) << prm_str;
		//����prm�ļ�
		const std::string prm_path = (tmp_folder_path / (path(input_file_name).stem().string() + ".prm")).string();
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
		//const std::string heg_bat = load_template_string(current_path + "\\templates\\HEG_RUN.tt");
		const std::string heg_bat = get_heg_bat_template();
		const std::string bat_str = str(boost::format(heg_bat) % heg_home % prm_path);
		BOOST_LOG_TRIVIAL(debug) << "Bat�ļ����ݣ�\n" << bat_str;

		const std::string bat_path = (tmp_folder_path / (path(input_file_name).stem().string() + ".bat")).string();
		ofs.open(bat_path);
		if (ofs)
		{
			ofs << bat_str;
			ofs.flush();
		}
		BOOST_LOG_TRIVIAL(debug) << "Bat�ļ��ѱ�����" << prm_path;

		//! �°�HEG����ʹ��system������
		std::string run_str = str(boost::format("cmd.exe /c %1%") % bat_path);
		system(run_str.c_str());
		BOOST_LOG_TRIVIAL(debug) << "������heg�ű�����ȡ��.tif�ļ�*Ԥ��*��λ��" << output_filename;
	}
	catch (std::exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << "����Heg��ȡ.tif�ļ������쳣���쳣��Ϣ��" << e.what();
	}
}
