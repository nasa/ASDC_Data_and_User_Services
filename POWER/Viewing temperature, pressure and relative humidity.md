# The POWER Project

The NASA [Prediction Of Worldwide Energy Resources (POWER) Project](https://power.larc.nasa.gov/) provides solar and 
meteorological data sets from NASA research to support renewable energy, 
building energy efficiency, and agricultural needs. 
The data obtained through the POWER web services is made possible with collaboration from 
the Atmospheric Science Data Center (ASDC). The POWER Knowledgebase binder—a code repository—allows 
a user to obtain all available POWER data via the POWER Zarr Datastore and 
run any provided codes to analyze and visualize the data.

To make regional NetCDF of daily temperature values using the Jupyter Notebook:

1. Navigate to POWER Knowledgebase Repository at https://git.earthdata.nasa.gov/projects/POWER/repos/power-knowledgebase/browse.

2. Click "launch binder" button displayed in README.md.

3. After a few moments, a custom Jupyter Notebook environment will be created.

4. In the pane on the left, navigate to power-knowledgebase > presentations > Visualization and NetCDF Export.ipynb.

5. Open and review the notebook. Ensure the code specifies the correct ZARR filename (cell 2), geography, and time period (cell 4).

6. Click the double arrow `>>` above the notebook to run the notebook in order.

7. Your NetCDF will be saved in the base folder.
