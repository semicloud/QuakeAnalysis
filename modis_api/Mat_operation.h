#pragma once

#include <armadillo>
#include <string>
#include <vector>
#include <optional>
#include <boost/format.hpp>

namespace modis_api
{
	class __declspec(dllexport) Mat_operation
	{
	private:
		/**
		 * \brief mat_list是否能求均值，即列表中每个矩阵的行列数是否相等
		 * \param mat_list
		 * \return
		 */
		static bool same_size(const std::vector<arma::fmat>& mat_list);

		/**
		 * \brief vec中的元素是否都相等
		 * \tparam T
		 * \param vec
		 * \return
		 */
		template<typename T>
		static bool all_equal(const std::vector<T>& vec)
		{
			return adjacent_find(vec.begin(), vec.end(), std::not_equal_to<T>()) == vec.end();
		}

		/**
		 * \brief 计算8点涡度，已经实现了，隐藏在这里吧，如果需要就拖出来
		 * \param f_mat
		 * \param default_value
		 * \return
		 */
		static boost::optional<arma::fmat> get_eddy_field_8(arma::fmat& f_mat, float default_value);

	public:
		Mat_operation();
		~Mat_operation();
		/**
		 * \brief 求多个矩阵的平均值，结果还是一个矩阵，要求各个矩阵的行列数一样
		 * \param mat_list
		 * \param default_value 矩阵的默认值，遇到此默认值则跳过
		 * \return
		 */
		static std::optional<arma::fmat> mean_mat_by_each_pixel(std::vector<arma::fmat>& mat_list, int default_value);

		/**
		 * \brief 矩阵描述
		 * \param mat
		 * \return
		 */
		static std::string mat_desc(arma::fmat& mat);
	};

}
