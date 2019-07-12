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

		static bool create_tif(const std::string& fn, double* geo_trans,
			const std::string& proj, arma::fmat mat);

		static bool get_geo_trans_and_proj(const std::string& fn,
			std::vector<double>& out_geo_trans, std::string& out_proj);

		/**
		 * \brief ��tif�ļ���dnֵ��ȡ���������fmat�У���ȡʧ�ܷ���nullptr
		 * \param tif_path tif�ļ�·��
		 * \return
		 */
		static boost::optional<arma::fmat> read_tif_to_fmat(const std::string& tif_path);
		/**
		 * \brief ������ֵд��tif�ļ���д��ɹ�����true��ʧ�ܷ���false
		 * \param tif_path tif�ļ�·��
		 * \param mat �������
		 */
		static bool write_fmat_to_tif(const std::string& tif_path, arma::fmat& mat);

		/**
		 * \brief ����gdal_translate.exeת��tif�ļ�
		 * \param source_path Դtif�ļ�·��
		 * \param dest_path Ŀ��tif�ļ�·��
		 * \param param ת������ �� -ot Float32
		 * \return ת���ɹ�����true��ʧ�ܷ���false
		 */
		static bool translate_copy(const std::filesystem::path& source_path, const std::filesystem::path& dest_path, const std::string& param = "");


		/**
		 * \brief ����gdal_edit.py����tif�ļ���NO_DATA_VALUE����
		 * \param source_path tif�ļ�·��
		 * \param no_data_value NO_DATA_VALUEֵ
		 * \return �ɹ�����true��ʧ�ܷ���false
		 */
		static bool set_no_data_value(const std::string& source_path, float no_data_value);


		/**
		 * \brief ��ȡĿ���ļ���NO_DATA_VALUE
		 * \param source_path Ŀ��tif�ļ�·��
		 * \return Ŀ��tif�ļ���NO_DATA_VALUE����
		 */
		static float get_no_data_value(const std::string& source_path);

		/**
		 * \brief ��ȡMXD02XXX.HDF�ļ��е�radiance_scales��radiance_offsets
		 * ����ֵ��2*16���󣬸þ���ĵ�һ����radiance_scales��ֵ���ڶ�����radiance_offsets��ֵ
		 * Ŀǰ��ֻ����������ݣ��Ժ������չ����������
		 * \parma path hdf�ļ�·��
		 * \parma logger ���ó��򴫹�������־����
		 * \return
		 */
		static boost::optional<arma::fmat> read_radiance_scales_and_offsets(const std::filesystem::path& path);


		/**
		 * \brief ��ȡHDF�ļ���GeoBound����
		 * \param hdf_path hdf�ļ�·��
		 * \param sds �����ݼ�����
		 * \param ulx upper left x���������
		 * \param uly upper left y���������
		 * \param lrx lower right x���������
		 * \param lry lower right y���������
		 * \return ��ȡ�ɹ�����ture��ʧ�ܷ���false
		 * \remark �ú���ͨ����ȡ�����ݼ�����������ȡ�ռ䷶Χ�����ܣ��Ǵ���ģ���Ӧ��ʹ��
		 * \remark �ú������ú���ֻ��Ϊ�˼�¼�����ֶ�ȡGeoBound�ķ���
		 */
		static bool read_geo_bound(const std::string& hdf_path, const std::string& sds, double& ulx, double& uly, double& lrx, double &lry);

		/**
		 * \brief ��ȡHDF�ļ���GeoBound����
		 * \param hdf_path hdf�ļ�·��
		 * \return ��ȡ�ɹ�����true��ʧ�ܷ���false
		 */
		static bool read_geo_bound_py_h5(const std::string& hdf_path, const std::string& tmp_folder,
			double& ulx, double& uly, double& lrx, double& lry);
	};
}



