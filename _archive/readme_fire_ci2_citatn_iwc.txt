FIRE Cirrus II IFO
UND Citation PMS 2D-C Probe
Ice Water Content Analysis File Format


The following documentation describes the file format for ice water content and 
other parameters derived from data taken by the University of North Dakota Citation 
aircraft PMS 2D-C probe during the FIRE Cirrus IFO - II.  These files were generated 
using software provided by Dr. Andy Heymsfield of NCAR; questions concerning the 
derivation of the computed variables should be directed to him.  A total of nine (9) 
files have been generated, one for each flight except 11/28/91, when the 2D probe 
was inoperative.

The file structure contains 17 records of ASCII header information, followed by ASCII 
data values.  The values were computed for 5-second intervals during times when the 
2D probe was detecting particles.  The file layout is given by the following FORTRAN 
code, which indicates the formats used to write the data files:


Header Information:


** Aircraft Name, Date, Flight Number, Flight Start and End Times
      WRITE (2,1000) AIRCRAFT,IDATE,IFLT,IBOP,IEOP
1000  FORMAT (A8,A12,3I10)

** Probe Resolution 2D-C, 2D-P (not used)
      WRITE (2,1010) A1,CRES,A2,PRES
1010  FORMAT (1X,A30,F6.0,10X,A2,F6.0)

** Header Text
      WRITE (2,1020) ALPHA
1020  FORMAT (A60)

** Area Ratio Bin Endpoints
      WRITE (2,1030) (ARENDS(J),J=1,11)
1030  FORMAT (1X,11F5.2)

** Header Text, Number of Size Bins, Header Text
      WRITE (2,1050) A1,NCHAN(I),ALPHA
1050  FORMAT (A13,I2,A60)

** End Points of the Size Bins
      WRITE (2,1060) (CHENDS(J,I),J=1,NCHAN(I)+1)
1060  FORMAT (10X,20F7.0)

** Concentration Averaging Time
      WRITE (2,1080) A1,NSEC,ALPHA
1080  FORMAT (A29,I10,A60)

** Header Text
      WRITE (2,9002)
9002  FORMAT (' C-PROBE CHANNEL MIDPOINTS (MICRONS):')

** 2D-C Size Bin Midpoints
      WRITE (2,9003) (CHMID(I,1),I=1,NCHAN(1))
9003  FORMAT (19F7.1)

** Header Text
      WRITE (2,9004)
9004  FORMAT (' P-PROBE CHANNEL MIDPOINTS (MICRONS):')
** 2D-P Size Bin Midpoints (not used)
      WRITE (2,9005) (CHMID(I,2),I=1,NCHAN(2))
9005  FORMAT (19F7.1)

** Header Text, Crossover Size (not used)
      WRITE (2,9001) XOVER,NC2
9001  FORMAT (' WHEN COMPOSITE SIZE SPECTRUM GENERATED, C-TO-P-PROBE',
     &        ' CROSSOVER OCCURS AT',F8.1,' MICRONS. THERE ARE',I3,
     &        ' CHANNELS.')

** Minimum Threshold for Accepted Area Ratio
      WRITE (2,9006) MINAR
9006  FORMAT (' PARTICLES BELOW AREA RATIO BIN',I3,' ARE REJECTED.')

** Program or User-Defined Habits
      WRITE (2,9007) NAMHAB(ISHAB)
9007  FORMAT (' HABITS DETERMINED BY: ',A8)

** Header Text
      WRITE (2,9008)
9008  FORMAT (' HABITS: SM SPATIAL, SM COLUMN, LG SPATIAL, LG COLUMN,',
     &        ' SPATIAL W/ EXTENSIONS, BULLET ROSETTE, PLATE')

** Date of Run, Version Number
      WRITE (2,2010) ITODA,JVERS
2010  FORMAT (1X,2A8,5X,A11)

** Column Labels
      WRITE(2,1010)
1010  FORMAT (//5X,'TIMES',7X,'PS    TEMP   ALT    USTAR DBARM  DMAX',
     &        6X,'W1',6X,'W2',6X,'W3',6X,'W4  WTOTAL',5X,'DT8    DT12',
     &        6X,'M1',6X,'M2  DPTC'/
     &        7X,'RH',5X,'IWC    XM1    XM2    XM3  PRECIP   DBZ  VBAR',
     &        ' C-TOTAL  LE 200 200-500 500-800  GT 800     CE8',
     &        '    CE12',6X,'M3',6X,'M4'/
     &       9X,'   TIMP    RHORH       SI       SW',
     &       5X,'LAMBDA      NZERO     RSQ')


Data


      WRITE(2,1000) IT1,IT2,PS,TEMP,ALT,USTAR,DBARM,DMAX,(W(I),I=1,4),
     &             WTOT,DT8,DT12,(TMASS(I),I=1,2),DPTC,RH,
     &             RIWC,(XM(I),I=1,3),R,DBZ,VBAR,TTCONC,
     &             (CBIN(I),I=1,4),CE8,CE12,(TMASS(I),I=3,4),
     &             TIMP,RHORH,SI,SW,LAMBDA,NZERO,RSQ,ICP(KK)
1000  FORMAT (2I7,F6.0,F7.1,F7.0,1PE8.1,0PF6.3,F6.3,9(1PE8.1),0PF6.1/
     &     6X,F4.0,0PF7.4,F7.4,F7.4,F7.4,1PE8.1,0PF6.1,F6.1,5(1PE8.1),
     &       0PF8.2,F8.2,2(1PE8.1)/10X,0PF6.2,3(1PE9.1),2(1PE11.3),
     &       0PF8.4,1X,A2)



The following table describes the output variables and their units:


VARIABLE		DESCRIPTION							UNITS
----------------------------------------------------------------------
IT1,IT2		MEASUREMENT TIME INTERVAL				HH/MM/SS
PS			STATIC PRESSURE						mb
TEMP			AMBIENT TEMPERATURE						degrees C
ALT			PRESSURE ALTITUDE						m
USTAR		VERTICAL VELOCITY NEEDED TO KEEP THE 		cm/s
				RELATIVE HUMIDITY CONSTANT
DBARM		MEDIAN PARTICLE MASS WEIGHTED DIAMETER		cm
DMAX			MAXIMUM PARTICLE DIAMETER				cm
W1			DIFFUSIONAL GROWTH RATE IN CHANNEL 1		g/sec
W2			DIFFUSIONAL GROWTH RATE IN CHANNEL 2		g/sec
W3			DIFFUSIONAL GROWTH RATE IN CHANNEL 3		g/sec
W4			DIFFUSIONAL GROWTH RATE IN CHANNEL 4		g/sec
WTOT			TOTAL DIFFUSTIONAL GROWTH RATE			g/sec
DT8			DEPLETION TIME (8 micron droplets)			sec
DT12			DEPLETION TIME (12 micron droplets)		sec
TMASS1		IWC IN CHANNEL 1						g/m^3
TMASS2		IWC IN CHANNEL 2						g/m^3
DPTC			DEW POINT TEMPERATURE (EG&G)				degrees C
RH			RELATIVE HUMIDITY (EG&G)					%
RIWC			ICE WATER CONTENT						g/m^3
XM1			ICE WATER CONTENT BASED ON SNOW HABIT		g/m^3
XM2			ICE WATER CONTENT BASED ON SMALL SNOW HABIT	g/m^3
XM3			ICE WATER CONTENT BASED ON LARGE SNOW HABIT	g/m^3
R			PRECIPITATION RATE						mm/hr
DBZ			RADAR REFLECTIVITY FACTOR				decibels
VBAR			MEAN REFLECTIVITY WEIGHTED WITH THE		cm/s
				TERMINAL VELOCITY
TTCONC		TOTAL PARTICLE CONCENTRATION				#/L
CBIN1		PARTICLE CONCENTRATION WITHIN THE RANGE LE 200  #/L
CBIN2		PARTICLE CONCENTRATION WITHIN THE RANGE 200-500 #/L
CBIN3		PARTICLE CONCENTRATION WITHIN THE RANGE 500-800 #/L
CBIN4		PARTICLE CONCENTRATION WITHIN THE RANGE GT 800  #/L
CE8			COLLECTION EFFICIENCY (8 micron droplets)	none
CE12			COLLECTION EFFICIENCY (12 micron droplets)	none
TMASS3		IWC IN CHANNEL 3						g/m^3
TMASS4		IWC IN CHANNEL 4						g/m^3
TIMP			# OF CRYSTAL-CRYSTAL COLUMNS				sec^(1-)
RHORH		WATER VAPOR DENSITY						g/cm^3
SI			SUPERSATURATION WITH RESPECT TO ICE		%
SW			SUPERSATURATION WITH RESPECT TO WATER		%
LAMBDA		COEFFICIENTS USED TO FIT THE EQUATION		#/cm^3
NZERO			N=N0*EXP(-LAMBDA*D)					#/L/mm
RSQ			COEFFICIENT OF THE FIT					none
ICP			PROBE TYPE (C OR P)						none
