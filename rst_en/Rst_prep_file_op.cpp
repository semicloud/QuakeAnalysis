#include "pch.h"
#include "../modis_api/Date_utils.h"
#include "../modis_api/File_operation.h"
#include "Rst_prep_file_op.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <set>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace fs = boost::filesystem;
namespace alg = boost::algorithm;
namespace gr = boost::gregorian;

Rst_prep_file_op::Rst_prep_file_op() = default;
Rst_prep_file_op::~Rst_prep_file_op() = default;


/**
 * \brief ���ݿ�ʼ���ںͽ������ڣ��Լ���С���������doy�ַ���
 * \param date_start ��ʼ����
 * \param date_end ��������
 * \param min_year ��С���
 * \return
 */
std::vector<std::string> Rst_prep_file_op::generate_doys(
	const gr::date& date_start,
	const gr::date& date_end,
	const unsigned& min_year)
{
	std::vector<std::string> ret;
	const std::string doy_start = modis_api::Date_utils::get_doy_str(date_start);
	const std::string doy_end = modis_api::Date_utils::get_doy_str(date_end);
	const std::string doy_day_start =
		modis_api::Date_utils::get_doy_day(doy_start);
	const std::string doy_day_end = modis_api::Date_utils::get_doy_day(doy_end);
	const std::vector<std::string> doy_span =
		modis_api::Date_utils::get_doy_day_span(doy_day_start, doy_day_end);
	const unsigned start_year = date_start.year();
	for (unsigned year = start_year; year != min_year - 1; year--)
	{
		for (const std::string& doy : doy_span)
		{
			const std::string gen_doy
				= boost::str(boost::format("%1%_%2%") % year % doy);
			ret.push_back(gen_doy);
		}
	}
	//BOOST_LOG_TRIVIAL(debug) << "���ɵ�" << date_start <<
	//	"��" << date_end << "����" << min_year << "�꣩��DOY��"
	//	<< alg::join(ret, ",");
	return ret;
}


/**
 * \brief ����ȫ�����ļ�·���б��ȡ�����Ϣ
 * \param all_file_paths ȫ��tif�ļ�·��
 * \return ���vector����������
 */
std::vector<unsigned> Rst_prep_file_op::get_file_years(
	const std::vector<std::string>& all_file_paths)
{
	std::set<unsigned> years;
	for (const auto& path : all_file_paths)
	{
		std::string year, doy;
		if (parse_file_year_doy(path, year, doy))
			years.insert(boost::lexical_cast<unsigned>(year));
	}
	std::vector<unsigned> ret = std::vector<unsigned>(years.begin(), years.end());
	std::vector<std::string> svec(ret.size());
	std::transform(ret.begin(), ret.end(), svec.begin(), [](const unsigned y)
	{ return std::to_string(y); });
	//BOOST_LOG_TRIVIAL(debug) << "ȫ����ݣ�" << alg::join(svec, ",");
	return ret;
}

/**
 * \brief ����������ȫ���ļ��л�ȡ�����ļ�
 * \param all_file_paths ȫ���ļ�·��
 * \param cur_date ����
 * \param k �����ļ���������Ϊk���ȡcur_dataǰ��k�������
 * \return �����ļ���·��
 */
std::vector<std::string> Rst_prep_file_op::get_adjacent_file_paths(
	const std::vector<std::string>& all_file_paths,
	const gr::date& cur_date, int k)
{
	std::vector<std::string> ret;
	std::vector<gr::date> pre_dates, next_dates;
	//BOOST_LOG_TRIVIAL(debug) << "��ǰ���ڣ�" << cur_date;

	const gr::date earliest_date = cur_date + gr::days(-k);
	for (auto it = gr::day_iterator(earliest_date); it != cur_date; ++it)
		pre_dates.push_back(*it);
	// BOOST_LOG_TRIVIAL(debug) << "��ǰ" << k << "������ڣ�";
	// for (gr::date& d : pre_dates)
	// 	BOOST_LOG_TRIVIAL(debug) << "\t" << d;

	const gr::date latest_date = cur_date + gr::days(k + 1);
	auto it = gr::day_iterator(cur_date + gr::days(1));
	for (; it != latest_date; ++it)
		next_dates.push_back(*it);
	// BOOST_LOG_TRIVIAL(debug) << "����" << k << "������ڣ�";
	// for (gr::date& d : next_dates)
	// 	BOOST_LOG_TRIVIAL(debug) << "\t" << d;

	std::vector<gr::date> adj_dates;
	std::copy(pre_dates.begin(), pre_dates.end(), back_inserter(adj_dates));
	std::copy(next_dates.begin(), next_dates.end(), back_inserter(adj_dates));

	for (const gr::date& adj_date : adj_dates)
	{
		const std::string doy =
			modis_api::Date_utils::get_doy_str_underline(adj_date);
		std::string file_path;
		if (exist_file_according_to_doy(all_file_paths, doy, file_path))
			ret.push_back(file_path);
	}

	BOOST_LOG_TRIVIAL(debug) << cur_date << "(" <<
		modis_api::Date_utils::get_doy_str_underline(cur_date) << ") ��"
		<< k << "���ٽ��ļ�Ϊ��" <<
		alg::join(modis_api::File_operation::get_file_names(ret), ",");
	return ret;
}

/**
 * \brief ����������ȫ���ļ��л�ȡ��ʷ�ļ�
 * \param all_file_paths ȫ���ļ�·��
 * \param cur_date ����
 * \param top_year ���δ����������
 * \return ��ʷ�ļ���·��
 */
std::vector<std::string> Rst_prep_file_op::get_hist_file_paths(
	const std::vector<std::string>& all_file_paths,
	const gr::date& cur_date, const unsigned top_year)
{
	std::vector<std::string> ret;
	// ��ǰ���ڵ�doy
	const std::string cur_doy =
		modis_api::Date_utils::get_doy_str_underline(cur_date);
	// ȫ�����
	const std::vector<unsigned> years = get_file_years(all_file_paths);
	// ��С���
	const unsigned min_year = *std::min_element(years.begin(), years.end());
	// ��ǰ���
	const unsigned cur_year = boost::lexical_cast<unsigned>
		(modis_api::Date_utils::get_doy_year_underline(cur_doy));
	// ��ǰ����doy��day����
	const std::string cur_day =
		modis_api::Date_utils::get_doy_day_underline(cur_doy);
	// ��С���ʹ��δ���������������o(�s���t)o
	if (cur_year == min_year)
	{
		for (const std::string& p : all_file_paths)
		{
			std::string str_year, str_day;
			if (parse_file_year_doy(p, str_year, str_day))
			{
				const unsigned file_year =
					boost::lexical_cast<unsigned>(str_year);
				if ((file_year <= top_year && file_year > cur_year)
					&& str_day == cur_day)
					ret.push_back(p);
			}
		}
		return ret;
	}
	for (const std::string& p : all_file_paths)
	{
		std::string str_year, file_day;
		if (parse_file_year_doy(p, str_year, file_day))
		{
			const unsigned file_year =
				boost::lexical_cast<unsigned>(str_year);
			if (file_year < cur_year && file_day == cur_day)
				ret.push_back(p);
		}
	}
	return ret;
}

/**
 * \brief ����doy�ַ�����ȡ�ļ�·��
 * \param all_file_paths �ļ�·������
 * \param doy doy�ַ���
 * \param out_file_path ����doy���ļ�·��
 * \return �����ļ��򷵻�true�������ڷ���false
 */
bool Rst_prep_file_op::exist_file_according_to_doy(
	const std::vector<std::string>& all_file_paths,
	const std::string& doy, std::string& out_file_path)
{
	const auto p = std::find_if(all_file_paths.begin(), all_file_paths.end(),
		[&doy](const std::string& s)
	{
		return s.find(doy) != std::string::npos;
	});
	if (p != all_file_paths.end())
	{
		out_file_path = *p;
		return true;
	}
	BOOST_LOG_TRIVIAL(warning) << "file related to " << doy << " not found!";
	return false;
}

/**
 * \brief ��doy�����л�ȡĳ��ݵ�doy
 * \param year ���
 * \param all_doys doy����
 * \return year��ݵ�doy����
 */
std::vector<std::string> Rst_prep_file_op::get_doys_by_year(const unsigned year,
	const std::vector<std::string>& all_doys)
{
	std::vector<std::string> ret;
	for (const std::string& doy : all_doys)
	{
		if (modis_api::Date_utils::get_doy_year(doy) == std::to_string(year))
			ret.push_back(doy);
	}
	std::sort(ret.begin(), ret.end());
	BOOST_LOG_TRIVIAL(debug) << year << "���DOY��" <<
		boost::algorithm::join(ret, ",");
	return ret;
}

/**
 * \brief
 * \return
 */
std::vector<std::string> Rst_prep_file_op::get_files_by_doys(
	const std::vector<std::string>& all_file_paths,
	const std::vector<std::string>& doys)
{
	std::vector<std::string> ret;
	for (const std::string& doy : doys)
	{
		for (const std::string& file : all_file_paths)
		{
			if (file.find(doy) != std::string::npos)
				ret.push_back(file);
		}
	}

	std::sort(ret.begin(), ret.end());

	std::vector<std::string> file_names;
	std::transform(ret.begin(), ret.end(), back_inserter(file_names),
		[](auto& f) {return fs::path(f).filename().string(); });
	BOOST_LOG_TRIVIAL(debug) << "����DOY�ҵ����ļ���" <<
		boost::algorithm::join(file_names, ",");
	return ret;
}

/**
 * \brief ��ȡ�ļ����е���ݺ�doy��Ϣ
 * \param file_path �ļ�·��
 * \param year ��������year
 * \param doy ��������doy
 */
bool Rst_prep_file_op::parse_file_year_doy(const std::string& file_path,
	std::string& year, std::string& doy)
{
	const std::string file_name_without_ex = fs::path(file_path).
		filename().stem().string();
	const auto sep_number = std::count(file_name_without_ex.begin(),
		file_name_without_ex.end(), '_');
	if (sep_number != 2)
	{
		BOOST_LOG_TRIVIAL(error) << "�޷���" << file_path
			<< "�н�����ݼ�doy��Ϣ��";
		return false;
	}
	std::vector<std::string> strs;
	boost::split(strs, file_name_without_ex, boost::is_any_of("_"));
	//BOOST_LOG_TRIVIAL(debug) << "Split file name: " << file_name_without_ex
	//	<< ", result: " << boost::algorithm::join(strs, ", ");
	year = strs[1];
	doy = strs[2];
	//BOOST_LOG_TRIVIAL(debug) << "Parse year and doy: " << year << ", " << doy;
	return true;
}

/**
 * \brief ����doy�����ص������Ƿ���ڣ����������ڵ����ݷŵ���3��������
 * \param generated_doys ���ɵ�doy�б�
 * \param all_files �ļ��б�
 * \param doys_file_not_exist ����������ļ���doy
 * \return
 */
bool Rst_prep_file_op::all_data_ready(
	const std::vector<std::string>& generated_doys,
	const std::vector<std::string>& all_files,
	std::vector<std::string>& doys_file_not_exist)
{
	for (const std::string& doy : generated_doys)
	{
		if (std::find_if(all_files.begin(), all_files.end(),
			[&doy](const std::string& f) {
			return f.find(doy) != std::string::npos;
		}) == all_files.end())
			doys_file_not_exist.push_back(doy);
	}
	if (!doys_file_not_exist.empty())
		BOOST_LOG_TRIVIAL(debug) << "�ҵ�"
		<< doys_file_not_exist.size() << "��" << "����������ļ���doy��"
		<< alg::join(doys_file_not_exist, ",");
	return doys_file_not_exist.empty();
}


