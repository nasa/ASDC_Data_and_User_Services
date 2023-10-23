import os
import sys
import requests
import codecs
import numpy as np
import h5py
import matplotlib.pyplot as plt
from urllib.request import urlopen, Request
from pathlib import Path

def read_pandora_web():
# function read_pandora_web returns the list of available Pandora sites
  url = 'http://data.pandonia-global-network.org/'
  page = urlopen(url)
  html_bytes = page.read()
  html = html_bytes.decode("utf-8")
  html_len = len(html)

  pos1 = 0

  big_line = str(html)
  lines = big_line.split('\r\n')

  ref_lines = [i for i in lines if 'href' in i]
  refs = [None]*len(ref_lines)
  cnt = -1
  for line in ref_lines:
    pos1 = line.index('"')
    pos2 = line.index('"', pos1+1)
    if pos1 > 0 and pos2 > pos1 and line[pos2-1] =='/' and line[pos1+1].isupper():
      cnt = cnt + 1
      refs[cnt] = line[pos1+1 : pos2-1]
    
  return refs[0:cnt+1]


def instrument_path(site):
# function instrument_path takes Pandora site name and returns url of the L2 ozone product data file
  url = 'http://data.pandonia-global-network.org/' + site + '/'
  page = urlopen(url)
  html_bytes = page.read()
  html = html_bytes.decode("utf-8")
  html_len = len(html)

  pos1 = 0
  big_line = str(html)
  lines = big_line.split('\r\n')

  ref_lines = [i for i in lines if 'href' in i]
  for line in ref_lines:
    pos1 = line.index('"')
    pos2 = line.index('"', pos1+1)
    if pos1 > 0 and pos2 > pos1 and line[pos2-1] =='/' and line[pos1+1 : pos1 + 8] == 'Pandora':
      link = url + line[pos1+1 : pos2] + 'L2/' + line[pos1+1 : pos2-1] + '_' + site + '_L2_rout2p1-8.txt'
      break

  return link    


def list_epic_L4_tro3(yyyymm):
# function list_epic_L4_tro3 takes month of interest in the form of an 6-character string yyyymm.
# the function returns a list of all DSCOVR_EPIC_L4_TRO3 files publicly availabe at the ASDC, see variable url below.
# user must have an account at https://urs.earthdata.nasa.gov/
# and use that account to create and update variable token.
# if token is outdated, the list will not be created.   
  yyyy = yyyymm[0 : 4]
  mm   = yyyymm[4 : 7]
  url = 'https://asdc.larc.nasa.gov/data/DSCOVR/EPIC/L4_TrO3_01/'
  token = 'eyJ0eXAiOiJKV1QiLCJvcmlnaW4iOiJFYXJ0aGRhdGEgTG9naW4iLCJzaWciOiJlZGxqd3RwdWJrZXlfb3BzIiwiYWxnIjoiUlMyNTYifQ.eyJ0eXBlIjoiVXNlciIsInVpZCI6ImFsZXhyYWQ3MSIsImV4cCI6MTY5MDAzOTQ3MSwiaWF0IjoxNjg0ODU1NDcxLCJpc3MiOiJFYXJ0aGRhdGEgTG9naW4ifQ.YsdKnbqy7SO6ZIct154AlXV7-gg6gZK9JY_-iHCHUABjQHS8mCO23CaM-GRsn1KaIbM2SI4F6sPJtZqcZcgMKcmx8YSjozpSbswjlDPl1pvxxtVbntTsskc42Ekaup_LgGT_9cWXMEajFWIEcRtdmdfj3RokXqPzwwfeUyehhD5hYyydgdd3v7FgibZDh9Cf-wdgPlK9sdgoENUQ9X45npAUVsG2topzaVl0jSHC-4-3-gYWl_8xuD_SdKutP5mkuG7FsR8FNV72vBVWOwd_bUGoyBrbrjOtydDb95s8c9Py5K1h8Yr114Mmim2gpM7AF6k52syCnqHS2mXGGSOkhQ'
  
  header={"Authorization": f"Bearer {token}"}
  
  req = Request(url+yyyy+'/'+mm+'/', headers=header)
  with urlopen(req) as response:
    html_bytes = response.read()
    html = html_bytes.decode("utf-8")
    html_len = len(html)
    big_line = str(html)
    lines = big_line.split('\n')
    nlines = len(lines)

  ref_lines = [i for i in lines if ('href' in i) and ('DSCOVR_EPIC_L4_TrO3_01' in i)]
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


def check_site(site_name, refs):
  site_list = []
  for line in refs:
    if site_name in line:
      site_list.append(line) 
      
  return site_list
  

def take_pandora_sites(refs):
# function take_pandora_sites takes user input on Pandora site needed to be read.
  print('please select a Pandora site name from the list')
  for ref in refs:
    print(ref)
  
  answer = 'y'
  pandora_sites = []
  while answer == 'y':
    site_name = input('Enter a name of a Pandora site: ')
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

      for site in site_list:
        if site == site_name:
          pandora_sites.append(site_name)
          print('site ', site_name, 'was found and added to the list of sites ')
          break
        
    if site_num == 1:
      pandora_sites.append(site_list[0])
      print('site ', site_list[0], 'was found and added to the list of sites ')

    answer = input('Do you want to enter another site name? ("y" or "n")')
    
  return pandora_sites


def download_epic(url):
# function download_epic takes data file of interest given by its URL
# from the list created by function list_epic_L4_tro3.
# the function returns filename and request response_code for control purposes.
# download is successful if response_code == 200.
# user must have an account at https://urs.earthdata.nasa.gov/
# and use that account to create and update variable token.
# if token is outdated, the list will not be created.   
  token="eyJ0eXAiOiJKV1QiLCJvcmlnaW4iOiJFYXJ0aGRhdGEgTG9naW4iLCJzaWciOiJlZGxqd3RwdWJrZXlfb3BzIiwiYWxnIjoiUlMyNTYifQ.eyJ0eXBlIjoiVXNlciIsInVpZCI6ImFsZXhyYWQ3MSIsImV4cCI6MTY4OTM3NDI4NywiaWF0IjoxNjg0MTkwMjg3LCJpc3MiOiJFYXJ0aGRhdGEgTG9naW4ifQ.0XtLHfPZK8D_YiQBMfJLnHkbT-392YSlB6awj1jYtmH_73fIYcMDAqH9xOEPE2M8Xp35xAzJZdrB-YKmpT3ZnNeI8jveQa7EAenYqKdvgMWG_oRX2vQAgQdRURBTJs4ZRH_8hlZgaNiht2v2ieYUx9EjagqIqJ96LEYhsNmAo0D8U25IH8P-uHWeVwHPFif9AFdczKU_H4bmkwd_F6Z9UKsQb53hUOGiNY_1aPveJ6id4fchU1tY6KMp6fidDXVo7UMwUo3Wn_1_P55X1MNHZhbae__rc-a_Wd4P-Z_4aECbkE-JgrAbO1hQdFNvE4AzUiL2T0RAun3Pmme_UjNu1Q"
  header={"Authorization": f"Bearer {token}"}
  response = requests.get(url, headers=header)
  response_code = response.status_code
  file_name = url.split('/')[-1]

  if response_code == 200:
    content = response.content
    data_path = Path(file_name)
    data_path.write_bytes(content)

  return file_name, response_code


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
  
  
def timestamp2day_epic(fname):
# function timestamp2day-epic converts EPIC L4 file name into a set of 3 numbers:
# integer year, month, and real day.
# real day of the month is number of days since the beginning of the month, e.g. 18:00 May 31 is 30.75. 
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


def timestamp2day_pandora(timestamp):
# function timestamp2day converts Pandora timestamp of the format 'yyyymmddThhmmssZ' into a set of 3 numbers:
# integer year, month, and real day of the month.
# real day of the month is number of days since the beginning of the month, e.g. 18:00 May 31 is 30.75. 

  yyyy = int(timestamp[0:4])
  mm = int(timestamp[4:6])
  dd = int(timestamp[6:8])
  hh = int(timestamp[9:11])
  mn = int(timestamp[11:13])
  ss = float(timestamp[13:17])

  day = dd - 1 + (hh + mn/60. + ss/3600.)/24.

  return yyyy, mm, day


def read_pandora_file(fname, yyyymm):
# function read_pandora_file takes Pandora data file name assuming it has been downloaded
# and list of months of interest in the form [yyyymm, ...].
# the function returns latitude and longitude of the Pandora site
# and an array of arrays of 4 columns: year, month, fractional day, and ozone column in Dobson units (DU).
# fractional day is time in day since beginning of the month,
# e.g. May 31, 18:00 would have fractional day of 30.75.

  DU_conversion = 1./4.4615E-04
  n_days = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]

  nlines = 0
  with codecs.open(fname, 'r', encoding='utf-8', errors='ignore') as f:

    while True:
      line = f.readline()

      if not line:
        break

      nlines = nlines + 1

  nhl = 0 # number of header lines

  with codecs.open(fname, 'r', encoding='utf-8', errors='ignore') as f:

    while True:
# Get next line from file
      line = f.readline()

      if nhl < nlines:
        nhl += 1

      if line.find('Short location name:') >= 0:
        loc_name = line.split()[-1] # location name, to be used in the output file name

      if line.find('Location latitude [deg]:') >= 0:
        lat = float(line.split()[-1]) # location latitude

      if line.find('Location longitude [deg]:') >= 0:
        lon = float(line.split()[-1]) # location longitude

      if line.find('--------') >= 0:
        ndc = 0 # number of data columns
        break

    while True:
# Get next line from file
      line = f.readline()

      if nhl < nlines:
        nhl += 1

      if line.find('--------') >= 0:
        break

      ndc += 1

    ndl = nlines - nhl
    print(ndl, ' data lines found in ', fname)
    data = np.array([[None for j in range(4)] for i in range(ndl)])

# now reading line with data
    cnt = -1
    while True:
      line = f.readline()

      if not line:
        break

      line_split = line.split()
      cnt += 1
      data[cnt, 0], data[cnt, 1], data[cnt, 2] = timestamp2day_pandora(line_split[0])
      data[cnt, 3] = float(line_split[38])*DU_conversion

    nmonth = len(yyyymm)
    monthly_data = np.empty(nmonth, dtype=object) # create an array of arrays; each element is for a single month of interest presented in yyyymm.
    true_monthly_data = np.empty(nmonth, dtype=object) # create an array of arrays; each element is for a single month of interest presented in yyyymm; this is to be the output of function read_pandora_file.
    for k in range(nmonth): # cycle by supplied months, yyyymm
# now search for specific year/month
      current_year = int(yyyymm[k][0:4])
      current_month = int(yyyymm[k][4:6])

      nd = n_days[current_month - 1]
      if (current_year//4)*4 == current_year and current_month == 2:
        nd = nd + 1
      
      monthly_data[k] = data[(data[:, 0] == current_year) & (data[:, 1] == current_month) & (data[:, 3] > 0.)]
      ndata = len(monthly_data[k])
      print(ndata, ' observations read for ', yyyymm[k], 'at Pandora site ', loc_name, lat,'N, ', lon,'E')

# Now splitting days, remove zeroes
      daily_data = np.empty(nd, dtype=object)
      true_daily_data = np.empty(nd, dtype=object) # days with data, if a day does not have data then corresponding array is empty

      for i in range(nd):
        daily_data[i] = monthly_data[k][(monthly_data[k][:, 2] > i) & (monthly_data[k][:, 2] < i + 1) & (monthly_data[k][:, 3] > 0.)]
        n_daily_data = len(daily_data[i])

        if n_daily_data > 0:
          true_daily_data[i] = daily_data[i]
        else:
          true_daily_data[i] = []
        
      true_monthly_data[k] = true_daily_data

  return lat, lon, true_monthly_data
      
      
def read_DSCOVR_EPIC_L4_totO3(fname, lat0, lon0):
# function read_DSCOVR_EPIC_L4_totO3 reads EPIC L4 file supplied with fname and returns O3 column(s)
# in all points of interest supplied as lists of latitudes, lat0, ang longitudes, lon0.
# the function returns a list of the same length as lat0 and lon0

# Open file.
  f = h5py.File(fname, 'r')

  lat = np.array(f['Latitude'])
  lon = np.array(f['Longitude'])
  totO3 = np.array(f['TotalColumnOzone'])
  f.close()

  nlat = len(lat)
  nlon = len(lon)
  
  n_poi = min(len(lat0), len(lon0))

# interpolate ozone column for the POIs in a cycle
  O3_col = [0.]*n_poi

  for k in range(n_poi):
  
    i0 = -1
    j0 = -1

# search for coordinates
    for i in range(nlat-1):
      if lat0[k] > lat[i] and lat0[k] < lat[i+1]:
        i0 = i
        break
  
    for j in range(nlon-1):
      if lon0[k] > lon[j] and lon0[k] < lon[j+1]:
        j0 = j
        break

    if i0 < 0 or j0 < 0:
      print(lat0[k], min(lat), max(lat), lon0[k], min(lon), max(lon), fname)
      sys.exit()
  
    wtSW = (lat[i0+1] - lat0[k])*(lon[j0+1] - lon0[k]) # weight for totO3[i0, j0]
    wtNW = (lat0[k] - lat[i0])*(lon[j0+1] - lon0[k])   # weight for totO3[i0+1, j0]
    wtSE = (lat[i0+1] - lat0[k])*(lon0[k] - lon[j0])   # weight for totO3[i0, j0+1]
    wtNE = (lat0[k] - lat[i0])*(lon0[k] - lon[j0])     # weight for totO3[i0+1, j0+1]
  
    wt_sum = 0.
  
    if totO3[i0, j0] > 0.:
      wt_sum = wt_sum + wtSW
      O3_col[k] = O3_col[k] + wtSW*totO3[i0, j0]
    if totO3[i0+1, j0] > 0.:
      wt_sum = wt_sum + wtNW
      O3_col[k] = O3_col[k] + wtNW*totO3[i0+1, j0]
    if totO3[i0, j0+1] > 0.:
      wt_sum = wt_sum + wtSE
      O3_col[k] = O3_col[k] + wtSE*totO3[i0, j0+1]
    if totO3[i0+1, j0+1] > 0.:
      wt_sum = wt_sum + wtNE
      O3_col[k] = O3_col[k] + wtNE*totO3[i0+1, j0+1]
  
    if wt_sum > 0.:
      O3_col[k] = O3_col[k]/wt_sum

  return O3_col


# this code is derived from read_Pandora_web_03.py !!! the comment is to be removed

# main program begins here
# the intent of this code is to plot daily graphs of total ozone columns
# derived from DSCOVR_EPIC_L4_TrO3 data over Pandora sun-photometer site(s) and compare against Pandora O3 columns.
# the code takes month(s) of interest, yyyymm, as command line argument. If no arguments provided, execution stops.
# the code takes Pandora site(s) as user input. 
# the code enable O3 diurnal cycle research. 
narg = len(sys.argv)

if narg == 1:
  print('please supply at least one yyyymm of interest')
  print('program terminated')
  sys.exit()

nmonth = narg - 1
yyyymm = [None]*(nmonth)
yyyymm[0 : nmonth] = sys.argv[1 : narg]

n_days = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]

# now create lists of link of available EPIC L4 files for the month(s) of interest
epic_links = np.empty(nmonth, dtype=object)
print('gathering lists of EPIC L4 O3 data for selected months') 
for i in range(nmonth):
  epic_links[i] = list_epic_L4_tro3(yyyymm[i])

# Discovering available Pandora site.
# Please bear in mind that some sites do not have O3 data files
print('gathering Pandora sites information') 
refs = read_pandora_web()

pandora_sites = take_pandora_sites(refs) # create list of Pandora sites of interest
print('the following sites were selected')
print(pandora_sites)
print('from the list of existing Pandora sites')

# create a list of !AVAILABLE! Pandora files for the list of Pandora sites
pandora_files = []
non_existing_sites = []
for site in pandora_sites:
  link = instrument_path(site)
  pandora_fname = link.split('/')[-1]
  print(pandora_fname)

# check if file exists in the local directory, if not download from Pandora site
  if not os.path.exists(pandora_fname):
    print(pandora_fname,' does not exit in local directory, downloading from the web')
    print(link)

    pandora_fname, response_code = download(link)

    if response_code == 200:
      print('Pandora L2 file ', pandora_fname, ' has been downloaded')
      pandora_files.append(pandora_fname)
    else:
      print('Pandora L2 file ', link, ' does not exist')
      non_existing_sites.append(site)
      print('Pandora site ', site, ' has been removed from the list')

  else:
    print(pandora_fname,' exits in local directory')
    pandora_files.append(pandora_fname)

for site in non_existing_sites:
  pandora_sites.remove(site)
print(pandora_sites)

# read Pandora file(s).
# These files are bulky, but there are just a few of them.
# For this reason, they are not removed from the local directory but left for potential future use
nfiles = len(pandora_files)
lat = [None]*nfiles
lon = [None]*nfiles
# pandora_data is an object with the following structure:
# [number of Pandora sites, number of months] * [number of days in particular month].
# data for a particular day [site, month][day] is a rectangular array with 4 columns
# yyyy mm day O3(DU),
# where day is a fractional day from the beginning of the month,
# e.g. observation of 302 DU at 12:48 PM on Feb. 28, 2000 would be
# 2000 2 27.533333 302.0 
pandora_data = np.empty((nfiles, nmonth), dtype=object)

for k in range(nfiles):
  lat[k], lon[k], pandora_data[k, :] = read_pandora_file(pandora_files[k], yyyymm)
    
# now reading EPIC data and create object EPIC_data with the same structure as pandora_data
# [number of Pandora sites, number of months] * [number of days in particular month].
# be aware that the last dimension is different from Pandora data structure, it will be handled later.

# epic_data is an object with the following structure:
# [number of Pandora sites] * [number of months] * [number of observations in particular month].
# data for a particular day [site][month][day] is a rectangular array with 4 columns
# yyyy mm day O3(DU),
# where day is a fractional day from the beginning of the month,
# e.g. observation of 302 DU at 12:48 PM on Feb. 28, 2000 would be
# 2000 2 27.533333 302.0 
epic_data = np.empty((nfiles, nmonth), dtype=object) # nfiles is the number of Pandora files that were read, i.e. this is the number of sites

for i in range(nmonth):
  epic_list = epic_links[i]
  n_epic_links = len(epic_list)
  array = np.empty((n_epic_links, nfiles)) # nfiles is the number of Pandora files that were read, i.e. this is the number of sites
  dates = np.empty((n_epic_links, 3)) # nfiles is the number of Pandora files that were read, i.e. this is the number of sites

  cnt = -1 
  for j in range(n_epic_links):
    url = str(epic_list[j])
    fname, response_code = download_epic(url)
# epic_data_loc is 1D array of EPIC O3 retrievals over Pandora sites returned by function read_DSCOVR_EPIC_L4_totO3, length of nfiles
    if response_code == 200:
      epic_data_loc = read_DSCOVR_EPIC_L4_totO3(fname, lat, lon)
      year, mm, day = timestamp2day_epic(fname)
      cnt = cnt + 1 
      dates[cnt, :] = [year, mm, day]
      array[cnt, :] = epic_data_loc
      os.remove(fname)

  for k in range(nfiles):
    epic_data[k, i] = np.concatenate((dates[0 : cnt+1, :], array[0 : cnt+1, k:k+1]), axis=1)

# remove zeroes from EPIC data
epic_data_clean = np.empty((nfiles, nmonth), dtype=object) # nfiles is the number of Pandora files that were read, i.e. this is the number
for k in range(nfiles):
  for i in range(nmonth):     

    current_year = int(yyyymm[i][0:4])
    current_month = int(yyyymm[i][4:6])

    nd = n_days[current_month - 1]
    if (current_year//4)*4 == current_year and current_month == 2:
      nd = nd + 1

    epic_daily_data = np.empty(nd, dtype=object)
    true_epic_daily_data = np.empty(nd, dtype=object) # days with data

    for j in range(nd):
      epic_daily_data[j] = epic_data[k, i][(epic_data[k, i][:, 2] > j) & (epic_data[k, i][:, 2] < j + 1) & (epic_data[k, i][:, 3] > 0.)]
      n_daily_data = len(epic_daily_data[j])

      if n_daily_data > 0:
        true_epic_daily_data[j] = epic_daily_data[j]
      else:
        true_epic_daily_data[j] = []
        
    epic_data_clean[k, i] = true_epic_daily_data[:]

# plotting diurnal cycles
for k in range(nfiles):
  for i in range(nmonth):     

    out_fname_blank = 'total_O3'+pandora_sites[k]+yyyymm[i]
    current_year = int(yyyymm[i][0:4])
    current_month = int(yyyymm[i][4:6])

    nd = n_days[current_month - 1]
    if (current_year//4)*4 == current_year and current_month == 2:
      nd = nd + 1
      
    for j in range(nd):
      pandora_ndata = len(pandora_data[k, i][j])
      epic_ndata = len(epic_data_clean[k, i][j])

      if epic_ndata > 0 and pandora_ndata > 0:
        fig, ax = plt.subplots()
        ax.plot((pandora_data[k, i][j][:,2] - j)*24, pandora_data[k, i][j][:,3], linestyle="-", color='c')
        ax.plot((epic_data_clean[k, i][j][:,2] - j)*24, epic_data_clean[k, i][j][:,3], linestyle="None", marker='*', markersize=8, color='m')

        ax.set_xlabel(r'GMT, hour', fontsize=12)
        ax.set_ylabel(r'O3, DU', fontsize=12)
        plt.title('total O3, '+pandora_sites[k]+yyyymm[i]+str('%2.2i' % (j+1)))
        plt.savefig(out_fname_blank+str('%2.2i' % (j+1))+'.jpg', format='jpg', dpi=300)
        plt.close()

      if epic_ndata == 0 and pandora_ndata > 0:
        fig, ax = plt.subplots()
        ax.plot((pandora_data[k, i][j][:,2] - j)*24, pandora_data[k, i][j][:,3], linestyle="-", color='c')

        ax.set_xlabel(r'GMT, hour', fontsize=12)
        ax.set_ylabel(r'O3, DU', fontsize=12)

        plt.title('total O3, '+pandora_sites[k]+yyyymm[i]+str('%2.2i' % (j+1)))
        plt.savefig(out_fname_blank+str('%2.2i' % (j+1))+'.jpg', format='jpg', dpi=300)
      plt.close()

      if epic_ndata > 0 and pandora_ndata == 0:
        fig, ax = plt.subplots()
        ax.plot((epic_data_clean[k, i][j][:,2] - j)*24, epic_data_clean[k, i][j][:,3], linestyle="None", marker='*', markersize=8, color='m')

        ax.set_xlabel(r'GMT, hour', fontsize=12)
        ax.set_ylabel(r'O3, DU', fontsize=12)

        plt.title('total O3, '+pandora_sites[k]+yyyymm[i]+str('%2.2i' % (j+1)))
        plt.savefig(out_fname_blank+str('%2.2i' % (j+1))+'.jpg', format='jpg', dpi=300)
    plt.close()

sys.exit()
