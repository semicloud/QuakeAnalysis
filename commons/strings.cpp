#include "stdafx.h"
#include "strings.h"
#include <algorithm>
#include <cctype> /*std::isspace*/
#include <vector>

std::string commons::strings::trim_but_one_inner_copy(const std::string& str)
{
	using namespace std;
	string str1 = trim_copy(str);
	vector<char> characters;
	for (size_t i = 0; i < str1.length(); i++)
	{
		if (str1[i] != ' ')
			characters.push_back(str1[i]);
		else
		{
			characters.push_back(' ');
			size_t j = i;
			while (str1[j] == ' ')
			{
				if (str1[j + 1] != ' ')
				{
					i = j;
					break;
				}
				j++;
			}
		}
	}
	characters.push_back('\0');
	return std::string(characters.data());
}

void commons::strings::trim_left(std::string& str)
{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch)
	{
		return !std::isspace(ch);
	}));
}

std::string commons::strings::trim_left_copy(std::string str)
{
	trim_left(str);
	return str;
}

void commons::strings::trim_right(std::string& str)
{
	str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), str.end());
}

std::string commons::strings::trim_right_copy(std::string str)
{
	trim_right(str);
	return str;
}

void commons::strings::trim(std::string& str)
{
	trim_left(str);
	trim_right(str);
}

std::string commons::strings::trim_copy(std::string str)
{
	trim(str);
	return str;
}

