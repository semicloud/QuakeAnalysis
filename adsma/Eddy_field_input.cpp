#include "pch.h"
#include "Eddy_field_input.h"

Eddy_field_input::Eddy_field_input(const YAML::Node& n, const std::string& product_name,
	const std::string& product_type) : Common_input(n), m_product_name(product_name),
	m_product_type(product_type)
{
	const YAML::Node sub_node = m_node["EddyField"][product_name];
	assert(sub_node.IsDefined());
	m_is_calc_ref = sub_node["CalcRef"].as<bool>();
	m_is_calc_ano = sub_node["CalcAno"].as<bool>();
	m_ano_method = sub_node["AnoMethod"].as<int>();
	m_is_plot_ref = sub_node["PlotBackground"].IsDefined();
	m_is_plot_ano = sub_node["PlotEddyField"].IsDefined();
}
