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
// * \brief 检查输入的合法性
// * \param inputs
// * \param bad_file_paths 不合法的文件列表
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
//			//BOOST_LOG_TRIVIAL(error) << file_path << "文件全零，无法进行处理！";
//		}
//	}
//	return  bad_file_paths.empty();
//}

/**
 * \brief 预处理RST算法输入数据
 * \param inputs RST算法输入对象
 */
void Rst_prep::preprocess(const std::vector<Rst_prep_input>& inputs)
{
	for (const Rst_prep_input& input : inputs)
	{
		process(input);
	}
}

/**
 * \brief 计算矩阵的忽略零值的平均值
 * \param mat 矩阵
 * \return 忽略零值的平均值
 */
float Rst_prep::mean_mat_skip_zero(const arma::fmat& mat)
{
	return mean(nonzeros(vectorise(mat)));
}

/**
 * \brief 获取向量的忽略零值的平均值
 * \param vec 向量
 * \return 忽略零值的平均值
 */
float Rst_prep::mean_vec_skip_zero(const arma::fvec& vec)
{
	return mean(nonzeros(vec));
}

/**
 * \brief 如果向量不全为零，则计算向量平均值
 * \brief 放入ret中，返回true；向量全为零返回false
 * \param vec 向量
 * \param ret 平均值
 * \return
 */
bool Rst_prep::mean_vec_if_not_all_zero(const arma::fvec& vec, float& ret)
{
	if (all(vec) == 0) return false;
	ret = mean(nonzeros(vec));
	return true;
}

/**
 * \brief 依次获取每一个矩阵(r,c)处的像元
 * \param mats 矩阵序列（排序）
 * \param row r
 * \param col c
 * \return 像元值序列
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
 * \brief 处理每一个Rst算法输入对象
 * \param rp Rst输入对象
 */
void Rst_prep::process(const Rst_prep_input& rp)
{
	arma::fmat cur_mat = *modis_api::Gdal_operation
		::read_tif_to_fmat(rp.get_curr_file());

	// any:check whether any element is non-zero
	// or satisfies a relational condition
	if (!any(arma::vectorise(cur_mat)))
	{
		BOOST_LOG_TRIVIAL(error) << "数据集" << rp.get_curr_file()
			<< "全为零！跳过该数据集的处理！";
		return;
	}

	// 当前矩阵的逐像元平均值
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
				// 相邻矩阵、历史矩阵该像元处的平均值
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
						<< "经过背景值填充后，"
						<< r << ", " << c << "处像元值依然为零！";
				}
			}
		}
	}

	fs::create_directory(fs::path(rp.get_output_file()).parent_path());
	if (fs::exists(rp.get_output_file())) fs::remove(rp.get_output_file());
	fs::copy(rp.get_curr_file(), rp.get_output_file());
	modis_api::Gdal_operation::write_fmat_to_tif(rp.get_output_file(), cur_mat);
}


