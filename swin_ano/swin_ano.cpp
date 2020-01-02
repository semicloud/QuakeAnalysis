// swin_ano.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "../gdal_lib/gdal_lib.h"
#include "../logging/log_setting.h"
#include "../tec_api/tec_ano.h"

std::string to_doy_string(boost::gregorian::date const& d)
{
	return (boost::format("%1$04d%2$03d") % d.year() % d.day_of_year()).str();
}

bool process_command_line(int argc, char** argv,
	boost::gregorian::date& start, boost::gregorian::date& endd, size_t& wlen,
	boost::filesystem::path& input_dir, boost::filesystem::path& output_dir, bool& debug, bool& has_err)
{
	std::string start_str{};
	std::string end_str{};
	std::string input_dir_str{};
	std::string output_dir_str{};
	try
	{
		boost::program_options::options_description desc("Usage");
		desc.add_options()
			("help,h", "show help message")
			("version,v", "show version")
			("start,s", boost::program_options::value<std::string>(&start_str)->required(), "start date, format: yyyymmdd")
			("end,e", boost::program_options::value<std::string>(&end_str)->required(), "end date, format: yyyymmdd")
			("input-dir,i", boost::program_options::value<std::string>(&input_dir_str)->required(), "input file directory")
			("output-dir,o", boost::program_options::value<std::string>(&output_dir_str)->required(), "output file directory")
			("wlen,w", boost::program_options::value<size_t>(&wlen)->required(), "window size")
			("debug,d", boost::program_options::bool_switch(&debug), "run program in debug mode");
		boost::program_options::variables_map vm;
		boost::program_options::store(parse_command_line(argc, argv, desc), vm);
		if (vm.count("help"))
		{
			std::cout << desc << "\n";
			return false;
		}
		if (vm.count("version"))
		{
			std::cout << "Sliding Window Abnormal Detection Program, version 1.0\n" << desc << "\n";
			return false;
		}
		boost::program_options::notify(vm);

		start = boost::gregorian::date_from_iso_string(start_str);
		endd = boost::gregorian::date_from_iso_string(end_str);
		input_dir = boost::filesystem::path{ input_dir_str };
		output_dir = boost::filesystem::path{ output_dir_str };
	}
	catch (std::exception& e)
	{
		std::cerr << "Error. " << e.what() << "\n";
		has_err = true;
		return false;
	}

	if (start >= endd)
	{
		std::cerr << "start date must be smaller than end date" << "\n";
		has_err = true;
		return false;
	}

	BOOST_LOG_TRIVIAL(info) << "Pased options:";
	BOOST_LOG_TRIVIAL(info) << "start: " << to_simple_string(start) << ", " << to_doy_string(start);
	BOOST_LOG_TRIVIAL(info) << "end  : " << to_simple_string(endd) << ", " << to_doy_string(endd);
	BOOST_LOG_TRIVIAL(info) << "wlen : " << wlen;
	BOOST_LOG_TRIVIAL(info) << "input-dir: " << input_dir;
	BOOST_LOG_TRIVIAL(info) << "output-dir :" << output_dir;
	BOOST_LOG_TRIVIAL(info) << "debug: " << debug;

	return true;
}

boost::filesystem::path get_output_path(boost::filesystem::path const& p, boost::posix_time::ptime const& t)
{
	const std::string yyyy = (boost::format("%1$04d") % t.date().year()).str();
	const std::string doy = (boost::format("%1$03d") % t.date().day_of_year()).str();
	const std::string hh = (boost::format("%1$02d") % t.time_of_day().hours()).str();
	const std::string product = boost::to_upper_copy(p.filename().string());
	const std::string file_name = (boost::format("SWin_%1%_%2%_%3%_%4%.tif") % product % yyyy % doy % hh).str();
	return p / file_name;
}

int swin(boost::gregorian::date start, boost::gregorian::date endd, size_t wlen,
	boost::filesystem::path input_dir, boost::filesystem::path output_dir, bool debug)
{
	constexpr size_t out_dim = 3;
	std::vector<boost::filesystem::path> files = tec_api::get_data_paths(input_dir, start, endd, wlen);
	tec_api::timed_tensor_series<float, 2> series = tec_api::timed_tensor_series<float, 2>::load_from_files(files);
	tec_api::timed_tensor_series<float, out_dim> ano = ano_slid_win(series, wlen);
	const size_t xs = ano[0].tensor_ptr()->shape(2);
	const size_t ys = ano[0].tensor_ptr()->shape(1);
	BOOST_LOG_TRIVIAL(debug) << "output xs: " << xs << ", ys: " << ys;

	const std::string proj = gdal_lib::wgs84_proj4();
	std::shared_ptr<double> geo_trans = gdal_lib::default_geo_trans();
	std::map<std::string, std::string> options = gdal_lib::tif_options_for_rgb();

	const double ndv = 0;
	const size_t bn = 3;

	for (size_t i = 0; i != ano.size(); ++i)
	{
		tec_api::timed_tensor<float, out_dim>& t = ano[i];
		const boost::filesystem::path outpath{ get_output_path(output_dir, t.time()) };
		if (!boost::filesystem::exists(outpath.parent_path()))
			boost::filesystem::create_directories(outpath.parent_path());
		gdal_lib::create_tif<float>(outpath.string(), t.tensor_ptr()->data(), ndv, xs, ys, bn, proj, geo_trans.get(), options);
		BOOST_LOG_TRIVIAL(info) << outpath << " saved..";
	}
	return 0;
}

int main(int argc, char** argv)
{
	boost::gregorian::date start{};
	boost::gregorian::date end{};
	size_t wlen = 0;
	boost::filesystem::path input_dir{};
	boost::filesystem::path output_dir{};
	bool debug = false;
	bool err = false;
	logger::init_console_logger();
	const bool b = process_command_line(argc, argv, start, end, wlen, input_dir, output_dir, debug, err);
	if (err) return EXIT_FAILURE;
	if (!b) return EXIT_SUCCESS;
	logger::set_severity(debug ? boost::log::trivial::debug : boost::log::trivial::info);

	swin(start, end, wlen, input_dir, output_dir, debug);
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
