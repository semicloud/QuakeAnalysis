#pragma once
#include <string>
#include <fstream>
#include <sstream>
namespace  modis_api
{
	using namespace std;
	typedef  const string& cs;
	class __declspec(dllexport) Mrt_utils
	{
		static string load_template_string(const string& file_path);
	public:
		Mrt_utils();
		~Mrt_utils();
		static void run_mrt(cs input_file_name,
			double min_lon, double max_lon, double min_lat, double max_lat,
			cs output_file_name, cs temp_dir, cs spatial_subset_type, cs resampling_type, cs output_projection_type,
			cs output_projection_parameters, cs datum, double output_pixel_size);
	};
}


