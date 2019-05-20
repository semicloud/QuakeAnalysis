#pragma once

#include <string>

using namespace std;

namespace modis_api
{
	/**
	 * \brief SWATH2GRID.EXE Prm文件参数设置
	 */
	class __declspec(dllexport) Mrt_swath_prm_setting
	{
	private:
		string _input_hdf_file;
		string _geo_loc_hdf_file;
		string _sds_str;
		double _ulc_lng;
		double _ulc_lat;
		double _lrc_lng;
		double _lrc_lat;
		string _output_gtiff_file;
	public:
		Mrt_swath_prm_setting();
		~Mrt_swath_prm_setting();

		string get_input_hdf_file() const
		{
			return _input_hdf_file;
		}

		string get_geo_loc_hdf_file() const
		{
			return _geo_loc_hdf_file;
		}

		string get_sds_str() const
		{
			return _sds_str;
		}

		double get_ulc_lng() const
		{
			return _ulc_lng;
		}

		double get_ulc_lat() const
		{
			return _ulc_lat;
		}

		double get_lrc_lng() const
		{
			return _lrc_lng;
		}

		double get_lrc_lat() const
		{
			return _lrc_lat;
		}

		string get_output_gtiff_file() const
		{
			return _output_gtiff_file;
		}


		void set_input_hdf_file(const string& cs)
		{
			_input_hdf_file = cs;
		}

		void set_geo_loc_hdf_file(const string& cs)
		{
			_geo_loc_hdf_file = cs;
		}

		void set_sds_str(const string& cs)
		{
			_sds_str = cs;
		}

		void set_ulc_lng(double ulc_lng)
		{
			_ulc_lng = ulc_lng;
		}

		void set_ulc_lat(double ulc_lat)
		{
			_ulc_lat = ulc_lat;
		}

		void set_lrc_lng(double lrc_lng)
		{
			_lrc_lng = lrc_lng;
		}

		void set_lrc_lat(double lrc_lat)
		{
			_lrc_lat = lrc_lat;
		}

		void set_output_gtiff_file(const string& cs)
		{
			_output_gtiff_file = cs;
		}
	};
}



