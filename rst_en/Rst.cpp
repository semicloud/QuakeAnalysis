#include "pch.h"
#include "Rst.h"
#include "Cube_utils.h"
#include "Wavelet_utils.h"
#include <boost/log/trivial.hpp>
#include <iostream>

Rst::Rst() = default;
Rst::~Rst() = default;

/**
 * \brief 利用多年历史数据构建年变基准场
 * \param cubes cubes 一个cube就是一年的历史数据
 * \return 背景场cube
 */
arma::fcube Rst::construct_background_field(
	const std::vector<arma::fcube>& cubes)
{
	//对每一个历史数据cube，使用一维小波变换进行处理，然后求其平均值作为背景场
	std::vector<arma::fcube> wavelet1d_cubes;
	for (const arma::fcube& cube : cubes)
	{
		arma::fcube wavelet1d_cube = Cube_utils::cube_wavelet1d(cube);
		wavelet1d_cubes.push_back(wavelet1d_cube);
	}

	arma::fcube ret_cube = Cube_utils::mean_cubes(wavelet1d_cubes);

	return ret_cube;
}

