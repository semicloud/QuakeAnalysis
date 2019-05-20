#pragma once
#include <armadillo>
#include <boost/log/trivial.hpp>

/**
 * \brief 小波变换工具类
 * \brief 包括一维小波变换、二维小波变换
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
 * \brief 获取左侧最接近2的幂次方的书，比如25，返回16；129返回128,333返回256
 * \brief 工具函数，因为gsl的小波变换函数要求数据大小为2的幂数，因此使用该函数
 * \brief 对数据大小进行限制（序列必须大于32！！！！）
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
	BOOST_LOG_TRIVIAL(debug) << "调整为2的幂数，从"
		<< old << "调整到" << new_val;
	return new_val;
}


