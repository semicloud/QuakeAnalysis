#pragma once

namespace gdal_lib
{
	/**
	 * \brief ����Tif�ļ�
	 * \param path Tif�ļ�·��
	 * \param band_num ������
	 * \param x_size ÿ���������ݵ�����
	 * \param y_size ÿ���������ݵ�����
	 * \param proj ͶӰ�ַ���
	 * \param geo_trans GeoTransform����
	 * \param data_type �������ͣ�Ĭ��ΪGDT_Float32
	 * \return �����ɹ�����0��ʧ�ܷ���-1
	 */
	//int create_tif(std::string const& path, int band_num, int x_size, int y_size, std::string const& proj = std::string{}, double* geo_trans = nullptr, GDALDataType data_type = GDT_Float32);

	/**
	 * \brief ����Tif�ļ�
	 * \param path Tif�ļ�·��
	 * \param band_num ������
	 * \param x_size ÿ���������ݵ�����
	 * \param y_size ÿ���������ݵ�����
	 * \param ref_path �ο�Tif�ļ���·�����Ӹ�Tif�ļ��ж�ȡGeoTransform�Լ�ͶӰ��Ϣ
	 * \param data_type �������ͣ�Ĭ��ΪGDT_Float32
	 * \return �����ɹ�����0��ʧ�ܷ���-1
	 */
	//int create_tif(std::string const& path, int band_num, int x_size, int y_size, std::string const& ref_path, GDALDataType data_type = GDT_Float32);

	__declspec(dllexport) int __cdecl create_tif(std::string const& path, xt::xtensor<float, 3> const& data, int ndv);

}
