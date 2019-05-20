#pragma once
#include <string>
#include <yaml-cpp/yaml.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/date_time/local_time/dst_transition_day_rules.hpp>
#include <boost/log/trivial.hpp>

namespace proc_MxD021km
{
	using namespace  std;

	class Options_yaml
	{
	private:
		string _input_hdf_file;
		string _temp_dir;
		double _min_lon;
		double _max_lon;
		double _min_lat;
		double _max_lat;
		std::string _band;
		std::string _mrt_kernel_type;
		std::string _mrt_projection_type;
		std::string _mrt_projection_args;
		//std::string _mrt_projection_sphere;
		std::string _mrt_pixel_size;
		/**
		 * \brief 最终结果数据集路径
		 */
		std::string _output_image_file;

	public:
		/**
		 * \brief 输入的HDF列表文件
		 * \return
		 */
		string input_hdf_file() const
		{
			return _input_hdf_file;
		}

		string temp_dir()const
		{
			return _temp_dir;
		}

		double min_lon() const
		{
			return _min_lon;
		}

		double max_lon() const
		{
			return _max_lon;
		}

		double min_lat() const
		{
			return _min_lat;
		}

		double max_lat() const
		{
			return _max_lat;
		}

		std::string mrt_kernel_type() const
		{
			return _mrt_kernel_type;
		}

		std::string mrt_projection_type() const
		{
			return _mrt_projection_type;
		}

		std::string mrt_projection_args() const
		{
			return _mrt_projection_args;
		}

		std::string band() const
		{
			return _band;
		}

		// std::string mrt_projection_sphere() const
		// {
		// 	return _mrt_projection_sphere;
		// }

		std::string mrt_pixel_size() const
		{
			return _mrt_pixel_size;
		}

		std::string output_image_file() const
		{
			return _output_image_file;
		}

		Options_yaml(const YAML::Node&);
		~Options_yaml();
		static void check_node(const YAML::Node&);
	};
}



