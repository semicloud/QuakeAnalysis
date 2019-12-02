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