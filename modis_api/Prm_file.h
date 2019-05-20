#pragma once
#include <string>
#include <iostream>
#include "Prm_setting.h"

using namespace std;

namespace modis_api
{
	/**
	 * \brief 该类主要用于处理swath2grid.exe使用的prm配置文件
	 * \brief 包括生成Prm文件字符串、导出Prm文件至指定目录等
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


