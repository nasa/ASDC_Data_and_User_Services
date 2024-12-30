import os
import sys
import numpy as np
import h5py
from urllib.request import urlopen, Request
from pathlib import Path
import argparse
import datetime
import time


'''
Finds all hierarchy paths within the HDF file
'''
def findpaths(name):
  param_paths.append(name)


'''
Reads the HDF5 input file corresponding to the file path
'''
def read_input(input_file):
  try:
    f = h5py.File(input_file, "r" )
  except:
    print("Unable to find or read hdf5 input granule file")
    sys.exit(1)

  f.visit(findpaths)

  return f


def define_grid(ngrid2D):
# function grid_2D maps 2D array var2D from arbitrary geopositions given by arrays lat2D and lon2D 
# onto a regular grid defined by number of bins given in ngrid2D. 
# the function returns the new grid and gridded variable.
# arrays lat2D, lon2D, and var2D must have the same shape,
# otherwise fill value array of the shape of var2D is returned.
# ngrid2D is a two-element array,
# ngrid2D[0] defines number of latitude bins,
# ngrid2D[1] defines number of longitude bins.
# fv is the fill value of the input array var2D   

  nlat = ngrid2D[0]
  nlon = ngrid2D[1]
  dlat = 180./nlat  
  dlon = 360./nlon  
  lat1D = np.empty(nlat) # array of the centers of the latitude bins
  lon1D = np.empty(nlon) # array of the centers of the longitude bins
  for i in range(nlat):
    lat1D[i] = -90. + dlat*(i + 0.5) # the range of latitude is [-90., +90.]
  for i in range(nlon):
    lon1D[i] = -180. + dlon*(i + 0.5) # the range of latitude is [-180., +180.]

  return lat1D, lon1D


def grid_2D(lat2D, lon2D, var2D, fv, lat1D, lon1D):
# function grid_2D maps 2D array var2D from arbitrary geopositions given by arrays lat2D and lon2D 
# onto a regular grid defined by two 1D arrays  lat1D, lon1D. 
# the function returns the new grid and gridded variable.
# arrays lat2D, lon2D, and var2D must have the same shape,
# otherwise fill value array of the shape of var2D is returned.
# ngrid2D is a two-element array,
# ngrid2D[0] defines number of latitude bins,
# ngrid2D[1] defines number of longitude bins.
# fv is the fill value of the input array var2D   

  nlat = len(lat1D)
  nlon = len(lon1D)
  dlat = 180./nlat  
  dlon = 360./nlon  

  var2Dgrid = np.empty((nlat, nlon)) # this is the output array
  var2Dgrid[:, :] = 0.
  cnt2Dgrid = np.empty((nlat, nlon), dtype=np.int16) # this is the array of counts
  cnt2Dgrid[:, :] = 0
# check whether arrays lat2D, lon2D, and var2D have the same shape
  var2Dshape = var2D.shape  
  if lat2D.shape != var2Dshape or lon2D.shape != var2Dshape:
    var2Dgrid[:, :] = fv
    return lat1D, lon1D, var2Dgrid  

  for i in range(var2Dshape[0]):
    for j in range(var2Dshape[1]):
# apparently, fill value for ozone is -999.0, while fill value for both lat and lon is inf
      if(var2D[i, j] < 0.):
        continue
      if(lat2D[i, j] < -90. or lat2D[i, j] >  90.):
        continue
      if(lon2D[i, j] <-180. or lon2D[i, j] > 180.):
        continue

      m = int(np.floor((lat2D[i, j] + 90.)/dlat)) # latitude bin        
      if m not in range(nlat):
        print(i, j, lat2D[i, j], dlat)
        sys.exit(1)         
      n = int(np.floor((lon2D[i, j] + 180.)/dlon)) # longitude bin
      if n not in range(nlon):
        print(i, j, lon2D[i, j], dlon)
        sys.exit(1)         

      var2Dgrid[m, n] = var2Dgrid[m, n] + var2D[i, j] 
      cnt2Dgrid[m, n] = cnt2Dgrid[m, n] + 1

  var2Dgrid[cnt2Dgrid==0] = fv
  var2Dgrid[cnt2Dgrid!=0] = var2Dgrid[cnt2Dgrid!=0]/cnt2Dgrid[cnt2Dgrid!=0]

  return var2Dgrid


def read_epic_l2_to3(fname):
# function read_epic_l2_to3 reads epic_l2_to3 product file given by its fname
# and returns arrays of wavelengths, 2D latitudes and longitudes, 2D TO3 and its fill value, scalar.   
#  param_paths = [] # this is going to be a list of items (groups  and datasets in the file
  try:
    f = h5py.File(fname, "r" )
  except:
    print("Unable to find or read hdf5 input granule file: ", fname)
    wrongf = open('bad_files_list', 'a')
    wrongf.write(fname+'\n')
    wrongf.close()
    lat2D = np.array([[0., 0.], [0., 0.]], dtype=np.float32)
    lon2D = np.array([[0., 0.], [0., 0.]], dtype=np.float32)
    o3_2D = 0.
    fv = 0.
    return lat2D, lon2D, aod3D, fv

  f.visit(findpaths)

  for name in param_paths:
    item = f[name]
# check whether an item is a dataset  
    if isinstance(item, h5py.Dataset):
      if 'Latitude' in name:
        lat2D = f[name][:]
        
      if 'Longitude' in name:
        lon2D = f[name][:]

      if 'Ozone' in name:
        o3_2D = f[name][:]
        fv = item.fillvalue

  f.close()

  return lat2D, lon2D, o3_2D, fv



# the main code begins here.
# this version takes the name of the list of EPIC_L2_TO3_03 files stored at /ASDC_archive.
# the list must contain full path names of the file.
# POIs are read from a dedicated file, 'POI_list'.
# there may be any number of POIs given by their lat and lon pairs. 
# POI's lat should be in the range -90. to +90. deg
# and POI's lon should be in the range -180. to +180. deg. 
narg = len(sys.argv)

try:
  fpoi = open('POI_list', 'r')
  pois = fpoi.readlines()
  fpoi.close()
  npositions = len(pois)
  positions = np.empty([npositions, 2])
  for i in range(npositions):
    pois[i].split()[-2]
    positions[i, 0] = np.float64(pois[i].split()[-2])
    positions[i, 1] = np.float64(pois[i].split()[-1])
except:    
  print('file "POI_list" cannot be opened')
  print('please creat file "POI_list" in the form:')
  print('full name; short name; state/sountry; lat lon: <latitude> <longitude>')
  print('one line per POI with arbitrary number of POIs and start over')
  print('program terminated')
  sys.exit(1)
  
fname = sys.argv[1]
print(fname)
fin = open(fname, 'r')
lines = fin.readlines()
fin.close()

# take yyyymm from lines[0]
pos = lines[0].index('DSCOVR_EPIC_L2_TO3_03') + 22
yyyymm = lines[0][pos : pos + 6]
print(yyyymm)
wrongf = open('bad_files_list', 'a')
wrongf.write('list of bad files '+yyyymm+'\n')
wrongf.close()

# open output file
fout = open('TO3_output_'+yyyymm, 'w')
fout.write('  timestamp   ')
for i in range(npositions):
  fout.write('   POI%1i  ' %(i+1))

fout.write('\n')

# this definition of ngrid2D array need to be made more flexible to accommodate user input
ngrid2D = np.array([360, 720], dtype = np.int32)
center = [int(ngrid2D[0]/2), int(ngrid2D[1]/2)]
ngrid3D = np.append(ngrid2D, 3)

lat1D, lon1D = define_grid(ngrid2D)
nlat = len(lat1D)
nlon = len(lon1D)

dlat = 180./nlat  
dlon = 360./nlon  

for line in lines:
  epic_name = line[:-1]
  print(epic_name)  
  
  pos = epic_name.index('DSCOVR_EPIC_L2_TO3_03') + 22
  epic_timestamp = epic_name[pos : pos + 14]

  param_paths = [] # this is going to be a list of items (groups  and datasets in the file
  lat2D, lon2D, o3_2D, fv = read_epic_l2_to3(epic_name)

  if lat2D[0, 0] == 0.:
    continue

  fout.write(epic_timestamp)

  o3_2Dgrid = np.empty(ngrid2D)

# this is the call of re-gridding routine
  o3_2Dgrid[:, :] = grid_2D(lat2D, lon2D, o3_2D, fv, lat1D, lon1D)

  for position in positions:
    [lat, lon] = position 
# search where position falls in the grid 
    m = int(np.floor((lat + 90.)/dlat)) # latitude bin
    if m not in range(nlat):
      print(lat, dlat, m, nlat)
      sys.exit(1)
         
    n = int(np.floor((lon + 180.)/dlon)) # longitude bin
    if n not in range(nlon):
      print(lon, dlon, n, nlon)
      sys.exit(1)

    if o3_2Dgrid[m, n] != fv:
      o3 = o3_2Dgrid[m, n]
    else:
      o3 = 0.
    fout.write(' %8.3f' %o3)

  fout.write('\n')

fout.close()
sys.exit(0)
