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

// �����������SUFFIX_PREPROCESSED_BT�����Լ�Ԥ�������ӵĺ�׺��
// ��SUFFIX_SZA��SUFFIX_CM����ȡʱ���߱�����ӵĺ�׺��Ϊ����������
// ���м������PREPROCESSED
const std::string adsma::mxd021km_conversion::SUFFIX_PREPROCESSED_BT = "_new";
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
 * \brief ֧�ֵĲ���ֵ������֧��30���Σ�
 */
const int AVAILABLE_BANDS[16] = { 20,21,22,23,24,25,27,28,29,30,31,32,33,34,35,36 };

// Move MXD_LUT_NAME to Rad2bt.h

int adsma::mxd021km_conversion::preprocess(const yamlArgs& options)
{
	using namespace arma;
	using namespace boost;
	using namespace std;
	using namespace std::filesystem;

	BOOST_LOG_TRIVIAL(info) << "Read MxD021km HDF data for one day...";
	vector<Input_file> hdf_files_1d = Input_file::load(options.hdflist_file());
	BOOST_LOG_TRIVIAL(info) << "�ҵ�" << hdf_files_1d.size() << "���������ļ�";
	if (hdf_files_1d.empty())
	{
		BOOST_LOG_TRIVIAL(error) << "Cannot find any HDF file";
		return 1;
	};

	//uword ROW_COUNT = 0, COLUMN_COUNT = 0;
	//lut���ļ�·����Ŀǰ��hdf�б�ĵ�һ��hdf�ļ����ļ�������Ϊ����lut��ı�ʶ
	const std::string first_hdf_path = hdf_files_1d.front().mxd02_file();
	BOOST_LOG_TRIVIAL(debug) << "First hdf: " << first_hdf_path;
	const path lut_table_path = Rad2bt::get_lut_table_path(first_hdf_path);
	std::optional<fmat> lut_cols_matrix = Rad2bt::load_lut_cols(lut_table_path, options.band());
	const fvec bt_lut = lut_cols_matrix->col(0);
	const fvec rad_lut = lut_cols_matrix->col(1);
	const uword line_number = lut_cols_matrix->n_rows;

	const path tmp_path(options.tmp_path());
	if (!exists(tmp_path))
		create_directory(tmp_path);
	BOOST_LOG_TRIVIAL(info) << "���TmpĿ¼��" << tmp_path;
	File_operation::clear_directory(tmp_path.string());

	//Ԥ����������tif�ļ��б�
	vector<path> new_bt_filelist_1d;

	for (const Input_file& infile : hdf_files_1d)
	{
		BOOST_LOG_TRIVIAL(info) << "��ʼ�����ļ�" << infile.mxd02_file();
		//����hdf�ļ��ļ�����������չ����
		const std::string bt_file_no_ext = path(infile.mxd02_file()).stem().string();
		Mrt_swath_prm_setting bt_prm(infile.mxd02_file(), infile.mxd03_file(), get_sds_bt(options.band()),
			options.min_lon(), options.max_lat(), options.max_lon(), options.min_lat(), tmp_path.string() + bt_file_no_ext + ".tif");
		Hdf_to_gtiff bt_convert(&bt_prm, tmp_path.string());
		bt_convert.do_convert();

		//ʹ��MRT��ȡ������tif�ļ�·��
		//const std::string bt_tif_file_path = str(format("%1%%2%%3%.tif") % tmp_path % bt_file_without_extension % get_suffix_bt(stoi(options.band())));
		const std::string bt_file = (tmp_path / str(format("%1%%2%.tif") % bt_file_no_ext % get_suffix_bt(options.band()))).string();
		if (exists(bt_file) && is_regular_file(bt_file))
		{
			BOOST_LOG_TRIVIAL(info) << "����tif�ļ���ȡ��ɣ���ȡ��tif�ļ���" << bt_file;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "����tif�ļ���ȡʧ�ܣ�����" << infile.mxd02_file() << "�Ĵ���";
			BOOST_LOG_TRIVIAL(error) << "�����tif�ļ�Ӧλ��" << bt_file << "������δ�ҵ����ļ�";
			BOOST_LOG_TRIVIAL(error) << "����tif�ļ���ȡʧ�ܣ�Ԥ��������˳�";
			exit(EXIT_FAILURE);
		}

		//̫���춥��hdf�ļ��ļ�����������չ����
		const std::string sza_file_no_ext = path(infile.mxd03_file()).stem().string();
		Mrt_swath_prm_setting sza_prm(infile.mxd03_file(), infile.mxd03_file(), SDS_SOLAR_ZENITH_ANGLE,
			options.min_lon(), options.max_lat(), options.max_lon(), options.min_lat(), tmp_path.string() + sza_file_no_ext + ".tif");
		Hdf_to_gtiff sza_convert(&sza_prm, tmp_path.string());
		sza_convert.do_convert();

		//ʹ��MRT��ȡ��̫���춥��tif�ļ�·��
		const std::string sza_file = (tmp_path / str(format("%1%%2%.tif") % sza_file_no_ext % SUFFIX_SZA)).string();
		if (exists(sza_file) && is_regular_file(sza_file))
		{
			BOOST_LOG_TRIVIAL(info) << "̫���춥��tif�ļ���ȡ��ɣ���ȡ��tif�ļ���" << sza_file;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "̫���춥��tif�ļ���ȡʧ�ܣ�����" << infile.mxd03_file() << "�Ĵ���";
			BOOST_LOG_TRIVIAL(error) << "�����tif�ļ�Ӧλ��" << sza_file << "������δ�ҵ����ļ�";
			BOOST_LOG_TRIVIAL(error) << "̫���춥��tif�ļ���ȡʧ�ܣ������˳�";
			exit(EXIT_FAILURE);
		}

		//����Ĥhdf�ļ��ļ�����������չ����
		const std::string cm_file_no_ext = path(infile.mxd03_file()).stem().string();
		Mrt_swath_prm_setting cm_prm(infile.mxd35_file(), infile.mxd03_file(), SDS_CLOUD_MASK,
			options.min_lon(), options.max_lat(), options.max_lon(), options.min_lat(), tmp_path.string() + cm_file_no_ext + ".tif");
		Hdf_to_gtiff cm_convert(&cm_prm, tmp_path.string());
		cm_convert.do_convert();

		//ʹ��MRT��ȡ������Ĥtif�ļ�·��
		const std::string cm_file = (tmp_path / str(format("%1%%2%.tif") % cm_file_no_ext % SUFFIX_CM)).string();
		if (exists(cm_file) && is_regular_file(cm_file))
		{
			BOOST_LOG_TRIVIAL(info) << "����Ĥtif�ļ���ȡ��ɣ���ȡ��tif�ļ���" << cm_file;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "����Ĥtif�ļ���ȡʧ�ܣ�����" << infile.mxd35_file() << "�Ĵ���";
			BOOST_LOG_TRIVIAL(error) << "�����tif�ļ�Ӧλ��" << cm_file << "������δ�ҵ����ļ�";
			BOOST_LOG_TRIVIAL(error) << "����Ĥtif�ļ���ȡʧ�ܣ������˳�";
			exit(EXIT_FAILURE);
		}

		fmat bt_mat = *Gdal_operation::read_tif_to_fmat(bt_file);
		BOOST_LOG_TRIVIAL(debug) << "��ȡ����DNֵ��" << Mat_operation::mat_desc(bt_mat);

		fmat sza_mat = *Gdal_operation::read_tif_to_fmat(sza_file);
		BOOST_LOG_TRIVIAL(debug) << "��ȡ̫���춥��DNֵ��" << Mat_operation::mat_desc(sza_mat);

		fmat cm_mat = *Gdal_operation::read_tif_to_fmat(cm_file);
		BOOST_LOG_TRIVIAL(debug) << "��ȡ����ĤDNֵ��" << Mat_operation::mat_desc(cm_mat);

		if (bt_mat.n_rows != sza_mat.n_rows || bt_mat.n_rows != cm_mat.n_rows
			|| sza_mat.n_rows != cm_mat.n_rows ||
			bt_mat.n_cols != sza_mat.n_cols || bt_mat.n_cols != cm_mat.n_cols ||
			sza_mat.n_cols != cm_mat.n_cols)
		{
			BOOST_LOG_TRIVIAL(error) << "�����С��ͬ���޷����㣬����" << infile.mxd02_file() << "�ļ��Ĵ���";
			BOOST_LOG_TRIVIAL(error) << "�����˳�";
			exit(EXIT_FAILURE);
		}

		// ����ϵ���궨��������ת��
		bt_calibration(bt_mat, infile.mxd02_file(), options.band(), tmp_path, bt_file_no_ext, bt_file);
		bt_transform(bt_mat, bt_lut, rad_lut, line_number, tmp_path, bt_file_no_ext, bt_file);

		// ̫���춥�����ݼ�����
		sza_calibration(sza_mat, tmp_path, sza_file_no_ext, sza_file);
		sza_filter(sza_mat, tmp_path, sza_file_no_ext, sza_file);

		// ����Ĥ���ݼ�����
		cm_filter(cm_mat, tmp_path, cm_file_no_ext, cm_file);

		//Ԥ���������ݼ���Ϊԭ���ݼ�����+preprocessed
		const std::string new_bt_name = str(format("%1%%2%.tif") % bt_file_no_ext %SUFFIX_PREPROCESSED_BT);
		const path new_bt_file = (tmp_path / new_bt_name);
		bt_sza_cm_product_save(bt_mat, sza_mat, cm_mat, tmp_path, bt_file_no_ext, bt_file, new_bt_file);

		BOOST_LOG_TRIVIAL(info) << infile.mxd02_file() << "Ԥ������ϣ�������ݼ��ǣ�" << new_bt_file;
		new_bt_filelist_1d.push_back(new_bt_file);
	}

	combine_save(new_bt_filelist_1d, tmp_path, options.output_image_file());

	if (!global_is_debug)
	{
		BOOST_LOG_TRIVIAL(info) << "׼�����tmpĿ¼��" << tmp_path;
		File_operation::clear_directory(tmp_path.string());
	}

	return 0;
}

bool adsma::mxd021km_conversion::find_file(const std::filesystem::path& folder_path,
	const std::string& pattern, std::filesystem::path& file_path)
{
	using namespace std;
	using namespace std::filesystem;

	BOOST_LOG_TRIVIAL(debug) << "��ѯĿ���ļ�" << pattern << "Ŀ��Ŀ¼��" << folder_path;
	if (!exists(folder_path))
	{
		BOOST_LOG_TRIVIAL(error) << "δƥ�䵽�ļ��������ڵ�Ŀ¼��" << folder_path;
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
	const 	int band_index = get_index_by_band(band);
	for (int i = 0; i < band_index; ++i)
		oss << "0, ";
	oss << "1";
	BOOST_LOG_TRIVIAL(debug) << "Band:" << band << ", SDS:" << oss.str();
	return oss.str();
}

std::string adsma::mxd021km_conversion::get_suffix_bt(int band)
{
	boost::format fmter = boost::format("_EV_1KM_Emissive_b%1%") % get_index_by_band(band);
	const std::string ans = fmter.str();
	BOOST_LOG_TRIVIAL(debug) << "Band:" << band << ", bt suffix: " << ans;
	return ans;
}

int adsma::mxd021km_conversion::get_index_by_band(int band)
{
	const auto distance = std::find(std::begin(AVAILABLE_BANDS), std::end(AVAILABLE_BANDS), band) - AVAILABLE_BANDS;
	BOOST_LOG_TRIVIAL(debug) << "band: " << band << ", index:" << distance;
	if (distance == 16)
		throw std::runtime_error(str(boost::format("unsupported band %1%!") % band));
	return static_cast<int>(distance);
}

void adsma::mxd021km_conversion::bt_calibration(arma::fmat& bt_mat, const std::filesystem::path& bt_hdf_file, int band,
	const std::filesystem::path& tmp_path, const std::string& bt_file_no_ext, const std::filesystem::path& bt_tif_file)
{
	using namespace arma;
	using namespace std;
	using namespace filesystem;
	const fmat coff_matrix = *Gdal_operation::read_radiance_scales_and_offsets(bt_hdf_file);
	const int ib = get_index_by_band(band);
	const float bt_scale = coff_matrix.at(0, ib);
	const float bt_offset = coff_matrix.at(1, ib);
	BOOST_LOG_TRIVIAL(info) << "��ȡ����ϵ������������Scale=" << bt_scale << "�� Offset=" << bt_offset;

	bt_mat.transform([&bt_scale, &bt_offset](const float dn)
	{ return abs(dn - NO_DATA_VALUE_BT) > TOLERANCE ? (dn - bt_offset) * bt_scale : 0; });
	BOOST_LOG_TRIVIAL(debug) << "ϵ�������������DNֵ��" << Mat_operation::mat_desc(bt_mat);
	// �����debugģʽ���򱣴�ÿһ���ļ�����
	if (global_is_debug)
	{
		const path bt_step1_file = tmp_path / (bt_file_no_ext + "_bt_step1.tif");
		Gdal_operation::translate_copy(bt_tif_file, bt_step1_file, "-ot Float32");
		Gdal_operation::write_fmat_to_tif(bt_step1_file.string(), bt_mat);
	}
}

void adsma::mxd021km_conversion::bt_transform(arma::fmat& bt_mat,
	const arma::fvec& bt_lut, const arma::fvec& rad_lut, arma::uword line_number,
	const std::filesystem::path& tmp_path, const std::string& bt_file_no_ext, const std::filesystem::path& bt_file)
{
	bt_mat.transform([&bt_lut, &rad_lut, &line_number](const float dn)
	{return Rad2bt::calc_band_bt(bt_lut, rad_lut, line_number, dn); });
	BOOST_LOG_TRIVIAL(debug) << "������ת���������ֵ��" << Mat_operation::mat_desc(bt_mat);
	if (global_is_debug)
	{
		const std::string bt_step2_file = tmp_path.string() + bt_file_no_ext + "_bt_step2.tif";
		Gdal_operation::translate_copy(bt_file, bt_step2_file, "-ot Float32");
		Gdal_operation::write_fmat_to_tif(bt_step2_file, bt_mat);
	}
}

void adsma::mxd021km_conversion::sza_calibration(arma::fmat& sza_mat, const std::filesystem::path& tmp_path,
	const std::string& sza_file_no_ext, const std::filesystem::path& sza_file)
{
	arma::uvec idx = arma::find(sza_mat < (NO_DATA_VALUE_SZA + 1));
	sza_mat.elem(idx).zeros();
	sza_mat = sza_mat * SCALE_SZA;
	BOOST_LOG_TRIVIAL(debug) << "ϵ��������̫���춥��DNֵ��" << Mat_operation::mat_desc(sza_mat);
	if (global_is_debug)
	{
		const std::string sza_step1_file = tmp_path.string() + sza_file_no_ext + "_sza_step1.tif";
		Gdal_operation::translate_copy(sza_file, sza_step1_file, "-ot Float32");
		Gdal_operation::write_fmat_to_tif(sza_step1_file, sza_mat);
	}
}

void adsma::mxd021km_conversion::sza_filter(arma::fmat& sza_mat, const std::filesystem::path& tmp_path,
	const std::string& sza_file_no_ext, const std::filesystem::path& sza_file)
{
	sza_mat.transform([](float dn) { return dn > THRESHOLD_SZA ? 1 : 0; });
	BOOST_LOG_TRIVIAL(debug) << ">85�����̫���춥��DNֵ��" << Mat_operation::mat_desc(sza_mat);
	if (global_is_debug)
	{
		const std::string sza_step2_file = tmp_path.string() + sza_file_no_ext + "_sza_step2.tif";
		Gdal_operation::translate_copy(sza_file, sza_step2_file, "-ot Float32");
		Gdal_operation::write_fmat_to_tif(sza_step2_file, sza_mat);
	}
}

void adsma::mxd021km_conversion::cm_filter(arma::fmat& cm_mat, const std::filesystem::path& tmp_path,
	const std::string& cm_file_no_ext, const std::filesystem::path& cm_file)
{
	// 35����Ĥ���ݼ�����
	cm_mat.transform([](float dn)
	{
		return (static_cast<int>(dn) & THRESHOLD_CM) == THRESHOLD_CM ? 1 : 0;
	});
	BOOST_LOG_TRIVIAL(debug) << "��ȡ������ݺ�����ĤDNֵ��" << Mat_operation::mat_desc(cm_mat);
	if (global_is_debug)
	{
		const std::string cm_step1_file = tmp_path.string() + cm_file_no_ext + "_cm_step1.tif";
		Gdal_operation::translate_copy(cm_file, cm_step1_file, "-ot Float32");
		Gdal_operation::write_fmat_to_tif(cm_step1_file, cm_mat);
	}
}

void adsma::mxd021km_conversion::bt_sza_cm_product_save(const arma::fmat& bt_mat, const arma::fmat& sza_mat, const arma::fmat& cm_mat,
	const std::filesystem::path& tmp_path, const std::string& bt_file_no_ext, const std::filesystem::path& bt_file, const std::filesystem::path& new_bt_file)
{
	using namespace boost;
	//���ռ�����
	arma::fmat result_matrix = bt_mat % sza_mat % cm_mat;
	BOOST_LOG_TRIVIAL(debug) << "��������ˣ�DNֵ��" << Mat_operation::mat_desc(result_matrix);

	//����GDAL�����ռ��������浽GTiff
	const 	bool ok = Gdal_operation::translate_copy(bt_file, new_bt_file, "-ot Float32");
	BOOST_LOG_TRIVIAL(debug) << "����gdal_translate����" << new_bt_file << "�������" << ok;

	if (!ok)
	{
		BOOST_LOG_TRIVIAL(error) << "����gdal_translate.exe���������˳�";
		exit(EXIT_FAILURE);
	}
	if (!exists(new_bt_file) || !is_regular_file(new_bt_file))
	{
		BOOST_LOG_TRIVIAL(error) << "δ�ҵ�Ԥ���������ݼ�" << new_bt_file << "�������˳�";
		exit(EXIT_FAILURE);
	}

	const 	bool write_result = Gdal_operation::write_fmat_to_tif(new_bt_file.string(), result_matrix);
	if (!write_result)
	{
		BOOST_LOG_TRIVIAL(error) << "��" << new_bt_file << "���ݼ�д������ʱ���ִ��󣬳����˳�";
		exit(EXIT_FAILURE);
	}
}

void adsma::mxd021km_conversion::combine_save(const std::vector<std::filesystem::path>& files,
	const std::filesystem::path& tmp_path, const std::filesystem::path& final_output_file)
{
	using namespace std;
	using namespace filesystem;

	BOOST_LOG_TRIVIAL(info) << "��ʼ׼���ϳ�Ԥ������" << files.size() << "�����ݼ�...";
	std::vector<arma::fmat> mats;
	mats.resize(files.size());
	// ��ʹ��transform֮ǰ�������ʼ��Ŀ������
	transform(files.begin(), files.end(),
		mats.begin(), [](const path& p) { return *Gdal_operation::read_tif_to_fmat(p.string()); });

	optional<arma::fmat> final_matrix = Mat_operation::mean_mat_by_each_pixel(mats, 0);
	if (!final_matrix)
	{
		BOOST_LOG_TRIVIAL(error) << "�ϳ�DNֵʧ��..";
		return;
	}
	BOOST_LOG_TRIVIAL(debug) << "�ϳ�DNֵ��" << Mat_operation::mat_desc(*final_matrix);

	const path& source_dataset_path = files.front();
	BOOST_LOG_TRIVIAL(debug) << "template file: " << source_dataset_path;
	BOOST_LOG_TRIVIAL(debug) << "output image file: " << final_output_file;

	if (exists(final_output_file)) remove(final_output_file);
	BOOST_LOG_TRIVIAL(debug) << final_output_file.parent_path();
	if (!exists(final_output_file.parent_path()))
		create_directories(final_output_file.parent_path());
	copy_file(source_dataset_path, final_output_file);

	Gdal_operation::write_fmat_to_tif(final_output_file.string(), *final_matrix);
	BOOST_LOG_TRIVIAL(info) << "�ϳɵ�DNֵ��д��tif�ļ���" << final_output_file.string();

	BOOST_LOG_TRIVIAL(info) << "����ļ�·����" << final_output_file.string();
}

adsma::mxd021km_conversion::mxd021km_conversion() = default;

adsma::mxd021km_conversion::~mxd021km_conversion() = default;
