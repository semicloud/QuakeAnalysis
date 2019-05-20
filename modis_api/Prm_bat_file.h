#pragma once

#include <string>

using namespace std;
namespace modis_api
{
	/**
	 * \brief 用于HDF2GTIFF的CMD脚本
	 */
	class __declspec(dllexport) Mrt_swath_prm_file_run
	{
	private:
		string _mrt_swath_path;
		string _prm_file_path;
	public:
		/**
		 * \brief
		 * \param mrt_swath_path 本地安装的MRT路径，例如C:\DevLib\MRTSwath
		 * \param prm_file_path prm文件路径
		 */
		Mrt_swath_prm_file_run(const string& mrt_swath_path, const string& prm_file_path);
		~Mrt_swath_prm_file_run();


		/**
		 * \brief 本地安装的MRT路径，例如C:\DevLib\MRTSwath
		 * \return
		 */
		string get_mrt_swath_path() const
		{
			return _mrt_swath_path;
		}

		/**
		 * \brief prm文件路径
		 * \return
		 */
		string get_prm_file_path() const
		{
			return _prm_file_path;
		}

		string get_prm_bat_content();

		/**
		 * \brief 将运行Prm文件的bat脚本保存到指定位置
		 * \param dest
		 */
		void export_prm_bat_file(const string& dest);
	};
}


