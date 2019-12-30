#pragma once

#include <gdal.h>
#include <gdal_priv.h>

namespace gdal_lib
{
	__declspec(dllexport) std::string __cdecl proj4_to_wkt(std::string const& proj4str);

	__declspec(dllexport) std::string __cdecl wkt_to_proj4(std::string const& wkt);

	inline int str2int(std::string const& s)
	{
		static std::map<std::string, int> m{ {"int",0},{"unsigned int",1},{"float",2} };
		return m.find(s) != m.end() ? m.find(s)->second : -1;
	}

	inline std::string get_wgs84_proj() { return "+proj=longlat +datum=WGS84 +no_defs"; }

	inline std::shared_ptr<double> get_default_geo_trans() { return std::shared_ptr<double>{new double[6]{ -179.5, 5.0, 0.0, 89.5, 0.0, -2.5 }, std::default_delete<double[]>()}; }

	inline  std::map<std::string, std::string> tif_options_for_grey() { return  { {"COMPRESS", "LZW"}, { "INTERLEAVE","BAND" } }; }

	inline  std::map<std::string, std::string> tif_options_for_rgb() { return  { {"COMPRESS", "LZW"}, { "INTERLEAVE","BAND" },{"PHOTOMETRIC","RGB"} }; }

	template<typename T> GDALDataType get_gdal_data_type()
	{
		switch (str2int(typeid(T).name()))
		{
		case 0:
			return GDT_Int32;
		case 1:
			return GDT_UInt32;
		case 2:
			return GDT_Float32;
		default:
			//BOOST_LOG_TRIVIAL(warning) << typeid(T) << " doesn't find a mapping type in GDAL.";
			return GDT_Unknown;
		}
	}

	template <typename T> int create_tif(std::string const& path, T* data, T no_data_value,
		size_t xs, size_t ys, size_t bn,
		std::string const& proj, double* geo_trans, std::map<std::string, std::string> options)
	{
		GDALRegister_GTiff();
		GDALDriver* driver{ GetGDALDriverManager()->GetDriverByName("GTiff") };
		if (!driver) throw std::runtime_error("load driver failed!");
		char **papsz_options = nullptr;
		for (auto p = options.cbegin(); p != options.cend(); ++p)
			papsz_options = CSLSetNameValue(papsz_options, p->first.c_str(), p->second.c_str());
		auto to_i = [](auto d) {return int(d);  };
		GDALDataset* ds = driver->Create(path.c_str(), to_i(xs), to_i(ys), to_i(bn),
			get_gdal_data_type<T>(), papsz_options);
		if (!ds) throw std::runtime_error("create dataset failed!");
		ds->RasterIO(GF_Write, 0, 0, to_i(xs), to_i(ys), data, to_i(xs), to_i(ys), get_gdal_data_type<T>(), to_i(bn), nullptr, 0, 0, 0);
		ds->SetProjection(proj4_to_wkt(proj).c_str());
		ds->SetGeoTransform(geo_trans);
		for (size_t i = 1; i != bn + 1; ++i)
		{
			ds->GetRasterBand(to_i(i))->SetColorInterpretation(GCI_GrayIndex);
			GDALSetRasterNoDataValue(ds->GetRasterBand(to_i(i)), static_cast<double>(no_data_value));
		}
		GDALClose(ds);
		return 0;
	}

	__declspec(dllexport) int __cdecl read_tif(std::string const& path, std::vector<float>& data, size_t& xs, size_t&ys, size_t& bn, double& ndv);

	__declspec(dllexport) double __cdecl read_ndv(std::string const& file);
	__declspec(dllexport) int __cdecl set_ndv(std::string const& file, double ndv);

	__declspec(dllexport) std::string __cdecl read_proj(std::string const& file);

	__declspec(dllexport) std::string __cdecl read_metadata(std::string const& file, std::string const& mn);
}
