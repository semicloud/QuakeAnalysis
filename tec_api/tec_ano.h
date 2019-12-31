#pragma once
#include "timed_tensor.h"
namespace tec_api
{
	__declspec(dllexport) std::vector<boost::filesystem::path> __cdecl list_all_files(boost::filesystem::path const& dir, std::string const& ext);

	__declspec(dllexport) int __cdecl sort_files_by_time_asc(std::vector<boost::filesystem::path>& paths);

	__declspec(dllexport) std::vector<boost::filesystem::path> __cdecl get_data_paths(boost::filesystem::path const& based_dir,
		boost::gregorian::date start, boost::gregorian::date stop, size_t wlen);

	std::vector<boost::filesystem::path> get_output_paths(boost::filesystem::path const& base_dir,
		std::vector<boost::posix_time::ptime> const& times);


	/**
	 * \brief 生成滑动窗口的索引值
	 * \param n 元素个数
	 * \param w 窗口长度
	 * \return 窗口个数*2矩阵，每1行是一个窗口
	 */
	__declspec(dllexport) xt::xtensor<size_t, 2> __cdecl window_indexes(size_t n, size_t w);

	/**
	 * \brief 电离层-异常提取-滑动平均法
	 * \tparam T 数据类型，默认为double
	 * \tparam OD 输出数据的维度，默认为3维，即x_j-mu(X)、sigma(X)
	 * \tparam ID 输入数据的维度，默认为2维
	 * \param data 输入数据
	 * \param wlen 窗长
	 * \return （时刻-张量）序列
	 */
	template<typename T = double, unsigned OD = 3, unsigned ID = 2>
	timed_tensor_series<T, OD> ano_slid_win(timed_tensor_series<T, ID> const& data, size_t wlen)
	{
		const size_t N = data.size();
		const xt::xtensor<size_t, 2> windows = window_indexes(N, wlen);
		const xt::xtensor<size_t, 2>::shape_type win_shp = windows.shape();
		const size_t n_rows = win_shp.at(0);
		const size_t n_cols = win_shp.at(1);
		assert(n_cols == 2);
		timed_tensor_series<T, OD> ans;
		for (size_t row = 0; row != n_rows; ++row)
		{
			xt::xtensor<size_t, 1> win{ xt::view(windows, row, xt::all()) };
			timed_tensor_series<T, 2> sub_series = data.sub_series(win.at(0), win.at(1));
			timed_tensor<T, 2> last = sub_series[sub_series.size() - 1];
			xt::xtensor<T, ID + 1> cube = sub_series.get_tensor_as_whole();
			xt::xtensor<T, ID> mean = xt::nan_to_num(xt::nanmean(cube, { 0 }));
			xt::xtensor<T, ID> diff = xt::nan_to_num(*last.tensor_ptr() - mean);
			xt::xtensor<T, ID> sigma = xt::nan_to_num(xt::nanstd(cube, { 0 }));
			std::shared_ptr<xt::xtensor<T, OD>> ptr =
				std::make_shared<xt::xtensor<T, OD>>(xt::stack(xt::xtuple(diff, mean, sigma)));
			// std::shared_ptr<xt::xtensor<T, OD>> ptr =
			// 	std::make_shared<xt::xtensor<T, OD>>(xt::stack(xt::xtuple(diff, sigma)));
			timed_tensor<T, OD> tt{ last.time(), ptr };
			ans.add(tt);
		}
		return ans;
	}

	/**
	 * \brief 电离层-异常提取-四分位法
	 * \tparam T 数据类型，默认为double
	 * \tparam OD 输出数据的维度，默认为2维
	 * \tparam ID 输入数据的维度，默认为2维
	 * \param data 输入数据
	 * \param wlen 窗长
	 * \return
	 */
	template<typename T = double, unsigned OD = 2, unsigned ID = 2>
	timed_tensor_series<T, OD> ano_quantile(timed_tensor_series<T, ID> const& data, size_t wlen);



}