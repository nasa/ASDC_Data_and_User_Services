

               CERES FSW R5-773 Readme File

 1.0 Introduction

     This readme file document provides information on the 
     Clouds and Earth's Radiant Energy System (CERES)
     FSW data sets.

     TERRA Data sets
     CER_FSW_Terra-FM1-MODIS_Edition2G   
     CER_FSW_Terra-FM2-MODIS_Edition2G


     AQUA Data sets
     CER_FSW_Aqua-FM3-MODIS_Edition2D  
     CER_FSW_Aqua-FM4-MODIS_Edition2D

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

        For a complete description of the CERES experiment and instruments,
        please refer to the documentation provided at the URL 
        http://science.larc.nasa.gov/ceres/index.html.
 
    2.2 Data Quality

        For a discussion regarding the quality of the 
        FSW data sets, please refer to the
        corresponding Quality Summary Document available from the URL
        http://eosweb.larc.nasa.gov/project/ceres/table_ceres.html.       

    2.3 Other pertinent information

        The CERES FSW product is produced from software maintained 
        by the CERES TISA Gridding Data Management Team.

    2.4 Science Contact

        1. Dr. Norman G. Loeb, CERES Interdisciplinary Principal Investigator
           NASA Langley Research Center
           Mail Stop 420
           Hampton, Virginia 23681-2199

           E-mail:   Norman.G.Loeb@nasa.gov
           FAX:      (757)864-7996
           Phone:    (757)864-5688


        2. Mr. David R. Doelling, TISA Working Group Chair
           NASA Langley Research Center
           Mail Stop 420
           Hampton, Virginia 23681-2199

           E-mail:   David.R.Doelling@nasa.gov
           FAX:      (757)864-7996
           Phone:    (757)864-2155

 3.0 Format and Packaging

      This package includes a program, written in C, which will read
      an FSW HDF file.  The examples provided in the main (FSW_readhdf.c)
      program are written specifically for SDS(s) and Vdata(s) in FSW files.
      This read program is provided as a template to the user who will need
      to modify it, if they need to read different FSW parameters.

      Information on the structure of the FSW file can be found
      in the Data Products Catalog (DPC) pages included in this
      package.  This information is also available in the enclosed
      dump file, CER_FSW_Sample_R5V1.dump.  Complete parameter
      descriptions are available in the Collection/User's Guide, which
      is on-line at URL: http://science.larc.nasa.gov/ceres/collect_guide/index.html.

    3.1 Sample Read Package Contents

        This package consists of the following files:

         1) CER_FSW_Sample_R5V1
         2) CER_FSW_Sample_R5V1.dump
         3) CER_FSW_Sample_R5V1.met
         4) DPC-FSW_R5V1.pdf
         5) FSW_HDFread.h
         6) FSW_readHDFfuncs.c
         7) FSW_readhdf.c
         8) compile_FSW_readhdf

         Descriptions for each file is as follows:
         1) 'CER_FSW_Sample_R5V1' is a sample 5-hourboxes FSW file
           which is used by the examples in 'FSW_readhdf.c' 

         2)'CER_FSW_Sample_R5V1.dump' is an ASCII file containing a
           description of the data in the sample FSW, 'CER_FSW_Sample_R5V1'.
           If a value in the FSW granule is out-of-range or not valid, a
           default value is inserted in its place.  Default values depend
           on data type.  CERES default values are greater than or equal to:
           INT2    - 32767
           INT4    - 2147483647
           REAL4   - 3.402823E+38
           REAL8   - 1.797693134862315E+308

           The 'CER_FSW_Sample_R5V1.dump' file includes a list of ALL the
           data on the 5 footprint sample FSW (273 parameters/SDSs' plus 
           header Vdata.  The output data are the same as what the user would 
           produce using hdp, but for this example, the CERES Team modified 
           the format of the output from the hdp command to make it easier 
           to read.

         3)CER_FSW_Sample_R5V1.met is an ASCII metadata file that is
           delivered with every data granule.  The file is written in
           Object Descriptive Language (ODL) and contains information that
           is associated with the origin, content, quality, and condition
           of the data granule.  The metadata information contained in
           the granule metadata file is also included as metadata on the
           FSW HDF data file.  An example of a metadata parameter found
           in the metadata file and on the FSW HDF file is LOCALGRANULEID,
           which has a value set to the original granule file name.

         4)'FSW_DPC_R5V1.pdf' is a copy of the CERES Data Product Catalog
           pages which describes the FSW product.

         5)'FSW_HDFread.h' is a header file for 'FSW_readhdf.c'
           and the FSW HDF read functions.

         6)'FSW_readHDFfuncs.c' is a file consisting of C functions that
           are linked with 'FSW_readhdf.c' and call the HDF functions.

         7)'FSW_readhdf.c' is the main program which accesses the HDF reading
           functions.  It consists of six examples of accessing SDS and
           Vdata structures from the included sample FSW file.  Comments are
           provided throughout the program.  This file must be modified if the
           user wishes to read SDSs or Vdatas other than those in the examples.

         8)'compile_FSW_readhdf' is a sample script to compile the C programs
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
         can be found on-line at the URL  http://www.hdfgroup.org.

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

         The FSW data files are written in HDF as collections of SDS(s) and
         Vdata(s).

     3.5 Breakdown of the File Naming Convention

         The FSW data granules adhere to the following CERES file
         naming convention:

                   CER_PID_SS_PS_CC.DataDate

         where: CER = the CERES experiment

                PID = the CERES product, e.g., FSW

                 SS = the CERES sampling strategy, e.g.,
                      Terra-FM2-MODIS, as defined by the CERES experiment.
                      For the FSW, this indicates the satellite and
                      instruments that were the source of the input
                      data into the software system that produced
                      the FSW data.

                 PS = the CERES production strategy, e.g.,
                      'Beta2', as defined by the CERES experiment.
                      'BetaN' production strategies indicate versions
                      of the product produced with algorithms that
                      are still being validated.  FSW data products
                      with BetaN production strategies may only be
                      available for a limited time and cannot be
                      be used to publish. FSW data products with
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
                      of the changes to the software, see the FSW
                      product Description/Abstract document available
                      from the URL
                      http://eosweb.larc.nasa.gov/PRODOCS/ceres/table_ceres.html

           DataDate = the date of the data included in the
                      file.  For the FSW, this date is in the format

                                  YYYYMMDDHH

                      where: YYYY = the four-digit data year,
                               MM = the two-digit month
                               DD = the two-digit day
                               HH = the two-digit hour (00-23)

  4.0 Science Parameters Information

      Refer to the FSW CERES Data Products Catalog pages included
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
      make up the FSW product and the number of data records contained
      on an actual FSW file are large, the sample software only
      demonstrates reading a limited number of SDSs for the five
      footprints included in the sample FSW file.  The contents of
      the FSW footprint, along with a list of all the SDS(s) contained
      on the FSW product, are described in the Data Products Catalog
      pages discussed in Section 3.1.

      A note to the user:

      It is recommended that before modifiying the FSW read program to
      read different FSW parameters, the user should first compile and
      run the FSW read program using the sample FSW file as input and
      compare the results that are created by the read program for
      each SDS/parameter with the list of values for corresponding
      parameters in the FSW sample dump file,'CER_FSW_Sample_R5V1.dump'.
      If this test is successful, then the user should modify the
      the FSW read program or their own software to read the parameters
      in which they are interested and run that software using the Sample
      FSW as input. The values read should be compared to the values
      of the corresponding parameters in the sample dump file.  If both
      of these test are successful, then the user will be ready to use
      their altered program with a full size FSW data file.

  6.0 Implementing Sample Read Software

     6.1 Compiling the Sample Read Software

        1. Download and install the HDF libraries from the NCSA web site

                   http://www.hdfgroup.org

          This code uses version HDF4.1r5.  Other versions of the HDF
          libraries may not work with this code. NCSA uses the GNU gzip
          compression utility for some packages. If necessary, the
          software needed to uncompress the file can be obtained from
          one of the GNU mirror sites listed at the URL:

                 http://www.gnu.org/order/ftp.html

        2. Edit the sample script 'compile_FSW_readhdf' to link the
           HDF libraries and compile the C programs.  Check the permissions
           on this file to make sure execute permission is set for the owner
           by typing at the command line (denoted by ">"):


            > ls -l compile_FSW_readhdf

           A line similar to the following will be displayed:

           -rwxr--r--  1 OWNER  GROUP 1450 Mar 6 14:49 compile_FSW_readhdf

           where OWNER and GROUP are the file owner and group information.
           The owner executable permission is the x in -rwxr--r--.

           To set the proper permissions, at the command line type:

            > chmod 744 compile_FSW_readhdf

        3. Execute the script 'compile_FSW_readhdf' to create the
           executable FSW_rd by typing at the command line:


            > source compile_FSW_readhdf

           or, on some systems, type:

            > compile_FSW_readhdf


     6.2 How to Run the Sample Read Software

         Execute FSW_rd by typing at the command line:

              > FSW_rd CER_FSW_Sample_R5V1


  7.0 Sample Output


The following is the screen output generated from executing the FSW_rd
program with the provided sample file.

> FSW_rd CER_FSW_Sample_R5V1

Name of HDF file to be read:
< CER_FSW_Sample_R5V1



*********************************************************
*    Example 1:                                         *
*********************************************************
Hour Box 1
Hour Box Number  = 1093664768

Hour Box 2
Hour Box Number  = 1102577664

Hour Box 3
Hour Box Number  = 1111228416

Hour Box 4
Hour Box Number  = 1114374144

Hour Box 5
Hour Box Number  = 1116602368



*********************************************************
*    Example 2:                                         *
*********************************************************

Hour box 1

Surface type percent coverage(1) = 0.000000
Surface type percent coverage(2) = 0.000000
Surface type percent coverage(3) = 0.000000
Surface type percent coverage(4) = 0.000000
Surface type percent coverage(5) = 0.000000
Surface type percent coverage(6) = 0.000000
Surface type percent coverage(7) = 0.000000
Surface type percent coverage(8) = 0.000000
Surface type percent coverage(9) = 0.000000
Surface type percent coverage(10) = 0.000000
Surface type percent coverage(11) = 0.000000
Surface type percent coverage(12) = 0.000000
Surface type percent coverage(13) = 0.000000
Surface type percent coverage(14) = 0.000000
Surface type percent coverage(15) = 0.000000
Surface type percent coverage(16) = 0.000000
Surface type percent coverage(17) = 100.000000
Surface type percent coverage(18) = 0.000000
Surface type percent coverage(19) = 0.000000
Surface type percent coverage(20) = 0.000000

Hour box 2

Surface type percent coverage(1) = 0.000000
Surface type percent coverage(2) = 0.000000
Surface type percent coverage(3) = 0.000000
Surface type percent coverage(4) = 0.000000
Surface type percent coverage(5) = 0.000000
Surface type percent coverage(6) = 0.000000
Surface type percent coverage(7) = 0.000000
Surface type percent coverage(8) = 0.000000
Surface type percent coverage(9) = 0.000000
Surface type percent coverage(10) = 0.000000
Surface type percent coverage(11) = 0.000000
Surface type percent coverage(12) = 0.000000
Surface type percent coverage(13) = 0.000000
Surface type percent coverage(14) = 0.000000
Surface type percent coverage(15) = 0.000000
Surface type percent coverage(16) = 0.000000
Surface type percent coverage(17) = 100.000000
Surface type percent coverage(18) = 0.000000
Surface type percent coverage(19) = 0.000000
Surface type percent coverage(20) = 0.000000

Hour box 3

Surface type percent coverage(1) = 0.000000
Surface type percent coverage(2) = 0.000000
Surface type percent coverage(3) = 0.000000
Surface type percent coverage(4) = 0.000000
Surface type percent coverage(5) = 0.000000
Surface type percent coverage(6) = 0.000000
Surface type percent coverage(7) = 0.000000
Surface type percent coverage(8) = 0.000000
Surface type percent coverage(9) = 0.000000
Surface type percent coverage(10) = 0.000000
Surface type percent coverage(11) = 0.000000
Surface type percent coverage(12) = 0.000000
Surface type percent coverage(13) = 0.000000
Surface type percent coverage(14) = 0.000000
Surface type percent coverage(15) = 0.000000
Surface type percent coverage(16) = 0.000000
Surface type percent coverage(17) = 100.000000
Surface type percent coverage(18) = 0.000000
Surface type percent coverage(19) = 0.000000
Surface type percent coverage(20) = 0.000000

Hour box 4

Surface type percent coverage(1) = 0.000000
Surface type percent coverage(2) = 0.000000
Surface type percent coverage(3) = 0.000000
Surface type percent coverage(4) = 0.000000
Surface type percent coverage(5) = 0.000000
Surface type percent coverage(6) = 0.000000
Surface type percent coverage(7) = 0.000000
Surface type percent coverage(8) = 0.000000
Surface type percent coverage(9) = 0.000000
Surface type percent coverage(10) = 0.000000
Surface type percent coverage(11) = 0.000000
Surface type percent coverage(12) = 0.000000
Surface type percent coverage(13) = 0.000000
Surface type percent coverage(14) = 0.000000
Surface type percent coverage(15) = 0.000000
Surface type percent coverage(16) = 0.000000
Surface type percent coverage(17) = 100.000000
Surface type percent coverage(18) = 0.000000
Surface type percent coverage(19) = 0.000000
Surface type percent coverage(20) = 0.000000

Hour box 5

Surface type percent coverage(1) = 0.000000
Surface type percent coverage(2) = 0.000000
Surface type percent coverage(3) = 0.000000
Surface type percent coverage(4) = 0.000000
Surface type percent coverage(5) = 0.000000
Surface type percent coverage(6) = 0.000000
Surface type percent coverage(7) = 0.000000
Surface type percent coverage(8) = 0.000000
Surface type percent coverage(9) = 0.000000
Surface type percent coverage(10) = 0.000000
Surface type percent coverage(11) = 0.000000
Surface type percent coverage(12) = 0.000000
Surface type percent coverage(13) = 0.000000
Surface type percent coverage(14) = 0.000000
Surface type percent coverage(15) = 0.000000
Surface type percent coverage(16) = 0.000000
Surface type percent coverage(17) = 100.000000
Surface type percent coverage(18) = 0.000000
Surface type percent coverage(19) = 0.000000
Surface type percent coverage(20) = 0.000000


*********************************************************
*    Example 3:                                         *
*********************************************************
Hour Box 1
Region Number  = 1190994432

Hour Box 2
Region Number  = 1190994432

Hour Box 3
Region Number  = 1190994432

Hour Box 4
Region Number  = 1190994432

Hour Box 5
Region Number  = 1190994432



*********************************************************
*    Example 4:                                         *
*********************************************************

Hour box 1

TOA SW TotalSky 

 Mean : 0.000000
 Std  : 0.000000
 NumObs : 0

Hour box 2

TOA SW TotalSky 

 Mean : 145.484406
 Std  : 30.704821
 NumObs : 13

Hour box 3

TOA SW TotalSky 

 Mean : 101.526581
 Std  : 9.838070
 NumObs : 14

Hour box 4

TOA SW TotalSky 

 Mean : 0.000000
 Std  : 0.000000
 NumObs : 0

Hour box 5

TOA SW TotalSky 

 Mean : 120.018448
 Std  : 27.811193
 NumObs : 23


*********************************************************
*    Example 5:                                         *
*********************************************************

Hour box 1

 Effective Pressure - low cloud 

 Mean : 862.241455
 Std  : 16.554298
 NumObs  : 32 

Hour box 2

 Effective Pressure - low cloud 

 Mean : 896.804810
 Std  : 24.872839
 NumObs  : 13 

Hour box 3

 Effective Pressure - low cloud 

 Mean : 886.145081
 Std  :  24.882843
 NumObs  : 14

Hour box 4

 Effective Pressure - low cloud 

 Mean : 860.844055
 Std  :  12.433323
 NumObs  : 33

Hour box 5

 Effective Pressure - low cloud 

 Mean : 776.498596
 Std  :  50.477188
 NumObs  : 17


*********************************************************
*    Example 6:                                         *
*********************************************************

ASSOCIATEDPLATFORMSHORTNAME = Terra          
     

8.0 Additional Information Regarding the CERES FSW Data.


     Last Updated: March 30, 2010
-----------------------------------------------------------------------------

