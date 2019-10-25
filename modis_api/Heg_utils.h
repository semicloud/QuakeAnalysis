#pragma once
#include <string>

namespace modis_api
{
	typedef const std::string& cs;
	class __declspec(dllexport) Heg_utils
	{
	private:
		/**
		 * \brief ��ȡheg��.prm�ļ�ģ�壬���ٴ�templatesĿ¼�µ�ģ���ļ���ȡ
		 * \return hegģ���ļ�
		 */
		static std::string get_heg_prm_template();

		/**
		 * \brief ��ȡ����heg��bat�ű�ģ�壬���ٴ�templateĿ¼�µ�ģ���ļ���ȡ
		 * \return heg batģ���ļ�
		 */
		static std::string get_heg_bat_template();
	public:
		Heg_utils();
		~Heg_utils();
		static void run_heg(cs input_file_name, cs object_name, cs field_name, int band_number,
			double output_pixel_size_x, double output_pixel_size_y, double min_lon, double max_lon, double min_lat, double max_lat,
			cs resampling_type, cs output_projection_type, cs ellipsoid_code, cs output_projection_parameters, cs output_filename, cs output_type, cs temp_dir);
	};
}


