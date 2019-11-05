#pragma once
#include <string>
#include "Heg_param.h"

namespace modis_api
{
	typedef  std::filesystem::path path;
	
	class __declspec(dllexport) Heg_utils
	{
	private:
		/**
		 * \brief 获取heg的.prm文件模板，不再从templates目录下的模板文件读取
		 * \return heg模板文件
		 */
		static std::string get_heg_prm_template();

		/**
		 * \brief 获取运行heg的bat脚本模板，不再从template目录下的模板文件读取
		 * \return heg bat模板文件
		 */
		static std::string get_heg_bat_template();
	public:
		Heg_utils();
		~Heg_utils();
		static void run_heg(Heg_param const& hp);
	};
}


