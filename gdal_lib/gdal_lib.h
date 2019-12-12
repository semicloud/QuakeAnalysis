#pragma once

namespace gdal_lib
{
	/**
	 * \brief 创建Tif文件
	 * \param path Tif文件路径
	 * \param band_num 波段数
	 * \param x_size 每个波段数据的列数
	 * \param y_size 每个波段数据的行数
	 * \param proj 投影字符串
	 * \param geo_trans GeoTransform数组
	 * \param data_type 数据类型，默认为GDT_Float32
	 * \return 创建成功返回0，失败返回-1
	 */
	//int create_tif(std::string const& path, int band_num, int x_size, int y_size, std::string const& proj = std::string{}, double* geo_trans = nullptr, GDALDataType data_type = GDT_Float32);

	/**
	 * \brief 创建Tif文件
	 * \param path Tif文件路径
	 * \param band_num 波段数
	 * \param x_size 每个波段数据的列数
	 * \param y_size 每个波段数据的行数
	 * \param ref_path 参考Tif文件的路径，从该Tif文件中读取GeoTransform以及投影信息
	 * \param data_type 数据类型，默认为GDT_Float32
	 * \return 创建成功返回0，失败返回-1
	 */
	//int create_tif(std::string const& path, int band_num, int x_size, int y_size, std::string const& ref_path, GDALDataType data_type = GDT_Float32);

	__declspec(dllexport) int __cdecl create_tif(std::string const& path, xt::xtensor<float, 3> const& data, int ndv);

}
