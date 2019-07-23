出图标题title,字号
指定出图范围（包含跨度）：最小经度,最大经度,经度间隔,最小纬度...
Color bar标题,字号
网格，字号
地图：
	背景场、原始数据：国家边界
	异常数据：边界、断裂带、地震目录、主要城市
		断裂带：灰色
		地震目录：给定的范围
		主要城市：黑色实心三角形
		
cython:单个的python文件转成c代码，转成exe
python转成exe

imshow->改成mesh color


预处理：
预处理时，不用分别设置MODh和MYD参数，设置一个MxD就可以

界面，流程部分分组，归档、预处理、标准数据出图一组、异常处理方法一组

界面上加入地理范围

有两种地理范围：预处理地理范围和出图地理范围

加一个设置按钮，把只需要设置一次的参数放到里面，比如预处理地理范围

## 2019-06-26

* proc_MxD04_3K，默认值改为-1 OK
* proc_MxD05_L2，读取GeoBounds属性从坐标矩阵中读取，不要从属性中读取 OK
* proc_MOD11A1，如果投影为ER，则Datum设为NoDatum，其他投影设为WGS84 OK
* 气溶胶AOD，水汽WV，地表温度LST，亮温BT
* 偏移指数_bat代码，偏移指数-多波段融合与质量检测 看不懂..
* 主yml的设计 部分OK
* 界面上，第二个流程改成“异常提取方法”， OK
* 生成等值线
* 涡度：方法Groupbox去掉，把方法1和方法2移动到计算涡度后面
* 地理范围加上空间说明
* 界面上，时间显示范围尽量小
* 做矩阵平均值计算时，读取第一个tif的nodatavalue作为nodatavalue

## 2019-07-16

需确定：
预处理生成的tif文件名命名规则：
BT_yyyy_doy.tif, AOD_yyyy_doy.tif WV_yyyy_doy.tif LST_yyyy_doy.tif

涡度及其背景场tif文件名命名规则：
* 方法1和方法2都这样存储，在存储目录上进行区分
* BT_ANO_yyyy_doy.tif BT_REF_yyyy_doy.tif
* AOD_REF_yyyy_doy.tif WV_REF_yyyy_doy.tif LST_REF_yyyy_doy.tif 
* 年变及其背景场tif文件名命名规则
			BT_ANO_yyyy_doy.tif ...
		BT_REF_yyyy_doy.tif ...
	rst算法tif文件名命名规则

TEC部分，几种数据的预处理方法
代码规范，提前约定

codg、jplg、rim放到一起

预处理数据集存放路径：

*modis_workspace*/Standard/MOD_BT/YYYY/BT_YYYY_DOY.tif