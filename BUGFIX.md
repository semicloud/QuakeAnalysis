# BUG修正记录

## proc_MxD021km

## proc_MxD11A1

### iss01(Solved)

Date: 2019-05-30

Info: 使用MRT提取tif文件后，使用GDAL对提取的tif文件进行数据类型转换时`(-ot Float32)`，GDAL报出ERROR 5: OSRCalcInvFlattening(): Wrong input values错误，并且以后每次对该数据集进行GDAL操作均会报出这个错误；

Solution: 在转换数据格式时，使用如下语句：

> gdal_translate -a_srs "+proj=eqc +lat_ts=0 +lat_0=0 +lon_0=0 +x_0=0 +y_0=0 +ellps=WGS84 +datum=WGS84 +units=m +no_defs" -ot Float32 in.tif out.tif

对MRT导出的tif文件设置一下投影属性，就不会报出上述错误了，目前已更新至procMxD11A1.exe中。