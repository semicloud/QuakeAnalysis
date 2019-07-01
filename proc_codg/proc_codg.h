#pragma once

#include <armadillo>
#include <optional>
#include <string>
#include <vector>

const  std::string TEC_MAP_START("START OF TEC MAP");
const std::string TEC_MAP_END("END OF TEC MAP");
const std::string DATA_SIGN("LAT/LON1/LON2/DLON/H");

/**
 * \brief ��������ֵ��ȡvector��һ����
 * \tparam T
 * \param v vector
 * \param start ������ʼֵ
 * \param end ��������ֵ
 * \return vector��������ʼֵ����������ֵ֮��Ĳ���
 */
template<typename T>
std::vector<T> slice(const std::vector<T>& v, size_t start, size_t end)
{
	std::vector<T> vec(end - start + 1);
	std::copy(v.begin() + start, v.begin() + end + 1, vec.begin());
	return vec;
}

/**
 * \brief ����CODG������fmat
 * \param file_path_str CODG����·��
 * \return
 */
std::vector<std::optional<arma::fmat>> load_codg(const std::string& file_path_str);

/**
 * \brief ���������н�����fmat
 * \param data_section codg�ļ��е�һСʱ�ڵ����ݲ���
 * \return
 */
std::optional<arma::fmat> load_fmat(const std::vector<std::string>& data_section);

/**
 * \brief ���������н�������������
 * \param data_lines
 * \return
 */
std::optional<arma::fvec> load_fvec(const std::vector<std::string>& data_lines);

/**
 * \brief ���ı��ļ����ص�vector<string>��
 * \param file_path_str �ı��ļ�·��
 * \return
 */
std::vector<std::string> load_text_file(const std::string& file_path_str);