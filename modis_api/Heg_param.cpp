#include "stdafx.h"
#include "Heg_param.h"


modis_api::Heg_param::Heg_param()
{
}

modis_api::Heg_param::Heg_param(std::filesystem::path const& input_file)
	: input_file_(input_file),
	object_name_(get_object_name(input_file)),
	field_name_(get_field_name(input_file)),
	band_number_(1),
	output_pixel_size_x_(get_pixel_size(input_file)),
	output_pixel_size_y_(get_pixel_size(input_file)),
	min_lon_(0.0), max_lon_(0.0), min_lat_(0.0), max_lat_(0.0),
	resampling_type_("NN"), output_projection_type_("CEA"), ellipsoid_code_("WGS84"),
	output_projection_parameters_("( 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0  )"),
	output_file_(), output_type_("GEO"), temp_folder_()
{
}

modis_api::Heg_param::~Heg_param() { }

std::string modis_api::Heg_param::get_object_name(std::filesystem::path const& input_file)
{
	const std::string type = input_file.filename().string().substr(3, 2);
	if (type == "04")
		return "mod04";
	else if (type == "05")
		return "mod05";
	throw std::runtime_error("error input file!");
}

std::string modis_api::Heg_param::get_field_name(std::filesystem::path const& input_file)
{
	const std::string type = input_file.filename().string().substr(3, 2);
	if (type == "04")
		return "Optical_Depth_Land_And_Ocean|";
	else if (type == "05")
		return "Water_Vapor_Infrared|";
	throw std::runtime_error("error input file!");
}

double modis_api::Heg_param::get_pixel_size(std::filesystem::path const& input_file)
{
	const std::string type = input_file.filename().string().substr(3, 2);
	if (type == "04")
		return 3000.0;
	else if (type == "05")
		return 5000.0;
	throw std::runtime_error("error input file!");
}
