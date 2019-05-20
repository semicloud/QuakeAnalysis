#pragma once
#include <string>
#define NOMINMAX
#include <Windows.h>

namespace modis_api
{
	class __declspec(dllexport) Program_operation
	{
	private:
		static LPSTR string_2_lpstr(const std::string&);
		static LPWSTR string_2_lpwstr(const std::string&);
	public:
		static  bool run(const std::string&);
		Program_operation();
		~Program_operation();
	};

}
