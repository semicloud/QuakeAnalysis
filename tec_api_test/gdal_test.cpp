#include "pch.h"
#include "../gdal_lib/gdal_lib.h"
#include <boost/test/unit_test.hpp>
#include <gdal.h>
#include <gdal_priv.h>

BOOST_AUTO_TEST_CASE(save_tif_test)
{
	xt::xtensor<float, 3> data = xt::random::randn<float>({ 3,100,100 });
	gdal_lib::create_tif("d:\\my.tif", data, -1);
	BOOST_TEST(boost::filesystem::exists("d:\\my.tif"));
}