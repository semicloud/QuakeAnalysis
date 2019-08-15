#include "pch.h"
#include "Adsma_settings.h"
#include "Common_input.h"
#include "../modis_api/Date_utils.h"
#include <boost/log/trivial.hpp>

Common_input::Common_input(const YAML::Node& node) : m_node(node)
{
	using namespace boost::gregorian;
	using namespace std;
	using namespace std::filesystem;
	using namespace YAML;

	m_workspace = node[adsma::settings::yaml::WORKSPACE].as<string>();
	BOOST_LOG_TRIVIAL(debug) << "Load workspace: " << m_workspace;

	m_tmp_dir = node[adsma::settings::yaml::TMP_PATH].as<string>();
	BOOST_LOG_TRIVIAL(debug) << "Load tmp dir: " << m_tmp_dir;

	const string start_date_str = node[adsma::settings::yaml::START_DATE].as<string>();
	m_start_date = modis_api::Date_utils::get_date_from_doy_str(start_date_str);
	BOOST_LOG_TRIVIAL(debug) << "Load start date: " << m_start_date;

	const string end_date_str = node[adsma::settings::yaml::END_DATE].as<string>();
	m_end_date = modis_api::Date_utils::get_date_from_doy_str(end_date_str);
	BOOST_LOG_TRIVIAL(debug) << "Load end date: " << m_end_date;

	m_shp_boundary = node["ShpBoundary"].as<string>();
	BOOST_LOG_TRIVIAL(debug) << "Boundary shape file: " << m_shp_boundary;

	m_shp_fault = node["ShpFault"].as<string>();
	BOOST_LOG_TRIVIAL(debug) << "Fault shape file: " << m_shp_fault;

	m_shp_city = node["ShpCity"].as<string>();
	BOOST_LOG_TRIVIAL(debug) << "City shape file: " << m_shp_city;

	m_quake_record = node["QuakeRecord"].as<string>();
	BOOST_LOG_TRIVIAL(debug) << "QuakeRecord file: " << m_quake_record;

	m_yml_folder = node["YmlFolderPath"].as<string>();
	BOOST_LOG_TRIVIAL(debug) << "Yml folder path: " << m_yml_folder;

	m_plot_extent = node["PlotExtent"].as<string>();
	BOOST_LOG_TRIVIAL(debug) << "Plot extent: " << m_plot_extent;
}

Common_input::Common_input(const std::filesystem::path& yml_path) : Common_input(load_node(yml_path))
{
}

YAML::Node Common_input::load_node(const std::filesystem::path& yml_path)
{
	YAML::Node node;
	try
	{
		node = YAML::LoadFile(yml_path.string());
	}
	catch (const YAML::BadFile& e)
	{
		std::cerr << e.what() << std::endl;
		BOOST_LOG_TRIVIAL(error) << "yml file parse error, error message: " <<
			e.what();
		exit(EXIT_FAILURE);
	}
	catch (const YAML::ParserException& e)
	{
		std::cerr << e.what() << std::endl;
		BOOST_LOG_TRIVIAL(error) << "yml file parse error, error message: " <<
			e.what();
		exit(EXIT_FAILURE);
	}
	return node;
}
