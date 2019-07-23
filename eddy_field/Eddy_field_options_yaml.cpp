#include "Eddy_field_options_yaml.h"
#include <boost/log/trivial.hpp>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <fstream>


Eddy_field_options_yaml::Eddy_field_options_yaml(std::string& path)
{
	using namespace std;
	using namespace filesystem;

	YAML::Node config;
	try
	{
		config = YAML::LoadFile(path);
		BOOST_LOG_TRIVIAL(debug) << "已加载配置文件：" << path;
	}
	catch (const YAML::ParserException &ex)
	{
		BOOST_LOG_TRIVIAL(error) << path << "文件解析错误！";
		BOOST_LOG_TRIVIAL(error) << ex.what();
	}

	m_workspace = config["Workspace"].as<std::string>();
	m_calc_ref = config["CalcRef"].as<bool>();
	m_calc_ano = config["CalcAno"].as<bool>();
	m_input_image_file = config["InputImageFile"].as<std::string>();
	m_ano_method = config["AnoMethod"].as<int>();
	m_ref_list_file = config["RefListFile"].as<std::string>();
	if (m_calc_ref && !exists(m_ref_list_file))
	{
		BOOST_LOG_TRIVIAL(error) << "背景场tif列表文件：" << m_ref_list_file << "不存在，无法生成背景场！";
		exit(EXIT_FAILURE);
	}
	m_ref_image_file = config["RefImageFile"].as<std::string>();
	m_output_ano_file = config["OutputAnoFile"].as<std::string>();
}

Eddy_field_options_yaml::~Eddy_field_options_yaml() = default;

std::vector<std::filesystem::path> Eddy_field_options_yaml::get_tif_files_for_ref_computing()
{
	using namespace std;
	std::vector<std::filesystem::path> paths;
	std::ifstream ifs(m_ref_list_file);
	if (!ifs)
	{
		BOOST_LOG_TRIVIAL(error) << "读取" << m_ref_list_file << "文件失败！";
	}
	std::string buffer;
	while (std::getline(ifs, buffer)) paths.emplace_back(buffer);
	BOOST_LOG_TRIVIAL(debug) << "获取 " << paths.size() << "个用于计算背景场的tif文件:";
	//for (auto it = paths.cbegin(); it != paths.cend(); ++it)
	//	BOOST_LOG_TRIVIAL(debug) << *it;
	return paths;
}

