#pragma once
#include <boost/date_time.hpp>
#include <filesystem>

namespace adsma
{
	namespace interpreter
	{
		namespace preprocess
		{
			namespace bt
			{
				/**
				* \brief ׼������Ԥ��������yml�ļ���hdflist�ļ�
				* \param workspace_path
				* \param tmp_path
				* \param date_start
				* \param date_end
				* \param product_type
				* \param pp_min_lon
				* \param pp_max_lon
				* \param pp_min_lat
				* \param pp_max_lat
				* \param band
				* \param mrt_kernel_type
				* \param mrt_projection_type
				* \param mrt_projection_args
				* \param mrt_pixel_size
				* \param yml_folder_path
				* \return
				*/
				int generate_pp_bt_yml_hdflist_files(
					const std::filesystem::path& workspace_path,
					const std::filesystem::path& tmp_path,
					const boost::gregorian::date& date_start,
					const boost::gregorian::date& date_end,
					const std::string& product_type,
					float pp_min_lon, float pp_max_lon, float pp_min_lat, float pp_max_lat,
					int band,
					const std::string& mrt_kernel_type,
					const std::string& mrt_projection_type,
					const std::string& mrt_projection_args,
					float mrt_pixel_size,
					const std::filesystem::path& yml_folder_path
				);

				/**
				 * \brief ��������Ԥ����Yml
				 * \param hdf_list_file_path HDF�б��ļ�·��
				 * \param tmp_path tmpĿ¼·��
				 * \param min_lon
				 * \param max_lon
				 * \param min_lat
				 * \param max_lat
				 * \param band ����
				 * \param mrt_kernel_type
				 * \param mrt_projection_type
				 * \param mrt_projection_args
				 * \param mrt_pixel_size
				 * \param output_image_path
				 * \return
				 */
				std::string get_preprocess_bt_yml_str(
					const std::filesystem::path& hdf_list_file_path,
					const std::filesystem::path& tmp_path,
					float min_lon, float max_lon,
					float min_lat, float max_lat,
					int band,
					const std::string& mrt_kernel_type,
					const std::string& mrt_projection_type,
					const std::string& mrt_projection_args,
					float mrt_pixel_size,
					const std::filesystem::path& output_image_path
				);

				/**
				 * \brief ��������Ԥ����HDF�ļ��б��ַ���
				 * \param workspace_path �����ռ�
				 * \param product_type ��Ʒ���MOD��MYD
				 * \param date ����
				 * \return ����Ԥ����HDF�ļ��б��ַ���
				 */
				std::string get_preprocess_bt_hdf_list_str(
					const std::filesystem::path& workspace_path,
					const std::string& product_type, const boost::gregorian::date& date);

			}
		}
	}
}


