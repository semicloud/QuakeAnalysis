#pragma once

#include <vector>

/**
 * \brief 按照索引值截取vector的一部分
 * \tparam T
 * \param v vector
 * \param start 索引开始值
 * \param end 索引结束值
 * \return vector中索引开始值与索引结束值之间的部分
 */
template<typename T>
std::vector<T> slice(const std::vector<T>& v, size_t start, size_t end)
{
	std::vector<T> vec(end - start + 1);
	std::copy(v.begin() + start, v.begin() + end + 1, vec.begin());
	return vec;
}