#include "..\\modis_api\\File_operation.h"
#include "..\\modis_api\\Gdal_operation.h"
#include "..\\modis_api\\Hdf_file.h"
#include "..\\modis_api\\Hdf_to_gtiff.h"
#include "..\\modis_api\\Mat_operation.h"
#include "..\\modis_api\\Mrt_swath_prm_setting.h"
#include "globals.h"
#include "Input_file.h"
#include "Preprocess_bt.h"
#include "Rad2bt.h"
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/format/free_funcs.hpp>
#include <sstream>

namespace fs = boost::filesystem;

const std::string proc_MxD021km::Preprocess_bt::SUFFIX_PREPROCESSED_BT = "_preprocessed";
//const string proc_MxD021km::Preprocess_bt::SDS_BRIGHTNESS_TEMPERATURE = "EV_1KM_Emissive, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1";
const std::string proc_MxD021km::Preprocess_bt::SDS_SOLAR_ZENITH_ANGLE = "SolarZenith";
const std::string proc_MxD021km::Preprocess_bt::SDS_CLOUD_MASK = "Cloud_Mask, 1";
//const string proc_MxD021km::Preprocess_bt::SUFFIX_BT = "_EV_1KM_Emissive_b10";
const std::string proc_MxD021km::Preprocess_bt::SUFFIX_SZA = "_SolarZenith";
const std::string proc_MxD021km::Preprocess_bt::SUFFIX_CM = "_Cloud_Mask_b0";

const int proc_MxD021km::Preprocess_bt::NO_DATA_VALUE_BT = 65535;
const int proc_MxD021km::Preprocess_bt::NO_DATA_VALUE_SZA = -32767;
const float proc_MxD021km::Preprocess_bt::SCALE_SZA = 0.01f;
const float proc_MxD021km::Preprocess_bt::THRESHOLD_SZA = 85.0f;
const int proc_MxD021km::Preprocess_bt::THRESHOLD_CM = 6;
const float proc_MxD021km::Preprocess_bt::TOLERANCE = 1E-05f;

/**
 * \brief ֧�ֵĲ���ֵ������֧��30���Σ�
 */
const int AVAILABLE_BANDS[16] = { 20,21,22,23,24,25,27,28,29,30,31,32,33,34,35,36 };

const std::string proc_MxD021km::Preprocess_bt::MOD_LUT_TABLE_NAME = "data//Terra_MODIS_rad2BT_LUT.txt";
const std::string proc_MxD021km::Preprocess_bt::MYD_LUT_TABLE_NAME = "data//Aqua_MODIS_rad2BT_LUT.txt";



void proc_MxD021km::Preprocess_bt::preprocess(const Options_yaml& options)
{
	std::vector<Input_file> input_files = Input_file::load(options.input_hdf_file());
	BOOST_LOG_TRIVIAL(info) << "�ҵ�" << input_files.size() << "���������ļ�";
	if (input_files.empty()) return;

	const std::string tmp_folder = options.temp_dir();
	if (!boost::filesystem::exists(tmp_folder))
		boost::filesystem::create_directory(tmp_folder);
	BOOST_LOG_TRIVIAL(info) << "���TmpĿ¼��" << tmp_folder;
	File_operation::clear_directory(tmp_folder);

	//Ԥ����������tif�ļ��б�
	std::vector<std::string> preprocessed_bt_file_paths;

	arma::uword ROW_COUNT = 0, COLUMN_COUNT = 0;
	//lut���ļ�·����Ŀǰ��hdf�б�ĵ�һ��hdf�ļ����ļ�������Ϊ����lut��ı�ʶ
	const std::string path_first_hdf = File_operation::read_file_all_lines(options.input_hdf_file()).at(1);
	BOOST_LOG_TRIVIAL(debug) << "First hdf: " << path_first_hdf;
	const std::string lut_table_path = get_lut_table(path_first_hdf);
	const arma::fmat lut_matrix = *Rad2bt::load_lut_table(lut_table_path);
	//lut��BT������
	int bt_col_index = Rad2bt::get_col_index(lut_table_path, Rad2bt::get_bt_col_name());
	//lut����������
	int rad_col_index = Rad2bt::get_col_index(lut_table_path, Rad2bt::get_rad_col_name(options.band()));
	BOOST_LOG_TRIVIAL(debug) << "Lut��BT������ֵ��" << bt_col_index << "�� Rad������ֵ��" << rad_col_index;
	if (rad_col_index == -1) exit(EXIT_FAILURE);
	//lut��BT��
	const float* bt_lut = lut_matrix.col(bt_col_index).colmem;
	//lut������
	const float* rad_lut = lut_matrix.col(rad_col_index).colmem;
	//lut����������
	const arma::uword line_number = lut_matrix.n_rows;

	for (const Input_file& input_file : input_files)
	{
		BOOST_LOG_TRIVIAL(info) << "��ʼ�����ļ�" << input_file.bt_hdf_file();
		Mrt_swath_prm_setting bt_prm_setting;
		bt_prm_setting.set_input_hdf_file(input_file.bt_hdf_file());
		bt_prm_setting.set_geo_loc_hdf_file(input_file.sza_hdf_file());
		bt_prm_setting.set_sds_str(get_sds_bt(stoi(options.band())));
		bt_prm_setting.set_ulc_lng(options.min_lon());
		bt_prm_setting.set_ulc_lat(options.max_lat());
		bt_prm_setting.set_lrc_lng(options.max_lon());
		bt_prm_setting.set_lrc_lat(options.min_lat());
		//����hdf�ļ��ļ�����������չ����
		const std::string bt_file_without_extension = fs::path(input_file.bt_hdf_file()).stem().string();
		bt_prm_setting.set_output_gtiff_file(tmp_folder + bt_file_without_extension + ".tif");
		Hdf_to_gtiff bt_convert(&bt_prm_setting, tmp_folder);
		bt_convert.do_convert();

		Mrt_swath_prm_setting sza_prm_setting;
		sza_prm_setting.set_input_hdf_file(input_file.sza_hdf_file());
		sza_prm_setting.set_geo_loc_hdf_file(input_file.sza_hdf_file());
		sza_prm_setting.set_sds_str(SDS_SOLAR_ZENITH_ANGLE);
		sza_prm_setting.set_ulc_lng(options.min_lon());
		sza_prm_setting.set_ulc_lat(options.max_lat());
		sza_prm_setting.set_lrc_lng(options.max_lon());
		sza_prm_setting.set_lrc_lat(options.min_lat());
		//̫���춥��hdf�ļ��ļ�����������չ����
		const std::string sza_file_without_extension = fs::path(input_file.sza_hdf_file()).stem().string();
		sza_prm_setting.set_output_gtiff_file(tmp_folder + sza_file_without_extension + ".tif");
		Hdf_to_gtiff sza_convert(&sza_prm_setting, tmp_folder);
		sza_convert.do_convert();

		Mrt_swath_prm_setting cm_prm_setting;
		cm_prm_setting.set_input_hdf_file(input_file.cm_hdf_file());
		cm_prm_setting.set_geo_loc_hdf_file(input_file.sza_hdf_file());
		cm_prm_setting.set_sds_str(SDS_CLOUD_MASK);
		cm_prm_setting.set_ulc_lng(options.min_lon());
		cm_prm_setting.set_ulc_lat(options.max_lat());
		cm_prm_setting.set_lrc_lng(options.max_lon());
		cm_prm_setting.set_lrc_lat(options.min_lat());
		//����Ĥhdf�ļ��ļ�����������չ����
		const std::string cm_file_without_extension = fs::path(input_file.sza_hdf_file()).stem().string();
		cm_prm_setting.set_output_gtiff_file(tmp_folder + cm_file_without_extension + ".tif");
		Hdf_to_gtiff cm_convert(&cm_prm_setting, tmp_folder);
		cm_convert.do_convert();

#pragma endregion

#pragma region ���tif�ļ��Ƿ����

		//ʹ��MRT��ȡ������tif�ļ�·��
		const std::string bt_tif_file_path = boost::str(boost::format("%1%%2%%3%.tif") % tmp_folder % bt_file_without_extension % get_suffix_bt(stoi(options.band())));
		if (fs::exists(bt_tif_file_path) && fs::is_regular_file(bt_tif_file_path))
		{
			BOOST_LOG_TRIVIAL(info) << "����tif�ļ���ȡ��ɣ���ȡ��tif�ļ���" << bt_tif_file_path;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "����tif�ļ���ȡʧ�ܣ�����" << input_file.bt_hdf_file() << "�Ĵ���";
			BOOST_LOG_TRIVIAL(error) << "�����tif�ļ�Ӧλ��" << bt_tif_file_path << "������δ�ҵ����ļ�";
			continue;
		}

		//ʹ��MRT��ȡ��̫���춥��tif�ļ�·��
		const std::string sza_tif_file_path = str(boost::format("%1%%2%%3%.tif") % tmp_folder % sza_file_without_extension % SUFFIX_SZA);
		if (fs::exists(sza_tif_file_path) && fs::is_regular_file(sza_tif_file_path))
		{
			BOOST_LOG_TRIVIAL(info) << "̫���춥��tif�ļ���ȡ��ɣ���ȡ��tif�ļ���" << sza_tif_file_path;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "̫���춥��tif�ļ���ȡʧ�ܣ�����" << input_file.sza_hdf_file() << "�Ĵ���";
			BOOST_LOG_TRIVIAL(error) << "�����tif�ļ�Ӧλ��" << sza_tif_file_path << "������δ�ҵ����ļ�";
			continue;
		}

		//ʹ��MRT��ȡ������Ĥtif�ļ�·��
		const std::string cm_tif_file_path = str(boost::format("%1%%2%%3%.tif") % tmp_folder % cm_file_without_extension % SUFFIX_CM);
		if (fs::exists(cm_tif_file_path) && fs::is_regular_file(cm_tif_file_path))
		{
			BOOST_LOG_TRIVIAL(info) << "����Ĥtif�ļ���ȡ��ɣ���ȡ��tif�ļ���" << cm_tif_file_path;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "����Ĥtif�ļ���ȡʧ�ܣ�����" << input_file.cm_hdf_file() << "�Ĵ���";
			BOOST_LOG_TRIVIAL(error) << "�����tif�ļ�Ӧλ��" << cm_tif_file_path << "������δ�ҵ����ļ�";
			continue;
		}

#pragma endregion 

		arma::fmat bt_matrix = *Gdal_operation::read_tif_to_fmat(bt_tif_file_path);
		BOOST_LOG_TRIVIAL(debug) << "��ȡ����DNֵ��" << Mat_operation::mat_desc(bt_matrix);

		arma::fmat sza_matrix = *Gdal_operation::read_tif_to_fmat(sza_tif_file_path);
		BOOST_LOG_TRIVIAL(debug) << "��ȡ̫���춥��DNֵ��" << Mat_operation::mat_desc(sza_matrix);

		//Gdal_operation::translate_copy(sza_tif_file_path, tmp_folder + sza_file_without_extension + "_dddddddddd.tif");
		//Gdal_operation::write_fmat_to_tif(tmp_folder + sza_file_without_extension + "_dddddddddd.tif", sza_matrix);

		arma::fmat cm_matrix = *Gdal_operation::read_tif_to_fmat(cm_tif_file_path);
		BOOST_LOG_TRIVIAL(debug) << "��ȡ����ĤDNֵ��" << Mat_operation::mat_desc(cm_matrix);

		if (bt_matrix.n_rows != sza_matrix.n_rows || bt_matrix.n_rows != cm_matrix.n_rows
			|| sza_matrix.n_rows != cm_matrix.n_rows ||
			bt_matrix.n_cols != sza_matrix.n_cols || bt_matrix.n_cols != cm_matrix.n_cols ||
			sza_matrix.n_cols != cm_matrix.n_cols)
		{
			BOOST_LOG_TRIVIAL(error) << "�����С��ͬ���޷����㣬����" << input_file.bt_hdf_file() << "�ļ��Ĵ���";
			continue;
		}

		ROW_COUNT = bt_matrix.n_rows;
		COLUMN_COUNT = bt_matrix.n_cols;
		BOOST_LOG_TRIVIAL(debug) << "ROWCOUNT=" << ROW_COUNT << ", COLUMN_COUNT=" << COLUMN_COUNT;

		const auto coff_matrix = *Gdal_operation::read_radiance_scales_and_offsets(input_file.bt_hdf_file());
		const auto bt_scale = coff_matrix.at(0, get_param_index_by_band(stoi(options.band())));
		const auto bt_offset = coff_matrix.at(1, get_param_index_by_band(stoi(options.band())));
		BOOST_LOG_TRIVIAL(info) << "��ȡ����ϵ������������Scale=" << bt_scale << "�� Offset=" << bt_offset;

		bt_matrix.transform([&bt_scale, &bt_offset](const float dn) { return abs(dn - NO_DATA_VALUE_BT) > TOLERANCE ? (dn - bt_offset) * bt_scale : 0; });
		BOOST_LOG_TRIVIAL(debug) << "ϵ�������������DNֵ��" << Mat_operation::mat_desc(bt_matrix);
		// �����debugģʽ���򱣴�ÿһ���ļ�����
		if (global_is_debug)
		{
			const std::string bt_step1_tif_file_path = tmp_folder + bt_file_without_extension + "_bt_step1.tif";
			Gdal_operation::translate_copy(bt_tif_file_path, bt_step1_tif_file_path, "-ot Float32");
			Gdal_operation::write_fmat_to_tif(bt_step1_tif_file_path, bt_matrix);
		}
		bt_matrix.transform([&bt_lut, &rad_lut, &line_number](const float dn) {return Rad2bt::calc_band_bt(bt_lut, rad_lut, line_number, dn); });
		BOOST_LOG_TRIVIAL(debug) << "������ת���������ֵ��" << Mat_operation::mat_desc(bt_matrix);
		if (global_is_debug)
		{
			const std::string bt_step2_tif_file_path = tmp_folder + bt_file_without_extension + "_bt_step2.tif";
			Gdal_operation::translate_copy(bt_tif_file_path, bt_step2_tif_file_path, "-ot Float32");
			Gdal_operation::write_fmat_to_tif(bt_step2_tif_file_path, bt_matrix);
		}

		// ̫���춥�����ݼ�����
		//sza_matrix.transform([](float dn) { return  abs(dn - NO_DATA_VALUE_SZA) > TOLERANCE ? dn * SCALE_SZA :0 ; });
		arma::uvec idx = arma::find(sza_matrix < (NO_DATA_VALUE_SZA + 1));
		sza_matrix.elem(idx).zeros();
		sza_matrix = sza_matrix * SCALE_SZA;
		BOOST_LOG_TRIVIAL(debug) << "ϵ��������̫���춥��DNֵ��" << Mat_operation::mat_desc(sza_matrix);
		if (global_is_debug)
		{
			const std::string sza_step1_tif_file_path = tmp_folder + sza_file_without_extension + "_sza_step1.tif";
			Gdal_operation::translate_copy(sza_tif_file_path, sza_step1_tif_file_path, "-ot Float32");
			Gdal_operation::write_fmat_to_tif(sza_step1_tif_file_path, sza_matrix);
		}

		sza_matrix.transform([](float dn) { return dn > THRESHOLD_SZA ? 1 : 0; });
		BOOST_LOG_TRIVIAL(debug) << ">85�����̫���춥��DNֵ��" << Mat_operation::mat_desc(sza_matrix);
		if (global_is_debug)
		{
			const std::string sza_step2_tif_file_path = tmp_folder + sza_file_without_extension + "_sza_step2.tif";
			Gdal_operation::translate_copy(sza_tif_file_path, sza_step2_tif_file_path, "-ot Float32");
			Gdal_operation::write_fmat_to_tif(sza_step2_tif_file_path, sza_matrix);
		}

		// 35����Ĥ���ݼ�����
		cm_matrix.transform([](float dn)
		{
			return (static_cast<int>(dn) & THRESHOLD_CM) == THRESHOLD_CM ? 1 : 0;
		});
		BOOST_LOG_TRIVIAL(debug) << "��ȡ������ݺ�����ĤDNֵ��" << Mat_operation::mat_desc(cm_matrix);
		if (global_is_debug)
		{
			const std::string cm_step1_tif_file_path = tmp_folder + cm_file_without_extension + "_cm_step1.tif";
			Gdal_operation::translate_copy(cm_tif_file_path, cm_step1_tif_file_path, "-ot Float32");
			Gdal_operation::write_fmat_to_tif(cm_step1_tif_file_path, cm_matrix);
		}

		//���ռ�����
		arma::fmat result_matrix = bt_matrix % sza_matrix % cm_matrix;
		BOOST_LOG_TRIVIAL(debug) << "��������ˣ�DNֵ��" << Mat_operation::mat_desc(result_matrix);

		//����GDAL�����ռ��������浽GTiff
		//Ԥ���������ݼ���Ϊԭ���ݼ�����+preprocessed
		const std::string bt_preprocess_dataset_name = str(boost::format("%1%%2%.tif") % bt_file_without_extension %SUFFIX_PREPROCESSED_BT);
		const std::string bt_preprocess_dataset_path = str(boost::format("%1%%2%") % tmp_folder % bt_preprocess_dataset_name);
		BOOST_LOG_TRIVIAL(debug) << "Ԥ���������ݼ���" << bt_preprocess_dataset_path;

		bool success = Gdal_operation::translate_copy(bt_tif_file_path, bt_preprocess_dataset_path, "-ot Float32");
		BOOST_LOG_TRIVIAL(debug) << "����gdal_translate����" << bt_preprocess_dataset_path << "�������" << success;

		if (!success)
		{
			BOOST_LOG_TRIVIAL(error) << "����gdal_translate.exe�������⣬����" << input_file.bt_hdf_file() << "�Ĵ���";
			continue;
		}
		if (!fs::exists(bt_preprocess_dataset_path) || !fs::is_regular_file(bt_preprocess_dataset_path))
		{
			BOOST_LOG_TRIVIAL(error) << "δ�ҵ�Ԥ���������ݼ�" << bt_preprocess_dataset_path << "!����" << input_file.bt_hdf_file() << "�Ĵ���";
			continue;
		}

		bool write_result = Gdal_operation::write_fmat_to_tif(bt_preprocess_dataset_path, result_matrix);
		if (!write_result)
		{
			BOOST_LOG_TRIVIAL(error) << "��" << bt_preprocess_dataset_path << "���ݼ�д������ʱ���ִ�������" << input_file.bt_hdf_file() << "�Ĵ���";
			continue;
		}

		BOOST_LOG_TRIVIAL(info) << input_file.bt_hdf_file() << "Ԥ������ϣ�������ݼ��ǣ�" << bt_preprocess_dataset_path;
		preprocessed_bt_file_paths.push_back(bt_preprocess_dataset_path);
	}

	BOOST_LOG_TRIVIAL(info) << "��ʼ׼���ϳ�Ԥ������" << preprocessed_bt_file_paths.size() << "�����ݼ�...";

	std::vector<arma::fmat> mats;
	mats.resize(preprocessed_bt_file_paths.size());
	// ��ʹ��transform֮ǰ�������ʼ��Ŀ������
	transform(preprocessed_bt_file_paths.begin(), preprocessed_bt_file_paths.end(),
		mats.begin(), [](const std::string p) { return *Gdal_operation::read_tif_to_fmat(p); });

	auto final_matrix = Mat_operation::mean_mat_by_each_pixel(mats, 0);
	if (!final_matrix)
	{
		BOOST_LOG_TRIVIAL(error) << "�ϳ�DNֵʧ��..";
		return;
	}
	BOOST_LOG_TRIVIAL(info) << "�ϳ�DNֵ��" << Mat_operation::mat_desc(*final_matrix);

	const std::string source_dataset_path = preprocessed_bt_file_paths[0];
	BOOST_LOG_TRIVIAL(debug) << "template file: " << source_dataset_path;
	BOOST_LOG_TRIVIAL(debug) << "output image file: " << options.output_image_file();

	if (fs::exists(options.output_image_file())) fs::remove(options.output_image_file());
	copy_file(source_dataset_path, options.output_image_file(), fs::copy_option::overwrite_if_exists);

	Gdal_operation::write_fmat_to_tif(options.output_image_file(), *final_matrix);
	BOOST_LOG_TRIVIAL(info) << "�ϳɵ�DNֵ��д��tif�ļ���" << options.output_image_file();

	BOOST_LOG_TRIVIAL(info) << "����ļ�·����" << options.output_image_file();

	if (!global_is_debug)
	{
		BOOST_LOG_TRIVIAL(info) << "׼�����tmpĿ¼��" << tmp_folder;
		File_operation::clear_directory(tmp_folder);
	}
}

/**
 * \brief ��ָ�����ļ����¸���pattern�ҵ���Ӧ���ļ�·��
 * \param folder �ļ���
 * \param pattern
 * \param file_path �ҵ����ļ�·����д�뵽�ñ�����
 * \return �ҵ��ļ��򷵻�true�����򷵻�false
 */
bool proc_MxD021km::Preprocess_bt::find_file(const std::string& folder, const std::string& pattern, std::string& file_path)
{
	BOOST_LOG_TRIVIAL(debug) << "��ѯĿ���ļ�" << pattern << "Ŀ��Ŀ¼��" << folder;
	if (!fs::exists(folder))
	{
		BOOST_LOG_TRIVIAL(error) << "δƥ�䵽�ļ��������ڵ�Ŀ¼��" << folder;
		file_path = std::string("");
		return false;
	}

	for (auto it = fs::directory_iterator(folder); it != fs::directory_iterator(); ++it)
	{
		if (is_regular_file(it->path()) && it->path().extension() == ".hdf" && it->path().string().find(pattern) != std::string::npos)
		{
			file_path = it->path().string();
			return true;
		}
	}
	file_path = std::string("");
	return false;
}

/**
 * \brief ������ȡ�Ĳ���ֵ��ȡ����SDS
 * \param band
 * \return
 */
std::string proc_MxD021km::Preprocess_bt::get_sds_bt(int band)
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

/**
 * \brief
 * \param band ������ȡ�Ĳ���ֵ��ȡ�������ݼ���׺
 * \return
 */
std::string proc_MxD021km::Preprocess_bt::get_suffix_bt(int band)
{
	std::string ans = str(boost::format("_EV_1KM_Emissive_b%1%") % get_param_index_by_band(band));
	BOOST_LOG_TRIVIAL(debug) << "Band:" << band << ", bt suffix: " << ans;
	return ans;
}

int proc_MxD021km::Preprocess_bt::get_param_index_by_band(int band)
{
	const auto distance = std::find(std::begin(AVAILABLE_BANDS), std::end(AVAILABLE_BANDS), band) - AVAILABLE_BANDS;
	BOOST_LOG_TRIVIAL(debug) << "band: " << band << ", index:" << distance;
	if (distance == 16) throw std::runtime_error(str(boost::format("unsupported band %1%!") % band));
	return static_cast<int>(distance);
}

std::string proc_MxD021km::Preprocess_bt::get_lut_table(const std::string& path_hdf_file)
{
	fs::path p(path_hdf_file);
	std::string ans;
	if (p.filename().string().find("MOD") != std::string::npos)
		ans = (fs::current_path() / MOD_LUT_TABLE_NAME).string();
	else if (p.filename().string().find("MYD") != std::string::npos)
		ans = (fs::current_path() / MYD_LUT_TABLE_NAME).string();
	else
		throw std::runtime_error("can not load lut table!");
	BOOST_LOG_TRIVIAL(debug) << "����lut��" << ans;
	return ans;
}



proc_MxD021km::Preprocess_bt::Preprocess_bt() = default;


proc_MxD021km::Preprocess_bt::~Preprocess_bt() = default;
