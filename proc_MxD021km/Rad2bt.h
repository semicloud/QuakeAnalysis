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
		* \brief ����HDF�ļ�����ȡ��ʹ�õ�lut��·��
		 * \param path_hdf_file hdf�ļ�·��
		 * \return lut��·��
		 */
	static std::filesystem::path get_lut_table_path(const std::filesystem::path& path_hdf_file);

	/**
	 * \brief ����Lut���е�������
	 * \param lut_table_path Lut��·��
	 * \param band ������
	 * \return һ�����о��󣬵�һ����bt�У��ڶ�������Ӧ�Ĳ�����
	 */
	static std::optional<arma::fmat> load_lut_cols(const std::filesystem::path& lut_table_path, const int band);

	/**
	 * \brief ����������ȡ�����ڵ�����
	 * \param lut_table_path lut��·��
	 * \param col_name ����
	 * \return
	 */
	static int get_col_index(const std::filesystem::path& lut_table_path, const std::string& col_name);

	/**
	 * \brief ����ת��
	 * \param bt_lut Lut��Bt��
	 * \param rad_lut Lut��Band��
	 * \param line_num Lut������
	 * \param rad ��ת����ֵ
	 * \return ת�����ֵ
	 */
	static float calc_band_bt(const arma::fvec& bt_lut, const arma::fvec& rad_lut, const arma::uword line_num, float rad);

private:

	static const std::string MOD_LUT_TABLE_NAME;
	static const std::string MYD_LUT_TABLE_NAME;

	/**
	 * \brief ����lut_table���ڴ���
	 * \param lut_table_path �ļ�·��
	 * \return
	 */
	static std::optional<arma::fmat> load_lut_table(const std::filesystem::path& lut_table_path);

	/**
	 * \brief ��ȡLut����BT�е�����
	 * \return
	 */
	static std::string get_bt_col_name()
	{
		return "BT";
	}

	/**
	 * \brief ��ȡRad�е�����
	 * \param band
	 * \return
	 */
	static std::string get_rad_col_name(const std::string& band)
	{
		return "Band" + band;
	}
};

