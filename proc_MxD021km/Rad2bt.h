#pragma once
#include <string>
#include <armadillo>
#include <boost/optional.hpp>

class Rad2bt
{
public:
	Rad2bt();
	~Rad2bt();

	/**
	 * \brief 加载lut_table到内存中
	 * \param lut_path 文件路径
	 * \return
	 */
	static boost::optional<arma::fmat> load_lut_table(const std::string& lut_path);

	/**
	 * \brief 根据列名获取列所在的索引
	 * \param lut_path lut表路径
	 * \param col_name 列名
	 * \return
	 */
	static int get_col_index(const std::string& lut_path, const std::string& col_name);

	static float calc_band_bt(const float bt_lut[], const float rad_lut[], const arma::uword line_num, float rad);

	/**
	 * \brief 获取Lut表中BT列的列名
	 * \return 
	 */
	inline static std::string get_bt_col_name()
	{
		return "BT";
	}

	/**
	 * \brief 获取Rad列的列名 
	 * \param band 
	 * \return 
	 */
	inline static std::string get_rad_col_name(const std::string& band)
	{
		return "Band" + band;
	}
};

