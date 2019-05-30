from mpl_toolkits.basemap import Basemap, cm
import numpy as np
import matplotlib.pyplot as plt
import gdal
import osr

min_lon = 100
max_lon = 140
min_lat = 20
max_lat = 50

fault_shp = r"E:\modis_workspace_test\faults\faults_deng_simple"
block_shp = r"E:\modis_workspace_test\faults\China_block3"
city_shp = r"E:\modis_workspace_test\faults\cities"
china_shp = r"E:\modis_workspace_test\faults\ne_50m_admin_0_countries_polyline"
mod11a1_tif = r"E:\modis_workspace_test\SampleData\MOD11A1\st_2019110.tif"

lon_0 = (min_lon + max_lon) / 2
lat_0 = (min_lat + max_lat) / 2

plt.figure(figsize=(12, 8))  # by inches, 1 inch means 80 pixels

# The default value is cyl, or Cylindrical Equidistant projection
# also known as Equirectangular projection or Plate Carrée
m = Basemap(projection='cyl', lon_0=lon_0, lat_0=lat_0, resolution='i',
            llcrnrlon=min_lon, urcrnrlon=max_lon,
            llcrnrlat=min_lat, urcrnrlat=max_lat, lat_ts=lat_0)
m.drawcoastlines()
m.drawcountries(linewidth=2)
m.drawparallels(np.arange(0, 81, 10), labels=[True, False, False, False])
m.drawmeridians(np.arange(10., 351, 10), labels=[False, False, False, True])

# m.drawmapboundary(fill_color='aqua')
# m.fillcontinents(color='coral', lake_color='aqua')


# Add shapefile
m.readshapefile(fault_shp, name='', linewidth=0.5, color='red')  # name can be empty for line shapefile
# m.readshapefile(block_shp, name='', linewidth=1, color='blue')
m.readshapefile(city_shp, name='city', linewidth=5, color='green')


print('before filter')
print(len(m.city))
print(m.city)

# filter shape file, otherwise plt.text will plot all text in the file!
# in projection cyl, city_pos is lonlat, other projection does not work
# may be need transform before filter
city_info, city_pos = zip(*[(info, pos) for info, pos in zip(m.city_info, m.city) if
                            min_lon < pos[0] < max_lon and min_lat < pos[1] < max_lat])

print('after filter')
print(len(city_info))

for info, pos in zip(city_info, city_pos):
    m.plot(pos[0], pos[1], color='green', marker='o', markersize=4)
    # xpt, ypt = m(pos[0], pos[1])
    # plt.text(xpt + 0.2, ypt + 0.2, info["NAME"])

#  Now lets add some tif
gdal.UseExceptions()
ds = gdal.Open(mod11a1_tif)
data = ds.GetRasterBand(1).ReadAsArray()
data = np.ma.masked_values(data, 0.0)  # hide no data value
ny = data.shape[0]
nx = data.shape[1]
print(ny)
print(nx)
lons, lats = m.makegrid(nx, ny)
x, y = m(lons, lats)
print("min=%d, max=%d" % (np.min(data), np.max(data)))
clevls = np.arange(np.min(data), np.max(data), 1)  # how to decide color levels?
print(clevls)
# Color map, see here: https://matplotlib.org/gallery/color/colormap_reference.html
cs = m.contourf(x, y, data, clevls, cmap=plt.cm.get_cmap('plasma'))
cbar = m.colorbar(cs, location='right', pad='5%')
cbar.set_label('LST')

plt.title('MODIS Land Surface Temperature 20190420', pad='20')
plt.show()
