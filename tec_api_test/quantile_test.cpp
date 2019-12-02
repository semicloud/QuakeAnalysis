#include "pch.h"
#include "../tec_api/quantile.h"
#include <boost/test/unit_test.hpp>
#include <gsl/gsl_statistics_double.h>

BOOST_AUTO_TEST_CASE(gsl_quantile_test)
{
	std::vector<double> vec{ 6, 47, 49, 15, 42, 41, 7, 39, 43, 40, 36 };
	std::sort(vec.begin(), vec.end());
	double q1 = gsl_stats_quantile_from_sorted_data(&vec[0], 1, vec.size(), 0.25);
	double q2 = gsl_stats_quantile_from_sorted_data(&vec[0], 1, vec.size(), 0.5);
	double q3 = gsl_stats_quantile_from_sorted_data(&vec[0], 1, vec.size(), 0.75);
	BOOST_TEST(q1 == 25.5, boost::test_tools::tolerance(0.001));
	BOOST_TEST(q2 == 40, boost::test_tools::tolerance(0.001));
	BOOST_TEST(q3 == 42.5, boost::test_tools::tolerance(0.001));
}