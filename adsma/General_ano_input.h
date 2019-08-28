#pragma once
#include "Common_input.h"

/**
 * \brief ƒÍ±‰ ‰»Î
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

private:
	std::string m_product_name;
	std::string m_product_type;
};

