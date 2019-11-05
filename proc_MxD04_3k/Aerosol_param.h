#pragma once

namespace proc_MxD04_3k
{
	class Aerosol_param
	{
		friend Aerosol_param load_aerosol_param(std::filesystem::path const&);
	public:
		Aerosol_param() = default;
		~Aerosol_param();

		std::filesystem::path hdf_list_file() const { return hdf_list_file_; }

		double min_lon() const { return min_lon_; }

		double max_lon() const { return max_lon_; }

		double min_lat() const { return min_lat_; }

		double max_lat() const { return max_lat_; }

		std::string resampling_type() const { return resampling_type_; }

		std::string output_projection_type() const { return output_projection_type_; }

		std::string output_projection_parameters() const { return output_projection_parameters_; }

		std::filesystem::path output_image_file() const { return output_image_file_; }

		std::filesystem::path tmp_path() const { return tmp_path_; }

		std::vector<std::filesystem::path> hdf_files() const;

	private:
		std::filesystem::path yml_path_;
		std::filesystem::path hdf_list_file_;
		double min_lon_;
		double max_lon_;
		double min_lat_;
		double max_lat_;
		std::string resampling_type_;
		std::string output_projection_type_;
		std::string output_projection_parameters_;
		std::filesystem::path output_image_file_;
		std::filesystem::path tmp_path_;
		static std::set<std::string> valid_attribute_names_;
	};
	Aerosol_param load_aerosol_param(std::filesystem::path const&);
	bool  is_valid_aerosol_yml_file(std::filesystem::path const& yml_path);
	bool  check_yml_exists(std::filesystem::path const& yml_path);
	bool  check_hdf_list_file_exists(std::filesystem::path const& hdf_list_file);
	bool check_hdf_files_exist(std::vector<std::filesystem::path> const& hdf_files);
	bool check_attributes_completeness(std::set<std::string> const& attributes);
}
