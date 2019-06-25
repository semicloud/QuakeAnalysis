#include "pch.h"
#include "Wavelet_utils.h"
#include <gsl/gsl_sort.h>
#include <gsl/gsl_wavelet.h>
#include <gsl/gsl_wavelet2d.h>
#include <iostream>

Wavelet_utils::Wavelet_utils() = default;
Wavelet_utils::~Wavelet_utils() = default;

// STRIDE��һ����1
const size_t STRIDE = 1;
// С������
const size_t WAVELET_ORDER = 4;

/**
 * \brief
 * \param vec
 * \return
 */
arma::fvec Wavelet_utils::wavelet_transform(const arma::fvec& vec)
{
	//���롢�������
	double* arr = new double[vec.size()];
	//ϵ������
	double* abscoeff = new double[vec.size()];
	//��ȡ�ɷ�ʹ�õľ���
	size_t* p = new size_t[vec.size()];
	//�˲�������ȡ����������
	int nc = 10;

	//���������ݿ���������������
	std::copy(vec.begin(), vec.end(), arr);
	//����С������
	gsl_wavelet* w = gsl_wavelet_alloc(gsl_wavelet_daubechies, WAVELET_ORDER);
	gsl_wavelet_workspace* work = gsl_wavelet_workspace_alloc(vec.size());
	//�ֽ�
	gsl_wavelet_transform_forward(w, arr, STRIDE, vec.size(), work);
	//��ȡ�ɷ֣��˲������޸�
	for (size_t i = 0; i < vec.size(); ++i)
		abscoeff[i] = fabs(arr[i]);
	gsl_sort_index(p, abscoeff, STRIDE, vec.size());
	for (size_t i = 0; (i + nc) < vec.size(); ++i)
		arr[p[i]] = 0;
	//�ع��ź�
	gsl_wavelet_transform_inverse(w, arr, STRIDE, vec.size(), work);
	//���췵�ض���
	arma::fvec ret(vec.size());
	for (size_t i = 0; i < vec.size(); ++i)
		ret(i) = static_cast<float>(arr[i]);
	// �ͷ���Դ
	gsl_wavelet_workspace_free(work);
	gsl_wavelet_free(w);
	delete[] p;
	delete[] abscoeff;
	delete[] arr;

	return ret;
}

/**
* \brief ��άС���任
* \param mat �������
* \return �任��ľ������
*/
arma::fmat Wavelet_utils::wavelet2d_transform(const arma::fmat& mat)
{
	//std::vector<double> data;
	const arma::uword n_row = mat.n_rows, n_col = mat.n_cols;
	const size_t size = n_row * n_col;
	//�������ȡ��������
	arma::fvec data = vectorise(mat.t());
	//�����ݸ��Ƶ���������
	double* arr_data = new double[data.size()];
	std::copy(data.begin(), data.end(), arr_data);

	//for(int i = 0 ; i < 10 ; ++ i)
	//{
	//	std::cout << arr_data[i] << std::endl;
	//}

	//С���ֽ�
	gsl_wavelet* w = gsl_wavelet_alloc(gsl_wavelet_daubechies, WAVELET_ORDER);
	//n_row�Ϳ���
	gsl_wavelet_workspace* work = gsl_wavelet_workspace_alloc(n_row);
	gsl_wavelet2d_transform_forward(w, arr_data, n_row, n_row, n_col, work);

	// TODO ��������������ȡ�����ݣ����ڲ����н�����ȡ���ع���������ԭ���ľ���

	//С���ع�
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

