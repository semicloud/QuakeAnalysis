#pragma once
#include "Common_input.h"
#include <string>

class Eddy_field_input : public Common_input
{
public:
	Eddy_field_input() = delete;
	Eddy_field_input(const Eddy_field_input&) = delete;
	~Eddy_field_input() = default;
	Eddy_field_input(const YAML::Node& n, const std::string& product_name, const std::string& product_type);

	std::string product_name() const
	{
		return m_product_name;
	}

	std::string product_type() const
	{
		return m_product_type;
	}

	std::string product_code() const
	{
		if (m_product_name == "BT")
			return "02";
		if (m_product_name == "AOD")
			return "04";
		if (m_product_name == "WV")
			return "05";
		if (m_product_name == "LST")
			return "11";
		return "Wrong product node";
	}

	std::string product_str() const
	{
		return m_product_type + product_code();
	}

	int ano_method() const
	{
		return m_ano_method;
	}

	bool is_calc_ref() const
	{
		return m_is_calc_ref;
	}

	bool is_calc_ano() const
	{
		return m_is_calc_ano;
	}

	bool is_plot_ref() const
	{
		return m_is_plot_ref;
	}

	bool is_plot_ano()const
	{
		return m_is_plot_ano;
	}

private:
	std::string m_product_name;
	std::string m_product_type;
	int m_ano_method = 1;
	bool m_is_calc_ref = true;
	bool m_is_calc_ano = true;
	bool m_is_plot_ref = true;
	bool m_is_plot_ano = true;
};

