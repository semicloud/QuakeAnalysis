// gdal_lib.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "gdal_lib.h"

std::string gdal_lib::proj4_to_wkt(std::string const& proj4str)
{
	char* wkt;
	OGRSpatialReference spatial;
	spatial.importFromProj4(proj4str.c_str());
	spatial.exportToWkt(&wkt);
	return wkt;
}

std::string gdal_lib::wkt_to_proj4(std::string const& wkt)
{
	char* p4str;
	OGRSpatialReference spatial;
	spatial.importFromWkt(wkt.c_str());
	spatial.exportToProj4(&p4str);
	return p4str;
}

int gdal_lib::read_tif(std::string const& path, std::vector<float>& data, size_t& xs, size_t& ys, size_t& bn, double& ndv)
{
	GDALAllRegister();
	GDALDataset* ds = (GDALDataset*)GDALOpenShared(path.c_str(), GA_ReadOnly);
	if (!ds) throw std::runtime_error("read tif file failed!");
	bn = ds->GetRasterCount();
	xs = ds->GetRasterXSize();
	ys = ds->GetRasterYSize();
	ndv = GDALGetRasterNoDataValue(ds->GetRasterBand(1), nullptr);
	GDALDataType dt = ds->GetRasterBand(1)->GetRasterDataType();
	float* arr = static_cast<float*>(CPLMalloc(sizeof(float) * xs * ys * bn));
	auto to_i = [](auto d) {return int(d); };
	ds->RasterIO(GF_Read, 0, 0, to_i(xs), to_i(ys), arr, to_i(xs), to_i(ys), GDT_Float32, to_i(bn), nullptr, 0, 0, 0, nullptr);
	data = std::vector<float>(arr, arr + xs * ys * bn);
	CPLFree(arr);
	GDALClose(ds);
	return 0;
}

double gdal_lib::read_ndv(std::string const& file)
{
	GDALRegister_GTiff();
	GDALDataset* ds = static_cast<GDALDataset*>(GDALOpenShared(file.c_str(), GA_ReadOnly));
	assert(ds != nullptr);
	assert(ds->GetRasterCount() > 0);
	const double ndv = GDALGetRasterNoDataValue(ds->GetRasterBand(1), nullptr);
	GDALClose(ds);
	return ndv;
}

int gdal_lib::set_ndv(std::string const& file, double ndv)
{
	GDALAllRegister();
	GDALDataset* ds = static_cast<GDALDataset*>(GDALOpenShared(file.c_str(), GA_Update));
	assert(ds != nullptr);
	assert(ds->GetRasterCount() > 0);
	for (int i = 1; i != ds->GetRasterCount() + 1; ++i)
	{
		ds->GetRasterBand(i)->SetNoDataValue(ndv);
	}
	
	GDALClose(ds);
	return 0;
}

std::string gdal_lib::read_proj(std::string const& file)
{
	GDALAllRegister();
	GDALDataset* ds = static_cast<GDALDataset*>(GDALOpenShared(file.c_str(), GA_ReadOnly));
	assert(ds != nullptr);
	std::string ans{ ds->GetProjectionRef() };
	GDALClose(ds);
	return ans;
}

std::string gdal_lib::read_metadata(std::string const& file, std::string const& mn)
{
	GDALAllRegister();
	GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpenShared(file.data(), GA_ReadOnly));
	std::stringstream errmsg;
	errmsg << "open dataset " << file << " failed";
	if (!dataset) throw std::runtime_error(errmsg.str());
	std::string ans{ dataset->GetMetadataItem(mn.data(), "") };
	GDALClose(dataset);
	return ans;
}
