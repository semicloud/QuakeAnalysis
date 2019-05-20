#pragma once
#include <string>
#include <boost/format.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <vector>

/**
 * \brief 涡度输入参数解析类
 */
class Eddy_field_options_yaml
{
private:
	std::string _yml_path;

	std::string _modis_workspace_folder;
	/**
	 * \brief 是否需要计算背景场
	 */
	bool _need_compute_ref;
	/**
	 * \brief 是否需要计算涡度
	 */
	bool _need_compute_eddy_field;
	/**
	 * \brief 方法，可以选择方法1或方法2
	 */
	int _method;
	/**
	 * \brief 日期字符串
	 */
	//std::string _dateStr;
	/**
	 * \brief 数据类型，BT、CM等，需要该字段配合Method字段确定存储的路径
	 */
	//std::string _data;
	/**
	 * \brief 数据类型 MOD或MYD
	 */
	//std::string _type;
	/**
	 * \brief 用于计算背景场的tif文件列表所在的txt文件路径
	 */
	std::string _support_ref_tif_file;
	/**
	 * \brief 输入Tif文件，即要进行涡度计算的Tif文件
	 */
	std::string _input_image_file;
	/**
	 * \brief 生成的背景场文件路径
	 */
	std::string _ref_image_file;
	/**
	 * \brief 生成的涡度文件路径
	 */
	std::string _output_eddy_field_image_file;

public:
	Eddy_field_options_yaml(std::string&);
	~Eddy_field_options_yaml();

	/**
	 * \brief 背景场文件路径
	 * \return 
	 */
	std::string ref_image_file() const
	{
		return _ref_image_file;
	}

	/**
	 * \brief 输出涡度文件路径
	 * \return 
	 */
	std::string output_eddy_field_image_file() const
	{
		return _output_eddy_field_image_file;
	}

	/**
	 * \brief 输入tif文件路径
	 * \return 
	 */
	std::string input_image_file() const
	{
		return _input_image_file;
	}

	/**
	 * \brief MODIS工作空间
	 * \return 
	 */
	std::string modis_workspace_folder() const
	{
		return _modis_workspace_folder;
	}

	/**
	 * \brief 是否生成背景场
	 * \return 
	 */
	bool need_compute_ref() const
	{
		return _need_compute_ref;
	}

	/**
	 * \brief 是否计算涡度
	 * \return 
	 */
	bool need_compute_eddy_field() const
	{
		return _need_compute_eddy_field;
	}

	/**
	 * \brief 计算方法，1或者2
	 * \return 
	 */
	int method() const
	{
		return _method;
	}

	/*std::string date_str() const
	{
		return _dateStr;
	}

	std::string data() const
	{
		return _data;
	}

	std::string year() const
	{
		return _dateStr.substr(0, 4);
	}

	std::string day() const
	{
		return _dateStr.substr(4, 3);
	}

	std::string type() const
	{
		return _type;
	}*/

	std::string support_ref_tif_file() const
	{
		return _support_ref_tif_file;
	}
	
	/**
	 * \brief 获取用于计算背景场的tif文件路径列表（该路径文件列表来自于_support_ref_tif_file）
	 * \return
	 */
	std::vector<std::string> get_ref_tif_file_list();
};

