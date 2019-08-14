#pragma once
#include "pch.h"
#include "Common_input.h"
#include <yaml-cpp/yaml.h>
#include <string>

class Preprocess_input : public Common_input
{
public:
	Preprocess_input() = delete;
	Preprocess_input(const YAML::Node&, const std::string&, const std::string&);
	~Preprocess_input() = default;
	Preprocess_input(const Preprocess_input&) = delete;

	std::string product_name() const
	{
		return m_product_name;
	}

	std::string prodcut_type() const
	{
		return m_product_type;
	}

	float min_lon() const
	{
		return m_min_lon;
	}

	float max_lon() const
	{
		return m_max_lon;
	}

	float min_lat() const
	{
		return m_min_lat;
	}

	float max_lat() const
	{
		return m_max_lat;
	}

private:
	std::string m_product_name;
	std::string m_product_type;
	float m_min_lon = 0;
	float m_max_lon = 0;
	float m_min_lat = 0;
	float m_max_lat = 0;
	// 解析并初始化经纬度
	void init_lon_lat();
};

