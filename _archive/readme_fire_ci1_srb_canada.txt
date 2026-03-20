FIRE Cirrus 1 Surface Radiation Budget (SRB) Canada Data Set 
Read Software Readme File


1.0  Introduction

This data set is in the FIRE Standard Data Format (SDF).   This readme
file contains information about how to use the FIRE SDF sample read 
software.  This readme file includes the following five sections:

	Section 2.0 - describes all sample C programs (source files).

	Section 3.0 - discusses how to create the executables from the C program(s).

	Section 4.0 - demonstrates how to invoke the run-time executable(s).

	Section 5.0 - provides general information on the data set.

	Section 6.0 - provides more detailed implementation notes.

If you have questions while using any Langley DAAC sample read software, 
please contact the Langley DAAC Science, User and Data Support (SUDS) office. 
The SUDS may be reached by e-mail: userserv@eosdis.larc.nasa.gov, by telephone 
at (757)864-8656, or by FAX at (757)864-8807.  The SUDS mailing address is:

Langley DAAC Science, User and Data Support Office
NASA Langley Research Center
Mail Stop 157D
Hampton, VA 23681-0001


2.0  Read Software Files

All sample read programs are developed in the C-language.  These C programs have 
been tested on the `Sun4 (OS 4.1.3)' and `HP' UNIX-based workstations.  All library 
functions in the C source codes are tested for ANSI C conformance.  A `Makefile' is 
included for the convenience of creating the executable files from the C source 
files.  Five read software files are used to generate the FIRE SDF executable. 
These files and their purposes are listed as follows:

  Makefile.fci1  -  contains a set of commands to build the run-time executables.

  fci1srb_read.c -  reads the modified SDF formatted data.

  fire_lib.c     -  a collection of library routines that are used by the other 
                    C source files.

  fire_sdf.h     -  defines constants and data structures used in the read 
                    routines for the FIRE SDF formatted data sets.

  fsdf_header.c  -  parses the FIRE SDF header file and extracts the field 
                    definitions of each actual or implicit variable in the data 
                    description record for both ancillary and observation 
                    segments.

  fsdf_vtoc.c    -  reads a record of FIRE SDF Volume Table Of Contents (VTOC).


3.0  Creating C Executable Files

To build the sample read program from the C source files apply the `make' command 
to the make file (which defines the C source files listed in Section 2.0.)  
The symbol `%' in this section stands for the UNIX command line prompt.

3.1  Create with `make' command

To build all of the FIRE SDF executable files, rename the makefile to be 
either `Makefile' or `makefile'.  At the command line prompt, type

	% make all

If the make file has a different name, e.g., Makefile.fci1, type 

	% make -f Makefile.fci1 all

Three executable files (fsdf, fci1_srb, and fci1_aircraft) are created which
will read any of the FIRE Cirrus 1 data sets in SDF.  The executable file 'fci1_srb' 
is the only one needed for the FIRE_CI1_SRB_CANADA data set. 

If you want to build only one executable, at the command line prompt, type

	% make executable_name

where the executable_name can be either fsdf, fci1_srb, or fci1_aircraft. 
 
Makefile.fci1 assumes the library file, 'fire_lib.c', and the header file, 'fire_sdf.h', 
are in the same directory as the other C files.  If any of these C files are not in 
the same directory, the definitions in the makefile will need to be changed to point 
to the right directory. 


4.0  Invoking Read Software

This section illustrates the invocation of the executable file.  Section 4.1 describes 
how to run the FIRE SDF SRB read programs.

4.1 Running FIRE SDF Read Programs

At this time, the executable file for SDF data sets reads one data file at each 
invocation.  The SDF read program requires three files to execute this program 
properly.  In order to process each data file accurately, the header filename 
needs to be specified as the first argument (i.e., datasetname.hdr), the second 
argument is the VTOC filename (i.e., datasetname.toc), the third argument is
the data filename (i.e., datasetname.filenumber), the fourth argument is 
the ancillary data file (datasetname.001 or datasetname.002).  There are two
ancillary data files for this data set.  Only one is used when invoking the software.
   
For example, type:

% fci1_srb ci1_srb_ca.hdr ci1_srb_ca.toc ci1_srb_ca_861016_31.104 ci1_srb_ca_861016_31.001

The executable 'fci1_srb' will read the binary data files found in each data granule 
and create an output file called datasetname.filenumber.OBS, i.e., ci1_srb_ca_861016_31.104.OBS.  


5.0  Data Sets Information

A description of the data files, the variables in the data files, and the contacts 
for data production are given in the header files that accompany the data set.

5.1  FIRE SDF SRB Data Sets

These data sets were written in a modified FIRE Standard Data Format (SDF).  For a 
complete description of the FIRE SDF, please refer to the "Standard Data Format
for FIRE" written by NASA's Climate Data System (NCDS).  A header file, a volume table 
of contents (VTOC) file, an ASCII representation of the first data file and one or
two ancillary data files are included with each FIRE SDF SRB data file.  For the SRB data 
sets, the data description record section of the header file was modified to include a
record prefix subsection. 

5.1.1  SRB Canada Data Files 

FILE NAMING CONVENTION:

ci1_srb_ca.hdr              Header file, in CHARACTER format (ASCII).
                            Physical record length is 6400 bytes.
                            Logical record length is 80 bytes.

ci1_srb_ca.toc              Volume Table of Contents (VTOC) in ASCII.
                            Physical record length is 6400 bytes.
                            Logical record length is 160 bytes.
                            Brief description of files, beginning with 
                            the ancillary data file(s), the fourth (and 
                            fifth) physical file(s). 

ci1_srb_ca_861001_16.txt    Test Data in ASCII.  There are 2 test data files each
ci1_srb_ca_861016_31.txt    for a different time period.  Physical record length is 
                            6400 bytes.  Logical record length is 80 bytes.  Each 
                            logical record is an expanded "pixel packet", consisting 
                            of RSIZ bytes of the original file per "Test Data" logical 
                            record.  Note that this file represents the complete contents 
                            of the first "observation data" file, NOT of the first "data"  
                            file (the ancillary file).
            
ci1_srb_ca_861001_16.001    There are 4 ancillary files for this data set.  Ancillary 
ci1_srb_ca_861001_16.002    File (Lat/Lon grid) in scaled INTEGER*2 format.  Record prefix 
ci1_srb_ca_861016_31.001    length is RPFX bytes.  Logical record length is RSIZ bytes.  
ci1_srb_ca_861016_31.002    There are NREC logical records, or "pixels", per physical 
                            record.  Physical record length is thus (RPFX + NREC*RSIZ) bytes.
                            Within each logical record ("pixel") of the DATA area, bytes 
                            1 and 2 contain a LATITUDE in I*2 format, bytes 3 and 4 contain 
                            a LONGITUDE in I*2 format; currently, bytes 5 through RSIZ of 
                            each logical record are unused, and are reserved for possible  
                            future use.  The LAT/LON quantities are to be used as a lookup 
                            table to convert any (pixel, scanline) coordinate pair in each 
                            of the data files into a pair of LAT/LON coordinates.  First 
                            physical record is header record, in ASCII.
 
ci1_srb_ca_860930           Granule name.  Organizational strucuture of data from DAAC.
                            This data set contains 32 granules.

ci1_srb_ca_861001_1016.003  Files no. X through last - Where "X" = (1 + no. of 
                            ancillary files).  Observation Data Files in INTEGER*1 
                            format.  Record prefix length is RPFX bytes.  Logical 
                            record length is RSIZ bytes.  There are NREC logical 
                            records, or "pixels", per physical record.  Physical 
                            record length is thus (RPFX + NREC*RSIZ) bytes.  Contains 
                            various flags and physical quantities in coded format, 
                            obtained from ISCCP analysis of B3 radiance data.  First 
                            physical record is header record, in ASCII.
 
NOTES ON DATA:

Each physical record is composed of a "record prefix area" and
"record data area".  The record prefix area contains the following
information:

     FNUM (I*4) - Data file number on tape.
     RNUM (I*4) - Data record sequence number within the file.
     CKSM (I*4) - Checksum (simply the sum of all byte values within
                  the DATA area that follows PREFIX area).
     RSIZ (I*4) - The size, in bytes, of each logical record in the
                  DATA area.
     NREC (I*4) - The number of logical records in the DATA area.
     RPFX (I*4) - The size, in bytes, of the PREFIX area.

VARIABLES:

Each of the observation data files contain 24 variables. Each variable has been 
defined as a one byte unsigned integer.  Two variables (Lat/Lon) are stored in 
each ancillary data file in INTEGER*2 format.  In order to scale the data so they 
are 1-byte, 2-bytes, or 4-bytes positive integers the following equation is used:

Q = (R - A)  *  (2**(b - N)) 

where R is the actual (real) data value, b=7 for 1 byte integers, b=15 for 2 byte 
integers, and b=31 for 4 byte integers and Q is rounded to a positive integer.  All 
records and parameters within each record have been defined including their minimum 
and maximum values in the header file filename.hdr

NOTES ON DATA:

Questions about instrumentation and specific data parameters (including their derivation 
utilization and units) should be directed to Goddard Institute of Space Studies (GISS). 
There are small discrepancies pertaining to the SRB data set.  Some of the values from 
data files and ancillary files were out of range when compared against the ranges provided 
by the VTOC, and the maximum and minimum values from the Header files.  The data producers 
response to these discrepancies was "What is in VTOC is the definition of the region, what 
is in the Ancillary files is a subset of scan lines falling in that region. A given scan 
line may have some pixels with lat/lon outside the region."

INSTRUMENTATION:

This information is provided within the header file for this data set.

CONTACTS FOR DATA PRODUCTION INFORMATION:

Dr. William B. Rossow
NASA Goddard Institute for Space Studies
2880 Broadway
New York, NY 10025 USA
Phone: (212) 678-5567
Phone: (212) 678-5552 FAX
E-mail: clwbr@giss.nasa.gov  


6.0  IMPLEMENTATION NOTES 

6.1  FIRE SDF SRB program

This read program is capable of reading a modified SDF file that is available at the 
Langley DAAC.  Because of the modifications made in this read program to handle the SDF 
data files, this program could not read any other SDF files not from the Langley DAAC. 

APPENDIX

1.0  Output from running `make all' on HP workstation

HP% make -f Makefile.fci1 all

	cc -O -Aa  -I.   -c  fire_lib.c

	rm -f ./lib_fire.a

	ar rlv ./lib_fire.a `lorder fire_lib.o | tsort`

a - fire_lib.o

ar: creating ./lib_fire.a

	ranlib ./lib_fire.a

ranlib: ar already did it for you, see ar(1).

	rm -f fci1_aircraft

	cc -O -Aa  -I.    -o fci1_aircraft genpro.c ./lib_fire.a

	cc -O -Aa  -I.   -c  firesdf_read.c

	cc -O -Aa  -I.   -c  fsdf_header.c

	cc -O -Aa  -I.   -c  fsdf_vtoc.c

	rm -f fsdf

	cc -O -Aa  -I.    -o fsdf firesdf_read.o  fsdf_header.o  fsdf_vtoc.o ./lib_fire.a -lm

	rm -f fci1_srb

	cc -O -Aa  -I.    -DSRB -o fci1_srb fci1srb_read.c  fsdf_header.c  fsdf_vtoc.c ./lib_fire.a -
lm

fci1srb_read.c:

fsdf_header.c:

fsdf_vtoc.c:


2.0  Output from running `make all' on Sun workstation

SUNOS% make -f Makefile.fci1 all

acc -O   -I.  -sun4 -c  fire_lib.c

rm -f lib_fire.a

ar rlv lib_fire.a `lorder fire_lib.o | tsort`

a - fire_lib.o

ar: creating lib_fire.a

ranlib lib_fire.a

rm -f fci1_aircraft

acc -O   -I.   -sun4 -o fci1_aircraft genpro.c ./lib_fire.a

acc -O   -I.  -sun4 -c  firesdf_read.c

acc -O   -I.  -sun4 -c  fsdf_header.c

acc -O   -I.  -sun4 -c  fsdf_vtoc.c

rm -f fsdf

acc -O   -I.   -sun4 -o fsdf firesdf_read.o  fsdf_header.o  fsdf_vtoc.o ./lib_fire.a /usr/lib/
libm.a

rm -f fci1_srb

acc -O   -I.   -sun4 -DSRB -o fci1_srb fci1srb_read.c  fsdf_header.c  fsdf_vtoc.c ./lib_fire.a 
/usr/lib/libm.a

fci1srb_read.c:

fsdf_header.c:

fsdf_vtoc.c:

Linking:
