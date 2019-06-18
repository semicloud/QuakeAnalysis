from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt
import numpy as np
import rasterio
from pyproj import Proj

file = r"E:\modis_workspace_test\SampleData\MOD11A1\st_2019110.tif"

# ds = rasterio.open(file)

# Read the file data and metadata
with rasterio.open(r"E:\modis_workspace_test\SampleData\MOD11A1\st_2019110.tif") as src:
    meta = src.meta
    data = src.read(1, masked=True)

print(meta)
# meta['affine'] = [1000.0, 0.0, 12231436.171314925, 0.0, -1000.0, 5559743.714234057]
# Extract shape/extent of the raster in its CRS
map_width = meta['width'] * meta['transform'][0]
map_height = meta['height'] * meta['transform'][0]
xmin = meta['transform'][2]
xmax = xmin + map_width
ymax = meta['transform'][5]
ymin = ymax - map_height
llproj = (xmin, ymin)
urproj = (xmax, ymax)
extent = [xmin, xmax, ymin, ymax]  # [left, right, bottom, top]

# Instantiate projection class and compute longlat coordinates of
# the raster's ll and ur corners
crs = meta['crs']
p = Proj(**crs)
llll = p(*llproj, inverse=True)
urll = p(*urproj, inverse=True)

# Instantiate Basemap class
m = Basemap(llcrnrlon=llll[0], llcrnrlat=llll[1], urcrnrlon=urll[0], urcrnrlat=urll[1],
            projection='cyl',
            resolution='l', lat_0=crs['lat_0'], lon_0=crs['lon_0'])
# There might be other parameters to set depending on your CRS


# draw coastlines.
m.drawcoastlines()
# PLot raster
m.imshow(data, origin='upper', extent=extent)
m.colorbar()
# draw parallels and meridians.
parallels = np.arange(0., 81, 5.)
meridians = np.arange(10., 351., 5.)
# labels = [left,right,top,bottom]
m.drawparallels(parallels, labels=[True, False, False, False])
m.drawmeridians(meridians, labels=[False, False, False, True])
plt.show()
plt.savefig('figure_name.png', dpi=100, transparent=True)
