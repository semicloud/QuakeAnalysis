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
	boost::filesystem::path dir{ "D:\\tec\\Standard\\Codg\\2017\\7\\14\\" };
	std::vector<boost::filesystem::path> paths;
	const auto all_files = boost::make_iterator_range(boost::filesystem::directory_iterator(dir));
	std::copy_if(all_files.begin(), all_files.end(), std::back_inserter(paths),
		[](boost::filesystem::directory_entry const& e) { return is_regular_file(e.path()) && e.path().extension() == ".tif"; });
	tec_api::timed_tensor_series<double, 2> series = tec_api::timed_tensor_series<>::load_from_files(paths);
	tec_api::timed_tensor_series<double, 3> ano = ano_slid_win(series, 3);
	//xt::xtensor<float, 3> x = xt::cast<float>();
	gdal_lib::create_tif("d:\\ano.tif", *ano[0].tensor_ptr(), -1);
}

