#pragma once
#include "Options_yaml.h"
#include <string>

using namespace std;

namespace proc_MxD021km
{
	class Preprocess_bt
	{
	private:
		static bool find_file(const string&, const string&, string&);
		static const  string SUFFIX_PREPROCESSED_BT;
		//static const  string SDS_BRIGHTNESS_TEMPERATURE;
		static const  string SDS_SOLAR_ZENITH_ANGLE;
		static const  string SDS_CLOUD_MASK;
		//static const  string SUFFIX_BT;
		static const  string SUFFIX_SZA;
		static const  string SUFFIX_CM;

		static const int NO_DATA_VALUE_BT;
		static const int NO_DATA_VALUE_SZA;
		static const float SCALE_SZA;
		static const float THRESHOLD_SZA;
		static const int THRESHOLD_CM;
		static const float TOLERANCE;

		static const string MOD_LUT_TABLE_NAME;
		static const string MYD_LUT_TABLE_NAME;

		/**
		 * \brief ����HDF�ļ�����ȡ��ʹ�õ�lut��·��
		 * \param path_hdf_file hdf�ļ�·��
		 * \return 
		 */
		static string get_lut_table(const string& path_hdf_file);

		static string get_sds_bt(int band);

		static string get_suffix_bt(int band);

		/**
		 * \brief ���ݲ���������ȡϵ��������ϵ��������
		 * \param band int
		 * \return
		 */
		static int get_param_index_by_band(int band);

	public:
		Preprocess_bt();
		~Preprocess_bt();
		/**
		 * \brief ִ��Ԥ����
		 */
		static void preprocess(const Options_yaml&);

	
	};
}



