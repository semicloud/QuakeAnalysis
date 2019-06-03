from mpl_toolkits.basemap import Basemap, cm
from pyproj import Proj
import matplotlib.pyplot as plt
import numpy as np
import rasterio

# thank https://gis.stackexchange.com/questions/224619/how-to-map-albers-projection-raster-in-basemap-of-python

tif_file_path = r'E:\modis_workspace_test\SampleData\MOD11A1\st_2019110.tif'
fault_shp_path = r'E:\modis_workspace_test\faults\faults_deng_simple'

with rasterio.open(tif_file_path) as ds_raster:
    meta = ds_raster.meta
    data = ds_raster.read(1, masked=True)

map_width = meta['width'] * meta['transform'][0]
map_height = meta['height'] * meta['transform'][0]
xmin = meta['transform'][2]
xmax = xmin + map_width
ymax = meta['transform'][5]
ymin = ymax - map_height
llproj = (xmin, ymin)
urproj = (xmax, ymax)
extent = [xmin, xmax, ymin, ymax]
print(extent)

crs = meta['crs']
tif_proj = crs['proj']
if tif_proj == "eqc":
    tif_proj = "cyl"
p = Proj(**crs)
llll = p(*llproj, inverse=True)
urll = p(*urproj, inverse=True)
print(llll)
print(urll)

plt.figure(figsize=(12, 8))

m = Basemap(llcrnrlon=llll[0], llcrnrlat=llll[1], urcrnrlon=urll[0], urcrnrlat=urll[1],
            projection=tif_proj, resolution='i', lat_0=crs['lat_0'], lon_0=crs['lon_0'])

m.drawparallels(np.arange(0, 81, 10), labels=[True, False, False, False])
m.drawmeridians(np.arange(10., 351, 10), labels=[False, False, False, True])

m.drawcoastlines()
m.drawcountries(linewidth=2)

# LST don't need fault data
# m.readshapefile(fault_shp_path, name='', linewidth=0.5, color='red')

m.imshow(data, origin='upper', extent=extent, cmap='nipy_spectral')
cbar = m.colorbar(location='right')
cbar.set_label('Land surface temperature (K)', fontsize=13)

#  Add earthquakes LST don't need earthquakes
# df = pd.read_csv('earthquakes.csv')
# min_marker_size = 1.5
# print(df)
# for lat, lon, mag in zip(df['latitude'], df['longitude'], df['mag']):
#     x, y = m(lon, lat)
#     m.plot(lon, lat, plot_utils.get_marker_color(mag), markersize=mag * min_marker_size)
#     plt.text(lon, lat, mag)

plt.title('MODIS Land Surface Temperature 20190420', pad='10', fontsize=20)

# plt.show()

plt.savefig('figure.png', dpi=600, bbox_inches='tight')
