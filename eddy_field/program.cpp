#include <boost/log/support/date_time.hpp>
#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include "..//modis_api//File_operation.h"
#include "..//modis_api//Logger_setting.h"
#include "Eddy_field.h"
#include "Eddy_field_options_yaml.h"
#include "Eddy_ref.h"

const std::string PROGRAM = "eddy_field";
const std::string VERSION = "1.0";

void check_data(const Eddy_field_options_yaml&);

int main(int argc, char* argv[]) {
	using namespace std;
	using namespace std::filesystem;

	// BOOST_LOG_NAMED_SCOPE("Scope");
	modis_api::init_console_logger();
	modis_api::init_file_logger("logs\\", PROGRAM);
	modis_api::set_logger_severity(boost::log::trivial::info);

	string yml_path;
	boost::program_options::options_description desc("Usage: ");
	desc.add_options()("help,h", "展示帮助信息")(
		"yml,y", boost::program_options::value<std::string>(&yml_path),
		"指定yml配置文件")("debug,d", "以debug模式运行本程序")("version,v",
			"显示版本信息");
	boost::program_options::variables_map vm;

	try {
		store(parse_command_line(argc, argv, desc), vm);
		vm.notify();
	}
	catch (exception& e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return EXIT_SUCCESS;
	}

	if (vm.count("version")) {
		std::cout << PROGRAM + " " + VERSION << std::endl;
		return EXIT_SUCCESS;
	}

	if (vm.count("yml")) {
		yml_path = vm["yml"].as<std::string>();
		if (!exists(yml_path)) {
			BOOST_LOG_TRIVIAL(error) << "找不到YML配置文件：" << yml_path;
			return EXIT_FAILURE;
		}
		BOOST_LOG_TRIVIAL(info) << "YML PATH: " << yml_path;
		if (vm.count("debug")) {
			modis_api::set_logger_severity(boost::log::trivial::debug);
			BOOST_LOG_TRIVIAL(debug) << "程序将以debug模式运行..";
		}
	}

	auto options_yaml = Eddy_field_options_yaml(yml_path);

	check_data(options_yaml);

	// 方法1
	if (options_yaml.method() == 1) {
		// 需要计算背景场
		if (options_yaml.calc_ref()) {
			Eddy_ref::compute_eddy_ref_m1(options_yaml);
		}
		// 需要计算涡度
		if (options_yaml.calc_ano()) {
			Eddy_field::compute_eddy_field_m1(options_yaml);
		}
	}

	// 方法2
	if (options_yaml.method() == 2) {
		// 需要计算背景场
		if (options_yaml.calc_ref()) {
			Eddy_ref::compute_eddy_ref_m2(options_yaml);
		}
		// 需要计算涡度
		if (options_yaml.calc_ano()) {
			Eddy_field::compute_eddy_field_m2(options_yaml);
		}
	}

	return 0;
}

/**
 * \brief 检查输入的.yml文件是否合法，不合法则给出提示，直接退出程序
 */
void check_data(const Eddy_field_options_yaml& options) {
	using namespace std;
	using namespace std::filesystem;
	// 不存在workspacefolder，非法！
	if (!exists(options.workspace())) {
		std::cerr << "MODIS workspace folder " << options.workspace()
			<< " not found!";
		exit(EXIT_FAILURE);
	}
	// 方法不是1或2，非法！
	if (options.method() != 1 && options.method() != 2) {
		std::cerr << "method must be 1 or 2!";
		exit(EXIT_FAILURE);
	}
	if (options.calc_ano()) {
		// 如果计算涡度，则必须给出输入tif文件
		if (!exists(options.input_image_file())) {
			std::cerr << "input image file " << options.input_image_file()
				<< " not found!";
			exit(EXIT_FAILURE);
		}
	}
	if (options.calc_ref()) {
		// 如果生成影响场，support_ref_tif_file必须存在
		if (!exists(options.ref_list_file())) {
			std::cerr << "reference tif list file " << options.ref_list_file()
				<< " not found!";
			exit(EXIT_FAILURE);
		}
	}
	else {
		// 如不生成影响场，则ref_image_file必须存在
		if (!exists(options.ref_image_file())) {
			std::cerr << "ref image file " << options.ref_image_file()
				<< " not found!";
			exit(EXIT_FAILURE);
		}
	}
}
