#pragma once
#include "Rst_prep_input.h"
#include <armadillo>
#include <vector>
#include <string>

/**
 * \brief RST算法的预处数据操作类
 */
class Rst_prep
{
public:
	Rst_prep();
	~Rst_prep();

	//static bool check_inputs(const std::vector<Rst_prep_input>&,
		//std::vector<std::string>&);
	static void preprocess(const std::vector<Rst_prep_input>&);
private:
	static void process(const Rst_prep_input&);
	static float mean_mat_skip_zero(const arma::fmat&);
	static float mean_vec_skip_zero(const arma::fvec&);
	static bool mean_vec_if_not_all_zero(const arma::fvec&, float&);
	static arma::fvec retrieve_z(const std::vector<arma::fmat>&,
		const arma::uword, const arma::uword);
};

