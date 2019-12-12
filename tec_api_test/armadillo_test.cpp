#include "pch.h"
#include <armadillo>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_nan)
{
	arma::fvec v1{ 1,2,3 };
	BOOST_TEST(arma::sum(v1) == 6);
}