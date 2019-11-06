#pragma once
#include "stdafx.h"
#include <string>

namespace commons
{
	namespace strings
	{
		/**
		 * \brief 删除字符串内多余的字符，非空格字符间仅保留一个空格
		 * \param str
		 * \return 处理后的字符串
		 * \remark "a     b" -> "a b"
		 */
		__declspec(dllexport) std::string trim_but_one_inner_copy(const std::string& str);

		__declspec(dllexport) void trim_left(std::string& str);

		__declspec(dllexport) std::string trim_left_copy(std::string str);

		__declspec(dllexport) void trim_right(std::string& str);

		__declspec(dllexport) std::string trim_right_copy(std::string str);

		__declspec(dllexport) void trim(std::string& str);

		__declspec(dllexport) std::string trim_copy(std::string str);
	}
}