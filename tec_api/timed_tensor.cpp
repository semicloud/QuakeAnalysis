#include "stdafx.h"
#include "timed_tensor.h"


boost::posix_time::ptime tec_api::parse_time(boost::filesystem::path const& file_path)
{
	const std::string hour = file_path.filename().stem().string();
	const std::string day = file_path.parent_path().stem().string();
	const std::string month = file_path.parent_path().parent_path().stem().string();
	const std::string year = file_path.parent_path().parent_path().parent_path().stem().string();
	const std::string tstr = (boost::format("%1%-%2%-%3% %4%:00:00") % year % month % day % hour).str();
	const boost::posix_time::ptime time(boost::posix_time::time_from_string(tstr));
	return time;
}

xt::xtensor<double, 2> tec_api::load_tif(boost::filesystem::path const& p)
{
	GDALAllRegister();
	std::unique_ptr<GDALDataset> ds{ static_cast<GDALDataset*>(GDALOpenShared(p.string().data(), GA_ReadOnly)) };
	if (!ds) throw std::exception("read tif file to xt::xtensor error!");
	std::unique_ptr<GDALRasterBand> band{ static_cast<GDALRasterBand*>(ds->GetRasterBand(1)) };
	if (!band) throw std::exception("read tif band to xt::xtensor error!");
	const int x_size = band->GetXSize();
	const int y_size = band->GetYSize();
	float* data = static_cast<float*>(CPLMalloc(sizeof(float) * x_size * y_size));
	band->RasterIO(GF_Read, 0, 0, x_size, y_size, data, x_size, y_size, GDT_Float32, 0, 0);
	const size_t n_col = static_cast<size_t>(x_size);
	const size_t n_row = static_cast<size_t>(y_size);
	const std::vector<size_t> shape{ n_row,n_col };
	xt::xtensor<double, 2> ans{ xt::adapt(data, n_col * n_row, xt::no_ownership() ,shape) };
	CPLFree(data);
	band.release();  // bandָ�벻��������������dataset����ʱ���Զ�����band����
	return ans;
}