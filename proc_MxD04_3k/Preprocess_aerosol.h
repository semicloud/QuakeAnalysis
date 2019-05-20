#pragma once

#include <yaml-cpp/yaml.h>
#include <string>

namespace proc_MxD04_3k
{
	typedef const std::string& cs;

	class Preprocess_aerosol
	{
		static void check_node(const std::string& yml_path, const YAML::Node& node, const std::string& attr_name);
		static std::string load_template_string(const std::string& file_path);
	public:
		Preprocess_aerosol();
		~Preprocess_aerosol();
		static void preprocess(const std::string& yml_path, const YAML::Node& node);
	};
}

