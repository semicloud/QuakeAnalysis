#pragma once
#include <armadillo>
#include <vector>

/**
 * \brief Rst�����㷨
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
	 * \brief ����aliceָ��
	 * \param val ��ֵ
	 * \param mean ��ֵ
	 * \param sd ��׼��
	 * \return aliceָ��
	 */
	static inline float alice(float val, float mean, float sd)
	{
		return (val - mean) / sd;
	}
};

