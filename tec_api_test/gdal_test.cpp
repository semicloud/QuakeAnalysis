#include "pch.h"
#include "../gdal_lib/gdal_lib.h"
#include <boost/test/unit_test.hpp>
#include <gdal.h>
#include <gdal_priv.h>

BOOST_AUTO_TEST_CASE(create_one_band_tif_test)
{
	const std::string file{ "d:\\aa.tif" };
	const size_t no_data_value = 0;
	xt::xtensor<unsigned, 2> data{ {1,2,3},{4,5,6} };
	const size_t xs = data.shape(1); // x_size为列数
	const size_t ys = data.shape(0); // y_size为行数
	gdal_lib::create_tif<unsigned>(file, data.data(), no_data_value,
		xs, ys, 1, gdal_lib::wgs84_proj4(), gdal_lib::default_geo_trans().get(), gdal_lib::tif_options_for_grey());
	BOOST_TEST(boost::filesystem::exists(file));
	BOOST_TEST(gdal_lib::read_ndv(file) == 0);
}

BOOST_AUTO_TEST_CASE(create_multiple_band_tif_test)
{
	const std::string file{ "d:\\multiple_bands.tif" };
	const double no_data_value = 0;
	xt::xtensor<unsigned, 3> d = xt::cast<unsigned>(xt::arange<int>(24).reshape({ 2,3,4 }));
	const size_t xs = d.shape(2);
	const size_t ys = d.shape(1);
	const size_t bn = d.shape(0);
	gdal_lib::create_tif<unsigned>(file, d.data(), 0, xs, ys, bn, gdal_lib::wgs84_proj4(),
		gdal_lib::default_geo_trans().get(), gdal_lib::tif_options_for_rgb());
}

BOOST_AUTO_TEST_CASE(gdal_datatype_test)
{
	BOOST_TEST(typeid(int).name() == "int");
	BOOST_TEST(typeid(unsigned).name() == "unsigned int");
	BOOST_TEST(typeid(float).name() == "float");
	BOOST_TEST(typeid(double).name() == "double");
}

BOOST_AUTO_TEST_CASE(str2int_test)
{
	BOOST_TEST(gdal_lib::str2int("int") == 0);
	BOOST_TEST(gdal_lib::str2int("unsigned int") == 1);
	BOOST_TEST(gdal_lib::str2int("float") == 2);
	BOOST_TEST(gdal_lib::str2int("xx") == -1);
}

BOOST_AUTO_TEST_CASE(get_gdal_data_type_test)
{
	BOOST_TEST(gdal_lib::get_gdal_data_type<unsigned>() == GDT_UInt32);
	BOOST_TEST(gdal_lib::get_gdal_data_type<float>() == GDT_Float32);
	BOOST_TEST(gdal_lib::get_gdal_data_type<double>() == GDT_Unknown);
}

BOOST_AUTO_TEST_CASE(read_tif_test_one_band)
{
	std::vector<float> data;
	size_t xs = 0;
	size_t ys = 0;
	size_t bn = 0;
	double ndv = 0;
	int i = gdal_lib::read_tif("D:\\aa.tif", data, xs, ys, bn, ndv);
	BOOST_TEST(i == 0);
	BOOST_TEST(xs == 3);
	BOOST_TEST(ys == 2);
	BOOST_TEST(ndv == 0.0);

	std::vector<std::string> sdata;
	std::transform(data.begin(), data.end(), std::back_inserter(sdata),
		[](auto d) {return std::to_string(d); });
	BOOST_TEST_MESSAGE(boost::join(sdata, " "));
}

BOOST_AUTO_TEST_CASE(read_tif_test_multiple_band)
{
	std::vector<float> data;
	size_t xs = 0;
	size_t ys = 0;
	size_t bn = 0;
	double ndv = 0;
	int i = gdal_lib::read_tif("D:\\multiple_bands.tif", data, xs, ys, bn, ndv);
	BOOST_TEST(i == 0);
	BOOST_TEST(xs == 4);
	BOOST_TEST(ys == 3);
	BOOST_TEST(bn == 2);
	BOOST_TEST(ndv == 0.0);

	std::vector<std::string> sdata;
	std::transform(data.begin(), data.end(), std::back_inserter(sdata),
		[](auto d) {return std::to_string(d); });
	BOOST_TEST_MESSAGE(boost::join(sdata, " "));
}

BOOST_AUTO_TEST_CASE(get_proj_test)
{
	std::string wkt = gdal_lib::read_proj("d:\\aa.tif");
	BOOST_TEST_MESSAGE(wkt);
	BOOST_CHECK_EQUAL(boost::trim_copy(gdal_lib::wkt_to_proj4(wkt)), gdal_lib::wgs84_proj4());
}

BOOST_AUTO_TEST_CASE(wkt_to_proj4_test)
{
	std::string wkt = "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433],AUTHORITY[\"EPSG\",\"4326\"]]";
	BOOST_CHECK_EQUAL(boost::trim_copy(gdal_lib::wkt_to_proj4(wkt)), gdal_lib::wgs84_proj4());
}

BOOST_AUTO_TEST_CASE(read_ndv_test)
{
	BOOST_CHECK_EQUAL(gdal_lib::read_ndv("D:\\aa.tif"), 0.0);
}

BOOST_AUTO_TEST_CASE(set_ndv_test)
{
	double ndv = 9999;
	std::string file = "D:\\aa.tif";
	gdal_lib::set_ndv(file, ndv);
	BOOST_CHECK_EQUAL(gdal_lib::read_ndv(file), ndv);
	gdal_lib::set_ndv(file, 0);
	BOOST_CHECK_EQUAL(gdal_lib::read_ndv(file), 0);
}

BOOST_AUTO_TEST_CASE(read_metadata_test)
{
	const std::string expected{ "Area" };
	const std::string actual =
		gdal_lib::read_metadata("d:\\aa.tif", "AREA_OR_POINT");
	BOOST_TEST(expected == actual);
}