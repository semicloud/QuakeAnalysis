#include "pch.h"
#include "Rst.h"
#include "Cube_utils.h"
#include "Wavelet_utils.h"
#include <boost/log/trivial.hpp>
#include <iostream>

Rst::Rst() = default;
Rst::~Rst() = default;

/**
 * \brief ���ö�����ʷ���ݹ�������׼��
 * \param cubes cubes һ��cube����һ�����ʷ����
 * \return ������cube
 */
arma::fcube Rst::construct_background_field(
	const std::vector<arma::fcube>& cubes)
{
	//��ÿһ����ʷ����cube��ʹ��һάС���任���д���Ȼ������ƽ��ֵ��Ϊ������
	std::vector<arma::fcube> wavelet1d_cubes;
	for (const arma::fcube& cube : cubes)
	{
		arma::fcube wavelet1d_cube = Cube_utils::cube_wavelet1d(cube);
		wavelet1d_cubes.push_back(wavelet1d_cube);
	}

	arma::fcube ret_cube = Cube_utils::mean_cubes(wavelet1d_cubes);

	return ret_cube;
}

