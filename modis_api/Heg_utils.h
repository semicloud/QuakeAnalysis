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
		 * \brief ��ȡheg��.prm�ļ�ģ�壬���ٴ�templatesĿ¼�µ�ģ���ļ���ȡ
		 * \return hegģ���ļ�
		 */
		static std::string get_heg_prm_template();

		/**
		 * \brief ��ȡ����heg��bat�ű�ģ�壬���ٴ�templateĿ¼�µ�ģ���ļ���ȡ
		 * \return heg batģ���ļ�
		 */
		static std::string get_heg_bat_template();
	public:
		Heg_utils();
		~Heg_utils();
		static void run_heg(Heg_param const& hp);
	};
}


