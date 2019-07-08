#include "pch.h"
#define BOOST_TEST_MODULE strings.cpp
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_DYN_LINK
#include "strings.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test1)
{
	std::string s1 = trim_inner_copy("a               b");
	std::string s2 = trim_inner_copy("a        ");
	std::string s3 = trim_inner_copy("          a  b");
	std::string s4 = trim_inner_copy("   a   b   c    ");
	std::string s5 = trim_inner_copy("abc  ");
	std::string s6 = trim_inner_copy("  abc      def                  g  h  ");
	std::cout << s1 << std::endl;
	BOOST_TEST(s1 == "a b");
	BOOST_TEST(s2 == "a");
	BOOST_TEST(s3 == "a b");
	BOOST_TEST(s4 == "a b c");
	BOOST_TEST(s5 == "abc");
	BOOST_TEST(s6 == "abc def g h");
}