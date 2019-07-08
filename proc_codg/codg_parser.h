#pragma once
#include <vector>
#include <string>
#include <optional>
#include <armadillo>



/**
 * \brief ���������н�����fmat
 * \param data_section codg�ļ��е�һСʱ�ڵ����ݲ���
 * \return
 */
std::optional<arma::fmat> parse_fmat(const std::vector<std::string>& dataSec);
