#pragma once
#include <yaml-cpp/yaml.h>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/format.hpp>
#include <optional>
#include <string>
#include <filesystem>

#pragma region Yaml字段名常量

const std::string BT_NAME = "BT";
const std::string AOD_NAME = "AOD";
const std::string WV_NAME = "WV";
const std::string LST_NAME = "LST";
const std::string BT_CODE = "02";
const std::string AOD_CODE = "04";
const std::string WV_CODE = "05";
const std::string LST_CODE = "11";

const std::string PREPROCESS = "Preprocess";
const std::string BAND = "Band";

const std::string EDDY_FIELD = "EddyField";
const std::string CALC_REF = "CalcRef";
const std::string CALC_ANO = "CalcAno";
const std::string ANO_METHOD = "AnoMethod";
const std::string PLOT_BACKGROUND = "PlotBackground";
const std::string PLOT_EDDYFIELD = "PlotEddyField";
const std::string TITLE = "Title";
const std::string BAR_TITLE = "BarTitle";

const std::string START_DATE = "StartDate";
const std::string END_DATE = "EndDate";
const std::string WORKSPACE = "Workspace";
const std::string TMP_PATH = "TmpPath";
const std::string YML_FOLDER_PATH = "YmlFolderPath";
const std::string SELECTED_PRODUCTS = "SelectedProducts";
const std::string PREPROCESS_EXTENT = "PreprocessExtent";

#pragma endregion

#pragma  region  程序用常量

/**
 * \brief 预处理Standard文件夹名称
 */
const std::string PP_STANDARD_FOLDER = "Standard";
/**
 * \brief 产品类型MOD
 */
const std::string PROD_TYPE_MOD = "MOD";
/**
 * \brief 产品类型MYD
 */
const std::string PROD_TYPE_MYD = "MYD";

const std::string BT_FOLDER_SUFFIX = "021KM";
const std::string SZA_FOLDER_SUFFIX = "03";
const std::string CM_FOLDER_SUFFIX = "35_L2";
const std::string AOD_FOLDER_SUFFIX = "04_3K";
const std::string WV_FOLDER_SUFFIX = "05_L2";
const std::string LST_FOLDER_SUFFIX = "11A1";

#pragma  endregion

std::optional<YAML::Node> load_yml(const std::string& yml_path_str);

/**
 * \brief 从Node中解析出开始日期和结束日期
 * \param node
 * \return
 */
std::vector<boost::gregorian::date> parse_date(const YAML::Node& node);

int process(const std::string& yml_path_str);

#pragma region 预处理相关

/**
 * \brief 准备地表温度的预处理，生成yml文件和hdflist文件
 * \param workspace_path
 * \param tmp_path
 * \param date_start
 * \param date_end
 * \param m_product
 * \param pp_min_lon
 * \param pp_max_lon
 * \param pp_min_lat
 * \param pp_max_lat
 * \param resampling_type
 * \param output_projection_type
 * \param output_projection_parameters
 * \param output_pixel_size
 * \param yml_folder_path
 * \return
 */
int generate_pp_lst_yml_hdflist_files(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const boost::gregorian::date& date_start,
	const boost::gregorian::date& date_end,
	const std::string& product_type,
	float pp_min_lon, float pp_max_lon, float pp_min_lat, float pp_max_lat,
	const std::string& resampling_type,
	const std::string& output_projection_type,
	const std::string& output_projection_parameters,
	const float output_pixel_size,
	const std::filesystem::path& yml_folder_path
);

/**
 * \brief 准备云掩膜（AOD）和水汽（WV）的预处理，生成yml文件和hdflist文件
 * \param workspace_path
 * \param tmp_path
 * \param date_start
 * \param date_end
 * \param m_product
 * \param pp_min_lon
 * \param pp_max_lon
 * \param pp_min_lat
prepare_aod_or_wv * \param pp_max_lat
 * \param resampling_type
 * \param output_projection_type
 * \param output_projection_parameters
 * \param yml_folder_path
 * \return
 */
int generate_pp_aod_or_wv_yml_hdflist_files(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const boost::gregorian::date& date_start,
	const boost::gregorian::date& date_end,
	const std::string& m_product,
	float pp_min_lon, float pp_max_lon, float pp_min_lat, float pp_max_lat,
	const std::string& resampling_type,
	const std::string& output_projection_type,
	const std::string& output_projection_parameters,
	const std::filesystem::path& yml_folder_path
);

/**
 * \brief 准备亮温预处理，生成yml文件和hdflist文件
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
 * \brief 生成云掩膜和水汽的预处理HDF文件列表
 * \param workspace_path 工作空间
 * \param product_code 产品代码，只能是04或05
 * \param product_type 产品类别，只能是MOD或MYD
 * \param date
 * \return
 */
std::string get_preprocess_aod_wv_hdf_list_str(
	const std::filesystem::path& workspace_path,
	const std::string& product_code,
	const std::string& product_type,
	const boost::gregorian::date& date);

/**
 * \brief 生成云掩膜和水汽的预处理Yml
 * \param hdf_list_file_path
 * \param tmp_path
 * \param min_lon
 * \param max_lon
 * \param min_lat
 * \param max_lat
 * \param resampling_type
 * \param output_projection_type
 * \param output_projection_parameters
 * \param output_image_path
 * \return
 */
std::string get_preprocess_aod_wv_yml_str(
	const std::filesystem::path& hdf_list_file_path,
	const std::filesystem::path& tmp_path,
	float min_lon, float max_lon,
	float min_lat, float max_lat,
	const std::string& resampling_type,
	const std::string& output_projection_type,
	const std::string& output_projection_parameters,
	const std::filesystem::path& output_image_path
);

/**
 * \brief 生成地表温度预处理HDF文件列表字符串
 * \param workspace_path
 * \param product_type
 * \param date
 * \return
 */
std::string get_preprocess_lst_hdf_list_str(
	const std::filesystem::path& workspace_path,
	const std::string& product_type, const boost::gregorian::date& date
);

/**
 * \brief 生成地表温度的yml
 * \param hdf_list_file_path
 * \param tmp_path
 * \param min_lon
 * \param max_lon
 * \param min_lat
 * \param max_lat
 * \param resampling_type
 * \param output_projection_type
 * \param output_projection_parameters
 * \param output_pixel_size
 * \param output_image_path
 * \return
 */
std::string get_preprocess_lst_yml_str(
	const std::filesystem::path& hdf_list_file_path,
	const std::filesystem::path& tmp_path,
	float min_lon, float max_lon,
	float min_lat, float max_lat,
	const std::string& resampling_type,
	const std::string& output_projection_type,
	const std::string& output_projection_parameters,
	float output_pixel_size,
	const std::filesystem::path& output_image_path
);

/**
 * \brief 生成亮温预处理HDF文件列表字符串
 * \param workspace_path 工作空间
 * \param product_type 产品类别，MOD或MYD
 * \param date 日期
 * \return 亮温预处理HDF文件列表字符串
 */
std::string get_preprocess_bt_hdf_list_str(
	const std::filesystem::path& workspace_path,
	const std::string& product_type, const boost::gregorian::date& date);

/**
 * \brief 生成亮温预处理Yml
 * \param hdf_list_file_path HDF列表文件路径
 * \param tmp_path tmp目录路径
 * \param min_lon
 * \param max_lon
 * \param min_lat
 * \param max_lat
 * \param band 波段
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
 * \brief 获取预处理结果输出目录（部分）
 * \param product_type 产品类型，MOD或MYD
 * \param product_name 产品名称
 * \return 预处理结果输出目录（部分）
 */
inline std::string get_pp_folder(const std::string& product_type, const std::string& product_name)
{
	return (boost::format("%1%_%2%") % product_type % product_name).str();
}

/**
 * \brief 获取预处理输出文件名
 * \param product_name 产品名称
 * \param year 年份
 * \param day DOY
 * \return 预处理输出文件名
 */
inline std::string get_pp_output_file_name(const std::string& product_name, const std::string& year, const std::string& day)
{
	return (boost::format("%1%_%2%_%3%.tif") % product_name % year % day).str();
}

#pragma endregion 预处理相关

#pragma region 涡度相关

/**
 * \brief 生成涡度Yml文件以及背景场计算需要的HdfList文件
 * \param workspace_path
 * \param tmp_path
 * \param product
 * \param product_type
 * \param start_date
 * \param end_date
 * \param calc_ref
 * \param calc_ano
 * \param ano_method
 * \param yml_folder_path
 * \return
 */
int generate_eddyfield_yml_hdflist_files(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product,
	const std::string& product_type,
	const boost::gregorian::date& start_date,
	const boost::gregorian::date& end_date,
	bool calc_ref, bool calc_ano, int ano_method,
	const std::filesystem::path& yml_folder_path
);

/**
 * \brief 生成涡度Yml字符串
 * \param workspace_path MODIS工作空间目录
 * \param tmp_path
 * \param calc_ref 是否计算背景场
 * \param calc_ano 是否计算涡度
 * \param ano_method 方法类别，1或者2
 * \param input_image_file_path 输入Tif文件，即要进行涡度计算的Tif文件
 * \param ref_list_file_path 用于生成背景场的Tif列表文件
 * \param ref_image_file_path 背景场文件路径，如果生成背景场，则将生成的背景场保存到这个文件，如果不生成背景场，则用该文件作为背景场
 * \param output_ano_file_path 生成的涡度文件路径
 * \return
 */
std::string get_eddyfield_yml_str(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	bool calc_ref, bool calc_ano, int ano_method,
	const std::filesystem::path& input_image_file_path,
	const std::filesystem::path& ref_list_file_path,
	const std::filesystem::path& ref_image_file_path,
	const std::filesystem::path& output_ano_file_path
);

/**
 * \brief 获取涡度背景场hdflist字符串
 * \param workspace_path
 * \param tmp_path
 * \param product
 * \param product_type
 * \param ano_method
 * \param date
 * \param years 用于生成涡度背景场的年份，即date的年份之前的年份
 * \return 涡度背景场hdflist字符串
 */
std::string get_eddyfield_ref_hdflist_str(
	const std::filesystem::path& workspace_path,
	const std::filesystem::path& tmp_path,
	const std::string& product,
	const std::string& product_type,
	const boost::gregorian::date& date,
	const std::vector<unsigned>& years,
	int ano_method
);



#pragma endregion 涡度相关

/**
 * \brief 解析最小、最大经纬度
 * \param lon_lat_str 经纬度字符串
 * \param out_min_lon 最小经度，输出
 * \param out_max_lon 最大经度，输出
 * \param out_min_lat 最小纬度，输出
 * \param out_max_lat 最大纬度，输出
 * \return
 */
int split_lonlat_str(const std::string& lon_lat_str, float& out_min_lon,
	float& out_max_lon, float& out_min_lat, float& out_max_lat);

/**
 * \brief 将unordered map翻译为yml str
 * \param umap
 * \return
 */
std::string get_yml_str(const std::unordered_map<std::string, std::string>& umap);

/**
 * \brief 相关待预处理数据是否已存在
 * \param workspace 工作空间目录
 * \param product 产品，要求使用产品全称，如MOD11A1
 * \param year_and_day 年份及天数字符串
 * \return 数据存在返回true，不存在返回false
 */
bool is_data_exist(const std::filesystem::path& workspace,
	const std::string& product, const std::string& year_and_day);

/**
 * \brief 获取一个产品的预处理数据年份集合
 * \param workspace_path 工作空间目录
 * \param product_type 产品类型，MOD或MYD
 * \param product 产品名称，限于[BT, AOD, WV, LST]
 * \return 该产品的预处理数据年份集合，升序排序
 */
std::vector<unsigned int> get_years_of_product(
	const std::filesystem::path& workspace_path,
	const std::string& product_type,
	const std::string& product
);