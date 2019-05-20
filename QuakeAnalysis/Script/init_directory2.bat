rem 本脚本用于初始化电离层数据处理软件的MODIS数据管理目录

echo 当前路径 %cd%
echo 下面开始初始化MODIS数据管理目录..

rem 创建原始数据存储目录..
mkdir modis_workspace\MOD021km
mkdir modis_workspace\MYD021km
mkdir modis_workspace\MOD04_3k
mkdir modis_workspace\MYD04_3k
mkdir modis_workspace\MOD05_L2
mkdir modis_workspace\MYD05_L2
mkdir modis_workspace\MOD11A1
mkdir modis_workspace\MYD11A1
mkdir modis_workspace\MOD35_L2
mkdir modis_workspace\MYD35_L2

rem 创建预处理数据目录..
mkdir modis_workspace\Standard
mkdir modis_workspace\Standard\MOD_BT
mkdir modis_workspace\Standard\MYD_BT
mkdir modis_workspace\Standard\MOD_LST
mkdir modis_workspace\Standard\MYD_LST
mkdir modis_workspace\Standard\MOD_TPW
mkdir modis_workspace\Standard\MYD_TPW
mkdir modis_workspace\Standard\MOD_AOD
mkdir modis_workspace\Standard\MYD_AOD

rem 创建背景场数据目录..
mkdir modis_workspace\Ref
mkdir modis_workspace\Ref\EF_MOD_BT
mkdir modis_workspace\Ref\EF_MYD_BT
mkdir modis_workspace\Ref\GA_MOD_BT
mkdir modis_workspace\Ref\GA_MYD_BT
mkdir modis_workspace\Ref\RST_MOD_BT
mkdir modis_workspace\Ref\RST_MYD_BT
%-------------------------------------%
mkdir modis_workspace\Ref\EF_MOD_LST
mkdir modis_workspace\Ref\EF_MYD_LST
mkdir modis_workspace\Ref\GA_MOD_LST
mkdir modis_workspace\Ref\GA_MYD_LST
mkdir modis_workspace\Ref\RST_MOD_LST
mkdir modis_workspace\Ref\RST_MYD_LST
%-------------------------------------%
mkdir modis_workspace\Ref\EF_MOD_TPW
mkdir modis_workspace\Ref\EF_MYD_TPW
mkdir modis_workspace\Ref\GA_MOD_TPW
mkdir modis_workspace\Ref\GA_MYD_TPW
mkdir modis_workspace\Ref\RST_MOD_TPW
mkdir modis_workspace\Ref\RST_MYD_TPW
%-------------------------------------%
mkdir modis_workspace\Ref\EF_MOD_AOD
mkdir modis_workspace\Ref\EF_MYD_AOD
mkdir modis_workspace\Ref\GA_MOD_AOD
mkdir modis_workspace\Ref\GA_MYD_AOD
mkdir modis_workspace\Ref\RST_MOD_AOD
mkdir modis_workspace\Ref\RST_MYD_AOD
%-------------------------------------%

rem 创建异常数据目录..
mkdir modis_workspace\Ano
mkdir modis_workspace\Ano\EF_MOD_BT
mkdir modis_workspace\Ano\EF_MYD_BT
mkdir modis_workspace\Ano\GA_MOD_BT
mkdir modis_workspace\Ano\GA_MYD_BT
mkdir modis_workspace\Ano\RST_MOD_BT
mkdir modis_workspace\Ano\RST_MYD_BT
%-------------------------------------%
mkdir modis_workspace\Ano\EF_MOD_LST
mkdir modis_workspace\Ano\EF_MYD_LST
mkdir modis_workspace\Ano\GA_MOD_LST
mkdir modis_workspace\Ano\GA_MYD_LST
mkdir modis_workspace\Ano\RST_MOD_LST
mkdir modis_workspace\Ano\RST_MYD_LST
%-------------------------------------%
mkdir modis_workspace\Ano\EF_MOD_TPW
mkdir modis_workspace\Ano\EF_MYD_TPW
mkdir modis_workspace\Ano\GA_MOD_TPW
mkdir modis_workspace\Ano\GA_MYD_TPW
mkdir modis_workspace\Ano\RST_MOD_TPW
mkdir modis_workspace\Ano\RST_MYD_TPW
%-------------------------------------%
mkdir modis_workspace\Ano\EF_MOD_AOD
mkdir modis_workspace\Ano\EF_MYD_AOD
mkdir modis_workspace\Ano\GA_MOD_AOD
mkdir modis_workspace\Ano\GA_MYD_AOD
mkdir modis_workspace\Ano\RST_MOD_AOD
mkdir modis_workspace\Ano\RST_MYD_AOD
%-------------------------------------%

rem 创建预处理后数据的出图数据目录..
mkdir modis_workspace\Std_Map
mkdir modis_workspace\Std_Map\MOD_BT
mkdir modis_workspace\Std_Map\MYD_BT
mkdir modis_workspace\Std_Map\MOD_LST
mkdir modis_workspace\Std_Map\MYD_LST
mkdir modis_workspace\Std_Map\MOD_TPW
mkdir modis_workspace\Std_Map\MYD_TPW
mkdir modis_workspace\Std_Map\MOD_AOD
mkdir modis_workspace\Std_Map\MYD_AOD


rem 创建背景场的出图数据目录..
mkdir modis_workspace\Ref_Map
mkdir modis_workspace\Ref_Map\EF_MOD_BT
mkdir modis_workspace\Ref_Map\EF_MYD_BT
mkdir modis_workspace\Ref_Map\GA_MOD_BT
mkdir modis_workspace\Ref_Map\GA_MYD_BT
mkdir modis_workspace\Ref_Map\RST_MOD_BT
mkdir modis_workspace\Ref_Map\RST_MYD_BT
%-------------------------------------%
mkdir modis_workspace\Ref_Map\EF_MOD_LST
mkdir modis_workspace\Ref_Map\EF_MYD_LST
mkdir modis_workspace\Ref_Map\GA_MOD_LST
mkdir modis_workspace\Ref_Map\GA_MYD_LST
mkdir modis_workspace\Ref_Map\RST_MOD_LST
mkdir modis_workspace\Ref_Map\RST_MYD_LST
%-------------------------------------%
mkdir modis_workspace\Ref_Map\EF_MOD_TPW
mkdir modis_workspace\Ref_Map\EF_MYD_TPW
mkdir modis_workspace\Ref_Map\GA_MOD_TPW
mkdir modis_workspace\Ref_Map\GA_MYD_TPW
mkdir modis_workspace\Ref_Map\RST_MOD_TPW
mkdir modis_workspace\Ref_Map\RST_MYD_TPW
%-------------------------------------%
mkdir modis_workspace\Ref_Map\EF_MOD_AOD
mkdir modis_workspace\Ref_Map\EF_MYD_AOD
mkdir modis_workspace\Ref_Map\GA_MOD_AOD
mkdir modis_workspace\Ref_Map\GA_MYD_AOD
mkdir modis_workspace\Ref_Map\RST_MOD_AOD
mkdir modis_workspace\Ref_Map\RST_MYD_AOD
%-------------------------------------%

rem 创建异常数据的出图数据目录..
mkdir modis_workspace\Ano_Map
mkdir modis_workspace\Ano_Map\EF_MOD_BT
mkdir modis_workspace\Ano_Map\EF_MYD_BT
mkdir modis_workspace\Ano_Map\GA_MOD_BT
mkdir modis_workspace\Ano_Map\GA_MYD_BT
mkdir modis_workspace\Ano_Map\RST_MOD_BT
mkdir modis_workspace\Ano_Map\RST_MYD_BT
%-------------------------------------%
mkdir modis_workspace\Ano_Map\EF_MOD_LST
mkdir modis_workspace\Ano_Map\EF_MYD_LST
mkdir modis_workspace\Ano_Map\GA_MOD_LST
mkdir modis_workspace\Ano_Map\GA_MYD_LST
mkdir modis_workspace\Ano_Map\RST_MOD_LST
mkdir modis_workspace\Ano_Map\RST_MYD_LST
%-------------------------------------%
mkdir modis_workspace\Ano_Map\EF_MOD_TPW
mkdir modis_workspace\Ano_Map\EF_MYD_TPW
mkdir modis_workspace\Ano_Map\GA_MOD_TPW
mkdir modis_workspace\Ano_Map\GA_MYD_TPW
mkdir modis_workspace\Ano_Map\RST_MOD_TPW
mkdir modis_workspace\Ano_Map\RST_MYD_TPW
%-------------------------------------%
mkdir modis_workspace\Ano_Map\EF_MOD_AOD
mkdir modis_workspace\Ano_Map\EF_MYD_AOD
mkdir modis_workspace\Ano_Map\GA_MOD_AOD
mkdir modis_workspace\Ano_Map\GA_MYD_AOD
mkdir modis_workspace\Ano_Map\RST_MOD_AOD
mkdir modis_workspace\Ano_Map\RST_MYD_AOD
%-------------------------------------%

rem 创建矢量等值线数据目录..
mkdir modis_workspace\Contour
mkdir modis_workspace\Contour\EF_MOD_BT
mkdir modis_workspace\Contour\EF_MYD_BT
mkdir modis_workspace\Contour\GA_MOD_BT
mkdir modis_workspace\Contour\GA_MYD_BT
mkdir modis_workspace\Contour\RST_MOD_BT
mkdir modis_workspace\Contour\RST_MYD_BT
%-------------------------------------%
mkdir modis_workspace\Contour\EF_MOD_LST
mkdir modis_workspace\Contour\EF_MYD_LST
mkdir modis_workspace\Contour\GA_MOD_LST
mkdir modis_workspace\Contour\GA_MYD_LST
mkdir modis_workspace\Contour\RST_MOD_LST
mkdir modis_workspace\Contour\RST_MYD_LST
%-------------------------------------%
mkdir modis_workspace\Contour\EF_MOD_TPW
mkdir modis_workspace\Contour\EF_MYD_TPW
mkdir modis_workspace\Contour\GA_MOD_TPW
mkdir modis_workspace\Contour\GA_MYD_TPW
mkdir modis_workspace\Contour\RST_MOD_TPW
mkdir modis_workspace\Contour\RST_MYD_TPW
%-------------------------------------%
mkdir modis_workspace\Contour\EF_MOD_AOD
mkdir modis_workspace\Contour\EF_MYD_AOD
mkdir modis_workspace\Contour\GA_MOD_AOD
mkdir modis_workspace\Contour\GA_MYD_AOD
mkdir modis_workspace\Contour\RST_MOD_AOD
mkdir modis_workspace\Contour\RST_MYD_AOD
%-------------------------------------%

rem 创建Temp文件夹..
mkdir modis_workspace\Tmp

echo 初始化数据管理目录结束..