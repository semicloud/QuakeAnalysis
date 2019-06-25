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
		BOOST_LOG_TRIVIAL(debug) << "已加载" << file_path << "文件";
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
	BOOST_LOG_TRIVIAL(debug) << "调用Heg提取.tif文件，输入文件：" << input_file_name;
	BOOST_LOG_TRIVIAL(debug) << "提取操作将在" << temp_dir << "目录下完成";
	if (!boost::filesystem::exists(temp_dir))
	{
		boost::filesystem::create_directories(temp_dir);
		BOOST_LOG_TRIVIAL(debug) << "已创建目录" << temp_dir;
	}

	try
	{
		std::string current_path = boost::filesystem::current_path().string();
		// 找到程序目录下的Heg_prm.tt文件，该文件是HEG使用的PRM文件的一个模板
		std::string heg_prm = load_template_string(current_path + "\\templates\\HEG.tt");
		// 向模板里面填入参数
		std::string prm_str = str(boost::format(heg_prm) % input_file_name % object_name % field_name % band_number
			% output_pixel_size_x % output_pixel_size_y % max_lat % min_lon % min_lat % max_lon % resampling_type
			% output_projection_type % ellipsoid_code % output_projection_parameters % output_filename % output_type);
		//BOOST_LOG_TRIVIAL(info) << prm_str;
		//保存prm文件
		const std::string prm_path = temp_dir + boost::filesystem::path(input_file_name).stem().string() + ".prm";
		BOOST_LOG_TRIVIAL(debug) << "Prm文件内容：\n" << prm_str;
		std::ofstream ofs(prm_path);
		if (ofs)
		{
			ofs << prm_str;
			ofs.flush();
		}
		ofs.close(); // 如果要重新打开，必须先关闭
		BOOST_LOG_TRIVIAL(debug) << "Prm文件已保存至" << prm_path;

		// heg要求的Prm文件为Unix的，所以调用dos_2_unix转换一下
		const std::string dos_2_unix = current_path + "\\dos2unix.exe " + prm_path;
		system(dos_2_unix.c_str());
		BOOST_LOG_TRIVIAL(debug) << "已调用dos2unix.exe将" << prm_path << "文件转换为Unix格式";

		const std::string heg_home = current_path + "\\HEG_Win\\";
		//调用HEG的bat文件模板
		const std::string heg_bat = load_template_string(current_path + "\\templates\\HEG_RUN.tt");
		const std::string bat_str = str(boost::format(heg_bat) % heg_home % prm_path);
		BOOST_LOG_TRIVIAL(debug) << "Bat文件内容：\n" << bat_str;

		const std::string bat_path = temp_dir + boost::filesystem::path(input_file_name).stem().string() + ".bat";
		ofs.open(bat_path);
		if (ofs)
		{
			ofs << bat_str;
			ofs.flush();
		}
		BOOST_LOG_TRIVIAL(debug) << "Bat文件已保存至" << prm_path;

		//o(╯□╰)o HEG是这么的牛逼，system运行不出来，只能调用WindowsAPI CreateProcess来运行
		std::string run_str = str(boost::format("cmd.exe /c %1%") % bat_path);
		Program_operation::run(run_str);
		BOOST_LOG_TRIVIAL(debug) << "已运行heg脚本，提取的.tif文件*预计*将位于" << output_filename;
	}
	catch (std::exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << "调用Heg提取.tif文件出现异常，异常消息：" << e.what();
	}
}
