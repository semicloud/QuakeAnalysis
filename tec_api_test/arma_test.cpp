#include "pch.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_cube_median)
{
	arma::arma_rng::set_seed(42);
	arma::cube c = arma::randn(2, 3, 10);
	BOOST_TEST_MESSAGE(c);
}