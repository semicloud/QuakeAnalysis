#include "pch.h"
#include "../tec_api/process_kp_ap.h"
#include <boost/test/unit_test.hpp>

const std::string kp_ap_file =
"D:\\项目\\2018-地震电离层数据处理\\电离层数据\\kp_ap\\kp_ap_2017.dat";

BOOST_AUTO_TEST_CASE(kp_ap_file_exist)
{
	BOOST_TEST(boost::filesystem::exists(kp_ap_file));
}

BOOST_AUTO_TEST_CASE(process_kp_ap_test)
{
	std::string content{};
	boost::filesystem::load_string_file(kp_ap_file, content);
	const std::string ans = proc_kp_ap::process_kp_ap(content);
	BOOST_TEST(ans.size() > 0);
	BOOST_TEST_MESSAGE(ans);
}