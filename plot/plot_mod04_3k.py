from mpl_toolkits.basemap import Basemap, cm
from pyproj import Proj
import matplotlib.pyplot as plt
import numpy as np
import rasterio

tif_file_path = r'E:\modis_workspace_test\MOD04_3k\mod04_3k_2019110.tif'
# fault_shp_path = r'E:\modis_workspace_test\faults\faults_deng_simple'
country_shp_path = r'E:\modis_workspace_test\faults\ne_50m_admin_0_countries_polyline'

out_fig_path = r"figure_mod04_3k_2019110.png"
out_fig_dpi = 600

figure_title = "MODIS MOD04_3k Aerosol 2019110"
figure_width = 12  # inch
figure_height = 8

cmap_name = "cool"
cbar_title = "Aerosol"

# ==================================================================

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
print(crs)
tif_proj = crs['proj']
if tif_proj == "eqc":
    tif_proj = "cyl"
p = Proj(**crs)
llll = p(*llproj, inverse=True)
urll = p(*urproj, inverse=True)
print(llll)
print(urll)

plt.figure(figsize=(figure_width, figure_height))

# Be careful, the projection of mod05_l2's tif is different from mod11a1 and mod021km
# So the Basemap parameter is different
m = Basemap(llcrnrlon=llll[0], llcrnrlat=llll[1], urcrnrlon=urll[0], urcrnrlat=urll[1],
            projection=tif_proj, resolution='i', lon_0=crs['lon_0'])

m.drawparallels(np.arange(0, 81, 10), labels=[True, False, False, False])
m.drawmeridians(np.arange(10., 351, 10), labels=[False, False, False, True])

# m.drawcoastlines()
# m.drawcountries(linewidth=2)

# LST don't need fault data
# m.readshapefile(fault_shp_path, name='', linewidth=0.5, color='red')
m.readshapefile(country_shp_path, name='', linewidth=2)

m.imshow(data, origin='upper', extent=extent, cmap=cmap_name)
cbar = m.colorbar(location='right')
cbar.set_label(cbar_title, fontsize=13)

#  Add earthquakes LST don't need earthquakes
# df = pd.read_csv('earthquakes.csv')
# min_marker_size = 1.5
# print(df)
# for lat, lon, mag in zip(df['latitude'], df['longitude'], df['mag']):
#     x, y = m(lon, lat)
#     m.plot(lon, lat, plot_utils.get_marker_color(mag), markersize=mag * min_marker_size)
#     plt.text(lon, lat, mag)

plt.title(figure_title, pad='10', fontsize=20)

# plt.show()

plt.savefig(out_fig_path, dpi=out_fig_dpi, bbox_inches='tight')
