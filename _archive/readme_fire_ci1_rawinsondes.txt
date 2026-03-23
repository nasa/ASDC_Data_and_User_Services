FIRE Cirrus 1 Rawinsonde Data Set 
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

  firesdf_read.c -  reads the SDF formatted data.

  fire_lib.c     -  a collection of library routines that are used by the other 
                    C source files.

  fire_sdf.h     -  defines constants and data structures used in the read 
                    routines for the FIRE SDF formatted data sets.

  fsdf_header.c  -  parses the FIRE SDF header file and extracts the field 
                    definitions of each actual or implicit variable in the data 
                    description record (DDR) for both ancillary and observation 
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
will read any of the FIRE Cirrus 1 data sets in SDF.  The executable file 'fsdf' 
is the only one needed for the FIRE_CI1_RAWINSONDES data set. 

If you want to build only one executable, at the command line prompt, type

	% make executable_name

where the executable_name can be either fsdf, fci1_srb, or fci1_aircraft. 
 
Makefile.fci1 assumes the library file, 'fire_lib.c', and the header file, 'fire_sdf.h', 
are in the same directory as the other C files.  If any of these C files are not in 
the same directory, the definitions in the makefile will need to be changed to point 
to the right directory. 


4.0  Invoking Read Software

This section illustrates the invocation of the executable file.  Section 4.1 describes 
how to run the FIRE SDF read programs.

4.1 Running FIRE SDF Read Programs

At this time, the executable file for SDF data sets reads one data file at each 
invocation.  The SDF read program requires three files to execute this program 
properly.  In order to process each data file accurately, the header filename 
needs to be specified as the first argument (i.e., datasetname.hdr), the second 
argument is the VTOC filename (i.e., datasetname.toc), and finally the third 
argument is the data filename.   For example, type:

	% fsdf ci1_raobs.hdr ci1_raobs.toc ci1_raob_861013_1102.001 

The executable 'fsdf' will read the binary data files found in each data granule 
and send the output to the standard output device.  The program will then prompt 
you to select which parameters to output to the standard device.  The program can also 
run in a manner so that the output can be sent to a file by typing:

	% fsdf ci1_raobs.hdr ci1_raobs.toc ci1_raob_861013_1102.001 
             num_of_params param1 param2 param3 ... param# > any_file_name

where num_of_params is the number of parameters you would like to output to the 
file, and param1 ... param# are the actual list of parameter numbers to be written 
to the file.


5.0  Data Sets Information

A description of the data files, the variables in the data files, and the contacts 
for data production are given in the header files that accompany the data set.

5.1  FIRE SDF Data Sets

These data sets were written in the FIRE Standard Data Format (SDF).  For a 
complete description of this format, please refer to the "Standard Data Format
for FIRE" written by NASA's Climate Data System (NCDS).  Most data sets did not 
follow this format exactly as stated in this paper; however, these differences 
are noted in the read program.  A header file, a volume table of contents (VTOC) 
file, and an ASCII representation of the first data file are included with
the actual data files.

5.1.1  Rawinsonde Data Files 

FILE NAMING CONVENTION:

            ci1_raobs.hdr        (header file)

            ci1_raobs.toc        (VTOC file)

            ci1_raobs.txt        (ASCII representation of the first data file, .004)

            ci1_raobs_861013     (granule name)

            ci1_raob_861013_1102.001 (first binary data file in the granule)

This data set contains 22 granules.

VARIABLES:

All records and parameters within this data set have been defined, including their 
minimum and maximum values, in the header file, ci1_raobs.hdr.

NOTES ON DATA:

The header file has been modified to reflect the changed record names in each file. 
How ever, the same parameters exist for each record. The reason for changing the record 
names is the external read header file reads a specific record name followed by the
parameters defined within each record and prior to the change, the file did not possess 
this information.

The read program has been modified to compute the WIND SPEEDSHEAR parameter in 
each record. The reason is that if you read the integer value and convert the value 
to a real value, the new result will always be out of the minimum/maximum range as 
defined within the header file.  The data producer recommended a new formula to convert 
the integer value that was read in and converted to a real value, the real value must be 
divided by 2097152.0.  Finally, 400.0 must then be subtracted from this new result to 
get the proper value within the minimum/maximum range.  This formula has been implemented 
in the read program.

The fill value number for this data set is the maximum 4-byte integer word size of 
2147483647.

There are two full records of (112*2) ASCII header information in each data record file.

There were no problems with values being out of the minimum/maximum range.

INSTRUMENTATION:

This information is provided within the header file for this data set.

CONTACTS FOR DATA PRODUCTION INFORMATION:

Dr. David O'C. Starr
Code 913.0
NASA Goddard Space Flight Center
Greenbelt, MD  20771 
Phone:  (301) 286-9129 
e-mail: starr@climate.gsfc.nasa.gov


6.0  IMPLEMENTATION NOTES 

6.1  FIRE SDF program

The fsdf_header.c program builds the data structure for the variables defined 
in the Variable Description (VD) record.  When the variable values are to be 
displayed as octal, or hexadecimal format, the scaling constant `b' of the 
conversion function defined in the VD record is stored as 0 for the convenience 
of coding (the header file is not changed).  It is assumed these variables 
require no conversion from integer to floating point values. 

FIRE SDF rules recommend that the bad or missing data to be represented by the 
largest possible integer value for each data type, but some data sets use different 
values as the fill value.  This FIRE Cirrus 1 SDF sample read program knows what these 
fill values are and they are identified in the code.

This standard read program is capable of reading a SDF file that is available at the 
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
