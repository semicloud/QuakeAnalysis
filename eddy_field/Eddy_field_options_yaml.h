#pragma once
#include <string>
#include <boost/format.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <vector>

/**
 * \brief �ж��������������
 */
class Eddy_field_options_yaml
{
private:
	std::string _yml_path;

	std::string _modis_workspace_folder;
	/**
	 * \brief �Ƿ���Ҫ���㱳����
	 */
	bool _need_compute_ref;
	/**
	 * \brief �Ƿ���Ҫ�����ж�
	 */
	bool _need_compute_eddy_field;
	/**
	 * \brief ����������ѡ�񷽷�1�򷽷�2
	 */
	int _method;
	/**
	 * \brief �����ַ���
	 */
	//std::string _dateStr;
	/**
	 * \brief �������ͣ�BT��CM�ȣ���Ҫ���ֶ����Method�ֶ�ȷ���洢��·��
	 */
	//std::string _data;
	/**
	 * \brief �������� MOD��MYD
	 */
	//std::string _type;
	/**
	 * \brief ���ڼ��㱳������tif�ļ��б����ڵ�txt�ļ�·��
	 */
	std::string _support_ref_tif_file;
	/**
	 * \brief ����Tif�ļ�����Ҫ�����жȼ����Tif�ļ�
	 */
	std::string _input_image_file;
	/**
	 * \brief ���ɵı������ļ�·��
	 */
	std::string _ref_image_file;
	/**
	 * \brief ���ɵ��ж��ļ�·��
	 */
	std::string _output_eddy_field_image_file;

public:
	Eddy_field_options_yaml(std::string&);
	~Eddy_field_options_yaml();

	/**
	 * \brief �������ļ�·��
	 * \return 
	 */
	std::string ref_image_file() const
	{
		return _ref_image_file;
	}

	/**
	 * \brief ����ж��ļ�·��
	 * \return 
	 */
	std::string output_eddy_field_image_file() const
	{
		return _output_eddy_field_image_file;
	}

	/**
	 * \brief ����tif�ļ�·��
	 * \return 
	 */
	std::string input_image_file() const
	{
		return _input_image_file;
	}

	/**
	 * \brief MODIS�����ռ�
	 * \return 
	 */
	std::string modis_workspace_folder() const
	{
		return _modis_workspace_folder;
	}

	/**
	 * \brief �Ƿ����ɱ�����
	 * \return 
	 */
	bool need_compute_ref() const
	{
		return _need_compute_ref;
	}

	/**
	 * \brief �Ƿ�����ж�
	 * \return 
	 */
	bool need_compute_eddy_field() const
	{
		return _need_compute_eddy_field;
	}

	/**
	 * \brief ���㷽����1����2
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
	 * \brief ��ȡ���ڼ��㱳������tif�ļ�·���б���·���ļ��б�������_support_ref_tif_file��
	 * \return
	 */
	std::vector<std::string> get_ref_tif_file_list();
};

