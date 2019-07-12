#pragma once

#include <string>
#include <vector>

namespace adsma
{
	class Input_file
	{
	private:
		std::string _bt_hdf_file;
		std::string _sza_hdf_file;
		std::string _cm_hdf_file;
	public:
		Input_file(const std::string& bt_hdf_file, const std::string& sza_hdf_file, const std::string& cm_hdf_file);
		~Input_file();
		static std::vector<Input_file> load(const std::string& file_path);
		std::string bt_hdf_file() const
		{
			return _bt_hdf_file;
		}

		std::string sza_hdf_file() const
		{
			return _sza_hdf_file;
		}

		std::string cm_hdf_file() const
		{
			return _cm_hdf_file;
		}
	};

}

