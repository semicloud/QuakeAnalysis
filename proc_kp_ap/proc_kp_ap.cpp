
// proc_kp_ap.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "proc_kp_ap.h"
#include  "../tec_api/process_kp_ap.h"

bool process_command_line(int argc, char** argv, std::string& in_file, std::string& out_file)
{
	try
	{
		boost::program_options::options_description desc("Usage:");
		desc.add_options()
			("help,h", "show help message")
			("version,v", "show version")
			("input-file,i", boost::program_options::value<std::string>(&in_file)->required(), "input dst file path")
			("output-file,o", boost::program_options::value<std::string>(&out_file)->required(), "output txt file path");

		boost::program_options::variables_map vm;
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

		if (vm.count("help"))
		{
			std::cout << desc << "\n";
			return false;
		}

		if (vm.count("version"))
		{
			std::cout << "Kp_ap preprocess program version 1.0\n" << desc << "\n";
			return false;
		}

		boost::program_options::notify(vm); // check it!
	}
	catch (std::exception& e)
	{
		std::cerr << "Error." << e.what() << "\n";
		return false;
	}
	return true;
}


int main(int argc, char** argv)
{
	std::string  in_file{  };
	std::string out_file{};
	const bool b = process_command_line(argc, argv, in_file, out_file);
	if (!b)
		return  EXIT_FAILURE;

	if (!boost::filesystem::exists(in_file))
	{
		std::cerr << "Error. Input file " << in_file << " not found!\n";
		return EXIT_FAILURE;
	}

	std::string file_content;
	boost::filesystem::load_string_file(in_file, file_content);
	const std::string ans = proc_kp_ap::process_kp_ap(file_content);
	if (boost::filesystem::exists(out_file))
		boost::filesystem::remove(out_file);
	boost::filesystem::save_string_file(out_file, ans);
	return EXIT_SUCCESS;
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
