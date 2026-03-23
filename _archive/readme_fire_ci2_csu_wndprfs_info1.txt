WIND PROFILER

COLORADO STATE UNIVERSITY
ATMOSPHERIC SCIENCE DEPARTMENT
FIRE II EXPERIMENT (CIRRUS IFO)

The wind profiler data in fire2wind.tar.Z are single cycle data from 
FIRE II Cirrus IFO, Parsons, KS. 

FILENAMES:  YYMMDDHH.wp where YY is the year, MM the month, DD the day and
            HH the hour the data were collected.

FORMAT:  Header (4I2)             year, month, day, hour(UTC)
         Data   (I2,F7.3,2F10.3)  minute, height(km above MSL), u(m/s), v(m/s)

BAD DATA FLAG: -9999. in u and v

This data has been though a quality control program using the Weber and 
Wuertz QC subroutines.  Thresholds of 4m/s and 5% size of largest pattern 
was used on combined-low/high-mode-opposing-beams profile.  Note vertical 
velocity was assumed to be zero. (28% of the dataset was marked as "bad".)

