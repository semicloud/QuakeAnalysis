#include "Eddy_field_options_yaml.h"
#include <iostream>
#include <boost/log/trivial.hpp>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/yaml.h>
#include <boost/filesystem.hpp>

Eddy_field_options_yaml::Eddy_field_options_yaml(std::string& path)
{
	YAML::Node config;
	try
	{
		config = YAML::LoadFile(path);
		BOOST_LOG_TRIVIAL(info) << "已加载配置文件：" << path;
	}
	catch (const YAML::ParserException &ex)
	{
		BOOST_LOG_TRIVIAL(error) << path << "文件解析错误！";
		BOOST_LOG_TRIVIAL(error) << ex.what();
	}

	_modis_workspace_folder = config["Workspace"].as<std::string>();
	if (_modis_workspace_folder[_modis_workspace_folder.size() - 1] != '\\')
		_modis_workspace_folder = _modis_workspace_folder + "\\";
	_need_compute_ref = config["CalcRef"].as<bool>();
	_need_compute_eddy_field = config["CalcAno"].as<bool>();
	_input_image_file = config["InputImageFile"].as<std::string>();
	_method = config["AnoMethod"].as<int>();
	//_dateStr = config["Date"].as<std::string>();
	//_data = config["Data"].as<std::string>();
	//_type = config["Type"].as<std::string>();
	_support_ref_tif_file = config["RefListFile"].as<std::string>();
	if (_need_compute_ref && !boost::filesystem::exists(_support_ref_tif_file))
	{
		BOOST_LOG_TRIVIAL(error) << "背景场tif列表文件：" << _support_ref_tif_file << "不存在，无法生成背景场！";
		exit(EXIT_FAILURE);
	}
	_ref_image_file = config["RefImageFile"].as<std::string>();
	_output_eddy_field_image_file = config["OutputAnoFile"].as<std::string>();
}

Eddy_field_options_yaml::~Eddy_field_options_yaml() = default;

std::vector<std::string> Eddy_field_options_yaml::get_ref_tif_file_list()
{
	std::vector<std::string> svec;
	std::ifstream ifs(_support_ref_tif_file);
	if (ifs)
	{
		std::string buffer;
		while (std::getline(ifs, buffer)) svec.push_back(buffer);
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "读取" << _support_ref_tif_file << "文件失败！";
	}
	BOOST_LOG_TRIVIAL(debug) << "获取 " << svec.size() << "个用于计算背景场的tif文件:";
	for (auto it = svec.cbegin(); it != svec.cend(); ++it)
		BOOST_LOG_TRIVIAL(debug) << *it;
	return svec;
}

