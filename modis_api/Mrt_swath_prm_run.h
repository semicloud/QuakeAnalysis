#pragma once

#include <string>

namespace modis_api
{
	/**
	 * \brief ����HDF2GTIFF��CMD�ű�
	 */
	class __declspec(dllexport) Mrt_swath_prm_run
	{
	private:
		std::string _mrt_swath_path;
		std::string _prm_file_path;
	public:
		/**
		 * \brief
		 * \param mrt_swath_path ���ذ�װ��MRT·��������C:\DevLib\MRTSwath
		 * \param prm_file_path prm�ļ�·��
		 */
		Mrt_swath_prm_run(const std::string& mrt_swath_path, const std::string& prm_file_path);
		~Mrt_swath_prm_run();


		/**
		 * \brief ���ذ�װ��MRT·��������C:\DevLib\MRTSwath
		 * \return
		 */
		std::string get_mrt_swath_path() const
		{
			return _mrt_swath_path;
		}

		/**
		 * \brief prm�ļ�·��
		 * \return
		 */
		std::string get_prm_file_path() const
		{
			return _prm_file_path;
		}

		std::string get_prm_bat_content();

		/**
		 * \brief ������Prm�ļ���bat�ű����浽ָ��λ��
		 * \param dest
		 */
		void export_prm_bat_file(const std::string& dest);
	};
}


