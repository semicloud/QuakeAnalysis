#pragma once
#include <vector>
#include <string>
#include <optional>
#include <armadillo>

/**
 * \brief 从数据行中解析出fmat
 * \param dataSec codg文件中的一小时内的数据部分
 * \return
 */
std::optional<arma::fmat> parse_fmat(const std::vector<std::string>& dataSec);

int parse_epoch(const std::vector<std::string>& dataSec, int& year, int& month, int& day, int&hour);