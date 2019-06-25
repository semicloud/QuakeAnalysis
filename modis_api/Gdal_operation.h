#pragma once

#include <string>
#include <boost/optional.hpp>
#include <armadillo>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

namespace modis_api
{
	class __declspec(dllexport) Gdal_operation
	{
	public:
		Gdal_operation();
		~Gdal_operation();

		static bool create_tif(const std::string& fn, double* geo_trans,
			const std::string& proj, arma::fmat mat);

		static bool get_geo_trans_and_proj(const std::string& fn,
			std::vector<double>& out_geo_trans, std::string& out_proj);

		/**
		 * \brief 将tif文件的dn值读取到矩阵对象fmat中，读取失败返回nullptr
		 * \param tif_path tif文件路径
		 * \return
		 */
		static boost::optional<arma::fmat> read_tif_to_fmat(const std::string& tif_path);
		/**
		 * \brief 将矩阵值写入tif文件，写入成功返回true，失败返回false
		 * \param tif_path tif文件路径
		 * \param mat 矩阵对象
		 */
		static bool write_fmat_to_tif(const std::string& tif_path, arma::fmat& mat);

		/**
		 * \brief 调用gdal_translate.exe转换tif文件
		 * \param source_path 源tif文件路径
		 * \param dest_path 目标tif文件路径
		 * \param param 转换参数 如 -ot Float32
		 * \return 转换成功返回true，失败返回false
		 */
		static bool translate_copy(const std::string& source_path, const std::string& dest_path, const std::string& param = "");


		/**
		 * \brief 调用gdal_edit.py设置tif文件的NO_DATA_VALUE属性
		 * \param source_path tif文件路径
		 * \param no_data_value NO_DATA_VALUE值
		 * \return 成功返回true，失败返回false
		 */
		static bool set_no_data_value(const std::string& source_path, float no_data_value);


		/**
		 * \brief 获取目标文件的NO_DATA_VALUE
		 * \param source_path 目标tif文件路径
		 * \return 目标tif文件的NO_DATA_VALUE属性
		 */
		static float get_no_data_value(const std::string& source_path);

		/**
		 * \brief 读取MXD02XXX.HDF文件中的radiance_scales和radiance_offsets
		 * 返回值是2*16矩阵，该矩阵的第一行是radiance_scales的值，第二行是radiance_offsets的值
		 * 目前是只针对亮温数据，以后可以扩展到其他数据
		 * \parma hdf_path hdf文件路径
		 * \parma logger 调用程序传过来的日志对象
		 * \return
		 */
		static boost::optional<arma::fmat> read_radiance_scales_and_offsets(const std::string& hdf_path);

		static bool read_geo_bound(const std::string& hdf_path, const std::string& sds, double& ulx, double& uly, double& lrx, double &lry);
	};
}



