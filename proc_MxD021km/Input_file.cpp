#include "Input_file.h"
#include "../modis_api/File_operation.h"
#include <boost/log/trivial.hpp>

adsma::Input_file::Input_file(const std::string& mxd02_file, const std::string& mxd03_file, const std::string& mxd35_file)
	: m_mxd02_file(mxd02_file), m_mxd03_file(mxd03_file), m_mxd35_file(mxd35_file)
{
}

adsma::Input_file::~Input_file() = default;

std::vector<adsma::Input_file> adsma::Input_file::load(const std::string& file_path)
{
	using namespace std;
	vector<Input_file> ivec;
	vector<string> svec;
	try
	{
		svec = modis_api::File_operation::read_file_all_lines(file_path);
	}
	catch (exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << e.what();
	}
	if (svec.empty()) return ivec;
	vector<string>::iterator it = svec.begin();
	while (it != svec.end())
	{
		if (*it == "#") ivec.emplace_back(*(it + 1), *(it + 2), *(it + 3));
		++it;
	}
	return ivec;
}
