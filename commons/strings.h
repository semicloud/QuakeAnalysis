#pragma once
#include "stdafx.h"
#include <string>

namespace commons
{
	namespace strings
	{
		/**
		 * \brief ɾ���ַ����ڶ�����ַ����ǿո��ַ��������һ���ո�
		 * \param str
		 * \return �������ַ���
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