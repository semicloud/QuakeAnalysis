#include "..\\modis_api\\File_operation.h"
#include "..\\modis_api\\Gdal_operation.h"
#include "..\\modis_api\\Hdf_to_gtiff.h"
#include "..\\modis_api\\Mat_operation.h"
#include "..\\modis_api\\Mrt_swath_prm_setting.h"
#include "globals.h"
#include "Input_file.h"
#include "mxd021km_conversion.h"
#include "Rad2bt.h"
#include <boost/format.hpp>
#include <boost/format/free_funcs.hpp>
#include <filesystem>
#include <sstream>
#include <optional>

const std::string adsma::mxd021km_conversion::SUFFIX_PREPROCESSED_BT = "_preprocessed";
const std::string adsma::mxd021km_conversion::SDS_SOLAR_ZENITH_ANGLE = "SolarZenith";
const std::string adsma::mxd021km_conversion::SDS_CLOUD_MASK = "Cloud_Mask, 1";
const std::string adsma::mxd021km_conversion::SUFFIX_SZA = "_SolarZenith";
const std::string adsma::mxd021km_conversion::SUFFIX_CM = "_Cloud_Mask_b0";

const int adsma::mxd021km_conversion::NO_DATA_VALUE_BT = 65535;
const int adsma::mxd021km_conversion::NO_DATA_VALUE_SZA = -32767;
const float adsma::mxd021km_conversion::SCALE_SZA = 0.01f;
const float adsma::mxd021km_conversion::THRESHOLD_SZA = 85.0f;
const int adsma::mxd021km_conversion::THRESHOLD_CM = 6;
const float adsma::mxd021km_conversion::TOLERANCE = 1E-05f;

/**
 * \brief 支持的波段值（程序不支持30波段）
 */
const int AVAILABLE_BANDS[16] = { 20,21,22,23,24,25,27,28,29,30,31,32,33,34,35,36 };

void adsma::mxd021km_conversion::preprocess(const yamlArgs& options)
{
	using namespace arma;
	using namespace boost;
	using namespace std;
	using namespace std::filesystem;

	vector<Input_file> input_files = Input_file::load(options.hdflist_file());
	BOOST_LOG_TRIVIAL(info) << "找到" << input_files.size() << "个待处理文件";
	if (input_files.empty()) return;

	const path tmp_path(options.tmp_path());
	if (!exists(tmp_path))
		create_directory(tmp_path);
	BOOST_LOG_TRIVIAL(info) << "清空Tmp目录：" << tmp_path;
	File_operation::clear_directory(tmp_path.string());

	//预处理后的亮温tif文件列表
	vector<path> preprocessed_bt_file_paths;

	uword ROW_COUNT = 0, COLUMN_COUNT = 0;
	//lut表文件路径，目前以hdf列表的第一个hdf文件的文件类型作为加载lut表的标识
	const std::string first_hdf_path = input_files.front().mxd02_file();
	BOOST_LOG_TRIVIAL(debug) << "First hdf: " << first_hdf_path;
	const path lut_table_path = Rad2bt::get_lut_table_path(first_hdf_path);
	std::optional<fmat> lut_cols_matrix = Rad2bt::load_lut_cols(lut_table_path, options.band());
	const fvec bt_lut = lut_cols_matrix->col(0);
	const fvec rad_lut = lut_cols_matrix->col(1);
	const uword line_number = lut_cols_matrix->n_rows;

	for (const Input_file& input_file : input_files)
	{
		BOOST_LOG_TRIVIAL(info) << "开始处理文件" << input_file.mxd02_file();
		//亮温hdf文件文件名（不带扩展名）
		const std::string bt_file_without_extension = path(input_file.mxd02_file()).stem().string();
		Mrt_swath_prm_setting bt_prm_setting(input_file.mxd02_file(), input_file.mxd03_file(), get_sds_bt(options.band()),
			options.min_lon(), options.max_lat(), options.max_lon(), options.min_lat(), tmp_path.string() + bt_file_without_extension + ".tif");
		Hdf_to_gtiff bt_convert(&bt_prm_setting, tmp_path.string());
		bt_convert.do_convert();

		//太阳天顶角hdf文件文件名（不带扩展名）
		const std::string sza_file_without_extension = path(input_file.mxd03_file()).stem().string();
		Mrt_swath_prm_setting sza_prm_setting(input_file.mxd03_file(), input_file.mxd03_file(), SDS_SOLAR_ZENITH_ANGLE,
			options.min_lon(), options.max_lat(), options.max_lon(), options.min_lat(), tmp_path.string() + sza_file_without_extension + ".tif");
		Hdf_to_gtiff sza_convert(&sza_prm_setting, tmp_path.string());
		sza_convert.do_convert();

		//云掩膜hdf文件文件名（不带扩展名）
		const std::string cm_file_without_extension = path(input_file.mxd03_file()).stem().string();
		Mrt_swath_prm_setting cm_prm_setting(input_file.mxd35_file(), input_file.mxd03_file(), SDS_CLOUD_MASK,
			options.min_lon(), options.max_lat(), options.max_lon(), options.min_lat(), tmp_path.string() + cm_file_without_extension + ".tif");
		Hdf_to_gtiff cm_convert(&cm_prm_setting, tmp_path.string());
		cm_convert.do_convert();

#pragma region 检查tif文件是否存在

		//使用MRT提取的亮温tif文件路径
		//const std::string bt_tif_file_path = str(format("%1%%2%%3%.tif") % tmp_path % bt_file_without_extension % get_suffix_bt(stoi(options.band())));
		const std::string bt_tif_file_path = (tmp_path / str(format("%1%%2%.tif") % bt_file_without_extension % get_suffix_bt(options.band()))).string();
		if (exists(bt_tif_file_path) && is_regular_file(bt_tif_file_path))
		{
			BOOST_LOG_TRIVIAL(info) << "亮温tif文件提取完成，提取的tif文件：" << bt_tif_file_path;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "亮温tif文件提取失败！跳过" << input_file.mxd02_file() << "的处理";
			BOOST_LOG_TRIVIAL(error) << "处理的tif文件应位于" << bt_tif_file_path << "，但并未找到该文件";
			BOOST_LOG_TRIVIAL(error) << "亮温tif文件提取失败，预处理程序退出";
			exit(EXIT_FAILURE);
		}

		//使用MRT提取的太阳天顶角tif文件路径
		const std::string sza_tif_file_path = (tmp_path / str(format("%1%%2%.tif") % sza_file_without_extension % SUFFIX_SZA)).string();
		if (exists(sza_tif_file_path) && is_regular_file(sza_tif_file_path))
		{
			BOOST_LOG_TRIVIAL(info) << "太阳天顶角tif文件提取完成，提取的tif文件：" << sza_tif_file_path;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "太阳天顶角tif文件提取失败！跳过" << input_file.mxd03_file() << "的处理";
			BOOST_LOG_TRIVIAL(error) << "处理的tif文件应位于" << sza_tif_file_path << "，但并未找到该文件";
			BOOST_LOG_TRIVIAL(error) << "太阳天顶角tif文件提取失败，程序退出";
			exit(EXIT_FAILURE);
		}

		//使用MRT提取的云掩膜tif文件路径
		const std::string cm_tif_file_path = (tmp_path / str(format("%1%%2%.tif") % cm_file_without_extension % SUFFIX_CM)).string();
		if (exists(cm_tif_file_path) && is_regular_file(cm_tif_file_path))
		{
			BOOST_LOG_TRIVIAL(info) << "云掩膜tif文件提取完成，提取的tif文件：" << cm_tif_file_path;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "云掩膜tif文件提取失败！跳过" << input_file.mxd35_file() << "的处理";
			BOOST_LOG_TRIVIAL(error) << "处理的tif文件应位于" << cm_tif_file_path << "，但并未找到该文件";
			BOOST_LOG_TRIVIAL(error) << "云掩膜tif文件提取失败，程序退出";
			exit(EXIT_FAILURE);
		}

#pragma endregion 

		fmat bt_matrix = *Gdal_operation::read_tif_to_fmat(bt_tif_file_path);
		BOOST_LOG_TRIVIAL(debug) << "提取亮温DN值：" << Mat_operation::mat_desc(bt_matrix);

		fmat sza_matrix = *Gdal_operation::read_tif_to_fmat(sza_tif_file_path);
		BOOST_LOG_TRIVIAL(debug) << "提取太阳天顶角DN值：" << Mat_operation::mat_desc(sza_matrix);

		fmat cm_matrix = *Gdal_operation::read_tif_to_fmat(cm_tif_file_path);
		BOOST_LOG_TRIVIAL(debug) << "提取云掩膜DN值：" << Mat_operation::mat_desc(cm_matrix);

		if (bt_matrix.n_rows != sza_matrix.n_rows || bt_matrix.n_rows != cm_matrix.n_rows
			|| sza_matrix.n_rows != cm_matrix.n_rows ||
			bt_matrix.n_cols != sza_matrix.n_cols || bt_matrix.n_cols != cm_matrix.n_cols ||
			sza_matrix.n_cols != cm_matrix.n_cols)
		{
			BOOST_LOG_TRIVIAL(error) << "矩阵大小不同，无法计算，跳过" << input_file.mxd02_file() << "文件的处理";
			BOOST_LOG_TRIVIAL(error) << "程序退出";
			exit(EXIT_FAILURE);
		}

		ROW_COUNT = bt_matrix.n_rows;
		COLUMN_COUNT = bt_matrix.n_cols;
		BOOST_LOG_TRIVIAL(debug) << "ROWCOUNT=" << ROW_COUNT << ", COLUMN_COUNT=" << COLUMN_COUNT;

		// 亮温系数标定、辐亮度转换
		bt_calibration(bt_matrix, input_file.mxd02_file(), options.band(), tmp_path, bt_file_without_extension, bt_tif_file_path);
		bt_transform(bt_matrix, bt_lut, rad_lut, line_number, tmp_path, bt_file_without_extension, bt_tif_file_path);

		// 太阳天顶角数据集处理
		sza_calibration(sza_matrix, tmp_path, sza_file_without_extension, sza_tif_file_path);
		sza_filter(sza_matrix, tmp_path, sza_file_without_extension, sza_tif_file_path);

		// 云掩膜数据集处理
		cm_filter(cm_matrix, tmp_path, cm_file_without_extension, cm_tif_file_path);

		//预处理后的数据集名为原数据集名后+preprocessed
		const std::string bt_preprocess_dataset_name = str(format("%1%%2%.tif") % bt_file_without_extension %SUFFIX_PREPROCESSED_BT);
		const path bt_preprocess_dataset_path = (tmp_path / bt_preprocess_dataset_name);
		bt_sza_cm_product_save(bt_matrix, sza_matrix, cm_matrix, tmp_path, bt_file_without_extension, bt_tif_file_path, bt_preprocess_dataset_path);

		BOOST_LOG_TRIVIAL(info) << input_file.mxd02_file() << "预处理完毕，结果数据集是：" << bt_preprocess_dataset_path;
		preprocessed_bt_file_paths.push_back(bt_preprocess_dataset_path);
	}

	combine_save(preprocessed_bt_file_paths, tmp_path, options.output_image_file());

	if (!global_is_debug)
	{
		BOOST_LOG_TRIVIAL(info) << "准备清空tmp目录：" << tmp_path;
		File_operation::clear_directory(tmp_path.string());
	}
}

bool adsma::mxd021km_conversion::find_file(const std::filesystem::path& folder_path,
	const std::string& pattern, std::filesystem::path& file_path)
{
	using namespace std;
	using namespace std::filesystem;

	BOOST_LOG_TRIVIAL(debug) << "查询目标文件" << pattern << "目标目录：" << folder_path;
	if (!exists(folder_path))
	{
		BOOST_LOG_TRIVIAL(error) << "未匹配到文件，不存在的目录：" << folder_path;
		file_path = std::string("");
		return false;
	}

	for (auto it = directory_iterator(folder_path); it != directory_iterator(); ++it)
	{
		if (is_regular_file(it->path()) && it->path().extension() == ".hdf" && it->path().string().find(pattern) != std::string::npos)
		{
			file_path = it->path();
			return true;
		}
	}
	return false;
}

std::string adsma::mxd021km_conversion::get_sds_bt(int band)
{
	std::ostringstream oss;
	oss << "EV_1KM_Emissive, ";
	const 	int band_index = get_param_index_by_band(band);
	for (int i = 0; i < band_index; ++i)
		oss << "0, ";
	oss << "1";
	BOOST_LOG_TRIVIAL(debug) << "Band:" << band << ", SDS:" << oss.str();
	return oss.str();
}

std::string adsma::mxd021km_conversion::get_suffix_bt(int band)
{
	std::string ans = str(boost::format("_EV_1KM_Emissive_b%1%") % get_param_index_by_band(band));
	BOOST_LOG_TRIVIAL(debug) << "Band:" << band << ", bt suffix: " << ans;
	return ans;
}

int adsma::mxd021km_conversion::get_param_index_by_band(int band)
{
	const auto distance = std::find(std::begin(AVAILABLE_BANDS), std::end(AVAILABLE_BANDS), band) - AVAILABLE_BANDS;
	BOOST_LOG_TRIVIAL(debug) << "band: " << band << ", index:" << distance;
	if (distance == 16) throw std::runtime_error(str(boost::format("unsupported band %1%!") % band));
	return static_cast<int>(distance);
}

void adsma::mxd021km_conversion::bt_calibration(arma::fmat& bt_matrix, const std::filesystem::path& bt_hdf_file_path, int band,
	const std::filesystem::path& tmp_path, const std::string& bt_file_without_extension, const std::filesystem::path& bt_tif_file_path)
{
	using namespace arma;
	using namespace std;
	using namespace filesystem;
	const fmat coff_matrix = *Gdal_operation::read_radiance_scales_and_offsets(bt_hdf_file_path);
	const int ib = get_param_index_by_band(band);
	const float bt_scale = coff_matrix.at(0, ib);
	const float bt_offset = coff_matrix.at(1, ib);
	BOOST_LOG_TRIVIAL(info) << "读取亮温系数矫正参数，Scale=" << bt_scale << "， Offset=" << bt_offset;

	bt_matrix.transform([&bt_scale, &bt_offset](const float dn)
	{ return abs(dn - NO_DATA_VALUE_BT) > TOLERANCE ? (dn - bt_offset) * bt_scale : 0; });
	BOOST_LOG_TRIVIAL(debug) << "系数矫正后的亮温DN值：" << Mat_operation::mat_desc(bt_matrix);
	// 如果是debug模式，则保存每一步的计算结果
	if (global_is_debug)
	{
		const path bt_step1_tif_file_path = tmp_path / (bt_file_without_extension + "_bt_step1.tif");
		Gdal_operation::translate_copy(bt_tif_file_path, bt_step1_tif_file_path, "-ot Float32");
		Gdal_operation::write_fmat_to_tif(bt_step1_tif_file_path.string(), bt_matrix);
	}
}

void adsma::mxd021km_conversion::bt_transform(arma::fmat& bt_matrix,
	const arma::fvec& bt_lut, const arma::fvec& rad_lut, arma::uword line_number,
	const std::filesystem::path& tmp_path, const std::string& bt_file_without_extension, const std::filesystem::path& bt_tif_file_path)
{
	bt_matrix.transform([&bt_lut, &rad_lut, &line_number](const float dn)
	{return Rad2bt::calc_band_bt(bt_lut, rad_lut, line_number, dn); });
	BOOST_LOG_TRIVIAL(debug) << "辐亮度转换后的亮温值：" << Mat_operation::mat_desc(bt_matrix);
	if (global_is_debug)
	{
		const std::string bt_step2_tif_file_path = tmp_path.string() + bt_file_without_extension + "_bt_step2.tif";
		Gdal_operation::translate_copy(bt_tif_file_path, bt_step2_tif_file_path, "-ot Float32");
		Gdal_operation::write_fmat_to_tif(bt_step2_tif_file_path, bt_matrix);
	}
}

void adsma::mxd021km_conversion::sza_calibration(arma::fmat& sza_matrix, const std::filesystem::path& tmp_path,
	const std::string& sza_file_without_extension, const std::filesystem::path& sza_tif_file_path)
{
	arma::uvec idx = arma::find(sza_matrix < (NO_DATA_VALUE_SZA + 1));
	sza_matrix.elem(idx).zeros();
	sza_matrix = sza_matrix * SCALE_SZA;
	BOOST_LOG_TRIVIAL(debug) << "系数修正后太阳天顶角DN值：" << Mat_operation::mat_desc(sza_matrix);
	if (global_is_debug)
	{
		const std::string sza_step1_tif_file_path = tmp_path.string() + sza_file_without_extension + "_sza_step1.tif";
		Gdal_operation::translate_copy(sza_tif_file_path, sza_step1_tif_file_path, "-ot Float32");
		Gdal_operation::write_fmat_to_tif(sza_step1_tif_file_path, sza_matrix);
	}
}

void adsma::mxd021km_conversion::sza_filter(arma::fmat& sza_matrix, const std::filesystem::path& tmp_path,
	const std::string& sza_file_without_extension, const std::filesystem::path& sza_tif_file_path)
{
	sza_matrix.transform([](float dn) { return dn > THRESHOLD_SZA ? 1 : 0; });
	BOOST_LOG_TRIVIAL(debug) << ">85处理后太阳天顶角DN值：" << Mat_operation::mat_desc(sza_matrix);
	if (global_is_debug)
	{
		const std::string sza_step2_tif_file_path = tmp_path.string() + sza_file_without_extension + "_sza_step2.tif";
		Gdal_operation::translate_copy(sza_tif_file_path, sza_step2_tif_file_path, "-ot Float32");
		Gdal_operation::write_fmat_to_tif(sza_step2_tif_file_path, sza_matrix);
	}
}

void adsma::mxd021km_conversion::cm_filter(arma::fmat& cm_matrix, const std::filesystem::path& tmp_path,
	const std::string& cm_file_without_extension, const std::filesystem::path& cm_tif_file_path)
{
	// 35云掩膜数据集处理
	cm_matrix.transform([](float dn)
	{
		return (static_cast<int>(dn) & THRESHOLD_CM) == THRESHOLD_CM ? 1 : 0;
	});
	BOOST_LOG_TRIVIAL(debug) << "提取清空数据后，云掩膜DN值：" << Mat_operation::mat_desc(cm_matrix);
	if (global_is_debug)
	{
		const std::string cm_step1_tif_file_path = tmp_path.string() + cm_file_without_extension + "_cm_step1.tif";
		Gdal_operation::translate_copy(cm_tif_file_path, cm_step1_tif_file_path, "-ot Float32");
		Gdal_operation::write_fmat_to_tif(cm_step1_tif_file_path, cm_matrix);
	}
}

void adsma::mxd021km_conversion::bt_sza_cm_product_save(const arma::fmat& bt_matrix, const arma::fmat& sza_matrix, const arma::fmat& cm_matrix,
	const std::filesystem::path& tmp_path, const std::string& bt_file_without_extension, const std::filesystem::path& bt_tif_file_path, const std::filesystem::path& output_file_path)
{
	using namespace boost;

	//最终计算结果
	arma::fmat result_matrix = bt_matrix % sza_matrix % cm_matrix;
	BOOST_LOG_TRIVIAL(debug) << "三矩阵相乘，DN值：" << Mat_operation::mat_desc(result_matrix);

	//调用GDAL将最终计算结果保存到GTiff
	//预处理后的数据集名为原数据集名后+preprocessed
	const std::string bt_preprocess_dataset_name = str(format("%1%%2%.tif") % bt_file_without_extension %SUFFIX_PREPROCESSED_BT);
	const std::string bt_preprocess_dataset_path = (tmp_path / bt_preprocess_dataset_name).string();
	BOOST_LOG_TRIVIAL(debug) << "预处理后的数据集：" << bt_preprocess_dataset_path;

	bool success = Gdal_operation::translate_copy(bt_tif_file_path, bt_preprocess_dataset_path, "-ot Float32");
	BOOST_LOG_TRIVIAL(debug) << "调用gdal_translate保存" << bt_preprocess_dataset_path << "，结果：" << success;

	if (!success)
	{
		BOOST_LOG_TRIVIAL(error) << "调用gdal_translate.exe出错，程序退出";
		exit(EXIT_FAILURE);
	}
	if (!std::filesystem::exists(bt_preprocess_dataset_path) || !std::filesystem::is_regular_file(bt_preprocess_dataset_path))
	{
		BOOST_LOG_TRIVIAL(error) << "未找到预处理后的数据集" << bt_preprocess_dataset_path << "，程序退出";
		exit(EXIT_FAILURE);
	}

	const 	bool write_result = Gdal_operation::write_fmat_to_tif(bt_preprocess_dataset_path, result_matrix);
	if (!write_result)
	{
		BOOST_LOG_TRIVIAL(error) << "向" << bt_preprocess_dataset_path << "数据集写入数据时出现错误，程序退出";
		exit(EXIT_FAILURE);
	}
}

void adsma::mxd021km_conversion::combine_save(const std::vector<std::filesystem::path>& files,
	const std::filesystem::path& tmp_path, const std::filesystem::path& final_output_file)
{
	using namespace std;
	using namespace filesystem;

	BOOST_LOG_TRIVIAL(info) << "开始准备合成预处理后的" << files.size() << "个数据集...";
	std::vector<arma::fmat> mats;
	mats.resize(files.size());
	// 在使用transform之前，必须初始化目标容器
	transform(files.begin(), files.end(),
		mats.begin(), [](const path& p) { return *Gdal_operation::read_tif_to_fmat(p.string()); });

	optional<arma::fmat> final_matrix = Mat_operation::mean_mat_by_each_pixel(mats, 0);
	if (!final_matrix)
	{
		BOOST_LOG_TRIVIAL(error) << "合成DN值失败..";
		return;
	}
	BOOST_LOG_TRIVIAL(debug) << "合成DN值：" << Mat_operation::mat_desc(*final_matrix);

	const path& source_dataset_path = files.front();
	BOOST_LOG_TRIVIAL(debug) << "template file: " << source_dataset_path;
	BOOST_LOG_TRIVIAL(debug) << "output image file: " << final_output_file;

	if (exists(final_output_file)) remove(final_output_file);
	BOOST_LOG_TRIVIAL(debug) << final_output_file.parent_path();
	if (!exists(final_output_file.parent_path()))
		create_directories(final_output_file.parent_path());
	copy_file(source_dataset_path, final_output_file);

	Gdal_operation::write_fmat_to_tif(final_output_file.string(), *final_matrix);
	BOOST_LOG_TRIVIAL(info) << "合成的DN值已写入tif文件：" << final_output_file.string();

	BOOST_LOG_TRIVIAL(info) << "结果文件路径：" << final_output_file.string();
}

adsma::mxd021km_conversion::mxd021km_conversion() = default;


adsma::mxd021km_conversion::~mxd021km_conversion() = default;
