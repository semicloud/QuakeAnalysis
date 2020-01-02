#pragma once

#include <vector>
#include <optional>
#include <armadillo>

namespace proc_hrtec
{
	class  HrtecItem
	{
	public:
		HrtecItem() = default;
		HrtecItem(const int y, const int m, const int d, const int h, std::optional<arma::fmat> mat)
			: m_year(y), m_month(m), m_day(d), m_hour(h), m_mat(mat) {}
		explicit HrtecItem(const std::vector<std::string>& dataSec);
		~HrtecItem();
		static std::vector<HrtecItem> load_items(const std::string& file_path_str);

		int year() const
		{
			return m_year;
		}

		int month() const
		{
			return m_month;
		}

		int day() const
		{
			return m_day;
		}

		int hour() const
		{
			return m_hour;
		}

		std::optional<arma::fmat> mat() const
		{
			return m_mat;
		}

	private:
		int m_year;
		int m_month;
		int m_day;
		int m_hour;
		std::optional<arma::fmat> m_mat;
	};
}

