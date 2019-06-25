#include "stdafx.h"
#include "Mrt_swath_prm.h"
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <fstream>
#include <sstream>


modis_api::Mrt_swath_prm::~Mrt_swath_prm()
{
}

std::string modis_api::Mrt_swath_prm::get_prm_file_content()
{
	//std::ifstream ifs("Prm.tt");
	std::string current_path = boost::filesystem::current_path().string() + "\\templates\\MRT_SWATH.tt";
	
	std::ifstream ifs(current_path);
	std::stringstream ss;
	ss << ifs.rdbuf();

	std::string tem = boost::str(boost::format(ss.str()) %
		_prm_setting.get_input_hdf_file() % _prm_setting.get_geo_loc_hdf_file() %
		_prm_setting.get_sds_str() % _prm_setting.get_ulc_lng() %
		_prm_setting.get_ulc_lat() % _prm_setting.get_lrc_lng() %
		_prm_setting.get_lrc_lat() % _prm_setting.get_output_gtiff_file());

	return tem;
}

void modis_api::Mrt_swath_prm::export_prm_file(const std::string& dest)
{
	const std::string content = get_prm_file_content();
	std::ofstream out(dest);
	out << content;
	out.close();
}
