#pragma once
#include <string>

namespace adsma::settings
{
	// Product Name
	const std::string BT_NAME = "BT";
	const std::string AOD_NAME = "AOD";
	const std::string WV_NAME = "WV";
	const std::string LST_NAME = "LST";

	// Product Code
	const std::string BT_CODE = "02";
	const std::string AOD_CODE = "04";
	const std::string WV_CODE = "05";
	const std::string LST_CODE = "11";

	// Product Type
	const std::string PROD_TYPE_MOD = "MOD";
	const std::string PROD_TYPE_MYD = "MYD";
	const std::string PP_STANDARD_FOLDER = "Standard";

	// Folder Suffix
	const std::string BT_FOLDER_SUFFIX = "021KM";
	const std::string SZA_FOLDER_SUFFIX = "03";
	const std::string CM_FOLDER_SUFFIX = "35_L2";
	const std::string AOD_FOLDER_SUFFIX = "04_3K";
	const std::string WV_FOLDER_SUFFIX = "05_L2";
	const std::string LST_FOLDER_SUFFIX = "11A1";
}

namespace adsma::settings::yaml
{
	const std::string START_DATE = "StartDate";
	const std::string END_DATE = "EndDate";
	const std::string WORKSPACE = "Workspace";
	const std::string TMP_PATH = "TmpPath";
	const std::string YML_FOLDER_PATH = "YmlFolderPath";
	const std::string SELECTED_PRODUCTS = "SelectedProducts";
	const std::string PREPROCESS_EXTENT = "PreprocessExtent";
}

namespace adsma::settings::yaml::preprocess
{
	const std::string PREPROCESS = "Preprocess";
	const std::string BAND = "Band";
}

namespace adsma::settings::yaml::eddy
{
	const std::string EDDY_FIELD = "EddyField";
	const std::string CALC_REF = "CalcRef";
	const std::string CALC_ANO = "CalcAno";
	const std::string ANO_METHOD = "AnoMethod";
	const std::string PLOT_BACKGROUND = "PlotBackground";
	const std::string PLOT_EDDYFIELD = "PlotEddyField";
	const std::string TITLE = "Title";
	const std::string BAR_TITLE = "BarTitle";
}

namespace adsma::settings::yaml::ga
{
	const std::string GENERAL_ANO = "GeneralAno";
}
