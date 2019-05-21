# 震前多参数异常自动提取软件ADSMA开发及使用说明

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

## 2019/05/21

1. 初步完成rst_en算法，已发给焦博测试；
2. 开始ADSMA MODIS部分的主界面编写工作；
