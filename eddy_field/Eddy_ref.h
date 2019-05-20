#pragma once
#include "Eddy_field_options_yaml.h"
#include <string>
#include <vector>

/**
 * \brief �жȱ���������������1�ͷ���2
 */
class Eddy_ref
{
private:
	static std::vector<std::string> get_tif_file_list_for_ref(const Eddy_field_options_yaml&, const std::string&);

public:
	Eddy_ref();
	~Eddy_ref();

	/**
	 * \brief ʹ�÷���1�����жȱ�����
	 * \param options ���ö���
	 */
	static void compute_eddy_ref_m1(Eddy_field_options_yaml& options);

	/**
	 * \brief ʹ�÷���2�����жȱ�����
	 * \param options ���ö���
	 */
	static void compute_eddy_ref_m2(Eddy_field_options_yaml& options);

};

