// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once
#pragma warning(disable:4251)

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容

#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK				// 指定BOOST库全部以动态链接形式装载
#endif

// Windows 头文件
#define NOMINMAX
#include <windows.h>


// 在此处引用程序需要的其他标头
