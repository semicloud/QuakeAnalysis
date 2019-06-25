#include "pch.h"
#include "Rst_prep.h"
#include "../eddy_field/Eddy_field.h"
#include "../modis_api/Gdal_operation.h"
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace fs = boost::filesystem;

Rst_prep::Rst_prep() = default;
Rst_prep::~Rst_prep() = default;

///**
// * \brief �������ĺϷ���
// * \param inputs
// * \param bad_file_paths ���Ϸ����ļ��б�
// */
//bool Rst_prep::check_inputs(const std::vector<Rst_prep_input>& inputs,
//	std::vector<std::string>& bad_file_paths)
//{
//	for (const auto& input : inputs)
//	{
//		std::string file_path = input.get_curr_file();
//		arma::fmat mat = *modis_api::Gdal_operation::read_tif_to_fmat(file_path);
//		if (all(vectorise(mat) == 0))
//		{
//			bad_file_paths.push_back(file_path);
//			//BOOST_LOG_TRIVIAL(error) << file_path << "�ļ�ȫ�㣬�޷����д���";
//		}
//	}
//	return  bad_file_paths.empty();
//}

/**
 * \brief Ԥ����RST�㷨��������
 * \param inputs RST�㷨�������
 */
void Rst_prep::preprocess(const std::vector<Rst_prep_input>& inputs)
{
	for (const Rst_prep_input& input : inputs)
	{
		process(input);
	}
}

/**
 * \brief �������ĺ�����ֵ��ƽ��ֵ
 * \param mat ����
 * \return ������ֵ��ƽ��ֵ
 */
float Rst_prep::mean_mat_skip_zero(const arma::fmat& mat)
{
	return mean(nonzeros(vectorise(mat)));
}

/**
 * \brief ��ȡ�����ĺ�����ֵ��ƽ��ֵ
 * \param vec ����
 * \return ������ֵ��ƽ��ֵ
 */
float Rst_prep::mean_vec_skip_zero(const arma::fvec& vec)
{
	return mean(nonzeros(vec));
}

/**
 * \brief ���������ȫΪ�㣬���������ƽ��ֵ
 * \brief ����ret�У�����true������ȫΪ�㷵��false
 * \param vec ����
 * \param ret ƽ��ֵ
 * \return
 */
bool Rst_prep::mean_vec_if_not_all_zero(const arma::fvec& vec, float& ret)
{
	if (all(vec) == 0) return false;
	ret = mean(nonzeros(vec));
	return true;
}

/**
 * \brief ���λ�ȡÿһ������(r,c)������Ԫ
 * \param mats �������У�����
 * \param row r
 * \param col c
 * \return ��Ԫֵ����
 */
arma::fvec Rst_prep::retrieve_z(const std::vector<arma::fmat>& mats,
	const arma::uword row, const arma::uword col)
{
	arma::fvec ret(mats.size(), arma::fill::zeros);
	for (size_t i = 0; i < mats.size(); ++i)
		ret(i) = mats.at(i)(row, col);
	return ret;
}

/**
 * \brief ����ÿһ��Rst�㷨�������
 * \param rp Rst�������
 */
void Rst_prep::process(const Rst_prep_input& rp)
{
	arma::fmat cur_mat = *modis_api::Gdal_operation
		::read_tif_to_fmat(rp.get_curr_file());

	// any:check whether any element is non-zero
	// or satisfies a relational condition
	if (!any(arma::vectorise(cur_mat)))
	{
		BOOST_LOG_TRIVIAL(error) << "���ݼ�" << rp.get_curr_file()
			<< "ȫΪ�㣡���������ݼ��Ĵ���";
		return;
	}

	// ��ǰ���������Ԫƽ��ֵ
	const float cur_mat_mean = mean_mat_skip_zero(cur_mat);

	std::vector<arma::fmat> hist_mats;
	for (const std::string& hist_file : rp.get_hist_files())
		hist_mats.push_back(*modis_api::Gdal_operation::read_tif_to_fmat(hist_file));

	std::vector<arma::fmat> adj_mats;
	for (const std::string& adj_file : rp.get_adjacent_files())
		adj_mats.push_back(*modis_api::Gdal_operation::read_tif_to_fmat(adj_file));

	for (arma::uword r = 0; r < cur_mat.n_rows; ++r)
	{
		for (arma::uword c = 0; c < cur_mat.n_cols; ++c)
		{
			//BOOST_LOG_TRIVIAL(info) << "R:" << r << ", C:" << c;
			float& value = cur_mat.at(r, c);
			if (value == 0)
			{
				// ���ھ�����ʷ�������Ԫ����ƽ��ֵ
				float adj_mean, hist_mean;
				arma::fvec adj_z_vec = retrieve_z(adj_mats, r, c);
				arma::fvec hist_z_vec = retrieve_z(hist_mats, r, c);
				if (!adj_mats.empty() &&
					mean_vec_if_not_all_zero(adj_z_vec, adj_mean))
				{
					value = adj_mean;
				}
				else if (!hist_mats.empty() &&
					mean_vec_if_not_all_zero(hist_z_vec, hist_mean))
				{
					value = hist_mean;
				}
				else
				{
					value = cur_mat_mean;
				}
				if (value == 0)
				{
					BOOST_LOG_TRIVIAL(error) << rp.get_curr_file()
						<< "��������ֵ����"
						<< r << ", " << c << "����Ԫֵ��ȻΪ�㣡";
				}
			}
		}
	}

	fs::create_directory(fs::path(rp.get_output_file()).parent_path());
	if (fs::exists(rp.get_output_file())) fs::remove(rp.get_output_file());
	fs::copy(rp.get_curr_file(), rp.get_output_file());
	modis_api::Gdal_operation::write_fmat_to_tif(rp.get_output_file(), cur_mat);
}


