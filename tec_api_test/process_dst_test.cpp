#include "pch.h"
#include "../tec_api/process_dst.h"
#include <boost/test/unit_test.hpp>

const static std::string dst_file
= "D:\\项目\\2018-地震电离层数据处理\\电离层数据\\kp_ap\\Dst_201412.dat";

BOOST_AUTO_TEST_CASE(load_file_test, *boost::unit_test::depends_on("file_exist"))
{
	std::string content = proc_dst::load_file(dst_file);
	BOOST_TEST(content.size() > 0);
	//BOOST_TEST_MESSAGE(content);
	//std::copy(strs.begin(), strs.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
}

BOOST_AUTO_TEST_CASE(str2vec_test, *boost::unit_test::depends_on("file_exist"))
{
	const std::string str = proc_dst::load_file(dst_file);
	std::vector<std::string> vec = proc_dst::str2vec(str);
	BOOST_TEST(!vec.empty());
	BOOST_TEST(vec.size() == 38);
	BOOST_TEST(boost::algorithm::contains(vec.at(5), "DAY"));
}

BOOST_AUTO_TEST_CASE(index_of_str_test, *boost::unit_test::disabled())
{
	const std::string str = proc_dst::load_file(dst_file);
	std::vector<std::string> vec = proc_dst::str2vec(str);
	BOOST_TEST(proc_dst::index_of_str(vec, "DAY") == 5);
	BOOST_TEST(proc_dst::index_of_str(vec, "Hourly") == 1);
	BOOST_TEST(proc_dst::index_of_str(vec, "DAT") == -1);
}

BOOST_AUTO_TEST_CASE(parse_year_month_test)
{
	auto t = proc_dst::parse_year_month("                                           DECEMBER   2014");
	BOOST_TEST(std::get<0>(t) == 2014);
	BOOST_TEST(std::get<1>(t) == 12);
}

BOOST_AUTO_TEST_CASE(boost_format_pad_test)
{
	boost::format fmt("%1%%2$02d");
	BOOST_TEST((fmt % 1999 % 1).str() == "199901");
	boost::format fmt2("%1%%2$02d%3$02d%4$02d");
	BOOST_TEST((fmt2 % 1999 % 12 % 1 % 4).str() == "1999120104");
	BOOST_TEST((fmt2 % 1999 % 3 % 12 % 24).str() == "1999031224");
}

BOOST_AUTO_TEST_CASE(get_time_vec_test)
{
	const std::vector<size_t> days{ 1,2 };
	const std::vector<std::string> times = proc_dst::get_time_vec(2019, 12, days);
	BOOST_TEST(times.size() == 24 * days.size());
	BOOST_TEST(times.at(0) == "2019120101");
	BOOST_TEST(times.at(1) == "2019120102");
	BOOST_TEST(times.at(times.size() - 1) == "2019120224");
}

BOOST_AUTO_TEST_CASE(get_day_vec_test, *boost::unit_test::depends_on("file_exist"))
{
	std::string content;
	boost::filesystem::load_string_file(dst_file, content);
	std::vector<size_t > days = proc_dst::get_day_vec(content);
	std::vector<size_t> expected;
	for (size_t i = 1; i != 32; ++i)
		expected.push_back(i);
	BOOST_TEST(days == expected);
}

BOOST_AUTO_TEST_CASE(get_value_vec_test, *boost::unit_test::depends_on("file_exist"))
{
	std::string content;
	boost::filesystem::load_string_file(dst_file, content);
	std::vector<int > values = proc_dst::get_value_vec(content);
	BOOST_TEST(values.size() == 31 * 24);
	BOOST_TEST(values.at(1) == -12);
	BOOST_TEST(values.at(values.size() - 1) == -23);
}

BOOST_AUTO_TEST_CASE(process_dst_test)
{
	std::string content;
	boost::filesystem::load_string_file(dst_file, content);
	std::string out = proc_dst::process_dst(content);
	BOOST_TEST_MESSAGE(out);
	BOOST_TEST(out.size() > 0);
}

BOOST_AUTO_TEST_CASE(file_exist)
{
	BOOST_TEST(boost::filesystem::exists(dst_file));
	BOOST_TEST_MESSAGE("file existence is checked..");
}