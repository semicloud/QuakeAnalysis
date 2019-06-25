#include "pch.h"
#include "Cube_utils.h"
#include "Wavelet_utils.h"
#include "../eddy_field/Eddy_field.h"
#include "../modis_api/Gdal_operation.h"
#include <armadillo>
#include <boost/log/trivial.hpp>

void Cube_utils::test_sd_cubes()
{
	arma::fcube cube1(3, 3, 3, arma::fill::randu);
	cube1.print(std::cout, "cube1");

	arma::fcube cube2(3, 3, 3, arma::fill::randn);
	cube2.print(std::cout, "cube2");

	arma::fcube cube3(3, 3, 3, arma::fill::randu);
	cube3.print(std::cout, "cube3");

	std::vector<arma::fcube> cubes = { cube1, cube2, cube3 };

	//arma::fcube mean_cube = mean_cubes(cubes);
	//mean_cube.print(std::cout, "mean cube");

	arma::fcube sd_cube = sd_cubes(cubes);
	sd_cube.print(std::cout, "sd cube");

	arma::fvec vec({ cube1(0,0,0),cube2(0,0,0),cube3(0,0,0) });
	float expected = arma::stddev(vec, 0);
	assert(sd_cube(0, 0, 0) - expected < 0.01);
}

Cube_utils::Cube_utils() = default;
Cube_utils::~Cube_utils() = default;

/**
 * \brief 加载数据立方
 * \param tif_paths tif文件路径
 * \return 数据立方
 */
arma::fcube Cube_utils::load_cube(const std::vector<std::string>& tif_paths)
{
	std::vector<arma::fmat> mats;
	mats.reserve(tif_paths.size());
	for (const std::string& file : tif_paths)
		mats.push_back(*modis_api::Gdal_operation::read_tif_to_fmat(file));

	const arma::uword n_row = mats.front().n_rows;
	const arma::uword n_col = mats.front().n_cols;
	const arma::uword n_zol = mats.size();
	arma::fcube cube(n_row, n_col, n_zol, arma::fill::zeros);
	for (arma::uword z = 0; z < n_zol; ++z)
	{
		for (arma::uword r = 0; r < n_row; ++r)
		{
			for (arma::uword c = 0; c < n_col; ++c)
			{
				cube(r, c, z) = mats[z](r, c);
			}
		}
	}

	BOOST_LOG_TRIVIAL(debug) << "Load cube, " << cube.size()
		<< " values present, Row=" << n_row << "， Col=" << n_col << "， Zol=" << n_zol;
	return cube;
}

void Cube_utils::shrink_cube_to_square(arma::fcube& cube)
{
	const arma::uword size = std::min(cube.n_rows, cube.n_cols);
	const arma::uword shrink_size = Wavelet_utils::get_left_closest_of_pow2(size);
	cube.resize(shrink_size, shrink_size, cube.n_slices);
	BOOST_LOG_TRIVIAL(debug) << "Shrink size: Cube(" << cube.n_rows << ", "
		<< cube.n_cols << ", " << cube.n_slices << ")";
}

/**
 * \brief 对cube进行按照z轴方向进行一维小波变换
 * \param cube cube
 * \return 小波转换后的cube
 */
arma::fcube Cube_utils::cube_wavelet1d(const arma::fcube& cube)
{
	const arma::uword n_row = cube.n_rows;
	const arma::uword n_col = cube.n_cols;
	const arma::uword n_slice = cube.n_slices;
	arma::fcube ret_cube(n_row, n_col, n_slice, arma::fill::zeros);
	for (arma::uword row = 0; row < n_row; ++row)
	{
		for (arma::uword col = 0; col < n_col; ++col)
		{
			const arma::fvec tube = cube.tube(row, col);
			Wavelet_utils utils;
			arma::fvec wav = utils.wavelet_transform(tube);
			//arma::fvec wav = Wavelet_utils::wavelet_transform(tube);
			for (arma::uword i = 0; i < wav.size(); ++i)
				ret_cube(row, col, i) = wav(i);
		}
	}
	BOOST_LOG_TRIVIAL(debug) << "Cube(" << n_row << ","
		<< n_col << "," << n_slice << ")" << " Z-axis 1d-wavelet process complete";
	return ret_cube;
}

/**
 * \brief 对cube中沿z轴方向的矩阵进行2维小波变换
 * \param cube cube
 * \return 变换后的cube
 */
arma::fcube Cube_utils::cube_wavelet2d(const arma::fcube& cube)
{
	const arma::uword n_row = cube.n_rows;
	const arma::uword n_col = cube.n_cols;
	const arma::uword n_slice = cube.n_slices;
	arma::fcube ret_cube(n_row, n_col, n_slice, arma::fill::zeros);
	for (arma::uword s = 0; s < n_slice; ++s)
	{
		Wavelet_utils utils;
		ret_cube.slice(s) = utils.wavelet2d_transform(cube.slice(s));
	}
	BOOST_LOG_TRIVIAL(debug) << "Cube(" << n_row << ","
		<< n_col << "," << n_slice << ")" << " 2d-wavelet process complete..";
	return ret_cube;
}

/**
 * \brief 在z轴方向上连接一组cube
 * \param cubes cubes
 * \return 连接后的cube
 */
arma::fcube Cube_utils::join_cubes_z(const std::vector<arma::fcube>& cubes)
{
	const arma::fcube first_cube = cubes.front();
	const arma::uword n_row = first_cube.n_rows;
	const arma::uword n_col = first_cube.n_cols;
	const arma::uword n_slice = first_cube.n_slices;
	const arma::uword total_slices = n_slice * cubes.size();
	arma::fcube ret_cube;
	for (const arma::fcube& cube : cubes)
	{
		ret_cube = join_slices(ret_cube, cube);
	}
	BOOST_LOG_TRIVIAL(debug) << cubes.size() << "个(" << n_row << ","
		<< n_col << "," << n_slice << ") Cube拼接完毕，拼接后的cube为("
		<< ret_cube.n_rows << "," << ret_cube.n_cols << ","
		<< ret_cube.n_slices << ")";
	return ret_cube;
}

/**
 * \brief 计算cube序列的平均值，其结果是一个cube
 * \param cubes
 * \return
 */
arma::fcube Cube_utils::mean_cubes(const std::vector<arma::fcube>& cubes)
{
	const arma::fcube first_cube = cubes.front();
	const arma::uword n_row = first_cube.n_rows;
	const arma::uword n_col = first_cube.n_cols;
	const arma::uword n_slice = first_cube.n_slices;
	const size_t N = cubes.size();

	arma::fcube ret_cube(n_row, n_col, n_slice, arma::fill::zeros);

	for (const arma::fcube& cube : cubes)
		ret_cube += cube;

	ret_cube = ret_cube / static_cast<float>(N);

	return ret_cube;
}

/**
 * \brief 计算Cube序列的标准差，其结果是一个Cube
 * \return
 */
arma::fcube Cube_utils::sd_cubes(const std::vector<arma::fcube>& cubes)
{
	if (cubes.empty())
		throw std::runtime_error("cubes are empty! can not compute standard deviation");
	if (cubes.size() == 1)
		throw std::runtime_error("only 1 cube, so can not compute standard deviation");

	const arma::fcube first_cube = cubes.front();
	const arma::uword n_row = first_cube.n_rows;
	const arma::uword n_col = first_cube.n_cols;
	const arma::uword n_slice = first_cube.n_slices;
	const size_t N = cubes.size();

	arma::fcube ret_cube(n_row, n_col, n_slice, arma::fill::zeros);

	const arma::fcube mean_cube = mean_cubes(cubes);

	arma::fcube sum_cube(n_row, n_col, n_slice, arma::fill::zeros);
	for (const arma::fcube& cube : cubes)
	{
		arma::fcube cube1 = cube - mean_cube;
		arma::fcube cube2 = square(cube1);
		sum_cube += cube2;
	}

	ret_cube = arma::sqrt(sum_cube / static_cast<float>((N - 1)));

	return ret_cube;
}


