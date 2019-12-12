#include "stdafx.h"
#include "tec_ano.h"

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
	std::vector<boost::filesystem::path> ans{
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\0.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\1.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\2.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\3.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\4.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\5.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\6.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\7.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\8.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\9.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\10.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\11.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\12.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\13.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\14.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\15.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\16.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\17.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\18.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\19.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\20.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\21.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\22.tif",
"D:\\tec\\Standard\\Codg\\2017\\7\\14\\23.tif"
	};
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
