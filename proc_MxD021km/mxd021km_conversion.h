#pragma once
#include "yamlArgs.h"
#include <string>
#include <armadillo>

namespace adsma
{
	class mxd021km_conversion
	{
	private:
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

		/**
		* \brief ��ָ�����ļ����¸���pattern�ҵ���Ӧ���ļ�·��
		* \param folder_path �ļ���·��
		* \param pattern
		* \param file_path �ҵ����ļ�·����д�뵽�ñ�����
		* \return �ҵ��ļ��򷵻�true�����򷵻�false
		*/
		static bool find_file(const std::filesystem::path& folder_path, const std::string& pattern, std::filesystem::path& file_path);

		/**
		 * \brief ������ȡ�Ĳ���ֵ��ȡ����SDS
		 * \param band ����ȡ�Ĳ���ֵ
		 * \return
		 */
		static std::string get_sds_bt(int band);

		/**
		 * \brief ���ݴ���ȡ�Ĳ���ֵ��������tif�ļ��ĺ�׺�����ҵ����ļ�
		 * \param band ����ֵ
		 * \return
		 */
		static std::string get_suffix_bt(int band);

		/**
		 * \brief ���ݲ���������ȡϵ��������ϵ��������
		 * \param band int
		 * \return �ò�����ϵ�������е�����ֵ
		 */
		static int get_param_index_by_band(int band);

		/**
		 * \brief ������tif�ļ�����ϵ���궨
		 * \param bt_matrix ���¾���
		 * \param bt_hdf_file_path ����hdf�ļ���������ȡ�궨ϵ��
		 * \param band Ҫ��ȡ�Ĳ���
		 * \param tmp_path tmpĿ¼
		 * \param bt_file_without_extension ����hdf�ļ�����������չ����
		 * \param bt_tif_file_path ����tif�ļ�·��
		 */
		static void bt_calibration(arma::fmat& bt_matrix,
			const std::filesystem::path& bt_hdf_file_path, int band,
			const std::filesystem::path& tmp_path,
			const std::string& bt_file_without_extension,
			const std::filesystem::path& bt_tif_file_path);

		/**
		 * \brief ������tif�ļ����з�����ת��
		 * \param bt_matrix ���¾���
		 * \param bt_lut lut��bt��
		 * \param rad_lut lut��band��
		 * \param line_number lut������
		 * \param tmp_path tmpĿ¼
		 * \param bt_file_without_extension ����hdf�ļ�����������չ����
		 * \param bt_tif_file_path ����tif�ļ�·��
		 */
		static void bt_transform(arma::fmat& bt_matrix,
			const arma::fvec& bt_lut, const arma::fvec& rad_lut, arma::uword line_number,
			const std::filesystem::path& tmp_path,
			const std::string& bt_file_without_extension,
			const std::filesystem::path& bt_tif_file_path);

		/**
		 * \brief ��̫���춥��tif�ļ��������ݱ궨
		 * \param sza_matrix ̫���춥�Ǿ���
		 * \param tmp_path tmpĿ¼
		 * \param sza_file_without_extension ̫���춥��hdf�ļ�����������չ����
		 * \param sza_tif_file_path ̫���춥��tif�ļ�·��
		 */
		static void sza_calibration(arma::fmat& sza_matrix,
			const std::filesystem::path& tmp_path,
			const std::string& sza_file_without_extension,
			const std::filesystem::path& sza_tif_file_path);

		/**
		 * \brief ��ȡ̫���춥�Ǿ����д���>85����Ԫ
		 * \param sza_matrix ̫���춥�Ǿ���
		 * \param tmp_path tmpĿ¼
		 * \param sza_file_without_extension ̫���춥��hdf�ļ�����������չ����
		 * \param sza_tif_file_path ̫���춥��tif�ļ�·��
		 */
		static void sza_filter(arma::fmat& sza_matrix,
			const std::filesystem::path& tmp_path,
			const std::string& sza_file_without_extension,
			const std::filesystem::path& sza_tif_file_path);

		/**
		 * \brief ��ȡ����Ĥ������
		 * \param cm_matrix ����Ĥ����
		 * \param tmp_path tmpĿ¼
		 * \param cm_file_without_extension ����Ĥhdf�ļ�����������չ����
		 * \param cm_tif_file_path ����Ĥtif�ļ�·��
		 */
		static void cm_filter(arma::fmat& cm_matrix,
			const std::filesystem::path& tmp_path,
			const std::string& cm_file_without_extension,
			const std::filesystem::path& cm_tif_file_path);

		/**
		 * \brief ���¡�̫���춥�ǡ�����Ĥ������Ԫ����ˣ�Ȼ�󱣴���tif�ļ�
		 * \param bt_matrix ���¾���
		 * \param sza_matrix ̫���춥�Ǿ���
		 * \param cm_matrix ����Ĥ����
		 * \param tmp_path tmpĿ¼
		 * \param bt_file_without_extension ����hdf�ļ�����������չ����
		 * \param bt_tif_file_path
		 * \param output_file_path ����ļ�·��
		 */
		static void bt_sza_cm_product_save(
			const arma::fmat& bt_matrix,
			const arma::fmat& sza_matrix,
			const arma::fmat& cm_matrix,
			const std::filesystem::path& tmp_path,
			const std::string& bt_file_without_extension,
			const std::filesystem::path& bt_tif_file_path,
			const std::filesystem::path& output_file_path);

		/**
		 * \brief �ϳ�Ԥ������ļ���������
		 * \param files ���ϳɵ�Ԥ�����ļ�
		 * \param tmp_path tmpĿ¼
		 * \param final_output_file ��������ļ�·��
		 */
		static void combine_save(
			const std::vector<std::filesystem::path>& files,
			const std::filesystem::path& tmp_path,
			const std::filesystem::path& final_output_file);

	public:
		mxd021km_conversion();
		~mxd021km_conversion();

		/**
		 * \brief ִ��Ԥ����
		 */
		static void preprocess(const yamlArgs&);
	};
}



