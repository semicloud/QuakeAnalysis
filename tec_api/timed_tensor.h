#pragma once

namespace tec_api
{
	template <typename T = double, unsigned D = 2>
	class __declspec(dllexport) timed_tensor
	{
		friend bool operator<(timed_tensor const& lhs, timed_tensor const& rhs)
		{
			return lhs.time() < rhs.time();
		}
	public:
		timed_tensor() : time_(boost::posix_time::not_a_date_time), tensor_ptr_(nullptr) {}
		timed_tensor(boost::posix_time::ptime time, std::shared_ptr<xt::xtensor<T, D>> tensor_ptr) : time_(time), tensor_ptr_(tensor_ptr) {}
		timed_tensor(timed_tensor const& other) : time_(other.time_), tensor_ptr_(other.tensor_ptr_) {}
		timed_tensor& operator=(timed_tensor const& rhs)
		{
			if (this == &rhs) return *this;
			time_ = rhs.time_;
			tensor_ptr_ = rhs.tensor_ptr_;
			return *this;
		}
		~timed_tensor() = default;

		boost::posix_time::ptime time() const { return time_; }
		std::shared_ptr<xt::xtensor<T, D>> tensor_ptr() const { return tensor_ptr_; }

	private:
		boost::posix_time::ptime time_;
		std::shared_ptr<xt::xtensor<T, D>> tensor_ptr_;
	};

	__declspec(dllexport) boost::posix_time::ptime __cdecl parse_time(boost::filesystem::path const&);
	__declspec(dllexport) xt::xtensor<double, 2 > __cdecl load_tif(boost::filesystem::path const&);

	template<typename T = double, size_t D = 2>
	xt::xtensor<T, D - 1> skipped_sum(xt::xtensor<T, D> const& tensor, size_t axis, T skipped_val)
	{
		auto reducer = [=](T t1, T t2)
		{
			if (t1 == skipped_val && t2 == skipped_val)
				return skipped_val;
			if (t1 == skipped_val && t2 != skipped_val)
				return t2;
			if (t1 != skipped_val && t2 == skipped_val)
				return t1;
			return t1 + t2;
		};
		return xt::reduce(reducer, tensor, { axis });
	}

	template<typename T = double, size_t D = 2>
	xt::xtensor<size_t, D - 1> skipped_count(xt::xtensor<T, D> const& tensor, size_t axis, T skipped_val)
	{
		xt::xtensor<size_t, D - 1> ans = xt::sum(xt::not_equal(tensor, skipped_val), { axis });
		return ans;
	}

	template<typename T = double, size_t D = 2>
	xt::xtensor<T, D - 1> skipped_mean(xt::xtensor<T, D> const& tensor, size_t axis, T skipped_val)
	{
		xt::xtensor<T, D - 1> sums = skipped_sum(tensor, axis, skipped_val);
		xt::xtensor<size_t, D - 1> cnt = skipped_count(tensor, axis, skipped_val);
		return xt::xtensor<T, D - 1>{sums / cnt};
	}

	template<typename T = double>
	xt::xtensor<double, 2> skipped_stddev(xt::xtensor<T, 3> const& tensor, T skipped_val)
	{
		const typename xt::xtensor<T, 3>::shape_type shp = tensor.shape();
		const size_t row_num = shp.at(1);
		const size_t col_num = shp.at(2);
		std::vector<size_t> ans_shp{ shp(1),shp(2) };
		xt::xtensor<double, 2> ans{ xt::zeros<double>{ans_shp} };
		for (size_t row = 0; row != row_num; ++row)
		{
			for (size_t col = 0; col != col_num; ++col)
			{
				xt::xtensor<double, 1> z{ xt::view(tensor, xt::all(), row, col) };
			}
		}
	}

	// template<typename T=double, size_t D=2>

	/**
	 * \brief 时间张量序列
	 * \tparam T 时间张量的数据类型，默认为double
	 * \tparam D 时间张量的维度，默认为2
	 */
	template<typename T = double, unsigned D = 2>
	class __declspec(dllexport) timed_tensor_series
	{
	public:
		timed_tensor_series() : timed_tensors_(std::vector<timed_tensor<T, D>>{}) {}
		~timed_tensor_series() = default;
		static timed_tensor_series load_from_files(std::vector<boost::filesystem::path> const&);
		timed_tensor_series sub_series(size_t, size_t) const;
		void add(timed_tensor<T, D> const&);
		size_t size() const& { return timed_tensors_.size(); }
		timed_tensor<T, D>& operator[](size_t);
		std::vector<boost::posix_time::ptime> get_times() const;
		xt::xtensor<T, D + 1> get_tensor_as_whole() const;
		void sort();
	private:
		std::vector<timed_tensor<T, D>> timed_tensors_;
	};

	template <typename T, unsigned D>
	void timed_tensor_series<T, D>::add(timed_tensor<T, D> const& tt)
	{
		timed_tensors_.push_back(tt);
	}

	template <typename T, unsigned D>
	timed_tensor<T, D>& timed_tensor_series<T, D>::operator[](size_t idx)
	{
		return timed_tensors_.at(idx);
	}

	template <typename T, unsigned D>
	std::vector<boost::posix_time::ptime> timed_tensor_series<T, D>::get_times() const
	{
		std::vector<boost::posix_time::ptime> times;
		std::transform(timed_tensors_.cbegin(), timed_tensors_.cend(), std::back_inserter(times));
		return times;
	}

	template <typename T, unsigned D>
	xt::xtensor<T, D + 1> timed_tensor_series<T, D>::get_tensor_as_whole() const
	{
		if (timed_tensors_.empty())
			throw std::runtime_error("timed_tensors is empty!");
		std::vector<T> container;
		const size_t C = timed_tensors_.size();
		const size_t NROW = timed_tensors_.at(0).tensor_ptr()->shape(0);
		const size_t NCOL = timed_tensors_.at(0).tensor_ptr()->shape(1);
		for (timed_tensor<T, D> tt : timed_tensors_)
		{
			std::copy(tt.tensor_ptr()->template begin<xt::layout_type::row_major>(),
				tt.tensor_ptr()->template end<xt::layout_type::row_major>(),
				std::back_inserter(container));
		}
		xt::xarray<T> arr = xt::adapt(container);
		arr.reshape({ C,NROW,NCOL });
		return arr;
	}

	template <typename T, unsigned D>
	void timed_tensor_series<T, D>::sort()
	{
		std::sort(timed_tensors_.begin(), timed_tensors_.end());
	}

	template <typename T, unsigned D>
	timed_tensor_series<T, D> timed_tensor_series<T, D>::load_from_files(std::vector<boost::filesystem::path> const& paths)
	{
		timed_tensor_series series;
		for (const boost::filesystem::path& path : paths)
		{
			boost::posix_time::ptime time = parse_time(path);
			std::shared_ptr<xt::xtensor<T, D>> ptr = std::make_shared<xt::xtensor<double, 2>>(load_tif(path));
			series.add(timed_tensor<T, D>(time, ptr));
		}
		series.sort();
		return series;
	}

	template <typename T, unsigned D>
	timed_tensor_series<T, D> timed_tensor_series<T, D>::sub_series(size_t start, size_t stop) const
	{
		timed_tensor_series<T, D> ans;
		for (size_t i = start; i != stop + 1; ++i)
			ans.add(timed_tensors_.at(i));
		return ans;
	}
}
