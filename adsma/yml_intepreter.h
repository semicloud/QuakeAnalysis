#pragma once
#include <yaml-cpp/yaml.h>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/format.hpp>
#include <optional>
#include <string>
#include <filesystem>

std::optional<YAML::Node> load_yml(const std::string& yml_path_str);

/**
 * \brief ��Node�н�������ʼ���ںͽ�������
 * \param node
 * \return
 */
std::vector<boost::gregorian::date> parse_date(const YAML::Node& node);

int process(const std::string& yml_path_str);

