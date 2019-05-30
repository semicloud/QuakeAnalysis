from mpl_toolkits.basemap import Basemap, cm
import numpy as np
import matplotlib.pyplot as plt
import gdal
import osr

min_lon = 100
max_lon = 140
min_lat = 20
max_lat = 50

lon_0 = (min_lon + max_lon) / 2
lat_0 = (min_lat + max_lat) / 2

# The default value is cyl, or Cylindrical Equidistant projection
# also known as Equirectangular projection or Plate Carrée
m = Basemap(projection='cyl', lon_0=lon_0, lat_0=lat_0, resolution='i',
            llcrnrlon=min_lon, urcrnrlon=max_lon,
            llcrnrlat=min_lat, urcrnrlat=max_lat, lat_ts=lat_0)
m.drawcoastlines()
m.drawcountries()
m.drawparallels(np.arange(0, 81, 10), labels=[True, False, False, False])
m.drawmeridians(np.arange(10., 351, 10), labels=[False, False, False, True])
# m.drawmapboundary(fill_color='aqua')
# m.fillcontinents(color='coral', lake_color='aqua')
plt.title('Title title title title')
plt.show()
