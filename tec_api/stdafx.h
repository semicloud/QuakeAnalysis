// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once

#define BOOST_ALL_DYN_LINK

#pragma warning(disable: 4251; disable: 4003)

#include "targetver.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

// 在此处引用程序需要的其他标头
// TODO 语言的混合模式修改成了否
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/string_file.hpp>
#include <boost/format.hpp>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>
#include <vector>
#include <gsl/gsl_statistics_double.h>
#include <string>
#include <vector>
#include <xtensor/xaccumulator.hpp>
#include <xtensor/xadapt.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xbuilder.hpp>
#include <xtensor/xindex_view.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xmasked_view.hpp>
#include <xtensor/xpad.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xsort.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

