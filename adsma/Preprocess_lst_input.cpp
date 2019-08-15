#include "pch.h"
#include "Adsma_settings.h"
#include "Preprocess_lst_input.h"


Preprocess_lst_input::Preprocess_lst_input(
	const YAML::Node& node, const std::string& product_type)
	:Preprocess_input(node, adsma::settings::LST_NAME, product_type)
{
	using namespace adsma::settings;
	using namespace  adsma::settings::yaml::preprocess;

	const YAML::Node sub_node = m_node[PREPROCESS][LST_NAME];
	m_resampling_type = sub_node["ResamplingType"].as<std::string>();
	m_output_projection_type = sub_node["OutputProjectionType"].as<std::string>();
	m_output_projection_parameters = sub_node["OutputProjectionParameters"].as<std::string>();
}
