#include "stdafx.h"
#include "tec_ano.h"

/**
 * \brief ���ݻ�Ŀ¼����ֹ���ڡ�������ȡtif�ļ�·��
 * \param based_dir ��Ŀ¼����D:\tec\Standard\Codg
 * \param start ��ʼ����
 * \param stop ��������
 * \param wlen ����
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
 * \brief ��ȡ����ļ�·��
 * \param base_dir �쳣�ļ���Ŀ¼
 * \param times ʱ�伯��
 * \return ����ļ�·������
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
