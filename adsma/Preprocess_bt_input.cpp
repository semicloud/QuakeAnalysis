#include "pch.h"
#include "Adsma_settings.h"
#include "Preprocess_bt_input.h"


Preprocess_bt_input::Preprocess_bt_input(const YAML::Node& node,
	const std::string& product_type)
	: Preprocess_input(node, adsma::settings::BT_NAME, product_type)
{
	using namespace std;
	using namespace adsma::settings;
	using namespace adsma::settings::yaml::preprocess;
	const auto sub_node = m_node[PREPROCESS][BT_NAME];
	m_band = sub_node[BAND].as<int>();
	m_mrt_kernel_type = sub_node["MRTProjectionType"].as<string>();
	m_mrt_projection_type = sub_node["MRTProjectionType"].as<string>();
	m_mrt_projection_args = sub_node["MRTProjectionArgs"].as<string>();
	m_mrt_pixel_size = sub_node["MRTPixelSize"].as<float>();
}
