#include "pch.h"
#include "Wavelet_utils.h"
#include <gsl/gsl_sort.h>
#include <gsl/gsl_wavelet.h>
#include <gsl/gsl_wavelet2d.h>
#include <iostream>

Wavelet_utils::Wavelet_utils() = default;
Wavelet_utils::~Wavelet_utils() = default;

// STRIDE，一般是1
const size_t STRIDE = 1;
// 小波阶数
const size_t WAVELET_ORDER = 4;

/**
 * \brief
 * \param vec
 * \return
 */
arma::fvec Wavelet_utils::wavelet_transform(const arma::fvec& vec)
{
	//输入、输出数组
	double* arr = new double[vec.size()];
	//系数数组
	double* abscoeff = new double[vec.size()];
	//提取成分使用的矩阵
	size_t* p = new size_t[vec.size()];
	//此参数需提取出来，后期
	int nc = 10;

	//把输入数据拷贝到输入数组里
	std::copy(vec.begin(), vec.end(), arr);
	//构建小波对象
	gsl_wavelet* w = gsl_wavelet_alloc(gsl_wavelet_daubechies, WAVELET_ORDER);
	gsl_wavelet_workspace* work = gsl_wavelet_workspace_alloc(vec.size());
	//分解
	gsl_wavelet_transform_forward(w, arr, STRIDE, vec.size(), work);
	//提取成分，此部分需修改
	for (size_t i = 0; i < vec.size(); ++i)
		abscoeff[i] = fabs(arr[i]);
	gsl_sort_index(p, abscoeff, STRIDE, vec.size());
	for (size_t i = 0; (i + nc) < vec.size(); ++i)
		arr[p[i]] = 0;
	//重构信号
	gsl_wavelet_transform_inverse(w, arr, STRIDE, vec.size(), work);
	//构造返回对象
	arma::fvec ret(vec.size());
	for (size_t i = 0; i < vec.size(); ++i)
		ret(i) = static_cast<float>(arr[i]);
	// 释放资源
	gsl_wavelet_workspace_free(work);
	gsl_wavelet_free(w);
	delete[] p;
	delete[] abscoeff;
	delete[] arr;

	return ret;
}

/**
* \brief 二维小波变换
* \param mat 矩阵对象
* \return 变换后的矩阵对象
*/
arma::fmat Wavelet_utils::wavelet2d_transform(const arma::fmat& mat)
{
	//std::vector<double> data;
	const arma::uword n_row = mat.n_rows, n_col = mat.n_cols;
	const size_t size = n_row * n_col;
	//按行序读取到向量中
	arma::fvec data = vectorise(mat.t());
	//将数据复制到输入数组
	double* arr_data = new double[data.size()];
	std::copy(data.begin(), data.end(), arr_data);

	//for(int i = 0 ; i < 10 ; ++ i)
	//{
	//	std::cout << arr_data[i] << std::endl;
	//}

	//小波分解
	gsl_wavelet* w = gsl_wavelet_alloc(gsl_wavelet_daubechies, WAVELET_ORDER);
	//n_row就可以
	gsl_wavelet_workspace* work = gsl_wavelet_workspace_alloc(n_row);
	gsl_wavelet2d_transform_forward(w, arr_data, n_row, n_row, n_col, work);

	// TODO 这里需加入阶数提取的内容，现在不进行阶数提取，重构出来还是原来的矩阵

	//小波重构
	gsl_wavelet2d_transform_inverse(w, arr_data, n_row, n_col, n_col, work);

	//std::vector<float> ret_data;
	arma::fvec foo_vec(size);
	for (size_t i = 0; i < size; ++i)
		foo_vec(i) = static_cast<float>(arr_data[i]);

	arma::fmat ret_mat(foo_vec);
	ret_mat.reshape(n_row, n_col);

	gsl_wavelet_workspace_free(work);
	gsl_wavelet_free(w);
	delete[] arr_data;

	return ret_mat.t();
}

