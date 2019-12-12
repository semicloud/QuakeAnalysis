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
		static bool write_fmat_to_tif(const std::string& tif_path, arma::fmat& mat, float no_data_value = 0);

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
		static bool set_no_data_value(const std::filesystem::path& source_path, float no_data_value);


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
		 * \brief ִ��gdal_translate.exe����
		 * \param src Դ���ݼ�����
		 * \param dst Ŀ�����ݼ�����
		 * \param opts ��������
		 * \return ת���ɹ�����true��ʧ�ܷ���false
		 */
		static bool gdal_translate(const std::string& src, const std::string& dst, const std::vector<std::string> & opts);

		/**
		 * \brief ��ȡ0��05���ݼ���geo_bound
		 * \param hdf_path 04��05��hdf�ļ�·��
		 * \param ulx
		 * \param uly
		 * \param lrx
		 * \param lry
		 * \return ��ȡ�ɹ�����true��ʧ�ܷ���false
		 */
		static bool read_geo_bound(std::filesystem::path const& hdf_path, double& ulx, double& uly,
			double& lrx, double& lry);

		/**
		 * \brief ��ȡ����gdal_translate.exe�������в������÷���ֻӦ��������Ĥ��ˮ����Ԥ����ʹ��
		 * \param min_lon ��С����
		 * \param max_lon ��󾭶�
		 * \param min_lat ��Сγ��
		 * \param max_lat ���γ��
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



