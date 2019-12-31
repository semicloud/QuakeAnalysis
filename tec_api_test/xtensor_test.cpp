#include "pch.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_xtensor_reducer)
{
	xt::random::seed(42);
	xt::xtensor<double, 3> arr = xt::random::randint({ 5,2,3 }, 1, 100);

	xt::xtensor<double, 2> m =
		xt::reduce([](double a, double b) { return std::max(a, b); }, arr, { 0 });
	BOOST_TEST_MESSAGE(arr);
	BOOST_TEST_MESSAGE("reduced:\n" << m);

	xt::xtensor<double, 2> median = xt::median(arr, 0);
	BOOST_TEST_MESSAGE("Median: \n" << median);
}

BOOST_AUTO_TEST_CASE(xtensor_iterator)
{
	xt::xtensor<double, 2> x{ xt::arange(12).reshape({3,4}) };
	BOOST_TEST_MESSAGE(x);
	BOOST_TEST(x.size() == 12);

	for (size_t row = 0; row != x.shape(0); ++row)
	{
		BOOST_TEST_MESSAGE(xt::view(x, row, xt::all()));
	}

	for (auto it = x.begin<xt::layout_type::row_major>(); it != x.end<xt::layout_type::row_major>(); ++it)
	{
		BOOST_TEST_MESSAGE(*it);
	}
}

BOOST_AUTO_TEST_CASE(nan_operation_test)
{
	xt::xarray<double> x1{ {1,2},{3, std::nan(0) } };
	xt::xtensor<double, 1> actual1 = xt::nansum(x1, { 0 });
	xt::xtensor<double, 1> expect1{ 4,2 };
	BOOST_TEST(actual1 == expect1);

	xt::xtensor<double, 1> actual2 = xt::nansum(x1, { 1 });
	xt::xtensor<double, 1> expect2{ 3,3 };
	BOOST_TEST(actual2 == expect2);

	xt::xtensor<double, 1>  actual3 = xt::nanmean(x1, { 0 });
	xt::xtensor<double, 1> expect3{ 2,2 };
	BOOST_TEST(actual3 == expect3);

	xt::xtensor<double, 2> sum = x1 + 1;
	BOOST_TEST_MESSAGE(sum);
	// nan_to_num
	// nan -> 0
	// -inf -> double::min()
	// +inf -> double::max()
	sum = xt::nan_to_num(sum);
	xt::xtensor<double, 2> sum_expected{ {2,3},{4,0} };
	BOOST_TEST(sum == sum_expected);
}

BOOST_AUTO_TEST_CASE(nan_replace_test)
{
	xt::xtensor<double, 2> x{ {1,1},{2,3} };
	BOOST_TEST_MESSAGE(x);
	xt::filtration(x, xt::equal(x, 1)) = std::nan(0);
	BOOST_TEST_MESSAGE(x);

	xt::xtensor<double, 2> x_expected{ {std::nan(0), std::nan(0)}, {2,3} };
	BOOST_TEST_MESSAGE(x_expected);
	//BOOST_TEST(x == x_expected);

	x = xt::nan_to_num(x);
	BOOST_TEST_MESSAGE(x);
	xt::filtration(x, xt::equal(x, 0)) = 9;
	xt::xtensor<double, 2> x_expected2{ {9,9}, {2,3} };
	BOOST_TEST_MESSAGE(x);
	BOOST_TEST(x == x_expected2);
}