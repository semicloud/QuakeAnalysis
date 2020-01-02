#pragma once
#include <vector>
#include <optional>
#include <string>
#include <armadillo>

namespace proc_hrtec
{
	std::optional<arma::fmat> parse_mat(const std::vector<std::string>& dataSec);

	int parse_epoch(const std::vector<std::string>& dataSec, int& year, int& month, int& day, int&hour);
}