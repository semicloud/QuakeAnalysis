#include "pch.h"
#include "Aerosol_param.h"

std::set<std::string> valid_attribute_names_ = {
	"HDFListFile", /* description */
	"MinLon",
	"MaxLon",
	"MinLat",
	"MaxLat",
	"ResamplingType",
	"OutputProjectionType",
	"OutputProjectionParameters",
	"OutputImageFile",
	"TmpPath"
};

proc_MxD04_3k::Aerosol_param::~Aerosol_param() { }

std::vector<std::filesystem::path> proc_MxD04_3k::Aerosol_param::hdf_files() const
{
	std::vector<std::string> tmp =
		modis_api::File_operation::read_file_all_lines(hdf_list_file_.string());
	std::vector<std::filesystem::path> ans;
	std::transform(tmp.cbegin(), tmp.cend(), std::back_inserter(ans), [](std::string const& s) { return std::filesystem::path(s); });
	return ans;
}

#pragma region checking

bool proc_MxD04_3k::is_valid_aerosol_yml_file(std::filesystem::path const& yml_path)
{
	if (!check_yml_exists(yml_path)) return false;
	const YAML::Node node(YAML::LoadFile(yml_path.string()));
	std::set<std::string> attribute_names;
	for (const auto& kv : node)
	{
		attribute_names.insert(kv.first.as<std::string>());
	}
	//std::transform(node.begin(), node.end(), std::back_inserter(attribute_names),
	//	[](YAML::const_iterator it) {return it->first.as<std::string>(); });

	if (!check_attributes_completeness(attribute_names)) return false;

	const std::filesystem::path hdf_list_file_path(node["HDFListFile"].as<std::string>());
	if (!check_hdf_list_file_exists(hdf_list_file_path)) return false;

	const std::vector<std::string> tmp = modis_api::File_operation::read_file_all_lines(hdf_list_file_path.string());
	std::vector<std::filesystem::path> hdf_files;
	std::transform(tmp.cbegin(), tmp.cend(), std::back_inserter(hdf_files), [](std::string const& s) {return std::filesystem::path(s); });
	if (!check_hdf_files_exist(hdf_files)) return false;

	return true;
}

bool proc_MxD04_3k::check_yml_exists(std::filesystem::path const& yml_path)
{
	if (!std::filesystem::exists(yml_path))
	{
		BOOST_LOG_TRIVIAL(error) << (boost::format("yml file %1% not found!") % yml_path).str();
		return false;
	}
	return true;
}

bool proc_MxD04_3k::check_hdf_list_file_exists(std::filesystem::path const& hdf_list_file)
{
	if (!std::filesystem::exists(hdf_list_file))
	{
		BOOST_LOG_TRIVIAL(error) << (boost::format("hdf list file %1% not found!") % hdf_list_file).str();
		return false;
	}
	return true;
}

bool proc_MxD04_3k::check_hdf_files_exist(std::vector<std::filesystem::path> const& hdf_files)
{
	std::vector<std::filesystem::path> not_exist_hdf_files;
	std::stringstream ss;
	for (auto it = hdf_files.cbegin(); it != hdf_files.cend(); ++it)
	{
		if (!std::filesystem::exists(*it))
		{
			not_exist_hdf_files.push_back(*it);
			ss << *it << ", ";
		}
	}
	if (!not_exist_hdf_files.empty())
	{
		BOOST_LOG_TRIVIAL(error) << not_exist_hdf_files.size() << " hdf file(s) not found: "
			<< ss.str();
		return false;
	}
	return true;
}

bool proc_MxD04_3k::check_attributes_completeness(std::set<std::string> const& attributes)
{
	std::vector<std::string> diff;
	std::set_difference(valid_attribute_names_.begin(), valid_attribute_names_.end(),
		attributes.begin(), attributes.end(),
		std::back_inserter(diff));
	if (!diff.empty())
	{
		std::stringstream ss;
		BOOST_LOG_TRIVIAL(error) << "attribute name [" << boost::algorithm::join(diff, ",") << "] not found!";
		return false;
	}
	return true;
}

#pragma endregion checking

proc_MxD04_3k::Aerosol_param proc_MxD04_3k::load_aerosol_params(std::filesystem::path const& yml_path)
{
	if (!is_valid_aerosol_yml_file(yml_path))
		throw std::runtime_error((boost::format("check %1% failed!") % yml_path).str());
	const YAML::Node node(YAML::LoadFile(yml_path.string()));
	Aerosol_param params;
	params.hdf_list_file_ = node["HDFListFile"].as<std::string>();
	params.min_lon_ = node["MinLon"].as<double>();
	params.max_lon_ = node["MaxLon"].as<double>();
	params.min_lat_ = node["MinLat"].as<double>();
	params.max_lat_ = node["MaxLat"].as<double>();
	params.resampling_type_ = node["ResamplingType"].as<std::string>();
	params.output_projection_type_ = node["OutputProjectionType"].as<std::string>();
	params.output_projection_parameters_ = node["OutputProjectionParameters"].as<std::string>();
	params.output_image_file_ = node["OutputImageFile"].as<std::string>(); //final output
	params.tmp_path_ = node["TmpPath"].as<std::string>();
	return params;
}

