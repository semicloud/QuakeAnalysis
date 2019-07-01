#pragma once
#include <yaml-cpp/yaml.h>
#include <cassert>
#include <iostream>
#include <filesystem>
#include <optional>
#include <string>

inline std::optional<YAML::Node> load_yml(const std::string& yml_path_str)
{
	using namespace std;
	assert(std::filesystem::exists(yml_path_str));
	try
	{
		YAML::Node node = YAML::LoadFile(yml_path_str);
		return optional<YAML::Node>(node);
	}
	catch (std::exception& e)
	{
		cerr << "parse yml error! message: " << e.what() << endl;
		std::exit(EXIT_FAILURE);
	}
}

int process(const std::string& yml_path_str);
