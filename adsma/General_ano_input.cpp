#include "pch.h"
#include "General_ano_input.h"


General_ano_input::General_ano_input(const YAML::Node& n, const std::string& product_name,
	const std::string& product_type)
	: Common_input(n), m_product_name(product_name), m_product_type(product_type)
{

}

General_ano_input::~General_ano_input()
{
}
