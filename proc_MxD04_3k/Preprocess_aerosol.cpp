#include "pch.h"
#include "Preprocess_aerosol.h"
#include "../modis_api/File_operation.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Heg_utils.h"
#include "../modis_api/Mat_operation.h"
#include <algorithm>
#include <armadillo>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <string>
#include <filesystem>
#include <vector>

proc_MxD04_3k::Preprocess_aerosol::Preprocess_aerosol() = default;
proc_MxD04_3k::Preprocess_aerosol::~Preprocess_aerosol() = default;

const std::string OBJECT_NAME = "mod04";
const std::string ELLIPSOID_CODE = "WGS84";
const std::string OUTPUT_TYPE = "GEO";
const std::string FIELD_NAME = "Optical_Depth_Land_And_Ocean|";
const int BAND_NUMBER = 1;
const double OUTPUT_PIXEL_SIZE_X = 3000, OUTPUT_PIXEL_SIZE_Y = 3000;
const float SCALE = 0.001f, OFFSET = 0.0f;
const int MAX_SLEEP = 5;
const float NO_DATA_VALUE = -9999; //经过GDAL裁剪后，NO_DATA_VALUE是否还是-9999

/**
 * \brief 检查.yml文件中的配置项是否有效
 * \param yml_path .yml文件路径
 * \param node 节点
 * \param attr_name 属性名
 */
void proc_MxD04_3k::Preprocess_aerosol::check_node(const std::string& yml_path, const YAML::Node& node, const std::string& attr_name)
{
	if (!node[attr_name])
	{
		BOOST_LOG_TRIVIAL(error) << yml_path << "文件中未找到[" << attr_name << "]配置项";
		exit(EXIT_FAILURE);
	}
}

/**
 * \brief 加载字符串模板文件
 * \param file_path 字符串模板文件
 * \return
 */
std::string proc_MxD04_3k::Preprocess_aerosol::load_template_string(const std::string& file_path)
{
	std::ifstream ifs(file_path);
	if (ifs)
	{
		std::stringstream ss;
		ss << ifs.rdbuf();
		ifs.close();
		return ss.str();
	}
	throw std::runtime_error("load file " + file_path + " failed!");
}

/**
 * \brief 预处理气溶胶数据
 * \param yml_path .yml文件路径
 * \param node YAML::Node
 */
void proc_MxD04_3k::Preprocess_aerosol::preprocess(const std::string& yml_path, const YAML::Node& node, bool debug_mode)
{
	using namespace boost;
	using namespace std;
	namespace fs = std::filesystem;

	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "开始进行Aerosol预处理，使用的.yml文件为：" << yml_path;
	vector<string> attr_names = { "HDFListFile", "MinLon", "MaxLon", "MinLat",
		"MaxLat", "ResamplingType","OutputProjectionType",
		"OutputProjectionParameters","OutputImageFile", "TmpPath" };
	for_each(attr_names.cbegin(), attr_names.cend(), [&yml_path, &node](const string& attr_name)
	{ check_node(yml_path, node, attr_name); });
	//.hdf文件列表文件，该文件是一个.txt文件
	const string hdf_file_list_file_path = node["HDFListFile"].as<string>();
	if (!fs::exists(hdf_file_list_file_path))
	{
		BOOST_LOG_TRIVIAL(error) << ".hdf列表文件" << hdf_file_list_file_path << "不存在";
		exit(EXIT_FAILURE);
	}

	const double min_lon = node["MinLon"].as<double>();
	const double max_lon = node["MaxLon"].as<double>();
	const double min_lat = node["MinLat"].as<double>();
	const double max_lat = node["MaxLat"].as<double>();
	const string resampling_type = node["ResamplingType"].as<string>();
	const string output_projection_type = node["OutputProjectionType"].as<string>();
	const string output_projection_parameters = node["OutputProjectionParameters"].as<string>();
	const string output_image_file = node["OutputImageFile"].as<string>(); //final output
	fs::path temp_dir(node["TmpPath"].as<string>());

	std::vector<string> hdf_files = modis_api::File_operation::read_file_all_lines(hdf_file_list_file_path);
	BOOST_LOG_TRIVIAL(info) << "共需处理" << hdf_files.size() << "个.hdf文件";
	for (const string& p : hdf_files)
		BOOST_LOG_TRIVIAL(debug) << "\t" << p;

	if (!exists(temp_dir))
	{
		create_directories(temp_dir);
		BOOST_LOG_TRIVIAL(debug) << "创建Temp目录：" << temp_dir;
	}
	modis_api::File_operation::clear_directory(temp_dir.string());
	BOOST_LOG_TRIVIAL(info) << "Temp目录：" << temp_dir << "准备完毕";

	std::vector<string> preprocessed_file_paths;
	for (const string& hdf_file_path : hdf_files)
	{
		double ulx, uly, lrx, lry;
		if (!modis_api::Gdal_operation::read_geo_bound_py_h5(hdf_file_path, temp_dir.string(), ulx, uly, lrx, lry))
		{
			BOOST_LOG_TRIVIAL(error) << "提取GeoBound失败，跳过" << hdf_file_path << "文件的处理";
			continue;
		}
		//Heg提取出的.tif文件路径
		const string heg_tif_path = (temp_dir / str(format("%1%_heg.%2%") % fs::path(hdf_file_path).stem().string() % "tif")).string();
		modis_api::Heg_utils::run_heg(hdf_file_path, OBJECT_NAME, FIELD_NAME, BAND_NUMBER,
			OUTPUT_PIXEL_SIZE_X, OUTPUT_PIXEL_SIZE_Y, uly, lry, lrx, ulx,
			resampling_type, output_projection_type, ELLIPSOID_CODE, output_projection_parameters,
			heg_tif_path, OUTPUT_TYPE, temp_dir.string());
		if (!fs::exists(heg_tif_path))
		{
			BOOST_LOG_TRIVIAL(error) << "未找到提取的.tif文件：" << heg_tif_path;
			BOOST_LOG_TRIVIAL(error) << "跳过" << hdf_file_path << "文件的处理";
			continue;
		}
		BOOST_LOG_TRIVIAL(debug) << "调用Heg提取.tif文件成功，提取的文件为：" << heg_tif_path;

		//经过GDAL处理的Heg提取的.tif文件路径
		const string heg_gdal_tif_path = (temp_dir / str(format("%1%_heg_gdal.tif") % fs::path(hdf_file_path).stem().string())).string();
		const string gdal_argument = str(format("-ot Float32 -projwin %1% %2% %3% %4% -projwin_srs EPSG:4326 -of GTiff -co \"COMPRESS = LZW\" -co \"INTERLEAVE = BAND\"")
			% min_lon % max_lat % max_lon % min_lat);
		modis_api::Gdal_operation::translate_copy(heg_tif_path, heg_gdal_tif_path, gdal_argument);
		if (!std::filesystem::exists(heg_gdal_tif_path))
		{
			BOOST_LOG_TRIVIAL(error) << "调用gdal_translate.exe处理" << heg_tif_path << "文件失败，跳过" << hdf_file_path << "文件的处理";
			BOOST_LOG_TRIVIAL(error) << "gdal_argument: " << gdal_argument;
			continue;
		}
		BOOST_LOG_TRIVIAL(debug) << "调用gdal_translate.exe处理" << heg_tif_path << "文件成功，结果为：" << heg_gdal_tif_path;

		//经过GDAL处理的Scale后的Heg提取的.tif文件的路径
		const string heg_gdal_scaled_tif_path = (temp_dir / str(format("%1%_heg_gdal_scaled.tif") % fs::path(hdf_file_path).stem().string())).string();
		fs::copy_file(heg_gdal_tif_path, heg_gdal_scaled_tif_path);
		auto mat_optional = modis_api::Gdal_operation::read_tif_to_fmat(heg_gdal_scaled_tif_path);
		if (!mat_optional)
		{
			BOOST_LOG_TRIVIAL(error) << "提取" << heg_gdal_scaled_tif_path << "文件信息失败，跳过" << hdf_file_path << "文件的处理";
			continue;
		}
		BOOST_LOG_TRIVIAL(debug) << "加载tif文件：" << heg_gdal_scaled_tif_path;
		BOOST_LOG_TRIVIAL(debug) << modis_api::Mat_operation::mat_desc(*mat_optional);

		mat_optional->transform([](float dn) -> float
		{
			if (dn < 0) return -1;
			return (dn - OFFSET) * SCALE;
		});

		BOOST_LOG_TRIVIAL(debug) << "执行Scale操作后：";
		BOOST_LOG_TRIVIAL(debug) << modis_api::Mat_operation::mat_desc(*mat_optional);

		modis_api::Gdal_operation::write_fmat_to_tif(heg_gdal_scaled_tif_path, *mat_optional);
		BOOST_LOG_TRIVIAL(debug) << "Scale操作完成，处理结果文件为：" << heg_gdal_scaled_tif_path;

		modis_api::Gdal_operation::set_no_data_value(heg_gdal_scaled_tif_path, -1);
		BOOST_LOG_TRIVIAL(debug) << "设置" << heg_gdal_scaled_tif_path << "的NODATAVALUE为-1";

		preprocessed_file_paths.push_back(heg_gdal_scaled_tif_path);
		BOOST_LOG_TRIVIAL(info) << hdf_file_path << "文件预处理完成";
	}

	BOOST_LOG_TRIVIAL(info) << "合成处理，共需合成" << preprocessed_file_paths.size() << "个.tif文件";
	for (const string& p : preprocessed_file_paths)
		BOOST_LOG_TRIVIAL(debug) << "\t" << p;
	std::vector<arma::fmat> mat_list;
	std::transform(preprocessed_file_paths.cbegin(), preprocessed_file_paths.cend(), std::back_inserter(mat_list),
		[](const string& p) { return *modis_api::Gdal_operation::read_tif_to_fmat(p);  });
	auto mean_mat_optional = modis_api::Mat_operation::mean_mat_by_each_pixel(mat_list, -1);
	if (!mean_mat_optional)
	{
		BOOST_LOG_TRIVIAL(error) << "矩阵合成出现错误";
		return;
	}

	BOOST_LOG_TRIVIAL(debug) << "准备输出最终结果文件：" << output_image_file;
	if (fs::exists(output_image_file))
	{
		fs::remove(output_image_file);
		BOOST_LOG_TRIVIAL(debug) << "最终结果文件：" << output_image_file << "已存在，删除之";
	}
	fs::path p(output_image_file);
	if (!fs::exists(p.parent_path()))
	{
		fs::create_directories(p.parent_path());
		BOOST_LOG_TRIVIAL(debug) << "创建目录：" << p.parent_path().string();
	}

	fs::copy_file(preprocessed_file_paths[0], output_image_file);
	BOOST_LOG_TRIVIAL(debug) << "复制文件：" << preprocessed_file_paths[0] << " -> " << output_image_file;

	modis_api::Gdal_operation::write_fmat_to_tif(output_image_file, *mean_mat_optional);
	BOOST_LOG_TRIVIAL(debug) << "成功向最终结果文件" << output_image_file << "写入DN值";

	modis_api::Gdal_operation::set_no_data_value(output_image_file, -1);
	BOOST_LOG_TRIVIAL(debug) << "设置" << output_image_file << "的NODATAVALUE为-1";

	BOOST_LOG_TRIVIAL(info) << "预处理完成，最终结果文件为：" << output_image_file;
	BOOST_LOG_TRIVIAL(info) << "";
	// 非debug模式下，程序运行结束清空temp目录
	if (!debug_mode)
	{
		modis_api::File_operation::clear_directory(temp_dir.string());
		BOOST_LOG_TRIVIAL(info) << "Temp目录" << temp_dir << "已清空..";
	}

}

