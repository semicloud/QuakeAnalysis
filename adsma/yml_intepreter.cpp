#include "pch.h"
#include "yml_intepreter.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <optional>
#include <filesystem>

int process(const std::string& yml_path_str)
{
	using namespace std;
	using namespace std::filesystem;
	using namespace boost;
	using namespace YAML;

	const path yml_path(yml_path_str);
	if (std::optional<Node> node_optional = load_yml(yml_path.string()))
	{
		const Node node = node_optional.value();
		const path workspace_path = node["Workspace"].as<string>();
		BOOST_ASSERT_MSG(exists(workspace_path) && is_directory(workspace_path),
			str(format("workspace path %1% is illegal!") % workspace_path).c_str());
		const path tmp_path = node["TmpPath"].as<string>();
		BOOST_ASSERT_MSG(exists(tmp_path) && is_directory(tmp_path),
			str(format("tmp path %1% is illegal!") % tmp_path).c_str());
		
		const vector<string> products = node["SelectedProducts"].as<vector<string>>();
		BOOST_ASSERT_MSG(!products.empty(), "No selected products found!");

		BOOST_LOG_TRIVIAL(debug) << "load " << products.size() << " products";
		BOOST_LOG_TRIVIAL(debug) << join(products, ",");


	}



	return EXIT_SUCCESS;
}
