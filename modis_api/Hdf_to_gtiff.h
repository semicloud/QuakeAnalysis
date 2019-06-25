#pragma once
#include "Mrt_swath_prm_setting.h"
#include "Mrt_swath_prm.h"
#include <string>

using namespace modis_api;

namespace modis_api
{
	class __declspec(dllexport) Hdf_to_gtiff
	{
	private:
		Mrt_swath_prm_setting* _prm_setting;
		Mrt_swath_prm* _prm_file;
		std::string _convert_at;

	public:
		Hdf_to_gtiff() {}
		Hdf_to_gtiff(Mrt_swath_prm_setting* prm_setting, std::string convert_at) : _prm_setting(prm_setting), _convert_at(convert_at)
		{
			_prm_file = new Mrt_swath_prm(*_prm_setting);
		}

		~Hdf_to_gtiff()
		{
			delete _prm_file;
		}

		void do_convert();
	};

}
