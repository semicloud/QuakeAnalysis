#pragma once
#include <filesystem>
#include <string>
#include <vector>

/**
 * \brief �ж��������������
 */
class Eddy_field_options_yaml {
public:
	Eddy_field_options_yaml(std::string&);
	~Eddy_field_options_yaml();

	// �������ļ�·��
	std::filesystem::path ref_image_file() const
	{
		return m_ref_image_file;
	}

	// �쳣�ļ�·��
	std::filesystem::path output_ano_file() const
	{
		return m_output_ano_file;
	}

	// �����ļ�·��
	std::filesystem::path input_image_file() const
	{
		return m_input_image_file;
	}

	// �����ռ�·��
	std::filesystem::path workspace() const
	{
		return m_workspace;
	}

	// �Ƿ���㱳����
	bool calc_ref() const
	{
		return m_calc_ref;
	}

	// �Ƿ�����ж�
	bool calc_ano() const
	{
		return m_calc_ano;
	}

	// ���㷽����1����2
	int method() const
	{
		return m_ano_method;
	}

	// ���㱳������Ҫ��tiflist�ļ�·�������ļ���һ��txt�ļ���ʹ�ø��ļ���ָ����tif�ļ����ɱ�����
	std::filesystem::path ref_list_file() const
	{
		return m_ref_list_file;
	}

	// ��ȡ���㱳��������Ҫ��֧���ļ�
	std::vector<std::filesystem::path> get_tif_files_for_ref_computing();

private:
	// ����yml�ļ�·��
	std::string m_yml;
	// �����ռ�·��
	std::string m_workspace;
	// �Ƿ���㱳����
	bool m_calc_ref = false;
	// �Ƿ�����쳣
	bool m_calc_ano = false;
	// ���㷽����1����2
	int m_ano_method = 1;
	// ���㱳������Ҫ��tiflist�ļ�·�������ļ���һ��txt�ļ���ʹ�ø��ļ���ָ����tif�ļ����ɱ�����
	std::string m_ref_list_file;
	// �����ļ�·��
	std::string m_input_image_file;
	// �������ļ�·��
	std::string m_ref_image_file;
	// �쳣�ļ�·��
	std::string m_output_ano_file;
};
