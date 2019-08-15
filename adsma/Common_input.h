#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <yaml-cpp/yaml.h>

/**
 * �Ѹ����㷨���õ����룬�繤���ռ�Ŀ¼��
 * TmpĿ¼��Shp�ļ�·���ȷŵ�������
 * ���ٺ��������Ĵ�������
 */

class Common_input
{
public:
	Common_input() = delete;
	Common_input(const Common_input&) = delete;
	explicit Common_input(const YAML::Node&);
	Common_input(const std::filesystem::path&);
	virtual ~Common_input() = default;

	std::filesystem::path workspace() const
	{
		return m_workspace;
	}

	std::filesystem::path tmp_dir() const
	{
		return m_tmp_dir;
	}

	std::filesystem::path yml_folder() const
	{
		return m_yml_folder;
	}

	boost::gregorian::date start_date() const
	{
		return m_start_date;
	}

	boost::gregorian::date end_date() const
	{
		return m_end_date;
	}

	std::filesystem::path shp_boundary() const
	{
		return m_shp_boundary;
	}

	std::filesystem::path shp_fault() const
	{
		return m_shp_fault;
	}

	std::filesystem::path shp_city() const
	{
		return m_shp_city;
	};

	std::filesystem::path quake_record() const
	{
		return m_quake_record;
	}

	std::string plot_extent() const
	{
		return m_plot_extent;
	}

protected:
	YAML::Node m_node;

private:
	std::filesystem::path m_workspace;
	std::filesystem::path m_tmp_dir;
	std::filesystem::path m_yml_folder;
	boost::gregorian::date m_start_date;
	boost::gregorian::date m_end_date;

	std::filesystem::path m_shp_boundary;
	std::filesystem::path m_shp_fault;
	std::filesystem::path m_shp_city;
	std::filesystem::path m_quake_record;
	std::string m_plot_extent;
	YAML::Node load_node(const std::filesystem::path&);
};
