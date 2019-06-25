#include "stdafx.h"
#include "Mrt_swath_prm_run.h"
#include <fstream>
#include <sstream>
#include <boost/format/free_funcs.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

modis_api::Mrt_swath_prm_run::Mrt_swath_prm_run(const std::string& mrt_swath_path, const std::string& prm_file_path)
{
	_mrt_swath_path = mrt_swath_path;
	_prm_file_path = prm_file_path;
}

modis_api::Mrt_swath_prm_run::~Mrt_swath_prm_run()
{
}

void modis_api::Mrt_swath_prm_run::export_prm_bat_file(const std::string& dest)
{
	std::string content = get_prm_bat_content();
	std::ofstream ofs(dest);
	ofs << content;
	ofs.close();
}

/**
 * \brief 获取运行Prm文件的bat脚本内容
 * \return
 */
std::string modis_api::Mrt_swath_prm_run::get_prm_bat_content()
{
	
	//std::ifstream ifs("RunPrm.tt");
	std::string current_path = boost::filesystem::current_path().string();
	std::ifstream ifs(current_path + "\\templates\\MRT_SWATH_RUN.tt");
	std::stringstream ss;
	ss << ifs.rdbuf();

	std::string tem = str(boost::format(ss.str()) % _mrt_swath_path % _prm_file_path);
	ifs.close();
	return tem;
}


