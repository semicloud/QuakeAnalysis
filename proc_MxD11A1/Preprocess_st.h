#pragma once

#include <string>
#include <yaml-cpp/yaml.h>

using namespace std;

namespace proc_MxD11A1
{
	/**
	 * \brief �ر��¶ȣ�Surface Temperature��Ԥ������
	 */
	class Preprocess_st
	{
		static void check_node(const string& yml_path, const YAML::Node& node, const string& attr_name);
		static string wrap_output_projection_parameters_str(const string& old);
	public:
		Preprocess_st();
		~Preprocess_st();
		static void preprocess(const string& yml_path, const YAML::Node& node, bool debug_mode);
	};
}


