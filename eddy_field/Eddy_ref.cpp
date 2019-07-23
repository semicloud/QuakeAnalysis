#include "..//modis_api//Gdal_operation.h"
#include "..//modis_api//Mat_operation.h"
#include "Eddy_ref.h"
#include "Eddy_field.h"
#include <armadillo>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>
#include <string>
#include <filesystem>
#include <vector>

/**
 * \brief ��ȡ�������ɱ�������Tif�ļ�·���б��ú������ڿ��ƶ���adasm.exe��
 * \brief Ŀǰ��ͨ��Eddy_field_options_yaml���get_ref_tif_file_list��Ա
 * \brief ��ȡ���㱳������tif�ļ��б�
 * \return
 */
std::vector<std::string> Eddy_ref::get_tif_file_list_for_ref(const Eddy_field_options_yaml& options, const std::string& type)
{
	throw std::exception("should not call this function..");

	//const auto data_folder = options.get_tif_directory(type);
	//BOOST_LOG_TRIVIAL(debug) << "�����ļ�����·����" << data_folder;

	//if (!fs::exists(data_folder))
	//{
	//	BOOST_LOG_TRIVIAL(error) << "�����ڵ�Ԥ���������ļ��У�" << data_folder;
	//	return std::vector<std::string>();
	//}

	//if (!fs::is_directory(data_folder))
	//{
	//	BOOST_LOG_TRIVIAL(error) << data_folder << "���ǺϷ���Ŀ¼��";
	//	return std::vector<std::string>();
	//}

	//std::vector<std::string> ans;

	//for (fs::recursive_directory_iterator it(data_folder); it != fs::recursive_directory_iterator(); ++it)
	//{
	//	if (!is_regular_file(*it)) continue;

	//	auto file_name = it->path().filename().string();
	//	// bt_2018100.tif�����м��_���ַ�����Ϊ�����֣�Ȼ����ȡ�ڶ����ֵ�ǰ4���ַ���Ϊ���
	//	std::vector<std::string> parts;
	//	split(parts, file_name, boost::is_any_of("_"));
	//	const auto file_year = boost::lexical_cast<int>(parts[1].substr(0, 4));
	//	const auto options_year = boost::lexical_cast<int>(options.year());
	//	auto file_day = parts[1].substr(4, 3); // day�����ַ������бȽ�
	//	//spdlog::debug("�ļ�{}Ϊ{}��{}�յĹ۲�����", it->path().string(), file_year, file_day);
	//	// ɸѡ�ļ�
	//	if (file_day == options.day() && file_year <= options_year)
	//	{
	//		ans.push_back(it->path().string());
	//	}
	//}

	//if (!ans.empty())
	//{
	//	BOOST_LOG_TRIVIAL(info) << "��Ԥ���������ļ���" << data_folder << "�¹��ҵ�" << ans.size() << "����" << options.day() << "��������ļ�";
	//	return ans;
	//}

	//BOOST_LOG_TRIVIAL(warning) << "δ��Ԥ���������ļ���" << data_folder << "���ҵ���" << options.day() << "������ݣ�";
	//return std::vector<std::string>();
}

Eddy_ref::Eddy_ref()
{
}


Eddy_ref::~Eddy_ref()
{
}

void Eddy_ref::compute_eddy_ref_m1(Eddy_field_options_yaml& options)
{
	using namespace std;
	using namespace std::filesystem;

	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "============ʹ�÷���1���ɱ�����===============";
	//auto file_path_list = get_tif_file_list_for_ref(options, type);
	const vector<path>  tif_files_for_ref_computing = options.get_tif_files_for_ref_computing();
	if (tif_files_for_ref_computing.empty())
	{
		BOOST_LOG_TRIVIAL(info) << "δ�ҵ����ݣ��޷����ɱ�������";
		BOOST_LOG_TRIVIAL(info) << "===============ʹ�÷���1���ɱ���������===========";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}
	std::vector<arma::fmat> mat_list;
	for (const auto& file_path : tif_files_for_ref_computing)
	{
		auto optional_fmat = modis_api::Gdal_operation::read_tif_to_fmat(file_path.string());
		if (optional_fmat)
			mat_list.push_back(*optional_fmat);
		else
			BOOST_LOG_TRIVIAL(error) << "��ȡdnֵʧ�ܣ�����" << file_path << "�Ĵ���";
	}

	auto mean_mat = modis_api::Mat_operation::mean_mat_by_each_pixel(mat_list, 0);
	if (!mean_mat)
	{
		BOOST_LOG_TRIVIAL(error) << "�����С��ͬ���޷����ɱ�������";
		BOOST_LOG_TRIVIAL(info) << "===============ʹ�÷���1���ɱ���������===========";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}

	// д��tif�ļ�

	/*
	 * copy_file with copy_options is dangerous!
	 * see: https://stackoverflow.com/questions/829468/how-to-perform-boostfilesystem-copy-file-with-overwrite
	 * Beware of boost::copy_file with copy_option::overwrite_if_exists!
	 * If the destination file exists and it is smaller than the source, the function will only overwrite the first size(from_file) bytes in the target file.
	 */
	if (exists(options.ref_image_file()))
		remove(options.ref_image_file());
	// ��Ŀ¼�������򴴽�֮
	if (!exists(options.ref_image_file().parent_path()))
		create_directories(options.ref_image_file().parent_path());
	// Ȼ���ƹ�ȥ
	copy_file(tif_files_for_ref_computing.front(), options.ref_image_file());
	// д������
	BOOST_LOG_TRIVIAL(debug) << "COPY " << tif_files_for_ref_computing.front() << " to " << options.ref_image_file();
	modis_api::Gdal_operation::write_fmat_to_tif(options.ref_image_file().string(), *mean_mat);

	BOOST_LOG_TRIVIAL(info) << "���ɵı������ļ�Ϊ��" << options.ref_image_file();
	BOOST_LOG_TRIVIAL(info) << "===============ʹ�÷���1���ɱ���������===========";
	BOOST_LOG_TRIVIAL(info) << "";
}

void Eddy_ref::compute_eddy_ref_m2(Eddy_field_options_yaml& options)
{
	using namespace  std;
	using namespace filesystem;
	BOOST_LOG_TRIVIAL(info) << "";
	BOOST_LOG_TRIVIAL(info) << "============ʹ�÷���2���ɱ�����===============";
	//auto file_path_list = get_tif_file_list_for_ref(options, type);
	const vector<path> tif_files_for_ref_computing = options.get_tif_files_for_ref_computing();
	if (tif_files_for_ref_computing.empty())
	{
		BOOST_LOG_TRIVIAL(warning) << "δ�ҵ����ݣ��޷����ɱ�������";
		BOOST_LOG_TRIVIAL(info) << "===============ʹ�÷���2���ɱ���������===========";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}
	vector<arma::fmat> mat_list;
	for (auto file_path : tif_files_for_ref_computing)
	{
		auto optional_fmat = modis_api::Gdal_operation::read_tif_to_fmat(file_path.string());
		if (optional_fmat)
			mat_list.push_back(*optional_fmat);
		else
			BOOST_LOG_TRIVIAL(error) << "��ȡdnֵʧ�ܣ�����" << file_path << "�Ĵ���";
	}

	// �жȾ����б�
	std::vector<arma::fmat> ef_mat_list;
	for (arma::Mat<float>& f_mat : mat_list)
	{
		auto ef_mat = Eddy_field::get_eddy_field_4(f_mat, 0);
		if (!ef_mat)
		{
			BOOST_LOG_TRIVIAL(error) << "���жȾ���ʱ���ִ��������þ���Ĵ���..";
			continue;
		}
		ef_mat_list.push_back(*ef_mat);
	}

	auto ef_mat_list_mean = modis_api::Mat_operation::mean_mat_by_each_pixel(ef_mat_list, 0);
	if (!ef_mat_list_mean)
	{
		BOOST_LOG_TRIVIAL(error) << "�жȾ����С��ͬ���޷����ɱ�������";
		BOOST_LOG_TRIVIAL(info) << "===============ʹ�÷���2���ɱ���������===========";
		BOOST_LOG_TRIVIAL(info) << "";
		return;
	}

	BOOST_LOG_TRIVIAL(info) << "�жȾ����ƽ��ֵ�������..";

	// д��tif�ļ�
	const path source_data_set = tif_files_for_ref_computing.front();
	const path dest_data_set = options.ref_image_file();
	BOOST_LOG_TRIVIAL(debug) << "Ŀ�������ļ���" << dest_data_set;

	if (exists(dest_data_set))
		remove(dest_data_set);
	// ��Ŀ¼�������򴴽�֮
	if (!exists(dest_data_set.parent_path()))
		create_directories(dest_data_set.parent_path());
	// Ȼ���ƹ�ȥ
	copy_file(source_data_set, dest_data_set);
	// д������
	BOOST_LOG_TRIVIAL(debug) << "COPY " << source_data_set << " to " << dest_data_set;
	modis_api::Gdal_operation::write_fmat_to_tif(dest_data_set.string(), *ef_mat_list_mean);

	BOOST_LOG_TRIVIAL(info) << "���ɵı������ļ�Ϊ��" << dest_data_set;
	BOOST_LOG_TRIVIAL(info) << "===============ʹ�÷���2���ɱ���������===========";
	BOOST_LOG_TRIVIAL(info) << "";
}
