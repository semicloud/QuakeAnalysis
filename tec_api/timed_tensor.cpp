#include "stdafx.h"
#include "timed_tensor.h"
#include "../gdal_lib/gdal_lib.h"

boost::posix_time::ptime tec_api::parse_time(boost::filesystem::path const& file_path)
{
	const std::string filename = file_path.filename().stem().string();
	std::vector<std::string> strs;
	boost::split(strs, filename, boost::is_any_of("_"));
	const std::string year = strs.at(1);
	const std::string hour = strs.at(3);
	const long day_of_year = boost::lexical_cast<long>(strs.at(2));
	const boost::gregorian::date d{ boost::lexical_cast<unsigned short>(year) ,1,1 };
	const boost::gregorian::date d2 = d + boost::gregorian::days(day_of_year - 1);
	const std::string tstr = (boost::format("%1%-%2%-%3% %4%:00:00") % year % d2.month() % d2.day() % hour).str();
	const boost::posix_time::ptime time(boost::posix_time::time_from_string(tstr));
	return time;
}

xt::xtensor<float, 2> tec_api::load_tif(boost::filesystem::path const& p)
{
	std::vector<float> data;
	size_t xs = 0, ys = 0, bn = 0;
	double ndv = 0;
	gdal_lib::read_tif(p.string(), data, xs, ys, bn, ndv);
	const std::vector<size_t> shape = boost::assign::list_of(ys)(xs);
	xt::xtensor<float, 2> ans{ xt::adapt(data.data(), ys * xs, xt::no_ownership(),shape) };
	xt::filtration(ans, xt::equal(ans, ndv)) = std::nanf(0);
	return ans;
}
