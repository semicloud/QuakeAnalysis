#include "pch.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Heg_utils.h"
#include "../modis_api/Mat_operation.h"
#include "Preprocess_water.h"
#include <boost/log/trivial.hpp>
#include <filesystem>
#include <optional>
#include <string>

proc_MxD05_L2::Preprocess_water::Preprocess_water() = default;
proc_MxD05_L2::Preprocess_water::~Preprocess_water() = default;

const std::string FIELD_NAME = "Water_Vapor_Infrared|";
int BAND_NUMBER = 1;
double OUTPUT_PIXEL_SIZE_X = 5000;
double OUTPUT_PIXEL_SIZE_Y = 5000;
int MAX_SLEEP = 5;
float NO_DATA_VALUE = -1;
float OFFSET = 0;
float SCALE = 0.001f;
const std::string OBJECT_NAME = "mod05";
const std::string ELLIPSOID_CODE = "WGS84";
const std::string OUTPUT_TYPE = "GEO";

/**
 * \brief Ԥ����ˮ������
 * \param yml_path .yml�ļ�·��
 * \param node YAML::Node
 * \param debug_mode
 */
void proc_MxD05_L2::Preprocess_water::preprocess(const std::string& yml_path, const YAML::Node& node, bool debug_mode)
{
	BOOST_LOG_TRIVIAL(info) << "\n��ʼ����Water VaporԤ����ʹ�õ�.yml�ļ�Ϊ��" << yml_path;
	Water_vapor_param param = load_param(yml_path);
	path tmp_dir(param.tmp_path());
	modis_api::File_operation::clear_directory(tmp_dir.string());

	auto tmp_name = [=](path const& p1, std::string const& appender) -> path
	{ return modis_api::File_operation::get_tmp_file(p1, tmp_dir, appender); };

	std::vector<path> processed_files;
	for (path const& hdf_file : param.hdf_files())
	{
		const path heg_tif_file(tmp_name(hdf_file, "heg"));
		const path gdal_tif_file(tmp_name(hdf_file, "heg_gdal"));
		const path scaled_tif_file(tmp_name(hdf_file, "heg_gdal_scaled"));

		heg_process(hdf_file, heg_tif_file, tmp_dir);
		assert(std::filesystem::exists(heg_tif_file));

		gdal_process(heg_tif_file, gdal_tif_file, param.min_lon(), param.max_lon(), param.min_lat(), param.max_lat());
		assert(std::filesystem::exists(gdal_tif_file));

		scale_process(gdal_tif_file, scaled_tif_file);
		assert(std::filesystem::exists(scaled_tif_file));

		processed_files.push_back(scaled_tif_file);
		BOOST_LOG_TRIVIAL(info) << hdf_file << "�ļ��������";
	}

	combine(processed_files, param.output_image_file());

	BOOST_LOG_TRIVIAL(info) << "Ԥ������ɣ����ս���ļ�Ϊ��" << param.output_image_file() << "\n";

	if (debug_mode) return;

	modis_api::File_operation::clear_directory(tmp_dir.string());
	BOOST_LOG_TRIVIAL(info) << "TempĿ¼" << tmp_dir << "�����..";
}

void proc_MxD05_L2::Preprocess_water::heg_process(path const& hdf_file, path const& out_file, path const& tmp_folder)
{
	double ulx = 0, uly = 0, lrx = 0, lry = 0;
	modis_api::Gdal_operation::read_geo_bound(hdf_file, ulx, uly, lrx, lry);
	modis_api::Heg_utils::run_heg(modis_api::Heg_param(hdf_file)
		.min_lon(uly).max_lon(lry).min_lat(lrx).max_lat(ulx)
		.output_file(out_file).temp_folder(tmp_folder));
	BOOST_LOG_TRIVIAL(debug) << "����Heg��ȡ.tif�ļ��ɹ�����ȡ���ļ�Ϊ��" << out_file;
}

void proc_MxD05_L2::Preprocess_water::gdal_process(path const& in_file, path const& out_file, double min_lon,
	double max_lon, double min_lat, double max_lat)
{
	modis_api::Gdal_operation::translate_copy(in_file, out_file,
		modis_api::Gdal_operation::get_reproj_arg(min_lon, max_lon, min_lat, max_lat));
	BOOST_LOG_TRIVIAL(debug) << "����gdal_translate.exe����" << in_file << "�ļ��ɹ������Ϊ��" << out_file;
}

void proc_MxD05_L2::Preprocess_water::scale_process(path const& in_file, path const& out_file)
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

int proc_MxD05_L2::Preprocess_water::scale(arma::fmat& m)
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
void proc_MxD05_L2::Preprocess_water::combine(std::vector<path> const& in_file_vec, path const& out_file)
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


proc_MxD05_L2::Water_vapor_param proc_MxD05_L2::load_param(path const& yml_path)
{
	Water_vapor_param param;
	try
	{
		param = proc_MxD05_L2::load_water_vapor_param(yml_path);
	}
	catch (std::exception& ex)
	{
		BOOST_LOG_TRIVIAL(error) << ex.what();
		BOOST_LOG_TRIVIAL(error) << "load config file failed!";
		exit(EXIT_FAILURE);
	}
	return param;
}
