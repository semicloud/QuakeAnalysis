// gdal_lib.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "gdal_lib.h"

int gdal_lib::create_tif(std::string const& path, xt::xtensor<float, 3> const& data, int ndv)
{
	GDALAllRegister();
	std::unique_ptr<GDALDriver> driver{ GetGDALDriverManager()->GetDriverByName("GTiff") };
	char **papszOptions = nullptr;
	//papszOptions = CSLSetNameValue(papszOptions, "TILED","YES");
	//papszOptions = CSLSetNameValue(papszOptions, "INTERLEAVE", "BAND");
	//papszOptions = CSLSetNameValue(papszOptions, "NODATAVALUE", std::to_string(ndv).c_str());
	papszOptions = CSLSetNameValue(papszOptions, "COMPRESS", "LZW");
	papszOptions = CSLSetNameValue(papszOptions, "PHOTOMETRIC", "RGB");
	const int x_size = data.shape(2);
	const int y_size = data.shape(1);
	const int band_num = data.shape(0);
	xt::xtensor<float, 3> d{ data };
	GDALDataset* ds = driver->Create(path.c_str(), x_size, y_size, band_num, GDT_Float32, papszOptions);
	ds->RasterIO(GF_Write, 0, 0, x_size, y_size, d.data(), x_size, y_size, GDT_Float32, 3, 0, 0, 0, 0);
	ds->GetRasterBand(1)->SetColorInterpretation(GCI_GrayIndex);
	ds->GetRasterBand(2)->SetColorInterpretation(GCI_GrayIndex);
	ds->GetRasterBand(3)->SetColorInterpretation(GCI_GrayIndex);
	GDALClose(ds);
	return 0;
}