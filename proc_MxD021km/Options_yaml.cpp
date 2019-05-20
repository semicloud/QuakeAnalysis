#include "Options_yaml.h"
#include <yaml-cpp/yaml.h>
#include <boost/log/expressions/attr.hpp>
#include <boost/format/free_funcs.hpp>


proc_MxD021km::Options_yaml::Options_yaml(const YAML::Node& node)
{
	try
	{
		check_node(node);
	}
	catch (exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << e.what();
		exit(EXIT_FAILURE);
	}
	_input_hdf_file = node["InputHdfFile"].as<string>();
	_temp_dir = node["TempDir"].as<string>();
	if (_temp_dir[_temp_dir.size() - 1] != '\\')
		_temp_dir.push_back('\\');
	_min_lon = node["MinLon"].as<double>();
	_max_lon = node["MaxLon"].as<double>();
	_min_lat = node["MinLat"].as<double>();
	_max_lat = node["MaxLat"].as<double>();
	_band = node["Band"].as<std::string>();
	_mrt_kernel_type = node["MRTKernelType"].as<std::string>();
	_mrt_projection_type = node["MRTProjectionType"].as<std::string>();
	_mrt_projection_args = node["MRTProjectionArgs"].as<std::string>();
	//_mrt_projection_sphere = node["MRTProjectionSphere"].as<std::string>();
	_mrt_pixel_size = node["MRTPixelSize"].as<std::string>();
	_output_image_file = node["OutputImageFile"].as<std::string>();
}


proc_MxD021km::Options_yaml::~Options_yaml() = default;

void proc_MxD021km::Options_yaml::check_node(const YAML::Node& node)
{
	vector<string> node_names = { "InputHdfFile", "TempDir" ,"MinLon", "MaxLon", "MinLat", "MaxLat","Band",
		"MRTKernelType","MRTProjectionType","MRTProjectionArgs","MRTPixelSize","OutputImageFile" };
	for (const auto& node_name : node_names)
		if (!node[node_name]) // Node²»´æÔÚ
			throw runtime_error(boost::str(boost::format("node %1% not found!") % node_name));
}

