Codes `epic_l2_aer_timeseries_web.py`, `epic_l2_aer_timeseries.py`, `epic_l2_to3_timeseries_web.py`, 
and `epic_l2_to3_timeseries.py`
read DSCOVR EPIC L2 aerosol and ozone products, re-grid the AOD and total ozone column to a regular grid,
and write timeseries of these variables at the positions of interest (POIs) to output files.
The user must create a file named `POI_list` containing POI(s) inforamtion.
An example of `POI_list` is provided.

'_web' versions take a month of interest as a command line argument (in the form of a string `'yyyymm'`),
check the ASDC direct download site for all available data files, download, and process them.
The files are then deleted;
however, the user can comment the delete statement out (see comments in the codes).
To be able to find and download data files,
the user must have an account at https://urs.earthdata.nasa.gov/
and use that account to create an access token and update variable token (see comments in the codes).

Versions without '_web' are driven by a user-created list of data files that user must have on a local disk.
The list is supplied to the codes as a command line argument.
