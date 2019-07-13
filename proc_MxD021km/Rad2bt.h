#pragma once
#include <string>
#include <armadillo>
#include <optional>
#include <filesystem>

class Rad2bt
{
public:
	Rad2bt();
	~Rad2bt();

	/**
		* \brief 根据HDF文件名获取所使用的lut表路径
		 * \param path_hdf_file hdf文件路径
		 * \return lut表路径
		 */
	static std::filesystem::path get_lut_table_path(const std::filesystem::path& path_hdf_file);

	/**
	 * \brief 加载Lut表中的数据列
	 * \param lut_table_path Lut表路径
	 * \param band 波段名
	 * \return 一个两列矩阵，第一列是bt列，第二列是相应的波段列
	 */
	static std::optional<arma::fmat> load_lut_cols(const std::filesystem::path& lut_table_path, const int band);

	/**
	 * \brief 根据列名获取列所在的索引
	 * \param lut_table_path lut表路径
	 * \param col_name 列名
	 * \return
	 */
	static int get_col_index(const std::filesystem::path& lut_table_path, const std::string& col_name);

	/**
	 * \brief 亮温转换
	 * \param bt_lut Lut表Bt列
	 * \param rad_lut Lut表Band列
	 * \param line_num Lut表行数
	 * \param rad 待转换的值
	 * \return 转换后的值
	 */
	static float calc_band_bt(const arma::fvec& bt_lut, const arma::fvec& rad_lut, const arma::uword line_num, float rad);

private:

	static const std::string MOD_LUT_TABLE_NAME;
	static const std::string MYD_LUT_TABLE_NAME;

	/**
	 * \brief 加载lut_table到内存中
	 * \param lut_table_path 文件路径
	 * \return
	 */
	static std::optional<arma::fmat> load_lut_table(const std::filesystem::path& lut_table_path);

	/**
	 * \brief 获取Lut表中BT列的列名
	 * \return
	 */
	static std::string get_bt_col_name()
	{
		return "BT";
	}

	/**
	 * \brief 获取Rad列的列名
	 * \param band
	 * \return
	 */
	static std::string get_rad_col_name(const std::string& band)
	{
		return "Band" + band;
	}
};

