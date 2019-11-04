#include "pch.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Heg_utils.h"
#include "../modis_api/Mat_operation.h"
#include "Preprocess_aerosol.h"
#include "Aerosol_param.h"
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
void proc_MxD04_3k::Preprocess_aerosol::preprocess(const std::string& ymlPath, const YAML::Node& node, bool useDebugMode)
{
	using namespace std;

	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "��ʼ����AerosolԤ����ʹ�õ�.yml�ļ�Ϊ��" << ymlPath;

	Aerosol_param param = load_params(ymlPath);
	std::filesystem::path tmpFolder(node["TmpPath"].as<string>());
	prepare_tmp_folder(tmpFolder);

	std::vector<std::filesystem::path> preprocessed_tif_path_vec;
	for (std::filesystem::path const& hdf_file : param.hdf_files())
	{
		const string hdf_name_no_ext = hdf_file.stem().string();
		const std::string heg_tif_file_name = (boost::format("%1%_heg.%2%") % hdf_name_no_ext % "tif").str();
		const std::filesystem::path heg_tif_file = tmpFolder / heg_tif_file_name;  //Heg��ȡ����.tif�ļ�·��
		const std::string gdal_tif_file_name = (boost::format("%1%_heg_gdal.tif") % hdf_name_no_ext).str();
		const std::filesystem::path tif_gdal_path = tmpFolder / gdal_tif_file_name;  //gdal�����.tif�ļ�·��
		const std::string tif_scale_name = (boost::format("%1%_heg_gdal_scaled.tif") % hdf_name_no_ext).str();
		const std::filesystem::path tif_scale_path = tmpFolder / tif_scale_name;  //scale���.tif�ļ�·��

		// ! x��γ�ȣ�y�Ǿ���
		// ! ulx�����γ��
		// ! lrx����Сγ��
		// ! uly����С����
		// ! lry����󾭶�
		double ulx = 0, uly = 0, lrx = 0, lry = 0;
		modis_api::Gdal_operation::read_geo_bound(hdf_file, ulx, uly, lrx, lry);
		modis_api::Heg_utils::run_heg(modis_api::Heg_param(hdf_file)
			.min_lon(uly).max_lon(lry).min_lat(lrx).max_lat(ulx)
			.output_file(heg_tif_file).temp_folder(tmpFolder));
		BOOST_LOG_TRIVIAL(debug) << "����Heg��ȡ.tif�ļ��ɹ�����ȡ���ļ�Ϊ��" << heg_tif_file;

		//����GDAL�����Heg��ȡ��.tif�ļ�·��
		modis_api::Gdal_operation::translate_copy(heg_tif_file, tif_gdal_path,
			get_gdal_argument(param.min_lon(), param.max_lon(), param.min_lat(), param.max_lat()));
		BOOST_LOG_TRIVIAL(debug) << "����gdal_translate.exe����" << heg_tif_file << "�ļ��ɹ������Ϊ��" << tif_gdal_path;

		//����GDAL�����Scale���Heg��ȡ��.tif�ļ���·��
		std::filesystem::copy_file(tif_gdal_path, tif_scale_path);
		boost::optional<arma::fmat> oMat = modis_api::Gdal_operation::read_tif_to_fmat(tif_scale_path.string());
		if (!oMat)
		{
			BOOST_LOG_TRIVIAL(error) << "��ȡ" << tif_scale_path << "�ļ�����ʧ�ܣ������˳�";
			exit(EXIT_FAILURE);
		}
		BOOST_LOG_TRIVIAL(debug) << "����tif�ļ���" << tif_scale_path;
		BOOST_LOG_TRIVIAL(debug) << modis_api::Mat_operation::mat_desc(*oMat);
		scale(*oMat);
		BOOST_LOG_TRIVIAL(debug) << "ִ��Scale������";
		BOOST_LOG_TRIVIAL(debug) << modis_api::Mat_operation::mat_desc(*oMat);

		modis_api::Gdal_operation::write_fmat_to_tif(tif_scale_path.string(), *oMat);
		BOOST_LOG_TRIVIAL(debug) << "Scale������ɣ��������ļ�Ϊ��" << tif_scale_path;

		modis_api::Gdal_operation::set_no_data_value(tif_scale_path, NO_DATA_VALUE);
		BOOST_LOG_TRIVIAL(debug) << "����" << tif_scale_path << "��NODATAVALUEΪ" << NO_DATA_VALUE;

		preprocessed_tif_path_vec.push_back(tif_scale_path);
		BOOST_LOG_TRIVIAL(info) << hdf_file << "�ļ�Ԥ�������";
	}

	BOOST_LOG_TRIVIAL(info) << "�ϳɴ�������ϳ�" << preprocessed_tif_path_vec.size() << "��.tif�ļ�";
	std::vector<arma::fmat> matVec = readToMat(preprocessed_tif_path_vec);

	std::optional<arma::fmat> oMeanMat = modis_api::Mat_operation::mean_mat(matVec, NO_DATA_VALUE);
	if (!oMeanMat)
	{
		BOOST_LOG_TRIVIAL(error) << "����ϳɳ��ִ���";
		return;
	}

	BOOST_LOG_TRIVIAL(debug) << "׼��������ս���ļ���" << param.output_image_file();
	if (std::filesystem::exists(param.output_image_file()))
	{
		std::filesystem::remove(param.output_image_file());
		BOOST_LOG_TRIVIAL(debug) << "���ս���ļ���" << param.output_image_file() << "�Ѵ��ڣ�ɾ��֮";
	}

	if (param.output_image_file().has_parent_path() && !std::filesystem::exists(param.output_image_file().parent_path()))
	{
		std::filesystem::create_directories(param.output_image_file().parent_path());
		BOOST_LOG_TRIVIAL(debug) << "����Ŀ¼��" << param.output_image_file().parent_path().string();
	}

	std::filesystem::copy_file(preprocessed_tif_path_vec[0], param.output_image_file());
	BOOST_LOG_TRIVIAL(debug) << "�����ļ���" << preprocessed_tif_path_vec[0] << " -> " << param.output_image_file();

	modis_api::Gdal_operation::write_fmat_to_tif(param.output_image_file().string(), *oMeanMat);
	BOOST_LOG_TRIVIAL(debug) << "�ɹ������ս���ļ�" << param.output_image_file() << "д��DNֵ";

	modis_api::Gdal_operation::set_no_data_value(param.output_image_file(), NO_DATA_VALUE);
	BOOST_LOG_TRIVIAL(debug) << "����" << param.output_image_file() << "��NODATAVALUEΪ" << NO_DATA_VALUE;

	BOOST_LOG_TRIVIAL(info) << "Ԥ������ɣ����ս���ļ�Ϊ��" << param.output_image_file();
	BOOST_LOG_TRIVIAL(info) << "";
	// ��debugģʽ�£��������н������tempĿ¼
	if (!useDebugMode)
	{
		modis_api::File_operation::clear_directory(tmpFolder.string());
		BOOST_LOG_TRIVIAL(info) << "TempĿ¼" << tmpFolder << "�����..";
	}
}

int proc_MxD04_3k::Preprocess_aerosol::prepare_tmp_folder(std::filesystem::path const& tmpFolder)
{
	if (!std::filesystem::exists(tmpFolder))
	{
		std::filesystem::create_directories(tmpFolder);
		BOOST_LOG_TRIVIAL(debug) << "����TempĿ¼��" << tmpFolder;
	}
	modis_api::File_operation::clear_directory(tmpFolder.string());
	BOOST_LOG_TRIVIAL(info) << "TempĿ¼��" << tmpFolder << "׼�����";
	return 0;
}

int proc_MxD04_3k::Preprocess_aerosol::scale(arma::fmat& m)
{
	m = (m - OFFSET)*SCALE;
	m.elem(arma::find(m <= 0)).fill(NO_DATA_VALUE);
	return 0;
}

std::vector<arma::fmat> proc_MxD04_3k::Preprocess_aerosol::readToMat(std::vector<std::filesystem::path> const& tifPathVec)
{
	std::vector<arma::fmat> ans;
	for (const std::filesystem::path& tifPath : tifPathVec)
	{
		boost::optional<arma::fmat> oMat = modis_api::Gdal_operation::read_tif_to_fmat(tifPath.string());
		if (!oMat)
		{
			BOOST_LOG_TRIVIAL(error) << "��ȡ" << tifPath << "�ļ�ʧ�ܣ������˳�";
			exit(EXIT_FAILURE);
		}
		ans.push_back(*oMat);
	}
	return ans;
}

proc_MxD04_3k::Aerosol_param proc_MxD04_3k::load_params(std::filesystem::path const& yml_path)
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
