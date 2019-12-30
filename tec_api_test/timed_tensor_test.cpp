#include "pch.h"
#include "../tec_api/timed_tensor.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_parse_time)
{
	auto actual = tec_api::parse_time("D:\\tec\\Standard\\Codg\\2017\\195\\CODG_2017_195_09.tif");
	const boost::gregorian::date d{ 2017,7,14 };
	boost::posix_time::ptime expected(d, boost::posix_time::hours(9));
	BOOST_TEST(actual == expected);
	BOOST_TEST_MESSAGE(expected);
	BOOST_TEST(actual + boost::posix_time::hours(1) == boost::posix_time::ptime(d, boost::posix_time::hours(10)));
	BOOST_TEST(actual - boost::posix_time::hours(1) == boost::posix_time::ptime(d, boost::posix_time::hours(8)));
}

BOOST_AUTO_TEST_CASE(load_tif_test)
{
	auto x = tec_api::load_tif("D:\\tec\\Standard\\Codg\\2017\\195\\CODG_2017_195_09.tif");
	std::ofstream ofs("D:\\tec\\Standard\\Codg\\2017\\195\\CODG_2017_195_09.tif");
	xt::dump_csv(ofs, x);
	BOOST_TEST(x.shape(0) = 71);
	BOOST_TEST(x.shape(1) = 73);
}

BOOST_AUTO_TEST_CASE(file_extension_test)
{
	boost::filesystem::path p("D:\\tec\Standard\\Codg\\2017\\1959.csv");
	BOOST_TEST(p.extension() == ".csv");
	const auto s = (boost::format("%1%-%2$02d-%3$02d") % 2019 % 2 % 2).str();
	BOOST_TEST(s == "2019-02-02");
}

BOOST_AUTO_TEST_CASE(load_from_files_test)
{
	boost::filesystem::path dir{ "D:\\tec\\Standard\\Codg\\2017\\195" };
	std::vector<boost::filesystem::path> paths;
	const auto all_files = boost::make_iterator_range(boost::filesystem::directory_iterator(dir));
	std::copy_if(all_files.begin(), all_files.end(), std::back_inserter(paths),
		[](boost::filesystem::directory_entry const& e) { return is_regular_file(e.path()) && e.path().extension() == ".tif"; });
	tec_api::timed_tensor_series<double, 2> series = tec_api::timed_tensor_series<>::load_from_files(paths);
	BOOST_TEST(paths.size() == series.size());
	BOOST_TEST(series[0].tensor_ptr()->size() == 71 * 73);
	BOOST_TEST(series[0].tensor_ptr()->shape(0) == 71);
	BOOST_TEST(series[0].tensor_ptr()->shape(1) == 73);
}

BOOST_AUTO_TEST_CASE(sort_test)
{
	boost::gregorian::date d{ 2017,7,14 };
	boost::posix_time::ptime start{ d,boost::posix_time::hours(0) };
}

BOOST_AUTO_TEST_CASE(sub_series_test)
{
	boost::filesystem::path dir{ "D:\\tec\\Standard\\Codg\\2017\\195" };
	std::vector<boost::filesystem::path> paths;
	const auto all_files = boost::make_iterator_range(boost::filesystem::directory_iterator(dir));
	std::copy_if(all_files.begin(), all_files.end(), std::back_inserter(paths),
		[](boost::filesystem::directory_entry const& e) { return is_regular_file(e.path()) && e.path().extension() == ".tif"; });
	tec_api::timed_tensor_series<double, 2> series = tec_api::timed_tensor_series<>::load_from_files(paths);
	BOOST_TEST(series.size() == 24);

	// for(size_t i =0 ; i != series.size(); ++i)
	// 	BOOST_TEST_MESSAGE(series[i].time());

	const boost::gregorian::date d{ 2017,7,14 };
	tec_api::timed_tensor_series<double, 2> sub = series.sub_series(3, 5);
	BOOST_TEST(sub[0].time() == boost::posix_time::ptime(d, boost::posix_time::hours(3)));
	BOOST_TEST(sub[1].time() == boost::posix_time::ptime(d, boost::posix_time::hours(4)));
	BOOST_TEST(sub[2].time() == boost::posix_time::ptime(d, boost::posix_time::hours(5)));
}

BOOST_AUTO_TEST_CASE(get_tensor_test)
{
	boost::filesystem::path dir{ "D:\\tec\\Standard\\Codg\\2017\\195" };
	std::vector<boost::filesystem::path> paths;
	const auto all_files = boost::make_iterator_range(boost::filesystem::directory_iterator(dir));
	std::copy_if(all_files.begin(), all_files.end(), std::back_inserter(paths),
		[](boost::filesystem::directory_entry const& e) { return is_regular_file(e.path()) && e.path().extension() == ".tif"; });
	tec_api::timed_tensor_series<double, 2> series = tec_api::timed_tensor_series<>::load_from_files(paths);
	BOOST_TEST(series.size() == 24);

	const boost::gregorian::date d{ 2017,7,14 };
	tec_api::timed_tensor_series<double, 2> sub = series.sub_series(3, 5);

	const xt::xtensor<double, 3> cube = sub.get_tensor_as_whole();
	BOOST_TEST(cube.dimension() == 3);
	xt::xtensor<double, 3>::shape_type shp{ 3,71,73 };
	BOOST_TEST(cube.shape() == shp, "wrong shape");
}
