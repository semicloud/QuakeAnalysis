#include "pch.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Heg_utils.h"
#include "../modis_api/Mat_operation.h"
#include "Aerosol_param.h"
#include "Preprocess_aerosol.h"
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <filesystem>
#include <string>
#include <vector>

proc_MxD04_3k::Preprocess_aerosol::Preprocess_aerosol() = default;
proc_MxD04_3k::Preprocess_aerosol::~Preprocess_aerosol() = default;

const std::string OBJECT_NAME = "mod04";
const std::string ELLIPSOID_CODE = "WGS84";
const std::string OUTPUT_TYPE = "GEO";
const std::string FIELD_NAME = "Optical_Depth_Land_And_Ocean|";
const int BAND_NUMBER = 1;
const double OUTPUT_PIXEL_SIZE_X = 3000;
const double OUTPUT_PIXEL_SIZE_Y = 3000;
const float SCALE = 0.001f;
const float OFFSET = 0.0f;
//const int MAX_SLEEP = 5;
const float NO_DATA_VALUE = -1; //����GDAL�ü���NO_DATA_VALUE�Ƿ���-9999

/**
 * \brief Ԥ�������ܽ�����
 * \param ymlPath .yml�ļ�·��
 * \param node YAML::Node
 */
void proc_MxD04_3k::Preprocess_aerosol::preprocess(const std::string& ymlPath, const YAML::Node& node, bool use_debug)
{
	BOOST_LOG_TRIVIAL(info) << "\n��ʼ����AerosolԤ����ʹ�õ�.yml�ļ�Ϊ��" << ymlPath;

	Aerosol_param param = load_params(ymlPath);
	path tmp_folder(param.tmp_path());
	modis_api::File_operation::clear_directory(tmp_folder.string());

	auto tmp_name = [=](path const& p1, std::string const& appender) -> path
	{ return modis_api::File_operation::get_tmp_file(p1, tmp_folder, appender); };

	std::vector<path> processed_file_vec;
	for (path const& hdf_file : param.hdf_files())
	{
		const path heg_tif_file = tmp_name(hdf_file, "heg"); //Heg��ȡ����.tif�ļ�·��
		const path gdal_tif_file = tmp_name(hdf_file, "heg_gdal");  //gdal�����.tif�ļ�·��
		const path scaled_tif_file = tmp_name(hdf_file, "heg_gdal_scaled");  //scale���.tif�ļ�·��

		heg_process(hdf_file, heg_tif_file, tmp_folder);
		assert(std::filesystem::exists(heg_tif_file));

		gdal_process(heg_tif_file, gdal_tif_file, param.min_lon(), param.max_lon(), param.min_lat(), param.max_lat());
		assert(std::filesystem::exists(gdal_tif_file));

		scale_process(gdal_tif_file, scaled_tif_file);
		assert(std::filesystem::exists(scaled_tif_file));

		processed_file_vec.push_back(scaled_tif_file);
		BOOST_LOG_TRIVIAL(info) << hdf_file << "�ļ��������";
	}

	combine(processed_file_vec, param.output_image_file());
	BOOST_LOG_TRIVIAL(info) << "Ԥ������ɣ����ս���ļ�Ϊ��" << param.output_image_file() << "\n";

	if (use_debug) return;
	modis_api::File_operation::clear_directory(tmp_folder.string());
	BOOST_LOG_TRIVIAL(info) << "TempĿ¼" << tmp_folder << "�����..";
}

void proc_MxD04_3k::Preprocess_aerosol::heg_process(path const& hdf_file, path const& out_file,
	path const& tmp_folder)
{
	double ulx = 0, uly = 0, lrx = 0, lry = 0;
	modis_api::Gdal_operation::read_geo_bound(hdf_file, ulx, uly, lrx, lry);
	modis_api::Heg_utils::run_heg(modis_api::Heg_param(hdf_file)
		.min_lon(uly).max_lon(lry).min_lat(lrx).max_lat(ulx)
		.output_file(out_file).temp_folder(tmp_folder));
	BOOST_LOG_TRIVIAL(debug) << "����Heg��ȡ.tif�ļ��ɹ�����ȡ���ļ�Ϊ��" << out_file;
}

void proc_MxD04_3k::Preprocess_aerosol::gdal_process(path const& in_file, path const& out_file, double min_lon,
	double max_lon, double min_lat, double max_lat)
{
	modis_api::Gdal_operation::translate_copy(in_file, out_file,
		modis_api::Gdal_operation::get_reproj_arg(min_lon, max_lon, min_lat, max_lat));
	BOOST_LOG_TRIVIAL(debug) << "����gdal_translate.exe����" << in_file << "�ļ��ɹ������Ϊ��" << out_file;
}

void proc_MxD04_3k::Preprocess_aerosol::scale_process(path const& in_file, path const& out_file)
{
	//����GDAL�����Scale���Heg��ȡ��.tif�ļ���·��
	std::filesystem::copy_file(in_file, out_file);
	boost::optional<arma::fmat> oMat = modis_api::Gdal_operation::read_tif_to_fmat(out_file.string());
	BOOST_LOG_TRIVIAL(debug) << "load file: " << out_file << "\n" << modis_api::Mat_operation::mat_desc(*oMat);
	scale(*oMat);
	BOOST_LOG_TRIVIAL(debug) << "after scale: \n" << modis_api::Mat_operation::mat_desc(*oMat);

	modis_api::Gdal_operation::write_fmat_to_tif(out_file.string(), *oMat);
	modis_api::Gdal_operation::set_no_data_value(out_file, NO_DATA_VALUE);
	BOOST_LOG_TRIVIAL(debug) << "set no_data_value=" << NO_DATA_VALUE << " to " << out_file;
}

int proc_MxD04_3k::Preprocess_aerosol::scale(arma::fmat& m)
{
	m = (m - OFFSET)*SCALE;
	m.elem(arma::find(m <= 0)).fill(NO_DATA_VALUE);
	return 0;
}

/**
 * \brief �ϳɲ����
 * \param in_file_vec ���ںϳɵ��ļ��б�
 * \param out_file �ϳ�����ļ�
 */
void proc_MxD04_3k::Preprocess_aerosol::combine(std::vector<path> const& in_file_vec, path const& out_file)
{
	BOOST_LOG_TRIVIAL(info) << "�ϳɴ�������ϳ�" << in_file_vec.size() << "��.tif�ļ�";
	std::vector<arma::fmat> matVec = modis_api::Mat_operation::load_mat_vec(in_file_vec);

	std::optional<arma::fmat> oMeanMat = modis_api::Mat_operation::mean_mat(matVec, NO_DATA_VALUE);

	BOOST_LOG_TRIVIAL(debug) << "׼��������ս���ļ���" << out_file;
	modis_api::File_operation::prepare_file_output_envir(out_file);

	std::filesystem::copy_file(in_file_vec.at(0), out_file);
	BOOST_LOG_TRIVIAL(debug) << "�����ļ���" << in_file_vec.at(0) << " -> " << out_file;

	modis_api::Gdal_operation::write_fmat_to_tif(out_file.string(), *oMeanMat);
	BOOST_LOG_TRIVIAL(debug) << "�ɹ������ս���ļ�" << out_file << "д��DNֵ";

	modis_api::Gdal_operation::set_no_data_value(out_file, NO_DATA_VALUE);
	BOOST_LOG_TRIVIAL(debug) << "����" << out_file << "��NODATAVALUEΪ" << NO_DATA_VALUE;
}

proc_MxD04_3k::Aerosol_param proc_MxD04_3k::load_params(path const& yml_path)
{
	Aerosol_param params;
	try
	{
		params = proc_MxD04_3k::load_aerosol_params(yml_path);
	}
	catch (std::exception& ex)
	{
		BOOST_LOG_TRIVIAL(error) << ex.what();
		BOOST_LOG_TRIVIAL(error) << "load config file failed!";
		exit(EXIT_FAILURE);
	}
	return params;
}
