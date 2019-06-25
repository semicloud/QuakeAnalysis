#include "stdafx.h"
#include "Program_operation.h"
#include "Windows.h"
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>


LPSTR modis_api::Program_operation::string_2_lpstr(const std::string& str)
{
	return const_cast<char*>(str.c_str());
}

LPWSTR modis_api::Program_operation::string_2_lpwstr(const std::string& str)
{
	auto cstr = str.c_str();
	auto dw_len = strlen(cstr) + 1;
	//算出合适的长度
	auto nw_len = MultiByteToWideChar(CP_ACP, 0, cstr, dw_len, NULL, 0);
	LPWSTR lpwstr = new WCHAR[dw_len];
	MultiByteToWideChar(CP_ACP, 0, cstr, dw_len, lpwstr, nw_len);
	return lpwstr;
}

bool modis_api::Program_operation::run(const std::string& cmd_str)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof si);
	si.cb = sizeof si;
	ZeroMemory(&pi, sizeof pi);

	if (!CreateProcess(NULL, string_2_lpwstr(cmd_str), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		BOOST_LOG_TRIVIAL(error) << "调用" << cmd_str << "出错，错误消息：" << GetLastError();
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	BOOST_LOG_TRIVIAL(debug) << "调用" << cmd_str << "成功";
	return true;
}

modis_api::Program_operation::Program_operation() = default;


modis_api::Program_operation::~Program_operation() = default;
