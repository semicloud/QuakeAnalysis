#include "stdafx.h"

#include "Mat_operation.h"
#include "../eddy_field/Eddy_field.h"
#include <boost/log/trivial.hpp>
#include <numeric>

modis_api::Mat_operation::Mat_operation()
{
}


modis_api::Mat_operation::~Mat_operation()
{
}

bool modis_api::Mat_operation::same_size(const std::vector<arma::fmat>& mat_list)
{
	std::vector<arma::uword> row_numbers, col_numbers;
	const auto begin = mat_list.begin(), end = mat_list.end();
	std::transform(begin, end, back_inserter(row_numbers), [](const arma::fmat& m) { return m.n_rows; });
	std::transform(begin, end, back_inserter(col_numbers), [](const arma::fmat& m) { return  m.n_cols; });

	const bool row_all_equal = all_equal(row_numbers);
	if (row_all_equal)
	{
		BOOST_LOG_TRIVIAL(debug) << mat_list.size() << "个矩阵的行数都相等";
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "矩阵的行数不相等，无法进行计算";
	}

	const bool col_all_equal = all_equal(col_numbers);
	if (col_all_equal)
	{
		BOOST_LOG_TRIVIAL(debug) << mat_list.size() << "个矩阵的列数都相等";
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "矩阵的列数不相等，无法进行计算";
	}

	// 如果所有的行不相等 或 列不相等，则把所有的行列数打出来
	if (!row_all_equal || !col_all_equal)
	{
		for (auto& mat : mat_list)
			BOOST_LOG_TRIVIAL(error) << "Row: " << mat.n_rows << ", Col: " << mat.n_cols;
	}

	return row_all_equal && col_all_equal;
}

boost::optional<arma::fmat> modis_api::Mat_operation::mean_mat_by_each_pixel(
	std::vector<arma::fmat>& mat_list, int default_value)
{
	if (!same_size(mat_list))
	{
		BOOST_LOG_TRIVIAL(error) << "矩阵大小不等，无法进行平均值计算！";
		return boost::optional<arma::fmat>();
	}
	const arma::uword n_rows = mat_list.front().n_rows;
	const arma::uword n_cols = mat_list.front().n_cols;

	//计算矩阵的平均值，主要逐个像元计算，并且忽略值为零的像元
	auto ans = arma::fmat(n_rows, n_cols, arma::fill::zeros);
	for (arma::uword i = 0; i < n_rows; ++i)
	{
		for (arma::uword j = 0; j < n_cols; ++j)
		{
			float sum = 0;
			float no_zeros_count = 0;
			for (const auto& mat : mat_list)
			{
				if (mat(i, j) != default_value)
				{
					sum += mat(i, j);
					++no_zeros_count;
				}
			}
			ans(i, j) = no_zeros_count != 0 ? sum / no_zeros_count : 0.0f;

			/*
			std::vector<float> vec(mat_list.size());
			for (const arma::fmat& m : mat_list) vec.push_back(m(i, j));
			//std::transform(mat_list.begin(), mat_list.end(), vec.begin(), [&i, &j](const arma::fmat m) {return m(i, j); });
			//各个矩阵该像元值都为零
			if (std::all_of(vec.begin(), vec.end(), [](const float dn) { return dn == 0.0f; }))
			{
				ans(i, j) = 0.0f;
				continue;
			}
			vec.erase(std::remove(vec.begin(), vec.end(), 0), vec.end()); // remove all zeros
			ans(i, j) = std::accumulate(vec.begin(), vec.end(), 0.0f) / static_cast<float>(vec.size());
			*/
		}
	}
	// 以引用的方式捕获， 哦？
	//for_each(mat_list.begin(), mat_list.end(), [&](const arma::fmat m) { ans += m; });
	//ans = ans / static_cast<float>(mat_list.size());

	BOOST_LOG_TRIVIAL(debug) << mat_list.size() << "个矩阵的逐像元平均值计算完毕";

	return boost::optional<arma::fmat>(ans);
}

std::string modis_api::Mat_operation::mat_desc(arma::fmat& mat)
{
	//const arma::fvec vec = arma::vectorise(mat);
	if (mat.has_inf())
		BOOST_LOG_TRIVIAL(warning) << "警告！矩阵中发现Inf值..";
	if (mat.has_nan())
		BOOST_LOG_TRIVIAL(warning) << "警告！矩阵中发现Nan值..";
	std::stringstream ss;
	mat(arma::span(0, 7), arma::span(0, 7)).print(ss, str(boost::format("Row:%1%, Col:%2%") % mat.n_rows % mat.n_cols));
	/*if (arma::all(vec) == 0)
	{
		return str(boost::format("MAT all zeros with ROW_COUNT:%1%, COL_COUNT:%2%") % mat.n_rows % mat.n_cols);
	}
	const arma::fvec sorted_vec = arma::sort(vec);
	const arma::uvec idx = arma::find(sorted_vec > 0);
	const float min_value_greater_than_zero = sorted_vec.at(idx.at(0));

	std::string desc = boost::str(boost::format("MAT INFO [ROW_COUNT:%1%, COL_COUNT:%2%, MIN:%3%,MAX:%4%]"
	) % mat.n_rows % mat.n_cols % min_value_greater_than_zero % mat.max());*/
	return ss.str();
}

