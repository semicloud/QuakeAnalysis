#pragma once
#include "Eddy_field_options_yaml.h"
#include <string>
#include <vector>

/**
 * \brief 涡度背景场，包括方法1和方法2
 */
class Eddy_ref
{
private:
	static std::vector<std::string> get_tif_file_list_for_ref(const Eddy_field_options_yaml&, const std::string&);

public:
	Eddy_ref();
	~Eddy_ref();

	/**
	 * \brief 使用方法1生成涡度背景场
	 * \param options 配置对象
	 */
	static void compute_eddy_ref_m1(Eddy_field_options_yaml& options);

	/**
	 * \brief 使用方法2生成涡度背景场
	 * \param options 配置对象
	 */
	static void compute_eddy_ref_m2(Eddy_field_options_yaml& options);

};

