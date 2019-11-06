#pragma once
#include <vector>

namespace commons
{
	namespace collections
	{
		/**
		 * \brief ��������ֵ��ȡvector��һ����
		 * \tparam T
		 * \param v vector
		 * \param start ������ʼֵ
		 * \param end ��������ֵ
		 * \return vector��������ʼֵ����������ֵ֮��Ĳ���
		 */
		template <typename T> __declspec(dllexport) extern
			std::vector<T> slice(const std::vector<T>& v, size_t start, size_t end)
		{
			std::vector<T> vec(end - start + 1);
			std::copy(v.begin() + start, v.begin() + end + 1, vec.begin());
			return vec;
		}
	}
}