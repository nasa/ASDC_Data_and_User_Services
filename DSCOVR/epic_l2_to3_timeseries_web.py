import os
import sys
import requests
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
      '''
      if(var2D[i, j] == fv):
        continue
      if(lat2D[i, j] == fv):
        continue
      if(lon2D[i, j] == fv):
        continue
      '''
# this part is modified from above since there is suspicion that fill value is set incorrectly in the product.
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
# function read_epic_l2_aer reads epic_l2_aer product file given by its fname
# and returns arrays of wavelengths, 2D latitudes and longitudes, 3D AOD and its fill value, scalar.   
#  param_paths = [] # this is going to be a list of items (groups  and datasets in the file
  f = read_input(fname) # this function opens file for reading, returns file object and populated the list of items

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


def list_epic_L2_TO3(yyyymm):
# function list_epic_L2_AER takes month of interest in the form of an 6-character string yyyymm.
# the function returns a list of all DSCOVR_EPIC_L2_TO3 files publicly availabe at the ASDC, see variable url below.
# user must have an account at https://urs.earthdata.nasa.gov/
# and use that account to create and update variable token.
# if token is outdated, the list will not be created.   
  yyyy = yyyymm[0 : 4]
  mm   = yyyymm[4 : 6]
  url = 'https://asdc.larc.nasa.gov/data/DSCOVR/EPIC/L2_TO3_03/'
  token = 'eyJ0eXAiOiJKV1QiLCJvcmlnaW4iOiJFYXJ0aGRhdGEgTG9naW4iLCJzaWciOiJlZGxqd3RwdWJrZXlfb3BzIiwiYWxnIjoiUlMyNTYifQ.eyJ0eXBlIjoiVXNlciIsInVpZCI6ImFsZXhyYWQ3MSIsImV4cCI6MTY5NTQzMjQ1OCwiaWF0IjoxNjkwMjQ4NDU4LCJpc3MiOiJFYXJ0aGRhdGEgTG9naW4ifQ.A5ZCKuKDfNRt-P3ftLKx7yp_HV5NiU3H8M1E-pEwriBvVLj-VdQaTQksJVD6DV3y78cYY5ar-xTjG7pwLE4qg-8hnyDn5B8nWl7l_C465do7Jb5cXKw9Mdf3vzp-jvCIw-KGLgET8jG_66gtFrRWSJmZydPgpWjVlGwBNaj1awkpBEKaoJP_DlQ4DbIBDCVCuHTngEqzvVL1NbeaNS8YfDqcmp3zw_nIga9bCPblJs4_LAvWSkdcEbfxy2PkGIZSKjvr4_U1vOSqXr-WAoilLPrYmU5nf6Af3Ec0j6Vwrqit5m8QooEcuR6BQR-Jm8FUy-6rtCvI86I8JdahwLmiMw'
  
  header={"Authorization": f"Bearer {token}"}
  print(url+yyyy+'/'+mm+'/')
#  sys.exit()
  req = Request(url+yyyy+'/'+mm+'/', headers=header)
  with urlopen(req) as response:
    html_bytes = response.read()
    html = html_bytes.decode("utf-8")
    html_len = len(html)
    big_line = str(html)
    lines = big_line.split('\n')
    nlines = len(lines)

  ref_lines = [i for i in lines if ('href' in i) and ('DSCOVR_EPIC_L2_TO3_03' in i)]
  print(len(ref_lines),' files found for ', yyyymm)
  refs = [None]*len(ref_lines)
  cnt = -1
  for line in ref_lines:
    pos1 = line.index('DSCOVR')
    pos2 = line.index('"', pos1+1)
    if pos1 > 0 and pos2 > pos1 and line[pos2-2 : pos2] =='h5':
      cnt = cnt + 1
      refs[cnt] = url+yyyy+'/'+mm+'/'+line[pos1 : pos2]
    
  return refs[0:cnt+1]


def download_epic(url):
# function download_epic takes data file of interest given by its URL
# from the list created by function list_epic_L4_tro3.
# the function returns filename and request response_code for control purposes.
# download is successful if response_code == 200.
# user must have an account at https://urs.earthdata.nasa.gov/
# and use that account to create and update variable token.
# if token is outdated, the list will not be created.   
  token='eyJ0eXAiOiJKV1QiLCJvcmlnaW4iOiJFYXJ0aGRhdGEgTG9naW4iLCJzaWciOiJlZGxqd3RwdWJrZXlfb3BzIiwiYWxnIjoiUlMyNTYifQ.eyJ0eXBlIjoiVXNlciIsInVpZCI6ImFsZXhyYWQ3MSIsImV4cCI6MTY5NTQzMjQ1OCwiaWF0IjoxNjkwMjQ4NDU4LCJpc3MiOiJFYXJ0aGRhdGEgTG9naW4ifQ.A5ZCKuKDfNRt-P3ftLKx7yp_HV5NiU3H8M1E-pEwriBvVLj-VdQaTQksJVD6DV3y78cYY5ar-xTjG7pwLE4qg-8hnyDn5B8nWl7l_C465do7Jb5cXKw9Mdf3vzp-jvCIw-KGLgET8jG_66gtFrRWSJmZydPgpWjVlGwBNaj1awkpBEKaoJP_DlQ4DbIBDCVCuHTngEqzvVL1NbeaNS8YfDqcmp3zw_nIga9bCPblJs4_LAvWSkdcEbfxy2PkGIZSKjvr4_U1vOSqXr-WAoilLPrYmU5nf6Af3Ec0j6Vwrqit5m8QooEcuR6BQR-Jm8FUy-6rtCvI86I8JdahwLmiMw'
  header={"Authorization": f"Bearer {token}"}
  response = requests.get(url, headers=header)
  response_code = response.status_code
  file_name = url.split('/')[-1]

  if response_code == 200:
    content = response.content
    data_path = Path(file_name)
    data_path.write_bytes(content)

  return file_name, response_code


# the main code begins here.
# this version takes a month of interest as its 1st argument.
# the form of the 1st argument is a string 'yyyymm'.
# a list of EPIC_L2_TO3_03 files available for that month at DDD,
# https://asdc.larc.nasa.gov/data/DSCOVR/EPIC/L2_TO3_03/yyyy/mm/, is created on the fly.
# The files from the created list are downloaded and processed one after another.
# They are deleted after processing to save disk space.
# If it is desirable to preserve the files for the future use comment out line 'os.remove(epic_name)' from the code.
# the list must contain full path names of the file.
# points of interest (POIs) are read from a dedicated file, 'POI_list',
# that lists POIs line by line. Every line must contain:
# <full name>; s<hort name>; <state/sountry>; <lat lon: > <latitude> <longitude>
# optionally, there may be any number of POIs given by their lat and lon pairs. 
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
# two last values in poi line are latitude and longitude:
    positions[i, 0] = np.float64(pois[i].split()[-2])
    positions[i, 1] = np.float64(pois[i].split()[-1])
except:    
  print('file "POI_list" cannot be opened')
  print('please creat file "POI_list" in the form:')
  print('full name; short name; state/sountry; lat lon: <latitude> <longitude>')
  print('one line per POI with arbitrary number of POIs and start over')
  print('program terminated')
  sys.exit(1)
  
yyyymm = sys.argv[1]
print(yyyymm)
# open output file
fout = open('TO3_output_'+yyyymm, 'w')
fout.write('  timestamp   ')
for i in range(npositions):
  fout.write('   POI%1i  ' %(i+1))

fout.write('\n')

refs = list_epic_L2_TO3(yyyymm)
print(len(refs))

# open output file
fout = open('output'+yyyymm, 'w')

# this definition of ngrid2D array need to be made more flexible to accommodate user input
ngrid2D = np.array([360, 720], dtype = np.int32)

lat1D, lon1D = define_grid(ngrid2D)
nlat = len(lat1D)
nlon = len(lon1D)

dlat = 180./nlat  
dlon = 360./nlon  

for ref in refs:
  epic_name, response_code = download_epic(ref)
  print(epic_name)  

  pos = epic_name.index('DSCOVR_EPIC_L2_TO3_03') + 22
  epic_timestamp = epic_name[pos : pos + 14]
  fout.write(epic_timestamp)

  param_paths = [] # this is going to be a list of items (groups  and datasets in the file
  lat2D, lon2D, o3_2D, fv = read_epic_l2_to3(epic_name)

  os.remove(epic_name) # comment out this line if it is desirable to preserve EPIC data files in the local directory

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
