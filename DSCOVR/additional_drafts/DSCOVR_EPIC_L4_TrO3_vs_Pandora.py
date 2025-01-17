import os
import sys
import requests
import earthaccess
import platform
from subprocess import Popen
import shutil
import codecs
import numpy as np
import h5py
import matplotlib.pyplot as plt
from urllib.request import urlopen, Request
from pathlib import Path
from datetime import datetime, timedelta # needed to work with time in plotting time series

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


# function check_site checks whether user entered site is in the list of available Pandora sites
def check_site(site_name, refs):
  site_list = []
  for line in refs:
    if site_name in line:
      site_list.append(line)

  return site_list


# function take_pandora_sites takes user input and checks whether the site is in the list of available Pandora sites
def take_pandora_sites(refs):
  print('please select a Pandora site name from the list')
  for ref in refs:
    print(ref)

  answer = 'y'
  while answer == 'y':
    site_name = input('Enter a name of a Pandora site: ')
    print(site_name)
    site_list = check_site(site_name, refs)
    site_num = len(site_list)
    if site_num == 0:
      print('site ', site_name, 'was not found')
      continue

    if site_num > 1:
      print('there are ', site_num, ' site names, select one from')
      for site in site_list: print(site)

      site_name = input('Enter an exact name of a Pandora site: ')
      if site_list.count(site_name) != 1:
        print('Entered name is not the exact match of one of the following sites')
        for site in site_list: print(site)
        print('program terminated')
        sys.exit()

      for site in site_list:
        if site == site_name:
          pandora_site = site_name
          print('site ', site_name, 'was found and added to the list of sites ')
          break

    if site_num == 1:
      pandora_site = site_list[0]
      print('site ', site_list[0], 'was found and added to the list of sites ')

    answer = 'n'

  return pandora_site


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


# Defining functions downloading Pandora data file with given url
def download(url):
  response = requests.get(url)
  response_code = response.status_code

  file_name = url.split('/')[-1]

  if response_code == 200:
    content = response.content
    data_path = Path(file_name)
    data_path.write_bytes(content)

  return file_name, response_code


# function read_timestamp converts Pandora timestamp of the format
# 'yyyymmddThhmmssZ' into a set of 6 numbers:
# integer year, month, day, hour, minute, and real second.
def read_timestamp(timestamp):

  yyyy = int(timestamp[0:4])
  mm = int(timestamp[4:6])
  dd = int(timestamp[6:8])
  hh = int(timestamp[9:11])
  mn = int(timestamp[11:13])
  ss = float(timestamp[13:17])

  return yyyy, mm, dd, hh, mn, ss


# function read_Pandora_O3_rout2p1_8. It is to be used for the future validation efforts.
# The difference with the original version is that instead of discriminating negative values of the total O3 column,
# it uses quality flags. It was previously found that QF == 0 does not occure often enough,
# so we will have to use QF == 10 (not-assured high quality).
#
# function read_Pandora_O3_rout2p1_8 reads Pandora total O3 column data files ending with rout2p1-8.
# Arguments:
# fname - name file to be read, string;
# start_date - beginning of the time interval of interest,
#              integer of the form YYYYMMDD;
# end_date -   end of the time interval of interest,
#              integer of the form YYYYMMDD.
#
# if start_date is greater than end_date, the function returns a numpy array
# with shape (0, 8), otherwise it returns an 8-column numpy array
# with with columns being year, month, day, hour, minute, second of observation
# and retrieved total O3 column along with its total uncertainty.
#
# O3 column and its uncertainties are in mol/m^2, so conversion to Dobson Units is
# performed by multiplication by DU_conversion = 1./4.4615E-04
def read_Pandora_O3_rout2p1_8_v2(fname, start_date, end_date):

  DU_conversion = 1./4.4615E-04

  data = np.empty([0, 8])
  if start_date > end_date: return -999., -999., data

  with codecs.open(fname, 'r', encoding='utf-8', errors='ignore') as f:

    while True:
# Get next line from file
      line = f.readline()

      if line.find('Short location name:') >= 0:
        loc_name = line.split()[-1] # location name, to be used in the output file name
        print('location name ', loc_name)

      if line.find('Location latitude [deg]:') >= 0:
        lat = float(line.split()[-1]) # location latitude
        print('location latitude ', lat)

      if line.find('Location longitude [deg]:') >= 0:
        lon = float(line.split()[-1]) # location longitude
        print('location longitude ', lon)

      if line.find('--------') >= 0: break

    while True:
# Get next line from file
      line = f.readline()

      if line.find('--------') >= 0: break

    while True:
# now reading line with data
      line = f.readline()

      if not line: break

      line_split = line.split()

      yyyy, mm, dd, hh, mn, ss = read_timestamp(line_split[0])
      date_stamp = yyyy*10000 + mm*100 + dd
      if date_stamp < start_date or date_stamp > end_date: continue

      QF = int(line_split[35]) # quality flag

      if QF == 0 or QF == 10:
        column = float(line_split[38])
#        column_unc = float(line_split[42]) # Total uncertainty of ozone total vertical column amount [moles per square meter]
        column_unc = float(line_split[43]) # rms-based uncertainty of ozone total vertical column amount [moles per square meter]
        data = np.append(data, [[yyyy, mm, dd, hh, mn, ss\
                               , column*DU_conversion\
                               , column_unc*DU_conversion]], axis = 0)

  return lat, lon, loc_name, data
      
      
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


# Smooth Pandora retievals and interplate them into other time series times
# Pandora timeseries has significantly more data points then TEMPO and DSCOVR. It is also very noisy.
# To make comparison easier, Pandora timeseries is interpolated to the moments of TEMPO and DSCOVR observations.

# Interpolation is performed by the function defined below with the help of Gaussian smooting as follow:
# x_int(t) = SUM(x_p(t_i)*wt(t_i, t)),
#
# wt(t_i, t) = exp(-(t - t_i)^2/(2 * sigma^2))/SUM(exp(-(t - t_i)^2/(2 * sigma^2))),
#
# where sums are taken over times t_i falling into time interval (t-dt_max, t+dt_max).
#
# Parameters dt_max and sigma can be chosen by the user.
def gauss_interpolation(timeseries, new_times):
#
# function gauss_interpolation takes 2D array timeseries with function
# to be interpolated and 1D array new_times containing times to which
# the function is to be interpolated
# arguments:
#     timeseries - array with at least 2 columns,
#                  1st column - times, 2nd (3rd, ...) column(s) - function to be interpolated
#     new_times  - 1D array of times to which the function(s) to be interpolated
#
# parameters
  dt_max = 0.0041666667 # 6 minutes expressed in days
  sigma = 0.00041666667 # 36 seconds expressed in days

  nnt = len(new_times)
  (nt, nfun) = timeseries.shape

  timeseries_smooth = np.empty([0, nfun])

  for new_time in new_times:
    llim = new_time - dt_max
    ulim = new_time + dt_max

    timeseries_subset = timeseries[((timeseries[:, 0] < ulim)\
                                    & (timeseries[:, 0] > llim))]
    if len(timeseries_subset) < 1: continue
    t_delta = timeseries_subset[:, 0] - new_time
    wt = np.exp(-t_delta**2/(2.*sigma**2))
    wt = wt/np.sum(wt)

    timeseries_smooth_loc = np.array([new_time])
    for ifun in range(1, nfun):
      timeseries_smooth_loc = np.append(timeseries_smooth_loc,\
                                        np.sum(timeseries_subset[:, ifun]*wt))

    timeseries_smooth = np.append(timeseries_smooth,\
                                  np.array([timeseries_smooth_loc]), axis = 0)

  return timeseries_smooth


# custom made function regress_0intercept takes vectors x and y
# representing coordinates and function values at these coordinates
# and returns slope of regression fit y = a*x
# along with coefficient of determination
def regress_0intercept(x, y):
  success = False

  if len(x) != len(y):
    a = 0.
    R2 = 0.

  elif len(x) == 1:
    if x[0] != 0.:
      a = y[0]/x[0]
      R2 = 1.
      success = True
    else:
      if y[0] != 0.:
        a = np.inf
        R2 = 1.
        success = True
      else:
        a = np.inf
        R2 = 0.

  else:
    xy_sum = np.dot(x, y)
    x2_sum = np.dot(x, x)
    a = xy_sum/x2_sum

    res_y = y - a*x
    res_sum_2 = np.dot(res_y, res_y)
    y2_sum = np.dot(y, y)
    sum_tot_2 = y2_sum - len(y)*np.mean(y)**2
    R2 = 1. - res_sum_2/sum_tot_2

    success = True

  return success, a, R2


# main program begins here
# the intent of this code is to plot daily graphs of total ozone columns
# derived from DSCOVR_EPIC_L4_TrO3 data over Pandora sun-photometer site(s) and compare against Pandora O3 columns.
# the code takes month(s) of interest, yyyymm, as command line argument. If no arguments provided, execution stops.
# the code takes Pandora site(s) as user input. 
# the code enable O3 diurnal cycle research. 

# Establishing access to EarthData
# User needs to create an account at https://www.earthdata.nasa.gov/
# Function earthaccess.login prompts for EarthData login and password.
auth = earthaccess.login(strategy="interactive", persist=True)

homeDir = os.path.expanduser("~") + os.sep

with open(homeDir + '.dodsrc', 'w') as file:
    file.write('HTTP.COOKIEJAR={}.urs_cookies\n'.format(homeDir))
    file.write('HTTP.NETRC={}.netrc'.format(homeDir))
    file.close()

print('Saved .dodsrc to:', homeDir)

# Set appropriate permissions for Linux/macOS
if platform.system() != "Windows":
    Popen('chmod og-rw ~/.netrc', shell=True)
else:
    # Copy dodsrc to working directory in Windows
    shutil.copy2(homeDir + '.dodsrc', os.getcwd())
    print('Copied .dodsrc to:', os.getcwd())

# Discovering available Pandora site.
# Please bear in mind that some sites do not have NO2 data files
print('gathering Pandora sites information')
refs = read_pandora_web()

pandora_site = take_pandora_sites(refs) # create list of Pandora sites of interest
print('the following sites were selected')
print(pandora_site)
print('from the list of existing Pandora sites')

# create a list of !AVAILABLE! Pandora files for the Pandora site
pandora_files = []

links = instrument_path(pandora_site)

npfiles = 0

for link in links:
  pandora_fname = link.split('/')[-1]

# check if file exists in the local directory, if not download from Pandora site
  if not os.path.exists(pandora_fname):
    print(pandora_fname,' does not exit in local directory, downloading from the web')
    print(link)

    pandora_fname, response_code = download(link)

    if response_code == 200:
      print('Pandora L2 file ', pandora_fname, ' has been downloaded')
      npfiles = npfiles + 1
      pandora_files.append(pandora_fname)
    else:
      print('Pandora L2 file ', link, ' does not exist')

  else:
    print(pandora_fname,' exits in local directory')
    npfiles = npfiles + 1
    pandora_files.append(pandora_fname)

if npfiles == 0: # no files were found, STOP here
  print('no files were found for Pandora site ', pandora_site, 'program terminated')
  sys.exit()
if npfiles > 1: # normally there should be only one file per site. if there are more - STOP
  print('there are too many files for site ', pandora_site, '- STOP and investigate file names below. Program terminated')
  for pandora_fname in pandora_files: print(pandora_fname)
  sys.exit()

# Selecting timeframe of interest common for all instruments
print('enter period of interest, start and end dates, in the form YYYYMMDD')
datestamp_ini = input('enter start date of interest ')
datestamp_fin = input('enter end date of interest ')

start_date = int(datestamp_ini)
end_date = int(datestamp_fin)

yyyy_ini = start_date//10000
mm_ini = (start_date//100 - yyyy_ini*100)
dd_ini = (start_date - yyyy_ini*10000 - mm_ini*100)

yyyy_fin = end_date//10000
mm_fin = (end_date//100 - yyyy_fin*100)
dd_fin = (end_date - yyyy_fin*10000 - mm_fin*100)
print(yyyy_ini, mm_ini, dd_ini, yyyy_fin, mm_fin, dd_fin)

date_start = str('%4.4i-%2.2i-%2.2i 00:00:00' %(yyyy_ini, mm_ini, dd_ini))
date_end = str('%4.4i-%2.2i-%2.2i 23:59:59' %(yyyy_fin, mm_fin, dd_fin))

# read Pandora file
pandora_file = pandora_files[0]
# pandora_data is a rectangular array with 8 columns
# yyyy mm dd hh mn sec O3_col(DU) O3_col_unc(DU)
lat, lon, POI_name, Pandora_data = read_Pandora_O3_rout2p1_8_v2(pandora_file, start_date, end_date)

if lat == -999.:
  print('error reading pandora file ', pandora_file, 'program terminated')
  sys.exit()

# print # of points in Pandora timeseries
n_Pandora_data = len(Pandora_data)
print(n_Pandora_data,\
' Pandora measurements found within period of interes between',\
date_start, 'and', date_end)
if n_Pandora_data == 0:
  print('program terminated')
  sys.exit()

print('Pandora data')
for Pd in Pandora_data: print(Pd)

short_name_DSCOVR = 'DSCOVR_EPIC_L4_TrO3' # collection name to search for in the EarthData

results_EPIC = earthaccess.search_data(short_name = short_name_DSCOVR\
                                         , temporal = (date_start, date_end)\
                                         , point = (lon, lat))

# Printing explicit links to the granules.
EPIC_links = []
for result in results_EPIC: EPIC_links.append(result['umm']['RelatedUrls'][0]['URL'])

failed_links = []
for EPIC_link in sorted(EPIC_links):
  print(EPIC_link)
  EPIC_fname = EPIC_link.split('/')[-1]
  downloaded_files = earthaccess.download(EPIC_link,
                                          local_path='../../_additional_scripts/DSCOVR')
# check if file exists in the local directory,
# if not, try to download again, if second attampt to download fails, remove from the list
  if not os.path.exists(EPIC_fname):
    downloaded_files = earthaccess.download(EPIC_link, local_path='../../_additional_scripts/DSCOVR')
    if not os.path.exists(EPIC_fname): failed_links.append(EPIC_link)

for link in failed_links: EPIC_links.remove(link)

# now reading EPIC data and create object EPIC_data with the similar structure to pandora_data
# EPIC_data is a rectangular array with 7 columns
# yyyy mm dd hh mn ss O3(DU)

n_EPIC = len(EPIC_links)
EPIC_data = np.empty([0, 7])
for EPIC_link in EPIC_links:
  EPIC_fname = EPIC_link.split('/')[-1]
  [epic_data_loc] = read_DSCOVR_EPIC_L4_totO3(EPIC_fname, [lat], [lon])
  if epic_data_loc <= 0.: continue
  timestamp = EPIC_link.split('_')[-2]
  yyyy = int(timestamp[0:4])
  mm = int(timestamp[4:6])
  dd = int(timestamp[6:8])
  hh = int(timestamp[8:10])
  mn = int(timestamp[10:12])
  ss = int(timestamp[12:14])
  print(yyyy, mm, dd, hh, mn, ss, epic_data_loc)
  EPIC_data = np.append(EPIC_data, [[yyyy, mm, dd, hh, mn, ss, epic_data_loc]], axis = 0)
  os.remove(EPIC_fname)

print('EPIC data')
for Ed in EPIC_data: print(Ed)

dt0 = datetime(yyyy_ini, mm_ini, dd_ini, 0, 0, 0) # this is moment of time at the beginning of the period of interest
dt_fin = datetime(yyyy_fin, mm_fin, dd_fin, 23, 59, 59, 999999) # this is time 1 microsecond before the end of the timeframe of interest

# creating Pandora timeseries
time_series_Pandora = np.empty([0, 3])

for line in Pandora_data:
  yyyy = int(line[0])
  mm = int(line[1])
  dd = int(line[2])
  hh = int(line[3])
  mn = int(line[4])
  ss = int(line[5])
# dt below is time since the beginning of the period of interest in hours
  dt = (datetime(yyyy, mm, dd, hh, mn, ss) - dt0).total_seconds()/86400.
  time_series_Pandora = np.append(time_series_Pandora,\
[[dt, line[6], line[7]]], axis = 0)

# creating DSCOVR timeseries
time_series_DSCOVR = np.empty([0, 2])

for line in EPIC_data:
  yyyy = int(line[0])
  mm = int(line[1])
  dd = int(line[2])
  hh = int(line[3])
  mn = int(line[4])
  ss = int(line[5])
# dt below is time since the beginning of the eriod of interest in hours
  dt = (datetime(yyyy, mm, dd, hh, mn, ss) - dt0).total_seconds()/86400.
  time_series_DSCOVR = np.append(time_series_DSCOVR,\
 [[dt, line[6]]], axis = 0)

# No error bars
plot_title = 'tot O3'+datestamp_ini+'_'+datestamp_fin+'\n'+POI_name
img_name = 'tot_O3_'+datestamp_ini+'_'+datestamp_fin+'_'+POI_name+'.jpg'

plt.plot(time_series_Pandora[:, 0], time_series_Pandora[:, 1],\
         label = "Pandora", c = 'r')
plt.plot(time_series_DSCOVR[:, 0], time_series_DSCOVR[:, 1],
         label = "DSCOVR", c = 'c')

# Set the range of x-axis
l_lim = 0.
u_lim = ((dt_fin - dt0).total_seconds() + 1.)/86400.
plt.xlim(l_lim, u_lim)

# some research is required to set the vertical range
# Set the range of y-axis
#l_lim = 275
#u_lim = 375
#plt.ylim(l_lim, u_lim)

plt.xlabel('GMT, day from beginning of '+datestamp_ini, fontsize=12)
plt.ylabel('total O3 column, DU', fontsize=12)

plt.legend(loc='lower left')

plt.title(plot_title+str(', %08.4fN %08.4fW' %(lat, -lon)))
plt.savefig(img_name, format='jpg', dpi=300)

plot_title = 'tot O3 unc '+datestamp_ini+'_'+datestamp_fin+'\n'+POI_name
img_name = 'tot_O3_unc_'+datestamp_ini+'_'+datestamp_fin+'_'+POI_name+'.jpg'

# Plotting DSCOVR EPIC and Pandora retievals with error bars
fig = plt.figure()

plt.errorbar(time_series_Pandora[:, 0],\
             time_series_Pandora[:, 1],\
             yerr=time_series_Pandora[:, 2],\
             label = "Pandora",\
             c = 'r', ls = '', marker = ".")

plt.errorbar(time_series_DSCOVR[:, 0], time_series_DSCOVR[:, 1],\
yerr=None, label = "DSCOVR", c = 'c', ls = '', marker = ".")

# Set the range of x-axis
l_lim = 0.
u_lim = ((dt_fin - dt0).total_seconds() + 1.)/86400.
plt.xlim(l_lim, u_lim)

# Set the range of y-axis
#l_lim = 275
#u_lim = 375
#plt.ylim(l_lim, u_lim)

plt.xlabel('GMT, day from beginning of '+datestamp_ini, fontsize=12)
plt.ylabel('total O3 column, DU', fontsize=12)

#plt.legend(loc='upper left')
plt.legend(loc='lower left')

plt.title(plot_title+str(', %08.4fN %08.4fW' %(lat, -lon)))
plt.savefig(img_name, format='jpg', dpi=300)

# Plotting DSCOVR EPIC and smoothed Pandora retievals with error bars
timeseries_Pandora_DSCOVR = gauss_interpolation(time_series_Pandora[:, 0:3],\
                                                time_series_DSCOVR[:, 0])

plot_title = 'tot O3 unc smooth '+datestamp_ini+'_'+datestamp_fin+'\n'+POI_name
img_name = 'tot_O3_unc_smooth_'+datestamp_ini+'_'+datestamp_fin+'_'+POI_name+'.jpg'

fig = plt.figure()

plt.errorbar(time_series_DSCOVR[:, 0], time_series_DSCOVR[:, 1],\
yerr=None, label = "DSCOVR", c = 'c', ls = '', marker = ".")

plt.errorbar(timeseries_Pandora_DSCOVR[:, 0],\
             timeseries_Pandora_DSCOVR[:, 1],\
             yerr=timeseries_Pandora_DSCOVR[:, 2],\
             label = "Pandora smoothed at DSCOVR times",\
             c = 'm', ls = '', marker = ".")

# Set the range of x-axis
l_lim = 0.
u_lim = ((dt_fin - dt0).total_seconds() + 1.)/86400.
plt.xlim(l_lim, u_lim)

# Set the range of y-axis
#l_lim = 275
#u_lim = 375
#plt.ylim(l_lim, u_lim)

plt.xlabel('GMT, day from beginning of '+datestamp_ini, fontsize=12)
plt.ylabel('total O3 column, DU', fontsize=12)

#plt.legend(loc='upper left')
plt.legend(loc='lower left')

plt.title(plot_title+str(', %08.4fN %08.4fW' %(lat, -lon)))
plt.savefig(img_name, format='jpg', dpi=300)
