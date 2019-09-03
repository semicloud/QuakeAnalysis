#pragma once
#include "Common_input.h"

/**
 * \brief �������
 */
class General_ano_input : public Common_input
{
public:
	General_ano_input() = delete;
	General_ano_input(const YAML::Node& n, const std::string& product_name,
		const std::string& product_type);
	~General_ano_input();

	std::string product_name() const
	{
		return m_product_name;
	}

	std::string product_type() const
	{
		return m_product_type;
	}

	bool is_plot_ref() const
	{
		return m_is_plot_ref;
	}

	bool is_plot_ano() const
	{
		return m_is_plot_ano;
	}

private:
	std::string m_product_name;
	std::string m_product_type;
	bool m_is_plot_ref;
	bool m_is_plot_ano;
};

