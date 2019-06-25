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
		BOOST_LOG_TRIVIAL(debug) << "已加载" << file_path << "文件";
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
	BOOST_LOG_TRIVIAL(debug) << "调用MRT提取.tif文件，输入文件为：" << input_file_name;
	BOOST_LOG_TRIVIAL(debug) << "提取操作将在" << temp_dir << "目录下完成";
	if (!boost::filesystem::exists(temp_dir))
	{
		boost::filesystem::create_directories(temp_dir);
		BOOST_LOG_TRIVIAL(debug) << "已创建目录" << temp_dir;
	}

	try
	{
		//fs::path current_path = fs::current_path();
		std::string current_path = fs::current_path().string();
		std::string mrt_prm_template_path = current_path + "//templates//MRT.tt";
		if (!fs::exists(mrt_prm_template_path))
			throw  std::runtime_error(boost::str(boost::format("未找到MRT的Prm模板文件%1%") % mrt_prm_template_path));
		std::string mrt_prm_template = load_template_string(mrt_prm_template_path);
		std::string mrt_prm_str = str(boost::format(mrt_prm_template) % input_file_name %spatial_subset_type
			% max_lat % min_lon % min_lat % max_lon % output_file_name
			% resampling_type % output_projection_type % output_projection_parameters % datum % output_pixel_size);
		//MRT .prm文件保存路径
		const  std::string mrt_prm_path = temp_dir + fs::path(input_file_name).stem().string() + ".prm";
		BOOST_LOG_TRIVIAL(debug) << "Prm文件内容：\n" << mrt_prm_str;
		if (fs::exists(mrt_prm_path)) fs::remove(mrt_prm_path);
		std::ofstream ofs(mrt_prm_path);
		if (ofs)
			ofs << mrt_prm_str;
		ofs.clear();
		ofs.close();
		BOOST_LOG_TRIVIAL(debug) << "Prm文件已保存至" << mrt_prm_path;

		std::string mrt_home = current_path + "\\MRT\\";
		std::string mrt_bat_template_path = current_path + "\\templates\\MRT_RUN.tt";
		if (!fs::exists(mrt_bat_template_path))
			throw  std::runtime_error(boost::str(boost::format("未找到MRT的Bat模板文件%1%") % mrt_bat_template_path));
		std::string mrt_bat_template = load_template_string(mrt_bat_template_path);
		std::string mrt_bat_str = str(boost::format(mrt_bat_template) % mrt_home % mrt_prm_path);
		BOOST_LOG_TRIVIAL(debug) << "Bat文件内容：\n" << mrt_bat_str;
		const  std::string mrt_bat_path = temp_dir + fs::path(input_file_name).stem().string() + ".bat";
		if (fs::exists(mrt_bat_path)) fs::remove(mrt_bat_path);

		ofs.open(mrt_bat_path);
		if (ofs)
		{
			ofs << mrt_bat_str;
			ofs.flush();
		}
		BOOST_LOG_TRIVIAL(debug) << "Bat文件已保存至" << mrt_bat_path;
		//string run_str = str(boost::format("cmd.exe /c %1%") % mrt_bat_path);

		//modis_api::Program_operation::run(run_str);
		system(mrt_bat_path.c_str());

		std::string generated_tif_file_path = temp_dir + fs::path(input_file_name).stem().string() + "_mrt.LST_Day_1km.tif";
		// 提取的tif文件名中有MRT自动加入的.LST_Day_1km，删除掉
		if (!fs::exists(generated_tif_file_path))
		{
			BOOST_LOG_TRIVIAL(error) << "调用MRT提取.tif文件失败！未找到" << generated_tif_file_path << "文件";
			return;
		}
		fs::rename(generated_tif_file_path, output_file_name);
		BOOST_LOG_TRIVIAL(debug) << generated_tif_file_path << "文件已重命名为" << output_file_name;
		BOOST_LOG_TRIVIAL(debug) << "调用MRT提取tif文件结束";
	}
	catch (std::exception& ex)
	{
		BOOST_LOG_TRIVIAL(error) << ex.what();
	}
}
