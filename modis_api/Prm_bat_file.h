#pragma once

#include <string>

using namespace std;
namespace modis_api
{
	/**
	 * \brief ����HDF2GTIFF��CMD�ű�
	 */
	class __declspec(dllexport) Mrt_swath_prm_file_run
	{
	private:
		string _mrt_swath_path;
		string _prm_file_path;
	public:
		/**
		 * \brief
		 * \param mrt_swath_path ���ذ�װ��MRT·��������C:\DevLib\MRTSwath
		 * \param prm_file_path prm�ļ�·��
		 */
		Mrt_swath_prm_file_run(const string& mrt_swath_path, const string& prm_file_path);
		~Mrt_swath_prm_file_run();


		/**
		 * \brief ���ذ�װ��MRT·��������C:\DevLib\MRTSwath
		 * \return
		 */
		string get_mrt_swath_path() const
		{
			return _mrt_swath_path;
		}

		/**
		 * \brief prm�ļ�·��
		 * \return
		 */
		string get_prm_file_path() const
		{
			return _prm_file_path;
		}

		string get_prm_bat_content();

		/**
		 * \brief ������Prm�ļ���bat�ű����浽ָ��λ��
		 * \param dest
		 */
		void export_prm_bat_file(const string& dest);
	};
}


