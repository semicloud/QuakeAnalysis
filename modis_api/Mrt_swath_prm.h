#pragma once
#include <string>
#include <iostream>
#include "Mrt_swath_prm_setting.h"

namespace modis_api
{
	/**
	 * \brief ������Ҫ���ڴ���swath2grid.exeʹ�õ�prm�����ļ�
	 * \brief ��������Prm�ļ��ַ���������Prm�ļ���ָ��Ŀ¼��
	 */
	class __declspec(dllexport) Mrt_swath_prm
	{
	private:
		Mrt_swath_prm_setting	_prm_setting;

	public:
		Mrt_swath_prm(const Mrt_swath_prm_setting& prm_setting)
		{
			_prm_setting = prm_setting;
		}

		~Mrt_swath_prm();

		std::string get_prm_file_content();
		void export_prm_file(const std::string& dest);
	};
}


