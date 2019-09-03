#include "pch.h"
#include "General_ano_input.h"


General_ano_input::General_ano_input(const YAML::Node& n, const std::string& product_name,
	const std::string& product_type)
	: Common_input(n), m_product_name(product_name), m_product_type(product_type)
{
	YAML::Node const sub_node = m_node["GeneralAno"][product_name];
	m_is_plot_ref = sub_node["PlotBackground"].as<bool>();
	m_is_plot_ano = sub_node["PlotGeneralAno"].as<bool>();
}

General_ano_input::~General_ano_input()
{
}
