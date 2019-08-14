#pragma once
#include "Preprocess_input.h"

class Preprocess_aod_wv_input : public Preprocess_input
{
public:
	Preprocess_aod_wv_input() = delete;
	~Preprocess_aod_wv_input() = default;
	Preprocess_aod_wv_input(const Preprocess_aod_wv_input&) = delete;

	Preprocess_aod_wv_input(const YAML::Node& node,
		const std::string& product_name,
		const std::string& product_type);

	std::string resampling_type() const
	{
		return m_resampling_type;
	}

	std::string output_projection_type() const
	{
		return m_output_projection_type;
	}

	std::string output_projection_parameters() const
	{
		return m_output_projection_parameters;
	}

private:
	std::string m_resampling_type;
	std::string m_output_projection_type;
	std::string m_output_projection_parameters;
};

