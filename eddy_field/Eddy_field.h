#pragma once

#include "Eddy_field_options_yaml.h"
#include <armadillo>
#include <boost/optional.hpp>

/**
 * \brief 涡度计算类
 */
class Eddy_field
{
private:
	static boost::optional<arma::fmat> get_eddy_field_8(arma::fmat& f_mat, float default_value);

public:
	Eddy_field();
	~Eddy_field();
	/**
	 * \brief 计算4点涡度
	 * \param f_mat 矩阵对象
	 * \param default_value 默认值
	 * \return 
	 */
	static boost::optional<arma::fmat> get_eddy_field_4(arma::fmat& f_mat, float default_value);

	static boost::optional<arma::fmat> get_eddy_field_4(arma::fmat& f_mat, float default_value, arma::fmat& spec_mat);

	/**
	 * \brief 计算涡度背景场 - 方法1
	 */
	static void compute_eddy_field_m1(Eddy_field_options_yaml&);
	
	/**
	 * \brief 计算涡度背景场 - 方法2
	 */
	static void compute_eddy_field_m2(Eddy_field_options_yaml&);
};

