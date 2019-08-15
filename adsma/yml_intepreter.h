#pragma once
#include "Preprocess_bt_input.h"
#include "Preprocess_aod_wv_input.h"
#include "Preprocess_lst_input.h"
#include <yaml-cpp/yaml.h>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <optional>
#include <string>
#include <filesystem>

std::optional<YAML::Node> load_yml(const std::string& yml_path_str);

/**
 * \brief 从Node中解析出开始日期和结束日期
 * \param node
 * \return
 */
std::vector<boost::gregorian::date> parse_date(const YAML::Node& node);

int process(const std::string& yml_path_str);

int preprocess_bt(const Preprocess_bt_input&);

int preprocess_aod(const Preprocess_aod_wv_input&);

int preprocess_wv(const Preprocess_aod_wv_input&);

int preprocess_lst(const Preprocess_lst_input&);

int eddy_field_bt();

int eddy_field_aod();

int eddy_field_wv();

int eddy_field_lst();