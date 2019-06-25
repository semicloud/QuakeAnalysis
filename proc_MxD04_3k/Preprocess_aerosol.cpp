#include "pch.h"
#include "Preprocess_aerosol.h"
#include <vector>
#include <string>
#include <algorithm>
#include <boost/log/trivial.hpp>
#include <armadillo>
#include <boost/filesystem.hpp>
#include "../modis_api/File_operation.h"
#include "../modis_api/Heg_utils.h"
#include "../modis_api/Gdal_operation.h"
#include "../modis_api/Mat_operation.h"
#include <boost/log/utility/setup/file.hpp>

proc_MxD04_3k::Preprocess_aerosol::Preprocess_aerosol() = default;
proc_MxD04_3k::Preprocess_aerosol::~Preprocess_aerosol() = default;

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
 * \brief 预处理水汽数据
 * \param yml_path .yml文件路径
 * \param node YAML::Node
 */
void proc_MxD04_3k::Preprocess_aerosol::preprocess(const std::string& yml_path, const YAML::Node& node, bool debug_mode)
{
	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "开始进行Aerosol预处理，使用的.yml文件为：" << yml_path;
	std::vector<std::string> attr_names = { "HDFListFile", "MinLon", "MaxLon", "MinLat",
		"MaxLat", "ResamplingType","OutputProjectionType",
		"OutputProjectionParameters","OutputImageFile", "TmpPath" };
	std::for_each(attr_names.cbegin(), attr_names.cend(), [&yml_path, &node](const std::string& attr_name)
	{ check_node(yml_path, node, attr_name); });
	//.hdf文件列表文件，该文件是一个.txt文件
	const std::string hdf_file_list_file_path = node["HDFListFile"].as<std::string>();
	if (!boost::filesystem::exists(hdf_file_list_file_path))
	{
		BOOST_LOG_TRIVIAL(error) << ".hdf列表文件" << hdf_file_list_file_path << "不存在";
		exit(EXIT_FAILURE);
	}

	//const string object_name = node["ObjectName"].as<string>();
	const double min_lon = node["MinLon"].as<double>();
	const double max_lon = node["MaxLon"].as<double>();
	const double min_lat = node["MinLat"].as<double>();
	const double max_lat = node["MaxLat"].as<double>();
	const std::string resampling_type = node["ResamplingType"].as<std::string>();
	//const string ellipsoid_code = node["EllipsoidCode"].as<string>();
	const std::string output_projection_type = node["OutputProjectionType"].as<std::string>();
	const std::string output_projection_parameters = node["OutputProjectionParameters"].as<std::string>();
	//const string output_type = node["OutputType"].as<string>();
	const std::string output_image_file = node["OutputImageFile"].as<std::string>(); //final output
	std::string temp_dir = node["TmpPath"].as<std::string>();
	if (temp_dir.at(temp_dir.size() - 1) != '\\')
	{
		temp_dir.push_back('\\');
		BOOST_LOG_TRIVIAL(debug) << "修改Temp目录：" << temp_dir;
	}
	BOOST_LOG_TRIVIAL(info) << yml_path << "文件有效性检查完毕";

	std::vector<std::string> hdf_files = modis_api::File_operation::read_file_all_lines(hdf_file_list_file_path);
	BOOST_LOG_TRIVIAL(info) << "共需处理" << hdf_files.size() << "个.hdf文件";
	for (const std::string& p : hdf_files)
		BOOST_LOG_TRIVIAL(debug) << "\t" << p;

	if (!boost::filesystem::exists(temp_dir))
	{
		boost::filesystem::create_directories(temp_dir);
		BOOST_LOG_TRIVIAL(debug) << "创建Temp目录：" << temp_dir;
	}
	modis_api::File_operation::clear_directory(temp_dir);
	BOOST_LOG_TRIVIAL(info) << "Temp目录：" << temp_dir << "准备完毕";

	std::vector<std::string> preprocessed_file_paths;
	for (const std::string& hdf_file_path : hdf_files)
	{
		double ulx, uly, lrx, lry;
		if (!modis_api::Gdal_operation::read_geo_bound(hdf_file_path, "\":mod04:Optical_Depth_Land_And_Ocean", ulx, uly, lrx, lry))
		{
			BOOST_LOG_TRIVIAL(error) << "提取GeoBound失败，跳过" << hdf_file_path << "文件的处理";
			continue;
		}
		//Heg提取出的.tif文件路径
		const std::string heg_tif_path = temp_dir + boost::filesystem::path(hdf_file_path).stem().string() + "_heg.tif";
		modis_api::Heg_utils::run_heg(hdf_file_path, OBJECT_NAME, FIELD_NAME, BAND_NUMBER,
			OUTPUT_PIXEL_SIZE_X, OUTPUT_PIXEL_SIZE_Y, uly, lry, lrx, ulx,
			resampling_type, output_projection_type, ELLIPSOID_CODE, output_projection_parameters,
			heg_tif_path, OUTPUT_TYPE, temp_dir);
		/*
		 * heg这么牛逼的软件，system()都不能运行，只能使用CreateProcess运行
		 * CreateProcess启动了就不管了，这样就让程序睡一段时间，等heg提取完毕
		 */
		 // auto sleep_time = 0;
		 // while (!boost::filesystem::exists(heg_tif_path))
		 // {
		 // 	++sleep_time;
		 // 	Sleep(1000);
		 // 	if (sleep_time == MAX_SLEEP)
		 // 	{
		 // 		BOOST_LOG_TRIVIAL(error) << "调用Heg提取.tif文件失败";
		 // 		break;
		 // 	}
		 // }
		if (!boost::filesystem::exists(heg_tif_path))
		{
			BOOST_LOG_TRIVIAL(error) << "未找到提取的.tif文件：" << heg_tif_path;
			BOOST_LOG_TRIVIAL(error) << "跳过" << hdf_file_path << "文件的处理";
			continue;
		}
		BOOST_LOG_TRIVIAL(debug) << "调用Heg提取.tif文件成功，提取的文件为：" << heg_tif_path;

		//经过GDAL处理的Heg提取的.tif文件路径
		const std::string heg_gdal_tif_path = temp_dir + boost::filesystem::path(hdf_file_path).stem().string() + "_heg_gdal.tif";
		const std::string gdal_argument = str(boost::format("-ot Float32 -projwin %1% %2% %3% %4% -projwin_srs EPSG:4326 -of GTiff -co \"COMPRESS = LZW\" -co \"INTERLEAVE = BAND\"")
			% min_lon % max_lat % max_lon % min_lat);
		modis_api::Gdal_operation::translate_copy(heg_tif_path, heg_gdal_tif_path, gdal_argument);
		if (!boost::filesystem::exists(heg_gdal_tif_path))
		{
			BOOST_LOG_TRIVIAL(error) << "调用gdal_translate.exe处理" << heg_tif_path << "文件失败，跳过" << hdf_file_path << "文件的处理";
			BOOST_LOG_TRIVIAL(error) << "gdal_argument: " << gdal_argument;
			continue;
		}
		BOOST_LOG_TRIVIAL(debug) << "调用gdal_translate.exe处理" << heg_tif_path << "文件成功，结果为：" << heg_gdal_tif_path;

		//经过GDAL处理的Scale后的Heg提取的.tif文件的路径
		const std::string heg_gdal_scaled_tif_path = temp_dir + boost::filesystem::path(hdf_file_path).stem().string() + "_heg_gdal_scaled.tif";
		boost::filesystem::copy_file(heg_gdal_tif_path, heg_gdal_scaled_tif_path);
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
			if (dn < 0 ) return 0;
			return (dn - OFFSET) * SCALE;
		});

		BOOST_LOG_TRIVIAL(debug) << "执行Scale操作后：";
		BOOST_LOG_TRIVIAL(debug) << modis_api::Mat_operation::mat_desc(*mat_optional);

		modis_api::Gdal_operation::write_fmat_to_tif(heg_gdal_scaled_tif_path, *mat_optional);
		BOOST_LOG_TRIVIAL(debug) << "Scale操作完成，处理结果文件为：" << heg_gdal_scaled_tif_path;

		preprocessed_file_paths.push_back(heg_gdal_scaled_tif_path);
		BOOST_LOG_TRIVIAL(info) << hdf_file_path << "文件预处理完成";
	}

	BOOST_LOG_TRIVIAL(info) << "合成处理，共需合成" << preprocessed_file_paths.size() << "个.tif文件";
	for (const std::string& p : preprocessed_file_paths)
		BOOST_LOG_TRIVIAL(debug) << "\t" << p;
	std::vector<arma::fmat> mat_list;
	std::transform(preprocessed_file_paths.cbegin(), preprocessed_file_paths.cend(), std::back_inserter(mat_list),
		[](const std::string& p) { return *modis_api::Gdal_operation::read_tif_to_fmat(p);  });
	auto mean_mat_optional = modis_api::Mat_operation::mean_mat_by_each_pixel(mat_list,0);
	if (!mean_mat_optional)
	{
		BOOST_LOG_TRIVIAL(error) << "矩阵合成出现错误";
		return;
	}

	BOOST_LOG_TRIVIAL(debug) << "准备输出最终结果文件：" << output_image_file;
	if (boost::filesystem::exists(output_image_file))
	{
		boost::filesystem::remove(output_image_file);
		BOOST_LOG_TRIVIAL(debug) << "最终结果文件：" << output_image_file << "已存在，删除之";
	}
	boost::filesystem::path p(output_image_file);
	if (!exists(p.parent_path()))
	{
		create_directories(p.parent_path());
		BOOST_LOG_TRIVIAL(debug) << "创建目录：" << p.parent_path().string();
	}

	boost::filesystem::copy_file(preprocessed_file_paths[0], output_image_file);
	BOOST_LOG_TRIVIAL(debug) << "复制文件：" << preprocessed_file_paths[0] << " -> " << output_image_file;

	modis_api::Gdal_operation::write_fmat_to_tif(output_image_file, *mean_mat_optional);
	BOOST_LOG_TRIVIAL(debug) << "成功向最终结果文件" << output_image_file << "写入DN值";

	BOOST_LOG_TRIVIAL(info) << "预处理完成，最终结果文件为：" << output_image_file;
	BOOST_LOG_TRIVIAL(info) << "";
	// 非debug模式下，程序运行结束清空temp目录
	if (!debug_mode)
	{
		modis_api::File_operation::clear_directory(temp_dir);
		BOOST_LOG_TRIVIAL(info) << "Temp目录" << temp_dir << "已清空..";
	}

}

