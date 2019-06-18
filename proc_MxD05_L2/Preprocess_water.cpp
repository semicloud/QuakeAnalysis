#include "pch.h"
#include "Preprocess_water.h"
#include <string>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include "../modis_api/File_operation.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Heg_utils.h"
#include "../modis_api/Mat_operation.h"


using namespace std;

proc_MxD05_L2::Preprocess_water::Preprocess_water() = default;


proc_MxD05_L2::Preprocess_water::~Preprocess_water() = default;

const string FIELD_NAME = "Water_Vapor_Infrared|";
int BAND_NUMBER = 1;
double OUTPUT_PIXEL_SIZE_X = 5000;
double OUTPUT_PIXEL_SIZE_Y = 5000;
int MAX_SLEEP = 5;
float NO_DATA_VALUE = -9999;
float OFFSET = 0;
float SCALE = 0.01f;

const string OBJECT_NAME = "mod05";
const string ELLIPSOID_CODE = "WGS84";
const string OUTPUT_TYPE = "GEO";

void proc_MxD05_L2::Preprocess_water::check_node(const std::string& yml_path, const YAML::Node& node, const std::string& attr_name)
{
	if (!node[attr_name])
	{
		BOOST_LOG_TRIVIAL(error) << yml_path << "�ļ���δ�ҵ�[" << attr_name << "]������";
		exit(EXIT_FAILURE);
	}
}

void proc_MxD05_L2::Preprocess_water::preprocess(const std::string& yml_path, const YAML::Node& node)
{
	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "��ʼ����Water VaporԤ����ʹ�õ�.yml�ļ�Ϊ��" << yml_path;

	vector<string> attr_names = { "HDFListFile", "MinLon", "MaxLon", "MinLat",
		"MaxLat", "ResamplingType","OutputProjectionType",
		"OutputProjectionParameters","OutputImageFile", "TmpPath" };
	for_each(attr_names.cbegin(), attr_names.cend(), [&yml_path, &node](const string& attr_name)
	{ check_node(yml_path, node, attr_name); });
	//.hdf�ļ��б��ļ������ļ���һ��.txt�ļ�
	const string hdf_file_list_file_path = node["HDFListFile"].as<string>();
	if (!boost::filesystem::exists(hdf_file_list_file_path))
	{
		BOOST_LOG_TRIVIAL(error) << ".hdf�б��ļ�" << hdf_file_list_file_path << "������";
		exit(EXIT_FAILURE);
	}

	//const string object_name = node["ObjectName"].as<string>();
	const double min_lon = node["MinLon"].as<double>();
	const double max_lon = node["MaxLon"].as<double>();
	const double min_lat = node["MinLat"].as<double>();
	const double max_lat = node["MaxLat"].as<double>();
	const string resampling_type = node["ResamplingType"].as<string>();
	//const string ellipsoid_code = node["EllipsoidCode"].as<string>();
	const string output_projection_type = node["OutputProjectionType"].as<string>();
	const string output_projection_parameters = node["OutputProjectionParameters"].as<string>();
	//const string output_type = node["OutputType"].as<string>();
	const string output_image_file = node["OutputImageFile"].as<string>(); //final output
	string temp_dir = node["TmpPath"].as<string>();
	if (temp_dir.at(temp_dir.size() - 1) != '\\')
	{
		temp_dir.push_back('\\');
		BOOST_LOG_TRIVIAL(debug) << "�޸�TempĿ¼��" << temp_dir;
	}
	BOOST_LOG_TRIVIAL(info) << yml_path << "�ļ���Ч�Լ�����";

	vector<string> hdf_files = modis_api::File_operation::read_file_all_lines(hdf_file_list_file_path);
	BOOST_LOG_TRIVIAL(info) << "���账��" << hdf_files.size() << "��.hdf�ļ�";
	for (const string& p : hdf_files)
		BOOST_LOG_TRIVIAL(debug) << "\t" << p;

	if (!boost::filesystem::exists(temp_dir))
	{
		boost::filesystem::create_directories(temp_dir);
		BOOST_LOG_TRIVIAL(debug) << "����TempĿ¼��" << temp_dir;
	}
	modis_api::File_operation::clear_directory(temp_dir);
	BOOST_LOG_TRIVIAL(info) << "TempĿ¼��" << temp_dir << "׼�����";

	vector<string> preprocessed_file_paths;
	for (const string& hdf_file_path : hdf_files)
	{
		double ulx, uly, lrx, lry;
		if (!modis_api::Gdal_operation::read_geo_bound(hdf_file_path, "\":mod05:Water_Vapor_Infrared", ulx, uly, lrx, lry))
		{
			BOOST_LOG_TRIVIAL(error) << "��ȡGeoBoundʧ�ܣ�����" << hdf_file_path << "�ļ��Ĵ���";
			continue;
		}
		//Heg��ȡ����.tif�ļ�·��
		const string heg_tif_path = temp_dir + boost::filesystem::path(hdf_file_path).stem().string() + "_heg.tif";
		modis_api::Heg_utils::run_heg(hdf_file_path, OBJECT_NAME, FIELD_NAME, BAND_NUMBER,
			OUTPUT_PIXEL_SIZE_X, OUTPUT_PIXEL_SIZE_Y, uly, lry, lrx, ulx,
			resampling_type, output_projection_type, ELLIPSOID_CODE, output_projection_parameters,
			heg_tif_path, OUTPUT_TYPE, temp_dir);

		/*
		 * heg��ôţ�Ƶ������system()���������У�ֻ��ʹ��CreateProcess����
		 * CreateProcess�����˾Ͳ����ˣ��������ó���˯һ��ʱ�䣬��heg��ȡ���
		 */

		 // auto sleep_time = 0;
		 // while (!boost::filesystem::exists(heg_tif_path))
		 // {
		 // 	++sleep_time;
		 // 	Sleep(1000);
		 // 	if (sleep_time == MAX_SLEEP)
		 // 	{
		 // 		BOOST_LOG_TRIVIAL(error) << "����Heg��ȡ.tif�ļ�ʧ��";
		 // 		break;
		 // 	}
		 // }

		//Sleep(10 * 1000);

		if (!boost::filesystem::exists(heg_tif_path))
		{
			BOOST_LOG_TRIVIAL(error) << "δ�ҵ���ȡ��.tif�ļ���" << heg_tif_path;
			BOOST_LOG_TRIVIAL(error) << "����" << hdf_file_path << "�ļ��Ĵ���";
			continue;
		}
		BOOST_LOG_TRIVIAL(debug) << "����Heg��ȡ.tif�ļ��ɹ�����ȡ���ļ�Ϊ��" << heg_tif_path;

		//����GDAL�����Heg��ȡ��.tif�ļ�·��
		const string heg_gdal_tif_path = temp_dir + boost::filesystem::path(hdf_file_path).stem().string() + "_heg_gdal.tif";
		const string gdal_argument = str(boost::format("-ot Float32 -projwin %1% %2% %3% %4% -projwin_srs EPSG:4326 -of GTiff -co \"COMPRESS = LZW\" -co \"INTERLEAVE = BAND\"")
			% min_lon % max_lat % max_lon % min_lat);
		modis_api::Gdal_operation::translate_copy(heg_tif_path, heg_gdal_tif_path, gdal_argument);
		if (!boost::filesystem::exists(heg_gdal_tif_path))
		{
			BOOST_LOG_TRIVIAL(error) << "����gdal_translate.exe����" << heg_tif_path << "�ļ�ʧ�ܣ�����" << hdf_file_path << "�ļ��Ĵ���";
			BOOST_LOG_TRIVIAL(error) << "gdal_argument: " << gdal_argument;
			continue;
		}
		BOOST_LOG_TRIVIAL(debug) << "����gdal_translate.exe����" << heg_tif_path << "�ļ��ɹ������Ϊ��" << heg_gdal_tif_path;

		//����GDAL�����Scale���Heg��ȡ��.tif�ļ���·��
		const string heg_gdal_scaled_tif_path = temp_dir + boost::filesystem::path(hdf_file_path).stem().string() + "_heg_gdal_scaled.tif";
		boost::filesystem::copy_file(heg_gdal_tif_path, heg_gdal_scaled_tif_path);
		auto mat_optional = modis_api::Gdal_operation::read_tif_to_fmat(heg_gdal_scaled_tif_path);
		if (!mat_optional)
		{
			BOOST_LOG_TRIVIAL(error) << "��ȡ" << heg_gdal_scaled_tif_path << "�ļ���Ϣʧ�ܣ�����" << hdf_file_path << "�ļ��Ĵ���";
			continue;
		}
		BOOST_LOG_TRIVIAL(debug) << "����tif�ļ���" << heg_gdal_scaled_tif_path;
		BOOST_LOG_TRIVIAL(debug) << modis_api::Mat_operation::mat_desc(*mat_optional);

		mat_optional->transform([](float dn) -> float
		{
			if (dn < 0) return 0;
			//if ((dn - NO_DATA_VALUE) < 1E-5) return 0;
			return (dn - OFFSET) * SCALE;
		});

		BOOST_LOG_TRIVIAL(debug) << "ִ��Scale������";
		BOOST_LOG_TRIVIAL(debug) << modis_api::Mat_operation::mat_desc(*mat_optional);

		modis_api::Gdal_operation::write_fmat_to_tif(heg_gdal_scaled_tif_path, *mat_optional);
		BOOST_LOG_TRIVIAL(debug) << "Scale������ɣ��������ļ�Ϊ��" << heg_gdal_scaled_tif_path;

		preprocessed_file_paths.push_back(heg_gdal_scaled_tif_path);
		BOOST_LOG_TRIVIAL(info) << hdf_file_path << "�ļ�Ԥ�������";
	}

	BOOST_LOG_TRIVIAL(info) << "�ϳɴ�������ϳ�" << preprocessed_file_paths.size() << "��.tif�ļ�";
	for (const string& p : preprocessed_file_paths)
		BOOST_LOG_TRIVIAL(debug) << "\t" << p;

	vector<arma::fmat> mat_list;
	std::transform(preprocessed_file_paths.cbegin(), preprocessed_file_paths.cend(), back_inserter(mat_list),
		[](const string& p) { return *modis_api::Gdal_operation::read_tif_to_fmat(p);  });
	auto mean_mat_optional = modis_api::Mat_operation::mean_mat_by_each_pixel(mat_list);
	if (!mean_mat_optional)
	{
		BOOST_LOG_TRIVIAL(error) << "����ϳɳ��ִ���";
		return;
	}

	BOOST_LOG_TRIVIAL(debug) << "׼��������ս���ļ���" << output_image_file;
	if (boost::filesystem::exists(output_image_file))
	{
		boost::filesystem::remove(output_image_file);
		BOOST_LOG_TRIVIAL(debug) << "���ս���ļ���" << output_image_file << "�Ѵ��ڣ�ɾ��֮";
	}
	boost::filesystem::path p(output_image_file);
	if (!exists(p.parent_path()))
	{
		create_directories(p.parent_path());
		BOOST_LOG_TRIVIAL(debug) << "����Ŀ¼��" << p.parent_path().string();
	}

	boost::filesystem::copy_file(preprocessed_file_paths[0], output_image_file);
	BOOST_LOG_TRIVIAL(debug) << "�����ļ���" << preprocessed_file_paths[0] << " -> " << output_image_file;

	modis_api::Gdal_operation::write_fmat_to_tif(output_image_file, *mean_mat_optional);
	BOOST_LOG_TRIVIAL(debug) << "�ɹ������ս���ļ�" << output_image_file << "д��DNֵ";

	BOOST_LOG_TRIVIAL(info) << "Ԥ������ɣ����ս���ļ�Ϊ��" << output_image_file;
	BOOST_LOG_TRIVIAL(info) << "";
}
