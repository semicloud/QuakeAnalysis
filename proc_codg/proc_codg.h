#pragma once
#include <filesystem>
#include <string>

namespace proc_codg
{
	bool process_command_line(int argc, char** argv,
		std::string& in_file, std::string& out_dir, bool& debug, bool& has_err);
	int process_codg(const std::string& in_file, const std::string& out_dir, bool is_debug);
}

