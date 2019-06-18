#include "pch.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Mat_operation.h"
#include "../modis_api/Mrt_utils.h"
#include "Preprocess_st.h"
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <sstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>


void proc_MxD11A1::Preprocess_st::check_node(const string& yml_path, const YAML::Node& node, const string& attr_name)
{
	if (!node[attr_name])
	{
		BOOST_LOG_TRIVIAL(error) << yml_path << "�ļ���δ�ҵ�[" << attr_name << "]������";
		exit(EXIT_FAILURE);
	}
}

/**
 * \brief ����output_projection_parameters
 * \param old
 * \return
 */
string proc_MxD11A1::Preprocess_st::wrap_output_projection_parameters_str(const string& old)
{
	ostringstream oss;
	vector<string> svec;
	split(svec, old, boost::is_any_of(" "));
	if (svec.size() != 15) throw runtime_error("wrong output_projection_parameters!");
	oss << "( " << endl << svec.at(0) << " " << svec.at(1) << " " << svec.at(2) << endl
		<< svec.at(3) << " " << svec.at(4) << " " << svec.at(5) << endl
		<< svec.at(6) << " " << svec.at(7) << " " << svec.at(8) << endl
		<< svec.at(9) << " " << svec.at(10) << " " << svec.at(11) << endl
		<< svec.at(12) << " " << svec.at(13) << " " << svec.at(14) << " )";
	BOOST_LOG_TRIVIAL(debug) << "wrap output_projection_parameters: " << oss.str();
	return oss.str();
}

proc_MxD11A1::Preprocess_st::Preprocess_st() = default;
proc_MxD11A1::Preprocess_st::~Preprocess_st() = default;

const float NO_DATA_VALUE = 0;
const float SCALE = 0.02f;
const float OFFSET = 0.0f;

/**
 * \brief Ԥ����ر��¶�����
 * \param yml_path .yml�����ļ�·��
 * \param node
 */
void proc_MxD11A1::Preprocess_st::preprocess(const string& yml_path, const YAML::Node& node)
{
	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "��ʼ����Surface TemperatureԤ����ʹ�õ�.yml�ļ�Ϊ��" << yml_path;

	vector<string> attr_names = { "HDFListFile","TmpPath", "MinLon", "MaxLon", "MinLat",
		"MaxLat", "OutputImageFile", "ResamplingType", "OutputProjectionType",
		"OutputProjectionParameters","Datum","OutputPixelSize" };
	for_each(attr_names.cbegin(), attr_names.cend(), [&yml_path, &node](const string& attr_name)
	{ check_node(yml_path, node, attr_name); });

	const string hdf_file_list_file_path = node["HDFListFile"].as<string>();
	if (!boost::filesystem::exists(hdf_file_list_file_path))
	{
		BOOST_LOG_TRIVIAL(error) << ".hdf�б��ļ�" << hdf_file_list_file_path << "������";
		exit(EXIT_FAILURE);
	}

	const double min_lon = node["MinLon"].as<double>();
	const double max_lon = node["MaxLon"].as<double>();
	const double min_lat = node["MinLat"].as<double>();
	const double max_lat = node["MaxLat"].as<double>();
	const string output_image_file = node["OutputImageFile"].as<string>();

	string temp_dir = node["TmpPath"].as<string>();

	const string spatial_subset_type = node["SpatialSubsetType"].as<string>();
	const string resampling_type = node["ResamplingType"].as<string>();
	const string output_projection_type = node["OutputProjectionType"].as<string>();
	const string output_projection_parameters =
		wrap_output_projection_parameters_str(node["OutputProjectionParameters"].as<string>());
	const string datum = node["Datum"].as<string>();
	const double output_pixel_size = node["OutputPixelSize"].as<double>();

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
		const string mrt_tif_path = temp_dir + boost::filesystem::path(hdf_file_path).stem().string() + "_mrt.tif";
		modis_api::Mrt_utils::run_mrt(hdf_file_path, min_lon, max_lon, min_lat, max_lat, mrt_tif_path, temp_dir,
			spatial_subset_type, resampling_type, output_projection_type,
			output_projection_parameters, datum, output_pixel_size);

		if (!boost::filesystem::exists(mrt_tif_path))
		{
			BOOST_LOG_TRIVIAL(error) << "δ�ҵ���ȡ��.tif�ļ���" << mrt_tif_path;
			BOOST_LOG_TRIVIAL(error) << "����" << hdf_file_path << "�ļ��Ĵ���";
			continue;
		}
		BOOST_LOG_TRIVIAL(debug) << "����MRT��ȡ.tif�ļ��ɹ�����ȡ���ļ�Ϊ��" << mrt_tif_path;

		const string mrt_gdal_tif_path = temp_dir + boost::filesystem::path(hdf_file_path).stem().string() + "_gdal.tif";
		// Fix iss01, see BUGFIX.md
		const string gdal_arguments = "-a_srs \"+proj=eqc +lat_ts=0 +lat_0=0 +lon_0=0 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs\" -ot Float32";
		BOOST_LOG_TRIVIAL(debug) << "gdal argument:" << gdal_arguments;
		// ����gdal�����������ͣ��ĳ�Float32
		modis_api::Gdal_operation::translate_copy(mrt_tif_path, mrt_gdal_tif_path, gdal_arguments);

		const string mrt_gdal_scaled_tif_path = temp_dir + boost::filesystem::path(hdf_file_path).stem().string() + "_gdal_scaled.tif";
		if (boost::filesystem::exists(mrt_gdal_scaled_tif_path)) boost::filesystem::remove(mrt_gdal_scaled_tif_path);
		boost::filesystem::copy_file(mrt_gdal_tif_path, mrt_gdal_scaled_tif_path);
		boost::optional<arma::fmat> mat_optional = modis_api::Gdal_operation::read_tif_to_fmat(mrt_gdal_scaled_tif_path);
		if (!mat_optional)
		{
			BOOST_LOG_TRIVIAL(error) << "��ȡ" << mrt_gdal_scaled_tif_path << "�ļ���Ϣʧ�ܣ�����" << hdf_file_path << "�ļ��Ĵ���";
			continue;
		}
		BOOST_LOG_TRIVIAL(debug) << "����tif�ļ���" << mrt_gdal_scaled_tif_path;
		BOOST_LOG_TRIVIAL(debug) << modis_api::Mat_operation::mat_desc(*mat_optional);

		mat_optional->transform([](float dn) -> float
		{
			if (dn - NO_DATA_VALUE < 1E-5) return 0;
			return (dn - OFFSET) * SCALE;
		});

		BOOST_LOG_TRIVIAL(debug) << "ִ��Scale������";
		BOOST_LOG_TRIVIAL(debug) << modis_api::Mat_operation::mat_desc(*mat_optional);

		modis_api::Gdal_operation::write_fmat_to_tif(mrt_gdal_scaled_tif_path, *mat_optional);
		BOOST_LOG_TRIVIAL(debug) << "Scale������ɣ��������ļ�Ϊ��" << mrt_gdal_scaled_tif_path;

		preprocessed_file_paths.push_back(mrt_gdal_scaled_tif_path);
		BOOST_LOG_TRIVIAL(info) << hdf_file_path << "�ļ�Ԥ�������";
	}

	if (preprocessed_file_paths.empty())
	{
		BOOST_LOG_TRIVIAL(error) << "δ�ҵ����ϳɵ������ļ���Ԥ��������˳�";
		exit(EXIT_FAILURE);
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
