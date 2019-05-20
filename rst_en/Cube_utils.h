#pragma once
#include <armadillo>
#include <vector>
#include <string>

/**
 * \brief 数据体的相关操作
 */
class Cube_utils
{
private:
	void test_sd_cubes();

public:
	Cube_utils();
	~Cube_utils();
	static arma::fcube load_cube(const std::vector<std::string>&);
	static void shrink_cube_to_square(arma::fcube& cube);
	static arma::fcube cube_wavelet1d(const arma::fcube&);
	static arma::fcube cube_wavelet2d(const arma::fcube&);
	static arma::fcube join_cubes_z(const std::vector<arma::fcube>&);
	static arma::fcube mean_cubes(const std::vector<arma::fcube>&);
	static arma::fcube sd_cubes(const std::vector<arma::fcube>&);

	//arma::fvec wavelet_transform(const arma::fvec&);
	//arma::fvec wavelet2d_transform(const arma::fmat&);

};

