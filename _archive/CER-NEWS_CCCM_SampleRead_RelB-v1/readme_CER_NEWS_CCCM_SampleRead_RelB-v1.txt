                   CERES CCCM RelB-v1 Readme File

1.0  Introduction
     This readme file document provides information on the
     NASA Energy and Water cycle Study (NEWS) and 
     Clouds and Earth's Radiant Energy System (CERES)
     CCCM data sets:

     Aqua:
       CER-NEWS_CCCM_Aqua-FM3-MODIS-CAL-CS_RelB1
	   
     This document includes the following sections:

        Section 2.0 - Data Set Description
        Section 3.0 - Format and Packaging
        Section 4.0 - Science Parameters Information
        Section 5.0 - Description of Sample Read Software
        Section 6.0 - Implementing Sample Read Software
        Section 7.0 - Sample Output
        Section 8.0 - Additional Data Information (This section is optional)

     If users have questions, please contact the Langley ASDC Science,
     Users and Data Services Office at:

         Atmospheric Science Data Center
         User and Data Services Office
         Mail Stop 157D
         2 South Wright Street
         NASA Langley Research Center
         Hampton, Virginia 23681-2199
         U.S.A.

         E-mail:   larc@eos.nasa.gov
         Phone:    (757)864-8656
         FAX:      (757)864-8807
         URL:      http://eosweb.larc.nasa.gov


 2.0 Data Set Description

    2.1 Instrumentation Description

        For a complete description of NEWS 
        please refer to the documentation provided at the URL
	http://nasa-news.org	
	and the CERES experiment and instruments,
        http://asd-www.larc.nasa.gov/ceres/ASDceres.html.
	
	CALIPSO
	http://www-calipso.larc.nasa.gov
	
	ClodSat
	http://cloudsat.atmos.colostate.edu/

    2.2 Data Quality

        For a discussion regarding the quality of the
        CCCM data sets, please refer to the
        corresponding Quality Summary Document available from the URL
        http://eosweb.larc.nasa.gov/project/ceres/table_ceres.html.

    2.3 Other pertinent information

        The CERES CCCM product is produced from software maintained
        by the Langley Atmospheric Data Fusion Team.

    2.4 Science Representatives
        *NOTE TO USER:
         If you have any questions concerning the sample code or 
         the data, please contact the Atmospheric Sciences Data
         Center User Services Office. The contact information is 
         provided above in Section 1.0.

         1. Dr. Seiji Kato, CCCM Principal Investigator
    	    NASA Langley Research Center
            Mail Stop 420
            Hampton, Virginia 23681-2199
            E-mail:   Seiji.Kato@nasa.gov
            FAX:      (757)864-7996
            Phone:    (757)864-7062



 3.0 Format and Packaging

     This package includes a program, written in C, which will read 
     an CCCM HDF file.  The examples provided in the main (CCCM_readhdf.c) 
     program are written specifically for SDS(s) and Vdata(s) in CCCM files.
     This read program is provided as a template to the user who will need 
     to modify it, if they need to read different CCCM parameters.

     Information on the structure of the CCCM file can be found
     in the Data Products Catalog (DPC) pages included in this 
     package. Complete parameter descriptions are available in 
     the Collection/User's Guide, which 
     is on-line at URL: http://asd-www.larc.nasa.gov/ceres/collect_guide.

    3.1 Sample Read Package Contents

        This package consists of the following files:

        1) CER-NEWS_CCCM_Sample_RelB1.hdf
        2) CER-NEWS_CCCM_Sample_RelB1.hdf.dump
        3) CER-NEWS_CCCM_Sample_RelB1.hdf.met
        4) DPC_SSF_CRS_C3M_RelB1_V1.pdf
        5) CCCM_HDFread.h
        6) CCCM_readHDFfuncs.c
        7) CCCM_readhdf.c
        8) compile_CCCM_readhdf
 
        Descriptions for each file is as follows:
        1)'CER-NEWS_CCCM_Sample_RelB1.hdf' is a sample 5-footprint CCCM file 
          which is used by the examples in 'CCCM_readhdf.c'

        2)'CER-NEWS_CCCM_Sample_RelB1.hdf.dump' is an ASCII file containing a 
          description of the data in the sample CCCM, 'CER-NEWS_CCCM_Sample_RelB1.hdf'.
          If a value in the CCCM granule is out-of-range or not valid, a 
          default value is inserted in its place.  Default values depend 
          on data type.  CERES default values are greater than or equal to:
          INT2    - 32767
          INT4    - 2147483647
          REAL4   - 3.402823E+38
          REAL8   - 1.797693134862315E+308

          The 'CER-NEWS_CCCM_Sample_RelB1.hdf.dump' file includes a list of ALL the 
          data on the 5 footprint sample CCCM (419 parameters/SDS's plus header 
          Vdata). The output data are the same as what the user would produce 
          using hdp, but for this example, the CERES Team modified the format
          of the output from the hdp command to make it easier to read.

        3) CER-NEWS_CCCM_Sample_RelB1.hdf.met is an ASCII metadata file that is 
          delivered with every data granule.  The file is written in 
          Object Descriptive Language (ODL) and contains information that 
          is associated with the origin, content, quality, and condition 
          of the data granule.  The metadata information contained in 
          the granule metadata file is also included as metadata on the 
          CCCM HDF data file.  An example of a metadata parameter found 
          in the metadata file and on the CCCM HDF file is LOCALGRANULEID, 
          which has a value set to the original granule file name.

        4)'DPC_SSF_CRS_C3M_RelB1_V1.pdf' is a copy of the CERES Data Product Catalog 
          pages which describes the CCCM product.

        5)'CCCM_HDFread.h' is a header file for 'CCCM_readhdf.c' 
          and the CCCM HDF read functions.

        6)'CCCM_readHDFfuncs.c' is a file consisting of C functions that
          are linked with 'CCCM_readhdf.c' and call the HDF functions.

        7)'CCCM_readhdf.c' is the main program which accesses the HDF reading
          functions.  It consists of six examples of accessing SDS and
          Vdata structures from the included sample CCCM file.  Comments are 
          provided throughout the program.  This file must be modified if the
          user wishes to read SDSs or Vdatas other than those in the examples.

        8)'compile_CCCM_readhdf' is a sample script to compile the C programs
          in a UNIX environment. The script must be modified for different
          platforms to properly compile the programs and correctly link
          the HDF libraries.

    3.2 Zip Information

        To read the files, first unzip them with Info-Zip software.
        Information about downloading and using this freeware is
        available at the URL:
        http://www.info-zip.org/pub/infozip/Info-ZIP.html

    3.3 HDF library

        HDF information, documentation, tutorials, libraries, etc.
        can be found on-line at the URL  http://hdf.ncsa.uiuc.edu.

        The utility hdp, provided in the HDF libraries, extracts
        information regarding the HDF file contents, such as the
        size, type, order, names, and the data itself.  Some
        examples are:

          hdp -H      - provides "help" on how to use the "hdp" utility
          hdp dumpsds [-options] <HDF file name> - lists information about
                      the SDSs found on the specified file
          hdp dumpvd  [-options] <HDF file name> - lists information about
                      the Vdatas found on the specified file

    3.4 Data Format

        The CCCM data files are written in HDF as collections of SDS(s) and 
        Vdata(s).

    3.5 Breakdown of the File Naming Convention

        The CCCM data granules adhere to the following CERES file
        naming convention:

                  CER-NEWS_PID_SS_PS_CC.DataDate

        where: CER = the CERES experiment

               PID = the NEWS product, e.g., CCCM

                SS = the NEWS sampling strategy, e.g.,
                     Aqua-FM3-MODIS-CAL-CS, as defined by the NEWS experiment.
                     For the CCCM, this indicates the satellite and
                     instruments that were the source of the input
                     data into the software system that produced
                     the CCCM data.

                PS = the NEWS production strategy, e.g.,
                     'RelBN', as defined by the NEWS experiment,
                     production strategies indicate versions
                     of a research product that may be under going 
					 improvements, but that contain validated data with which 
                     the user can publish.

                CC = the product configuration code,
                     e.g., '017018', assigned by the NEWS experiment.
                     This code is internal to the NEWS project, and
                     is updated whenever the software system producing
                     the product is modified for any reason. Multiple 
                     files that are identical in all aspects of the 
                     filename except for the 6 digit configuration code
                     differ little, if any, scientifically. Users may, 
                     therefore, analyze data from the same sampling
                     strategy and production strategy without regard 
                     to configuration code. For a complete history
                     of the changes to the software, see the CCCM
                     product Description/Abstract document available
                     from the URL
                     http://eosweb.larc.nasa.gov/PRODOCS/ceres/table_ceres.html

          DataDate = the date of the data included in the
                     file.  For the CCCM, this date is in the format

                                 YYYYMMDD

                     where: YYYY = the four-digit data year,
                              MM = the two-digit month
                              DD = the two-digit day


 4.0 Science Parameters Information

     Refer to the CCCM Data Products Catalog pages included
     with this package (listed under Section 3.1) for the following
     information for each science parameter:

           Name
           Units
           Type  (integer, real, character, etc...)
           Ranges (minimum and maximum values)
           Fill values / bad data flags


 5.0 Description of Sample Read Software

     The sample read software in this package is intended only
     as an example.  As both the number of SDSs and Vdatas that 
     make up the CCCM product and the number of data records contained
     on an actual CCCM file are large, the sample software only 
     demonstrates reading a limited number of SDSs for the five
     footprints included in the sample CCCM file.  The contents of
     the SSF footprint, along with a list of all the SDS(s) contained
     on the CCCM product, are described in the Data Products Catalog
     pages discussed in Section 3.1.

     A note to the user:
     
     It is recommended that before modifiying the CCCM read program to 
     read different CCCM parameters, the user should first compile and 
     run the CCCM read program using the sample CCCM file as input and 
     compare the results that are created by the read program for 
     each SDS/parameter with the list of values for corresponding 
     parameters in the CCCM sample dump file,'CER-NEWS_CCCM_Sample_RelB1.hdf.dump'.
     If this test is successful, then the user should modify the 
     the CCCM read program or their own software to read the parameters 
     in which they are interested and run that software using the Sample 
     CCCM as input. The values read should be compared to the values 
     of the corresponding parameters in the sample dump file.  If both 
     of these test are successful, then the user will be ready to use 
     their altered program with a full size CCCM data file.


 6.0 Implementing Sample Read Software

    6.1 Compiling the Sample Read Software

       1. Download and install the HDF libraries from the NCSA web site

                  http://hdf.ncsa.uiuc.edu

         This code uses version HDF4.1r5.  Other versions of the HDF
         libraries may not work with this code. NCSA uses the GNU gzip
         compression utility for some packages. If necessary, the
         software needed to uncompress the file can be obtained from
         one of the GNU mirror sites listed at the URL:

                http://www.gnu.org/order/ftp.html

       2. Edit the sample script 'compile_CCCM_readhdf' to link the
          HDF libraries and compile the C programs.  Check the permissions
          on this file to make sure execute permission is set for the owner
          by typing at the command line (denoted by ">"):

           > ls -l compile_CCCM_readhdf

          A line similar to the following will be displayed:

          -rwxr--r--  1 OWNER  GROUP 1450 Mar 6 14:49 compile_CCCM_readhdf

          where OWNER and GROUP are the file owner and group information.
          The owner executable permission is the x in -rwxr--r--.

          To set the proper permissions, at the command line type:

           > chmod 744 compile_CCCM_readhdf

       3. Execute the script 'compile_CCCM_readhdf' to create the
          executable cccm_rd by typing at the command line:

           > source compile_CCCM_readhdf

          or, on some systems, type:

           > compile_CCCM_readhdf

    6.2 How to Run the Sample Read Software

        Execute cccm_rd by typing at the command line:

             > cccm_rd CER-NEWS_CCCM_Sample_RelB1.hdf


 7.0 Sample Output


The following is the screen output generated from executing the ssf_rd
program with the provided sample file.


> ./cccm_rd CER-NEWS_CCCM_Sample_RelB1.hdf


Name of HDF file to be read:
< CER-NEWS_CCCM_Sample_RelB1.hdf



*********************************************************
*    Example 1:                                         *
*********************************************************

Footprint 1

X component of satellite inertial velocity = 7.359795

Footprint 2

X component of satellite inertial velocity = 7.353749

Footprint 3

X component of satellite inertial velocity = 5.214400

Footprint 4

X component of satellite inertial velocity = 4.025348

Footprint 5

X component of satellite inertial velocity = 3.264651


*********************************************************
*    Example 2:                                         *
*********************************************************

Footprint 1

Number of imager pixels in CERES FOV(1) = 1186
Number of imager pixels in CERES FOV(2) = 33

Footprint 2

Number of imager pixels in CERES FOV(1) = 1197
Number of imager pixels in CERES FOV(2) = 33

Footprint 3

Number of imager pixels in CERES FOV(1) = 1208
Number of imager pixels in CERES FOV(2) = 33

Footprint 4

Number of imager pixels in CERES FOV(1) = 1242
Number of imager pixels in CERES FOV(2) = 34

Footprint 5

Number of imager pixels in CERES FOV(1) = 1198
Number of imager pixels in CERES FOV(2) = 33


*********************************************************
*    Example 3:                                         *
*********************************************************

Footprint 1

Cloud layer top level height 
Cloud Group 1 Layer 1 cloud top height = 7.867701
Cloud Group 1 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 1 cloud top height = 7.447997
Cloud Group 2 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 1 cloud top height = 7.867701
Cloud Group 3 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 1 cloud top height = 8.767076
Cloud Group 4 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 1 cloud top height = 6.848420
Cloud Group 5 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 1 cloud top height = 7.447997
Cloud Group 6 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000

Footprint 2

Cloud layer top level height 
Cloud Group 1 Layer 1 cloud top height = 8.647161
Cloud Group 1 Layer 2 cloud top height = 6.938357
Cloud Group 1 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 1 cloud top height = 8.647161
Cloud Group 2 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 1 cloud top height = 8.647161
Cloud Group 3 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 1 cloud top height = 8.647161
Cloud Group 4 Layer 2 cloud top height = 7.268124
Cloud Group 4 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 1 cloud top height = 8.407330
Cloud Group 5 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 1 cloud top height = 7.118230
Cloud Group 6 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 1 cloud top height = 8.137510
Cloud Group 7 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000

Footprint 3

Cloud layer top level height 
Cloud Group 1 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000

Footprint 4

Cloud layer top level height 
Cloud Group 1 Layer 1 cloud top height = 6.518653
Cloud Group 1 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 1 cloud top height = 6.398737
Cloud Group 2 Layer 2 cloud top height = 1.392272
Cloud Group 2 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 1 cloud top height = 6.728505
Cloud Group 3 Layer 2 cloud top height = 1.392272
Cloud Group 3 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 1 cloud top height = 6.098949
Cloud Group 4 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 1 cloud top height = 6.098949
Cloud Group 5 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 1 cloud top height = 6.098949
Cloud Group 6 Layer 2 cloud top height = 1.392272
Cloud Group 6 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 1 cloud top height = 6.728505
Cloud Group 7 Layer 2 cloud top height = 4.929775
Cloud Group 7 Layer 3 cloud top height = 1.392272
Cloud Group 7 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 1 cloud top height = 6.098949
Cloud Group 8 Layer 2 cloud top height = 4.270240
Cloud Group 8 Layer 3 cloud top height = 1.392272
Cloud Group 8 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 1 cloud top height = 6.098949
Cloud Group 9 Layer 2 cloud top height = 3.640685
Cloud Group 9 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 1 cloud top height = 6.098949
Cloud Group 10 Layer 2 cloud top height = 4.839838
Cloud Group 10 Layer 3 cloud top height = 1.392272
Cloud Group 10 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 1 cloud top height = 7.358060
Cloud Group 11 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 1 cloud top height = 6.098949
Cloud Group 12 Layer 2 cloud top height = 4.120346
Cloud Group 12 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 1 cloud top height = 3.760600
Cloud Group 13 Layer 2 cloud top height = 1.392272
Cloud Group 13 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 1 cloud top height = 6.068971
Cloud Group 14 Layer 2 cloud top height = 3.760600
Cloud Group 14 Layer 3 cloud top height = 1.392272
Cloud Group 14 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 1 cloud top height = 6.188886
Cloud Group 15 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000

Footprint 5

Cloud layer top level height 
Cloud Group 1 Layer 1 cloud top height = 1.662082
Cloud Group 1 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 1 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 2 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 3 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 4 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 5 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 6 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 7 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 8 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 9 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 10 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 11 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 12 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 13 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 14 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 15 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 1 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 2 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 3 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 4 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 5 cloud top height = 340282346638528859811704183484516925440.000000
Cloud Group 16 Layer 6 cloud top height = 340282346638528859811704183484516925440.000000


*********************************************************
*    Example 4:                                         *
*********************************************************

Central wavelengths of imager channels

1.	0.469000
2.	0.555000
3.	0.645000
4.	0.858000
5.	0.905000
6.	1.240000
7.	1.375000
8.	1.640000
9.	2.130000
10.	3.792000
11.	4.050000
12.	6.720000
13.	8.550000
14.	11.030000
15.	12.020000
16.	13.340000
17.	13.640000
18.	13.940000
19.	14.240000
20.	0.000000


*********************************************************
*    Example 5:                                         *
*********************************************************

ASSOCIATEDPLATFORMSHORTNAME = AQUA                            


*********************************************************
*    Example 6:                                         *
*********************************************************

NUMBEROFRECORDS = 5

 
8.0 Additional Information Regarding CERES CCCM Data
 
 
 
     Last Updated: December 17, 2010
-----------------------------------------------------------------------------
