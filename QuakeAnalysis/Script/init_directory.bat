rem 本脚本用于初始化电离层数据处理软件的MODIS数据管理目录

echo 当前路径 %cd%
echo 下面开始初始化MODIS数据管理目录

%初始化数据存储目录%
mkdir modis_workspace\MXD02\MOD02
mkdir modis_workspace\MXD02\MYD02
mkdir modis_workspace\MXD03\MOD03
mkdir modis_workspace\MXD03\MYD03
mkdir modis_workspace\MXD04\MOD04
mkdir modis_workspace\MXD04\MYD04
mkdir modis_workspace\MXD05\MOD05
mkdir modis_workspace\MXD05\MYD05
mkdir modis_workspace\MXD11\MOD11
mkdir modis_workspace\MXD11\MYD11
mkdir modis_workspace\MXD35\MOD35
mkdir modis_workspace\MXD35\MYD35

%创建预处理数据目录%
%亮温%
mkdir modis_workspace\Standard\BT\BT_MOD
mkdir modis_workspace\Standard\BT\BT_MYD
%LST%
mkdir modis_workspace\Standard\LST\LST_MOD
mkdir modis_workspace\Standard\LST\LST_MYD
%水汽 WaterVapor%
mkdir modis_workspace\Standard\WaterVapor\WaterVapor_MOD
mkdir modis_workspace\Standard\WaterVapor\WaterVapor_MYD
%云掩膜？ Aerosol%
mkdir modis_workspace\Standard\Aerosol\Aerosol_MOD
mkdir modis_workspace\Standard\Aerosol\Aerosol_MYD

%创建背景场数据目录%
%亮温%
mkdir modis_workspace\Ref\BT\BT_MOD
mkdir modis_workspace\Ref\BT\BT_MYD
%LST%
mkdir modis_workspace\Ref\LST\LST_MOD
mkdir modis_workspace\Ref\LST\LST_MYD
%水汽 WaterVapor%
mkdir modis_workspace\Ref\WaterVapor\WaterVapor_MOD
mkdir modis_workspace\Ref\WaterVapor\WaterVapor_MYD
%云掩膜？ Aerosol%
mkdir modis_workspace\Ref\Aerosol\Aerosol_MOD
mkdir modis_workspace\Ref\Aerosol\Aerosol_MYD

%创建异常数据目录%
%亮温%
mkdir modis_workspace\Ano\BT\BT_MOD
mkdir modis_workspace\Ano\BT\BT_MYD
%LST%
mkdir modis_workspace\Ano\LST\LST_MOD
mkdir modis_workspace\Ano\LST\LST_MYD
%水汽 WaterVapor%
mkdir modis_workspace\Ano\WaterVapor\WaterVapor_MOD
mkdir modis_workspace\Ano\WaterVapor\WaterVapor_MYD
%云掩膜？ Aerosol%
mkdir modis_workspace\Ano\Aerosol\Aerosol_MOD
mkdir modis_workspace\Ano\Aerosol\Aerosol_MYD

%创建预处理后数据的出图数据目录%
%亮温%
mkdir modis_workspace\Std_Map\BT\BT_MOD
mkdir modis_workspace\Std_Map\BT\BT_MYD
%LST%
mkdir modis_workspace\Std_Map\LST\LST_MOD
mkdir modis_workspace\Std_Map\LST\LST_MYD
%水汽 WaterVapor%
mkdir modis_workspace\Std_Map\WaterVapor\WaterVapor_MOD
mkdir modis_workspace\Std_Map\WaterVapor\WaterVapor_MYD
%云掩膜？ Aerosol%
mkdir modis_workspace\Std_Map\Aerosol\Aerosol_MOD
mkdir modis_workspace\Std_Map\Aerosol\Aerosol_MYD

%创建背景场的出图数据目录%
%亮温%
mkdir modis_workspace\Ref_Map\BT\BT_MOD
mkdir modis_workspace\Ref_Map\BT\BT_MYD
%LST%
mkdir modis_workspace\Ref_Map\LST\LST_MOD
mkdir modis_workspace\Ref_Map\LST\LST_MYD
%水汽 WaterVapor%
mkdir modis_workspace\Ref_Map\WaterVapor\WaterVapor_MOD
mkdir modis_workspace\Ref_Map\WaterVapor\WaterVapor_MYD
%云掩膜？ Aerosol%
mkdir modis_workspace\Ref_Map\Aerosol\Aerosol_MOD
mkdir modis_workspace\Ref_Map\Aerosol\Aerosol_MYD

%创建异常数据的出图数据目录%
%亮温%
mkdir modis_workspace\Ano_Map\BT\BT_MOD
mkdir modis_workspace\Ano_Map\BT\BT_MYD
%LST%
mkdir modis_workspace\Ano_Map\LST\LST_MOD
mkdir modis_workspace\Ano_Map\LST\LST_MYD
%水汽 WaterVapor%
mkdir modis_workspace\Ano_Map\WaterVapor\WaterVapor_MOD
mkdir modis_workspace\Ano_Map\WaterVapor\WaterVapor_MYD
%云掩膜？ Aerosol%
mkdir modis_workspace\Ano_Map\Aerosol\Aerosol_MOD
mkdir modis_workspace\Ano_Map\Aerosol\Aerosol_MYD

%矢量等值线数据目录%
%亮温%
mkdir modis_workspace\Contour_Data\BT\BT_MOD
mkdir modis_workspace\Contour_Data\BT\BT_MYD
%LST%
mkdir modis_workspace\Contour_Data\LST\LST_MOD
mkdir modis_workspace\Contour_Data\LST\LST_MYD
%水汽 WaterVapor%
mkdir modis_workspace\Contour_Data\WaterVapor\WaterVapor_MOD
mkdir modis_workspace\Contour_Data\WaterVapor\WaterVapor_MYD
%云掩膜？ Aerosol%
mkdir modis_workspace\Contour_Data\Aerosol\Aerosol_MOD
mkdir modis_workspace\Contour_Data\Aerosol\Aerosol_MYD

%临时文件目录%
mkdir modis_workspace\Tmp

echo 初始化数据管理目录结束..