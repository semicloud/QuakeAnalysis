#pragma once
#include <string>
#include <yaml-cpp/yaml.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/date_time/local_time/dst_transition_day_rules.hpp>
#include <boost/log/trivial.hpp>

namespace adsma
{
	class yamlArgs
	{
	private:
		std::string m_hdflist_file;
		std::string m_tmp_path;
		double m_min_lon;
		double m_max_lon;
		double m_min_lat;
		double m_max_lat;
		int m_band;
		std::string m_mrt_kernel_type;
		std::string m_mrt_projection_type;
		std::string m_mrt_projection_args;
		//std::string _mrt_projection_sphere;
		std::string m_mrt_pixel_size;
		/**
		 * \brief 最终结果数据集路径
		 */
		std::string m_output_image_file;

	public:
		/**
		 * \brief 输入的HDF列表文件
		 * \return
		 */
		std::string hdflist_file() const
		{
			return m_hdflist_file;
		}

		std::string tmp_path()const
		{
			return m_tmp_path;
		}

		double min_lon() const
		{
			return m_min_lon;
		}

		double max_lon() const
		{
			return m_max_lon;
		}

		double min_lat() const
		{
			return m_min_lat;
		}

		double max_lat() const
		{
			return m_max_lat;
		}

		std::string mrt_kernel_type() const
		{
			return m_mrt_kernel_type;
		}

		std::string mrt_projection_type() const
		{
			return m_mrt_projection_type;
		}

		std::string mrt_projection_args() const
		{
			return m_mrt_projection_args;
		}

		int band() const
		{
			return m_band;
		}

		std::string mrt_pixel_size() const
		{
			return m_mrt_pixel_size;
		}

		std::string output_image_file() const
		{
			return m_output_image_file;
		}

		yamlArgs(const YAML::Node&);
		~yamlArgs();
		static void check_node(const YAML::Node&);
	};
}