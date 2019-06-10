from osgeo import ogr
from osgeo import osr

source = osr.SpatialReference()
source.ImportFromEPSG(32662)  # eqc

target = osr.SpatialReference()
target.ImportFromEPSG(4087)  # cyl

transform = osr.CoordinateTransformation(source, target)

point = ogr.CreateGeometryFromWkt("POINT (11119487.4284681174904108 2223743.7142340587452054)")
point.Transform(transform)

print(point.ExportToWkt())
