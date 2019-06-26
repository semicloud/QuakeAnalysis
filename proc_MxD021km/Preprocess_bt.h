#pragma once
#include "Options_yaml.h"
#include <string>

namespace proc_MxD021km
{
	class Preprocess_bt
	{
	private:
		static bool find_file(const std::string&, const std::string&, std::string&);
		static const  std::string SUFFIX_PREPROCESSED_BT;
		//static const  string SDS_BRIGHTNESS_TEMPERATURE;
		static const  std::string SDS_SOLAR_ZENITH_ANGLE;
		static const  std::string SDS_CLOUD_MASK;
		//static const  string SUFFIX_BT;
		static const  std::string SUFFIX_SZA;
		static const  std::string SUFFIX_CM;

		static const int NO_DATA_VALUE_BT;
		static const int NO_DATA_VALUE_SZA;
		static const float SCALE_SZA;
		static const float THRESHOLD_SZA;
		static const int THRESHOLD_CM;
		static const float TOLERANCE;

		static const std::string MOD_LUT_TABLE_NAME;
		static const std::string MYD_LUT_TABLE_NAME;

		/**
		 * \brief ����HDF�ļ�����ȡ��ʹ�õ�lut��·��
		 * \param path_hdf_file hdf�ļ�·��
		 * \return
		 */
		static std::string get_lut_table(const std::string& path_hdf_file);

		static std::string get_sds_bt(int band);

		static std::string get_suffix_bt(int band);

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



