#pragma once
#include <yaml-cpp/yaml.h>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/format.hpp>
#include <optional>
#include <string>
#include <filesystem>

#pragma region Yaml�ֶ�������

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

#pragma  region  �����ó���

/**
 * \brief Ԥ����Standard�ļ�������
 */
const std::string PP_STANDARD_FOLDER = "Standard";
/**
 * \brief ��Ʒ����MOD
 */
const std::string PROD_TYPE_MOD = "MOD";
/**
 * \brief ��Ʒ����MYD
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
 * \brief ��Node�н�������ʼ���ںͽ�������
 * \param node
 * \return
 */
std::vector<boost::gregorian::date> parse_date(const YAML::Node& node);

int process(const std::string& yml_path_str);

#pragma region Ԥ�������

/**
 * \brief ׼���ر��¶ȵ�Ԥ��������yml�ļ���hdflist�ļ�
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
 * \brief ׼������Ĥ��AOD����ˮ����WV����Ԥ��������yml�ļ���hdflist�ļ�
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
 * \brief ׼������Ԥ��������yml�ļ���hdflist�ļ�
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
 * \brief ��������Ĥ��ˮ����Ԥ����HDF�ļ��б�
 * \param workspace_path �����ռ�
 * \param product_code ��Ʒ���룬ֻ����04��05
 * \param product_type ��Ʒ���ֻ����MOD��MYD
 * \param date
 * \return
 */
std::string get_preprocess_aod_wv_hdf_list_str(
	const std::filesystem::path& workspace_path,
	const std::string& product_code,
	const std::string& product_type,
	const boost::gregorian::date& date);

/**
 * \brief ��������Ĥ��ˮ����Ԥ����Yml
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
 * \brief ���ɵر��¶�Ԥ����HDF�ļ��б��ַ���
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
 * \brief ���ɵر��¶ȵ�yml
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
 * \brief ��������Ԥ����HDF�ļ��б��ַ���
 * \param workspace_path �����ռ�
 * \param product_type ��Ʒ���MOD��MYD
 * \param date ����
 * \return ����Ԥ����HDF�ļ��б��ַ���
 */
std::string get_preprocess_bt_hdf_list_str(
	const std::filesystem::path& workspace_path,
	const std::string& product_type, const boost::gregorian::date& date);

/**
 * \brief ��������Ԥ����Yml
 * \param hdf_list_file_path HDF�б��ļ�·��
 * \param tmp_path tmpĿ¼·��
 * \param min_lon
 * \param max_lon
 * \param min_lat
 * \param max_lat
 * \param band ����
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
 * \brief ��ȡԤ���������Ŀ¼�����֣�
 * \param product_type ��Ʒ���ͣ�MOD��MYD
 * \param product_name ��Ʒ����
 * \return Ԥ���������Ŀ¼�����֣�
 */
inline std::string get_pp_folder(const std::string& product_type, const std::string& product_name)
{
	return (boost::format("%1%_%2%") % product_type % product_name).str();
}

/**
 * \brief ��ȡԤ��������ļ���
 * \param product_name ��Ʒ����
 * \param year ���
 * \param day DOY
 * \return Ԥ��������ļ���
 */
inline std::string get_pp_output_file_name(const std::string& product_name, const std::string& year, const std::string& day)
{
	return (boost::format("%1%_%2%_%3%.tif") % product_name % year % day).str();
}

#pragma endregion Ԥ�������

#pragma region �ж����

/**
 * \brief �����ж�Yml�ļ��Լ�������������Ҫ��HdfList�ļ�
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
 * \brief �����ж�Yml�ַ���
 * \param workspace_path MODIS�����ռ�Ŀ¼
 * \param tmp_path
 * \param calc_ref �Ƿ���㱳����
 * \param calc_ano �Ƿ�����ж�
 * \param ano_method �������1����2
 * \param input_image_file_path ����Tif�ļ�����Ҫ�����жȼ����Tif�ļ�
 * \param ref_list_file_path �������ɱ�������Tif�б��ļ�
 * \param ref_image_file_path �������ļ�·����������ɱ������������ɵı��������浽����ļ�����������ɱ����������ø��ļ���Ϊ������
 * \param output_ano_file_path ���ɵ��ж��ļ�·��
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
 * \brief ��ȡ�жȱ�����hdflist�ַ���
 * \param workspace_path
 * \param tmp_path
 * \param product
 * \param product_type
 * \param ano_method
 * \param date
 * \param years ���������жȱ���������ݣ���date�����֮ǰ�����
 * \return �жȱ�����hdflist�ַ���
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



#pragma endregion �ж����

/**
 * \brief ������С�����γ��
 * \param lon_lat_str ��γ���ַ���
 * \param out_min_lon ��С���ȣ����
 * \param out_max_lon ��󾭶ȣ����
 * \param out_min_lat ��Сγ�ȣ����
 * \param out_max_lat ���γ�ȣ����
 * \return
 */
int split_lonlat_str(const std::string& lon_lat_str, float& out_min_lon,
	float& out_max_lon, float& out_min_lat, float& out_max_lat);

/**
 * \brief ��unordered map����Ϊyml str
 * \param umap
 * \return
 */
std::string get_yml_str(const std::unordered_map<std::string, std::string>& umap);

/**
 * \brief ��ش�Ԥ���������Ƿ��Ѵ���
 * \param workspace �����ռ�Ŀ¼
 * \param product ��Ʒ��Ҫ��ʹ�ò�Ʒȫ�ƣ���MOD11A1
 * \param year_and_day ��ݼ������ַ���
 * \return ���ݴ��ڷ���true�������ڷ���false
 */
bool is_data_exist(const std::filesystem::path& workspace,
	const std::string& product, const std::string& year_and_day);

/**
 * \brief ��ȡһ����Ʒ��Ԥ����������ݼ���
 * \param workspace_path �����ռ�Ŀ¼
 * \param product_type ��Ʒ���ͣ�MOD��MYD
 * \param product ��Ʒ���ƣ�����[BT, AOD, WV, LST]
 * \return �ò�Ʒ��Ԥ����������ݼ��ϣ���������
 */
std::vector<unsigned int> get_years_of_product(
	const std::filesystem::path& workspace_path,
	const std::string& product_type,
	const std::string& product
);