#pragma once

#include <string>
#include <yaml-cpp/yaml.h>

namespace proc_MxD11A1
{
	/**
	 * \brief 地表温度（Surface Temperature）预处理类
	 */
	class Preprocess_st
	{
		static void check_node(const std::string& yml_path, const YAML::Node& node, const std::string& attr_name);
		static std::string wrap_output_projection_parameters_str(const std::string& old);
	public:
		Preprocess_st();
		~Preprocess_st();
		static void preprocess(const std::string& yml_path, const YAML::Node& node, bool debug_mode);
	};
}


