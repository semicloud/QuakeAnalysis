#pragma once
#include <string>
#include <iostream>
#include "Prm_setting.h"

using namespace std;

namespace modis_api
{
	/**
	 * \brief ������Ҫ���ڴ���swath2grid.exeʹ�õ�prm�����ļ�
	 * \brief ��������Prm�ļ��ַ���������Prm�ļ���ָ��Ŀ¼��
	 */
	class __declspec(dllexport) Mrt_swath_prm_file
	{
	private:
		Prm_setting	_prm_setting;

	public:
		Mrt_swath_prm_file(const Prm_setting& prm_setting)
		{
			_prm_setting = prm_setting;
		}

		~Mrt_swath_prm_file();

		string get_prm_file_content();
		void export_prm_file(const string& dest);
	};
}


