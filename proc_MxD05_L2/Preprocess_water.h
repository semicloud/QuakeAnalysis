#pragma once
#include <string>
#include <yaml-cpp/yaml.h>

namespace proc_MxD05_L2
{
	class Preprocess_water
	{
	public:
		Preprocess_water();
		~Preprocess_water();
		static void check_node(const std::string& yml_path, const YAML::Node& node, const std::string& attr_name);
		static void preprocess(const std::string& yml_path, const YAML::Node& node, bool debug_mode);
	};
}



