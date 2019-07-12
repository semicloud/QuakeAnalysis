#pragma once

#include <armadillo>
#include <string>
#include <vector>
#include <optional>
#include <boost/format.hpp>

namespace modis_api
{
	class __declspec(dllexport) Mat_operation
	{
	private:
		/**
		 * \brief mat_list�Ƿ������ֵ�����б���ÿ��������������Ƿ����
		 * \param mat_list
		 * \return
		 */
		static bool same_size(const std::vector<arma::fmat>& mat_list);

		/**
		 * \brief vec�е�Ԫ���Ƿ����
		 * \tparam T
		 * \param vec
		 * \return
		 */
		template<typename T>
		static bool all_equal(const std::vector<T>& vec)
		{
			return adjacent_find(vec.begin(), vec.end(), std::not_equal_to<T>()) == vec.end();
		}

		/**
		 * \brief ����8���жȣ��Ѿ�ʵ���ˣ�����������ɣ������Ҫ���ϳ���
		 * \param f_mat
		 * \param default_value
		 * \return
		 */
		static boost::optional<arma::fmat> get_eddy_field_8(arma::fmat& f_mat, float default_value);

	public:
		Mat_operation();
		~Mat_operation();
		/**
		 * \brief ���������ƽ��ֵ���������һ������Ҫ����������������һ��
		 * \param mat_list
		 * \param default_value �����Ĭ��ֵ��������Ĭ��ֵ������
		 * \return
		 */
		static std::optional<arma::fmat> mean_mat_by_each_pixel(std::vector<arma::fmat>& mat_list, int default_value);

		/**
		 * \brief ��������
		 * \param mat
		 * \return
		 */
		static std::string mat_desc(arma::fmat& mat);
	};

}
