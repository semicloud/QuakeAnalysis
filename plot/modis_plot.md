# MODIS plot tool specification

## Operating System

windows 10, 64 bit
## Tool
PyCharm

## Dependency

* python 3.6.8 64 bit
* [gdal-2.4.1-cp36-cp36m-win_amd64](https://download.lfd.uci.edu/pythonlibs/t4jqbe6o/GDAL-2.4.1-cp36-cp36m-win_amd64.whl) 
* numpy 1.16.4
* matplotlib 3.1.0
* [basemap 1.2.0](https://download.lfd.uci.edu/pythonlibs/t4jqbe6o/basemap-1.2.0-cp36-cp36m-win_amd64.whl), need matplotlib firstly
* pandas 0.24.2
* [rasterio 1.0.23](https://download.lfd.uci.edu/pythonlibs/t4jqbe6o/rasterio-1.0.23-cp36-cp36m-win_amd64.whl)

## Develop log

* 2019-06-03
  * Nearly complete plot_mod11a1.py, plot_mod021km.py
  * Add correct projection to basemap, see https://gis.stackexchange.com/questions/224619/how-to-map-albers-projection-raster-in-basemap-of-python 