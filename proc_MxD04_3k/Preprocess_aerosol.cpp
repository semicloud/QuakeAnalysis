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
const float NO_DATA_VALUE = -1; //经过GDAL裁剪后，NO_DATA_VALUE是否还是-9999

/**
 * \brief 预处理气溶胶数据
 * \param ymlPath .yml文件路径
 * \param node YAML::Node
 */
void proc_MxD04_3k::Preprocess_aerosol::preprocess(const std::string& ymlPath, const YAML::Node& node, bool useDebugMode)
{
	using namespace std;

	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "开始进行Aerosol预处理，使用的.yml文件为：" << ymlPath;

	Aerosol_param param = load_params(ymlPath);
	std::filesystem::path tmpFolder(node["TmpPath"].as<string>());
	prepare_tmp_folder(tmpFolder);

	std::vector<std::filesystem::path> preprocessed_tif_path_vec;
	for (std::filesystem::path const& hdf_file : param.hdf_files())
	{
		const string hdf_name_no_ext = hdf_file.stem().string();
		const std::string heg_tif_file_name = (boost::format("%1%_heg.%2%") % hdf_name_no_ext % "tif").str();
		const std::filesystem::path heg_tif_file = tmpFolder / heg_tif_file_name;  //Heg提取出的.tif文件路径
		const std::string gdal_tif_file_name = (boost::format("%1%_heg_gdal.tif") % hdf_name_no_ext).str();
		const std::filesystem::path tif_gdal_path = tmpFolder / gdal_tif_file_name;  //gdal处理的.tif文件路径
		const std::string tif_scale_name = (boost::format("%1%_heg_gdal_scaled.tif") % hdf_name_no_ext).str();
		const std::filesystem::path tif_scale_path = tmpFolder / tif_scale_name;  //scale后的.tif文件路径

		// ! x是纬度，y是经度
		// ! ulx：最大纬度
		// ! lrx：最小纬度
		// ! uly：最小经度
		// ! lry：最大经度
		double ulx = 0, uly = 0, lrx = 0, lry = 0;
		modis_api::Gdal_operation::read_geo_bound(hdf_file, ulx, uly, lrx, lry);
		modis_api::Heg_utils::run_heg(modis_api::Heg_param(hdf_file)
			.min_lon(uly).max_lon(lry).min_lat(lrx).max_lat(ulx)
			.output_file(heg_tif_file).temp_folder(tmpFolder));
		BOOST_LOG_TRIVIAL(debug) << "调用Heg提取.tif文件成功，提取的文件为：" << heg_tif_file;

		//经过GDAL处理的Heg提取的.tif文件路径
		modis_api::Gdal_operation::translate_copy(heg_tif_file, tif_gdal_path,
			get_gdal_argument(param.min_lon(), param.max_lon(), param.min_lat(), param.max_lat()));
		BOOST_LOG_TRIVIAL(debug) << "调用gdal_translate.exe处理" << heg_tif_file << "文件成功，结果为：" << tif_gdal_path;

		//经过GDAL处理的Scale后的Heg提取的.tif文件的路径
		std::filesystem::copy_file(tif_gdal_path, tif_scale_path);
		boost::optional<arma::fmat> oMat = modis_api::Gdal_operation::read_tif_to_fmat(tif_scale_path.string());
		if (!oMat)
		{
			BOOST_LOG_TRIVIAL(error) << "提取" << tif_scale_path << "文件数据失败，程序退出";
			exit(EXIT_FAILURE);
		}
		BOOST_LOG_TRIVIAL(debug) << "加载tif文件：" << tif_scale_path;
		BOOST_LOG_TRIVIAL(debug) << modis_api::Mat_operation::mat_desc(*oMat);
		scale(*oMat);
		BOOST_LOG_TRIVIAL(debug) << "执行Scale操作后：";
		BOOST_LOG_TRIVIAL(debug) << modis_api::Mat_operation::mat_desc(*oMat);

		modis_api::Gdal_operation::write_fmat_to_tif(tif_scale_path.string(), *oMat);
		BOOST_LOG_TRIVIAL(debug) << "Scale操作完成，处理结果文件为：" << tif_scale_path;

		modis_api::Gdal_operation::set_no_data_value(tif_scale_path, NO_DATA_VALUE);
		BOOST_LOG_TRIVIAL(debug) << "设置" << tif_scale_path << "的NODATAVALUE为" << NO_DATA_VALUE;

		preprocessed_tif_path_vec.push_back(tif_scale_path);
		BOOST_LOG_TRIVIAL(info) << hdf_file << "文件预处理完成";
	}

	BOOST_LOG_TRIVIAL(info) << "合成处理，共需合成" << preprocessed_tif_path_vec.size() << "个.tif文件";
	std::vector<arma::fmat> matVec = readToMat(preprocessed_tif_path_vec);

	std::optional<arma::fmat> oMeanMat = modis_api::Mat_operation::mean_mat(matVec, NO_DATA_VALUE);
	if (!oMeanMat)
	{
		BOOST_LOG_TRIVIAL(error) << "矩阵合成出现错误";
		return;
	}

	BOOST_LOG_TRIVIAL(debug) << "准备输出最终结果文件：" << param.output_image_file();
	if (std::filesystem::exists(param.output_image_file()))
	{
		std::filesystem::remove(param.output_image_file());
		BOOST_LOG_TRIVIAL(debug) << "最终结果文件：" << param.output_image_file() << "已存在，删除之";
	}

	if (param.output_image_file().has_parent_path() && !std::filesystem::exists(param.output_image_file().parent_path()))
	{
		std::filesystem::create_directories(param.output_image_file().parent_path());
		BOOST_LOG_TRIVIAL(debug) << "创建目录：" << param.output_image_file().parent_path().string();
	}

	std::filesystem::copy_file(preprocessed_tif_path_vec[0], param.output_image_file());
	BOOST_LOG_TRIVIAL(debug) << "复制文件：" << preprocessed_tif_path_vec[0] << " -> " << param.output_image_file();

	modis_api::Gdal_operation::write_fmat_to_tif(param.output_image_file().string(), *oMeanMat);
	BOOST_LOG_TRIVIAL(debug) << "成功向最终结果文件" << param.output_image_file() << "写入DN值";

	modis_api::Gdal_operation::set_no_data_value(param.output_image_file(), NO_DATA_VALUE);
	BOOST_LOG_TRIVIAL(debug) << "设置" << param.output_image_file() << "的NODATAVALUE为" << NO_DATA_VALUE;

	BOOST_LOG_TRIVIAL(info) << "预处理完成，最终结果文件为：" << param.output_image_file();
	BOOST_LOG_TRIVIAL(info) << "";
	// 非debug模式下，程序运行结束清空temp目录
	if (!useDebugMode)
	{
		modis_api::File_operation::clear_directory(tmpFolder.string());
		BOOST_LOG_TRIVIAL(info) << "Temp目录" << tmpFolder << "已清空..";
	}
}

int proc_MxD04_3k::Preprocess_aerosol::prepare_tmp_folder(std::filesystem::path const& tmpFolder)
{
	if (!std::filesystem::exists(tmpFolder))
	{
		std::filesystem::create_directories(tmpFolder);
		BOOST_LOG_TRIVIAL(debug) << "创建Temp目录：" << tmpFolder;
	}
	modis_api::File_operation::clear_directory(tmpFolder.string());
	BOOST_LOG_TRIVIAL(info) << "Temp目录：" << tmpFolder << "准备完毕";
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
			BOOST_LOG_TRIVIAL(error) << "读取" << tifPath << "文件失败，程序退出";
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
