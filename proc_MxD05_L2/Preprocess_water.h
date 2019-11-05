#pragma once
#include  "Water_vapor_param.h"
#include <string>
#include <yaml-cpp/yaml.h>

namespace proc_MxD05_L2
{
	class Preprocess_water
	{
	public:
		Preprocess_water();
		~Preprocess_water();
		static void preprocess(const std::string& yml_path, const YAML::Node& node, bool debug_mode);
	private:
		static void heg_process(path const& hdf_file, path const& out_file, path const& tmp_folder);
		static void gdal_process(path const& in_file, path const& out_file, double min_lon, double max_lon, double min_lat, double max_lat);
		static void scale_process(path const& in_file, path const& out_file);
		static int scale(arma::fmat&);
		static void combine(std::vector<path> const& in_file_vec, path const& out_file);
	};

	Water_vapor_param load_param(path const& yml_path);
}



