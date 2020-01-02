// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

#ifndef PCH_H
#define PCH_H

#pragma warning(disable:4251)

#define NOMINMAX
#define BOOST_ALL_DYN_LINK
// TODO: 添加要在此处预编译的标头
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp> 
#include <boost/log/trivial.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
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
#include <xtensor/xview.hpp>
#include <xtl/xoptional.hpp>
#include	 <boost/format.hpp>
#endif //PCH_H
