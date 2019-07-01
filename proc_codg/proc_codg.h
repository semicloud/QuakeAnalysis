#pragma once

#include <armadillo>
#include <optional>
#include <string>
#include <vector>

const  std::string TEC_MAP_START("START OF TEC MAP");
const std::string TEC_MAP_END("END OF TEC MAP");
const std::string DATA_SIGN("LAT/LON1/LON2/DLON/H");

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

/**
 * \brief 加载CODG数据至fmat
 * \param file_path_str CODG数据路径
 * \return
 */
std::vector<std::optional<arma::fmat>> load_codg(const std::string& file_path_str);

/**
 * \brief 从数据行中解析出fmat
 * \param data_section codg文件中的一小时内的数据部分
 * \return
 */
std::optional<arma::fmat> load_fmat(const std::vector<std::string>& data_section);

/**
 * \brief 从数据行中解析出具体数据
 * \param data_lines
 * \return
 */
std::optional<arma::fvec> load_fvec(const std::vector<std::string>& data_lines);

/**
 * \brief 将文本文件加载到vector<string>中
 * \param file_path_str 文本文件路径
 * \return
 */
std::vector<std::string> load_text_file(const std::string& file_path_str);