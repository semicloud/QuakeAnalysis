import os
import sys
from typing import Tuple

import matplotlib.pyplot as plt
import numpy as np
import osr
from mpl_toolkits.basemap import Basemap
from mpl_toolkits.basemap import pyproj
from osgeo import gdal
from osgeo import osr


class GeoRange:
    def __init__(self,
                 lat_min=0.0, lat_max=0.0,
                 lon_min=0.0, lon_max=0.0,
                 draw_bin=10.0):
        self.lat_min = lat_min
        self.lat_max = lat_max

        if lon_min > 180.0:
            lon_min -= 180.0
        self.lon_min = lon_min

        if lon_max > 180.0:
            lon_max -= 180.0
        self.lon_max = lon_max

        self.draw_bin = draw_bin

        if lat_min >= lat_max:
            print('lat_min is greater than or equal to lat_max')
            print('lat_min: %f <=> lat_min: %f' % (lat_min, lat_max))
            sys.exit(1)

        if lon_min >= lon_max:
            print('lon_min is greater than or equal to lon_max')
            print('lon_min: %f <=> lon_min: %f' % (lon_min, lon_max))
            sys.exit(1)


def read_data(fname: str) -> Tuple[np.array, np.array, np.array]:
    nodata = 0

    gdal.UseExceptions()
    ds = gdal.Open(fname)
    data = ds.GetRasterBand(1).ReadAsArray()
    # get the real LST
    data = np.ma.masked_values(data, nodata)

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
    wgs84 = pyproj.Proj("+init=EPSG:4326")

    lons, lats = pyproj.transform(origin_proj, wgs84, xv, yv)

    return data, lons, lats


def plot_lst_map(fname: str, border: GeoRange, title: str, out_dir: str) -> int:
    if not os.path.exists(fname):
        print('This file does not exist: %s' % fname)
        return 1
    # endif

    if not os.path.exists(out_dir):
        print('The output path does not exist: %s' % out_dir)
        return 1
    # endif

    lat_min = border.lat_min  # in degree
    lat_max = border.lat_max
    lon_min = border.lon_min
    lon_max = border.lon_max

    fig = plt.figure()
    mymap = Basemap(projection='cyl', resolution='i',
                    llcrnrlon=lon_min, llcrnrlat=lat_min,
                    urcrnrlon=lon_max, urcrnrlat=lat_max)

    data, lons, lats = read_data(fname)
    xx, yy = mymap(lons, lats)
    mymap.pcolormesh(xx, yy, data, cmap='nipy_spectral', rasterized=True)

    cbar = mymap.colorbar()
    cbar.set_label('Land surface temperature (K)')

    mymap.drawcoastlines(linewidth=0.5)
    mymap.drawcountries()

    # draw parallels and meridians.
    mymap.drawparallels(np.arange(10, 60, border.draw_bin), labels=[1, 0, 0, 0])
    mymap.drawmeridians(np.arange(70, 150, border.draw_bin), labels=[0, 0, 0, 1])

    plt.title(title)

    #
    fname = os.path.basename(fname)
    shortname, _ = os.path.splitext(fname)
    shortname += '.jpg'
    fname = os.path.join(out_dir, shortname)
    print('Output figure: ', fname)
    plt.savefig(fname, dpi=300, bbox_inches='tight')

    plt.close(fig)

    return 0


def main():
    file = r'E:\modis_workspace_test\MOD05_L2\MOD05_L2_2019110.tif'
    # file = r'E:\MYD11A1_2019_02_8D_N_CN.tif'
    border = GeoRange(lat_min=20, lat_max=50, lon_min=100, lon_max=138, draw_bin=10)
    title = 'MODIS LST'
    outpath = r'E:\CodeWorld\VSProjects\QuakeAnalysis\QuakeAnalysis\plot'
    plot_lst_map(file, border, title, outpath)

    return 0


if __name__ == '__main__':
    main()
