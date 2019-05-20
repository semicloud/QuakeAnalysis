#pragma once
#include <armadillo>
#include <boost/log/trivial.hpp>

/**
 * \brief С���任������
 * \brief ����һάС���任����άС���任
 */
class Wavelet_utils
{
public:
	Wavelet_utils();
	~Wavelet_utils();
	arma::fvec wavelet_transform(const arma::fvec&);
	arma::fmat wavelet2d_transform(const arma::fmat&);
	template <typename T> static T get_left_closest_of_pow2(T t);
};

/**
 * \brief ��ȡ�����ӽ�2���ݴη����飬����25������16��129����128,333����256
 * \brief ���ߺ�������Ϊgsl��С���任����Ҫ�����ݴ�СΪ2�����������ʹ�øú���
 * \brief �����ݴ�С�������ƣ����б������32����������
 * \tparam T
 * \param t
 * \return
 */
template <typename T>
T Wavelet_utils::get_left_closest_of_pow2(T t)
{
	T old = t;
	T new_val;
	if (t >= 32 && t < 64)
		new_val = static_cast<T>(32);
	else if (t >= 64 && t < 128)
		new_val = static_cast<T>(64);
	else if (t >= 128 && t < 256)
		new_val = static_cast<T>(128);
	else if (t >= 256 && t < 512)
		new_val = static_cast<T>(256);
	else if (t >= 512 && t < 1024)
		new_val = static_cast<T>(512);
	else if (t >= 1024 && t < 2048)
		new_val = static_cast<T>(1024);
	else if (t >= 2048 && t < 4096)
		new_val = static_cast<T>(2048);
	BOOST_LOG_TRIVIAL(debug) << "����Ϊ2����������"
		<< old << "������" << new_val;
	return new_val;
}


