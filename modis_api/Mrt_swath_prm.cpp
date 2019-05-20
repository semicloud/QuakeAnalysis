#include "stdafx.h"
#include <sstream>
#include <fstream>
#include <boost/format.hpp>
#include "Mrt_swath_prm.h"
#include <boost/filesystem.hpp>


modis_api::Mrt_swath_prm::~Mrt_swath_prm()
{
}

string modis_api::Mrt_swath_prm::get_prm_file_content()
{
	//std::ifstream ifs("Prm.tt");
	string current_path = boost::filesystem::current_path().string() + "\\templates\\MRT_SWATH.tt";
	
	std::ifstream ifs(current_path);
	std::stringstream ss;
	ss << ifs.rdbuf();

	string tem = str(boost::format(ss.str()) %
		_prm_setting.get_input_hdf_file() % _prm_setting.get_geo_loc_hdf_file() %
		_prm_setting.get_sds_str() % _prm_setting.get_ulc_lng() %
		_prm_setting.get_ulc_lat() % _prm_setting.get_lrc_lng() %
		_prm_setting.get_lrc_lat() % _prm_setting.get_output_gtiff_file());

	return tem;
}

void modis_api::Mrt_swath_prm::export_prm_file(const string& dest)
{
	const string content = get_prm_file_content();
	ofstream out(dest);
	out << content;
	out.close();
}
