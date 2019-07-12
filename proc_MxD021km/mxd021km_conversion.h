#pragma once
#include "yamlArgs.h"
#include <string>
#include <armadillo>

namespace adsma
{
	typedef  const std::string& cstr;

	class mxd021km_conversion
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
		 * \brief 根据HDF文件名获取所使用的lut表路径
		 * \param path_hdf_file hdf文件路径
		 * \return
		 */
		static std::string get_lut_table_path(const std::string& path_hdf_file);

		static std::string get_sds_bt(int band);

		static std::string get_suffix_bt(int band);

		/**
		 * \brief 根据波段数，获取系数矩阵中系数的索引
		 * \param band int
		 * \return
		 */
		static int get_param_index_by_band(int band);

		/**
		 * \brief 对亮温tif文件进行系数标定
		 * \param bt_matrix 亮温矩阵
		 * \param bt_hdf_file_path 亮温hdf文件，用于提取标定系数
		 * \param band 要提取的波段
		 * \param tmp_folder tmp目录
		 * \param bt_file_without_extension 亮温hdf文件名（不含扩展名）
		 * \param bt_tif_file_path 亮温tif文件路径
		 */
		static void bt_calibration(arma::fmat& bt_matrix, cstr bt_hdf_file_path, int band,
			const std::filesystem::path& tmp_folder, cstr bt_file_without_extension, cstr bt_tif_file_path);

		/**
		 * \brief 对亮温tif文件进行辐亮度转换
		 * \param bt_matrix 亮温矩阵
		 * \param bt_lut lut表bt列
		 * \param rad_lut lut表band列
		 * \param line_number lut表行数
		 * \param tmp_folder tmp目录
		 * \param bt_file_without_extension 亮温hdf文件名（不含扩展名）
		 * \param bt_tif_file_path 亮温tif文件路径
		 */
		static void bt_transform(arma::fmat& bt_matrix, const arma::fvec& bt_lut, const arma::fvec& rad_lut, arma::uword line_number,
			const std::filesystem::path& tmp_folder, cstr bt_file_without_extension, cstr bt_tif_file_path);

		/**
		 * \brief 对太阳天顶角tif文件进行数据标定
		 * \param sza_matrix 太阳天顶角矩阵
		 * \param tmp_folder tmp目录
		 * \param sza_file_without_extension 太阳天顶角hdf文件名（不含扩展名）
		 * \param sza_tif_file_path 太阳天顶角tif文件路径
		 */
		static void sza_calibration(arma::fmat& sza_matrix, const std::filesystem::path& tmp_folder,
			cstr sza_file_without_extension, cstr sza_tif_file_path);

		/**
		 * \brief 提取太阳天顶角矩阵中大于>85的像元
		 * \param sza_matrix 太阳天顶角矩阵
		 * \param tmp_folder tmp目录
		 * \param sza_file_without_extension 太阳天顶角hdf文件名（不含扩展名）
		 * \param sza_tif_file_path 太阳天顶角tif文件路径
		 */
		static void sza_filter(arma::fmat& sza_matrix, const std::filesystem::path& tmp_folder,
			cstr sza_file_without_extension, cstr sza_tif_file_path);

		/**
		 * \brief 提取云掩膜矩阵中
		 * \param cm_matrix 云掩膜矩阵
		 * \param tmp_folder tmp目录
		 * \param cm_file_without_extension 云掩膜hdf文件名（不含扩展名）
		 * \param cm_tif_file_path 云掩膜tif文件路径
		 */
		static void cm_filter(arma::fmat& cm_matrix, const std::filesystem::path& tmp_folder,
			cstr cm_file_without_extension, cstr cm_tif_file_path);

		/**
		 * \brief 亮温、太阳天顶角、云掩膜矩阵逐元素相乘，然后保存至tif文件
		 * \param bt_matrix 亮温矩阵
		 * \param sza_matrix 太阳天顶角矩阵
		 * \param cm_matrix 云掩膜矩阵
		 * \param tmp_folder tmp目录
		 * \param bt_file_without_extension 亮温hdf文件名（不含扩展名）
		 * \param bt_tif_file_path
		 * \param output_file_path 输出文件路径
		 */
		static void save_bt_sza_cm_product(const arma::fmat& bt_matrix,
			const arma::fmat& sza_matrix, const arma::fmat& cm_matrix,
			const std::filesystem::path& tmp_folder,
			cstr bt_file_without_extension, cstr bt_tif_file_path,
			cstr output_file_path);

		/**
		 * \brief 合成预处理的文件，并保存
		 * \param files 待合成的预处理文件
		 * \param tmp_folder tmp目录
		 * \param final_output_file 最终输出文件路径
		 */
		static void combine_save(const std::vector<std::string>& files,
			const std::filesystem::path& tmp_folder, cstr final_output_file);

	public:
		mxd021km_conversion();
		~mxd021km_conversion();
		/**
		 * \brief 执行预处理
		 */
		static void preprocess(const yamlArgs&);


	};
}



