#include "pch.h"
#include "proc_dst.h"
#include "../tec_api/process_dst.h"

int main(int argc, char** argv)
{
	std::string  in_file{  };
	std::string out_file{ };
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
	const std::string ans = proc_dst::process_dst(file_content);
	if (boost::filesystem::exists(out_file))
		boost::filesystem::remove(out_file);
	boost::filesystem::save_string_file(out_file, ans);
	return EXIT_SUCCESS;
}

bool process_command_line(int argc, char** argv, std::string& in_file, std::string& out_file)
{
	try
	{
		boost::program_options::options_description desc("Usage:");
		desc.add_options()
			("help,h", "show help message")
			("version,v", "show version")
			("input-file,i", boost::program_options::value<std::string>(&in_file)->required(), "input dst file")
			("output-file,o", boost::program_options::value<std::string>(&out_file)->required(), "output txt file");

		boost::program_options::variables_map vm;
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

		if (vm.count("help"))
		{
			std::cout << desc << "\n";
			return false;
		}

		if (vm.count("version"))
		{
			std::cout << "DST preprocess program version 1.0\n" << desc << "\n";
			return false;
		}

		boost::program_options::notify(vm);
	}
	catch (std::exception& e)
	{
		std::cerr << "Error." << e.what() << "\n";
		return false;
	}
	return true;
}