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
	m_hdflist_file = node["HDFListFile"].as<string>();
	m_tmp_path = node["TmpPath"].as<string>();
	if (m_tmp_path[m_tmp_path.size() - 1] != '\\')
		m_tmp_path.push_back('\\');
	m_min_lon = node["MinLon"].as<double>();
	m_max_lon = node["MaxLon"].as<double>();
	m_min_lat = node["MinLat"].as<double>();
	m_max_lat = node["MaxLat"].as<double>();
	m_band = node["Band"].as<int>();
	m_mrt_kernel_type = node["MRTKernelType"].as<string>();
	m_mrt_projection_type = node["MRTProjectionType"].as<string>();
	m_mrt_projection_args = node["MRTProjectionArgs"].as<string>();
	//_mrt_projection_sphere = node["MRTProjectionSphere"].as<string>();
	m_mrt_pixel_size = node["MRTPixelSize"].as<string>();
	m_output_image_file = node["OutputImageFile"].as<string>();
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

