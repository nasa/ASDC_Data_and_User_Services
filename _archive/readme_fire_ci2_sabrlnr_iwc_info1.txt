

                  FIRE SABRELINER AND KING AIR MICROPHYSICAL DATA SETS
                         README File for C-based Read Software


1.0 Introduction

This file contains information about the FIRE_CI2_KINGAIR_IWC (and FIRE_CI2_SABRLNR_IWC) 
data set read software. 
This README file includes the following sections:

	* Section 2.0 - describes the 'C' program (source file)

	* Section 3.0 - describes how to create the executable from the 'C'
                        program.

	* Section 4.0 - demonstrates how to invoke the run-time executable.

	* Section 5.0 - provides general information on the data sets.
	
If users have question while using the fire_ci2_iwc_read.c sample read 
software, please contact the Langley Science User and Data Support (SUDS) 
office.  The SUDS may be reached by E-mail: userserv@eosdis.larc.nasa.gov,
by telephone at (757) 864-8656, or by FAX at (757) 864-8807.  
The SUDS mailing address is:
	
	Langley DAAC Science User and Data Support Office
	NASA Langley Research Center
	Mail Stop 157D
	Hampton, VA 23681-0001


2.0 Read Software Files

The read program was developed in the 'C' language.  This program has been 
tested on the 'Sun4', the 'HP' UNIX-based workstation and a 'SGI'-based system.
There are no library routines that need to be loaded or linked.  A 'makefile'
is included for the convenience of creating the executable file from the source
file. These files and their purposes are listed as follows:

  * makefile_kingair_iwc (makefile_sabrlnr_iwc)- contains a set of commands to 
    build the run-time executable.

  * fire_ci2_iwc_read.c - reads the Sabreliner/Kingair microphysical data sets.


3.0 Creating C Executable Files

There are two ways to build the sample read program from the source 
file.  One is applying the 'make' command to the file makefile_kingair_iwc
(or makfile_sabrlnr_iwc), which defines the 'C' source file listed in Section 2.0.  
The other is using the 'C' compiler 'cc' (or 'acc' for ANSI 'C' compiler).  Section 
3.1 illustrates the first method, and Section 3.2 illustrates the second method.  
The first method is simpler to use than the second one.  The symbol '%' in the
section stands for the UNIX command line prompt.  

3.1 Creating with 'make' command

To build the executable files for the FIRE CI2 Kingair and Sabreliner microphysical data 
sets, at the command line prompt type:

	% make -f (makefile_kingair_iwc) or (makefile_sabrlnr_iwc) read_fire_iwc

3.2 Create with C compiler 'cc'

To use the command line to compile the FIRE CI2 Kingair and Sabreliner read software 
type:

	% cc -o read_fire_iwc fire_ci2_iwc_read.c


4.0 Invoking Read Software

This section illustrates the invocation of the executable file.  Section 4.1 
describes how to run the read program for the FIRE CI2 Kingair and Sabreliner data sets.

4.1 Running the Read Program for the FIRE CI2 Kingair and Sabreliner Data Sets
	
At the command line prompt (%) type 'read_fire_iwc' to run the file 
interactively.


The program will prompt the user for an input (granule) filename. The granule
names are in the form "ci2_kaiwcnn_yymmdd or ci2_sbiwcnn_mmddyy", where nn
is the flight number and yymmdd is the year, month and day.  For example,
ci2_kaiwc15_911206 or ci2_sbiwc16_911206.

Next, you will be prompted for an output file name.
The data file will be read and output sent to the output file name entered.  

When this program is executed, it displays the following banner page with 
information pertaining to the Langley DAAC. 

        *********************************************************************
        *                                                                   *
        *                    Langley DAAC                                   *
        *        PROJECT_LEVEL/DATA_SET_LEVEL READ SOFTWARE                 *
        *  VERSION NUMBER:                                                  *
        *  Language: C                                                      *
        *  Platforms supported:  SUN                                        *
        *                        HP                                         *
        *                        SGI                                        *
        *                        Solaris 2.3                                *
        * Contact:  Langley DAAC user and Data Services Office              *
        *           NASA Langley Research Center                            *
        *           Mail Stop 157D                                          *
        *           Hampton, VA 23681-0001                                  *
        *           PHONE:   (757)864-8656            FAX: (757)864-8807    *
        *                                                                   *
        *********************************************************************
     


5.0 Data Set Information

Data Set Name: FIRE II MICROPHYSICAL DATA FOR THE NCAR SABRELINER AND KING AIR DATA SETS

The microphysical parameters in the data set were derived from 2D probe data collected by 
the NCAR aircraft during FIRE II. THE 2D-C data were converted to size spectra according
to the guidelines given in Heymsfield and Baumgardner, where one element is added to the
size of a particle along the flight direction to account for the probe's intrinsic start-up 
time. Size is determined as the maximum dimension ($D_{max}$) along the flight direction or
optical array axis.  The nominal size resolution for the Sabreliner 2D probe is 50 microns/per
shadowed optical array element, for the Kingair is 25 microns/bin. Sample area (SA) is derived
using the depth of 32 size categories, nonuniformly spaced with higher resolution in the smaller 
classes. Particles within each size bin are subdivided into 10 "area ratio (AR)" bins, where
AR represents the ratio of particle area to the area of discs of diameter $D_{max}$. 

The microphysical parameters in the data set were derived from 2D probe data collected by the 
NCAR Sabreliner during FIRE II.  The derivation of the microphysical parameters is outlined in
the later reference Heymsfield (1977).  The vertical velocity is the "steady-state" velocity in
cm s-1 to keep the relative humidity at it's currently measured value.  Differential growth
rate represents the growth rate of the the particle population of different sizes at the 
current relative humidity.  The Total differential growth rate is the sum of the growth rate
in all channels.  The assumptions used for the IWC calculations are reported in Heymsfield;
also, generic size to mass equations are used.  Precipitation rate is calculated from particle
size and terminal velocity data, integrated over the size spectrum.  Concentration data are
as derived above .  Number of crystal-crystal collisions are derived from the data reported
by Hindman and the crystal terminal velocities.  Water vapor density and supersaturation
information in this data set should not be used--it is unreliable.  Curve fits to the data 
using least squares methods are provided.     

Start Date (mm/dd/yy):  11/09/91
Stop Date  (mm/dd/yy):  12/06/91

Spatial Resolution: 
Averages over 5 sec of aircraft time or approximately 1 km.

Smallest unit of data collected: 5 SECONDS

Data collected at day and night.

Data processing date: 05/11/95, 1/20/97

Data Set Point of Contact:

 NAME: DR. ANDREW J. HEYMSFIELD
 ADDRESS: NATIONAL CENTER FOR ATMOSPHERIC RESEARCH
          P.O. BOX 3000
          BOULDER, CO  80307-3000
 TELEPHONE: (303) 497-8181
 E-MAIL: HEYMS1@NCAR.UCAR.EDU         



                 ******************************************
                 *                                        *
                 *             Sample Output              *
                 *                                        *
                 ******************************************


SABRELINER25  NOV 91         6    173522    185452
 PROBE RESOLUTION (MICRONS): C:   50.          P:  200.
 AREA RATIO BIN ENDPOINTS:                                  
  0.00 0.20 0.30 0.40 0.50 0.60 0.70 0.80 0.90 1.00 2.00
FOR PROBE 1  12 BINS -- END POINTS (IN MICRONS) ARE:                       
              50.   100.   150.   200.   250.   300.   350.   400.   500.   750.  1000.  1500.  5000.
FOR PROBE 2  12 BINS -- END POINTS (IN MICRONS) ARE:                       
             200.   400.   600.   800.  1000.  1200.  1600.  2000.  2400.  2800.  3200.  4000. 10000.
 CONCENTRATIONS AVERAGED OVER         5 SECONDS                                                    
 C-PROBE CHANNEL MIDPOINTS (MICRONS):
   75.0  125.0  175.0  225.0  275.0  325.0  375.0  450.0  625.0  875.0 1250.0 3250.0
 P-PROBE CHANNEL MIDPOINTS (MICRONS):
  300.0  500.0  700.0  900.0 1100.0 1400.0 1800.0 2200.0 2600.0 3000.0 3600.0 7000.0
 WHEN COMPOSITE SIZE SPECTRUM GENERATED, C-TO-P-PROBE CROSSOVER OCCURS AT   750.1 MICRONS. THERE ARE 25 CHANNELS.
 PARTICLES BELOW AREA RATIO BIN  2 ARE REJECTED.
 HABITS DETERMINED BY: PROGRAM.
 HABITS: SM SPATIAL, SM COLUMN, LG SPATIAL, LG COLUMN, SPATIAL W/ EXTENSIONS, BULLET ROSETTE, PLATE
 (AS OF 02/17/94)     VERSION 1.0


     TIMES       PRES  TEMP   ALT       LAT      LON     USTAR  DBARM  DMAX      W1      W2      W3      W4
        WTOTAL   DT8    DT12      M1      M2      M3      M4  DPCRC  RH     IWC    XM1    XM2    XM3
          PRECIP   DBZ  VBAR  C-TOTAL  LE 200 200-500 500-800  GT 800    TIMP
                   RHORH       SI       SW     LAMBDA      NZERO     RSQ     XSAREA

 173525 173529  288.  -49.2  9425.   36.372  -95.591   1.4E+00  0.008  0.022  5.4E-13  1.2E-14  3.3E-15  7.0E-15
      5.6E-13 1.7E+07 1.6E+06 3.1E-03 9.3E-05 4.8E-05 1.5E-04 -53.0  64. 0.0034 0.0013 0.0007 0.0007
           2.2E-03 -31.7  26.6 1.7E+01 1.7E+01 8.9E-02 0.0E+00 0.0E+00 0.0E+00
                  4.3E-08   3.2E-02   6.0E-01  3.581E+02  7.689E+01  0.7245 9.739E-04 C 

 173530 173534  288.  -49.2  9422.   36.367  -95.599   1.2E+01  0.008  0.022  4.8E-12  5.0E-14  1.5E-14  2.9E-15
      4.9E-12 2.4E+06 9.1E+05 1.8E-02 3.2E-04 1.5E-04 4.2E-05 -52.9  65. 0.0188 0.0068 0.0035 0.0039
           1.2E-02 -25.6  19.0 1.0E+02 1.0E+02 2.5E-02 0.0E+00 0.0E+00 1.6E-03
                  4.4E-08   4.8E-02   6.0E-01  5.346E+02  2.057E+03  0.9174 5.667E-03 C 

 ....................................
     .............................
