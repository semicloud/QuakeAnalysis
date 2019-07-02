#pragma once
#include <yaml-cpp/yaml.h>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <optional>
#include <string>
#include <filesystem>

std::optional<YAML::Node> load_yml(const std::string& yml_path_str);

int process(const std::string& yml_path_str);

int generate_preprocess_bt_yml(const std::filesystem::path& workspace_path,
	std::filesystem::path&& tmp_path, const std::string& product, 
	const boost::gregorian::date& date_start,
	const boost::gregorian::date& date_end, const YAML::Node& node);
