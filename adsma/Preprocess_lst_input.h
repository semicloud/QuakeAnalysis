#pragma once
#include "Preprocess_input.h"

class Preprocess_lst_input : public Preprocess_input
{
public:
	Preprocess_lst_input() = delete;
	~Preprocess_lst_input() = default;
	Preprocess_lst_input(const Preprocess_lst_input&) = delete;
	Preprocess_lst_input(const YAML::Node& node, const std::string& product_type);

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

	float output_pixel_size() const
	{
		return m_output_pixel_size;
	}

private:
	std::string m_resampling_type;
	std::string m_output_projection_type;
	std::string m_output_projection_parameters;
	float m_output_pixel_size = 1000;
};

