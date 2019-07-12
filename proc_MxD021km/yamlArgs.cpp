#include "yamlArgs.h"
#include <yaml-cpp/yaml.h>
#include <boost/log/expressions/attr.hpp>
#include <boost/format/free_funcs.hpp>

adsma::yamlArgs::yamlArgs(const YAML::Node& node)
{
	using namespace std;
	try
	{
		check_node(node);
	}
	catch (std::exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << e.what();
		exit(EXIT_FAILURE);
	}
	_input_hdf_file = node["HDFListFile"].as<string>();
	_temp_dir = node["TmpPath"].as<string>();
	if (_temp_dir[_temp_dir.size() - 1] != '\\')
		_temp_dir.push_back('\\');
	_min_lon = node["MinLon"].as<double>();
	_max_lon = node["MaxLon"].as<double>();
	_min_lat = node["MinLat"].as<double>();
	_max_lat = node["MaxLat"].as<double>();
	_band = node["Band"].as<int>();
	_mrt_kernel_type = node["MRTKernelType"].as<string>();
	_mrt_projection_type = node["MRTProjectionType"].as<string>();
	_mrt_projection_args = node["MRTProjectionArgs"].as<string>();
	//_mrt_projection_sphere = node["MRTProjectionSphere"].as<string>();
	_mrt_pixel_size = node["MRTPixelSize"].as<string>();
	_output_image_file = node["OutputImageFile"].as<string>();
}


adsma::yamlArgs::~yamlArgs() = default;

void adsma::yamlArgs::check_node(const YAML::Node& node)
{
	std::vector<std::string> node_names = { "HDFListFile", "TmpPath" ,"MinLon", "MaxLon", "MinLat", "MaxLat","Band",
		"MRTKernelType","MRTProjectionType","MRTProjectionArgs","MRTPixelSize","OutputImageFile" };
	for (const auto& node_name : node_names)
		if (!node[node_name]) // Node²»´æÔÚ
			throw std::runtime_error(boost::str(boost::format("node %1% not found!") % node_name));
}

