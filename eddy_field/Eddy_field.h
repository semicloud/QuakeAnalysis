#pragma once

#include "Eddy_field_options_yaml.h"
#include <armadillo>
#include <boost/optional.hpp>

/**
 * \brief �жȼ�����
 */
class Eddy_field
{
private:
	static boost::optional<arma::fmat> get_eddy_field_8(arma::fmat& f_mat, float default_value);

public:
	Eddy_field();
	~Eddy_field();
	/**
	 * \brief ����4���ж�
	 * \param f_mat �������
	 * \param default_value Ĭ��ֵ
	 * \return 
	 */
	static boost::optional<arma::fmat> get_eddy_field_4(arma::fmat& f_mat, float default_value);

	static boost::optional<arma::fmat> get_eddy_field_4(arma::fmat& f_mat, float default_value, arma::fmat& spec_mat);

	/**
	 * \brief �����жȱ����� - ����1
	 */
	static void compute_eddy_field_m1(Eddy_field_options_yaml&);
	
	/**
	 * \brief �����жȱ����� - ����2
	 */
	static void compute_eddy_field_m2(Eddy_field_options_yaml&);
};

