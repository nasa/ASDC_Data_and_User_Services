          Clouds and Earth's Radiant Energy System (CERES)

                   CERES ES-4 R6V1-894 Readme File


 1.0 Introduction

     This readme file describes and shows how to read several ES-4 data sets:

     CER_ES4_TRMM_PFM_Edition1
     CER_ES4_TRMM_PFM_Edition2

     CER_ES4_Terra_FM1_Edition1
     CER_ES4_Terra_FM2_Edition1
     CER_ES4_FM1+FM2_Edition1
     CER_ES4_Terra-FM1_Edition1-CV
     CER_ES4_Terra-FM2_Edition1-CV
     CER_ES4_Terra_FM1_Edition2
     CER_ES4_Terra_FM2_Edition2
     CER_ES4_FM1+FM2_Edition2
     CER_ES4_Terra_Edition3

     CER_ES4_Aqua_FM3_Edition1
     CER_ES4_Aqua_FM4_Edition1
     CER_ES4_Aqua-FM3_Edition1-CV
     CER_ES4_Aqua-FM4_Edition1-CV
     CER_ES4_Aqua_FM3_Edition2
     CER_ES4_Aqua_FM4_Edition2
     CER_ES4_Aqua_Edition3

     CER_ES4_NPP-FM5_Edition1-CV

     CER_ES4_PFM+FM1_Edition1
     CER_ES4_PFM+FM2_Edition1
     CER_ES4_PFM+FM1+FM2_Edition1
     CER_ES4_PFM+FM1_Edition2
     CER_ES4_PFM+FM2_Edition2
     CER_ES4_PFM+FM1+FM2_Edition2

     CER_ES4_FM1+FM2+FM3+FM4_Edition1
     CER_ES4_FM2+FM3_Edition2
     CER_ES4_FM1+FM4_Edition2
     CER_ES4_FM1+FM3_Edition2
     CER_ES4_Terra+Aqua_Edition3

     This document includes the following sections:
        Section 2.0 - Data Set Description
        Section 3.0 - Format and Packaging
        Section 4.0 - Science Parameters Information
        Section 5.0 - Description of Sample Read Software
        Section 6.0 - Implementing Sample Read Software
        Section 7.0 - Sample Output
        Section 8.0 - Document History

     If users have questions, please contact the Langley ASDC Science,
     Users and Data Services Office at:

         Atmospheric Science Data Center
         User and Data Services Office
         Mail Stop 157D
         2 South Wright Street
         NASA Langley Research Center
         Hampton, Virginia 23681-2199
         U.S.A.

         E-mail:   larc-asdc-uds@lists.nasa.gov
         Phone:    (757) 864-8656
         FAX:      (757) 864-8807
         URL:      http://eosweb.larc.nasa.gov



 2.0 Data Set Description


    2.1 CERES Experiment and Instrumentation Description
              http://ceres.larc.nasa.gov

    2.2 ES4 Data Quality Summaries
              http://eosweb.larc.nasa.gov/PRODOCS/ceres/level3_es4_table.html

    2.3 Other pertinent information
        The CERES ES-4 product is produced from software maintained
        by the CERES Inversion Data Management Team.

    2.4 Science Representatives
        *NOTE TO USER:
         If you have any questions concerning the sample code or 
         the data, please contact the Atmospheric Sciences Data
         Center User Services Office. The contact information is 
         provided above in Section 1.0.

         1. Dr. Norman G. Loeb, CERES Principal Investigator
    	    NASA Langley Research Center
            Mail Stop 420
            Hampton, VA 23681-2199
            E-mail:   norman.g.loeb@nasa.gov
            FAX:      (757) 864-7996
            Phone:    (757) 864-5688

         2. Dr. Takmeng Wong, Subsystem 3.0 Working Group Chair
            NASA Langley Research Center
            Mail Stop 420
            Hampton, VA 23681-2199
            E-mail:   takmeng.wong@nasa.gov
            FAX:      (757) 864-7996
            Phone:    (757) 864-5607


 3.0 Format and Packaging

     This package includes a program, written in C, which will read 
     an ES-4 HDF file.  The examples provided in the main (ES4_readhdf.c) 
     program are written specifically for SDS(s) and Vdata(s) in ES-4 files.
     This read program is provided as a template to the user who will need 
     to modify it, if they need to read different ES-4 parameters.

     Information on the structure of the ES-4 file can be found
     in the Data Products Catalog (DPC) pages included in this 
     package.  This information is also available in the enclosed 
     dump file, CER_ES4_Aqua-FM3_Sample_024024.200306.dump.  Complete parameter 
     descriptions are available in the Collection/User's Guide, which 
     is on-line at URL http://ceres.larc.nasa.gov/collect_guide.php

    3.1 Sample Read Package Contents

        This package consists of the following files:

        1) CER_ES4_Aqua-FM3_Sample_024024.200306
        2) CER_ES4_Aqua-FM3_Sample_024024.200306.dump
        3) CER_ES4_Aqua-FM3_Sample_024024.200306.met
        4) DPC_ES4_R6V1.pdf
        5) ES4_HDFread.h
        6) ES4_readHDFfuncs.c
        7) ES4_readhdf.c
        8) compile_ES4_readhdf
        9) readme_cer_es4_SampleRead_R6V1-894 
 
        Descriptions for each file is as follows:

        1)'CER_ES4_Aqua-FM3_Sample_024024.200306' is a sample 5-footprint ES-4 file 
          which is used by the examples in 'ES4_readhdf.c'

        2)'CER_ES4_Aqua-FM3_Sample_024024.200306.dump' is an ASCII file containing a 
          description of the data in the sample ES-4, 
          'CER_ES4_Aqua-FM3_Sample_024024.200306'.
          If a value in the ES-4 granule is out-of-range or not valid, a 
          default value is inserted in its place.  Default values depend 
          on data type.  CERES default values are greater than or equal to:
          INT2    - 32767
          INT4    - 2147483647
          REAL4   - 3.402823E+38
          REAL8   - 1.797693134862315E+308

          The 'CER_ES4_Aqua-FM3_Sample_024024.200306.dump' file 
          includes a list of ALL the 
          data on the 5 footprint sample ES-4 (20 parameters/SDS's plus header 
          Vdata). The output data are the same as what the user would produce 
          using hdp, but for this example, the CERES Team modified the format
          of the output from the hdp command to make it easier to read.

        3) CER_ES4_Aqua-FM3_Sample_024024.200306.met is an ASCII metadata file that is 
          delivered with every data granule.  The file is written in 
          Object Descriptive Language (ODL) and contains information that 
          is associated with the origin, content, quality, and condition 
          of the data granule.  The metadata information contained in 
          the granule metadata file is also included as metadata on the 
          ES-4 HDF data file.  An example of a metadata parameter found 
          in the metadata file and on the ES-4 HDF file is LOCALGRANULEID, 
          which has a value set to the original granule file name.

        4)'DPC_ES4_R6V1.pdf' is a copy of the CERES Data Product Catalog 
          pages which describes the ES-4 product.

        5)'ES4_HDFread.h' is a header file for 'ES4_readhdf.c' 
          and the ES-4 HDF read functions.

        6)'ES4_readHDFfuncs.c' is a file consisting of C functions that
          are linked with 'ES4_readhdf.c' and call the HDF functions.

        7)'ES4_readhdf.c' is the main program which accesses the HDF reading
          functions.  It consists of six examples of accessing SDS and
          Vdata structures from the included sample ES-4 file.  Comments are 
          provided throughout the program.  This file must be modified if the
          user wishes to read SDSs or Vdatas other than those in the examples.

        8)'compile_ES4_readhdf' is a sample script to compile the C programs
          in a UNIX environment. The script must be modified for different
          platforms to properly compile the programs and correctly link
          the HDF libraries.

        9)'readme_cer_es4_SampleRead_R6V1-894' is this readme file describing
          the contents of the sample read package.

    3.2 Zip Information

        To read the files, first unzip them with Info-Zip software.
        Information about downloading and using this freeware is
        available at the URL:
        http://www.info-zip.org/pub/infozip/Info-ZIP.html

    3.3 HDF library

        HDF information, documentation, tutorials, libraries, etc.
        can be found on-line at URL http://eosweb.larc.nasa.gov/HBDOCS/hdf.html

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

        The ES-4 data files are written in HDF as collections of SDS(s) and 
        Vdata(s).

    3.5 Breakdown of the File Naming Convention

        The ES-4 data granules adhere to the following CERES file
        naming convention:

                  CER_PID_SS_PS_CC.DataDate

        where: CER = the CERES experiment

               PID = the CERES product, e.g., ES4

                SS = the CERES sampling strategy, e.g.,
                     Terra-FM2, as defined by the CERES experiment.
                     For the ES-4, this indicates the satellite and
                     instruments that were the source of the input
                     data into the software system that produced
                     the ES-4 data.

                PS = the CERES production strategy, e.g.,
                     'Beta2', as defined by the CERES experiment.
                     'Edition2' production strategies indicate versions
                     of the product produced with algorithms that
                     are still being validated.  ES-4 data products
                     with BetaN production strategies may only be
                     available for a limited time and cannot be
                     be used to publish. ES-4 data products with
                     'EditionN' production strategies usually indicate 
                     products that contain validated data with which 
                     the user can  publish.

                CC = the product configuration code,
                     e.g., '017018', assigned by the CERES experiment.
                     This code is internal to the CERES project, and
                     is updated whenever the software system producing
                     the product is modified for any reason. Multiple 
                     files that are identical in all aspects of the 
                     filename except for the 6 digit configuration code
                     differ little, if any, scientifically. Users may, 
                     therefore, analyze data from the same sampling
                     strategy and production strategy without regard 
                     to configuration code. For a complete history
                     of the changes to the software, see the ES-4
                     product Description/Abstract document available
                     from the URL
                     http://eosweb.larc.nasa.gov/GUIDE/dataset_documents/cer_es4.html

          DataDate = the date of the data included in the
                     file.  For the ES-4, this date is in the format

                                 YYYYMMDD

                     where: YYYY = the four-digit data year,
                              MM = the two-digit month
                              DD = the two-digit day


 4.0 Science Parameters Information

     Refer to the ES-4 CERES Data Products Catalog pages included
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
     make up the ES-4 product and the number of data records contained
     on an actual ES-4 file are large, the sample software only 
     demonstrates reading a limited number of SDSs for the five
     footprints included in the sample ES-4 file.  The contents of
     the ES-4 footprint, along with a list of all the SDS(s) contained
     on the ES-4 product, are described in the Data Products Catalog
     pages discussed in Section 3.1.

     A note to the user:
     
     It is recommended that before modifiying the ES-4 read program to 
     read different ES-4 parameters, the user should first compile and 
     run the ES-4 read program using the sample ES-4 file as input and 
     compare the results that are created by the read program for 
     each SDS/parameter with the list of values for corresponding 
     parameters in the ES-4 sample dump file,
     'CER_ES4_Aqua-FM3_Sample_024024.200306.dump'.
     If this test is successful, then the user should modify the 
     the ES-4 read program or their own software to read the parameters 
     in which they are interested and run that software using the Sample 
     ES-4 as input. The values read should be compared to the values 
     of the corresponding parameters in the sample dump file.  If both 
     of these test are successful, then the user will be ready to use 
     their altered program with a full size ES-4 data file.


 6.0 Implementing Sample Read Software


    6.1 Compiling the Sample Read Software

       1. Download and install the HDF libraries from The HDF Group website

                  http://www.hdfgroup.org/release4/obtain.html

         This code uses version HDF4.1r5.  Other versions of the HDF
         libraries (i.e. HDF5) may not work with this code. The GNU gzip
         compression utility for some packages. If necessary, the
         software needed to uncompress the file can be obtained from
         one of the GNU mirror sites listed at the URL:

                http://www.gnu.org/order/ftp.html

       2. Edit the sample script 'compile_ES4_readhdf' to link the
          HDF libraries and compile the C programs.  Check the permissions
          on this file to make sure execute permission is set for the owner
          by typing at the command line (denoted by ">"):

           > ls -l compile_ES4_readhdf

          A line similar to the following will be displayed:

          -rwxr--r--  1 OWNER  GROUP 1450 Mar 6 14:49 compile_ES4_readhdf

          where OWNER and GROUP are the file owner and group information.
          The owner executable permission is the x in -rwxr--r--.

          To set the proper permissions, at the command line type:

           > chmod 744 compile_ES4_readhdf

       3. Execute the script 'compile_ES4_readhdf' to create the
          executable es4_rd by typing at the command line:


           > source compile_ES4_readhdf

          or, on some systems, type:

           > compile_ES4_readhdf


    6.2 How to Run the Sample Read Software

        Execute es4_rd by typing at the command line:

             > es4_rd CER_ES4_Aqua-FM3_Sample_024024.200306


 7.0 Sample Output

The following is the screen output generated from executing the es4_rd
program with the provided sample file.

> es4_rd CER_ES4_Aqua-FM3_Sample_024024.200306

Name of ES4 HDF file to be read:
< CER_ES4_Aqua-FM3_Sample_024024.200306


*********************************************************
*  Example 1:   ( ONLY PRINTING MAX OF 5 PER LAT ZONE)  *
*********************************************************

Solar incidence for Lat Zone 1

   Longitude Region (1) = 373204.250000
   Longitude Region (2) = 340282346638528860000000000000000000000.000000
   Longitude Region (3) = 340282346638528860000000000000000000000.000000
   Longitude Region (4) = 340282346638528860000000000000000000000.000000
   Longitude Region (5) = 340282346638528860000000000000000000000.000000

Solar incidence for Lat Zone 2

   Longitude Region (1) = 372493.843750
   Longitude Region (2) = 372493.843750
   Longitude Region (3) = 372493.843750
   Longitude Region (4) = 372493.843750
   Longitude Region (5) = 372493.843750

Solar incidence for Lat Zone 3

   Longitude Region (1) = 371074.312500
   Longitude Region (2) = 371074.312500
   Longitude Region (3) = 371074.312500
   Longitude Region (4) = 371074.312500
   Longitude Region (5) = 371074.312500

Solar incidence for Lat Zone 4

   Longitude Region (1) = 368948.468750
   Longitude Region (2) = 368948.468750
   Longitude Region (3) = 368948.468750
   Longitude Region (4) = 368948.468750
   Longitude Region (5) = 368948.468750

Solar incidence for Lat Zone 5

   Longitude Region (1) = 366120.312500
   Longitude Region (2) = 366120.312500
   Longitude Region (3) = 366120.312500
   Longitude Region (4) = 366120.312500
   Longitude Region (5) = 366120.312500



************************************************************
*  Example 2:   ( ONLY PRINTING MAX OF 5 PER DIMENSION)    *
************************************************************

Day/Hour =  1

   Longwave flux for Lat Zone 1
      Longitude Region (1) = 216.800003
      Longitude Region (2) = 340282346638528860000000000000000000000.000000
      Longitude Region (3) = 340282346638528860000000000000000000000.000000
      Longitude Region (4) = 340282346638528860000000000000000000000.000000
      Longitude Region (5) = 340282346638528860000000000000000000000.000000

   Longwave flux for Lat Zone 2
      Longitude Region (1) = 221.500000
      Longitude Region (2) = 221.100006
      Longitude Region (3) = 220.300003
      Longitude Region (4) = 220.100006
      Longitude Region (5) = 219.600006

   Longwave flux for Lat Zone 3
      Longitude Region (1) = 220.899994
      Longitude Region (2) = 220.500000
      Longitude Region (3) = 219.699997
      Longitude Region (4) = 218.899994
      Longitude Region (5) = 219.199997

   Longwave flux for Lat Zone 4
      Longitude Region (1) = 219.000000
      Longitude Region (2) = 218.899994
      Longitude Region (3) = 217.699997
      Longitude Region (4) = 217.100006
      Longitude Region (5) = 216.100006

   Longwave flux for Lat Zone 5
      Longitude Region (1) = 214.500000
      Longitude Region (2) = 215.000000
      Longitude Region (3) = 215.300003
      Longitude Region (4) = 216.300003
      Longitude Region (5) = 218.699997


Day/Hour =  2

   Longwave flux for Lat Zone 1
      Longitude Region (1) = 217.399994
      Longitude Region (2) = 340282346638528860000000000000000000000.000000
      Longitude Region (3) = 340282346638528860000000000000000000000.000000
      Longitude Region (4) = 340282346638528860000000000000000000000.000000
      Longitude Region (5) = 340282346638528860000000000000000000000.000000

   Longwave flux for Lat Zone 2
      Longitude Region (1) = 219.600006
      Longitude Region (2) = 219.500000
      Longitude Region (3) = 219.899994
      Longitude Region (4) = 220.800003
      Longitude Region (5) = 220.600006

   Longwave flux for Lat Zone 3
      Longitude Region (1) = 221.000000
      Longitude Region (2) = 220.600006
      Longitude Region (3) = 221.600006
      Longitude Region (4) = 220.300003
      Longitude Region (5) = 219.300003

   Longwave flux for Lat Zone 4
      Longitude Region (1) = 224.399994
      Longitude Region (2) = 223.800003
      Longitude Region (3) = 223.100006
      Longitude Region (4) = 221.800003
      Longitude Region (5) = 220.800003

   Longwave flux for Lat Zone 5
      Longitude Region (1) = 224.000000
      Longitude Region (2) = 223.300003
      Longitude Region (3) = 221.300003
      Longitude Region (4) = 220.800003
      Longitude Region (5) = 219.199997


Day/Hour =  3

   Longwave flux for Lat Zone 1
      Longitude Region (1) = 214.699997
      Longitude Region (2) = 340282346638528860000000000000000000000.000000
      Longitude Region (3) = 340282346638528860000000000000000000000.000000
      Longitude Region (4) = 340282346638528860000000000000000000000.000000
      Longitude Region (5) = 340282346638528860000000000000000000000.000000

   Longwave flux for Lat Zone 2
      Longitude Region (1) = 214.000000
      Longitude Region (2) = 214.800003
      Longitude Region (3) = 214.600006
      Longitude Region (4) = 214.800003
      Longitude Region (5) = 215.399994

   Longwave flux for Lat Zone 3
      Longitude Region (1) = 218.000000
      Longitude Region (2) = 217.500000
      Longitude Region (3) = 218.899994
      Longitude Region (4) = 218.399994
      Longitude Region (5) = 217.899994

   Longwave flux for Lat Zone 4
      Longitude Region (1) = 221.699997
      Longitude Region (2) = 220.500000
      Longitude Region (3) = 218.899994
      Longitude Region (4) = 217.600006
      Longitude Region (5) = 216.899994

   Longwave flux for Lat Zone 5
      Longitude Region (1) = 226.600006
      Longitude Region (2) = 226.199997
      Longitude Region (3) = 226.899994
      Longitude Region (4) = 226.000000
      Longitude Region (5) = 226.699997


Day/Hour =  4

   Longwave flux for Lat Zone 1
      Longitude Region (1) = 216.699997
      Longitude Region (2) = 340282346638528860000000000000000000000.000000
      Longitude Region (3) = 340282346638528860000000000000000000000.000000
      Longitude Region (4) = 340282346638528860000000000000000000000.000000
      Longitude Region (5) = 340282346638528860000000000000000000000.000000

   Longwave flux for Lat Zone 2
      Longitude Region (1) = 215.500000
      Longitude Region (2) = 215.500000
      Longitude Region (3) = 214.399994
      Longitude Region (4) = 214.000000
      Longitude Region (5) = 214.699997

   Longwave flux for Lat Zone 3
      Longitude Region (1) = 212.800003
      Longitude Region (2) = 211.399994
      Longitude Region (3) = 211.800003
      Longitude Region (4) = 210.800003
      Longitude Region (5) = 212.100006

   Longwave flux for Lat Zone 4
      Longitude Region (1) = 218.199997
      Longitude Region (2) = 217.100006
      Longitude Region (3) = 216.800003
      Longitude Region (4) = 216.600006
      Longitude Region (5) = 216.699997

   Longwave flux for Lat Zone 5
      Longitude Region (1) = 221.800003
      Longitude Region (2) = 223.100006
      Longitude Region (3) = 224.199997
      Longitude Region (4) = 223.600006
      Longitude Region (5) = 223.399994


Day/Hour =  5

   Longwave flux for Lat Zone 1
      Longitude Region (1) = 215.100006
      Longitude Region (2) = 340282346638528860000000000000000000000.000000
      Longitude Region (3) = 340282346638528860000000000000000000000.000000
      Longitude Region (4) = 340282346638528860000000000000000000000.000000
      Longitude Region (5) = 340282346638528860000000000000000000000.000000

   Longwave flux for Lat Zone 2
      Longitude Region (1) = 213.199997
      Longitude Region (2) = 213.600006
      Longitude Region (3) = 214.600006
      Longitude Region (4) = 215.899994
      Longitude Region (5) = 216.399994

   Longwave flux for Lat Zone 3
      Longitude Region (1) = 207.100006
      Longitude Region (2) = 207.300003
      Longitude Region (3) = 209.199997
      Longitude Region (4) = 211.199997
      Longitude Region (5) = 213.600006

   Longwave flux for Lat Zone 4
      Longitude Region (1) = 209.300003
      Longitude Region (2) = 210.100006
      Longitude Region (3) = 212.800003
      Longitude Region (4) = 214.699997
      Longitude Region (5) = 214.300003

   Longwave flux for Lat Zone 5
      Longitude Region (1) = 212.800003
      Longitude Region (2) = 211.600006
      Longitude Region (3) = 213.100006
      Longitude Region (4) = 220.899994
      Longitude Region (5) = 222.100006



*********************************************************
*    Example 3:                                         *
*********************************************************
Geographic scene type for Region (1) = 3
Geographic scene type for Region (2) = 3
Geographic scene type for Region (3) = 3
Geographic scene type for Region (4) = 3
Geographic scene type for Region (5) = 3



*********************************************************
*    Example 4:                                         *
*********************************************************

ASSOCIATEDPLATFORMSHORTNAME = Aqua                            



-----------------------------------------------------------------------------



 8.0 Document history

     Version 0.00
        UnKnown
     Version 1.01
        New document format
        Dale Walikainen @SAIC 
        23 Dec 2004
     Version 2.00
        Updated for new data sets (Edition3, FM5), websites, contacts
        Jeremie Lande @SSAI
        20 Apr 2012