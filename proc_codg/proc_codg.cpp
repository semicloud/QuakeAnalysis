// proc_codg.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "proc_codg.h"
#include <armadillo>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/optional.hpp>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

// int load_codg(const std::string&);

std::vector<std::optional<arma::fmat>>
load_codg(const std::string& file_path_str)
{
	using namespace arma;
	using namespace std;
	using namespace std::filesystem;

	vector<optional<fmat>> ans;
	const path codg_file_path(file_path_str);
	assert(exists(codg_file_path));
	const vector<string> lines = load_text_file(codg_file_path.string());
	vector<size_t> indexes_start;
	vector<size_t> indexes_end;
	for (vector<string>::const_iterator it = lines.cbegin();
		it != lines.cend(); ++it)
	{
		if (it->find(TEC_MAP_START) != string::npos)
			indexes_start.push_back(distance(lines.cbegin(), it));
		if (it->find(TEC_MAP_END) != string::npos)
			indexes_end.push_back(distance(lines.cbegin(), it));
	}
	assert(!indexes_start.empty());
	assert(indexes_start.size() == indexes_end.size());
	for (size_t i = 0; i != indexes_start.size(); ++i)
	{
		vector<string> data_section = slice(lines, indexes_start[i], indexes_end[i]);
		optional<fmat> optional_mat = load_fmat(data_section);
		ans.push_back(optional_mat);
	}
	return ans;
}

std::optional<arma::fmat> load_fmat(const std::vector<std::string>& data_section)
{
	using namespace std;
	string mat_str;
	for (vector<string>::const_iterator it = data_section.cbegin();
		it != data_section.cend(); ++it)
	{
		if (it->find(DATA_SIGN) != string::npos)
		{
			const size_t index = distance(data_section.cbegin(), it);
			const size_t start_index = index + 1, end_index = index + 5;
			vector<string> data_lines = slice(data_section, start_index, end_index);
			for_each(data_lines.begin(), data_lines.end(), [](string& data_line)
			{boost::trim(data_line); });
			string data_line = boost::join(data_lines, "   ");
			data_line.append(";");
			cout << data_line << endl;
			mat_str.append(data_line);
		}
	}
	const arma::fmat mat(mat_str);
	return optional<arma::fmat>(mat);
}

std::vector<std::string> load_text_file(const std::string& file_path_str)
{
	using namespace std;
	vector<string> ans;
	ifstream stream(file_path_str);
	string line;
	while (getline(stream, line))
		ans.push_back(line);
	cout << ans.size() << " lines loaded from " << file_path_str << endl;
	return ans;
}

int main()
{
	using namespace std;
	std::cout << "Hello World!\n";
	std::cout << std::filesystem::exists("C:\\demo.txt") << std::endl;
	auto mats = load_codg("CODG1950.17I");
	cout << mats.size() << " mats loaded.." << endl;
	return 0;
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
