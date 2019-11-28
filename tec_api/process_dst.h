#pragma once

namespace proc_dst
{
	__declspec(dllexport) std::string __cdecl load_file(boost::filesystem::path const&);

	__declspec(dllexport) std::vector<std::string> __cdecl str2vec(std::string const&);

	__declspec(dllexport) int __cdecl index_of_str(std::vector<std::string>&, std::string const&);

	__declspec(dllexport) std::tuple<int, int> __cdecl parse_year_month(std::string const&);

	__declspec(dllexport) std::vector<size_t> __cdecl get_day_vec(std::string const&);

	__declspec(dllexport) std::vector<int> __cdecl get_value_vec(std::string const&);

	__declspec(dllexport) std::vector<std::string> __cdecl get_time_vec(int, int, std::vector<size_t> const&);

	__declspec(dllexport) std::string __cdecl process_dst(std::string const&);

}