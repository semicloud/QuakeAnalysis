import sys
import os
import h5py
import numpy as np

if __name__ == '__main__':
    hdf_file_path=sys.argv[1]
    dataset=sys.argv[2]
    print('hdf file path: %s' % hdf_file_path)
    print('dataset: %s' % dataset)

    hdf_file_name=os.path.basename(hdf_file_path)
    print("hdf file name: %s" % hdf_file_name)

    hdf_file_name_without_extension=os.path.splitext(hdf_file_name)[0]
    print("hdf file name without extension: %s" % hdf_file_name_without_extension)

    f=h5py.File(hdf_file_path)
    lon=np.array(f["/{}/Geolocation Fields/Longitude".format(dataset)])
    lat=np.array(f["/{}/Geolocation Fields/Latitude".format(dataset)])
    print(lon)
    print(lat)
    min_lon, max_lon = np.min(lon), np.max(lon)
    min_lat, max_lat = np.min(lat), np.max(lat)
    print("min_lon: %.2f, max_lon: %f" % (min_lon, max_lon))
    print("min_lat: %.2f, max_lat: %f" % (min_lat, max_lat))

    out_file_path=os.path.splitext(hdf_file_path)[0] + "_geo.txt"
    if os.path.exists(out_file_path):
        os.remove(out_file_path)
    with open(out_file_path, 'w') as text_file:
        print("%.2f %.2f %.2f %.2f" % (min_lon, max_lon, min_lat, max_lat), file=text_file)
    print("Geolocation saving to %s" % out_file_path)

