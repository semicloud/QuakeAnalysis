#include "pch.h"
#include "strings.h"
#include <boost/algorithm/string.hpp>
#include <vector>



std::string trim_inner_copy(const std::string& str)
{
	using namespace std;
	string str1 = boost::trim_copy(str);
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

