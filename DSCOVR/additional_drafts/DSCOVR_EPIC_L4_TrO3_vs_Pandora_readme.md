The code plots three different versions of total ozone column timeseries from 
DSCOVR_EPIC_L4_TrO3 and Pandora data. 
The code allows a user to select a Pandora station of interest and input the desired timeframe. 
DSCOVR EPIC data covering the location of the selected Pandora station and timeframe are 
downloaded on-the-fly by the `earthaccess` library. 
Both Pandora and DSCOVR EPIC data are read to form timeseries. 
The timeseries are first plotted without uncertainties of Pandora retrievals that are added 
in the second version of the plot. This version is usually messy as there are many more 
Pandora retrievals than that from EPIC. 
For this reason, Pandora measurements (along with their uncertainties) are averaged (smoothed) 
to the times of EPIC measurements. This makes two timeseries directly comparable. 
The result is plotted in the third version of the graph.
