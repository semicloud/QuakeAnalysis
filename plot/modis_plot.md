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
* 2019-06-05
  * Add plot_mod04_3k.py, plot_mod05_l2.py, the *fundamental* data plotting almost done, next we will deal with the GUI part, after that, we will go on our *abnormal* plotting work.
* 2019-06-10
  * changing `imshow()` to `pcolormesh()`, in order to fit the extent of base map (not the fore map, namely .tif file), see Jiao_Plot.py. Thanks, Dr.Jiao.
  * Add modis_plot.yml to project, a configuration file used plotting
  * Add modis_plot.py, the main script of plotting map
* 2019-06-11
  * complete modis_plot.py and modis_plot.yml, retrieve several arguments from plot script.
  * Using Cython convert py to exe, see Py2exe.bat, before that, one should use pip install Cython first 
  * `pip install Cython`

