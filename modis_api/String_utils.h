#pragma once
#include <vector>
#include <string>
#include <algorithm>

class String_utils
{
public:
	String_utils();
	~String_utils();

	template<typename T> static
		std::vector<std::string> conv_vec_str(const std::vector<T>& vec);
};

template <typename T>
std::vector<std::string> String_utils::conv_vec_str(const std::vector<T>& vec)
{
	std::vector<std::string> ret(vec.size());
	std::transform(vec.begin(), vec.end(), ret.begin(),
		[](const T& t) { return std::to_string(t); });
	return ret;
}

