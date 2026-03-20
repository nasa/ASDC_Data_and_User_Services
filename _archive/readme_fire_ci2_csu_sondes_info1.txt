
                         FIRE Cirrus IFO Phase II
                           CSU Rawindsonde Data
                         Parsons KG&E Power Plant
                             Parsons, Kansas

        Information on instrumentation and the data can be found in the
        corresponding CSU blue book on the Parsons site.  Sondes were  
        launched primarily in support of surface radiation measurements
        for the period from 13 Nov 1991 (day 317) to 6 Dec 1991 (day 340).
        The site was at 37 deg 18 min N and 95 deg 07 min W.

        FILES: There are 45 data files in fire2sonde.tar.Z.  They are
               named yymmdd.hhmms where yy is the year, mm is the month,
               dd is the day, hh is the hour, and mm is the minute of
               the launch time.

        FORMAT: (3I3,F6.2,6F10.3)

        VARIABLES: 10 variables (in order)
                   julian day (UTC)
                   hour (UTC)
                   minute (UTC)
                   second (UTC)
                   geopotential height (km)
                   pressure (mb)
                   temperature (K)
                   relative humidity (%)
                   azimuth angle (deg)
                   elevation angle (deg)

         FLAGS:  Data with values of -9999 is considered bad.

         WARNING:  Because of problems tracking the sonde, the angles
                   of the tracking antenna should be used with caution.
                   They are provided to allow a calculation of an 
                   appoximate location of the sonde. 

          REFERENCE: Cox, S., G. Beck, C. Cornwall, J. Davis, P. Hein,
                     C. Lappen, R. Song, J. Withrow, and D. Wood, 1992:
                     "CSU FIRE II Cirrus Field Experiment:  Description
                     of Field Deployment Phase."  Atmospheric Science
                     Report #506, 79pp.

