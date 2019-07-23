#pragma once
#include <filesystem>
#include <string>
#include <vector>

/**
 * \brief 涡度输入参数解析类
 */
class Eddy_field_options_yaml {
public:
	Eddy_field_options_yaml(std::string&);
	~Eddy_field_options_yaml();

	// 背景场文件路径
	std::filesystem::path ref_image_file() const
	{
		return m_ref_image_file;
	}

	// 异常文件路径
	std::filesystem::path output_ano_file() const
	{
		return m_output_ano_file;
	}

	// 输入文件路径
	std::filesystem::path input_image_file() const
	{
		return m_input_image_file;
	}

	// 工作空间路径
	std::filesystem::path workspace() const
	{
		return m_workspace;
	}

	// 是否计算背景场
	bool calc_ref() const
	{
		return m_calc_ref;
	}

	// 是否计算涡度
	bool calc_ano() const
	{
		return m_calc_ano;
	}

	// 计算方法，1或者2
	int method() const
	{
		return m_ano_method;
	}

	// 计算背景场需要的tiflist文件路径，该文件是一个txt文件，使用该文件中指定的tif文件生成背景场
	std::filesystem::path ref_list_file() const
	{
		return m_ref_list_file;
	}

	// 获取计算背景场所需要的支持文件
	std::vector<std::filesystem::path> get_tif_files_for_ref_computing();

private:
	// 输入yml文件路径
	std::string m_yml;
	// 工作空间路径
	std::string m_workspace;
	// 是否计算背景场
	bool m_calc_ref = false;
	// 是否计算异常
	bool m_calc_ano = false;
	// 计算方法，1或者2
	int m_ano_method = 1;
	// 计算背景场需要的tiflist文件路径，该文件是一个txt文件，使用该文件中指定的tif文件生成背景场
	std::string m_ref_list_file;
	// 输入文件路径
	std::string m_input_image_file;
	// 背景场文件路径
	std::string m_ref_image_file;
	// 异常文件路径
	std::string m_output_ano_file;
};
