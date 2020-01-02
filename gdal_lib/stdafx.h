// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once

#pragma warning(disable:4251)

#include "targetver.h"
#define BOOST_ALL_DYN_LINK
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
#define NOMINMAX
// Windows 头文件
#include <windows.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp> 
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>
#include <vector>
#include <xtensor/xaccumulator.hpp>
#include <xtensor/xadapt.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xcsv.hpp>
#include <xtensor/xindex_view.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xmasked_view.hpp>
#include <xtensor/xnpy.hpp>
#include <xtensor/xoptional.hpp>
#include <xtensor/xoptional_assembly.hpp>
#include <xtensor/xpad.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xreducer.hpp>
#include <xtensor/xsort.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>
#include <xtl/xoptional.hpp>

// 在此处引用程序需要的其他标头
