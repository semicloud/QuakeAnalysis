#pragma once
#include <string>
#include <Windows.h>

namespace modis_api
{
	using namespace std;
	typedef const string& cs;
	class __declspec(dllexport) Heg_utils
	{
	private:
		static string load_template_string(const string& file_path);
	public:
		Heg_utils();
		~Heg_utils();
		static void run_heg(cs input_file_name, cs object_name, cs field_name, int band_number,
			double output_pixel_size_x, double output_pixel_size_y, double min_lon, double max_lon, double min_lat, double max_lat,
			cs resampling_type, cs output_projection_type, cs ellipsoid_code, cs output_projection_parameters, cs output_filename, cs output_type, cs temp_dir);
	};
}


