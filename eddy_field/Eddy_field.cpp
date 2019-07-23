#include "Eddy_field.h"
#include "..//modis_api//Gdal_operation.h"
#include <boost/log/trivial.hpp>
#include <string>
#include <filesystem>

/**
 * \brief 计算8点涡度
 * \param f_mat
 * \param default_value
 * \return
 */
std::optional<arma::fmat> Eddy_field::get_eddy_field_8(arma::fmat& f_mat, float default_value)
{
	const arma::uword row_count = f_mat.n_rows, col_count = f_mat.n_cols;
	arma::fmat ans(row_count, col_count, arma::fill::zeros);
	ans.row(0) = f_mat.row(0);
	ans.row(row_count - 1) = f_mat.row(row_count - 1);
	ans.col(0) = f_mat.col(0);
	ans.col(col_count - 1) = f_mat.col(col_count - 1);

	for (int i = 1; i < row_count - 1; ++i)
	{
		for (int j = 1; j < col_count - 1; ++j)
		{
			if (f_mat(i, j) != default_value &&
				f_mat(i, j - 1) != default_value && f_mat(i, j + 1) != default_value &&
				f_mat(i - 1, j) != default_value && f_mat(i + 1, j) != default_value &&
				f_mat(i - 1, j - 1) != default_value && f_mat(i + 1, j - 1) != default_value &&
				f_mat(i - 1, j - 1) != default_value && f_mat(i + 1, j + 1) != default_value)
			{
				ans(i, j) = 8 * f_mat(i, j) - f_mat(i, j - 1) - f_mat(i, j + 1) - f_mat(i - 1, j) -
					f_mat(i + 1, j)
					- f_mat(i - 1, j - 1) - f_mat(i + 1, j - 1) - f_mat(i - 1, j - 1) -
					f_mat(i + 1, j + 1);
			}
			else
			{
				ans(i, j) = f_mat(i, j);
			}
		}
	}

	return std::optional<arma::fmat>(ans);
}

Eddy_field::Eddy_field()
{
}


Eddy_field::~Eddy_field()
{
}

/**
 * \brief
 * \param f_mat 计算4点涡度
 * \param default_value default_value
 * \return
 */
std::optional<arma::fmat> Eddy_field::get_eddy_field_4(arma::fmat& f_mat, float default_value)
{
	const arma::uword row_count = f_mat.n_rows, col_count = f_mat.n_cols;
	arma::fmat ans(row_count, col_count, arma::fill::zeros);
	ans.row(0) = f_mat.row(0);
	ans.row(row_count - 1) = f_mat.row(row_count - 1);
	ans.col(0) = f_mat.col(0);
	ans.col(col_count - 1) = f_mat.col(col_count - 1);

	for (arma::uword ir = 1; ir < row_count - 1; ++ir)
	{
		for (arma::uword ic = 1; ic < col_count - 1; ++ic)
		{
			if (f_mat(ir, ic) != default_value && f_mat(ir, ic - 1) != default_value
				&& f_mat(ir, ic + 1) != default_value && f_mat(ir - 1, ic) != default_value
				&& f_mat(ir + 1, ic) != default_value)
			{
				ans(ir, ic) = 4 * f_mat(ir, ic) - f_mat(ir, ic - 1) - f_mat(ir, ic + 1) - f_mat(ir - 1, ic) -
					f_mat(ir + 1, ic);
			}
			else
			{
				ans(ir, ic) = -999;
			}
		}
	}
	//BOOST_LOG_TRIVIAL(info) << "涡度计算完成..";
	return std::optional<arma::fmat>(ans);
}

/**
 * \brief 计算4点涡度，sepc_mat是一个参照矩阵
 * \param f_mat
 * \param default_value
 * \param spec_mat
 * \return
 */
std::optional<arma::fmat> Eddy_field::get_eddy_field_4(arma::fmat& f_mat, float default_value, arma::fmat& spec_mat)
{
	const arma::uword row_count = f_mat.n_rows, col_count = f_mat.n_cols;
	arma::fmat ans(row_count, col_count, arma::fill::zeros);
	ans.row(0) = f_mat.row(0);
	ans.row(row_count - 1) = f_mat.row(row_count - 1);
	ans.col(0) = f_mat.col(0);
	ans.col(col_count - 1) = f_mat.col(col_count - 1);

	const auto is_not_zero = [&spec_mat, &default_value](const arma::uword r, const arma::uword c)
		->bool { return !(spec_mat(r, c) - default_value < 1E-4); };

	for (arma::uword ir = 1; ir < row_count - 1; ++ir)
	{
		for (arma::uword ic = 1; ic < col_count - 1; ++ic)
		{
			if (is_not_zero(ir, ic) &&
				is_not_zero(ir, ic - 1) &&
				is_not_zero(ir, ic + 1) &&
				is_not_zero(ir - 1, ic) &&
				is_not_zero(ir + 1, ic))
			{
				ans(ir, ic) = 4 * f_mat(ir, ic) - f_mat(ir, ic - 1) - f_mat(ir, ic + 1) - f_mat(ir - 1, ic) -
					f_mat(ir + 1, ic);
			}
			else
			{
				ans(ir, ic) = -999;
			}
		}
	}
	//BOOST_LOG_TRIVIAL(info) << "涡度计算完成..";
	return std::optional<arma::fmat>(ans);
}

/**
 * \brief 计算涡度，方法1
 * \param type
 */
void Eddy_field::compute_eddy_field_m1(Eddy_field_options_yaml& options)
{
	// 获取待计算的tif文件DN值m1
	// 获取背景场tif文件DN值m2
	// m3 = m1 - m2;
	// 求m3的涡度
	// 保存计算结果
	using namespace std;
	using namespace filesystem;
	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "=============使用方法1计算涡度====================";

	//const string tif_path = options.get_tif_path(type);
	const std::string tif_path = options.input_image_file();
	if (!exists(tif_path) || !is_regular_file(tif_path))
	{
		BOOST_LOG_TRIVIAL(error) << "目标tif文件" << tif_path << "不存在或不是常规文件，无法计算涡度！";
		BOOST_LOG_TRIVIAL(info) << "=============使用方法1计算涡度结束====================";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}
	BOOST_LOG_TRIVIAL(info) << "目标tif文件：" << tif_path;

	//const string ref_tif_path = options.get_ref_tif_path(type);
	const std::string ref_tif_path = options.ref_image_file();
	if (!exists(ref_tif_path) || !is_regular_file(ref_tif_path))
	{
		BOOST_LOG_TRIVIAL(error) << "背景场文件" << ref_tif_path << "不存在或不是常规文件，无法计算涡度！";
		BOOST_LOG_TRIVIAL(info) << "=============使用方法1计算涡度结束====================";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}
	BOOST_LOG_TRIVIAL(info) << "背景场tif文件：" << ref_tif_path;

	auto bt_mat = modis_api::Gdal_operation::read_tif_to_fmat(tif_path);
	auto ref_fmat = modis_api::Gdal_operation::read_tif_to_fmat(ref_tif_path);

	if (!bt_mat || !ref_fmat)
	{
		BOOST_LOG_TRIVIAL(error) << "目标数据文件或背景场数据读取失败！无法计算涡度！";
		BOOST_LOG_TRIVIAL(info) << "=============使用方法1计算涡度结束====================";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}

	if (bt_mat->n_rows != ref_fmat->n_rows || bt_mat->n_cols != ref_fmat->n_cols)
	{
		BOOST_LOG_TRIVIAL(error) << "矩阵大小不同，无法进行计算..";
		BOOST_LOG_TRIVIAL(error) << str(boost::format("Mat1[%1%, %2%] Mat2[%3%, %4%]") % bt_mat->n_rows % bt_mat->n_cols % ref_fmat->n_rows % ref_fmat->n_cols);
		return;
	}

	arma::fmat diff_mat = (*bt_mat) - (*ref_fmat);

	auto ef_mat = get_eddy_field_4(diff_mat, 0, *bt_mat);
	if (!ef_mat)
	{
		BOOST_LOG_TRIVIAL(error) << "计算涡度出现异常！";
		BOOST_LOG_TRIVIAL(info) << "=============使用方法1计算涡度结束====================";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}

	// 涡度tif路径
	//const string ef_tif_path = options.get_ef_tif_path(type);
	const path ef_tif_path(options.output_eddy_field_image_file());

	if (exists(ef_tif_path))
		remove(ef_tif_path);
	if (!exists(ef_tif_path.parent_path()))
		create_directories(ef_tif_path.parent_path());
	// 复制过去一个tif文件
	copy_file(tif_path, ef_tif_path);
	// 写入数据，齐活
	modis_api::Gdal_operation::write_fmat_to_tif(ef_tif_path.string(), *ef_mat);

	BOOST_LOG_TRIVIAL(info) << "=============使用方法1计算涡度结束====================";
	BOOST_LOG_TRIVIAL(info) << "";
}

/**
 * \brief 计算涡度，方法2
 */
void Eddy_field::compute_eddy_field_m2(Eddy_field_options_yaml& options)
{
	using namespace std;
	using namespace filesystem;
	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "=============使用方法2计算涡度结束====================";

	//const string tif_path = options.get_tif_path(type);
	const path tif_path(options.input_image_file());
	if (!exists(tif_path) || !is_regular_file(tif_path))
	{
		BOOST_LOG_TRIVIAL(error) << "目标tif文件" << tif_path << "不存在或不是常规文件，无法计算涡度！";
		BOOST_LOG_TRIVIAL(info) << "=============使用方法2计算涡度结束====================";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}
	BOOST_LOG_TRIVIAL(info) << "目标tif文件：" << tif_path;

	//const string ref_tif_path = options.get_ref_tif_path(type);
	const path ref_tif_path(options.ref_image_file());
	if (!exists(ref_tif_path) || !is_regular_file(ref_tif_path))
	{
		BOOST_LOG_TRIVIAL(error) << "背景场文件" << ref_tif_path << "不存在或不是常规文件，无法计算涡度！";
		BOOST_LOG_TRIVIAL(info) << "=============使用方法2计算涡度结束====================";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}
	BOOST_LOG_TRIVIAL(info) << "背景场tif文件：" << ref_tif_path;

	auto fmat = modis_api::Gdal_operation::read_tif_to_fmat(tif_path.string());
	auto ref_fmat = modis_api::Gdal_operation::read_tif_to_fmat(ref_tif_path.string());

	if (!fmat || !ref_fmat)
	{
		BOOST_LOG_TRIVIAL(error) << "目标数据文件或背景场数据读取失败！无法计算涡度！";
		BOOST_LOG_TRIVIAL(info) << "=============使用方法2计算涡度结束====================";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}

	arma::fmat diff_mat = (*fmat) - (*ref_fmat); // 已经生成背景场了，涡度相减就完了

	// 涡度tif路径
	//const string ef_tif_path = options.get_ef_tif_path(type);
	const path ef_tif_path(options.output_eddy_field_image_file());

	if (exists(ef_tif_path))
		remove(ef_tif_path);
	create_directories(ef_tif_path.parent_path());
	// 复制过去一个tif文件
	copy_file(tif_path, ef_tif_path);
	// 写入数据，齐活
	modis_api::Gdal_operation::write_fmat_to_tif(ef_tif_path.string(), diff_mat);

	BOOST_LOG_TRIVIAL(info) << "=============使用方法2计算涡度结束====================";
	BOOST_LOG_TRIVIAL(info) << "";
}
