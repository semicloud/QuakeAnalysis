#pragma once

#include "Aerosol_param.h"
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <string>

namespace proc_MxD04_3k
{
	typedef const std::string& cs;
	typedef std::vector<std::filesystem::path> paths;

	class Preprocess_aerosol
	{
	public:
		Preprocess_aerosol();
		~Preprocess_aerosol();
		static void preprocess(const std::string& ymlPath, const YAML::Node& node, bool useDebugMode);
		static std::string get_gdal_argument(double minLng, double maxLng, double minLat, double maxLat)
		{
			const std::string argument = (boost::format("-ot Float32 -projwin %1% %2% %3% %4% -projwin_srs EPSG:4326 -of GTiff -co \"COMPRESS = LZW\" -co \"INTERLEAVE = BAND\"") % minLng % maxLat % maxLng % minLat).str();
			return argument;
		}

		static int prepare_tmp_folder(std::filesystem::path const&);
		static int scale(arma::fmat&);
		static std::vector<arma::fmat> readToMat(std::vector<std::filesystem::path> const&);

	};

	Aerosol_param load_params(std::filesystem::path const& yml_path);
}

