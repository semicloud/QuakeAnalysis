#pragma once

namespace modis_api
{
	/**
	 * \brief 调用Heg使用的参数
	 */
	class __declspec(dllexport) Heg_param
	{
	public:
		Heg_param();
		explicit Heg_param(std::filesystem::path const& input_file);
		~Heg_param();

#pragma region Setters

		Heg_param& input_file(std::filesystem::path const& input_file)
		{
			input_file_ = input_file;
			return *this;
		}

		Heg_param& object_name(std::string const& object_name)
		{
			object_name_ = object_name;
			return *this;
		}

		Heg_param& field_name(std::string const& field_name)
		{
			field_name_ = field_name;
			return *this;
		}

		Heg_param& band_number(int band_number)
		{
			band_number_ = band_number;
			return *this;
		}

		Heg_param& output_pixel_size_x(double ox)
		{
			output_pixel_size_x_ = ox;
			return *this;
		}

		Heg_param& output_pixel_size_y(double oy)
		{
			output_pixel_size_y_ = oy;
			return *this;
		}

		Heg_param& min_lon(double minlon)
		{
			min_lon_ = minlon;
			return *this;
		}

		Heg_param& max_lon(double maxlon)
		{
			max_lon_ = maxlon;
			return *this;
		}

		Heg_param& min_lat(double minlat)
		{
			min_lat_ = minlat;
			return *this;
		}

		Heg_param& max_lat(double maxlat)
		{
			max_lat_ = maxlat;
			return *this;
		}

		Heg_param& resampling_type(std::string const& rt)
		{
			resampling_type_ = rt;
			return *this;
		}

		Heg_param& output_projection_type(std::string const& opt)
		{
			output_projection_type_ = opt;
			return *this;
		}

		Heg_param& ellipsoid_code(std::string const& ec)
		{
			ellipsoid_code_ = ec;
			return *this;
		}

		Heg_param& output_projection_parameters(std::string const& opp)
		{
			output_projection_parameters_ = opp;
			return *this;
		}

		Heg_param& output_file(std::filesystem::path const& ofp)
		{
			output_file_ = ofp;
			return *this;
		}

		Heg_param& output_type(std::string const& ot)
		{
			output_type_ = ot;
			return *this;
		}

		Heg_param& temp_folder(std::filesystem::path const& tf)
		{
			temp_folder_ = tf;
			return *this;
		}

#pragma endregion Setters

#pragma region Getters

		std::filesystem::path input_file() const
		{
			return input_file_;
		}

		/**
		 * \brief
		 * \return 气溶胶默认值："mod04"，水汽默认值："mod05"
		 */
		std::string object_name() const
		{
			return object_name_;
		}

		/**
		 * \brief
		 * \return 气溶胶默认值："Optical_Depth_Land_And_Ocean|"，水汽默认值："Water_Vapor_Infrared|"
		 */
		std::string field_name() const
		{
			return field_name_;
		}

		/**
		 * \brief
		 * \return  默认值 1
		 */
		int band_number() const
		{
			return band_number_;
		}

		/**
		 * \brief
		 * \return 气溶胶默认值："3000.0"，水汽默认值："5000.0"
		 */
		double output_pixel_size_x() const
		{
			return output_pixel_size_x_;
		}

		/**
 * \brief
 * \return 气溶胶默认值："3000.0"，水汽默认值："5000.0"
 */
		double output_pixel_size_y() const
		{
			return output_pixel_size_y_;
		}

		double min_lon() const
		{
			return min_lon_;
		}

		double max_lon() const
		{
			return max_lon_;
		}

		double min_lat() const
		{
			return min_lat_;
		}

		double max_lat() const
		{
			return max_lat_;
		}

		/**
		 * \brief
		 * \return 默认值"NN"
		 */
		std::string resampling_type() const
		{
			return resampling_type_;
		}

		/**
		 * \brief
		 * \return 默认值"CEA"
		 */
		std::string output_projection_type() const
		{
			return output_projection_type_;
		}

		/**
		 * \brief
		 * \return 默认值"WGS84"
		 */
		std::string ellipsoid_code() const
		{
			return ellipsoid_code_;
		}

		/**
		 * \brief
		 * \return 默认值"( 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0  )"
		 */
		std::string output_projection_parameters() const
		{
			return output_projection_parameters_;
		}

		std::filesystem::path output_file() const
		{
			return output_file_;
		}

		/**
		 * \brief
		 * \return 默认值"GEO"
		 */
		std::string output_type() const
		{
			return output_type_;
		}

		std::filesystem::path temp_folder() const
		{
			return temp_folder_;
		}

#pragma endregion Getters

	private:
		std::filesystem::path input_file_;
		std::string object_name_;
		std::string field_name_;
		int band_number_;
		double output_pixel_size_x_;
		double output_pixel_size_y_;
		double min_lon_;
		double max_lon_;
		double min_lat_;
		double max_lat_;
		std::string resampling_type_;
		std::string output_projection_type_;
		std::string ellipsoid_code_;
		std::string output_projection_parameters_;
		std::filesystem::path output_file_;
		std::string output_type_;
		std::filesystem::path temp_folder_;

		static std::string get_object_name(std::filesystem::path const& input_file);
		static std::string get_field_name(std::filesystem::path const& input_file);
		static double get_pixel_size(std::filesystem::path const& input_file);
	};
}