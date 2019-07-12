#include "stdafx.h"

#include "Gdal_operation.h"
#include <armadillo>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <gdal_priv.h>
#include <filesystem>

modis_api::Gdal_operation::Gdal_operation()
{
}

modis_api::Gdal_operation::~Gdal_operation()
{
}

bool modis_api::Gdal_operation::create_tif(const std::string& fn,
	double* geo_trans, const std::string& proj, arma::fmat mat)
{
	GDALAllRegister();
	const char* pszFormat = "GTiff";
	GDALDriver* driver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	if (driver == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "GDAL does't support data format " << pszFormat;
		return false;
	}
	char **papszOptions = nullptr;
	papszOptions = CSLSetNameValue(papszOptions, "INTERLEAVE", "BAND");
	papszOptions = CSLSetNameValue(papszOptions, "COMPRESS", "LZW");

	const int nrows = static_cast<int>(mat.n_rows);
	const int ncols = static_cast<int>(mat.n_cols);

	GDALDataset* po_dataset = driver->Create(fn.c_str(), ncols, nrows,
		1, GDT_Float32, papszOptions);

	if (po_dataset == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "GDAL dataset cannot be created!";
		return false;
	}

	if (proj.length() > 0)
	{
		po_dataset->SetProjection(proj.c_str());
		po_dataset->SetGeoTransform(geo_trans);
	}

	GDALClose(static_cast<GDALDatasetH>(po_dataset));

	return write_fmat_to_tif(fn, mat);
}

bool modis_api::Gdal_operation::get_geo_trans_and_proj(const std::string& fn,
	std::vector<double>& out_geo_trans, std::string& out_proj)
{
	GDALRegister_GTiff();
	GDALDataset* gdal_dataset = static_cast<GDALDataset*>
		(GDALOpenShared(fn.c_str(), GA_ReadOnly));
	if (gdal_dataset == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "使用GDAL打开" << fn << "文件失败！";
		return false;
	}

	double geo_trans[6];
	gdal_dataset->GetGeoTransform(geo_trans);
	out_proj = gdal_dataset->GetProjectionRef();
	std::copy(&geo_trans[0], &geo_trans[6], std::back_inserter(out_geo_trans));

	std::vector<std::string> svec(6);
	std::transform(out_geo_trans.begin(), out_geo_trans.end(), svec.begin(),
		[](const double& f) { return std::to_string(f);  });

	BOOST_LOG_TRIVIAL(debug) << "读取" << fn << "文件的GEO_TRANS属性：" <<
		boost::algorithm::join(svec, ",");
	BOOST_LOG_TRIVIAL(debug) << "读取" << fn << "文件的PROJ_REF属性：" <<
		out_proj;

	GDALClose(gdal_dataset);
	return true;
}

boost::optional<arma::fmat> modis_api::Gdal_operation::read_tif_to_fmat(const std::string& tif_path)
{
#pragma region 防御代码

	if (!std::filesystem::exists(tif_path) || !std::filesystem::is_regular_file(tif_path))
	{
		BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("非法的tif文件：%1%，读取失败！") % tif_path);
		return boost::optional<arma::fmat>();
	}

	//GDALAllRegister();
	//只注册gtiff格式就可以，不用注册其他的，提高效率
	GDALRegister_GTiff();

	GDALDataset* data_set = static_cast<GDALDataset*>(GDALOpenShared(tif_path.data(), GA_ReadOnly));
	if (!data_set)
	{
		BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("打开%1%文件失败！") % tif_path);
		return boost::optional<arma::fmat>();
	}

# pragma endregion 

	auto band = static_cast<GDALRasterBand*>(data_set->GetRasterBand(1));
	// x_size是列数，y_size是行数
	const int x_size = band->GetXSize();
	const int y_size = band->GetYSize();
	// tif文件的数据类型是float，读取的时候使用的数据类型必须是float，不能是double
	const auto band_data = static_cast<float*>(CPLMalloc(sizeof(float) * x_size * y_size));
	band->RasterIO(GF_Read, 0, 0, x_size, y_size, band_data, x_size, y_size, GDT_Float32, 0, 0);

	/*BOOST_LOG_TRIVIAL(info) << "tif_path: " << tif_path << ", Type: " << band->GetRasterDataType();
	if(band->GetRasterDataType() == GDT_Int16)
	{
		short* band_data_int16 = static_cast<short*>(CPLMalloc(sizeof(short) * x_size * y_size));
		band->RasterIO(GF_Read, 0, 0, x_size, y_size, band_data_int16, x_size, y_size, GDT_Int16, 0, 0);
		arma::Mat<short> s_sort(band_data_int16, y_size, x_size);
		arma::Mat<short> ss(band_data_int16, y_size, x_size);
		for(int i = 0 ; i<10 ; ++i)
		{
			BOOST_LOG_TRIVIAL(info) << band_data_int16[i];
		}
		BOOST_LOG_TRIVIAL(info) << "int16 row0: " << s_sort(0, arma::span(0, 9));
		BOOST_LOG_TRIVIAL(info) << "int16 col0: " << s_sort(arma::span(0, 9), 0);
	}*/

	//ARMADILLO含有的小bug，必须按照列、行，然后再转置的次序生成矩阵对象
	arma::Mat<float> f_mat(band_data, x_size, y_size);
	f_mat = f_mat.t();

	// see: https://www.gdal.org/gdal_tutorial.html Reading Raster Data
	CPLFree(band_data);
	GDALClose(data_set);
	BOOST_LOG_TRIVIAL(debug) << str(boost::format("load %1%，Row=%2%，Col=%3%") % tif_path % y_size % x_size);

	return boost::optional<arma::fmat>(f_mat);
}

bool modis_api::Gdal_operation::write_fmat_to_tif(const std::string& tif_path, arma::fmat& mat)
{
	BOOST_LOG_TRIVIAL(debug) << "准备向" << tif_path << "文件写入DN值..";
	//BOOST_LOG_TRIVIAL(info) << "in write fmat to tif:" << mat.row(0);

#pragma region 防御代码

	if (!std::filesystem::exists(tif_path) || !std::filesystem::is_regular_file(tif_path))
	{
		BOOST_LOG_TRIVIAL(error) << "目标tif文件" << tif_path << "不存在，或格式错误！";
		return false;
	}

	GDALRegister_GTiff();

	GDALDataset* data_set = static_cast<GDALDataset*>(GDALOpenShared(tif_path.c_str(), GA_Update));
	if (data_set == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "无法打开文件" << tif_path;
		return false;
	}

#pragma endregion 

	GDALRasterBand* band = data_set->GetRasterBand(1);
	const int x_size = band->GetXSize();
	const int y_size = band->GetYSize();
	arma::frowvec row_vec = vectorise(mat, 1);
	//BOOST_LOG_TRIVIAL(info) << "row_vec: " << row_vec(arma::span(0, 10));
	band->RasterIO(GF_Write, 0, 0, x_size, y_size, row_vec.memptr(), x_size, y_size, GDT_Float32, 0, 0);
	band->SetNoDataValue(0);
	GDALClose(data_set);
	BOOST_LOG_TRIVIAL(debug) << "成功！已向" << tif_path << "文件写入DN值";
	return true;
}

bool modis_api::Gdal_operation::translate_copy(const std::filesystem::path& source_path,
	const std::filesystem::path& dest_path, const std::string& param)
{
	const std::string cmd = std::filesystem::current_path().string()
		+ str(boost::format("\\gdal_translate.exe %1% %2% %3%") % param % source_path.string() % dest_path.string());
	BOOST_LOG_TRIVIAL(debug) << "GDAL cmd: " << cmd;
	return system(cmd.c_str()) == 0;
}

bool modis_api::Gdal_operation::set_no_data_value(const std::string& source_path, float no_data_value)
{
	const std::string cmd = str(boost::format("gdal_edit.py -a_nodata %1% %2%") % no_data_value % source_path);
	BOOST_LOG_TRIVIAL(debug) << "GDAL cmd: " << cmd;
	return system(cmd.c_str()) == 0;
}

float modis_api::Gdal_operation::get_no_data_value(const std::string& source_path)
{
	if (!std::filesystem::exists(source_path) || !std::filesystem::is_regular_file(source_path))
	{
		BOOST_LOG_TRIVIAL(error) << "目标tif文件" << source_path << "不存在，或格式错误！";
		return false;
	}

	GDALRegister_GTiff();

	GDALDataset* data_set = static_cast<GDALDataset*>(GDALOpenShared(source_path.c_str(), GA_ReadOnly));
	if (data_set == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "无法打开文件" << source_path;
		return false;
	}

	GDALRasterBand* band = data_set->GetRasterBand(1);
	const 	float no_data_value = static_cast<float>(band->GetNoDataValue());
	BOOST_LOG_TRIVIAL(debug) << "文件" << source_path << "的NO_DATA_VALUE为" << no_data_value;
	return no_data_value;
}


boost::optional<arma::fmat> modis_api::Gdal_operation::read_radiance_scales_and_offsets(const std::filesystem::path& hdf_path)
{
	BOOST_LOG_TRIVIAL(info) << "从" << hdf_path << "文件读取字段值，字段名为radiance_scales和radiance_offsets";
	if (!std::filesystem::exists(hdf_path) || !std::filesystem::is_regular_file(hdf_path))
	{
		BOOST_LOG_TRIVIAL(error) << "HDF文件" << hdf_path << "不存在或不是规范文件，无法读取相关字段值";
		return boost::optional<arma::fmat>();
	}

	GDALAllRegister();

	std::string sds_path = "HDF4_EOS:EOS_SWATH:\"" + hdf_path.string() + "\":MODIS_SWATH_Type_L1B:EV_1KM_Emissive";
	BOOST_LOG_TRIVIAL(debug) << "sds_path: " << sds_path;

	GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(sds_path.data(), GA_ReadOnly));
	if (!dataset)
	{
		BOOST_LOG_TRIVIAL(error) << "使用GDAL打开数据集" << sds_path << "失败，无法读取相关字段值";
		return boost::optional<arma::fmat>();
	}
	std::string scales = dataset->GetMetadataItem("radiance_scales", "");
	std::string offsets = dataset->GetMetadataItem("radiance_offsets", "");
	BOOST_LOG_TRIVIAL(debug) << "scales: " << scales;
	BOOST_LOG_TRIVIAL(debug) << "offsets: " << offsets;
	// build matrix
	std::stringstream ss;
	ss << scales << ";" << offsets;

	arma::fmat ans_mat(ss.str());
	//cout << ans_mat << endl;

	BOOST_LOG_TRIVIAL(info) << "读取结束，共读取" << ans_mat.size() << "个数值";
	GDALClose(dataset);

	return boost::optional<arma::fmat>(ans_mat);
}

bool modis_api::Gdal_operation::read_geo_bound(const std::string& hdf_path, const std::string& sds, double& ulx, double& uly, double& lrx,
	double& lry)
{
	if (!std::filesystem::exists(hdf_path) || !std::filesystem::is_regular_file(hdf_path))
	{
		BOOST_LOG_TRIVIAL(error) << "HDF文件" << hdf_path << "不存在或不是规范文件，无法读取相关字段值";
		return false;
	}

	GDALAllRegister();

	std::string sds_path = "HDF4_EOS:EOS_SWATH:\"" + hdf_path + sds;

	BOOST_LOG_TRIVIAL(debug) << "sds_path: " << sds_path;

	GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(sds_path.data(), GA_ReadOnly));
	if (!dataset)
	{
		BOOST_LOG_TRIVIAL(error) << "使用GDAL打开数据集" << sds_path << "失败，无法读取相关字段值";
		return false;
	}

	std::string west_str = dataset->GetMetadataItem("WESTBOUNDINGCOORDINATE");
	std::string east_str = dataset->GetMetadataItem("EASTBOUNDINGCOORDINATE");
	std::string south_str = dataset->GetMetadataItem("SOUTHBOUNDINGCOORDINATE");
	std::string north_str = dataset->GetMetadataItem("NORTHBOUNDINGCOORDINATE");
	BOOST_LOG_TRIVIAL(debug) << "从" << hdf_path << "读取数据集四至：";
	BOOST_LOG_TRIVIAL(debug) << "West: " << west_str;
	BOOST_LOG_TRIVIAL(debug) << "East: " << east_str;
	BOOST_LOG_TRIVIAL(debug) << "South:" << south_str;
	BOOST_LOG_TRIVIAL(debug) << "North:" << north_str;

	ulx = stod(north_str);
	uly = stod(west_str);
	lrx = stod(south_str);
	lry = stod(east_str);

	GDALClose(dataset);

	return true;
}

bool modis_api::Gdal_operation::read_geo_bound_py_h5(const std::string& hdf_path,
	const std::string& tmp_folder,
	double& ulx, double& uly, double& lrx, double& lry)
{
	using namespace boost;
	using namespace std;
	using namespace std::filesystem;
	bool ans = false;
	const path hdf_path_obj(hdf_path);
	const path tmp_folder_obj(tmp_folder);

	const string hdf_file_name = hdf_path_obj.filename().stem().string();
	BOOST_LOG_TRIVIAL(debug) << "hdf file name: " << hdf_file_name;

	const path h5_path_obj = tmp_folder_obj / str(format("%1%.%2%") % hdf_file_name % "h5");
	BOOST_LOG_TRIVIAL(debug) << "h5 file path:" << h5_path_obj;

	const string h4h5convert = str(format("h4toh5convert.exe %1% %2%")
		% hdf_path % h5_path_obj.string());
	BOOST_LOG_TRIVIAL(debug) << h4h5convert;
	std::system(h4h5convert.c_str());
	if (exists(h5_path_obj))
	{
		const std::string py_command = str(format("read_geo_bound.py %1% %2%") %
			h5_path_obj.string() % to_lower_copy(hdf_file_name.substr(0, 5)));
		BOOST_LOG_TRIVIAL(debug) << py_command;
		std::system(py_command.c_str());
		const path geo_txt_path_obj = tmp_folder_obj / str(format("%1%_geo.%2%") % hdf_file_name % "txt");
		BOOST_LOG_TRIVIAL(debug) << "geo txt path:" << geo_txt_path_obj.string();
		if (exists(geo_txt_path_obj))
		{
			std::string str;
			getline(ifstream(geo_txt_path_obj.string()), str);
			BOOST_LOG_TRIVIAL(debug) << "Geo bounds:" << str;
			vector<string> geo_bounds;
			split(geo_bounds, str, is_any_of(" "));
			BOOST_LOG_TRIVIAL(debug) << "Load geo bounds: " << join(geo_bounds, ", ");
			if (geo_bounds.size() != 4)
				return false;
			ulx = stod(geo_bounds[3]);
			uly = stod(geo_bounds[0]);
			lrx = stod(geo_bounds[2]);
			lry = stod(geo_bounds[1]);
			ans = true;
		}
	}
	return ans;
}