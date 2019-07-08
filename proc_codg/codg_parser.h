#pragma once
#include <vector>
#include <string>
#include <optional>
#include <armadillo>

/**
 * \brief ���������н�����fmat
 * \param dataSec codg�ļ��е�һСʱ�ڵ����ݲ���
 * \return
 */
std::optional<arma::fmat> parse_fmat(const std::vector<std::string>& dataSec);

int parse_epoch(const std::vector<std::string>& dataSec, int& year, int& month, int& day, int&hour);