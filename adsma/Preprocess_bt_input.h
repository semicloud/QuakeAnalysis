#pragma once
#include "Preprocess_input.h"

class Preprocess_bt_input : public Preprocess_input
{
public:
	Preprocess_bt_input() = delete;
	~Preprocess_bt_input() = default;
	Preprocess_bt_input(const Preprocess_bt_input&) = delete;
	/**
	 * \brief ����Ԥ��������
	 * \param node node
	 * \param product_type ��Ʒ����
	 */
	Preprocess_bt_input(const YAML::Node & node, const std::string& product_type);

	int band() const
	{
		return m_band;
	}

	std::string mrt_kernel_type() const
	{
		return m_mrt_kernel_type;
	}

	std::string mrt_projection_type() const
	{
		return m_mrt_projection_type;
	}

	std::string mrt_projection_args() const
	{
		return m_mrt_projection_args;
	}

	float mrt_pixel_size() const
	{
		return m_mrt_pixel_size;
	}

private:
	int m_band = 31;
	std::string m_mrt_kernel_type;
	std::string m_mrt_projection_type;
	std::string m_mrt_projection_args;
	float m_mrt_pixel_size;
};

