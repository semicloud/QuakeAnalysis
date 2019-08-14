#include "pch.h"
#include "Adsma_settings.h"
#include "Preprocess_input.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>


/**
 * \brief 预处理输入基类
 * \param node Node
 * \param product_name 产品名称，如BT、AOD、WV、LST等
 * \param product_type 产品类型，MOD或MYD
 */
Preprocess_input::Preprocess_input(const YAML::Node& node,
	const std::string& product_name,
	const std::string& product_type)
	:Common_input(node), m_product_name(product_name),
	m_product_type(product_type)
{
	init_lon_lat();
}

void Preprocess_input::init_lon_lat()
{
	using namespace std;
	const string pp_extent_attr = adsma::settings::yaml::PREPROCESS_EXTENT;
	const string lon_lat_str = m_node[pp_extent_attr].as<string>();
	vector<string> extent_strs;
	boost::split(extent_strs, lon_lat_str, boost::is_any_of(" "));
	BOOST_ASSERT(extent_strs.size() == 4);
	m_min_lon = stof(extent_strs[0]);
	m_max_lon = stof(extent_strs[1]);
	m_min_lat = stof(extent_strs[2]);
	m_max_lat = stof(extent_strs[3]);
	BOOST_LOG_TRIVIAL(debug) << (boost::format("Parse lon lat from %1%: %2%, %3%, %4%, %5%")
		% lon_lat_str % m_min_lon % m_max_lon % m_min_lat % m_max_lat).str();
}
