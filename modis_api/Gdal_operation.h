#pragma once

#include <armadillo>
#include <boost/format.hpp>
#include <string>

namespace modis_api
{
	class __declspec(dllexport) Gdal_operation
	{
	public:
		Gdal_operation();
		~Gdal_operation();

		static std::string proj4_to_wkt(std::string const& proj4str);
		
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
		static bool write_fmat_to_tif(const std::string& tif_path, arma::fmat& mat, float no_data_value = 0);

		/**
		 * \brief 调用gdal_translate.exe转换tif文件
		 * \param source_path 源tif文件路径
		 * \param dest_path 目标tif文件路径
		 * \param param 转换参数 如 -ot Float32
		 * \return 转换成功返回true，失败返回false
		 */
		static bool translate_copy(const std::filesystem::path& source_path, const std::filesystem::path& dest_path, const std::string& param = "");


		/**
		 * \brief 调用gdal_edit.py设置tif文件的NO_DATA_VALUE属性
		 * \param source_path tif文件路径
		 * \param no_data_value NO_DATA_VALUE值
		 * \return 成功返回true，失败返回false
		 */
		static bool set_no_data_value(const std::filesystem::path& source_path, float no_data_value);


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
		 * \parma path hdf文件路径
		 * \parma logger 调用程序传过来的日志对象
		 * \return
		 */
		static boost::optional<arma::fmat> read_radiance_scales_and_offsets(const std::filesystem::path& path);

		/**
		 * \brief 执行gdal_translate.exe命令
		 * \param src 源数据集名称
		 * \param dst 目标数据集名称
		 * \param opts 参数集合
		 * \return 转换成功返回true，失败返回false
		 */
		static bool gdal_translate(const std::string& src, const std::string& dst, const std::vector<std::string> & opts);

		/**
		 * \brief 读取0或05数据集的geo_bound
		 * \param hdf_path 04或05的hdf文件路径
		 * \param ulx
		 * \param uly
		 * \param lrx
		 * \param lry
		 * \return 读取成功返回true，失败返回false
		 */
		static bool read_geo_bound(std::filesystem::path const& hdf_path, double& ulx, double& uly,
			double& lrx, double& lry);

		/**
		 * \brief 获取用于gdal_translate.exe的命令行参数，该方法只应用于云掩膜和水汽的预处理使用
		 * \param min_lon 最小经度
		 * \param max_lon 最大经度
		 * \param min_lat 最小纬度
		 * \param max_lat 最大纬度
		 * \return arg
		 */
		static std::string get_reproj_arg(double min_lon, double max_lon, double min_lat, double max_lat)
		{
			return (boost::format("-ot Float32 -projwin %1% %2% %3% %4% -projwin_srs EPSG:4326 -of GTiff -co \"COMPRESS = LZW\" -co \"INTERLEAVE = BAND\"") % min_lon % max_lat % max_lon % min_lat).str();
		}

	private:
		static bool read_min_max_loc(std::string const&, double&, double&);

	};
}



