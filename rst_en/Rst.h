#pragma once
#include <armadillo>
#include <vector>

/**
 * \brief Rst核心算法
 */
class Rst
{
public:
	Rst();
	~Rst();
	static arma::fcube construct_background_field(
		const std::vector<arma::fcube>&);
	static arma::fcube construct_alice_cube(const arma::fcube&,
		const std::vector<arma::fcube>&);
private:
	/**
	 * \brief 计算alice指数
	 * \param val 数值
	 * \param mean 均值
	 * \param sd 标准差
	 * \return alice指数
	 */
	static inline float alice(float val, float mean, float sd)
	{
		return (val - mean) / sd;
	}
};

