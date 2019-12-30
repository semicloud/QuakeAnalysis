#include "pch.h"
#include "../tec_api/tec_ano.h"
#include "../gdal_lib/gdal_lib.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(window_indexes_test)
{
	//size_t n = 7;
	//size_t w = 3;
	//xt::xarray<size_t> t1 = xt::arange<int>(0, int(n - w + 1));
	//xt::xarray<size_t> t2 = xt::arange<int>(int(w - 1), int(n));
	//BOOST_TEST_MESSAGE(t1);
	//BOOST_TEST_MESSAGE(t2);

	//auto x = xt::transpose(xt::stack(xt::xtuple(t1, t2), 0));
	//BOOST_TEST_MESSAGE(x);

	xt::xtensor<size_t, 2> actual = tec_api::window_indexes(7, 3);
	xt::xtensor<size_t, 2> expected{
	   {0,2},{1,3},{2,4},{3,5},{4,6}
	};
	BOOST_TEST(actual.shape(0) == 5);
	BOOST_TEST(actual.shape(1) == 2);
	BOOST_TEST(actual.dimension() == 2);
	BOOST_TEST(actual == expected, "Actual: " << actual);
}

BOOST_AUTO_TEST_CASE(ano_slid_win_test)
{
	boost::filesystem::path dir{ "D:\\tec\\Standard\\Codg\\2017\\195" };
	std::vector<boost::filesystem::path> paths;
	const auto all_files = boost::make_iterator_range(boost::filesystem::directory_iterator(dir));
	std::copy_if(all_files.begin(), all_files.end(), std::back_inserter(paths),
		[](boost::filesystem::directory_entry const& e) { return is_regular_file(e.path()) && e.path().extension() == ".tif"; });
	tec_api::timed_tensor_series<double, 2> series = tec_api::timed_tensor_series<>::load_from_files(paths);
	tec_api::timed_tensor_series<double, 3> ano = ano_slid_win(series, 3);

	const size_t xs = ano[0].tensor_ptr()->shape(2);
	const size_t ys = ano[0].tensor_ptr()->shape(1);
	const size_t band_num = ano[0].tensor_ptr()->shape(0);
	xt::xtensor<float, 3> d = xt::cast<float>(*ano[0].tensor_ptr());
	int i = gdal_lib::create_tif<float>("d:\\ano.tif", d.data(), 0, xs, ys, band_num, gdal_lib::get_wgs84_proj(), gdal_lib::get_default_geo_trans().get(), gdal_lib::tif_options_for_rgb());
	BOOST_TEST(i == 0);
}

BOOST_AUTO_TEST_CASE(list_all_files_test)
{
	const boost::filesystem::path dir{ "D:\\tec\\Standard\\Codg\\2017\\196" };
	const std::string ext = ".tif";
	auto all_files = tec_api::list_all_files(dir, ext);
	BOOST_TEST(all_files.size() == 24);
}

BOOST_AUTO_TEST_CASE(sort_files_by_time_asc_test)
{
	std::vector<boost::filesystem::path> files;
	files.push_back("D:\\tec\\Standard\\Codg\\2017\\196\\CODG_2017_196_07.tif");
	files.push_back("D:\\tec\\Standard\\Codg\\2017\\196\\CODG_2017_196_06.tif");
	files.push_back("D:\\tec\\Standard\\Codg\\2017\\196\\CODG_2017_196_05.tif");

	std::vector<boost::filesystem::path> expected;
	expected.push_back("D:\\tec\\Standard\\Codg\\2017\\196\\CODG_2017_196_05.tif");
	expected.push_back("D:\\tec\\Standard\\Codg\\2017\\196\\CODG_2017_196_06.tif");
	expected.push_back("D:\\tec\\Standard\\Codg\\2017\\196\\CODG_2017_196_07.tif");

	tec_api::sort_files_by_time_asc(files);
	BOOST_CHECK_EQUAL_COLLECTIONS(files.begin(), files.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(date_conv_test)
{
	boost::gregorian::date d{ 2017,7,14 };
	BOOST_TEST_MESSAGE(d);
	BOOST_TEST_MESSAGE(d.day_of_year());

	boost::gregorian::date d1{ 2017,1,1 };
	BOOST_TEST_MESSAGE(d1.day_of_year());
	BOOST_TEST_MESSAGE((d1 += boost::gregorian::days(195 - 1)));
}

BOOST_AUTO_TEST_CASE(get_data_paths_test)
{
	const boost::gregorian::date start{ 2017,7,17 };
	const boost::gregorian::date stop{ 2017,7,17 };
	const size_t wlen = 2;
	const boost::filesystem::path base_dir{ "D:\\tec\\Standard\\Codg" };
	std::vector<boost::filesystem::path> files = tec_api::get_data_paths(base_dir, start, stop, wlen);

	const std::string expected_files{ "D:\\tmp\\expected_paths.txt" };
	std::vector<std::string> strs{};
	std::string str{};
	boost::filesystem::load_string_file(expected_files, str);
	boost::split(strs, str, boost::algorithm::is_any_of("\r\n"), boost::token_compress_on);
	std::vector<boost::filesystem::path> expected;
	std::transform(strs.begin(), strs.end(), std::back_inserter(expected), [](auto s) {return s; });

	for (auto p : files)
		BOOST_TEST_MESSAGE(p);

	BOOST_CHECK_EQUAL_COLLECTIONS(files.begin(), files.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(get_dir_last_name)
{
	boost::filesystem::path p{ "D:\\tec\\Ano\\CODG" };
	BOOST_TEST_MESSAGE(p.filename());
	BOOST_CHECK_EQUAL(p.filename().string(), "CODG");
}