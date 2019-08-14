#include "pch.h"
#include "Adsma_settings.h"
#include "Preprocess_aod_wv_input.h"
#include <boost/log/trivial.hpp>
#include <string>

Preprocess_aod_wv_input::Preprocess_aod_wv_input(const YAML::Node& node,
	const std::string& product_name,const std::string& product_type)
	:Preprocess_input(node,product_name, product_type)
{
	using namespace adsma::settings;
	using namespace adsma::settings::yaml::preprocess;
	if (m_node[PREPROCESS][product_name].IsDefined())
	{
		const auto sub_node = m_node[PREPROCESS][product_name];
		m_resampling_type = sub_node["ResamplingType"].as<std::string>();
		m_output_projection_type = sub_node["OutputProjectionType"].as<std::string>();
		m_output_projection_parameters = sub_node["OutputProjectionParameters"].as<std::string>();
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "未找到相关预处理字段";
		exit(EXIT_FAILURE);
	}
}
