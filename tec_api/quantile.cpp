#include "stdafx.h"
#include "quantile.h"

std::vector<double> quantiles(std::vector<double> const& vec)
{
	std::vector<double> ans;
	std::vector<double> v{ vec };
	std::sort(v.begin(), v.end());
	const size_t len = v.size();
	for (float q : std::vector<float>{ 0.25,0.5,0.75 })
		ans.push_back(gsl_stats_quantile_from_sorted_data(&v[0], 1, len, q));
	return ans;
}
