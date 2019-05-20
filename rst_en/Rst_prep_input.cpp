#include "pch.h"
#include "Rst_prep_input.h"
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/algorithm/string.hpp>
#include <yaml-cpp/yaml.h>
#include "../modis_api/Date_utils.h"
#include "../modis_api/File_operation.h"
#include <boost/filesystem/operations.hpp>
#include "Rst_prep_file_op.h"

namespace fs = boost::filesystem;
namespace gr = boost::gregorian;
typedef modis_api::Date_utils date_utils;

std::vector<Rst_prep_input>
Rst_prep_input::load(unsigned current_year,
	const std::vector<std::string>& all_tif_files,
	const std::vector<std::string>& generated_doys,
	const std::string& output_dir, unsigned k)
{
	// Rst算法预处理对象
	std::vector<Rst_prep_input> ret;
	for (const std::string& doy : generated_doys)
	{
		gr::date doy_date = date_utils::get_date_from_doy_str_underline(doy);
		std::string doy_file;
		if (!Rst_prep_file_op::exist_file_according_to_doy(all_tif_files, doy, doy_file))
		{
			BOOST_LOG_TRIVIAL(error) << "file related to " << doy
				<< " not found, skip fill background value";
			continue;
		}
		std::vector<std::string> adj_file_paths =
			Rst_prep_file_op::get_adjacent_file_paths(all_tif_files, doy_date, k);

		std::vector<std::string> hist_file_paths =
			Rst_prep_file_op::get_hist_file_paths(all_tif_files,
				doy_date, current_year);

		std::vector<std::string> adj_file_names =
			modis_api::File_operation::get_file_names(adj_file_paths);

		std::vector<std::string> hist_file_names =
			modis_api::File_operation::get_file_names(hist_file_paths);

		BOOST_LOG_TRIVIAL(debug) << fs::path(doy_file).filename().string()
			<< "的近邻文件：" << boost::algorithm::join(adj_file_names, ",");
		BOOST_LOG_TRIVIAL(debug) << fs::path(doy_file).filename().string()
			<< "的历史文件：" << boost::algorithm::join(hist_file_names, ",");

		const std::string output_file = output_dir
			+ fs::path(doy_file).filename().string();
		ret.emplace_back(doy_file, adj_file_paths, hist_file_paths, output_file);
	}

	return ret;
}
