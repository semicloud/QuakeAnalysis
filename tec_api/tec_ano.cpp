#include "stdafx.h"
#include "tec_ano.h"
#include "timed_tensor.h"

std::vector<boost::filesystem::path> tec_api::list_all_files(boost::filesystem::path const& dir, std::string const& ext)
{
	std::vector<boost::filesystem::path> all_files;
	boost::filesystem::recursive_directory_iterator it{ dir };
	for (; it != boost::filesystem::recursive_directory_iterator{}; ++it)
	{
		if (it->path().extension() == ext)
			all_files.push_back(it->path());
	}
	return all_files;
}

int tec_api::sort_files_by_time_asc(std::vector<boost::filesystem::path>& paths)
{
	std::sort(paths.begin(), paths.end(), [](auto p1, auto p2)
	{
		boost::posix_time::ptime pt1 = parse_time(p1);
		boost::posix_time::ptime pt2 = parse_time(p2);
		return p1 <= p2;
	});
	return 0;
}

/**
 * \brief 根据基目录、起止日期、窗长获取tif文件路径
 * \param based_dir 基目录，如D:\tec\Standard\Codg
 * \param start 开始日期
 * \param stop 结束日期
 * \param wlen 窗长
 * \return
 */
std::vector<boost::filesystem::path> tec_api::get_data_paths(boost::filesystem::path const& based_dir,
	boost::gregorian::date start, boost::gregorian::date stop, size_t wlen)
{
	auto all_files = list_all_files(based_dir, ".tif");
	sort_files_by_time_asc(all_files);

	boost::posix_time::ptime start_time{ start, boost::posix_time::hours(0) };
	
	auto it1 = std::find_if(all_files.begin(), all_files.end(), [&start_time](boost::filesystem::path const& p) {return parse_time(p) == start_time; });
	if (it1 == all_files.end()) throw std::runtime_error("the file in start file not found!");
	if ((it1 - wlen) < all_files.begin()) throw std::runtime_error("no sufficient file!");
	const auto itw = it1 - wlen;
	std::vector<boost::filesystem::path> ans{ itw,it1 };

	for (; it1 != all_files.end(); ++it1)
	{
		const boost::gregorian::date file_date = parse_time(*it1).date();
		if (file_date >= start && file_date <= stop)
			ans.push_back(*it1);
	}

	sort_files_by_time_asc(ans);
	
	return ans;
}

/**
 * \brief 获取输出文件路径
 * \param base_dir 异常文件基目录
 * \param times 时间集合
 * \return 输出文件路径集合
 */
std::vector<boost::filesystem::path> tec_api::get_output_paths(boost::filesystem::path const& base_dir,
	std::vector<boost::posix_time::ptime> const& times)
{
	return std::vector<boost::filesystem::path>{};
}

xt::xtensor<size_t, 2> tec_api::window_indexes(size_t n, size_t w)
{
	xt::xarray<size_t> t1 = xt::arange<int>(0, int(n - w + 1));
	xt::xarray<size_t> t2 = xt::arange<int>(int(w - 1), int(n));
	xt::xtensor<size_t, 2> ans{ xt::transpose(xt::stack(xt::xtuple(t1, t2), 0)) };
	return  ans;
}
