#pragma once
#include <vector>
#include <string>
#include <optional>
#include <armadillo>



/**
 * \brief 从数据行中解析出fmat
 * \param data_section codg文件中的一小时内的数据部分
 * \return
 */
std::optional<arma::fmat> parse_fmat(const std::vector<std::string>& dataSec);
