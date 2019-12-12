#pragma once
#include <yaml-cpp/yaml.h>
#include <string>
#include <filesystem>
#include <optional>

namespace proc_codg
{
	int process_codg(const std::string& yml_path_str, bool is_debug);

	std::optional<YAML::Node> load_variables(const std::string& yml_path);

	bool check_variables(const YAML::Node& node);

}

