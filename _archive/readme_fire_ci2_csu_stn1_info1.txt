                
	                 FIRE Cirrus IFO Phase II
	                      CSU Station 1
	                 Parsons KG&E Power Plant
	                     Parsons, Kansas

	Information on instrumentation and the data can be found in the 
	corresponding CSU blue book on the Parsons site.  Instantaneous
	values of the parameters were collected every 2 minutes for the 
        period from 11 Nov 1991 (day 315) to 8 Dec 1991 (day 342).  The
	site was at 37 deg 18 min N and 95 deg 07 min W.

        FILES:  Named station1.nnn, where nnn is the julian day (UTC)

	FORMAT:  Free format with data separated by white spaces.

	VARIABLES:  19 variables with units and estimated precision (in units)
	            time (fraction of julian day, UTC)             0.0001157
	            wind speed (m/s)                               0.5
	            wind direction (degrees)                       5.0
	            temperature (C)                                0.2
	            relative humidity (%)                          5.0
	            solar total irradiance (W/m2)                  5.0
	            near infrared irradiance (W/m2)                5.0
	            infrared irradiance (W/m2)                     5.0
	            dome temperature (K)                           0.2
	            sink temperature (K)                           0.2
	            direct solar - yellow filter (W/m2) [0 sec]    5.0
	            direct solar - red filter (W/m2) [30 sec]      5.0
	            direct solar - dark red filter (W/m2) [60 sec] 5.0
	            direct solar - no filter (W/m2) [90 sec]       5.0
	            direct solar - no filter (W/m2) [0 sec]        5.0
	            direct solar - no filter (W/m2) [30 sec]       5.0
	            direct solar - no filter (W/m2) [60 sec]       5.0
	            direct solar - no filter (W/m2) [90 sec]       5.0
	            hour-minute (hhmm UTC)                         0.16667

	FLAGS:  Missing data marked with -9999.

	INSTRUMENTATION:
                Campbell data logger -- time, hour-minute
                R.M. Young Wind Sensor -- wind speed and direction
                Campbell Model 207 Probe -- temperature and relative humidity
                Eppley Pyranometer -- solar total and near infrared irradiance
                                      (.28-2.8 microns)
                Eppley Pyrgeometer -- infrared irradiance (4-50 microns)),
                                      and dome and sink temperatures
                Eppley Pyrheliometer -- direct solar irradiance 
                                       (no filter -- .28-2.8 microns;
                                        yellow filter -- .53-2.8 microns;
                                        red filter -- .695-2.8 microns;
                                        dark red filter -- 1.0-2.8 microns)

