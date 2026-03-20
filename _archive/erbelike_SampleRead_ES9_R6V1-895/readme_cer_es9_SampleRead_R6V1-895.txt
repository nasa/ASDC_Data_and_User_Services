             Clouds and Earth's Radiant Energy System (CERES)

                      CERES ES-9 R6V1-895 Readme File


 1.0 Introduction

     This readme file describes and shows how to read several ES-9 data sets:

     CER_ES9_TRMM_PFM_Edition1
     CER_ES9_TRMM_PFM_Edition2

     CER_ES9_Terra_FM1_Edition1
     CER_ES9_Terra_FM2_Edition1
     CER_ES9_FM1+FM2_Edition1
     CER_ES9_Terra-FM1_Edition1-CV
     CER_ES9_Terra-FM2_Edition1-CV
     CER_ES9_Terra_FM1_Edition2
     CER_ES9_Terra_FM2_Edition2
     CER_ES9_FM1+FM2_Edition2
     CER_ES9_Terra_Edition3

     CER_ES9_Aqua_FM3_Edition1
     CER_ES9_Aqua_FM4_Edition1
     CER_ES9_Aqua-FM3_Edition1-CV
     CER_ES9_Aqua-FM4_Edition1-CV
     CER_ES9_Aqua_FM3_Edition2
     CER_ES9_Aqua_FM4_Edition2
     CER_ES9_Aqua_Edition3

     CER_ES9_NPP-FM5_Edition1-CV

     CER_ES9_PFM+FM1_Edition1
     CER_ES9_PFM+FM2_Edition1
     CER_ES9_PFM+FM1+FM2_Edition1
     CER_ES9_PFM+FM1_Edition2
     CER_ES9_PFM+FM2_Edition2
     CER_ES9_PFM+FM1+FM2_Edition2

     CER_ES9_FM1+FM2+FM3+FM4_Edition1
     CER_ES9_FM2+FM3_Edition2
     CER_ES9_FM1+FM4_Edition2
     CER_ES9_FM1+FM3_Edition2
     CER_ES9_Terra+Aqua_Edition3

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

    2.2 ES9 Data Quality Summaries
              http://eosweb.larc.nasa.gov/PRODOCS/ceres/level3_es9_table.html

    2.3 Other pertinent information
        The CERES ES-9 product is produced from software maintained
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
     an ES-9 HDF file.  The examples provided in the main (ES9_readhdf.c) 
     program are written specifically for SDS(s) and Vdata(s) in ES-9 files.
     This read program is provided as a template to the user who will need 
     to modify it, if they need to read different ES-9 parameters.

     Information on the structure of the ES-9 file can be found
     in the Data Products Catalog (DPC) pages included in this 
     package.  This information is also available in the enclosed 
     dump file, CER_ES9_Aqua-FM3_Sample_024024.200306.dump.  Complete parameter 
     descriptions are available in the Collection/User's Guide, which 
     is on-line at URL http://ceres.larc.nasa.gov/collect_guide.php

    3.1 Sample Read Package Contents

        This package consists of the following files:

        1) CER_ES9_Aqua-FM3_Sample_024024.200306
        2) CER_ES9_Aqua-FM3_Sample_024024.200306.dump
        3) CER_ES9_Aqua-FM3_Sample_024024.200306.met
        4) DPC_ES9_R6V1.pdf
        5) ES9_HDFread.h
        6) ES9_readHDFfuncs.c
        7) ES9_readhdf.c
        8) compile_ES9_readhdf
        9) readme_cer_es9_SampleRead_R6V1-895 
 
        Descriptions for each file is as follows:

        1)'CER_ES9_Aqua-FM3_Sample_024024.200306' is a sample 5-footprint ES-9 file 
          which is used by the examples in 'ES9_readhdf.c'

        2)'CER_ES9_Aqua-FM3_Sample_024024.200306.dump' is an ASCII file containing a 
          description of the data in the sample ES-9, 
          'CER_ES9_Aqua-FM3_Sample_024024.200306'.
          If a value in the ES-9 granule is out-of-range or not valid, a 
          default value is inserted in its place.  Default values depend 
          on data type.  CERES default values are greater than or equal to:
          INT2    - 32767
          INT4    - 2147483647
          REAL4   - 3.402823E+38
          REAL8   - 1.797693134862315E+308

          The 'CER_ES9_Aqua-FM3_Sample_024024.200306.dump' file 
          includes a list of ALL the 
          data on the 5 footprint sample ES-9 (20 parameters/SDS's plus header 
          Vdata). The output data are the same as what the user would produce 
          using hdp, but for this example, the CERES Team modified the format
          of the output from the hdp command to make it easier to read.

        3) CER_ES9_Aqua-FM3_Sample_024024.200306.met is an ASCII metadata file that is 
          delivered with every data granule.  The file is written in 
          Object Descriptive Language (ODL) and contains information that 
          is associated with the origin, content, quality, and condition 
          of the data granule.  The metadata information contained in 
          the granule metadata file is also included as metadata on the 
          ES-9 HDF data file.  An example of a metadata parameter found 
          in the metadata file and on the ES-9 HDF file is LOCALGRANULEID, 
          which has a value set to the original granule file name.

        4)'DPC_ES9_R6V1.pdf' is a copy of the CERES Data Product Catalog 
          pages which describes the ES-9 product.

        5)'ES9_HDFread.h' is a header file for 'ES9_readhdf.c' 
          and the ES-9 HDF read functions.

        6)'ES9_readHDFfuncs.c' is a file consisting of C functions that
          are linked with 'ES9_readhdf.c' and call the HDF functions.

        7)'ES9_readhdf.c' is the main program which accesses the HDF reading
          functions.  It consists of six examples of accessing SDS and
          Vdata structures from the included sample ES-9 file.  Comments are 
          provided throughout the program.  This file must be modified if the
          user wishes to read SDSs or Vdatas other than those in the examples.

        8)'compile_ES9_readhdf' is a sample script to compile the C programs
          in a UNIX environment. The script must be modified for different
          platforms to properly compile the programs and correctly link
          the HDF libraries.

        9)'readme_cer_es9_SampleRead_R6V1-895' is this readme file describing
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

        The ES-9 data files are written in HDF as collections of SDS(s) and 
        Vdata(s).

    3.5 Breakdown of the File Naming Convention

        The ES-9 data granules adhere to the following CERES file
        naming convention:

                  CER_PID_SS_PS_CC.DataDate

        where: CER = the CERES experiment

               PID = the CERES product, e.g., ES9

                SS = the CERES sampling strategy, e.g.,
                     Terra-FM2, as defined by the CERES experiment.
                     For the ES-9, this indicates the satellite and
                     instruments that were the source of the input
                     data into the software system that produced
                     the ES-9 data.

                PS = the CERES production strategy, e.g.,
                     'Beta2', as defined by the CERES experiment.
                     'Edition2' production strategies indicate versions
                     of the product produced with algorithms that
                     are still being validated.  ES-9 data products
                     with BetaN production strategies may only be
                     available for a limited time and cannot be
                     be used to publish. ES-9 data products with
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
                     of the changes to the software, see the ES-9
                     product Description/Abstract document available
                     from the URL
                     http://eosweb.larc.nasa.gov/GUIDE/dataset_documents/cer_es9.html

          DataDate = the date of the data included in the
                     file.  For the ES-9, this date is in the format

                                 YYYYMMDD

                     where: YYYY = the four-digit data year,
                              MM = the two-digit month
                              DD = the two-digit day


 4.0 Science Parameters Information

     Refer to the ES-9 CERES Data Products Catalog pages included
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
     make up the ES-9 product and the number of data records contained
     on an actual ES-9 file are large, the sample software only 
     demonstrates reading a limited number of SDSs for the five
     footprints included in the sample ES-9 file.  The contents of
     the ES-9 footprint, along with a list of all the SDS(s) contained
     on the ES-9 product, are described in the Data Products Catalog
     pages discussed in Section 3.1.

     A note to the user:
     
     It is recommended that before modifiying the ES-9 read program to 
     read different ES-9 parameters, the user should first compile and 
     run the ES-9 read program using the sample ES-9 file as input and 
     compare the results that are created by the read program for 
     each SDS/parameter with the list of values for corresponding 
     parameters in the ES-9 sample dump file,
     'CER_ES9_Aqua-FM3_Sample_024024.200306.dump'.
     If this test is successful, then the user should modify the 
     the ES-9 read program or their own software to read the parameters 
     in which they are interested and run that software using the Sample 
     ES-9 as input. The values read should be compared to the values 
     of the corresponding parameters in the sample dump file.  If both 
     of these test are successful, then the user will be ready to use 
     their altered program with a full size ES-9 data file.


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

       2. Edit the sample script 'compile_ES9_readhdf' to link the
          HDF libraries and compile the C programs.  Check the permissions
          on this file to make sure execute permission is set for the owner
          by typing at the command line (denoted by ">"):

           > ls -l compile_ES9_readhdf

          A line similar to the following will be displayed:

          -rwxr--r--  1 OWNER  GROUP 1450 Mar 6 14:49 compile_ES9_readhdf

          where OWNER and GROUP are the file owner and group information.
          The owner executable permission is the x in -rwxr--r--.

          To set the proper permissions, at the command line type:

           > chmod 744 compile_ES9_readhdf

       3. Execute the script 'compile_ES9_readhdf' to create the
          executable es9_rd by typing at the command line:

           > source compile_ES9_readhdf

          or, on some systems, type:

           > compile_ES9_readhdf


    6.2 How to Run the Sample Read Software

        Execute es9_rd by typing at the command line:

             > es9_rd CER_ES9_Aqua-FM3_Sample_024024.200306


 7.0 Sample Output

The following is the screen output generated from executing the es9_rd
program with the provided sample file.

> es9_rd CER_ES9_Aqua-FM3_Sample_024024.200306

Name of ES9 HDF file to be read:
< CER_ES9_Aqua-FM3_Sample_024024.200306


************************************************************
*    Example 1:  ( ONLY PRINTING MAX OF 5 HOUR BOX DATAS)   *
*************************************************************
Hourbox (1) for Longwave flux = 215.199997
Hourbox (2) for Longwave flux = 213.800003
Hourbox (3) for Longwave flux = 214.699997
Hourbox (4) for Longwave flux = 214.800003
Hourbox (5) for Longwave flux = 216.300003



*********************************************************
*    Example 2:   ( ONLY PRINTING MAX OF 5 PER DAY)     *
*********************************************************

Longwave flux standard deviation for day 1

Region (1) = 1.420000
Region (2) = 1.510000
Region (3) = 1.480000
Region (4) = 2.100000
Region (5) = 2.080000

Longwave flux standard deviation for day 2

Region (1) = 0.700000
Region (2) = 2.380000
Region (3) = 3.120000
Region (4) = 2.540000
Region (5) = 2.640000

Longwave flux standard deviation for day 3

Region (1) = 0.920000
Region (2) = 3.690000
Region (3) = 4.570000
Region (4) = 3.680000
Region (5) = 3.850000

Longwave flux standard deviation for day 4

Region (1) = 0.790000
Region (2) = 6.510000
Region (3) = 5.640000
Region (4) = 6.010000
Region (5) = 5.340000

Longwave flux standard deviation for day 5

Region (1) = 1.720000
Region (2) = 10.800000
Region (3) = 10.900000
Region (4) = 10.230000
Region (5) = 9.760000



*********************************************************
*    Example 3:                                         *
*********************************************************

ASSOCIATEDPLATFORMSHORTNAME = Aqua                            



-----------------------------------------------------------------------------



 8.0 Document history

     Version 0.00
        UnKnown
     Version 1.00
        Dale Walikainen @SAIC
        30 Sep 2004
     Version 1.01
        Sample File name corrected
        Dale Walikainen @SAIC 
        22 Dec 2004
     Version 2.00
        Updated for new data sets (Edition3, FM5), websites, contacts
        Jeremie Lande @SSAI
        20 Apr 2012