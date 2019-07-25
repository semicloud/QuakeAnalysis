#include "pch.h"
#include "Adsma_settings.h"
#include "Interpreter_helper.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <yaml-cpp/yaml.h>


int adsma::interpreter::helper::split_lonlat_str(const std::string& lon_lat_str, float& out_min_lon, float& out_max_lon, float& out_min_lat,
	float& out_max_lat)
{
	using namespace std;
	vector<string> extent_strs;
	split(extent_strs, lon_lat_str, boost::is_any_of(" "));
	BOOST_ASSERT(extent_strs.size() == 4);
	out_min_lon = stof(extent_strs[0]);
	out_max_lon = stof(extent_strs[1]);
	out_min_lat = stof(extent_strs[2]);
	out_max_lat = stof(extent_strs[3]);
	BOOST_LOG_TRIVIAL(debug) << str(boost::format("Parse lon lat from %1%: %2%, %3%, %4%, %5%")
		% lon_lat_str % out_min_lon % out_max_lon % out_min_lat % out_max_lat);
	return EXIT_SUCCESS;
}

std::string adsma::interpreter::helper::get_yml_str(const std::unordered_map<std::string, std::string>& umap)
{
	using namespace std;
	using namespace YAML;
	BOOST_ASSERT_MSG(!umap.empty(), "empty map!");
	Emitter emt;
	emt << BeginMap;
	for (unordered_map<string, string>::const_iterator it = umap.cbegin();
		it != umap.cend(); ++it)
	{
		emt << Key << it->first << Value << it->second;
	}
	emt << EndMap;
	return emt.c_str();
}

bool adsma::interpreter::helper::is_data_exist(const std::filesystem::path& workspace, const std::string& product, const std::string& year_and_day)
{
	using namespace std;
	using namespace std::filesystem;
	using namespace boost;
	const string year = year_and_day.substr(0, 4);
	const string day = year_and_day.substr(4, 3);
	const path data_folder = workspace / product / year / day;

	if (!exists(data_folder))
	{
		BOOST_LOG_TRIVIAL(error) << "不存在的目录：" << data_folder << "，无法进行预处理";
		return false;
	}
	if (filesystem::is_empty(data_folder))
	{
		BOOST_LOG_TRIVIAL(error) << "目录为空：" << data_folder << "，无法进行预处理";
		return false;
	}
	return true;
}

std::vector<unsigned int> adsma::interpreter::helper::get_years_of_product(
	const std::filesystem::path& workspace_path,
	const std::string& product_type,
	const std::string& product)
{
	using namespace std;
	using namespace filesystem;

	vector<unsigned int> years;
	const path pp_data_path = workspace_path / settings::PP_STANDARD_FOLDER /
		preprocess::get_pp_folder(product_type, product);
	for (directory_iterator it(pp_data_path); it != directory_iterator(); ++it)
	{
		const string str = it->path().string().substr(it->path().string().size() - 4, 4);
		const unsigned int year = boost::lexical_cast<unsigned int>(str);
		years.push_back(year);
	}
	sort(years.begin(), years.end());
	BOOST_LOG_TRIVIAL(debug) << "Years of " << product_type << product << ": "
		<< join(years | boost::adaptors::transformed([](const unsigned& y) {return to_string(y); }), " ");
	return years;
}