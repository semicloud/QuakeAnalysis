#include "Input_file.h"
#include <fstream>
#include "../modis_api/File_operation.h"
#include <boost/log/trivial.hpp>


proc_MxD021km::Input_file::Input_file(const std::string& bt_hdf_file, const std::string& sza_hdf_file, const std::string& cm_hdf_file)
	: _bt_hdf_file(bt_hdf_file), _sza_hdf_file(sza_hdf_file), _cm_hdf_file(cm_hdf_file)
{
}

proc_MxD021km::Input_file::~Input_file() = default;

std::vector<proc_MxD021km::Input_file> proc_MxD021km::Input_file::load(const std::string& file_path)
{
	std::vector<Input_file> ivec;
	std::vector<std::string> svec;
	try
	{
		svec = modis_api::File_operation::read_file_all_lines(file_path);
	}
	catch (std::exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << e.what();
	}
	if (svec.empty()) return ivec;
	auto it = svec.begin();
	while (it != svec.end())
	{
		if (*it == "#") ivec.emplace_back(*(it + 1), *(it + 2), *(it + 3));
		++it;
	}
	return ivec;
}
