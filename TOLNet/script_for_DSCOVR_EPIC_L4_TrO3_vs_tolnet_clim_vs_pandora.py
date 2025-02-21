import os
import sys
import numpy as np
import math
import codecs
import requests
import h5py
from pyhdf.SD import SD, SDC
from jdcal import gcal2jd, jd2gcal
import matplotlib.pyplot as plt
from urllib.request import urlopen, Request
from pathlib import Path


def read_tolnet_clim(fname):
# function read_tolnet_clim takes a name of TOLNET clim file, fname, and returns the following values 
# lat_int, lon_int, start_time[0], stop_time[0], o3_column, tro3_column
# where
# lat_int, lon_int - position of the instrument
# start_time, stop_time time of the beginning and conclusion of the measurement, MJD200
# o3_column, tro3_column - total and tropospheric ozone columns, DU 

  hdf = SD(fname, SDC.READ)

  dsets = hdf.datasets()

  for dset in dsets:
    dset_data = hdf.select(dset)
    data = dset_data[:]

    if dset == 'ALTITUDE.INSTRUMENT':
      print('TOLNET '+dset, data[0])

    if dset == 'LATITUDE.INSTRUMENT':
      print('TOLNET '+dset, data[0])
      lat_int =  data[0] # latitude of TOLNET instrument, this is used to interpolate EPIC L4 ozone data
    if dset == 'LONGITUDE.INSTRUMENT':
      print('TOLNET '+dset, data[0])
      lon_int = data[0] # longitude of TOLNET instrument, this is used to interpolate EPIC L4 ozone data

    if dset == 'ALTITUDE':
      altitudes = data

    if dset == 'DATETIME.START':
      start_time = data
  
    if dset == 'DATETIME.STOP':
      stop_time = data

    if dset == 'INTEGRATION.TIME':
      integration_time = data
  
    if dset == 'O3.NUMBER.DENSITY_ABSORPTION.DIFFERENTIAL':
      o3_number_density = data
    
  hdf.end()

  ntimes = len(start_time)
  if ntimes != 1:
    print('too many time occasions, ', ntimes)
    print('return zeroes')
    return 0., 0., 0., 0., 0., 0.

  nalt = len(altitudes)
  dz = altitudes[1 : nalt] - altitudes[0 : nalt-1]
  max_dz = max(dz)
  min_dz = min(dz)
  if max_dz != min_dz:
    print('uneven vertical integration steps ', max_dz, min_dz, 'return zeroes')
    return 0., 0., 0., 0., 0., 0.

# now integrate TOLNET profile to get total and tropospheric column. Please note that profile does not cover altitudes above 25km, so total column is less than it should be
  tro3_number_density = o3_number_density[altitudes<10000.]
  if max_dz == min_dz:
    profile = o3_number_density
    profile[profile < 0] = 0.
    tr_profile = tro3_number_density
    tr_profile[tr_profile < 0] = 0.
# integrated ozone profile has units molecules per m^2; division by 2.6867E+20 converts result into DU, see https://www.temis.nl/general/dobsonunit.php
    o3_column = sum(profile[1:-1], (profile[0] + profile[-1])*0.5) * min_dz/2.6867E+20
    tro3_column = sum(tr_profile[1:-1], (tr_profile[0] + tr_profile[-1])*0.5) * min_dz/2.6867E+20

  return lat_int, lon_int, start_time[0], stop_time[0], o3_column, tro3_column


def read_pandora_web():
  url = 'https://data.pandonia-global-network.org/'
  page = urlopen(url)
  html_bytes = page.read()
  html = html_bytes.decode("utf-8")
  html_len = len(html)

  pos1 = 0

  big_line = str(html)
  lines = big_line.split('\n')

  ref_lines = [i for i in lines if 'href' in i]
  refs = []
  for line in ref_lines:
    pos1 = line.find('"')
    pos2 = line.rfind('"')
    if pos1 > 0 and pos2 > pos1 and line[pos2-1] =='/' and line[pos1+1] == '.':
      refs.append(line[pos1+3 : pos2-1])

  return refs
  

# Pandora site may have several instruments. In this case each instrument has its own directory.
# However, the most recent version of the O3 data, rout2p1_8, is available only in one of these directories.
# The function creates all possible links, but some of them may be non-existing. This is checked and cleared later.
def instrument_path(site):
  url = 'https://data.pandonia-global-network.org/' + site + '/'
  page = urlopen(url)
  html_bytes = page.read()
  html = html_bytes.decode("utf-8")
  html_len = len(html)

  pos1 = 0
  big_line = str(html)
  lines = big_line.split('\n')

  ref_lines = [i for i in lines if 'href' in i]
  links = []
  for line in ref_lines:

    pos1 = line.find('"')
    pos2 = line.rfind('"')
    if pos1 > 0 and pos2 > pos1 and line[pos2-1] =='/' and\
       line[pos1+3 : pos1 + 10] == 'Pandora':
      link = url + line[pos1+3 : pos2] + 'L2/' + line[pos1+3 : pos2-1] + '_' + site + '_L2_rout2p1-8.txt'
      print(link)
      links.append(link)

  return links    


def check_site(site_name, refs):
# function check_site checks whether user-entered site_name is in any name
# in the list Pandora ozone data files and return the list of all occurences. 
  site_list = []
  for line in refs:
    if site_name in line:
      site_list.append(line) 
      
  return site_list
  

def take_pandora_sites(refs):
# function take_pandora_sites takes user input on Pandora site needed to be read.
# since there is only one place where Pandora and TOLNET instrument are co-located,
# JPL Table Mountain facility, this function was modified from its original version
# so, that this specific site (WrightwoodCA) is hardcoded.
# if other sites need to be considered,
# see comments in the function body to go back to its original version. 
  print('please select a Pandora site name from the list')
  for ref in refs:
    print(ref)
  
  answer = 'y'
  while answer == 'y':
    site_name = input('Enter a name of a Pandora site: ')
# this is to speed up processing of the single site of interest. remove this part and un-comment the line above 
#    site_name = 'WrightwoodCA'
# remove the line above to get back to general case    
    print(site_name)
    site_list = check_site(site_name, refs)
    site_num = len(site_list)
    if site_num == 0:
      print('site ', site_name, 'was not found')

    if site_num > 1:
      print('there are ', site_num, ' site names, select one from')
      for site in site_list:
        print(site)

      site_name = input('Enter a name of a Pandora site: ')
# this is to speed up processing of the single site of interest. remove this part and un-comment the line above
#      site_name = 'WrightwoodCA'
# remove the line above to get back to general case    
      for site in site_list:
        if site == site_name:
          pandora_site = site_name
          print('site ', site_name, 'was found')
          break
        
    if site_num == 1:
      pandora_site = site_list[0]
      print('site ', site_list[0], 'was found')

    answer = 'n'
    
  return pandora_site


def download(url):
# function download takes a URL of file in the web and downloads it.
# the function is intended to work with sources that DO NOT require any authorization.
# the function returns the name of the file and return request response code for control purposes
# download is succesful if response_code == 200.
  response = requests.get(url)
  response_code = response.status_code

  file_name = url.split('/')[-1]

  if response_code == 200:
    content = response.content
    data_path = Path(file_name)
    data_path.write_bytes(content)

  return file_name, response_code
  
  
def timestamp2day_pandora(timestamp):
# function timestamp2day converts Pandora timestamp of the format 'yyyymmddThhmmssZ'
# into a set of 3 numbers: integer year, month, and real day of the month.
# fractional part of the day is part of the day elapsed from the beginning of the day.

  yyyy = int(timestamp[0:4])
  mm = int(timestamp[4:6])
  dd = int(timestamp[6:8])
  hh = int(timestamp[9:11])
  mn = int(timestamp[11:13])
  ss = float(timestamp[13:17])

  day = dd - 1 + (hh + mn/60. + ss/3600.)/24.

  return yyyy, mm, day


def read_pandora_file(fname, yyyymmdd):
# function read_pandora_file takes Pandora data file name assuming it has been downloaded
# and date stamp of interest in the form yyyymmdd.
# the function returns latitude and longitude of the Pandora site
# and an array of 4 columns: year, month, fractional day, and ozone column in Dobson units (DU).
# fractional day is time in day since beginning of the month,
# e.g. May 31, 6:00 pm would have fractional day of 30.75.
  DU_conversion = 1./4.4615E-04
  current_year = int(yyyymmdd[0:4])
  current_month = int(yyyymmdd[4:6])
  current_day = int(yyyymmdd[6:8])
  print(current_year, current_month, current_day)

  with codecs.open(fname, 'r', encoding='utf-8', errors='ignore') as f:

    while True:
# Get next line from file
      line = f.readline()

      if line.find('Short location name:') >= 0:
        loc_name = line.split()[-1] # location name, to be used in the output file name

      if line.find('Location latitude [deg]:') >= 0:
        lat = float(line.split()[-1]) # location latitude

      if line.find('Location longitude [deg]:') >= 0:
        lon = float(line.split()[-1]) # location longitude

      if line.find('--------') >= 0:
        break

    while True:
# Get next line from file
      line = f.readline()

      if line.find('--------') >= 0:
        break

# now reading line with data
    daily_data = []
    while True:
      line = f.readline()

      if not line:
        break

      line_split = line.split()
      yyyy, mm, day = timestamp2day_pandora(line_split[0])
      o3_col = float(line_split[38])*DU_conversion
        
      if(yyyy == current_year):
        if(mm == current_month):
          if(int(day)+1 == current_day):
            daily_data.append([yyyy, mm, day, o3_col])

  return lat, lon, np.array(daily_data)
      
      
def list_epic_L4_tro3(yyyymmdd):
# function list_epic_L4_tro3 takes date of interest in the form of an 8-character string yyyymmdd.
# the function returns a list of all DSCOVR_EPIC_L4_TRO3 files publicly availabe at the ASDC, see variable url below.
# user must have an account at https://urs.earthdata.nasa.gov/
# and use that account to create and update variable token.
# if token is outdated, the list will not be created.   
  yyyy = yyyymmdd[0 : 4]
  mm   = yyyymmdd[4 : 6]
  dd   = yyyymmdd[6 : 8]
  url = 'https://asdc.larc.nasa.gov/data/DSCOVR/EPIC/L4_TrO3_01/'
  token = 'eyJ0eXAiOiJKV1QiLCJvcmlnaW4iOiJFYXJ0aGRhdGEgTG9naW4iLCJzaWciOiJlZGxqd3RwdWJrZXlfb3BzIiwiYWxnIjoiUlMyNTYifQ.eyJ0eXBlIjoiVXNlciIsInVpZCI6ImFsZXhyYWQ3MSIsImV4cCI6MTcyNzYzNTE3MSwiaWF0IjoxNzIyNDUxMTcxLCJpc3MiOiJFYXJ0aGRhdGEgTG9naW4ifQ.sWRMKbhZolbqcyWTN8sHwGjlI3hN8wBH_SFGNbhak0HSsy84-PxlpyYglKQkscoo3G7uDEzlgVTT87ZyPNrWZZKLQvQusk1P3DNG1FwfnOUhCOfzb0lPiFQmrpT8besHcVvvwJHW3fOyvMqiE6QnCPOWEyw3Nzgg73MWzU4X7ocOQaJJOTh5bTpXjGT1Bqufv2v-IcmA9RNyarkC76BQy7QoFiCGTaWse6V_OCHSCr5_pjHM4hVp4kdxDo8peI4L3-1wQ442HoqebTyNN9cfWFSKaosrfDd8bI93vAyjW7NmUTEedpPQ6nxxkAtHuSP4KghPWxGXmZ5pekuoSPJIEQ'
  
  header={"Authorization": f"Bearer {token}"}
  
  try:
    req = Request(url+yyyy+'/'+mm+'/', headers=header)
  except:
    return []

  with urlopen(req) as response:
    html_bytes = response.read()
    html = html_bytes.decode("utf-8")
    html_len = len(html)
    big_line = str(html)
    lines = big_line.split('\n')
    nlines = len(lines)

  ref_lines = [i for i in lines if ('href' in i) and ('DSCOVR_EPIC_L4_TrO3_01_'+yyyymmdd in i)]
  print(len(ref_lines),' files found for ', yyyymmdd)
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
  token = 'eyJ0eXAiOiJKV1QiLCJvcmlnaW4iOiJFYXJ0aGRhdGEgTG9naW4iLCJzaWciOiJlZGxqd3RwdWJrZXlfb3BzIiwiYWxnIjoiUlMyNTYifQ.eyJ0eXBlIjoiVXNlciIsInVpZCI6ImFsZXhyYWQ3MSIsImV4cCI6MTcyNzYzNTE3MSwiaWF0IjoxNzIyNDUxMTcxLCJpc3MiOiJFYXJ0aGRhdGEgTG9naW4ifQ.sWRMKbhZolbqcyWTN8sHwGjlI3hN8wBH_SFGNbhak0HSsy84-PxlpyYglKQkscoo3G7uDEzlgVTT87ZyPNrWZZKLQvQusk1P3DNG1FwfnOUhCOfzb0lPiFQmrpT8besHcVvvwJHW3fOyvMqiE6QnCPOWEyw3Nzgg73MWzU4X7ocOQaJJOTh5bTpXjGT1Bqufv2v-IcmA9RNyarkC76BQy7QoFiCGTaWse6V_OCHSCr5_pjHM4hVp4kdxDo8peI4L3-1wQ442HoqebTyNN9cfWFSKaosrfDd8bI93vAyjW7NmUTEedpPQ6nxxkAtHuSP4KghPWxGXmZ5pekuoSPJIEQ'
  header={"Authorization": f"Bearer {token}"}
  response = requests.get(url, headers=header)
  response_code = response.status_code
  file_name = url.split('/')[-1]

  if response_code == 200:
    content = response.content
    data_path = Path(file_name)
    data_path.write_bytes(content)

  return file_name, response_code


def timestamp2day_epic(fname):
# function timestamp2day-epic converts EPIC L4 file name into a set of 3 numbers:
#   integer year, month, and real day.
# fractional part of the day is part of the day elapsed from the beginning of the day.
  strpos = fname.rfind('TrO3')
  timestamp = fname[strpos+8:strpos+22]
  int_timestamp = int(timestamp)
  yyyy = int(timestamp[0:4])
  mm = int(timestamp[4:6])
  dd = int(timestamp[6:8])
  hh = int(timestamp[8:10])
  mn = int(timestamp[10:12])
  ss = int(timestamp[12:14])

  day = dd - 1 + (hh + mn/60. + ss/3600.)/24.

  return yyyy, mm, day


def read_DSCOVR_EPIC_L4_O3(fname, lat0, lon0):
# function read_DSCOVR_EPIC_L4_O3 reads EPIC L4 file supplied with fname and returns total and tropospheric O3 columns
# in the point of interest supplied by latitude, lat0, and longitude, lon0.

# Open file.
  f = h5py.File(fname, 'r')

  totO3_col = 0.
  tr_O3_col = 0.
  if not ('Latitude' in f):
    print('no Latitude dataset in ' + fname)
    return totO3_col, tr_O3_col
  if not ('Longitude' in f):
    print('no Longitude dataset in ' + fname)
    return totO3_col, tr_O3_col
  if not ('TotalColumnOzone' in f):
    print('no TotalColumnOzone dataset in ' + fname)
    return totO3_col, tr_O3_col
  if not ('TroposphericColumnOzone' in f):
    print('no TroposphericColumnOzone dataset in ' + fname)
    return totO3_col, tr_O3_col
    
  try:
    lat = np.array(f['Latitude'])
  except:
    return totO3_col, tr_O3_col
  try:
    lon = np.array(f['Longitude'])
  except:
    return totO3_col, tr_O3_col
  try:
    totO3 = np.array(f['TotalColumnOzone'])
  except:
    return totO3_col, tr_O3_col
  try:
    tr_O3 = np.array(f['TroposphericColumnOzone'])
  except:
    return totO3_col, tr_O3_col
  
  f.close()

  i0 = -1
  j0 = -1

  nlat = len(lat)
  nlon = len(lon)

# search for coordinates
  for i in range(nlat-1):
    if lat0 > lat[i] and lat0 < lat[i+1]:
      i0 = i
      break
  
  for j in range(nlon-1):
    if lon0 > lon[j] and lon0 < lon[j+1]:
      j0 = j
      break

  if i0 < 0 or j0 < 0:
    print(lat0, min(lat), max(lat), lon0, min(lon), max(lon), fname)
    sys.exit()

  wtSW = (lat[i0+1] - lat0)*(lon[j0+1] - lon0) # weight for totO3[i0, j0]
  wtNW = (lat0 - lat[i0])*(lon[j0+1] - lon0)   # weight for totO3[i0+1, j0]
  wtSE = (lat[i0+1] - lat0)*(lon0 - lon[j0])   # weight for totO3[i0, j0+1]
  wtNE = (lat0 - lat[i0])*(lon0 - lon[j0])     # weight for totO3[i0+1, j0+1]
  
  wt_sum = 0.
  if totO3[i0, j0] > 0.:
    wt_sum = wt_sum + wtSW
    totO3_col = totO3_col + wtSW*totO3[i0, j0]
  if totO3[i0+1, j0] > 0.:
    wt_sum = wt_sum + wtNW
    totO3_col = totO3_col + wtNW*totO3[i0+1, j0]
  if totO3[i0, j0+1] > 0.:
    wt_sum = wt_sum + wtSE
    totO3_col = totO3_col + wtSE*totO3[i0, j0+1]
  if totO3[i0+1, j0+1] > 0.:
    wt_sum = wt_sum + wtNE
    totO3_col = totO3_col + wtNE*totO3[i0+1, j0+1]
  
  if wt_sum > 0.:
    totO3_col = totO3_col/wt_sum

  wt_sum = 0.
  if tr_O3[i0, j0] > 0.:
    wt_sum = wt_sum + wtSW
    tr_O3_col = tr_O3_col + wtSW*tr_O3[i0, j0]
  if tr_O3[i0+1, j0] > 0.:
    wt_sum = wt_sum + wtNW
    tr_O3_col = tr_O3_col + wtNW*tr_O3[i0+1, j0]
  if tr_O3[i0, j0+1] > 0.:
    wt_sum = wt_sum + wtSE
    tr_O3_col = tr_O3_col + wtSE*tr_O3[i0, j0+1]
  if tr_O3[i0+1, j0+1] > 0.:
    wt_sum = wt_sum + wtNE
    tr_O3_col = tr_O3_col + wtNE*tr_O3[i0+1, j0+1]
  
  if wt_sum > 0.:
    tr_O3_col = tr_O3_col/wt_sum
    
  return totO3_col, tr_O3_col


# The code compares ozone columns from DSCOVR EPIC L4 ozone product,
# Pandora total ozone, and integrated TOLNET clim tropospheric columns.
# the code is driven by the list of TOLNET clim files.
# 
# main code begins here.
#
# first, read TOLNET clim data files provided as a list in the command line argument.
# WARNING: providing non clim data, e.g. hirez, will fail the code.

listname = sys.argv[1] # absolute path/name should be provided here

f = open(listname, 'r')

while True:
# Get next line from file
  line = f.readline()
  if not line:
    break
  fname = line[0 : -1]

  (lat_int, lon_int, start_time, stop_time, o3_column, tro3_column) = read_tolnet_clim(fname) 
  (yi, mi, di, pdi) = jd2gcal(2400000.5, 51544.+start_time)
  (yf, mf, df, pdf) = jd2gcal(2400000.5, 51544.+stop_time)
  hhi = int(pdi*24)
  mmi = int((pdi*24 - hhi)*60.)
  ssi = ((pdi*24 - hhi)*60. - mmi)*60.
  hhf = int(pdf*24)
  mmf = int((pdf*24 - hhf)*60.)
  ssf = ((pdf*24 - hhf)*60. - mmf)*60.
  print(lat_int, lon_int, yi, mi, di, pdi, yf, mf, df, pdf, o3_column, tro3_column)

# second, create a list of Pandora sites;
# find Pandora site in the closest vicinity of TOLNET.
#
# discovering available Pandora site. please bear in mind that some sites do not have O3 data files
  print('gathering Pandora sites information') 
  refs = read_pandora_web()

  pandora_site = take_pandora_sites(refs) # create list of Pandora sites of interest
  print('the following site was selected')
  print(pandora_site)
  print('from the list of existing Pandora sites')

  link = instrument_path(pandora_site)
  pandora_fname = link[0].split('/')[-1]
  print(pandora_fname)

# check if file exists in the local directory, if not download from Pandora site.
# Pandora data files are bulky, so it is recommended to keep them if multiple use is intended.
# if not, uncomment line 'os.remove(fname)' below and the code will delete files after reading. 
  if not os.path.exists(pandora_fname):
    print(pandora_fname,' does not exit in local directory, downloading from the web')
    print(link)

    pandora_fname, response_code = download(link)

    if response_code == 200:
      print('Pandora L2 file ', pandora_fname, ' has been downloaded')
    else:
      print('Pandora L2 file ', link, ' does not exist')
      print('program terminated')
      sys.exit()

  else:
    print(pandora_fname,' exits in local directory')

  yyyymmdd = str('%4.4i%2.2i%2.2i'%(yi, mi, di))
  lat, lon, pandora_data = read_pandora_file(pandora_fname, yyyymmdd)
  print(lat, lon)
  print(pandora_data)
  
# line below can be uncommented if user does not want to keep Pandora files.
# this is not recommended as downloading again takes extra time.  
#  os.remove(fname)


# third, find EPIC L4 data on the same date then read these files. Both total and tropospheric column are to be read
# create lists of link of available EPIC L4 files for the month(s) of interest
  print('gathering lists of EPIC L4 O3 data for selected date') 
  epic_links = list_epic_L4_tro3(yyyymmdd)
  for link in epic_links:
    print(link)
  n_epic_links = len(epic_links) 

# now reading EPIC data and create array EPIC_data
  array = np.empty((n_epic_links, 2)) # second dimension of 2 is for total and tropospheric columns
  dates = np.empty((n_epic_links, 3)) # second dimension of 3 is for year, mm, day

  cnt = -1 
  for j in range(n_epic_links):
    url = str(epic_links[j])
    fname, response_code = download_epic(url)
# epic_data_loc is 2D array of EPIC O3 retrievals over Pandora sites returned by function read_DSCOVR_EPIC_L4_O3
    if response_code == 200:
      cnt = cnt + 1
      array[cnt, 0], array[cnt, 1] = read_DSCOVR_EPIC_L4_O3(fname, lat, lon)
      year, mm, day = timestamp2day_epic(fname)
      dates[cnt, :] = [year, mm, day]
      os.remove(fname)
  epic_data = np.concatenate((dates[0 : cnt+1, :], array[0 : cnt+1, 0:2]), axis=1)

# remove zeroes from EPIC data
  epic_data_clean = epic_data[epic_data[:, 3]>0] 
  print(epic_data_clean)
#  sys.exit()
# plotting diurnal cycles
  pandora_ndata = len(pandora_data)
  epic_ndata = len(epic_data_clean)

  if epic_ndata > 0 and pandora_ndata > 0:
    fig, ax = plt.subplots()
    ax.set_xlim([0, 24])
    ax.set_xticks(np.arange(0, 24, step=3))
    ax.plot([(x % 1)*24 for x in pandora_data[:, 2]], pandora_data[:,3], linestyle="-", color='c')
    ax.plot([(x % 1)*24 for x in epic_data_clean[:,2]], epic_data_clean[:,3], linestyle="None", marker='*', markersize=8, color='m')
    ax.plot([pdi*24, pdf*24], [o3_column, o3_column], linestyle="--", color='b')

    ax.set_xlabel(r'GMT, hour', fontsize=12)
    ax.set_ylabel(r'total O3, DU', fontsize=12)
    plt.title('total O3, '+pandora_site+' '+yyyymmdd)
    plt.savefig('total_O3_column_'+pandora_site+' '+yyyymmdd+'.jpg', format='jpg', dpi=300)
    plt.close()

    fig, ax = plt.subplots()
    ax.set_xlim([0, 24])
    ax.set_xticks(np.arange(0, 24, step=3))
    ax.plot([(x % 1)*24 for x in epic_data_clean[:,2]], epic_data_clean[:,4], linestyle="None", marker='*', markersize=8, color='m')
    ax.plot([pdi*24, pdf*24], [tro3_column, tro3_column], linestyle="--", color='b')

    ax.set_xlabel(r'GMT, hour', fontsize=12)
    ax.set_ylabel(r'tr O3, DU', fontsize=12)
    plt.title('tropospheric O3 column, '+pandora_site+' '+yyyymmdd)
    plt.savefig('tr_O3_column_'+pandora_site+'_'+yyyymmdd+'.jpg', format='jpg', dpi=300)
    plt.close()

  if epic_ndata == 0 and pandora_ndata > 0:
    fig, ax = plt.subplots()
    ax.set_xlim([0, 24])
    ax.set_xticks(np.arange(0, 24, step=3))
    ax.plot([(x % 1)*24 for x in pandora_data[:, 2]], pandora_data[:,3], linestyle="-", color='c')
    ax.plot([pdi*24, pdf*24], [o3_column, o3_column], linestyle="--", color='b')

    ax.set_xlabel(r'GMT, hour', fontsize=12)
    ax.set_ylabel(r'total O3, DU', fontsize=12)
    plt.title('total O3, '+pandora_site+' '+yyyymmdd)
    plt.savefig('total_O3_column_'+pandora_site+' '+yyyymmdd+'.jpg', format='jpg', dpi=300)
    plt.close()

  if epic_ndata > 0 and pandora_ndata == 0:
    fig, ax = plt.subplots()
    ax.set_xlim([0, 24])
    ax.set_xticks(np.arange(0, 24, step=3))
    ax.plot([(x % 1)*24 for x in epic_data_clean[:,2]], epic_data_clean[:,3], linestyle="None", marker='*', markersize=8, color='m')
    ax.plot([pdi*24, pdf*24], [o3_column, o3_column], linestyle="--", color='b')

    ax.set_xlabel(r'GMT, hour', fontsize=12)
    ax.set_ylabel(r'total O3, DU', fontsize=12)
    plt.title('total O3, '+pandora_site+' '+yyyymmdd)
    plt.savefig('total_O3_column_'+pandora_site+' '+yyyymmdd+'.jpg', format='jpg', dpi=300)
    plt.close()

    fig, ax = plt.subplots()
    ax.set_xlim([0, 24])
    ax.set_xticks(np.arange(0, 24, step=3))
    ax.plot([(x % 1)*24 for x in epic_data_clean[:,2]], epic_data_clean[:,4], linestyle="None", marker='*', markersize=8, color='m')
    ax.plot([pdi*24, pdf*24], [tro3_column, tro3_column], linestyle="--", color='b')

    ax.set_xlabel(r'GMT, hour', fontsize=12)
    ax.set_ylabel(r'tr O3, DU', fontsize=12)
    plt.title('tropospheric O3 column, '+pandora_site+' '+yyyymmdd)
    plt.savefig('tr_O3_column_'+pandora_site+'_'+yyyymmdd+'.jpg', format='jpg', dpi=300)
    plt.close()
    
f.close()    

sys.exit()
