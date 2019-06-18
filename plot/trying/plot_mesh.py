import gdal
import osr
from mpl_toolkits.basemap import Basemap, cm
from pyproj import Proj
import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
import rasterio

from matplotlib.colors import BoundaryNorm
from matplotlib.ticker import MaxNLocator


def parse_proj4_str(proj4_str: str):
    prj_info = {'proj': '', 'lat_1': 0.0, 'lat_2': 0.0, 'lat_0': 0.0, 'lon_0': 0.0, 'x_0': 0, 'y_0': 0}
    strs = ''.join(proj4_str.split())
    strs = strs.split('+')
    for ss in strs:
        ss = ss.split('=')
        name = ss[0]
        if name == 'proj':
            prj_info['proj'] = ss[1]
        elif name == 'lat_1':
            prj_info['lat_1'] = float(ss[1])
        elif name == 'lat_2':
            prj_info['lat_2'] = float(ss[1])
        elif name == 'lat_0':
            prj_info['lat_0'] = float(ss[1])
        elif name == 'lon_0':
            prj_info['lon_0'] = float(ss[1])
        elif name == 'x_0':
            prj_info['x_0'] = int(ss[1])
        elif name == 'y_0':
            prj_info['y_0'] = int(ss[1])
    return prj_info


tif_file_path = r'E:\modis_workspace_test\MOD021km\bt_2019110.tif'
# fault_shp_path = r'E:\modis_workspace_test\faults\faults_deng_simple'
country_shp_path = r'E:\modis_workspace_test\faults\ne_50m_admin_0_countries_polyline'

i_band = 1
gdal.UseExceptions()
ds = gdal.Open(tif_file_path)
data = ds.GetRasterBand(i_band).ReadAsArray()
data = np.ma.masked_values(data, 0)

fig = plt.figure()
proj = ds.GetProjection()
print("proj:%s" % proj)
in_spatial_ref = osr.SpatialReference()
in_spatial_ref.ImportFromWkt(proj)
proj4_str = in_spatial_ref.ExportToProj4()
print('export proj4: %s' % proj4_str)
proj_info = parse_proj4_str(proj4_str)
print("parsed proj info: %s" % proj_info)

x0, xinc, _, y0, _, yinc = ds.GetGeoTransform()  # _ means not important variables
print(ds.GetGeoTransform())

nx, ny = (ds.RasterXSize, ds.RasterYSize)
x1 = x0 + xinc * nx
y1 = y0 + yinc * ny
print([x0, x1, y0, y1])

# x0, xinc, _, y1, _, yinc = ds.GetGeoTransform()  # _ means not important variables
# print(ds.GetGeoTransform())
#
# nx, ny = (ds.RasterXSize, ds.RasterYSize)
# x1 = x0 + xinc * nx
# y0 = y1 + yinc * ny
# print([x0, x1, y0, y1])

print("old proj: " + proj_info['proj'])
if proj_info['proj'] == 'eqc':
    proj_info['proj'] = "cyl"

proj_name = proj_info['proj']
lat_1 = proj_info['lat_1']
lat_2 = proj_info['lat_2']
lon_0 = proj_info['lon_0']
lat_0 = proj_info['lat_0']

mymap = Basemap(resolution='i', llcrnrlon=80, llcrnrlat=15, urcrnrlon=140, urcrnrlat=65,
                projection=proj_name, lat_1=lat_1, lat_2=lat_2, lat_0=lat_0, lon_0=lon_0)

delta_x, delta_y = mymap(lon_0, lat_0)
vec_lon = np.linspace(x0 + delta_x, x1 + delta_x, nx)
vec_lat = np.linspace(y0 + delta_y, y1 + delta_y, ny)

from osgeo import ogr
from osgeo import osr
from pyproj import Proj

source = osr.SpatialReference()
source.ImportFromEPSG(32662)  # eqc

target = osr.SpatialReference()
target.ImportFromEPSG(4326)  # cyl

transform = osr.CoordinateTransformation(source, target)

xx, yy = np.meshgrid(vec_lon, vec_lat)

print(np.shape(xx))
print(np.shape(xx)[0])

p_xx = np.zeros(np.shape(xx))
p_yy = np.zeros(np.shape(yy))

# very poor iteration, but effective
for i in np.arange(0, np.shape(xx)[0]):
    for j in np.arange(0, np.shape(xx)[1]):
        lon = xx[i][j]
        lat = yy[i][j]
        p_xx[i][j] = transform.TransformPoint(lon, lat)[0]
        p_yy[i][j] = transform.TransformPoint(lon, lat)[1]

print(p_xx)
print(p_yy)

print(np.shape(xx))
print(np.shape(yy))

cmap = plt.get_cmap('nipy_spectral')
# norm = BoundaryNorm(MaxNLocator(nbins=100).tick_values(data.min(), data.max()), ncolors=cmap.N, clip=True)

cax = mymap.pcolormesh(p_xx, p_yy, data, cmap=cmap, rasterized=True)
cbar = mymap.colorbar(location='right')
cbar.set_label('Brightness Temperature (K?)', fontsize=13)

mymap.drawcoastlines(linewidth=0.5)
mymap.drawcountries(color='0.25')

# draw parallels and meridians.
mymap.drawparallels(np.arange(0, 81, 10), labels=[True, False, False, False])
mymap.drawmeridians(np.arange(10., 351, 10), labels=[False, False, False, True])

plt.savefig('111.png', dpi=600, bbox_inches='tight')
# plt.show()
del mymap

plt.close(fig)
