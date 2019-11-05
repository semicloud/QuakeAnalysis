#pragma once

#include "Aerosol_param.h"
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <string>

namespace proc_MxD04_3k
{
	typedef const std::string& cs;
	typedef std::filesystem::path path;
	typedef std::vector<std::filesystem::path> paths;

	class Preprocess_aerosol
	{
	public:
		Preprocess_aerosol();
		~Preprocess_aerosol();
		static void preprocess(const std::string& ymlPath, const YAML::Node& node, bool use_debug);
	private:
		static void heg_process(path const& hdf_file, path const& out_file, path const& tmp_folder);
		static void gdal_process(path const& in_file, path const& out_file, double min_lon, double max_lon, double min_lat, double max_lat);
		static void scale_process(path const& in_file, path const& out_file);
		static int scale(arma::fmat&);
		static void combine(std::vector<path> const& in_file_vec, path const& out_file);
	};

	Aerosol_param load_params(std::filesystem::path const& yml_path);
}

