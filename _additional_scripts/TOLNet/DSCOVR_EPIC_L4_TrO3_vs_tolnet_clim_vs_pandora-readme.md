Python code `DSCOVR_EPIC_L4_TrO3_vs_tolnet_clim_vs_pandora.py` compares total and tropospheric ozone
columns integrated from ozone profiles retrieved by TOLNet lidar against total columns retrieved by
Pandora and DSCOVR EPIC spectrometer and tropospheric column retrieved by EPIC.
The code works only with clim TOLNet data.
These data files are part of TOLNet_JPL_Data_1 data collection and have  their granule names
beginning with "groundbased_lidar.o3_nasa.jpl003_clim_table.mountain.ca".
They can be downloaded from https://asdc.larc.nasa.gov/data/TOLNet/JPL_Data_1/ or from the EarthData,
see https://search.earthdata.nasa.gov/search/granules?p=C2565841261-LARC_ASDC&pg[0][v]=f&pg[0][id]=groundbased_lidar.o3_nasa.jpl003_clim_table.mountain.ca*&pg[0][gsk]=-start_date&q=TOLNet_JPL_Data_1&tl=1726081915.72!3!!].

The code is driven by the list of TOLNet granules provided by the user as the code command line argument.
Once a TOLNet file has been read, the code interactively requests the name of the Pandora station.
Since currently TOLNet clim data are  only available at JPL Table Mountain location,
the user is supposed to select "WrightwoodCA" as the right Pandora station name.
If more TOLNet location provide this type of data in the future,
it is the user's responsibility to choose suitable Pandora station.
Pandora ozone retrievals file is automatically downloaded and read for the day of TOLNet retrievals.

The code automatically downloads DSCOVR_EPIC_L4_TrO3_01 granules for the day of TOLNet retrievals. 
Gridded total and tropospheric columns are interpolated to the location of the TOLNet instrument.

The data are plotted in two graphs for each TOLNet granule. Since Pandora measurements and
retrievals are significantly more frequent than that of DSCOVR and TOLNet,
they are shown as a solid line.
TOLNet retrievals are results of temporal integration within an approximately 2-hour time period, 
for this reason, TOLNet retrievals are presented as flat dash line. DSCOVR EPIC columns are presented by stars.

Bear in mind that either Pandora or DSCOVR EPIC retrievals (or both)
may not be available for a particular TOLNet granule. This does not prevent the code from
preventing plotting available results.

Important notice on automatic downloads. The routines performing checks and downloads of
DSCOVR EPIC data require an up-to-date EarthData user token. To obtain one,
a user needs to register in the EarthData, at [http://urs.earthdata.nasa.gov/].
Once registered and logged in, click "generate token" option at the top of the user profile page.
