# 震前多参数异常自动提取软件ADSMA开发日志

## 2019/04/03

程序目录说明：

* HEG_Win\: HEG所在目录
* MRTSWath\: MRTSWath所在目录
* log\: 软件日志
* ymlsamples\: .yml配置文件示例及.hdf列表文件示例

程序调用示例：
> proc_MxD021km.exe --yml .\ymlsamples\proc_MxD021km.yml -d


## 2019/04/09

1. 可执行程序目录下添加了templates\文件夹，该文件夹下存储了可执行程序使用的若干文件模板；
2. 初步完成了proc_MxD11A1.exe程序

## 2019/04/12

基本完成了general_ano.exe程序，示例yml文件位于.\ymlsamples目录下，调用示例：

> general_ano.exe --yml .\ymlsamples\general_ano.yml

## 2019/04/17

1. 在焦博的帮助下，优化了04、05、11的预处理程序；
2. 亮温的lut表加入了对MYD数据的支持，加入了Aqua_MODIS_rad2BT_LUT.txt表；
3. 加入针对亮温数据的多波段支持，自动生成SDS和Suffix_bt（不支持30波段）;

## 2019/05/30

1. 初步完成了rst_en.exe，真是巨~~复杂~~有意思的一段代码
2. 全工程去除了DevExpress依赖，界面部分100%Pure Winform
3. 开始着手MODIS界面部分的编码，由于界面部分涉及较多人机交互，需要与焦博深入沟通后再编码
4. 初步完成了出图程序plot.py

## 2019/06/18

1. 修改了若干yml文件的配置项属性名，代码也做了相应的修改
2. 修正了若干程序不清空temp文件夹的bug
3. 修改目标文件名：eddy_field -> eddy_field_ano.exe， genenal_ano.exe -> juping_ano.exe
4. 将Aqua_MODIS_rad2BT_LUT.txt、Terra_MODIS_rad2BT_LUT.txt移动到data文件夹下
5. 更新Heg版本至2.15.1
6. 合并modis_plot分支
7. 新建TestData文件夹，用于存放测试数据以及准备测试数据的相关下载脚本

# 2019/11/05

1. 完成proc_MxD04_3k的重构工作；
2. 完成proc_MxD05_L2的重构工作；