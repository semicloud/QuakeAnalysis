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
	 * \brief ����lut_table���ڴ���
	 * \param lut_path �ļ�·��
	 * \return
	 */
	static boost::optional<arma::fmat> load_lut_table(const std::string& lut_path);

	/**
	 * \brief ����������ȡ�����ڵ�����
	 * \param lut_path lut��·��
	 * \param col_name ����
	 * \return
	 */
	static int get_col_index(const std::string& lut_path, const std::string& col_name);

	static float calc_band_bt(const float bt_lut[], const float rad_lut[], const arma::uword line_num, float rad);

	/**
	 * \brief ��ȡLut����BT�е�����
	 * \return 
	 */
	inline static std::string get_bt_col_name()
	{
		return "BT";
	}

	/**
	 * \brief ��ȡRad�е����� 
	 * \param band 
	 * \return 
	 */
	inline static std::string get_rad_col_name(const std::string& band)
	{
		return "Band" + band;
	}
};

