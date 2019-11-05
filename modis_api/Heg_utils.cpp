#include "stdafx.h"
#include "Heg_utils.h"
#include "File_operation.h"
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

void modis_api::Heg_utils::run_heg(Heg_param const& hp)
{
	path tmp_folder(hp.temp_folder());
	BOOST_LOG_TRIVIAL(debug) << "调用Heg提取.tif文件，输入文件：" << hp.input_file();
	BOOST_LOG_TRIVIAL(debug) << "提取操作将在" << tmp_folder << "目录下完成";
	File_operation::create_directories_if_not_exist(tmp_folder);

	try
	{
		std::string current_path = std::filesystem::current_path().string();
		std::string heg_prm = get_heg_prm_template();

		// 向模板里面填入参数
		std::string prm_str = boost::str(boost::format(heg_prm) % hp.input_file().string() % hp.object_name() % hp.field_name() % hp.band_number()
			% hp.output_pixel_size_x() % hp.output_pixel_size_y() % hp.max_lat() % hp.min_lon() % hp.min_lat() % hp.max_lon() % hp.resampling_type()
			% hp.output_projection_type() % hp.ellipsoid_code() % hp.output_projection_parameters() % hp.output_file().string() % hp.output_type());
		//保存prm文件
		const path prm_path = File_operation::get_tmp_file(hp.input_file(), tmp_folder, "", "prm");
		File_operation::write_to_file(prm_path.string(), prm_str);
		BOOST_LOG_TRIVIAL(debug) << "Prm文件已保存至" << prm_path << "，文件内容：" << prm_str;

		// heg要求的Prm文件为Unix的，所以调用dos_2_unix转换一下
		const std::string dos2unix = current_path + "\\dos2unix.exe " + prm_path.string();
		system(dos2unix.c_str());
		BOOST_LOG_TRIVIAL(debug) << "已调用dos2unix.exe将" << prm_path << "文件转换为Unix格式";

		const std::string heg_home = current_path + "\\HEG_Win\\";
		//调用HEG的bat文件模板
		//const std::string heg_bat = load_template_string(current_path + "\\templates\\HEG_RUN.tt");
		const std::string heg_bat = get_heg_bat_template();
		const std::string bat_str = str(boost::format(heg_bat) % heg_home % prm_path);
		BOOST_LOG_TRIVIAL(debug) << "Bat文件内容：\n" << bat_str;

		const std::string bat_path = (tmp_folder / (hp.input_file().stem().string() + ".bat")).string();
		File_operation::write_to_file(bat_path, bat_str);
		BOOST_LOG_TRIVIAL(debug) << "Bat文件已保存至" << prm_path;

		//! 新版HEG可以使用system运行了
		std::string run_str = str(boost::format("cmd.exe /c %1%") % bat_path);
		system(run_str.c_str());
		BOOST_LOG_TRIVIAL(debug) << "已运行heg脚本，提取的.tif文件*预计*将位于" << hp.output_file();

		if (!std::filesystem::exists(hp.output_file()))
		{
			BOOST_LOG_TRIVIAL(error) << "未找到heg提取的.tif文件：" << hp.output_file();
			BOOST_LOG_TRIVIAL(error) << "程序退出";
			exit(EXIT_FAILURE);
		}
	}
	catch (std::exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << "调用Heg提取.tif文件出现异常，异常消息：" << e.what();
	}
}
