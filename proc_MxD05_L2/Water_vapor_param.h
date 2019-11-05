#pragma once

namespace proc_MxD05_L2
{
	typedef std::filesystem::path path;

	class Water_vapor_param
	{
		friend Water_vapor_param load_water_vapor_param(path const&);
	public:
		Water_vapor_param() = default;
		~Water_vapor_param();

		path hdf_list_file() const { return hdf_list_file_; }
		double min_lon() const { return min_lon_; }
		double max_lon() const { return max_lon_; }
		double min_lat() const { return min_lat_; }
		double max_lat() const { return max_lat_; }
		std::string resampling_type() const { return resampling_type_; }
		std::string output_projection_type() const { return output_projection_type_; }
		std::string output_projection_parameters() const { return output_projection_parameters_; }
		path output_image_file() const { return output_image_file_; }
		path tmp_path() const { return tmp_path_; }
		std::vector<path> hdf_files() const;
		
	private:
		path yml_path_;
		path hdf_list_file_;
		double min_lon_;
		double max_lon_;
		double min_lat_;
		double max_lat_;
		std::string resampling_type_;
		std::string output_projection_type_;
		std::string output_projection_parameters_;
		path output_image_file_;
		path tmp_path_;
		static std::set<std::string> valid_attribute_names_;
	};

	Water_vapor_param load_water_vapor_param(path const& yml_path);
	bool is_valid_water_vapor_yml_file(path const& yml_path);
	bool check_yml_exists(path const& yml_path);
	bool check_hdf_list_file_exists(path const& hdf_list_file);
	bool check_hdf_files_exist(std::vector<path> const& hdf_files);
	bool check_attributes_completeness(std::set<std::string> const& attributes);
}
