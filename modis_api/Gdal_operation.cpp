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
#include <gdal_utils.h>
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
		BOOST_LOG_TRIVIAL(error) << "ʹ��GDAL��" << fn << "�ļ�ʧ�ܣ�";
		return false;
	}

	double geo_trans[6];
	gdal_dataset->GetGeoTransform(geo_trans);
	out_proj = gdal_dataset->GetProjectionRef();
	std::copy(&geo_trans[0], &geo_trans[6], std::back_inserter(out_geo_trans));

	std::vector<std::string> svec(6);
	std::transform(out_geo_trans.begin(), out_geo_trans.end(), svec.begin(),
		[](const double& f) { return std::to_string(f);  });

	BOOST_LOG_TRIVIAL(debug) << "��ȡ" << fn << "�ļ���GEO_TRANS���ԣ�" <<
		boost::algorithm::join(svec, ",");
	BOOST_LOG_TRIVIAL(debug) << "��ȡ" << fn << "�ļ���PROJ_REF���ԣ�" <<
		out_proj;

	GDALClose(gdal_dataset);
	return true;
}

boost::optional<arma::fmat> modis_api::Gdal_operation::read_tif_to_fmat(const std::string& tif_path)
{
#pragma region ��������

	if (!std::filesystem::exists(tif_path) || !std::filesystem::is_regular_file(tif_path))
	{
		BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("�Ƿ���tif�ļ���%1%����ȡʧ�ܣ�") % tif_path);
		return boost::optional<arma::fmat>();
	}

	//GDALAllRegister();
	//ֻע��gtiff��ʽ�Ϳ��ԣ�����ע�������ģ����Ч��
	GDALRegister_GTiff();

	GDALDataset* data_set = static_cast<GDALDataset*>(GDALOpenShared(tif_path.data(), GA_ReadOnly));
	if (!data_set)
	{
		BOOST_LOG_TRIVIAL(error) << boost::str(boost::format("��%1%�ļ�ʧ�ܣ�") % tif_path);
		return boost::optional<arma::fmat>();
	}

# pragma endregion 

	auto band = static_cast<GDALRasterBand*>(data_set->GetRasterBand(1));
	// x_size��������y_size������
	const int x_size = band->GetXSize();
	const int y_size = band->GetYSize();
	// tif�ļ�������������float����ȡ��ʱ��ʹ�õ��������ͱ�����float��������double
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

	//ARMADILLO���е�Сbug�����밴���С��У�Ȼ����ת�õĴ������ɾ������
	arma::Mat<float> f_mat(band_data, x_size, y_size);
	f_mat = f_mat.t();

	// see: https://www.gdal.org/gdal_tutorial.html Reading Raster Data
	CPLFree(band_data);
	GDALClose(data_set);
	BOOST_LOG_TRIVIAL(debug) << str(boost::format("load %1%��Row=%2%��Col=%3%") % tif_path % y_size % x_size);

	return boost::optional<arma::fmat>(f_mat);
}

bool modis_api::Gdal_operation::write_fmat_to_tif(const std::string& tif_path, arma::fmat& mat)
{
	BOOST_LOG_TRIVIAL(debug) << "׼����" << tif_path << "�ļ�д��DNֵ..";
	//BOOST_LOG_TRIVIAL(info) << "in write fmat to tif:" << mat.row(0);

#pragma region ��������

	if (!std::filesystem::exists(tif_path) || !std::filesystem::is_regular_file(tif_path))
	{
		BOOST_LOG_TRIVIAL(error) << "Ŀ��tif�ļ�" << tif_path << "�����ڣ����ʽ����";
		return false;
	}

	GDALRegister_GTiff();

	GDALDataset* data_set = static_cast<GDALDataset*>(GDALOpenShared(tif_path.c_str(), GA_Update));
	if (data_set == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "�޷����ļ�" << tif_path;
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
	BOOST_LOG_TRIVIAL(debug) << "�ɹ�������" << tif_path << "�ļ�д��DNֵ";
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

bool modis_api::Gdal_operation::set_no_data_value(const std::filesystem::path& source_path, float no_data_value)
{
	const std::string cmd = str(boost::format("gdal_edit.py -a_nodata %1% %2%") % no_data_value % source_path);
	BOOST_LOG_TRIVIAL(debug) << "GDAL cmd: " << cmd;
	return system(cmd.c_str()) == 0;
}

float modis_api::Gdal_operation::get_no_data_value(const std::string& source_path)
{
	if (!std::filesystem::exists(source_path) || !std::filesystem::is_regular_file(source_path))
	{
		BOOST_LOG_TRIVIAL(error) << "Ŀ��tif�ļ�" << source_path << "�����ڣ����ʽ����";
		return false;
	}

	GDALRegister_GTiff();

	GDALDataset* data_set = static_cast<GDALDataset*>(GDALOpenShared(source_path.c_str(), GA_ReadOnly));
	if (data_set == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "�޷����ļ�" << source_path;
		return false;
	}

	GDALRasterBand* band = data_set->GetRasterBand(1);
	const 	float no_data_value = static_cast<float>(band->GetNoDataValue());
	BOOST_LOG_TRIVIAL(debug) << "�ļ�" << source_path << "��NO_DATA_VALUEΪ" << no_data_value;
	return no_data_value;
}


boost::optional<arma::fmat> modis_api::Gdal_operation::read_radiance_scales_and_offsets(const std::filesystem::path& hdf_path)
{
	BOOST_LOG_TRIVIAL(info) << "��" << hdf_path << "�ļ���ȡ�ֶ�ֵ���ֶ���Ϊradiance_scales��radiance_offsets";
	if (!std::filesystem::exists(hdf_path) || !std::filesystem::is_regular_file(hdf_path))
	{
		BOOST_LOG_TRIVIAL(error) << "HDF�ļ�" << hdf_path << "�����ڻ��ǹ淶�ļ����޷���ȡ����ֶ�ֵ";
		return boost::optional<arma::fmat>();
	}

	GDALAllRegister();

	std::string sds_path = "HDF4_EOS:EOS_SWATH:\"" + hdf_path.string() + "\":MODIS_SWATH_Type_L1B:EV_1KM_Emissive";
	BOOST_LOG_TRIVIAL(debug) << "sds_path: " << sds_path;

	GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(sds_path.data(), GA_ReadOnly));
	if (!dataset)
	{
		BOOST_LOG_TRIVIAL(error) << "ʹ��GDAL�����ݼ�" << sds_path << "ʧ�ܣ��޷���ȡ����ֶ�ֵ";
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

	BOOST_LOG_TRIVIAL(info) << "��ȡ����������ȡ" << ans_mat.size() << "����ֵ";
	GDALClose(dataset);

	return boost::optional<arma::fmat>(ans_mat);
}

bool modis_api::Gdal_operation::gdal_translate(
	const std::string& src, const std::string& dst,
	const std::vector<std::string>& opts)
{
	GDALAllRegister();

	char** params = nullptr;
	for (const std::string& option : opts)
		params = CSLAddString(params, option.c_str());
	GDALTranslateOptions* options = GDALTranslateOptionsNew(params, nullptr);

	int bUsgError = FALSE;
	GDALDatasetH src_handle = GDALOpen(src.c_str(), GA_ReadOnly);
	GDALDataset* dst_dataset = (GDALDataset*)GDALTranslate(dst.c_str(), src_handle, options, &bUsgError);
	GDALClose(dst_dataset);
	GDALClose(src_handle);
	GDALTranslateOptionsFree(options);
	CSLDestroy(params);
	assert(bUsgError == FALSE);
	return std::filesystem::exists(dst);
}

bool modis_api::Gdal_operation::read_geo_bound(std::filesystem::path const& hdfPath,
	double& ulx, double& uly, double& lrx, double& lry)
{
	// get mod04 or mod05
	const std::string type_num_str = boost::to_lower_copy(hdfPath.filename().string().substr(3, 2));
	assert(type_num_str == "04" || type_num_str == "05");
	// mod and myd -> mod
	const std::string type_code = (boost::format("mod%1%") % type_num_str).str();
	const std::string lng_ds_path = (boost::format("HDF4_EOS:EOS_SWATH_GEOL:%1%:mod%2%:Longitude") % hdfPath % type_num_str).str();
	const std::string lat_ds_path = (boost::format("HDF4_EOS:EOS_SWATH_GEOL:%1%:mod%2%:Latitude") % hdfPath % type_num_str).str();
	BOOST_LOG_TRIVIAL(debug) << "lng_ds:" << lng_ds_path;
	BOOST_LOG_TRIVIAL(debug) << "lat_ds:" << lat_ds_path;
	double min_lng = 0, max_lng = 0, min_lat = 0, max_lat = 0;
	const bool readLng = read_min_max_loc(lng_ds_path, min_lng, max_lng);
	const bool readLat = read_min_max_loc(lat_ds_path, min_lat, max_lat);
	bool ans = readLng && readLat;
	if (ans)
	{
		ulx = max_lat;
		uly = min_lng;
		lrx = min_lat;
		lry = max_lng;
	}
	return ans;
}

bool modis_api::Gdal_operation::read_min_max_loc(std::string const& ds, double& min, double& max)
{
	GDALAllRegister();

	GDALDataset* pDataset = static_cast<GDALDataset*>(GDALOpen(ds.c_str(), GA_ReadOnly));
	if (!pDataset)
	{
		GDALClose(pDataset);
		return false;
	}
	GDALRasterBand* pBand(pDataset->GetRasterBand(1));
	if (!pBand) return false;
	const int x_size = pBand->GetXSize();
	const int y_size = pBand->GetYSize();
	float* pData = static_cast<float*>(CPLMalloc(sizeof(float)* x_size * y_size));
	pBand->RasterIO(GF_Read, 0, 0, x_size, y_size, pData, x_size, y_size, GDT_Float32, 0, 0);
	min = *std::min_element(pData, pData + x_size * y_size);
	max = *std::max_element(pData, pData + x_size * y_size);
	CPLFree(pData);
	GDALClose(pDataset);
	return true;
}
