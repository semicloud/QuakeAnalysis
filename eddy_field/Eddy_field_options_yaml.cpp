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
		BOOST_LOG_TRIVIAL(info) << "�Ѽ��������ļ���" << path;
	}
	catch (const YAML::ParserException &ex)
	{
		BOOST_LOG_TRIVIAL(error) << path << "�ļ���������";
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
		BOOST_LOG_TRIVIAL(error) << "������tif�б��ļ���" << _support_ref_tif_file << "�����ڣ��޷����ɱ�������";
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
		BOOST_LOG_TRIVIAL(error) << "��ȡ" << _support_ref_tif_file << "�ļ�ʧ�ܣ�";
	}
	BOOST_LOG_TRIVIAL(debug) << "��ȡ " << svec.size() << "�����ڼ��㱳������tif�ļ�:";
	for (auto it = svec.cbegin(); it != svec.cend(); ++it)
		BOOST_LOG_TRIVIAL(debug) << *it;
	return svec;
}

