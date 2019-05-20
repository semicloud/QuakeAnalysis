#pragma once

#include <string>
#include <vector>

using namespace std;

namespace proc_MxD021km
{
	class Input_file
	{
	private:
		string _bt_hdf_file;
		string _sza_hdf_file;
		string _cm_hdf_file;
	public:
		Input_file(const string& bt_hdf_file, const string& sza_hdf_file, const string& cm_hdf_file);
		~Input_file();
		static vector<Input_file> load(const string& file_path);
		string bt_hdf_file() const
		{
			return _bt_hdf_file;
		}

		string sza_hdf_file() const
		{
			return _sza_hdf_file;
		}

		string cm_hdf_file() const
		{
			return _cm_hdf_file;
		}
	};

}

