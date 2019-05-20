#include "stdafx.h"
#include "Hdf_to_gtiff.h"
#include <boost/filesystem.hpp>
#include "Mrt_swath_prm_run.h"
#include <boost/log/trivial.hpp>

namespace fs = boost::filesystem;

void Hdf_to_gtiff::do_convert()
{
	//TODO ����ǰ�Ļ�������
	BOOST_LOG_TRIVIAL(debug) << "------------------------------------------------------";
	const string hdf_file_name = fs::path(_prm_setting->get_input_hdf_file()).filename().string();
	const string tif_file_name = fs::path(_prm_setting->get_output_gtiff_file()).filename().string();
	BOOST_LOG_TRIVIAL(debug) << "MRT CONVERT:";
	BOOST_LOG_TRIVIAL(debug) << "FROM: " << hdf_file_name;
	BOOST_LOG_TRIVIAL(debug) << "TO: " << tif_file_name;

	const string prm_file_path = _convert_at + "hdf2tiff.prm";
	_prm_file->export_prm_file(prm_file_path);

	const string prm_bat_file_path = _convert_at + "run.bat";
	Mrt_swath_prm_run prm_bat_file(fs::current_path().string() + "\\MRTSwath\\", prm_file_path);
	prm_bat_file.export_prm_bat_file(prm_bat_file_path);

	int ans = system(prm_bat_file_path.c_str());
	BOOST_LOG_TRIVIAL(debug) << "����SWATH2GRID.EXE����������" << ans;
	BOOST_LOG_TRIVIAL(info) << "��" << hdf_file_name << "����ȡtif�ļ�" << tif_file_name << "�ɹ�";
	BOOST_LOG_TRIVIAL(debug) << "------------------------------------------------------";
}
