# MODIS plot script
import argparse
import os, warnings
import yaml
import logging
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cbook
import pyproj
from osgeo import gdal
from osgeo import osr
from typing import Tuple
from mpl_toolkits.basemap import Basemap

# suppress matplotlib warnings
warnings.filterwarnings("ignore", category=matplotlib.cbook.mplDeprecation)

### Used constants ###

NO_DATA = 0.0

### Used constants end ###

logging.basicConfig(level=logging.INFO)

parser = argparse.ArgumentParser(description='MODIS plot script, Version:')
parser.add_argument('-y', '--yml', help='yml配置文件', required=True)

args = parser.parse_args()

### Parse variables ###

plot_variables = yaml.load(open(args.yml, encoding='utf-8'), Loader=yaml.FullLoader)
plot_title = plot_variables['PlotTitle']
plot_title_size = plot_variables['PlotTitleSize']
plot_extent = plot_variables['PlotExtent']
min_lon, max_lon, span_lon, min_lat, max_lat, span_lat = [float(s) for s in plot_extent.split(',')]
logging.debug(u'load min_lon, max_lon, span_lon: %f, %f, %f' % (min_lon, max_lon, span_lon))
logging.debug(u'load min_lat, max_lat, span_lat: %f, %f, %f' % (min_lat, max_lat, span_lat))
color_bar_name = plot_variables['BarName']
color_bar_title = plot_variables['BarTitle']
color_bar_title_size = plot_variables['BarTitleSize']

input_file = plot_variables['InputFile']
output_file = plot_variables['OutputFile']
output_file_dpi = float(plot_variables['OutputDpi'])
output_file_width, output_file_height = [float(s) for s in plot_variables['OutputSize'].split(',')]
logging.debug("output file width, height (inches): %f, %f" % (output_file_width, output_file_height))

has_shp_boundary = 'ShpBoundary' in plot_variables
has_shp_fault = 'ShpFault' in plot_variables
has_shp_city = 'ShpCity' in plot_variables
has_quake_record = 'QuakeRecord' in plot_variables

logging.debug(u'Has boundary shape file? ' + str(has_shp_boundary))
shp_boundary = ""
if has_shp_boundary:
    shp_boundary = plot_variables['ShpBoundary']
    logging.debug("load boundary shape file: " + shp_boundary)

logging.debug(u'Has fault shape file? ' + str(has_shp_boundary))
shp_fault = ""
if has_shp_fault:
    shp_fault = plot_variables['ShpFault']
    logging.debug('load fault shape file:' + shp_fault)

logging.debug(u'Has city shape file? ' + str(has_shp_boundary))
shp_city = ""
if has_shp_city:
    shp_city = plot_variables['ShpCity']
    logging.debug('load city shape file:' + shp_city)

logging.debug('Has quake record? ' + str(has_quake_record))
quake_record = ""
if has_quake_record:
    quake_record = plot_variables['QuakeRecord']
    logging.debug('load quake record: ' + quake_record)


### Parse input end ###

### Read data ###

def read_data(fname: str) -> Tuple[np.array, np.array, np.array]:
    gdal.UseExceptions()
    ds = gdal.Open(fname)
    data = ds.GetRasterBand(1).ReadAsArray()
    # get the real LST
    data = np.ma.masked_values(data, NO_DATA)

    geotrans = ds.GetGeoTransform()
    proj = ds.GetProjection()
    nx = ds.RasterXSize
    ny = ds.RasterYSize

    x0, xinc, _, y0, _, yinc = geotrans
    x1 = x0 + xinc * nx
    y1 = y0 + yinc * ny
    xv = np.linspace(x0, x1, nx)
    yv = np.linspace(y0, y1, ny)
    xv, yv = np.meshgrid(xv, yv)

    # Get the projection info
    in_spatial_ref = osr.SpatialReference()
    in_spatial_ref.ImportFromWkt(proj)
    proj4_str = in_spatial_ref.ExportToProj4()
    origin_proj = pyproj.Proj(proj4_str)
    wgs84 = pyproj.Proj("+init=EPSG:4326")  ## basemap proj
    lons, lats = pyproj.transform(origin_proj, wgs84, xv, yv)
    return data, lons, lats


### Read data end ###


### Plotting ###

if not os.path.exists(input_file):
    print('This file does not exist: %s' % input_file)
    exit(1)
# endif

if os.path.exists(output_file):
    os.remove(output_file)
# endif

fig = plt.figure()
mymap = Basemap(projection='cyl', resolution='h',
                llcrnrlon=min_lon, llcrnrlat=min_lat,
                urcrnrlon=max_lon, urcrnrlat=max_lat)

data, lons, lats = read_data(input_file)
xx, yy = mymap(lons, lats)
mymap.pcolormesh(xx, yy, data, cmap=color_bar_name, rasterized=True)

cbar = mymap.colorbar()
cbar.set_label(color_bar_title, size=color_bar_title_size)

mymap.drawcoastlines(linewidth=0.5)
mymap.drawcountries()

# draw parallels and meridians.
mymap.drawparallels(np.arange(-90, 90, span_lat), labels=[1, 0, 0, 0])
mymap.drawmeridians(np.arange(-180, 180, span_lon), labels=[0, 0, 0, 1])

if has_shp_fault and shp_fault != "":
    mymap.readshapefile(shapefile=shp_fault, name='', color='gray')

if has_shp_boundary and shp_boundary != "":
    mymap.readshapefile(shapefile=shp_boundary, name='')

plt.title(plot_title, size=plot_title_size)

#
# fname = os.path.basename(fname)
# shortname, _ = os.path.splitext(fname)
# shortname += '.jpg'
# fname = os.path.join(out_dir, shortname)
print('Output figure: ', output_file)
plt.savefig(output_file, dpi=output_file_dpi, bbox_inches='tight')

del mymap
plt.close(fig)

### Plotting end ###;：
