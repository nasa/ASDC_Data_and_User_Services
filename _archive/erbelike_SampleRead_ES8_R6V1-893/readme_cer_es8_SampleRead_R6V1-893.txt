             Clouds and Earth's Radiant Energy System (CERES)

                    CERES ES-8 R6V1-893 Readme File



1.0  Introduction

     This readme document provides information on the
     following Clouds and Earth's Radiant Energy System (CERES)
     ES-8 data sets:
          
     CER_ES8_TRMM_PFM_Edition2  ++  see NOTE

     CER_ES8_Terra_FM1_Edition1 ++  see NOTE
     CER_ES8_Terra_FM2_Edition1 ++  see NOTE
     CER_ES8_Terra-FM1_Edition1-CV
     CER_ES8_Terra-FM2_Edition1-CV
     CER_ES8_Terra_FM1_Edition2
     CER_ES8_Terra_FM2_Edition2
     CER_ES8_Terra_FM1_Edition3
     CER_ES8_Terra_FM2_Edition3

     CER_ES8_Aqua_FM3_Edition1
     CER_ES8_Aqua_FM4_Edition1
     CER_ES8_Aqua-FM1_Edition1-CV
     CER_ES8_Aqua-FM2_Edition1-CV
     CER_ES8_Aqua_FM3_Edition2
     CER_ES8_Aqua_FM4_Edition2
     CER_ES8_Aqua_FM3_Edition3
     CER_ES8_Aqua_FM4_Edition3

     CER_ES8_NPP-FM5_Edition1-CV

     NOTE: The Spectral Response Functions Vdata is only available on ES-8 files
           with a product configuration code (see Sec 3.5) greater than 021018.
           The sample read software reads the Spectral Response Functions Vdata.
           If a file with a product configuration code less than or equal to 
           021018 is read with this software package, Example 5 will fail with
           message: "Current Vref is bad!"

     This document includes the following sections:
        Section 2.0 - Data Set Description
        Section 3.0 - Format and Packaging
        Section 4.0 - Science Parameters Information
        Section 5.0 - Description of Sample Read Software
        Section 6.0 - Implementing Sample Read Software
        Section 7.0 - Sample Output
        Section 8.0 - Document history

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


    2.1 Instrumentation Description

        For a complete description of the CERES experiment and instruments,
        please refer to the documentation provided at the URL
        http://ceres.larc.nasa.gov

    2.2 Data Quality

        For a discussion regarding the quality of the ES-8 data sets,
        please refer to the corresponding Quality Summary Document
        available from the URL
        http://eosweb.larc.nasa.gov/PRODOCS/ceres/level2_es8_table.html.

    2.3 Other pertinent information
        The CERES ES-8 product is produced from software maintained
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

         2. Dr. Kory J. Priestley, Subsystem 2.0 Working Group Chair
    	    NASA Langley Research Center
            Mail Stop 420
            Hampton, VA 23681-2199
            E-mail:   kory.j.priestley@nasa.gov
            FAX:      (757) 864-7996
            Phone:    (757) 864-8147


 3.0 Format and Packaging

     This package includes a program, written in C, which will read 
     an ES-8 HDF file.  The examples provided in the main (ES8_readhdf.c) 
     program are written specifically for SDS(s) and Vdata(s) in ES-8 files.
     This read program is provided as a template to the user who will need 
     to modify it, if they need to read different ES-8 parameters.

     Information on the structure of the ES-8 file can be found
     in the Data Products Catalog (DPC) pages included in this 
     package.  This information is also available in the enclosed 
     dump file, CER_ES8_Sample_022019.20010601.dump.  Complete parameter 
     descriptions are available in the Collection/User's Guide, which 
     is on-line at URL: http://ceres.larc.nasa.gov/collect_guide.php.

    3.1 Sample Read Package Contents

        This package consists of the following files:

        1) CER_ES8_Sample_022019.20010601
        2) CER_ES8_Sample_022019.20010601.dump
        3) CER_ES8_Sample_022019.20010601.met
        4) DPC_ES8_R6V1.pdf
        5) ES8_HDFread.h
        6) ES8_readHDFfuncs.c
        7) ES8_readhdf.c
        8) compile_ES8_readhdf
        9) readme_cer_es8_SampleRead_R6V1-893 

        Descriptions for each file is as follows:
        1)'CER_ES8_Sample_022019.20010601' is a sample 5-footprint ES-8 file 
          which is used by the examples in 'ES8_readhdf.c'

        2)'CER_ES8_Sample_022019.20010601.dump' is an ASCII file containing a 
          description of the data in the sample ES-8, 
          'CER_ES8_Sample_022019.20010601'.
          If a value in the ES-8 granule is out-of-range or not valid, a 
          default value is inserted in its place.  Default values depend 
          on data type.  CERES default values are greater than or equal to:
          INT2    - 32767
          INT4    - 2147483647
          REAL4   - 3.402823E+38
          REAL8   - 1.797693134862315E+308

          The 'CER_ES8_Sample_022019.20010601.dump' file 
          includes a list of ALL the 
          data on the 5 footprint sample ES-8 (20 parameters/SDS's plus header 
          Vdata). The output data are the same as what the user would produce 
          using hdp, but for this example, the CERES Team modified the format
          of the output from the hdp command to make it easier to read.

        3)CER_ES8_Sample_022019.20010601.met is an ASCII metadata file that is 
          delivered with every data granule.  The file is written in 
          Object Descriptive Language (ODL) and contains information that 
          is associated with the origin, content, quality, and condition 
          of the data granule.  The metadata information contained in 
          the granule metadata file is also included as metadata on the 
          ES-8 HDF data file.  An example of a metadata parameter found 
          in the metadata file and on the ES-8 HDF file is LOCALGRANULEID, 
          which has a value set to the original granule file name.

        4)'DPC_ES8_R6V1.pdf' is a copy of the CERES Data Product Catalog 
          pages which describes the ES-8 product.

        5)'ES8_HDFread.h' is a header file for 'ES8_readhdf.c' 
          and the ES-8 HDF read functions.

        6)'ES8_readHDFfuncs.c' is a file consisting of C functions that
          are linked with 'ES8_readhdf.c' and call the HDF functions.

        7)'ES8_readhdf.c' is the main program which accesses the HDF reading
          functions.  It consists of six examples of accessing SDS and
          Vdata structures from the included sample ES-8 file.  Comments are 
          provided throughout the program.  This file must be modified if the
          user wishes to read SDSs or Vdatas other than those in the examples.

        8)'compile_ES8_readhdf' is a sample script to compile the C programs
          in a UNIX environment. The script must be modified for different
          platforms to properly compile the programs and correctly link
          the HDF libraries.

        9)'readme_cer_es8_SampleRead_R6V1-893' is this readme file describing
          the contents of the sample read package.

    3.2 Zip Information

        To read the files, first unzip them with Info-Zip software.
        Information about downloading and using this freeware is
        available at the URL:
        http://www.info-zip.org/pub/infozip/Info-ZIP.html

    3.3 HDF library

        HDF information, documentation, tutorials, libraries, etc.
        can be found on-line at the URL http://eosweb.larc.nasa.gov/HBDOCS/hdf.html

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

        The ES-8 data files are written in HDF as collections of SDS(s) and 
        Vdata(s).

    3.5 Breakdown of the File Naming Convention

        The ES-8 data granules adhere to the following CERES file
        naming convention:

                  CER_PID_SS_PS_CC.DataDate

        where: CER = the CERES experiment

               PID = the CERES product, e.g., ES8

                SS = the CERES sampling strategy, e.g.,
                     Terra-FM2, as defined by the CERES experiment.
                     For the ES-8, this indicates the satellite and
                     instruments that were the source of the input
                     data into the software system that produced
                     the ES-8 data.

                PS = the CERES production strategy, e.g.,
                     'Beta2', as defined by the CERES experiment.
                     'Edition2' production strategies indicate versions
                     of the product produced with algorithms that
                     are still being validated.  ES-8 data products
                     with BetaN production strategies may only be
                     available for a limited time and cannot be
                     be used to publish. ES-8 data products with
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
                     of the changes to the software, see the ES-8
                     Data Set Guide/Abstract document available
                     from the URL
                     http://eosweb.larc.nasa.gov/GUIDE/dataset_documents/cer_es8.html

          DataDate = the date of the data included in the
                     file.  For the ES-8, this date is in the format

                                 YYYYMMDD

                     where: YYYY = the four-digit data year,
                              MM = the two-digit month
                              DD = the two-digit day


 4.0 Science Parameters Information

     Refer to the ES-8 CERES Data Products Catalog pages included
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
     make up the ES-8 product and the number of data records contained
     on an actual ES-8 file are large, the sample software only 
     demonstrates reading a limited number of SDSs for the five
     footprints included in the sample ES-8 file.  The contents of
     the ES-8 footprint, along with a list of all the SDS(s) contained
     on the ES-8 product, are described in the Data Products Catalog
     pages discussed in Section 3.1.

     A note to the user:
     
     It is recommended that before modifiying the ES-8 read program to 
     read different ES-8 parameters, the user should first compile and 
     run the ES-8 read program using the sample ES-8 file as input and 
     compare the results that are created by the read program for 
     each SDS/parameter with the list of values for corresponding 
     parameters in the ES-8 sample dump file,
     'CER_ES8_Sample_022019.20010601.dump'.
     If this test is successful, then the user should modify the 
     the ES-8 read program or their own software to read the parameters 
     in which they are interested and run that software using the Sample 
     ES-8 as input. The values read should be compared to the values 
     of the corresponding parameters in the sample dump file.  If both 
     of these test are successful, then the user will be ready to use 
     their altered program with a full size ES-8 data file.


 6.0 Implementing Sample Read Software


    6.1 Compiling the Sample Read Software

       1. Download and install the HDF libraries from The HDF Group website

                  http://www.hdfgroup.org/release4/obtain.html

         This code uses version HDF4.1r5.  Other versions of the HDF
         libraries (i.e. HDF5) may not work with this code. The GNU gzip
         compression utility is used for some packages. If necessary, the
         software needed to uncompress the file can be obtained from
         one of the GNU mirror sites listed at the URL:

                http://www.gnu.org/order/ftp.html

       2. Edit the sample script 'compile_ES8_readhdf' to link the
          HDF libraries and compile the C programs.  Check the permissions
          on this file to make sure execute permission is set for the owner
          by typing at the command line (denoted by ">"):

           > ls -l compile_ES8_readhdf

          A line similar to the following will be displayed:

          -rwxr--r--  1 OWNER  GROUP 1450 Mar 6 14:49 compile_ES8_readhdf

          where OWNER and GROUP are the file owner and group information.
          The owner executable permission is the x in -rwxr--r--.

          To set the proper permissions, at the command line type:

           > chmod 744 compile_ES8_readhdf

       3. Execute the script 'compile_ES8_readhdf' to create the
          executable es8_rd by typing at the command line:

           > source compile_ES8_readhdf

          or, on some systems, type:

           > compile_ES8_readhdf


    6.2 How to Run the Sample Read Software

        Execute es8_rd by typing at the command line:

             > es8_rd CER_ES8_Sample_022019.20010601


 7.0 Sample Output

The following is the screen output generated from executing the es8_rd
program with the provided sample file.

> es8_rd CER_ES8_Sample_022019.20010601

Name of HDF file to be read:
< CER_ES8_Sample_022019.20010601


*********************************************************
*  Example 1:   ( ONLY PRINTING MAX OF 5 REGIONS)       *
*********************************************************

CERES SW unfiltered radiance for Region 1
   Scan (1) = 340282346638528860000000000000000000000.000000
   Scan (2) = 340282346638528860000000000000000000000.000000
   Scan (3) = 340282346638528860000000000000000000000.000000
   Scan (4) = 340282346638528860000000000000000000000.000000
   Scan (5) = 340282346638528860000000000000000000000.000000
   Scan (6) = 340282346638528860000000000000000000000.000000
   Scan (7) = 340282346638528860000000000000000000000.000000
   Scan (8) = 340282346638528860000000000000000000000.000000
   Scan (9) = 340282346638528860000000000000000000000.000000
   Scan (10) = 340282346638528860000000000000000000000.000000
   Scan (11) = 340282346638528860000000000000000000000.000000
   Scan (12) = 340282346638528860000000000000000000000.000000
   Scan (13) = 340282346638528860000000000000000000000.000000
   Scan (14) = 340282346638528860000000000000000000000.000000
   Scan (15) = 340282346638528860000000000000000000000.000000
   Scan (16) = 340282346638528860000000000000000000000.000000
   Scan (17) = 340282346638528860000000000000000000000.000000
   Scan (18) = 340282346638528860000000000000000000000.000000
   Scan (19) = 340282346638528860000000000000000000000.000000
   Scan (20) = 340282346638528860000000000000000000000.000000
   Scan (21) = 340282346638528860000000000000000000000.000000
   Scan (22) = 340282346638528860000000000000000000000.000000
   Scan (23) = 340282346638528860000000000000000000000.000000
   Scan (24) = 340282346638528860000000000000000000000.000000
   Scan (25) = 340282346638528860000000000000000000000.000000
   Scan (26) = 340282346638528860000000000000000000000.000000
   Scan (27) = 340282346638528860000000000000000000000.000000
   Scan (28) = 340282346638528860000000000000000000000.000000
   Scan (29) = 340282346638528860000000000000000000000.000000
   Scan (30) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (31) = 340282346638528860000000000000000000000.000000
   Scan (32) = 340282346638528860000000000000000000000.000000
   Scan (33) = 340282346638528860000000000000000000000.000000
   Scan (34) = 340282346638528860000000000000000000000.000000
   Scan (35) = 340282346638528860000000000000000000000.000000
   Scan (36) = 340282346638528860000000000000000000000.000000
   Scan (37) = 340282346638528860000000000000000000000.000000
   Scan (38) = 340282346638528860000000000000000000000.000000
   Scan (39) = 340282346638528860000000000000000000000.000000
   Scan (40) = 340282346638528860000000000000000000000.000000
   Scan (41) = 340282346638528860000000000000000000000.000000
   Scan (42) = 340282346638528860000000000000000000000.000000
   Scan (43) = 340282346638528860000000000000000000000.000000
   Scan (44) = 340282346638528860000000000000000000000.000000
   Scan (45) = 340282346638528860000000000000000000000.000000
   Scan (46) = 340282346638528860000000000000000000000.000000
   Scan (47) = 340282346638528860000000000000000000000.000000
   Scan (48) = 340282346638528860000000000000000000000.000000
   Scan (49) = 340282346638528860000000000000000000000.000000
   Scan (50) = 340282346638528860000000000000000000000.000000
   Scan (51) = 340282346638528860000000000000000000000.000000
   Scan (52) = 340282346638528860000000000000000000000.000000
   Scan (53) = 340282346638528860000000000000000000000.000000
   Scan (54) = 340282346638528860000000000000000000000.000000
   Scan (55) = 340282346638528860000000000000000000000.000000
   Scan (56) = 340282346638528860000000000000000000000.000000
   Scan (57) = 340282346638528860000000000000000000000.000000
   Scan (58) = 340282346638528860000000000000000000000.000000
   Scan (59) = 340282346638528860000000000000000000000.000000
   Scan (60) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (61) = 340282346638528860000000000000000000000.000000
   Scan (62) = 340282346638528860000000000000000000000.000000
   Scan (63) = 340282346638528860000000000000000000000.000000
   Scan (64) = 340282346638528860000000000000000000000.000000
   Scan (65) = 340282346638528860000000000000000000000.000000
   Scan (66) = 340282346638528860000000000000000000000.000000
   Scan (67) = 340282346638528860000000000000000000000.000000
   Scan (68) = 340282346638528860000000000000000000000.000000
   Scan (69) = 340282346638528860000000000000000000000.000000
   Scan (70) = 0.000000
   Scan (71) = 0.000000
   Scan (72) = 0.000000
   Scan (73) = 0.000000
   Scan (74) = 0.000000
   Scan (75) = 0.000000
   Scan (76) = 0.000000
   Scan (77) = 0.000000
   Scan (78) = 0.000000
   Scan (79) = 0.000000
   Scan (80) = 0.000000
   Scan (81) = 0.000000
   Scan (82) = 0.000000
   Scan (83) = 0.000000
   Scan (84) = 0.000000
   Scan (85) = 0.000000
   Scan (86) = 0.000000
   Scan (87) = 0.000000
   Scan (88) = 0.000000
   Scan (89) = 0.000000
   Scan (90) = 0.000000

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (91) = 0.000000
   Scan (92) = 0.000000
   Scan (93) = 0.000000
   Scan (94) = 0.000000
   Scan (95) = 0.000000
   Scan (96) = 0.000000
   Scan (97) = 0.000000
   Scan (98) = 0.000000
   Scan (99) = 0.000000
   Scan (100) = 0.000000
   Scan (101) = 0.000000
   Scan (102) = 0.000000
   Scan (103) = 0.000000
   Scan (104) = 0.000000
   Scan (105) = 0.000000
   Scan (106) = 0.000000
   Scan (107) = 0.000000
   Scan (108) = 0.000000
   Scan (109) = 0.000000
   Scan (110) = 0.000000
   Scan (111) = 0.000000
   Scan (112) = 0.000000
   Scan (113) = 0.000000
   Scan (114) = 0.000000
   Scan (115) = 0.000000
   Scan (116) = 0.000000
   Scan (117) = 0.000000
   Scan (118) = 0.000000
   Scan (119) = 0.000000
   Scan (120) = 0.000000

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (121) = 0.000000
   Scan (122) = 0.000000
   Scan (123) = 0.000000
   Scan (124) = 0.000000
   Scan (125) = 0.000000
   Scan (126) = 0.000000
   Scan (127) = 0.000000
   Scan (128) = 0.000000
   Scan (129) = 0.000000
   Scan (130) = 0.000000
   Scan (131) = 0.000000
   Scan (132) = 0.000000
   Scan (133) = 0.000000
   Scan (134) = 0.000000
   Scan (135) = 0.000000
   Scan (136) = 0.000000
   Scan (137) = 0.000000
   Scan (138) = 0.000000
   Scan (139) = 0.000000
   Scan (140) = 0.000000
   Scan (141) = 0.000000
   Scan (142) = 0.000000
   Scan (143) = 0.000000
   Scan (144) = 0.000000
   Scan (145) = 1.174902
   Scan (146) = 1.314437
   Scan (147) = 1.271605
   Scan (148) = 1.553189
   Scan (149) = 1.694108
   Scan (150) = 1.689943

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (151) = 1.684259
   Scan (152) = 1.825226
   Scan (153) = 1.959617
   Scan (154) = 2.095374
   Scan (155) = 2.230909
   Scan (156) = 2.511454
   Scan (157) = 2.650057
   Scan (158) = 2.646945
   Scan (159) = 2.642156
   Scan (160) = 2.782717
   Scan (161) = 2.781561
   Scan (162) = 2.633473
   Scan (163) = 2.485633
   Scan (164) = 2.338020
   Scan (165) = 2.192453
   Scan (166) = 2.333968
   Scan (167) = 2.477160
   Scan (168) = 2.474948
   Scan (169) = 2.618030
   Scan (170) = 2.474080
   Scan (171) = 2.615438
   Scan (172) = 2.469773
   Scan (173) = 2.612980
   Scan (174) = 2.754270
   Scan (175) = 3.038878
   Scan (176) = 3.036308
   Scan (177) = 2.892123
   Scan (178) = 2.889855
   Scan (179) = 2.742426
   Scan (180) = 2.883859

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (181) = 2.881704
   Scan (182) = 2.877809
   Scan (183) = 2.877499
   Scan (184) = 2.877203
   Scan (185) = 3.018621
   Scan (186) = 3.303351
   Scan (187) = 3.446082
   Scan (188) = 3.443514
   Scan (189) = 3.441017
   Scan (190) = 3.451093
   Scan (191) = 3.448725
   Scan (192) = 3.446412
   Scan (193) = 3.445858
   Scan (194) = 3.588470
   Scan (195) = 3.874094
   Scan (196) = 4.157783
   Scan (197) = 4.298068
   Scan (198) = 4.296784
   Scan (199) = 4.292281
   Scan (200) = 4.291195
   Scan (201) = 4.576520
   Scan (202) = 4.716785
   Scan (203) = 4.710499
   Scan (204) = 4.559374
   Scan (205) = 4.406612
   Scan (206) = 4.397017
   Scan (207) = 4.389125
   Scan (208) = 4.383069
   Scan (209) = 4.235771
   Scan (210) = 4.233771

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (211) = 4.374959
   Scan (212) = 4.370899
   Scan (213) = 4.231647
   Scan (214) = 4.362999
   Scan (215) = 4.651226
   Scan (216) = 5.080374
   Scan (217) = 5.511014
   Scan (218) = 5.794243
   Scan (219) = 6.220516
   Scan (220) = 6.652109
   Scan (221) = 7.539888
   Scan (222) = 8.540198
   Scan (223) = 9.684646
   Scan (224) = 10.396180
   Scan (225) = 11.104252
   Scan (226) = 11.524340
   Scan (227) = 11.661588
   Scan (228) = 12.087049
   Scan (229) = 12.800460
   Scan (230) = 13.515153
   Scan (231) = 14.226602
   Scan (232) = 15.365129
   Scan (233) = 16.647032
   Scan (234) = 16.490685
   Scan (235) = 14.899551
   Scan (236) = 14.604744
   Scan (237) = 16.176195
   Scan (238) = 17.749014
   Scan (239) = 18.321997
   Scan (240) = 17.743401

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (241) = 16.022049
   Scan (242) = 14.168962
   Scan (243) = 14.473610
   Scan (244) = 16.882099
   Scan (245) = 17.503647
   Scan (246) = 16.733791
   Scan (247) = 16.841139
   Scan (248) = 16.264139
   Scan (249) = 15.398413
   Scan (250) = 13.384885
   Scan (251) = 10.083461
   Scan (252) = 7.187734
   Scan (253) = 5.607489
   Scan (254) = 5.042212
   Scan (255) = 4.629266
   Scan (256) = 4.360572
   Scan (257) = 4.230804
   Scan (258) = 3.950617
   Scan (259) = 3.811600
   Scan (260) = 3.520022
   Scan (261) = 3.239828
   Scan (262) = 3.097850
   Scan (263) = 2.668528
   Scan (264) = 2.380821
   Scan (265) = 2.381468
   Scan (266) = 2.385158
   Scan (267) = 2.248489
   Scan (268) = 2.258363
   Scan (269) = 1.983940
   Scan (270) = 1.708925

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (271) = 1.470658
   Scan (272) = 0.000000
   Scan (273) = 0.000000
   Scan (274) = 0.000000
   Scan (275) = 0.000000
   Scan (276) = 0.000000
   Scan (277) = 0.000000
   Scan (278) = 0.000000
   Scan (279) = 0.000000
   Scan (280) = 0.000000
   Scan (281) = 0.000000
   Scan (282) = 0.000000
   Scan (283) = 0.000000
   Scan (284) = 0.000000
   Scan (285) = 0.000000
   Scan (286) = 0.000000
   Scan (287) = 0.000000
   Scan (288) = 0.000000
   Scan (289) = 0.000000
   Scan (290) = 0.000000
   Scan (291) = 0.000000
   Scan (292) = 0.000000
   Scan (293) = 0.000000
   Scan (294) = 0.000000
   Scan (295) = 0.000000
   Scan (296) = 0.000000
   Scan (297) = 0.000000
   Scan (298) = 0.000000
   Scan (299) = 0.000000
   Scan (300) = 0.000000

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (301) = 0.000000
   Scan (302) = 0.000000
   Scan (303) = 0.000000
   Scan (304) = 0.000000
   Scan (305) = 0.000000
   Scan (306) = 0.000000
   Scan (307) = 0.000000
   Scan (308) = 0.000000
   Scan (309) = 0.000000
   Scan (310) = 0.000000
   Scan (311) = 0.000000
   Scan (312) = 0.000000
   Scan (313) = 0.000000
   Scan (314) = 0.000000
   Scan (315) = 0.000000
   Scan (316) = 0.000000
   Scan (317) = 0.000000
   Scan (318) = 0.000000
   Scan (319) = 0.000000
   Scan (320) = 0.000000
   Scan (321) = 0.000000
   Scan (322) = 0.000000
   Scan (323) = 0.000000
   Scan (324) = 0.000000
   Scan (325) = 0.000000
   Scan (326) = 0.000000
   Scan (327) = 0.000000
   Scan (328) = 0.000000
   Scan (329) = 0.000000
   Scan (330) = 0.000000

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (331) = 0.000000
   Scan (332) = 0.000000
   Scan (333) = 0.000000
   Scan (334) = 0.000000
   Scan (335) = 0.000000
   Scan (336) = 0.000000
   Scan (337) = 0.000000
   Scan (338) = 0.000000
   Scan (339) = 0.000000
   Scan (340) = 0.000000
   Scan (341) = 0.000000
   Scan (342) = 0.000000
   Scan (343) = 0.000000
   Scan (344) = 0.000000
   Scan (345) = 0.000000
   Scan (346) = 0.000000
   Scan (347) = 0.000000
   Scan (348) = 0.000000
   Scan (349) = 0.000000
   Scan (350) = 0.000000
   Scan (351) = 0.000000
   Scan (352) = 0.000000
   Scan (353) = 0.000000
   Scan (354) = 0.000000
   Scan (355) = 0.000000
   Scan (356) = 0.000000
   Scan (357) = 0.000000
   Scan (358) = 0.000000
   Scan (359) = 0.000000
   Scan (360) = 0.000000

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (361) = 0.000000
   Scan (362) = 0.000000
   Scan (363) = 0.000000
   Scan (364) = 0.000000
   Scan (365) = 0.000000
   Scan (366) = 0.000000
   Scan (367) = 0.000000
   Scan (368) = 0.000000
   Scan (369) = 0.000000
   Scan (370) = 0.000000
   Scan (371) = 0.000000
   Scan (372) = 0.000000
   Scan (373) = 0.000000
   Scan (374) = 0.000000
   Scan (375) = 0.000000
   Scan (376) = 0.000000
   Scan (377) = 0.000000
   Scan (378) = 0.000000
   Scan (379) = 0.000000
   Scan (380) = 0.000000
   Scan (381) = 0.000000
   Scan (382) = 0.000000
   Scan (383) = 0.000000
   Scan (384) = 0.000000
   Scan (385) = 0.000000
   Scan (386) = 0.000000
   Scan (387) = 0.000000
   Scan (388) = 0.000000
   Scan (389) = 0.000000
   Scan (390) = 0.000000

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (391) = 0.000000
   Scan (392) = 0.000000
   Scan (393) = 0.000000
   Scan (394) = 0.000000
   Scan (395) = 0.000000
   Scan (396) = 0.000000
   Scan (397) = 0.000000
   Scan (398) = 0.000000
   Scan (399) = 0.000000
   Scan (400) = 0.000000
   Scan (401) = 0.000000
   Scan (402) = 0.000000
   Scan (403) = 0.000000
   Scan (404) = 0.000000
   Scan (405) = 0.000000
   Scan (406) = 0.000000
   Scan (407) = 0.000000
   Scan (408) = 0.000000
   Scan (409) = 0.000000
   Scan (410) = 0.000000
   Scan (411) = 0.000000
   Scan (412) = 0.000000
   Scan (413) = 0.000000
   Scan (414) = 0.000000
   Scan (415) = 0.000000
   Scan (416) = 0.000000
   Scan (417) = 0.000000
   Scan (418) = 0.000000
   Scan (419) = 0.000000
   Scan (420) = 0.000000

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (421) = 0.000000
   Scan (422) = 0.000000
   Scan (423) = 0.000000
   Scan (424) = 0.000000
   Scan (425) = 0.000000
   Scan (426) = 0.000000
   Scan (427) = 0.000000
   Scan (428) = 0.000000
   Scan (429) = 0.000000
   Scan (430) = 0.000000
   Scan (431) = 0.000000
   Scan (432) = 0.000000
   Scan (433) = 0.000000
   Scan (434) = 0.000000
   Scan (435) = 0.000000
   Scan (436) = 0.000000
   Scan (437) = 0.000000
   Scan (438) = 0.000000
   Scan (439) = 0.000000
   Scan (440) = 0.000000
   Scan (441) = 0.000000
   Scan (442) = 0.000000
   Scan (443) = 0.000000
   Scan (444) = 0.000000
   Scan (445) = 0.000000
   Scan (446) = 0.000000
   Scan (447) = 0.000000
   Scan (448) = 0.000000
   Scan (449) = 0.000000
   Scan (450) = 0.000000

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (451) = 0.000000
   Scan (452) = 0.000000
   Scan (453) = 0.000000
   Scan (454) = 0.000000
   Scan (455) = 1.039983
   Scan (456) = 1.036649
   Scan (457) = 1.033318
   Scan (458) = 1.029988
   Scan (459) = 1.023711
   Scan (460) = 1.165017
   Scan (461) = 1.306173
   Scan (462) = 1.448673
   Scan (463) = 1.444903
   Scan (464) = 1.584350
   Scan (465) = 1.580477
   Scan (466) = 1.705880
   Scan (467) = 1.846889
   Scan (468) = 1.984721
   Scan (469) = 2.122396
   Scan (470) = 1.973045
   Scan (471) = 1.967322
   Scan (472) = 1.963269
   Scan (473) = 2.244455
   Scan (474) = 2.381865
   Scan (475) = 2.520870
   Scan (476) = 2.516335
   Scan (477) = 2.653547
   Scan (478) = 2.648976
   Scan (479) = 2.789731
   Scan (480) = 2.785152

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (481) = 2.639023
   Scan (482) = 2.347881
   Scan (483) = 2.204216
   Scan (484) = 2.058925
   Scan (485) = 2.060841
   Scan (486) = 2.349570
   Scan (487) = 2.347552
   Scan (488) = 2.345559
   Scan (489) = 2.347053
   Scan (490) = 2.490253
   Scan (491) = 2.346404
   Scan (492) = 2.346179
   Scan (493) = 2.344290
   Scan (494) = 2.487519
   Scan (495) = 2.485489
   Scan (496) = 2.483484
   Scan (497) = 2.481503
   Scan (498) = 2.622986
   Scan (499) = 2.624155
   Scan (500) = 2.480249
   Scan (501) = 2.621811
   Scan (502) = 2.476332
   Scan (503) = 2.617932
   Scan (504) = 2.615932
   Scan (505) = 2.472108
   Scan (506) = 2.326874
   Scan (507) = 2.325253
   Scan (508) = 2.323623
   Scan (509) = 2.622838
   Scan (510) = 2.902460

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (511) = 2.753658
   Scan (512) = 2.456645
   Scan (513) = 2.308215
   Scan (514) = 2.446302
   Scan (515) = 2.734579
   Scan (516) = 2.882707
   Scan (517) = 3.173676
   Scan (518) = 3.320885
   Scan (519) = 3.606340
   Scan (520) = 3.605052
   Scan (521) = 3.745535
   Scan (522) = 3.739558
   Scan (523) = 3.590374
   Scan (524) = 3.439729
   Scan (525) = 3.289181
   Scan (526) = 3.142192
   Scan (527) = 3.281837
   Scan (528) = 3.424796
   Scan (529) = 3.424365
   Scan (530) = 3.422252
   Scan (531) = 3.418455
   Scan (532) = 3.414673
   Scan (533) = 3.702574
   Scan (534) = 3.990121
   Scan (535) = 4.420598
   Scan (536) = 4.993898
   Scan (537) = 5.281648
   Scan (538) = 5.857203
   Scan (539) = 6.285845
   Scan (540) = 6.735397

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (541) = 7.162425
   Scan (542) = 7.590595
   Scan (543) = 7.875436
   Scan (544) = 7.870162
   Scan (545) = 7.861167
   Scan (546) = 7.996750
   Scan (547) = 7.989323
   Scan (548) = 8.269625
   Scan (549) = 8.408217
   Scan (550) = 8.833040
   Scan (551) = 9.117412
   Scan (552) = 9.111242
   Scan (553) = 8.962221
   Scan (554) = 8.956683
   Scan (555) = 9.095890
   Scan (556) = 9.524232
   Scan (557) = 10.098171
   Scan (558) = 10.670216
   Scan (559) = 10.952845
   Scan (560) = 11.127892
   Scan (561) = 11.121650
   Scan (562) = 11.542959
   Scan (563) = 11.821661
   Scan (564) = 11.959177
   Scan (565) = 11.669335
   Scan (566) = 10.628569
   Scan (567) = 9.333246
   Scan (568) = 8.474786
   Scan (569) = 7.902218
   Scan (570) = 6.895691

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (571) = 5.722144
   Scan (572) = 4.573534
   Scan (573) = 4.000588
   Scan (574) = 3.429499
   Scan (575) = 3.296291
   Scan (576) = 3.010106
   Scan (577) = 2.862081
   Scan (578) = 2.575336
   Scan (579) = 2.422493
   Scan (580) = 2.571983
   Scan (581) = 2.430718
   Scan (582) = 2.431075
   Scan (583) = 2.572859
   Scan (584) = 2.425508
   Scan (585) = 2.566594
   Scan (586) = 2.569091
   Scan (587) = 2.431360
   Scan (588) = 2.296747
   Scan (589) = 2.016722
   Scan (590) = 1.605799
   Scan (591) = 1.472431
   Scan (592) = 1.627470
   Scan (593) = 1.636745
   Scan (594) = 1.645073
   Scan (595) = 0.000000
   Scan (596) = 0.000000
   Scan (597) = 0.000000
   Scan (598) = 0.000000
   Scan (599) = 0.000000
   Scan (600) = 0.000000

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (601) = 0.000000
   Scan (602) = 0.000000
   Scan (603) = 0.000000
   Scan (604) = 0.000000
   Scan (605) = 0.000000
   Scan (606) = 0.000000
   Scan (607) = 0.000000
   Scan (608) = 0.000000
   Scan (609) = 0.000000
   Scan (610) = 0.000000
   Scan (611) = 0.000000
   Scan (612) = 0.000000
   Scan (613) = 0.000000
   Scan (614) = 0.000000
   Scan (615) = 0.000000
   Scan (616) = 0.000000
   Scan (617) = 0.000000
   Scan (618) = 0.000000
   Scan (619) = 0.000000
   Scan (620) = 0.000000
   Scan (621) = 0.000000
   Scan (622) = 0.000000
   Scan (623) = 0.000000
   Scan (624) = 0.000000
   Scan (625) = 0.000000
   Scan (626) = 0.000000
   Scan (627) = 0.000000
   Scan (628) = 0.000000
   Scan (629) = 0.000000
   Scan (630) = 0.000000

CERES SW unfiltered radiance for Region 1 (Continued)
   Scan (631) = 0.000000
   Scan (632) = 0.000000
   Scan (633) = 0.000000
   Scan (634) = 0.000000
   Scan (635) = 0.000000
   Scan (636) = 0.000000
   Scan (637) = 0.000000
   Scan (638) = 0.000000
   Scan (639) = 0.000000
   Scan (640) = 0.000000
   Scan (641) = 0.000000
   Scan (642) = 0.000000
   Scan (643) = 0.000000
   Scan (644) = 0.000000
   Scan (645) = 0.000000
   Scan (646) = 0.000000
   Scan (647) = 0.000000
   Scan (648) = 0.000000
   Scan (649) = 0.000000
   Scan (650) = 0.000000
   Scan (651) = 0.000000
   Scan (652) = 0.000000
   Scan (653) = 0.000000
   Scan (654) = 0.000000
   Scan (655) = 0.000000
   Scan (656) = 0.000000
   Scan (657) = 0.000000
   Scan (658) = 0.000000
   Scan (659) = 0.000000
   Scan (660) = 0.000000


CERES SW unfiltered radiance for Region 2
   Scan (1) = 0.000000
   Scan (2) = 0.000000
   Scan (3) = 0.000000
   Scan (4) = 0.000000
   Scan (5) = 0.000000
   Scan (6) = 0.000000
   Scan (7) = 0.000000
   Scan (8) = 0.000000
   Scan (9) = 0.000000
   Scan (10) = 0.000000
   Scan (11) = 0.000000
   Scan (12) = 0.000000
   Scan (13) = 0.000000
   Scan (14) = 0.000000
   Scan (15) = 0.000000
   Scan (16) = 0.000000
   Scan (17) = 0.000000
   Scan (18) = 0.000000
   Scan (19) = 0.000000
   Scan (20) = 0.000000
   Scan (21) = 0.000000
   Scan (22) = 0.000000
   Scan (23) = 0.000000
   Scan (24) = 0.000000
   Scan (25) = 0.000000
   Scan (26) = 0.000000
   Scan (27) = 0.000000
   Scan (28) = 0.000000
   Scan (29) = 0.000000
   Scan (30) = 0.000000

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (31) = 0.000000
   Scan (32) = 0.000000
   Scan (33) = 0.000000
   Scan (34) = 0.000000
   Scan (35) = 0.000000
   Scan (36) = 0.000000
   Scan (37) = 0.000000
   Scan (38) = 0.000000
   Scan (39) = 0.000000
   Scan (40) = 0.000000
   Scan (41) = 0.000000
   Scan (42) = 0.000000
   Scan (43) = 0.000000
   Scan (44) = 0.000000
   Scan (45) = 0.000000
   Scan (46) = 0.000000
   Scan (47) = 0.000000
   Scan (48) = 0.000000
   Scan (49) = 0.000000
   Scan (50) = 0.000000
   Scan (51) = 0.000000
   Scan (52) = 0.000000
   Scan (53) = 0.000000
   Scan (54) = 0.000000
   Scan (55) = 0.000000
   Scan (56) = 0.000000
   Scan (57) = 0.000000
   Scan (58) = 0.000000
   Scan (59) = 0.000000
   Scan (60) = 0.000000

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (61) = 0.000000
   Scan (62) = 0.000000
   Scan (63) = 0.000000
   Scan (64) = 0.000000
   Scan (65) = 0.000000
   Scan (66) = 0.000000
   Scan (67) = 0.000000
   Scan (68) = 0.000000
   Scan (69) = 0.000000
   Scan (70) = 0.000000
   Scan (71) = 0.000000
   Scan (72) = 0.000000
   Scan (73) = 0.000000
   Scan (74) = 0.000000
   Scan (75) = 0.000000
   Scan (76) = 0.000000
   Scan (77) = 0.000000
   Scan (78) = 0.000000
   Scan (79) = 0.000000
   Scan (80) = 0.000000
   Scan (81) = 0.000000
   Scan (82) = 0.000000
   Scan (83) = 0.000000
   Scan (84) = 0.000000
   Scan (85) = 0.000000
   Scan (86) = 0.000000
   Scan (87) = 0.000000
   Scan (88) = 0.000000
   Scan (89) = 0.000000
   Scan (90) = 0.000000

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (91) = 0.000000
   Scan (92) = 2.063805
   Scan (93) = 2.053887
   Scan (94) = 2.337553
   Scan (95) = 2.630634
   Scan (96) = 2.680507
   Scan (97) = 2.821320
   Scan (98) = 2.817337
   Scan (99) = 2.679158
   Scan (100) = 2.544855
   Scan (101) = 2.693722
   Scan (102) = 3.128311
   Scan (103) = 3.560908
   Scan (104) = 3.991787
   Scan (105) = 3.990416
   Scan (106) = 3.702637
   Scan (107) = 3.271169
   Scan (108) = 2.981723
   Scan (109) = 2.691720
   Scan (110) = 2.544214
   Scan (111) = 2.541977
   Scan (112) = 2.686903
   Scan (113) = 2.833328
   Scan (114) = 2.833473
   Scan (115) = 2.830988
   Scan (116) = 2.793717
   Scan (117) = 2.644569
   Scan (118) = 2.495349
   Scan (119) = 2.491906
   Scan (120) = 2.488378

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (121) = 2.628403
   Scan (122) = 2.626904
   Scan (123) = 2.624230
   Scan (124) = 2.623895
   Scan (125) = 2.477582
   Scan (126) = 2.476237
   Scan (127) = 2.473690
   Scan (128) = 2.327483
   Scan (129) = 2.326260
   Scan (130) = 2.180167
   Scan (131) = 2.176580
   Scan (132) = 2.030552
   Scan (133) = 2.173162
   Scan (134) = 2.315564
   Scan (135) = 2.314148
   Scan (136) = 2.453781
   Scan (137) = 2.446855
   Scan (138) = 2.587524
   Scan (139) = 2.584384
   Scan (140) = 2.434767
   Scan (141) = 2.137046
   Scan (142) = 1.843928
   Scan (143) = 1.696219
   Scan (144) = 1.839040
   Scan (145) = 1.981635
   Scan (146) = 2.124024
   Scan (147) = 2.253276
   Scan (148) = 2.250070
   Scan (149) = 2.390720
   Scan (150) = 2.531196

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (151) = 2.669846
   Scan (152) = 2.664431
   Scan (153) = 2.662439
   Scan (154) = 2.657047
   Scan (155) = 2.655131
   Scan (156) = 2.793616
   Scan (157) = 2.789824
   Scan (158) = 2.786059
   Scan (159) = 2.784122
   Scan (160) = 2.780415
   Scan (161) = 2.490808
   Scan (162) = 2.343614
   Scan (163) = 2.344088
   Scan (164) = 2.198863
   Scan (165) = 2.345109
   Scan (166) = 2.347231
   Scan (167) = 2.349278
   Scan (168) = 2.349558
   Scan (169) = 2.349813
   Scan (170) = 2.351704
   Scan (171) = 2.351893
   Scan (172) = 2.495924
   Scan (173) = 2.350421
   Scan (174) = 2.061229
   Scan (175) = 1.916231
   Scan (176) = 1.913597
   Scan (177) = 1.621522
   Scan (178) = 1.615690
   Scan (179) = 1.753513
   Scan (180) = 1.748911

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (181) = 1.603964
   Scan (182) = 1.750416
   Scan (183) = 1.900017
   Scan (184) = 2.047526
   Scan (185) = 2.049238
   Scan (186) = 2.052469
   Scan (187) = 2.197889
   Scan (188) = 2.196164
   Scan (189) = 2.197507
   Scan (190) = 2.210458
   Scan (191) = 2.210277
   Scan (192) = 2.353719
   Scan (193) = 2.354816
   Scan (194) = 2.354453
   Scan (195) = 2.352673
   Scan (196) = 2.349478
   Scan (197) = 2.489915
   Scan (198) = 2.630193
   Scan (199) = 2.915448
   Scan (200) = 3.200427
   Scan (201) = 3.342966
   Scan (202) = 3.483964
   Scan (203) = 3.624894
   Scan (204) = 3.620632
   Scan (205) = 3.616463
   Scan (206) = 3.615357
   Scan (207) = 3.615777
   Scan (208) = 3.469698
   Scan (209) = 3.322414
   Scan (210) = 3.175339

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (211) = 3.318703
   Scan (212) = 3.173149
   Scan (213) = 3.026329
   Scan (214) = 3.024804
   Scan (215) = 3.171278
   Scan (216) = 3.462475
   Scan (217) = 3.754412
   Scan (218) = 3.758341
   Scan (219) = 3.760804
   Scan (220) = 3.758616
   Scan (221) = 3.932209
   Scan (222) = 4.078510
   Scan (223) = 4.075131
   Scan (224) = 3.782084
   Scan (225) = 3.776839
   Scan (226) = 3.776119
   Scan (227) = 3.919762
   Scan (228) = 3.917791
   Scan (229) = 3.916946
   Scan (230) = 3.916117
   Scan (231) = 4.057503
   Scan (232) = 4.055495
   Scan (233) = 3.909199
   Scan (234) = 4.050689
   Scan (235) = 4.193111
   Scan (236) = 4.191065
   Scan (237) = 4.189062
   Scan (238) = 4.188154
   Scan (239) = 4.186226
   Scan (240) = 4.184331

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (241) = 4.407609
   Scan (242) = 4.261427
   Scan (243) = 4.258553
   Scan (244) = 4.397740
   Scan (245) = 4.395836
   Scan (246) = 4.253020
   Scan (247) = 4.026005
   Scan (248) = 3.739062
   Scan (249) = 3.594638
   Scan (250) = 3.593589
   Scan (251) = 3.590340
   Scan (252) = 3.408991
   Scan (253) = 3.109344
   Scan (254) = 2.959543
   Scan (255) = 2.954685
   Scan (256) = 2.953997
   Scan (257) = 2.667385
   Scan (258) = 2.523255
   Scan (259) = 2.089079
   Scan (260) = 2.066740
   Scan (261) = 1.913253
   Scan (262) = 1.762669
   Scan (263) = 1.765971
   Scan (264) = 2.051855
   Scan (265) = 2.046144
   Scan (266) = 2.184080
   Scan (267) = 2.614745
   Scan (268) = 2.760632
   Scan (269) = 2.762414
   Scan (270) = 2.332541

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (271) = 2.208748
   Scan (272) = 2.362231
   Scan (273) = 2.658372
   Scan (274) = 2.953404
   Scan (275) = 3.243775
   Scan (276) = 3.389951
   Scan (277) = 3.684701
   Scan (278) = 4.981040
   Scan (279) = 6.146585
   Scan (280) = 5.563041
   Scan (281) = 4.835770
   Scan (282) = 4.970111
   Scan (283) = 5.536566
   Scan (284) = 6.178580
   Scan (285) = 6.891842
   Scan (286) = 7.896393
   Scan (287) = 9.183055
   Scan (288) = 10.459656
   Scan (289) = 340282346638528860000000000000000000000.000000
   Scan (290) = 340282346638528860000000000000000000000.000000
   Scan (291) = 340282346638528860000000000000000000000.000000
   Scan (292) = 340282346638528860000000000000000000000.000000
   Scan (293) = 340282346638528860000000000000000000000.000000
   Scan (294) = 340282346638528860000000000000000000000.000000
   Scan (295) = 340282346638528860000000000000000000000.000000
   Scan (296) = 340282346638528860000000000000000000000.000000
   Scan (297) = 340282346638528860000000000000000000000.000000
   Scan (298) = 340282346638528860000000000000000000000.000000
   Scan (299) = 340282346638528860000000000000000000000.000000
   Scan (300) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (301) = 340282346638528860000000000000000000000.000000
   Scan (302) = 340282346638528860000000000000000000000.000000
   Scan (303) = 340282346638528860000000000000000000000.000000
   Scan (304) = 340282346638528860000000000000000000000.000000
   Scan (305) = 340282346638528860000000000000000000000.000000
   Scan (306) = 340282346638528860000000000000000000000.000000
   Scan (307) = 340282346638528860000000000000000000000.000000
   Scan (308) = 340282346638528860000000000000000000000.000000
   Scan (309) = 340282346638528860000000000000000000000.000000
   Scan (310) = 340282346638528860000000000000000000000.000000
   Scan (311) = 340282346638528860000000000000000000000.000000
   Scan (312) = 340282346638528860000000000000000000000.000000
   Scan (313) = 340282346638528860000000000000000000000.000000
   Scan (314) = 340282346638528860000000000000000000000.000000
   Scan (315) = 340282346638528860000000000000000000000.000000
   Scan (316) = 340282346638528860000000000000000000000.000000
   Scan (317) = 340282346638528860000000000000000000000.000000
   Scan (318) = 340282346638528860000000000000000000000.000000
   Scan (319) = 340282346638528860000000000000000000000.000000
   Scan (320) = 340282346638528860000000000000000000000.000000
   Scan (321) = 340282346638528860000000000000000000000.000000
   Scan (322) = 340282346638528860000000000000000000000.000000
   Scan (323) = 340282346638528860000000000000000000000.000000
   Scan (324) = 340282346638528860000000000000000000000.000000
   Scan (325) = 340282346638528860000000000000000000000.000000
   Scan (326) = 340282346638528860000000000000000000000.000000
   Scan (327) = 340282346638528860000000000000000000000.000000
   Scan (328) = 340282346638528860000000000000000000000.000000
   Scan (329) = 340282346638528860000000000000000000000.000000
   Scan (330) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (331) = 340282346638528860000000000000000000000.000000
   Scan (332) = 340282346638528860000000000000000000000.000000
   Scan (333) = 340282346638528860000000000000000000000.000000
   Scan (334) = 340282346638528860000000000000000000000.000000
   Scan (335) = 340282346638528860000000000000000000000.000000
   Scan (336) = 340282346638528860000000000000000000000.000000
   Scan (337) = 340282346638528860000000000000000000000.000000
   Scan (338) = 340282346638528860000000000000000000000.000000
   Scan (339) = 340282346638528860000000000000000000000.000000
   Scan (340) = 340282346638528860000000000000000000000.000000
   Scan (341) = 340282346638528860000000000000000000000.000000
   Scan (342) = 340282346638528860000000000000000000000.000000
   Scan (343) = 340282346638528860000000000000000000000.000000
   Scan (344) = 340282346638528860000000000000000000000.000000
   Scan (345) = 340282346638528860000000000000000000000.000000
   Scan (346) = 340282346638528860000000000000000000000.000000
   Scan (347) = 340282346638528860000000000000000000000.000000
   Scan (348) = 340282346638528860000000000000000000000.000000
   Scan (349) = 340282346638528860000000000000000000000.000000
   Scan (350) = 340282346638528860000000000000000000000.000000
   Scan (351) = 340282346638528860000000000000000000000.000000
   Scan (352) = 340282346638528860000000000000000000000.000000
   Scan (353) = 340282346638528860000000000000000000000.000000
   Scan (354) = 340282346638528860000000000000000000000.000000
   Scan (355) = 340282346638528860000000000000000000000.000000
   Scan (356) = 340282346638528860000000000000000000000.000000
   Scan (357) = 340282346638528860000000000000000000000.000000
   Scan (358) = 340282346638528860000000000000000000000.000000
   Scan (359) = 340282346638528860000000000000000000000.000000
   Scan (360) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (361) = 340282346638528860000000000000000000000.000000
   Scan (362) = 340282346638528860000000000000000000000.000000
   Scan (363) = 340282346638528860000000000000000000000.000000
   Scan (364) = 340282346638528860000000000000000000000.000000
   Scan (365) = 340282346638528860000000000000000000000.000000
   Scan (366) = 340282346638528860000000000000000000000.000000
   Scan (367) = 340282346638528860000000000000000000000.000000
   Scan (368) = 340282346638528860000000000000000000000.000000
   Scan (369) = 340282346638528860000000000000000000000.000000
   Scan (370) = 340282346638528860000000000000000000000.000000
   Scan (371) = 340282346638528860000000000000000000000.000000
   Scan (372) = 340282346638528860000000000000000000000.000000
   Scan (373) = 340282346638528860000000000000000000000.000000
   Scan (374) = 340282346638528860000000000000000000000.000000
   Scan (375) = 340282346638528860000000000000000000000.000000
   Scan (376) = 340282346638528860000000000000000000000.000000
   Scan (377) = 340282346638528860000000000000000000000.000000
   Scan (378) = 340282346638528860000000000000000000000.000000
   Scan (379) = 340282346638528860000000000000000000000.000000
   Scan (380) = 340282346638528860000000000000000000000.000000
   Scan (381) = 340282346638528860000000000000000000000.000000
   Scan (382) = 340282346638528860000000000000000000000.000000
   Scan (383) = 340282346638528860000000000000000000000.000000
   Scan (384) = 340282346638528860000000000000000000000.000000
   Scan (385) = 340282346638528860000000000000000000000.000000
   Scan (386) = 340282346638528860000000000000000000000.000000
   Scan (387) = 340282346638528860000000000000000000000.000000
   Scan (388) = 340282346638528860000000000000000000000.000000
   Scan (389) = 26.197927
   Scan (390) = 24.728001

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (391) = 23.843882
   Scan (392) = 22.673630
   Scan (393) = 22.512634
   Scan (394) = 23.791414
   Scan (395) = 24.486847
   Scan (396) = 23.015078
   Scan (397) = 20.683264
   Scan (398) = 19.666050
   Scan (399) = 19.240379
   Scan (400) = 18.964123
   Scan (401) = 18.250814
   Scan (402) = 16.955812
   Scan (403) = 15.516636
   Scan (404) = 14.657290
   Scan (405) = 14.232599
   Scan (406) = 13.380024
   Scan (407) = 12.240327
   Scan (408) = 11.240897
   Scan (409) = 10.666310
   Scan (410) = 10.804032
   Scan (411) = 10.946144
   Scan (412) = 10.664849
   Scan (413) = 9.954004
   Scan (414) = 9.525026
   Scan (415) = 9.045257
   Scan (416) = 8.613562
   Scan (417) = 8.333246
   Scan (418) = 8.345657
   Scan (419) = 8.070127
   Scan (420) = 7.939948

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (421) = 7.809653
   Scan (422) = 7.245663
   Scan (423) = 6.535264
   Scan (424) = 6.108427
   Scan (425) = 5.965621
   Scan (426) = 5.823433
   Scan (427) = 5.394287
   Scan (428) = 5.105337
   Scan (429) = 5.244717
   Scan (430) = 5.531360
   Scan (431) = 6.106504
   Scan (432) = 7.259249
   Scan (433) = 8.268310
   Scan (434) = 8.414051
   Scan (435) = 7.678652
   Scan (436) = 6.815848
   Scan (437) = 6.384371
   Scan (438) = 6.383535
   Scan (439) = 6.236787
   Scan (440) = 6.089939
   Scan (441) = 5.511831
   Scan (442) = 5.076702
   Scan (443) = 4.785368
   Scan (444) = 4.643947
   Scan (445) = 4.933532
   Scan (446) = 4.935168
   Scan (447) = 4.791735
   Scan (448) = 4.933284
   Scan (449) = 5.076938
   Scan (450) = 5.075365

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (451) = 5.073673
   Scan (452) = 4.639507
   Scan (453) = 3.917054
   Scan (454) = 3.479785
   Scan (455) = 3.190530
   Scan (456) = 2.902820
   Scan (457) = 2.758860
   Scan (458) = 2.758464
   Scan (459) = 2.614192
   Scan (460) = 2.608875
   Scan (461) = 2.606337
   Scan (462) = 2.603553
   Scan (463) = 2.456908
   Scan (464) = 2.453847
   Scan (465) = 2.739553
   Scan (466) = 2.725431
   Scan (467) = 2.867143
   Scan (468) = 3.010313
   Scan (469) = 3.440996
   Scan (470) = 3.581653
   Scan (471) = 3.432612
   Scan (472) = 3.285481
   Scan (473) = 2.850755
   Scan (474) = 2.706050
   Scan (475) = 2.847384
   Scan (476) = 3.132342
   Scan (477) = 2.988948
   Scan (478) = 2.843883
   Scan (479) = 2.555088
   Scan (480) = 2.266591

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (481) = 2.124035
   Scan (482) = 1.837680
   Scan (483) = 1.984889
   Scan (484) = 2.130027
   Scan (485) = 2.129302
   Scan (486) = 2.131944
   Scan (487) = 1.992260
   Scan (488) = 1.849294
   Scan (489) = 1.848640
   Scan (490) = 1.844599
   Scan (491) = 1.693133
   Scan (492) = 1.688944
   Scan (493) = 1.546078
   Scan (494) = 1.692739
   Scan (495) = 1.696914
   Scan (496) = 1.697614
   Scan (497) = 1.557637
   Scan (498) = 1.703787
   Scan (499) = 1.849653
   Scan (500) = 1.704474
   Scan (501) = 1.704839
   Scan (502) = 1.850522
   Scan (503) = 1.853544
   Scan (504) = 1.853573
   Scan (505) = 1.709715
   Scan (506) = 1.709857
   Scan (507) = 1.852423
   Scan (508) = 1.848104
   Scan (509) = 1.999617
   Scan (510) = 0.000000

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (511) = 0.000000
   Scan (512) = 0.000000
   Scan (513) = 0.000000
   Scan (514) = 0.000000
   Scan (515) = 0.000000
   Scan (516) = 0.000000
   Scan (517) = 0.000000
   Scan (518) = 0.000000
   Scan (519) = 0.000000
   Scan (520) = 0.000000
   Scan (521) = 0.000000
   Scan (522) = 0.000000
   Scan (523) = 0.000000
   Scan (524) = 0.000000
   Scan (525) = 0.000000
   Scan (526) = 0.000000
   Scan (527) = 0.000000
   Scan (528) = 0.000000
   Scan (529) = 0.000000
   Scan (530) = 0.000000
   Scan (531) = 0.000000
   Scan (532) = 0.000000
   Scan (533) = 0.000000
   Scan (534) = 0.000000
   Scan (535) = 0.000000
   Scan (536) = 0.000000
   Scan (537) = 0.000000
   Scan (538) = 0.000000
   Scan (539) = 0.000000
   Scan (540) = 0.000000

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (541) = 0.000000
   Scan (542) = 0.000000
   Scan (543) = 0.000000
   Scan (544) = 0.000000
   Scan (545) = 0.000000
   Scan (546) = 0.000000
   Scan (547) = 0.000000
   Scan (548) = 0.000000
   Scan (549) = 0.000000
   Scan (550) = 0.000000
   Scan (551) = 0.000000
   Scan (552) = 0.000000
   Scan (553) = 0.000000
   Scan (554) = 0.000000
   Scan (555) = 0.000000
   Scan (556) = 0.000000
   Scan (557) = 0.000000
   Scan (558) = 0.000000
   Scan (559) = 0.000000
   Scan (560) = 0.000000
   Scan (561) = 0.000000
   Scan (562) = 0.000000
   Scan (563) = 0.000000
   Scan (564) = 0.000000
   Scan (565) = 0.000000
   Scan (566) = 0.000000
   Scan (567) = 0.000000
   Scan (568) = 0.000000
   Scan (569) = 0.000000
   Scan (570) = 0.000000

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (571) = 0.000000
   Scan (572) = 0.000000
   Scan (573) = 0.000000
   Scan (574) = 0.000000
   Scan (575) = 0.000000
   Scan (576) = 0.000000
   Scan (577) = 0.000000
   Scan (578) = 0.000000
   Scan (579) = 0.000000
   Scan (580) = 1.593034
   Scan (581) = 1.582224
   Scan (582) = 1.572524
   Scan (583) = 1.716367
   Scan (584) = 1.998989
   Scan (585) = 2.277832
   Scan (586) = 2.556152
   Scan (587) = 2.983315
   Scan (588) = 3.129461
   Scan (589) = 2.987683
   Scan (590) = 2.859652
   Scan (591) = 3.441387
   Scan (592) = 4.310853
   Scan (593) = 4.743631
   Scan (594) = 5.170460
   Scan (595) = 6.605909
   Scan (596) = 8.042898
   Scan (597) = 8.180740
   Scan (598) = 8.049516
   Scan (599) = 8.765600
   Scan (600) = 10.335449

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (601) = 11.319515
   Scan (602) = 12.159631
   Scan (603) = 15.468081
   Scan (604) = 19.614191
   Scan (605) = 22.178818
   Scan (606) = 24.311432
   Scan (607) = 27.476744
   Scan (608) = 340282346638528860000000000000000000000.000000
   Scan (609) = 340282346638528860000000000000000000000.000000
   Scan (610) = 340282346638528860000000000000000000000.000000
   Scan (611) = 340282346638528860000000000000000000000.000000
   Scan (612) = 340282346638528860000000000000000000000.000000
   Scan (613) = 340282346638528860000000000000000000000.000000
   Scan (614) = 340282346638528860000000000000000000000.000000
   Scan (615) = 340282346638528860000000000000000000000.000000
   Scan (616) = 340282346638528860000000000000000000000.000000
   Scan (617) = 340282346638528860000000000000000000000.000000
   Scan (618) = 340282346638528860000000000000000000000.000000
   Scan (619) = 340282346638528860000000000000000000000.000000
   Scan (620) = 340282346638528860000000000000000000000.000000
   Scan (621) = 340282346638528860000000000000000000000.000000
   Scan (622) = 340282346638528860000000000000000000000.000000
   Scan (623) = 340282346638528860000000000000000000000.000000
   Scan (624) = 340282346638528860000000000000000000000.000000
   Scan (625) = 340282346638528860000000000000000000000.000000
   Scan (626) = 340282346638528860000000000000000000000.000000
   Scan (627) = 340282346638528860000000000000000000000.000000
   Scan (628) = 340282346638528860000000000000000000000.000000
   Scan (629) = 340282346638528860000000000000000000000.000000
   Scan (630) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 2 (Continued)
   Scan (631) = 340282346638528860000000000000000000000.000000
   Scan (632) = 340282346638528860000000000000000000000.000000
   Scan (633) = 340282346638528860000000000000000000000.000000
   Scan (634) = 340282346638528860000000000000000000000.000000
   Scan (635) = 340282346638528860000000000000000000000.000000
   Scan (636) = 340282346638528860000000000000000000000.000000
   Scan (637) = 340282346638528860000000000000000000000.000000
   Scan (638) = 340282346638528860000000000000000000000.000000
   Scan (639) = 340282346638528860000000000000000000000.000000
   Scan (640) = 340282346638528860000000000000000000000.000000
   Scan (641) = 340282346638528860000000000000000000000.000000
   Scan (642) = 340282346638528860000000000000000000000.000000
   Scan (643) = 340282346638528860000000000000000000000.000000
   Scan (644) = 340282346638528860000000000000000000000.000000
   Scan (645) = 340282346638528860000000000000000000000.000000
   Scan (646) = 340282346638528860000000000000000000000.000000
   Scan (647) = 340282346638528860000000000000000000000.000000
   Scan (648) = 340282346638528860000000000000000000000.000000
   Scan (649) = 340282346638528860000000000000000000000.000000
   Scan (650) = 340282346638528860000000000000000000000.000000
   Scan (651) = 340282346638528860000000000000000000000.000000
   Scan (652) = 340282346638528860000000000000000000000.000000
   Scan (653) = 340282346638528860000000000000000000000.000000
   Scan (654) = 340282346638528860000000000000000000000.000000
   Scan (655) = 340282346638528860000000000000000000000.000000
   Scan (656) = 340282346638528860000000000000000000000.000000
   Scan (657) = 340282346638528860000000000000000000000.000000
   Scan (658) = 340282346638528860000000000000000000000.000000
   Scan (659) = 340282346638528860000000000000000000000.000000
   Scan (660) = 340282346638528860000000000000000000000.000000


CERES SW unfiltered radiance for Region 3
   Scan (1) = 340282346638528860000000000000000000000.000000
   Scan (2) = 340282346638528860000000000000000000000.000000
   Scan (3) = 340282346638528860000000000000000000000.000000
   Scan (4) = 340282346638528860000000000000000000000.000000
   Scan (5) = 340282346638528860000000000000000000000.000000
   Scan (6) = 340282346638528860000000000000000000000.000000
   Scan (7) = 340282346638528860000000000000000000000.000000
   Scan (8) = 340282346638528860000000000000000000000.000000
   Scan (9) = 340282346638528860000000000000000000000.000000
   Scan (10) = 340282346638528860000000000000000000000.000000
   Scan (11) = 340282346638528860000000000000000000000.000000
   Scan (12) = 340282346638528860000000000000000000000.000000
   Scan (13) = 340282346638528860000000000000000000000.000000
   Scan (14) = 340282346638528860000000000000000000000.000000
   Scan (15) = 340282346638528860000000000000000000000.000000
   Scan (16) = 340282346638528860000000000000000000000.000000
   Scan (17) = 340282346638528860000000000000000000000.000000
   Scan (18) = 340282346638528860000000000000000000000.000000
   Scan (19) = 340282346638528860000000000000000000000.000000
   Scan (20) = 340282346638528860000000000000000000000.000000
   Scan (21) = 340282346638528860000000000000000000000.000000
   Scan (22) = 340282346638528860000000000000000000000.000000
   Scan (23) = 340282346638528860000000000000000000000.000000
   Scan (24) = 340282346638528860000000000000000000000.000000
   Scan (25) = 340282346638528860000000000000000000000.000000
   Scan (26) = 340282346638528860000000000000000000000.000000
   Scan (27) = 340282346638528860000000000000000000000.000000
   Scan (28) = 340282346638528860000000000000000000000.000000
   Scan (29) = 340282346638528860000000000000000000000.000000
   Scan (30) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (31) = 340282346638528860000000000000000000000.000000
   Scan (32) = 340282346638528860000000000000000000000.000000
   Scan (33) = 340282346638528860000000000000000000000.000000
   Scan (34) = 340282346638528860000000000000000000000.000000
   Scan (35) = 340282346638528860000000000000000000000.000000
   Scan (36) = 340282346638528860000000000000000000000.000000
   Scan (37) = 340282346638528860000000000000000000000.000000
   Scan (38) = 340282346638528860000000000000000000000.000000
   Scan (39) = 340282346638528860000000000000000000000.000000
   Scan (40) = 340282346638528860000000000000000000000.000000
   Scan (41) = 340282346638528860000000000000000000000.000000
   Scan (42) = 340282346638528860000000000000000000000.000000
   Scan (43) = 340282346638528860000000000000000000000.000000
   Scan (44) = 340282346638528860000000000000000000000.000000
   Scan (45) = 340282346638528860000000000000000000000.000000
   Scan (46) = 340282346638528860000000000000000000000.000000
   Scan (47) = 340282346638528860000000000000000000000.000000
   Scan (48) = 340282346638528860000000000000000000000.000000
   Scan (49) = 340282346638528860000000000000000000000.000000
   Scan (50) = 340282346638528860000000000000000000000.000000
   Scan (51) = 340282346638528860000000000000000000000.000000
   Scan (52) = 340282346638528860000000000000000000000.000000
   Scan (53) = 340282346638528860000000000000000000000.000000
   Scan (54) = 340282346638528860000000000000000000000.000000
   Scan (55) = 340282346638528860000000000000000000000.000000
   Scan (56) = 340282346638528860000000000000000000000.000000
   Scan (57) = 340282346638528860000000000000000000000.000000
   Scan (58) = 340282346638528860000000000000000000000.000000
   Scan (59) = 340282346638528860000000000000000000000.000000
   Scan (60) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (61) = 340282346638528860000000000000000000000.000000
   Scan (62) = 340282346638528860000000000000000000000.000000
   Scan (63) = 340282346638528860000000000000000000000.000000
   Scan (64) = 340282346638528860000000000000000000000.000000
   Scan (65) = 340282346638528860000000000000000000000.000000
   Scan (66) = 340282346638528860000000000000000000000.000000
   Scan (67) = 340282346638528860000000000000000000000.000000
   Scan (68) = 340282346638528860000000000000000000000.000000
   Scan (69) = 340282346638528860000000000000000000000.000000
   Scan (70) = 56.233406
   Scan (71) = 48.547855
   Scan (72) = 44.794228
   Scan (73) = 42.547737
   Scan (74) = 40.757431
   Scan (75) = 39.415585
   Scan (76) = 38.227577
   Scan (77) = 37.636349
   Scan (78) = 37.199081
   Scan (79) = 35.581814
   Scan (80) = 33.668865
   Scan (81) = 32.343952
   Scan (82) = 31.752907
   Scan (83) = 32.190784
   Scan (84) = 32.778076
   Scan (85) = 31.754824
   Scan (86) = 29.275963
   Scan (87) = 26.811920
   Scan (88) = 25.078442
   Scan (89) = 23.214191
   Scan (90) = 21.065834

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (91) = 19.931587
   Scan (92) = 20.236906
   Scan (93) = 21.112923
   Scan (94) = 21.408804
   Scan (95) = 20.694847
   Scan (96) = 19.981058
   Scan (97) = 19.559317
   Scan (98) = 18.994808
   Scan (99) = 18.282101
   Scan (100) = 18.435019
   Scan (101) = 18.155125
   Scan (102) = 17.008781
   Scan (103) = 15.290134
   Scan (104) = 14.147663
   Scan (105) = 13.437286
   Scan (106) = 12.583661
   Scan (107) = 11.730627
   Scan (108) = 11.017529
   Scan (109) = 10.882420
   Scan (110) = 10.459847
   Scan (111) = 9.314759
   Scan (112) = 7.873363
   Scan (113) = 6.720038
   Scan (114) = 6.144236
   Scan (115) = 6.439500
   Scan (116) = 8.006097
   Scan (117) = 10.033984
   Scan (118) = 11.193314
   Scan (119) = 10.908168
   Scan (120) = 9.757375

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (121) = 8.756698
   Scan (122) = 8.192835
   Scan (123) = 7.774094
   Scan (124) = 7.064892
   Scan (125) = 6.353303
   Scan (126) = 5.639479
   Scan (127) = 5.209129
   Scan (128) = 5.064355
   Scan (129) = 4.915910
   Scan (130) = 4.911825
   Scan (131) = 5.342724
   Scan (132) = 6.497447
   Scan (133) = 7.652384
   Scan (134) = 7.797061
   Scan (135) = 7.074792
   Scan (136) = 6.211405
   Scan (137) = 5.491012
   Scan (138) = 4.771335
   Scan (139) = 4.194129
   Scan (140) = 3.907825
   Scan (141) = 3.910343
   Scan (142) = 3.767258
   Scan (143) = 3.914613
   Scan (144) = 4.061509
   Scan (145) = 3.917446
   Scan (146) = 3.771559
   Scan (147) = 3.760821
   Scan (148) = 3.759945
   Scan (149) = 3.760691
   Scan (150) = 3.761268

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (151) = 3.617855
   Scan (152) = 3.472648
   Scan (153) = 3.041629
   Scan (154) = 2.896787
   Scan (155) = 2.897599
   Scan (156) = 3.043899
   Scan (157) = 3.044209
   Scan (158) = 2.900560
   Scan (159) = 2.471132
   Scan (160) = 2.331500
   Scan (161) = 2.046239
   Scan (162) = 2.048775
   Scan (163) = 2.052848
   Scan (164) = 1.911187
   Scan (165) = 2.058845
   Scan (166) = 2.062310
   Scan (167) = 1.920158
   Scan (168) = 1.774800
   Scan (169) = 1.627818
   Scan (170) = 1.333577
   Scan (171) = 1.332241
   Scan (172) = 1.481491
   Scan (173) = 1.341017
   Scan (174) = 1.341046
   Scan (175) = 1.195478
   Scan (176) = 0.000000
   Scan (177) = 0.000000
   Scan (178) = 0.000000
   Scan (179) = 0.000000
   Scan (180) = 0.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (181) = 0.000000
   Scan (182) = 0.000000
   Scan (183) = 0.000000
   Scan (184) = 0.000000
   Scan (185) = 0.000000
   Scan (186) = 0.000000
   Scan (187) = 0.000000
   Scan (188) = 0.000000
   Scan (189) = 0.000000
   Scan (190) = 0.000000
   Scan (191) = 0.000000
   Scan (192) = 0.000000
   Scan (193) = 0.000000
   Scan (194) = 0.000000
   Scan (195) = 0.000000
   Scan (196) = 0.000000
   Scan (197) = 0.000000
   Scan (198) = 0.000000
   Scan (199) = 0.000000
   Scan (200) = 0.000000
   Scan (201) = 0.000000
   Scan (202) = 0.000000
   Scan (203) = 0.000000
   Scan (204) = 0.000000
   Scan (205) = 0.000000
   Scan (206) = 0.000000
   Scan (207) = 0.000000
   Scan (208) = 0.000000
   Scan (209) = 0.000000
   Scan (210) = 0.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (211) = 0.000000
   Scan (212) = 0.000000
   Scan (213) = 0.000000
   Scan (214) = 0.000000
   Scan (215) = 0.000000
   Scan (216) = 0.000000
   Scan (217) = 0.000000
   Scan (218) = 0.000000
   Scan (219) = 0.000000
   Scan (220) = 0.000000
   Scan (221) = 0.000000
   Scan (222) = 0.000000
   Scan (223) = 0.000000
   Scan (224) = 0.000000
   Scan (225) = 0.000000
   Scan (226) = 0.000000
   Scan (227) = 0.000000
   Scan (228) = 0.000000
   Scan (229) = 0.000000
   Scan (230) = 0.000000
   Scan (231) = 0.000000
   Scan (232) = 0.000000
   Scan (233) = 0.000000
   Scan (234) = 0.000000
   Scan (235) = 0.000000
   Scan (236) = 0.000000
   Scan (237) = 0.000000
   Scan (238) = 0.000000
   Scan (239) = 0.000000
   Scan (240) = 0.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (241) = 0.000000
   Scan (242) = 0.000000
   Scan (243) = 0.000000
   Scan (244) = 0.000000
   Scan (245) = 0.000000
   Scan (246) = 0.000000
   Scan (247) = 0.000000
   Scan (248) = 0.000000
   Scan (249) = 0.000000
   Scan (250) = 0.000000
   Scan (251) = 0.000000
   Scan (252) = 0.000000
   Scan (253) = 0.000000
   Scan (254) = 0.000000
   Scan (255) = 0.000000
   Scan (256) = 0.000000
   Scan (257) = 0.000000
   Scan (258) = 0.000000
   Scan (259) = 0.000000
   Scan (260) = 0.000000
   Scan (261) = 0.000000
   Scan (262) = 0.000000
   Scan (263) = 0.000000
   Scan (264) = 1.538362
   Scan (265) = 1.678382
   Scan (266) = 2.102546
   Scan (267) = 2.524406
   Scan (268) = 2.949291
   Scan (269) = 3.519436
   Scan (270) = 3.801434

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (271) = 4.236247
   Scan (272) = 5.095213
   Scan (273) = 6.242624
   Scan (274) = 6.096614
   Scan (275) = 6.393714
   Scan (276) = 7.834250
   Scan (277) = 10.551929
   Scan (278) = 11.825077
   Scan (279) = 14.128882
   Scan (280) = 18.583967
   Scan (281) = 22.167814
   Scan (282) = 23.437334
   Scan (283) = 26.160961
   Scan (284) = 28.756968
   Scan (285) = 30.490709
   Scan (286) = 35.813625
   Scan (287) = 46.365067
   Scan (288) = 340282346638528860000000000000000000000.000000
   Scan (289) = 340282346638528860000000000000000000000.000000
   Scan (290) = 340282346638528860000000000000000000000.000000
   Scan (291) = 340282346638528860000000000000000000000.000000
   Scan (292) = 340282346638528860000000000000000000000.000000
   Scan (293) = 340282346638528860000000000000000000000.000000
   Scan (294) = 340282346638528860000000000000000000000.000000
   Scan (295) = 340282346638528860000000000000000000000.000000
   Scan (296) = 340282346638528860000000000000000000000.000000
   Scan (297) = 340282346638528860000000000000000000000.000000
   Scan (298) = 340282346638528860000000000000000000000.000000
   Scan (299) = 340282346638528860000000000000000000000.000000
   Scan (300) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (301) = 340282346638528860000000000000000000000.000000
   Scan (302) = 340282346638528860000000000000000000000.000000
   Scan (303) = 340282346638528860000000000000000000000.000000
   Scan (304) = 340282346638528860000000000000000000000.000000
   Scan (305) = 340282346638528860000000000000000000000.000000
   Scan (306) = 340282346638528860000000000000000000000.000000
   Scan (307) = 340282346638528860000000000000000000000.000000
   Scan (308) = 340282346638528860000000000000000000000.000000
   Scan (309) = 340282346638528860000000000000000000000.000000
   Scan (310) = 340282346638528860000000000000000000000.000000
   Scan (311) = 340282346638528860000000000000000000000.000000
   Scan (312) = 340282346638528860000000000000000000000.000000
   Scan (313) = 340282346638528860000000000000000000000.000000
   Scan (314) = 340282346638528860000000000000000000000.000000
   Scan (315) = 340282346638528860000000000000000000000.000000
   Scan (316) = 340282346638528860000000000000000000000.000000
   Scan (317) = 340282346638528860000000000000000000000.000000
   Scan (318) = 340282346638528860000000000000000000000.000000
   Scan (319) = 340282346638528860000000000000000000000.000000
   Scan (320) = 340282346638528860000000000000000000000.000000
   Scan (321) = 340282346638528860000000000000000000000.000000
   Scan (322) = 340282346638528860000000000000000000000.000000
   Scan (323) = 340282346638528860000000000000000000000.000000
   Scan (324) = 340282346638528860000000000000000000000.000000
   Scan (325) = 340282346638528860000000000000000000000.000000
   Scan (326) = 340282346638528860000000000000000000000.000000
   Scan (327) = 340282346638528860000000000000000000000.000000
   Scan (328) = 340282346638528860000000000000000000000.000000
   Scan (329) = 340282346638528860000000000000000000000.000000
   Scan (330) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (331) = 340282346638528860000000000000000000000.000000
   Scan (332) = 340282346638528860000000000000000000000.000000
   Scan (333) = 340282346638528860000000000000000000000.000000
   Scan (334) = 340282346638528860000000000000000000000.000000
   Scan (335) = 340282346638528860000000000000000000000.000000
   Scan (336) = 340282346638528860000000000000000000000.000000
   Scan (337) = 340282346638528860000000000000000000000.000000
   Scan (338) = 340282346638528860000000000000000000000.000000
   Scan (339) = 340282346638528860000000000000000000000.000000
   Scan (340) = 340282346638528860000000000000000000000.000000
   Scan (341) = 340282346638528860000000000000000000000.000000
   Scan (342) = 340282346638528860000000000000000000000.000000
   Scan (343) = 340282346638528860000000000000000000000.000000
   Scan (344) = 340282346638528860000000000000000000000.000000
   Scan (345) = 340282346638528860000000000000000000000.000000
   Scan (346) = 340282346638528860000000000000000000000.000000
   Scan (347) = 340282346638528860000000000000000000000.000000
   Scan (348) = 340282346638528860000000000000000000000.000000
   Scan (349) = 340282346638528860000000000000000000000.000000
   Scan (350) = 340282346638528860000000000000000000000.000000
   Scan (351) = 340282346638528860000000000000000000000.000000
   Scan (352) = 340282346638528860000000000000000000000.000000
   Scan (353) = 340282346638528860000000000000000000000.000000
   Scan (354) = 340282346638528860000000000000000000000.000000
   Scan (355) = 340282346638528860000000000000000000000.000000
   Scan (356) = 340282346638528860000000000000000000000.000000
   Scan (357) = 340282346638528860000000000000000000000.000000
   Scan (358) = 340282346638528860000000000000000000000.000000
   Scan (359) = 340282346638528860000000000000000000000.000000
   Scan (360) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (361) = 340282346638528860000000000000000000000.000000
   Scan (362) = 340282346638528860000000000000000000000.000000
   Scan (363) = 340282346638528860000000000000000000000.000000
   Scan (364) = 340282346638528860000000000000000000000.000000
   Scan (365) = 340282346638528860000000000000000000000.000000
   Scan (366) = 340282346638528860000000000000000000000.000000
   Scan (367) = 340282346638528860000000000000000000000.000000
   Scan (368) = 340282346638528860000000000000000000000.000000
   Scan (369) = 340282346638528860000000000000000000000.000000
   Scan (370) = 340282346638528860000000000000000000000.000000
   Scan (371) = 340282346638528860000000000000000000000.000000
   Scan (372) = 340282346638528860000000000000000000000.000000
   Scan (373) = 340282346638528860000000000000000000000.000000
   Scan (374) = 340282346638528860000000000000000000000.000000
   Scan (375) = 340282346638528860000000000000000000000.000000
   Scan (376) = 340282346638528860000000000000000000000.000000
   Scan (377) = 340282346638528860000000000000000000000.000000
   Scan (378) = 340282346638528860000000000000000000000.000000
   Scan (379) = 340282346638528860000000000000000000000.000000
   Scan (380) = 340282346638528860000000000000000000000.000000
   Scan (381) = 340282346638528860000000000000000000000.000000
   Scan (382) = 340282346638528860000000000000000000000.000000
   Scan (383) = 340282346638528860000000000000000000000.000000
   Scan (384) = 340282346638528860000000000000000000000.000000
   Scan (385) = 340282346638528860000000000000000000000.000000
   Scan (386) = 340282346638528860000000000000000000000.000000
   Scan (387) = 340282346638528860000000000000000000000.000000
   Scan (388) = 340282346638528860000000000000000000000.000000
   Scan (389) = 340282346638528860000000000000000000000.000000
   Scan (390) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (391) = 340282346638528860000000000000000000000.000000
   Scan (392) = 340282346638528860000000000000000000000.000000
   Scan (393) = 340282346638528860000000000000000000000.000000
   Scan (394) = 340282346638528860000000000000000000000.000000
   Scan (395) = 340282346638528860000000000000000000000.000000
   Scan (396) = 340282346638528860000000000000000000000.000000
   Scan (397) = 340282346638528860000000000000000000000.000000
   Scan (398) = 340282346638528860000000000000000000000.000000
   Scan (399) = 340282346638528860000000000000000000000.000000
   Scan (400) = 54.054375
   Scan (401) = 52.708660
   Scan (402) = 50.032551
   Scan (403) = 46.331276
   Scan (404) = 46.795490
   Scan (405) = 49.341347
   Scan (406) = 47.548576
   Scan (407) = 41.954567
   Scan (408) = 38.442673
   Scan (409) = 38.012897
   Scan (410) = 36.117012
   Scan (411) = 32.765854
   Scan (412) = 33.357777
   Scan (413) = 35.851780
   Scan (414) = 34.553799
   Scan (415) = 29.468941
   Scan (416) = 24.251499
   Scan (417) = 21.510044
   Scan (418) = 21.525143
   Scan (419) = 23.282633
   Scan (420) = 24.756786

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (421) = 24.202278
   Scan (422) = 21.760172
   Scan (423) = 18.738777
   Scan (424) = 16.871189
   Scan (425) = 16.594397
   Scan (426) = 16.605862
   Scan (427) = 14.602913
   Scan (428) = 12.027005
   Scan (429) = 11.897918
   Scan (430) = 12.630574
   Scan (431) = 12.214604
   Scan (432) = 12.085029
   Scan (433) = 12.964805
   Scan (434) = 14.415616
   Scan (435) = 16.007151
   Scan (436) = 16.311983
   Scan (437) = 14.884769
   Scan (438) = 12.591505
   Scan (439) = 10.586342
   Scan (440) = 9.871661
   Scan (441) = 9.442935
   Scan (442) = 8.577829
   Scan (443) = 7.279661
   Scan (444) = 6.416116
   Scan (445) = 5.986917
   Scan (446) = 5.848185
   Scan (447) = 5.708718
   Scan (448) = 6.146892
   Scan (449) = 7.016716
   Scan (450) = 7.597753

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (451) = 7.453588
   Scan (452) = 6.878215
   Scan (453) = 5.872332
   Scan (454) = 5.015016
   Scan (455) = 4.734607
   Scan (456) = 5.032180
   Scan (457) = 5.615977
   Scan (458) = 5.909559
   Scan (459) = 5.767923
   Scan (460) = 5.335253
   Scan (461) = 4.759238
   Scan (462) = 4.470715
   Scan (463) = 4.618705
   Scan (464) = 4.481009
   Scan (465) = 4.197714
   Scan (466) = 3.902822
   Scan (467) = 3.761006
   Scan (468) = 3.331416
   Scan (469) = 3.047786
   Scan (470) = 2.762314
   Scan (471) = 2.762643
   Scan (472) = 2.764568
   Scan (473) = 2.624289
   Scan (474) = 2.629594
   Scan (475) = 2.345125
   Scan (476) = 2.348395
   Scan (477) = 2.207600
   Scan (478) = 2.068527
   Scan (479) = 2.071419
   Scan (480) = 2.074093

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (481) = 1.936167
   Scan (482) = 1.794771
   Scan (483) = 2.084806
   Scan (484) = 1.944512
   Scan (485) = 1.800963
   Scan (486) = 1.510345
   Scan (487) = 1.365369
   Scan (488) = 1.365892
   Scan (489) = 1.222453
   Scan (490) = 0.000000
   Scan (491) = 0.000000
   Scan (492) = 0.000000
   Scan (493) = 0.000000
   Scan (494) = 0.000000
   Scan (495) = 0.000000
   Scan (496) = 0.000000
   Scan (497) = 0.000000
   Scan (498) = 0.000000
   Scan (499) = 0.000000
   Scan (500) = 0.000000
   Scan (501) = 0.000000
   Scan (502) = 0.000000
   Scan (503) = 0.000000
   Scan (504) = 0.000000
   Scan (505) = 0.000000
   Scan (506) = 0.000000
   Scan (507) = 0.000000
   Scan (508) = 0.000000
   Scan (509) = 0.000000
   Scan (510) = 0.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (511) = 0.000000
   Scan (512) = 0.000000
   Scan (513) = 0.000000
   Scan (514) = 0.000000
   Scan (515) = 0.000000
   Scan (516) = 0.000000
   Scan (517) = 0.000000
   Scan (518) = 0.000000
   Scan (519) = 0.000000
   Scan (520) = 0.000000
   Scan (521) = 0.000000
   Scan (522) = 0.000000
   Scan (523) = 0.000000
   Scan (524) = 0.000000
   Scan (525) = 0.000000
   Scan (526) = 0.000000
   Scan (527) = 0.000000
   Scan (528) = 0.000000
   Scan (529) = 0.000000
   Scan (530) = 0.000000
   Scan (531) = 0.000000
   Scan (532) = 0.000000
   Scan (533) = 0.000000
   Scan (534) = 0.000000
   Scan (535) = 0.000000
   Scan (536) = 0.000000
   Scan (537) = 0.000000
   Scan (538) = 0.000000
   Scan (539) = 0.000000
   Scan (540) = 0.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (541) = 0.000000
   Scan (542) = 0.000000
   Scan (543) = 0.000000
   Scan (544) = 0.000000
   Scan (545) = 0.000000
   Scan (546) = 0.000000
   Scan (547) = 0.000000
   Scan (548) = 0.000000
   Scan (549) = 0.000000
   Scan (550) = 0.000000
   Scan (551) = 0.000000
   Scan (552) = 0.000000
   Scan (553) = 0.000000
   Scan (554) = 0.000000
   Scan (555) = 0.000000
   Scan (556) = 0.000000
   Scan (557) = 0.000000
   Scan (558) = 0.000000
   Scan (559) = 0.000000
   Scan (560) = 0.000000
   Scan (561) = 0.000000
   Scan (562) = 0.000000
   Scan (563) = 0.000000
   Scan (564) = 0.000000
   Scan (565) = 0.000000
   Scan (566) = 0.000000
   Scan (567) = 0.000000
   Scan (568) = 0.000000
   Scan (569) = 0.000000
   Scan (570) = 0.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (571) = 0.000000
   Scan (572) = 0.000000
   Scan (573) = 0.000000
   Scan (574) = 0.000000
   Scan (575) = 0.000000
   Scan (576) = 0.000000
   Scan (577) = 0.000000
   Scan (578) = 0.000000
   Scan (579) = 0.000000
   Scan (580) = 0.000000
   Scan (581) = 0.000000
   Scan (582) = 0.000000
   Scan (583) = 0.000000
   Scan (584) = 1.567374
   Scan (585) = 1.707151
   Scan (586) = 1.985630
   Scan (587) = 2.116262
   Scan (588) = 2.537688
   Scan (589) = 3.104248
   Scan (590) = 3.678509
   Scan (591) = 4.101089
   Scan (592) = 4.812524
   Scan (593) = 6.243522
   Scan (594) = 6.813789
   Scan (595) = 7.531462
   Scan (596) = 11.274709
   Scan (597) = 14.720118
   Scan (598) = 15.429416
   Scan (599) = 18.442730
   Scan (600) = 23.916965

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (601) = 28.972097
   Scan (602) = 35.802612
   Scan (603) = 46.837082
   Scan (604) = 57.905514
   Scan (605) = 340282346638528860000000000000000000000.000000
   Scan (606) = 340282346638528860000000000000000000000.000000
   Scan (607) = 340282346638528860000000000000000000000.000000
   Scan (608) = 340282346638528860000000000000000000000.000000
   Scan (609) = 340282346638528860000000000000000000000.000000
   Scan (610) = 340282346638528860000000000000000000000.000000
   Scan (611) = 340282346638528860000000000000000000000.000000
   Scan (612) = 340282346638528860000000000000000000000.000000
   Scan (613) = 340282346638528860000000000000000000000.000000
   Scan (614) = 340282346638528860000000000000000000000.000000
   Scan (615) = 340282346638528860000000000000000000000.000000
   Scan (616) = 340282346638528860000000000000000000000.000000
   Scan (617) = 340282346638528860000000000000000000000.000000
   Scan (618) = 340282346638528860000000000000000000000.000000
   Scan (619) = 340282346638528860000000000000000000000.000000
   Scan (620) = 340282346638528860000000000000000000000.000000
   Scan (621) = 340282346638528860000000000000000000000.000000
   Scan (622) = 340282346638528860000000000000000000000.000000
   Scan (623) = 340282346638528860000000000000000000000.000000
   Scan (624) = 340282346638528860000000000000000000000.000000
   Scan (625) = 340282346638528860000000000000000000000.000000
   Scan (626) = 340282346638528860000000000000000000000.000000
   Scan (627) = 340282346638528860000000000000000000000.000000
   Scan (628) = 340282346638528860000000000000000000000.000000
   Scan (629) = 340282346638528860000000000000000000000.000000
   Scan (630) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 3 (Continued)
   Scan (631) = 340282346638528860000000000000000000000.000000
   Scan (632) = 340282346638528860000000000000000000000.000000
   Scan (633) = 340282346638528860000000000000000000000.000000
   Scan (634) = 340282346638528860000000000000000000000.000000
   Scan (635) = 340282346638528860000000000000000000000.000000
   Scan (636) = 340282346638528860000000000000000000000.000000
   Scan (637) = 340282346638528860000000000000000000000.000000
   Scan (638) = 340282346638528860000000000000000000000.000000
   Scan (639) = 340282346638528860000000000000000000000.000000
   Scan (640) = 340282346638528860000000000000000000000.000000
   Scan (641) = 340282346638528860000000000000000000000.000000
   Scan (642) = 340282346638528860000000000000000000000.000000
   Scan (643) = 340282346638528860000000000000000000000.000000
   Scan (644) = 340282346638528860000000000000000000000.000000
   Scan (645) = 340282346638528860000000000000000000000.000000
   Scan (646) = 340282346638528860000000000000000000000.000000
   Scan (647) = 340282346638528860000000000000000000000.000000
   Scan (648) = 340282346638528860000000000000000000000.000000
   Scan (649) = 340282346638528860000000000000000000000.000000
   Scan (650) = 340282346638528860000000000000000000000.000000
   Scan (651) = 340282346638528860000000000000000000000.000000
   Scan (652) = 340282346638528860000000000000000000000.000000
   Scan (653) = 340282346638528860000000000000000000000.000000
   Scan (654) = 340282346638528860000000000000000000000.000000
   Scan (655) = 340282346638528860000000000000000000000.000000
   Scan (656) = 340282346638528860000000000000000000000.000000
   Scan (657) = 340282346638528860000000000000000000000.000000
   Scan (658) = 340282346638528860000000000000000000000.000000
   Scan (659) = 340282346638528860000000000000000000000.000000
   Scan (660) = 340282346638528860000000000000000000000.000000


CERES SW unfiltered radiance for Region 4
   Scan (1) = 340282346638528860000000000000000000000.000000
   Scan (2) = 340282346638528860000000000000000000000.000000
   Scan (3) = 340282346638528860000000000000000000000.000000
   Scan (4) = 340282346638528860000000000000000000000.000000
   Scan (5) = 340282346638528860000000000000000000000.000000
   Scan (6) = 340282346638528860000000000000000000000.000000
   Scan (7) = 340282346638528860000000000000000000000.000000
   Scan (8) = 340282346638528860000000000000000000000.000000
   Scan (9) = 340282346638528860000000000000000000000.000000
   Scan (10) = 340282346638528860000000000000000000000.000000
   Scan (11) = 340282346638528860000000000000000000000.000000
   Scan (12) = 340282346638528860000000000000000000000.000000
   Scan (13) = 340282346638528860000000000000000000000.000000
   Scan (14) = 340282346638528860000000000000000000000.000000
   Scan (15) = 340282346638528860000000000000000000000.000000
   Scan (16) = 340282346638528860000000000000000000000.000000
   Scan (17) = 340282346638528860000000000000000000000.000000
   Scan (18) = 340282346638528860000000000000000000000.000000
   Scan (19) = 340282346638528860000000000000000000000.000000
   Scan (20) = 340282346638528860000000000000000000000.000000
   Scan (21) = 340282346638528860000000000000000000000.000000
   Scan (22) = 340282346638528860000000000000000000000.000000
   Scan (23) = 340282346638528860000000000000000000000.000000
   Scan (24) = 340282346638528860000000000000000000000.000000
   Scan (25) = 340282346638528860000000000000000000000.000000
   Scan (26) = 340282346638528860000000000000000000000.000000
   Scan (27) = 340282346638528860000000000000000000000.000000
   Scan (28) = 340282346638528860000000000000000000000.000000
   Scan (29) = 340282346638528860000000000000000000000.000000
   Scan (30) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (31) = 340282346638528860000000000000000000000.000000
   Scan (32) = 340282346638528860000000000000000000000.000000
   Scan (33) = 340282346638528860000000000000000000000.000000
   Scan (34) = 340282346638528860000000000000000000000.000000
   Scan (35) = 340282346638528860000000000000000000000.000000
   Scan (36) = 340282346638528860000000000000000000000.000000
   Scan (37) = 340282346638528860000000000000000000000.000000
   Scan (38) = 340282346638528860000000000000000000000.000000
   Scan (39) = 340282346638528860000000000000000000000.000000
   Scan (40) = 340282346638528860000000000000000000000.000000
   Scan (41) = 340282346638528860000000000000000000000.000000
   Scan (42) = 340282346638528860000000000000000000000.000000
   Scan (43) = 340282346638528860000000000000000000000.000000
   Scan (44) = 340282346638528860000000000000000000000.000000
   Scan (45) = 340282346638528860000000000000000000000.000000
   Scan (46) = 340282346638528860000000000000000000000.000000
   Scan (47) = 340282346638528860000000000000000000000.000000
   Scan (48) = 340282346638528860000000000000000000000.000000
   Scan (49) = 340282346638528860000000000000000000000.000000
   Scan (50) = 340282346638528860000000000000000000000.000000
   Scan (51) = 340282346638528860000000000000000000000.000000
   Scan (52) = 340282346638528860000000000000000000000.000000
   Scan (53) = 340282346638528860000000000000000000000.000000
   Scan (54) = 340282346638528860000000000000000000000.000000
   Scan (55) = 340282346638528860000000000000000000000.000000
   Scan (56) = 340282346638528860000000000000000000000.000000
   Scan (57) = 340282346638528860000000000000000000000.000000
   Scan (58) = 340282346638528860000000000000000000000.000000
   Scan (59) = 340282346638528860000000000000000000000.000000
   Scan (60) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (61) = 340282346638528860000000000000000000000.000000
   Scan (62) = 340282346638528860000000000000000000000.000000
   Scan (63) = 340282346638528860000000000000000000000.000000
   Scan (64) = 340282346638528860000000000000000000000.000000
   Scan (65) = 340282346638528860000000000000000000000.000000
   Scan (66) = 340282346638528860000000000000000000000.000000
   Scan (67) = 340282346638528860000000000000000000000.000000
   Scan (68) = 340282346638528860000000000000000000000.000000
   Scan (69) = 340282346638528860000000000000000000000.000000
   Scan (70) = 340282346638528860000000000000000000000.000000
   Scan (71) = 340282346638528860000000000000000000000.000000
   Scan (72) = 340282346638528860000000000000000000000.000000
   Scan (73) = 340282346638528860000000000000000000000.000000
   Scan (74) = 340282346638528860000000000000000000000.000000
   Scan (75) = 340282346638528860000000000000000000000.000000
   Scan (76) = 340282346638528860000000000000000000000.000000
   Scan (77) = 340282346638528860000000000000000000000.000000
   Scan (78) = 340282346638528860000000000000000000000.000000
   Scan (79) = 340282346638528860000000000000000000000.000000
   Scan (80) = 340282346638528860000000000000000000000.000000
   Scan (81) = 340282346638528860000000000000000000000.000000
   Scan (82) = 340282346638528860000000000000000000000.000000
   Scan (83) = 340282346638528860000000000000000000000.000000
   Scan (84) = 340282346638528860000000000000000000000.000000
   Scan (85) = 62.821316
   Scan (86) = 60.421078
   Scan (87) = 60.865654
   Scan (88) = 61.169174
   Scan (89) = 60.568176
   Scan (90) = 60.271080

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (91) = 56.209766
   Scan (92) = 46.523151
   Scan (93) = 38.289124
   Scan (94) = 34.654743
   Scan (95) = 33.508747
   Scan (96) = 33.097801
   Scan (97) = 33.272251
   Scan (98) = 33.736610
   Scan (99) = 32.599255
   Scan (100) = 29.573860
   Scan (101) = 28.000786
   Scan (102) = 27.585714
   Scan (103) = 25.722198
   Scan (104) = 22.566633
   Scan (105) = 19.563353
   Scan (106) = 18.006100
   Scan (107) = 18.894007
   Scan (108) = 19.631851
   Scan (109) = 18.206715
   Scan (110) = 15.630238
   Scan (111) = 14.350526
   Scan (112) = 16.527864
   Scan (113) = 19.866169
   Scan (114) = 20.031286
   Scan (115) = 17.600519
   Scan (116) = 15.306135
   Scan (117) = 14.023686
   Scan (118) = 12.593502
   Scan (119) = 10.584769
   Scan (120) = 8.435806

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (121) = 7.296080
   Scan (122) = 6.737564
   Scan (123) = 6.606540
   Scan (124) = 6.474328
   Scan (125) = 6.771126
   Scan (126) = 7.357143
   Scan (127) = 8.228379
   Scan (128) = 8.811009
   Scan (129) = 8.237044
   Scan (130) = 7.376285
   Scan (131) = 6.376666
   Scan (132) = 5.525163
   Scan (133) = 5.106032
   Scan (134) = 5.259904
   Scan (135) = 5.556188
   Scan (136) = 5.706254
   Scan (137) = 5.278497
   Scan (138) = 5.137410
   Scan (139) = 4.707442
   Scan (140) = 4.568023
   Scan (141) = 4.573557
   Scan (142) = 4.723855
   Scan (143) = 4.586933
   Scan (144) = 4.159478
   Scan (145) = 3.875502
   Scan (146) = 3.591532
   Scan (147) = 3.296471
   Scan (148) = 3.013743
   Scan (149) = 3.014402
   Scan (150) = 3.014704

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (151) = 2.874899
   Scan (152) = 2.736813
   Scan (153) = 2.598554
   Scan (154) = 2.458341
   Scan (155) = 2.463360
   Scan (156) = 2.322757
   Scan (157) = 2.325638
   Scan (158) = 2.328280
   Scan (159) = 2.185426
   Scan (160) = 2.189525
   Scan (161) = 2.193376
   Scan (162) = 2.053433
   Scan (163) = 1.910216
   Scan (164) = 1.767010
   Scan (165) = 1.478625
   Scan (166) = 1.338863
   Scan (167) = 0.000000
   Scan (168) = 0.000000
   Scan (169) = 0.000000
   Scan (170) = 0.000000
   Scan (171) = 0.000000
   Scan (172) = 0.000000
   Scan (173) = 0.000000
   Scan (174) = 0.000000
   Scan (175) = 0.000000
   Scan (176) = 0.000000
   Scan (177) = 0.000000
   Scan (178) = 0.000000
   Scan (179) = 0.000000
   Scan (180) = 0.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (181) = 0.000000
   Scan (182) = 0.000000
   Scan (183) = 0.000000
   Scan (184) = 0.000000
   Scan (185) = 0.000000
   Scan (186) = 0.000000
   Scan (187) = 0.000000
   Scan (188) = 0.000000
   Scan (189) = 0.000000
   Scan (190) = 0.000000
   Scan (191) = 0.000000
   Scan (192) = 0.000000
   Scan (193) = 0.000000
   Scan (194) = 0.000000
   Scan (195) = 0.000000
   Scan (196) = 0.000000
   Scan (197) = 0.000000
   Scan (198) = 0.000000
   Scan (199) = 0.000000
   Scan (200) = 0.000000
   Scan (201) = 0.000000
   Scan (202) = 0.000000
   Scan (203) = 0.000000
   Scan (204) = 0.000000
   Scan (205) = 0.000000
   Scan (206) = 0.000000
   Scan (207) = 0.000000
   Scan (208) = 0.000000
   Scan (209) = 0.000000
   Scan (210) = 0.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (211) = 0.000000
   Scan (212) = 0.000000
   Scan (213) = 0.000000
   Scan (214) = 0.000000
   Scan (215) = 0.000000
   Scan (216) = 0.000000
   Scan (217) = 0.000000
   Scan (218) = 0.000000
   Scan (219) = 0.000000
   Scan (220) = 0.000000
   Scan (221) = 0.000000
   Scan (222) = 0.000000
   Scan (223) = 0.000000
   Scan (224) = 0.000000
   Scan (225) = 0.000000
   Scan (226) = 0.000000
   Scan (227) = 0.000000
   Scan (228) = 0.000000
   Scan (229) = 0.000000
   Scan (230) = 0.000000
   Scan (231) = 0.000000
   Scan (232) = 0.000000
   Scan (233) = 0.000000
   Scan (234) = 0.000000
   Scan (235) = 0.000000
   Scan (236) = 0.000000
   Scan (237) = 0.000000
   Scan (238) = 0.000000
   Scan (239) = 0.000000
   Scan (240) = 0.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (241) = 0.000000
   Scan (242) = 0.000000
   Scan (243) = 0.000000
   Scan (244) = 0.000000
   Scan (245) = 0.000000
   Scan (246) = 0.000000
   Scan (247) = 0.000000
   Scan (248) = 0.000000
   Scan (249) = 0.000000
   Scan (250) = 0.000000
   Scan (251) = 0.000000
   Scan (252) = 0.000000
   Scan (253) = 0.000000
   Scan (254) = 0.000000
   Scan (255) = 0.000000
   Scan (256) = 0.000000
   Scan (257) = 0.000000
   Scan (258) = 0.000000
   Scan (259) = 0.000000
   Scan (260) = 0.000000
   Scan (261) = 0.000000
   Scan (262) = 0.000000
   Scan (263) = 0.000000
   Scan (264) = 0.000000
   Scan (265) = 0.000000
   Scan (266) = 1.508890
   Scan (267) = 1.788511
   Scan (268) = 2.351433
   Scan (269) = 2.915255
   Scan (270) = 3.338477

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (271) = 3.773289
   Scan (272) = 4.196859
   Scan (273) = 5.050127
   Scan (274) = 6.193288
   Scan (275) = 6.047266
   Scan (276) = 6.777477
   Scan (277) = 9.222096
   Scan (278) = 12.651670
   Scan (279) = 13.499632
   Scan (280) = 14.206186
   Scan (281) = 16.625166
   Scan (282) = 19.043018
   Scan (283) = 22.784460
   Scan (284) = 26.687347
   Scan (285) = 33.204861
   Scan (286) = 41.903278
   Scan (287) = 50.958591
   Scan (288) = 340282346638528860000000000000000000000.000000
   Scan (289) = 340282346638528860000000000000000000000.000000
   Scan (290) = 340282346638528860000000000000000000000.000000
   Scan (291) = 340282346638528860000000000000000000000.000000
   Scan (292) = 340282346638528860000000000000000000000.000000
   Scan (293) = 340282346638528860000000000000000000000.000000
   Scan (294) = 340282346638528860000000000000000000000.000000
   Scan (295) = 340282346638528860000000000000000000000.000000
   Scan (296) = 340282346638528860000000000000000000000.000000
   Scan (297) = 340282346638528860000000000000000000000.000000
   Scan (298) = 340282346638528860000000000000000000000.000000
   Scan (299) = 340282346638528860000000000000000000000.000000
   Scan (300) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (301) = 340282346638528860000000000000000000000.000000
   Scan (302) = 340282346638528860000000000000000000000.000000
   Scan (303) = 340282346638528860000000000000000000000.000000
   Scan (304) = 340282346638528860000000000000000000000.000000
   Scan (305) = 340282346638528860000000000000000000000.000000
   Scan (306) = 340282346638528860000000000000000000000.000000
   Scan (307) = 340282346638528860000000000000000000000.000000
   Scan (308) = 340282346638528860000000000000000000000.000000
   Scan (309) = 340282346638528860000000000000000000000.000000
   Scan (310) = 340282346638528860000000000000000000000.000000
   Scan (311) = 340282346638528860000000000000000000000.000000
   Scan (312) = 340282346638528860000000000000000000000.000000
   Scan (313) = 340282346638528860000000000000000000000.000000
   Scan (314) = 340282346638528860000000000000000000000.000000
   Scan (315) = 340282346638528860000000000000000000000.000000
   Scan (316) = 340282346638528860000000000000000000000.000000
   Scan (317) = 340282346638528860000000000000000000000.000000
   Scan (318) = 340282346638528860000000000000000000000.000000
   Scan (319) = 340282346638528860000000000000000000000.000000
   Scan (320) = 340282346638528860000000000000000000000.000000
   Scan (321) = 340282346638528860000000000000000000000.000000
   Scan (322) = 340282346638528860000000000000000000000.000000
   Scan (323) = 340282346638528860000000000000000000000.000000
   Scan (324) = 340282346638528860000000000000000000000.000000
   Scan (325) = 340282346638528860000000000000000000000.000000
   Scan (326) = 340282346638528860000000000000000000000.000000
   Scan (327) = 340282346638528860000000000000000000000.000000
   Scan (328) = 340282346638528860000000000000000000000.000000
   Scan (329) = 340282346638528860000000000000000000000.000000
   Scan (330) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (331) = 340282346638528860000000000000000000000.000000
   Scan (332) = 340282346638528860000000000000000000000.000000
   Scan (333) = 340282346638528860000000000000000000000.000000
   Scan (334) = 340282346638528860000000000000000000000.000000
   Scan (335) = 340282346638528860000000000000000000000.000000
   Scan (336) = 340282346638528860000000000000000000000.000000
   Scan (337) = 340282346638528860000000000000000000000.000000
   Scan (338) = 340282346638528860000000000000000000000.000000
   Scan (339) = 340282346638528860000000000000000000000.000000
   Scan (340) = 340282346638528860000000000000000000000.000000
   Scan (341) = 340282346638528860000000000000000000000.000000
   Scan (342) = 340282346638528860000000000000000000000.000000
   Scan (343) = 340282346638528860000000000000000000000.000000
   Scan (344) = 340282346638528860000000000000000000000.000000
   Scan (345) = 340282346638528860000000000000000000000.000000
   Scan (346) = 340282346638528860000000000000000000000.000000
   Scan (347) = 340282346638528860000000000000000000000.000000
   Scan (348) = 340282346638528860000000000000000000000.000000
   Scan (349) = 340282346638528860000000000000000000000.000000
   Scan (350) = 340282346638528860000000000000000000000.000000
   Scan (351) = 340282346638528860000000000000000000000.000000
   Scan (352) = 340282346638528860000000000000000000000.000000
   Scan (353) = 340282346638528860000000000000000000000.000000
   Scan (354) = 340282346638528860000000000000000000000.000000
   Scan (355) = 340282346638528860000000000000000000000.000000
   Scan (356) = 340282346638528860000000000000000000000.000000
   Scan (357) = 340282346638528860000000000000000000000.000000
   Scan (358) = 340282346638528860000000000000000000000.000000
   Scan (359) = 340282346638528860000000000000000000000.000000
   Scan (360) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (361) = 340282346638528860000000000000000000000.000000
   Scan (362) = 340282346638528860000000000000000000000.000000
   Scan (363) = 340282346638528860000000000000000000000.000000
   Scan (364) = 340282346638528860000000000000000000000.000000
   Scan (365) = 340282346638528860000000000000000000000.000000
   Scan (366) = 340282346638528860000000000000000000000.000000
   Scan (367) = 340282346638528860000000000000000000000.000000
   Scan (368) = 340282346638528860000000000000000000000.000000
   Scan (369) = 340282346638528860000000000000000000000.000000
   Scan (370) = 340282346638528860000000000000000000000.000000
   Scan (371) = 340282346638528860000000000000000000000.000000
   Scan (372) = 340282346638528860000000000000000000000.000000
   Scan (373) = 340282346638528860000000000000000000000.000000
   Scan (374) = 340282346638528860000000000000000000000.000000
   Scan (375) = 340282346638528860000000000000000000000.000000
   Scan (376) = 340282346638528860000000000000000000000.000000
   Scan (377) = 340282346638528860000000000000000000000.000000
   Scan (378) = 340282346638528860000000000000000000000.000000
   Scan (379) = 340282346638528860000000000000000000000.000000
   Scan (380) = 340282346638528860000000000000000000000.000000
   Scan (381) = 340282346638528860000000000000000000000.000000
   Scan (382) = 340282346638528860000000000000000000000.000000
   Scan (383) = 340282346638528860000000000000000000000.000000
   Scan (384) = 340282346638528860000000000000000000000.000000
   Scan (385) = 340282346638528860000000000000000000000.000000
   Scan (386) = 340282346638528860000000000000000000000.000000
   Scan (387) = 340282346638528860000000000000000000000.000000
   Scan (388) = 340282346638528860000000000000000000000.000000
   Scan (389) = 340282346638528860000000000000000000000.000000
   Scan (390) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (391) = 340282346638528860000000000000000000000.000000
   Scan (392) = 76.554192
   Scan (393) = 71.152695
   Scan (394) = 65.949760
   Scan (395) = 61.086067
   Scan (396) = 56.705177
   Scan (397) = 52.058701
   Scan (398) = 47.144505
   Scan (399) = 43.152260
   Scan (400) = 39.770092
   Scan (401) = 35.819851
   Scan (402) = 31.746223
   Scan (403) = 28.420023
   Scan (404) = 27.275127
   Scan (405) = 27.296997
   Scan (406) = 28.946501
   Scan (407) = 32.884476
   Scan (408) = 35.519638
   Scan (409) = 34.943546
   Scan (410) = 33.196609
   Scan (411) = 30.440157
   Scan (412) = 28.269205
   Scan (413) = 27.704514
   Scan (414) = 26.993633
   Scan (415) = 25.846371
   Scan (416) = 25.133759
   Scan (417) = 24.998173
   Scan (418) = 24.435093
   Scan (419) = 23.151232
   Scan (420) = 20.137646

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (421) = 17.411879
   Scan (422) = 16.270447
   Scan (423) = 16.280771
   Scan (424) = 16.291285
   Scan (425) = 15.007358
   Scan (426) = 13.433167
   Scan (427) = 11.284318
   Scan (428) = 8.993586
   Scan (429) = 7.856920
   Scan (430) = 9.017387
   Scan (431) = 11.185284
   Scan (432) = 12.779974
   Scan (433) = 12.502435
   Scan (434) = 10.790740
   Scan (435) = 9.638905
   Scan (436) = 9.080371
   Scan (437) = 8.667763
   Scan (438) = 8.106710
   Scan (439) = 7.399374
   Scan (440) = 6.543478
   Scan (441) = 5.831302
   Scan (442) = 5.544868
   Scan (443) = 5.259187
   Scan (444) = 5.402841
   Scan (445) = 5.981942
   Scan (446) = 7.136522
   Scan (447) = 7.715866
   Scan (448) = 7.145439
   Scan (449) = 6.285704
   Scan (450) = 5.568599

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (451) = 4.996933
   Scan (452) = 4.135796
   Scan (453) = 3.705049
   Scan (454) = 3.709108
   Scan (455) = 3.571102
   Scan (456) = 3.574422
   Scan (457) = 3.575665
   Scan (458) = 3.578202
   Scan (459) = 3.433757
   Scan (460) = 3.432361
   Scan (461) = 3.577368
   Scan (462) = 3.721940
   Scan (463) = 3.579583
   Scan (464) = 3.293941
   Scan (465) = 3.008452
   Scan (466) = 2.854230
   Scan (467) = 2.853550
   Scan (468) = 2.854401
   Scan (469) = 2.567922
   Scan (470) = 2.279836
   Scan (471) = 2.280777
   Scan (472) = 2.137951
   Scan (473) = 1.996866
   Scan (474) = 1.855745
   Scan (475) = 1.714590
   Scan (476) = 1.716959
   Scan (477) = 1.719129
   Scan (478) = 1.722785
   Scan (479) = 1.726226
   Scan (480) = 1.585908

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (481) = 1.590679
   Scan (482) = 0.000000
   Scan (483) = 0.000000
   Scan (484) = 0.000000
   Scan (485) = 0.000000
   Scan (486) = 0.000000
   Scan (487) = 0.000000
   Scan (488) = 0.000000
   Scan (489) = 0.000000
   Scan (490) = 0.000000
   Scan (491) = 0.000000
   Scan (492) = 0.000000
   Scan (493) = 0.000000
   Scan (494) = 0.000000
   Scan (495) = 0.000000
   Scan (496) = 0.000000
   Scan (497) = 0.000000
   Scan (498) = 0.000000
   Scan (499) = 0.000000
   Scan (500) = 0.000000
   Scan (501) = 0.000000
   Scan (502) = 0.000000
   Scan (503) = 0.000000
   Scan (504) = 0.000000
   Scan (505) = 0.000000
   Scan (506) = 0.000000
   Scan (507) = 0.000000
   Scan (508) = 0.000000
   Scan (509) = 0.000000
   Scan (510) = 0.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (511) = 0.000000
   Scan (512) = 0.000000
   Scan (513) = 0.000000
   Scan (514) = 0.000000
   Scan (515) = 0.000000
   Scan (516) = 0.000000
   Scan (517) = 0.000000
   Scan (518) = 0.000000
   Scan (519) = 0.000000
   Scan (520) = 0.000000
   Scan (521) = 0.000000
   Scan (522) = 0.000000
   Scan (523) = 0.000000
   Scan (524) = 0.000000
   Scan (525) = 0.000000
   Scan (526) = 0.000000
   Scan (527) = 0.000000
   Scan (528) = 0.000000
   Scan (529) = 0.000000
   Scan (530) = 0.000000
   Scan (531) = 0.000000
   Scan (532) = 0.000000
   Scan (533) = 0.000000
   Scan (534) = 0.000000
   Scan (535) = 0.000000
   Scan (536) = 0.000000
   Scan (537) = 0.000000
   Scan (538) = 0.000000
   Scan (539) = 0.000000
   Scan (540) = 0.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (541) = 0.000000
   Scan (542) = 0.000000
   Scan (543) = 0.000000
   Scan (544) = 0.000000
   Scan (545) = 0.000000
   Scan (546) = 0.000000
   Scan (547) = 0.000000
   Scan (548) = 0.000000
   Scan (549) = 0.000000
   Scan (550) = 0.000000
   Scan (551) = 0.000000
   Scan (552) = 0.000000
   Scan (553) = 0.000000
   Scan (554) = 0.000000
   Scan (555) = 0.000000
   Scan (556) = 0.000000
   Scan (557) = 0.000000
   Scan (558) = 0.000000
   Scan (559) = 0.000000
   Scan (560) = 0.000000
   Scan (561) = 0.000000
   Scan (562) = 0.000000
   Scan (563) = 0.000000
   Scan (564) = 0.000000
   Scan (565) = 0.000000
   Scan (566) = 0.000000
   Scan (567) = 0.000000
   Scan (568) = 0.000000
   Scan (569) = 0.000000
   Scan (570) = 0.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (571) = 0.000000
   Scan (572) = 0.000000
   Scan (573) = 0.000000
   Scan (574) = 0.000000
   Scan (575) = 0.000000
   Scan (576) = 0.000000
   Scan (577) = 0.000000
   Scan (578) = 0.000000
   Scan (579) = 0.000000
   Scan (580) = 0.000000
   Scan (581) = 0.000000
   Scan (582) = 0.000000
   Scan (583) = 0.000000
   Scan (584) = 0.000000
   Scan (585) = 0.000000
   Scan (586) = 1.592244
   Scan (587) = 1.730147
   Scan (588) = 1.871286
   Scan (589) = 2.443026
   Scan (590) = 2.739478
   Scan (591) = 2.597937
   Scan (592) = 2.601373
   Scan (593) = 3.179122
   Scan (594) = 4.188626
   Scan (595) = 4.902092
   Scan (596) = 6.188459
   Scan (597) = 8.048985
   Scan (598) = 8.637594
   Scan (599) = 8.775167
   Scan (600) = 10.060072

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (601) = 12.770079
   Scan (602) = 14.609735
   Scan (603) = 16.610832
   Scan (604) = 21.026237
   Scan (605) = 25.465696
   Scan (606) = 28.322336
   Scan (607) = 32.650810
   Scan (608) = 340282346638528860000000000000000000000.000000
   Scan (609) = 340282346638528860000000000000000000000.000000
   Scan (610) = 340282346638528860000000000000000000000.000000
   Scan (611) = 340282346638528860000000000000000000000.000000
   Scan (612) = 340282346638528860000000000000000000000.000000
   Scan (613) = 340282346638528860000000000000000000000.000000
   Scan (614) = 340282346638528860000000000000000000000.000000
   Scan (615) = 340282346638528860000000000000000000000.000000
   Scan (616) = 340282346638528860000000000000000000000.000000
   Scan (617) = 340282346638528860000000000000000000000.000000
   Scan (618) = 340282346638528860000000000000000000000.000000
   Scan (619) = 340282346638528860000000000000000000000.000000
   Scan (620) = 340282346638528860000000000000000000000.000000
   Scan (621) = 340282346638528860000000000000000000000.000000
   Scan (622) = 340282346638528860000000000000000000000.000000
   Scan (623) = 340282346638528860000000000000000000000.000000
   Scan (624) = 340282346638528860000000000000000000000.000000
   Scan (625) = 340282346638528860000000000000000000000.000000
   Scan (626) = 340282346638528860000000000000000000000.000000
   Scan (627) = 340282346638528860000000000000000000000.000000
   Scan (628) = 340282346638528860000000000000000000000.000000
   Scan (629) = 340282346638528860000000000000000000000.000000
   Scan (630) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 4 (Continued)
   Scan (631) = 340282346638528860000000000000000000000.000000
   Scan (632) = 340282346638528860000000000000000000000.000000
   Scan (633) = 340282346638528860000000000000000000000.000000
   Scan (634) = 340282346638528860000000000000000000000.000000
   Scan (635) = 340282346638528860000000000000000000000.000000
   Scan (636) = 340282346638528860000000000000000000000.000000
   Scan (637) = 340282346638528860000000000000000000000.000000
   Scan (638) = 340282346638528860000000000000000000000.000000
   Scan (639) = 340282346638528860000000000000000000000.000000
   Scan (640) = 340282346638528860000000000000000000000.000000
   Scan (641) = 340282346638528860000000000000000000000.000000
   Scan (642) = 340282346638528860000000000000000000000.000000
   Scan (643) = 340282346638528860000000000000000000000.000000
   Scan (644) = 340282346638528860000000000000000000000.000000
   Scan (645) = 340282346638528860000000000000000000000.000000
   Scan (646) = 340282346638528860000000000000000000000.000000
   Scan (647) = 340282346638528860000000000000000000000.000000
   Scan (648) = 340282346638528860000000000000000000000.000000
   Scan (649) = 340282346638528860000000000000000000000.000000
   Scan (650) = 340282346638528860000000000000000000000.000000
   Scan (651) = 340282346638528860000000000000000000000.000000
   Scan (652) = 340282346638528860000000000000000000000.000000
   Scan (653) = 340282346638528860000000000000000000000.000000
   Scan (654) = 340282346638528860000000000000000000000.000000
   Scan (655) = 340282346638528860000000000000000000000.000000
   Scan (656) = 340282346638528860000000000000000000000.000000
   Scan (657) = 340282346638528860000000000000000000000.000000
   Scan (658) = 340282346638528860000000000000000000000.000000
   Scan (659) = 340282346638528860000000000000000000000.000000
   Scan (660) = 340282346638528860000000000000000000000.000000


CERES SW unfiltered radiance for Region 5
   Scan (1) = 340282346638528860000000000000000000000.000000
   Scan (2) = 340282346638528860000000000000000000000.000000
   Scan (3) = 340282346638528860000000000000000000000.000000
   Scan (4) = 340282346638528860000000000000000000000.000000
   Scan (5) = 340282346638528860000000000000000000000.000000
   Scan (6) = 340282346638528860000000000000000000000.000000
   Scan (7) = 340282346638528860000000000000000000000.000000
   Scan (8) = 340282346638528860000000000000000000000.000000
   Scan (9) = 340282346638528860000000000000000000000.000000
   Scan (10) = 340282346638528860000000000000000000000.000000
   Scan (11) = 340282346638528860000000000000000000000.000000
   Scan (12) = 340282346638528860000000000000000000000.000000
   Scan (13) = 340282346638528860000000000000000000000.000000
   Scan (14) = 340282346638528860000000000000000000000.000000
   Scan (15) = 340282346638528860000000000000000000000.000000
   Scan (16) = 340282346638528860000000000000000000000.000000
   Scan (17) = 340282346638528860000000000000000000000.000000
   Scan (18) = 340282346638528860000000000000000000000.000000
   Scan (19) = 340282346638528860000000000000000000000.000000
   Scan (20) = 340282346638528860000000000000000000000.000000
   Scan (21) = 340282346638528860000000000000000000000.000000
   Scan (22) = 340282346638528860000000000000000000000.000000
   Scan (23) = 340282346638528860000000000000000000000.000000
   Scan (24) = 340282346638528860000000000000000000000.000000
   Scan (25) = 340282346638528860000000000000000000000.000000
   Scan (26) = 340282346638528860000000000000000000000.000000
   Scan (27) = 340282346638528860000000000000000000000.000000
   Scan (28) = 340282346638528860000000000000000000000.000000
   Scan (29) = 340282346638528860000000000000000000000.000000
   Scan (30) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (31) = 340282346638528860000000000000000000000.000000
   Scan (32) = 340282346638528860000000000000000000000.000000
   Scan (33) = 340282346638528860000000000000000000000.000000
   Scan (34) = 340282346638528860000000000000000000000.000000
   Scan (35) = 340282346638528860000000000000000000000.000000
   Scan (36) = 340282346638528860000000000000000000000.000000
   Scan (37) = 340282346638528860000000000000000000000.000000
   Scan (38) = 340282346638528860000000000000000000000.000000
   Scan (39) = 340282346638528860000000000000000000000.000000
   Scan (40) = 340282346638528860000000000000000000000.000000
   Scan (41) = 340282346638528860000000000000000000000.000000
   Scan (42) = 340282346638528860000000000000000000000.000000
   Scan (43) = 340282346638528860000000000000000000000.000000
   Scan (44) = 340282346638528860000000000000000000000.000000
   Scan (45) = 340282346638528860000000000000000000000.000000
   Scan (46) = 340282346638528860000000000000000000000.000000
   Scan (47) = 340282346638528860000000000000000000000.000000
   Scan (48) = 340282346638528860000000000000000000000.000000
   Scan (49) = 340282346638528860000000000000000000000.000000
   Scan (50) = 340282346638528860000000000000000000000.000000
   Scan (51) = 340282346638528860000000000000000000000.000000
   Scan (52) = 340282346638528860000000000000000000000.000000
   Scan (53) = 340282346638528860000000000000000000000.000000
   Scan (54) = 340282346638528860000000000000000000000.000000
   Scan (55) = 340282346638528860000000000000000000000.000000
   Scan (56) = 340282346638528860000000000000000000000.000000
   Scan (57) = 340282346638528860000000000000000000000.000000
   Scan (58) = 340282346638528860000000000000000000000.000000
   Scan (59) = 340282346638528860000000000000000000000.000000
   Scan (60) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (61) = 340282346638528860000000000000000000000.000000
   Scan (62) = 340282346638528860000000000000000000000.000000
   Scan (63) = 340282346638528860000000000000000000000.000000
   Scan (64) = 340282346638528860000000000000000000000.000000
   Scan (65) = 340282346638528860000000000000000000000.000000
   Scan (66) = 340282346638528860000000000000000000000.000000
   Scan (67) = 340282346638528860000000000000000000000.000000
   Scan (68) = 340282346638528860000000000000000000000.000000
   Scan (69) = 340282346638528860000000000000000000000.000000
   Scan (70) = 42.672363
   Scan (71) = 41.035950
   Scan (72) = 37.650986
   Scan (73) = 32.091877
   Scan (74) = 28.151716
   Scan (75) = 25.676237
   Scan (76) = 23.649399
   Scan (77) = 22.498167
   Scan (78) = 23.079374
   Scan (79) = 24.523365
   Scan (80) = 25.250206
   Scan (81) = 24.097605
   Scan (82) = 21.502272
   Scan (83) = 19.782291
   Scan (84) = 20.371496
   Scan (85) = 20.963280
   Scan (86) = 20.109562
   Scan (87) = 17.527319
   Scan (88) = 14.661480
   Scan (89) = 12.661191
   Scan (90) = 12.096308

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (91) = 12.250437
   Scan (92) = 11.690817
   Scan (93) = 10.697968
   Scan (94) = 9.994000
   Scan (95) = 9.721851
   Scan (96) = 9.403571
   Scan (97) = 9.277388
   Scan (98) = 9.010470
   Scan (99) = 8.455495
   Scan (100) = 7.753297
   Scan (101) = 6.902422
   Scan (102) = 6.622241
   Scan (103) = 6.341730
   Scan (104) = 5.914967
   Scan (105) = 5.629763
   Scan (106) = 5.772771
   Scan (107) = 6.775047
   Scan (108) = 8.357805
   Scan (109) = 9.371375
   Scan (110) = 8.518034
   Scan (111) = 7.375894
   Scan (112) = 6.376624
   Scan (113) = 5.520693
   Scan (114) = 5.093510
   Scan (115) = 4.951312
   Scan (116) = 4.492874
   Scan (117) = 4.203135
   Scan (118) = 4.205348
   Scan (119) = 4.067328
   Scan (120) = 3.929066

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (121) = 3.786195
   Scan (122) = 3.786387
   Scan (123) = 3.644119
   Scan (124) = 3.215195
   Scan (125) = 3.074219
   Scan (126) = 2.788677
   Scan (127) = 2.503187
   Scan (128) = 2.646223
   Scan (129) = 2.791276
   Scan (130) = 2.792668
   Scan (131) = 2.791319
   Scan (132) = 2.791016
   Scan (133) = 2.504029
   Scan (134) = 2.211787
   Scan (135) = 1.917859
   Scan (136) = 1.771401
   Scan (137) = 1.771106
   Scan (138) = 1.631976
   Scan (139) = 1.777718
   Scan (140) = 1.923101
   Scan (141) = 2.066684
   Scan (142) = 2.211451
   Scan (143) = 2.069472
   Scan (144) = 2.069358
   Scan (145) = 1.923008
   Scan (146) = 1.919913
   Scan (147) = 1.759596
   Scan (148) = 1.756408
   Scan (149) = 1.753074
   Scan (150) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (151) = 0.000000
   Scan (152) = 0.000000
   Scan (153) = 0.000000
   Scan (154) = 0.000000
   Scan (155) = 0.000000
   Scan (156) = 0.000000
   Scan (157) = 0.000000
   Scan (158) = 0.000000
   Scan (159) = 0.000000
   Scan (160) = 0.000000
   Scan (161) = 0.000000
   Scan (162) = 0.000000
   Scan (163) = 0.000000
   Scan (164) = 0.000000
   Scan (165) = 0.000000
   Scan (166) = 0.000000
   Scan (167) = 0.000000
   Scan (168) = 0.000000
   Scan (169) = 0.000000
   Scan (170) = 0.000000
   Scan (171) = 0.000000
   Scan (172) = 0.000000
   Scan (173) = 0.000000
   Scan (174) = 0.000000
   Scan (175) = 0.000000
   Scan (176) = 0.000000
   Scan (177) = 0.000000
   Scan (178) = 0.000000
   Scan (179) = 0.000000
   Scan (180) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (181) = 0.000000
   Scan (182) = 0.000000
   Scan (183) = 0.000000
   Scan (184) = 0.000000
   Scan (185) = 0.000000
   Scan (186) = 0.000000
   Scan (187) = 0.000000
   Scan (188) = 0.000000
   Scan (189) = 0.000000
   Scan (190) = 0.000000
   Scan (191) = 0.000000
   Scan (192) = 0.000000
   Scan (193) = 0.000000
   Scan (194) = 0.000000
   Scan (195) = 0.000000
   Scan (196) = 0.000000
   Scan (197) = 0.000000
   Scan (198) = 0.000000
   Scan (199) = 0.000000
   Scan (200) = 0.000000
   Scan (201) = 0.000000
   Scan (202) = 0.000000
   Scan (203) = 0.000000
   Scan (204) = 0.000000
   Scan (205) = 0.000000
   Scan (206) = 0.000000
   Scan (207) = 0.000000
   Scan (208) = 0.000000
   Scan (209) = 0.000000
   Scan (210) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (211) = 0.000000
   Scan (212) = 0.000000
   Scan (213) = 0.000000
   Scan (214) = 0.000000
   Scan (215) = 0.000000
   Scan (216) = 0.000000
   Scan (217) = 0.000000
   Scan (218) = 0.000000
   Scan (219) = 0.000000
   Scan (220) = 0.000000
   Scan (221) = 0.000000
   Scan (222) = 0.000000
   Scan (223) = 0.000000
   Scan (224) = 0.000000
   Scan (225) = 0.000000
   Scan (226) = 0.000000
   Scan (227) = 0.000000
   Scan (228) = 0.000000
   Scan (229) = 0.000000
   Scan (230) = 0.000000
   Scan (231) = 0.000000
   Scan (232) = 0.000000
   Scan (233) = 0.000000
   Scan (234) = 0.000000
   Scan (235) = 0.000000
   Scan (236) = 0.000000
   Scan (237) = 0.000000
   Scan (238) = 0.000000
   Scan (239) = 0.000000
   Scan (240) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (241) = 0.000000
   Scan (242) = 0.000000
   Scan (243) = 0.000000
   Scan (244) = 0.000000
   Scan (245) = 0.000000
   Scan (246) = 0.000000
   Scan (247) = 0.000000
   Scan (248) = 0.000000
   Scan (249) = 0.000000
   Scan (250) = 0.000000
   Scan (251) = 0.000000
   Scan (252) = 0.000000
   Scan (253) = 0.000000
   Scan (254) = 0.000000
   Scan (255) = 0.000000
   Scan (256) = 0.000000
   Scan (257) = 0.000000
   Scan (258) = 0.000000
   Scan (259) = 0.000000
   Scan (260) = 0.000000
   Scan (261) = 0.000000
   Scan (262) = 0.000000
   Scan (263) = 0.000000
   Scan (264) = 0.000000
   Scan (265) = 0.000000
   Scan (266) = 0.000000
   Scan (267) = 0.000000
   Scan (268) = 0.000000
   Scan (269) = 0.000000
   Scan (270) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (271) = 0.000000
   Scan (272) = 0.000000
   Scan (273) = 1.379385
   Scan (274) = 1.529261
   Scan (275) = 1.821648
   Scan (276) = 2.255322
   Scan (277) = 2.541801
   Scan (278) = 2.830047
   Scan (279) = 3.149802
   Scan (280) = 4.292508
   Scan (281) = 4.711584
   Scan (282) = 4.268099
   Scan (283) = 4.542079
   Scan (284) = 5.474901
   Scan (285) = 6.470273
   Scan (286) = 7.894053
   Scan (287) = 10.029873
   Scan (288) = 12.731207
   Scan (289) = 340282346638528860000000000000000000000.000000
   Scan (290) = 340282346638528860000000000000000000000.000000
   Scan (291) = 340282346638528860000000000000000000000.000000
   Scan (292) = 340282346638528860000000000000000000000.000000
   Scan (293) = 340282346638528860000000000000000000000.000000
   Scan (294) = 340282346638528860000000000000000000000.000000
   Scan (295) = 340282346638528860000000000000000000000.000000
   Scan (296) = 340282346638528860000000000000000000000.000000
   Scan (297) = 340282346638528860000000000000000000000.000000
   Scan (298) = 340282346638528860000000000000000000000.000000
   Scan (299) = 340282346638528860000000000000000000000.000000
   Scan (300) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (301) = 340282346638528860000000000000000000000.000000
   Scan (302) = 340282346638528860000000000000000000000.000000
   Scan (303) = 340282346638528860000000000000000000000.000000
   Scan (304) = 340282346638528860000000000000000000000.000000
   Scan (305) = 340282346638528860000000000000000000000.000000
   Scan (306) = 340282346638528860000000000000000000000.000000
   Scan (307) = 340282346638528860000000000000000000000.000000
   Scan (308) = 340282346638528860000000000000000000000.000000
   Scan (309) = 340282346638528860000000000000000000000.000000
   Scan (310) = 340282346638528860000000000000000000000.000000
   Scan (311) = 340282346638528860000000000000000000000.000000
   Scan (312) = 340282346638528860000000000000000000000.000000
   Scan (313) = 340282346638528860000000000000000000000.000000
   Scan (314) = 340282346638528860000000000000000000000.000000
   Scan (315) = 340282346638528860000000000000000000000.000000
   Scan (316) = 340282346638528860000000000000000000000.000000
   Scan (317) = 340282346638528860000000000000000000000.000000
   Scan (318) = 340282346638528860000000000000000000000.000000
   Scan (319) = 340282346638528860000000000000000000000.000000
   Scan (320) = 340282346638528860000000000000000000000.000000
   Scan (321) = 340282346638528860000000000000000000000.000000
   Scan (322) = 340282346638528860000000000000000000000.000000
   Scan (323) = 340282346638528860000000000000000000000.000000
   Scan (324) = 340282346638528860000000000000000000000.000000
   Scan (325) = 340282346638528860000000000000000000000.000000
   Scan (326) = 340282346638528860000000000000000000000.000000
   Scan (327) = 340282346638528860000000000000000000000.000000
   Scan (328) = 340282346638528860000000000000000000000.000000
   Scan (329) = 340282346638528860000000000000000000000.000000
   Scan (330) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (331) = 340282346638528860000000000000000000000.000000
   Scan (332) = 340282346638528860000000000000000000000.000000
   Scan (333) = 340282346638528860000000000000000000000.000000
   Scan (334) = 340282346638528860000000000000000000000.000000
   Scan (335) = 340282346638528860000000000000000000000.000000
   Scan (336) = 340282346638528860000000000000000000000.000000
   Scan (337) = 340282346638528860000000000000000000000.000000
   Scan (338) = 340282346638528860000000000000000000000.000000
   Scan (339) = 340282346638528860000000000000000000000.000000
   Scan (340) = 340282346638528860000000000000000000000.000000
   Scan (341) = 340282346638528860000000000000000000000.000000
   Scan (342) = 340282346638528860000000000000000000000.000000
   Scan (343) = 340282346638528860000000000000000000000.000000
   Scan (344) = 340282346638528860000000000000000000000.000000
   Scan (345) = 340282346638528860000000000000000000000.000000
   Scan (346) = 340282346638528860000000000000000000000.000000
   Scan (347) = 340282346638528860000000000000000000000.000000
   Scan (348) = 340282346638528860000000000000000000000.000000
   Scan (349) = 340282346638528860000000000000000000000.000000
   Scan (350) = 340282346638528860000000000000000000000.000000
   Scan (351) = 340282346638528860000000000000000000000.000000
   Scan (352) = 340282346638528860000000000000000000000.000000
   Scan (353) = 340282346638528860000000000000000000000.000000
   Scan (354) = 340282346638528860000000000000000000000.000000
   Scan (355) = 340282346638528860000000000000000000000.000000
   Scan (356) = 340282346638528860000000000000000000000.000000
   Scan (357) = 340282346638528860000000000000000000000.000000
   Scan (358) = 340282346638528860000000000000000000000.000000
   Scan (359) = 340282346638528860000000000000000000000.000000
   Scan (360) = 340282346638528860000000000000000000000.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (361) = 340282346638528860000000000000000000000.000000
   Scan (362) = 340282346638528860000000000000000000000.000000
   Scan (363) = 340282346638528860000000000000000000000.000000
   Scan (364) = 340282346638528860000000000000000000000.000000
   Scan (365) = 340282346638528860000000000000000000000.000000
   Scan (366) = 340282346638528860000000000000000000000.000000
   Scan (367) = 340282346638528860000000000000000000000.000000
   Scan (368) = 340282346638528860000000000000000000000.000000
   Scan (369) = 340282346638528860000000000000000000000.000000
   Scan (370) = 340282346638528860000000000000000000000.000000
   Scan (371) = 340282346638528860000000000000000000000.000000
   Scan (372) = 340282346638528860000000000000000000000.000000
   Scan (373) = 340282346638528860000000000000000000000.000000
   Scan (374) = 340282346638528860000000000000000000000.000000
   Scan (375) = 340282346638528860000000000000000000000.000000
   Scan (376) = 340282346638528860000000000000000000000.000000
   Scan (377) = 340282346638528860000000000000000000000.000000
   Scan (378) = 340282346638528860000000000000000000000.000000
   Scan (379) = 340282346638528860000000000000000000000.000000
   Scan (380) = 340282346638528860000000000000000000000.000000
   Scan (381) = 340282346638528860000000000000000000000.000000
   Scan (382) = 340282346638528860000000000000000000000.000000
   Scan (383) = 340282346638528860000000000000000000000.000000
   Scan (384) = 340282346638528860000000000000000000000.000000
   Scan (385) = 340282346638528860000000000000000000000.000000
   Scan (386) = 340282346638528860000000000000000000000.000000
   Scan (387) = 340282346638528860000000000000000000000.000000
   Scan (388) = 340282346638528860000000000000000000000.000000
   Scan (389) = 9.201614
   Scan (390) = 9.335083

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (391) = 8.754160
   Scan (392) = 7.606033
   Scan (393) = 6.175421
   Scan (394) = 4.890772
   Scan (395) = 4.175141
   Scan (396) = 3.599180
   Scan (397) = 3.310809
   Scan (398) = 3.168453
   Scan (399) = 3.174851
   Scan (400) = 3.179501
   Scan (401) = 3.041026
   Scan (402) = 3.042753
   Scan (403) = 3.045624
   Scan (404) = 2.904274
   Scan (405) = 2.757760
   Scan (406) = 2.606805
   Scan (407) = 2.742704
   Scan (408) = 2.599757
   Scan (409) = 2.456878
   Scan (410) = 0.000000
   Scan (411) = 0.000000
   Scan (412) = 0.000000
   Scan (413) = 0.000000
   Scan (414) = 0.000000
   Scan (415) = 0.000000
   Scan (416) = 0.000000
   Scan (417) = 0.000000
   Scan (418) = 0.000000
   Scan (419) = 0.000000
   Scan (420) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (421) = 0.000000
   Scan (422) = 0.000000
   Scan (423) = 0.000000
   Scan (424) = 0.000000
   Scan (425) = 0.000000
   Scan (426) = 0.000000
   Scan (427) = 0.000000
   Scan (428) = 0.000000
   Scan (429) = 0.000000
   Scan (430) = 0.000000
   Scan (431) = 0.000000
   Scan (432) = 0.000000
   Scan (433) = 0.000000
   Scan (434) = 0.000000
   Scan (435) = 0.000000
   Scan (436) = 0.000000
   Scan (437) = 0.000000
   Scan (438) = 0.000000
   Scan (439) = 0.000000
   Scan (440) = 0.000000
   Scan (441) = 0.000000
   Scan (442) = 0.000000
   Scan (443) = 0.000000
   Scan (444) = 0.000000
   Scan (445) = 0.000000
   Scan (446) = 0.000000
   Scan (447) = 0.000000
   Scan (448) = 0.000000
   Scan (449) = 0.000000
   Scan (450) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (451) = 0.000000
   Scan (452) = 0.000000
   Scan (453) = 0.000000
   Scan (454) = 0.000000
   Scan (455) = 0.000000
   Scan (456) = 0.000000
   Scan (457) = 0.000000
   Scan (458) = 0.000000
   Scan (459) = 0.000000
   Scan (460) = 0.000000
   Scan (461) = 0.000000
   Scan (462) = 0.000000
   Scan (463) = 0.000000
   Scan (464) = 0.000000
   Scan (465) = 0.000000
   Scan (466) = 0.000000
   Scan (467) = 0.000000
   Scan (468) = 0.000000
   Scan (469) = 0.000000
   Scan (470) = 0.000000
   Scan (471) = 0.000000
   Scan (472) = 0.000000
   Scan (473) = 0.000000
   Scan (474) = 0.000000
   Scan (475) = 0.000000
   Scan (476) = 0.000000
   Scan (477) = 0.000000
   Scan (478) = 0.000000
   Scan (479) = 0.000000
   Scan (480) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (481) = 0.000000
   Scan (482) = 0.000000
   Scan (483) = 0.000000
   Scan (484) = 0.000000
   Scan (485) = 0.000000
   Scan (486) = 0.000000
   Scan (487) = 0.000000
   Scan (488) = 0.000000
   Scan (489) = 0.000000
   Scan (490) = 0.000000
   Scan (491) = 0.000000
   Scan (492) = 0.000000
   Scan (493) = 0.000000
   Scan (494) = 0.000000
   Scan (495) = 0.000000
   Scan (496) = 0.000000
   Scan (497) = 0.000000
   Scan (498) = 0.000000
   Scan (499) = 0.000000
   Scan (500) = 0.000000
   Scan (501) = 0.000000
   Scan (502) = 0.000000
   Scan (503) = 0.000000
   Scan (504) = 0.000000
   Scan (505) = 0.000000
   Scan (506) = 0.000000
   Scan (507) = 0.000000
   Scan (508) = 0.000000
   Scan (509) = 0.000000
   Scan (510) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (511) = 0.000000
   Scan (512) = 0.000000
   Scan (513) = 0.000000
   Scan (514) = 0.000000
   Scan (515) = 0.000000
   Scan (516) = 0.000000
   Scan (517) = 0.000000
   Scan (518) = 0.000000
   Scan (519) = 0.000000
   Scan (520) = 0.000000
   Scan (521) = 0.000000
   Scan (522) = 0.000000
   Scan (523) = 0.000000
   Scan (524) = 0.000000
   Scan (525) = 0.000000
   Scan (526) = 0.000000
   Scan (527) = 0.000000
   Scan (528) = 0.000000
   Scan (529) = 0.000000
   Scan (530) = 0.000000
   Scan (531) = 0.000000
   Scan (532) = 0.000000
   Scan (533) = 0.000000
   Scan (534) = 0.000000
   Scan (535) = 0.000000
   Scan (536) = 0.000000
   Scan (537) = 0.000000
   Scan (538) = 0.000000
   Scan (539) = 0.000000
   Scan (540) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (541) = 0.000000
   Scan (542) = 0.000000
   Scan (543) = 0.000000
   Scan (544) = 0.000000
   Scan (545) = 0.000000
   Scan (546) = 0.000000
   Scan (547) = 0.000000
   Scan (548) = 0.000000
   Scan (549) = 0.000000
   Scan (550) = 0.000000
   Scan (551) = 0.000000
   Scan (552) = 0.000000
   Scan (553) = 0.000000
   Scan (554) = 0.000000
   Scan (555) = 0.000000
   Scan (556) = 0.000000
   Scan (557) = 0.000000
   Scan (558) = 0.000000
   Scan (559) = 0.000000
   Scan (560) = 0.000000
   Scan (561) = 0.000000
   Scan (562) = 0.000000
   Scan (563) = 3.598534
   Scan (564) = 3.737396
   Scan (565) = 0.000000
   Scan (566) = 0.000000
   Scan (567) = 0.000000
   Scan (568) = 0.000000
   Scan (569) = 0.000000
   Scan (570) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (571) = 0.000000
   Scan (572) = 0.000000
   Scan (573) = 0.000000
   Scan (574) = 0.000000
   Scan (575) = 0.000000
   Scan (576) = 0.000000
   Scan (577) = 0.000000
   Scan (578) = 0.000000
   Scan (579) = 0.000000
   Scan (580) = 0.000000
   Scan (581) = 0.000000
   Scan (582) = 0.000000
   Scan (583) = 0.000000
   Scan (584) = 0.000000
   Scan (585) = 0.000000
   Scan (586) = 0.000000
   Scan (587) = 0.000000
   Scan (588) = 0.000000
   Scan (589) = 0.000000
   Scan (590) = 0.000000
   Scan (591) = 0.000000
   Scan (592) = 0.000000
   Scan (593) = 0.000000
   Scan (594) = 0.000000
   Scan (595) = 0.000000
   Scan (596) = 0.000000
   Scan (597) = 0.000000
   Scan (598) = 0.000000
   Scan (599) = 0.000000
   Scan (600) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (601) = 0.000000
   Scan (602) = 0.000000
   Scan (603) = 0.000000
   Scan (604) = 0.000000
   Scan (605) = 0.000000
   Scan (606) = 0.000000
   Scan (607) = 0.000000
   Scan (608) = 0.000000
   Scan (609) = 0.000000
   Scan (610) = 0.000000
   Scan (611) = 0.000000
   Scan (612) = 0.000000
   Scan (613) = 0.000000
   Scan (614) = 0.000000
   Scan (615) = 0.000000
   Scan (616) = 0.000000
   Scan (617) = 0.000000
   Scan (618) = 0.000000
   Scan (619) = 0.000000
   Scan (620) = 0.000000
   Scan (621) = 0.000000
   Scan (622) = 0.000000
   Scan (623) = 0.000000
   Scan (624) = 0.000000
   Scan (625) = 0.000000
   Scan (626) = 0.000000
   Scan (627) = 0.000000
   Scan (628) = 0.000000
   Scan (629) = 0.000000
   Scan (630) = 0.000000

CERES SW unfiltered radiance for Region 5 (Continued)
   Scan (631) = 0.000000
   Scan (632) = 0.000000
   Scan (633) = 0.000000
   Scan (634) = 0.000000
   Scan (635) = 0.000000
   Scan (636) = 0.000000
   Scan (637) = 0.000000
   Scan (638) = 0.000000
   Scan (639) = 0.000000
   Scan (640) = 0.000000
   Scan (641) = 0.000000
   Scan (642) = 0.000000
   Scan (643) = 0.000000
   Scan (644) = 0.000000
   Scan (645) = 0.000000
   Scan (646) = 0.000000
   Scan (647) = 0.000000
   Scan (648) = 0.000000
   Scan (649) = 0.000000
   Scan (650) = 0.000000
   Scan (651) = 0.000000
   Scan (652) = 0.000000
   Scan (653) = 0.000000
   Scan (654) = 0.000000
   Scan (655) = 0.000000
   Scan (656) = 0.000000
   Scan (657) = 0.000000
   Scan (658) = 0.000000
   Scan (659) = 0.000000
   Scan (660) = 0.000000



************************************************************
*  Example 2:   ( ONLY PRINTING MAX OF 25 REGIONS)         *
************************************************************
Region (1),  CERES SW unfiltered radiance @[200],[400] = 4.291195 0.000000
Region (2),  CERES SW unfiltered radiance @[200],[400] = 3.200427 18.964123
Region (3),  CERES SW unfiltered radiance @[200],[400] = 0.000000 54.054375
Region (4),  CERES SW unfiltered radiance @[200],[400] = 0.000000 39.770092
Region (5),  CERES SW unfiltered radiance @[200],[400] = 0.000000 3.179501



************************************************************
*  Example 3:   ( ONLY PRINTING MAX OF 25 REGIONS)         *
************************************************************
Time of observation
   Region (1) = 2452061.500106
   Region (2) = 2452061.500183
   Region (3) = 2452061.500259
   Region (4) = 2452061.500336
   Region (5) = 2452061.500412



************************************************************
*  Example 4:   ( ONLY PRINTING MAX OF 25 REGIONS)         *
************************************************************
Time of observation
   Region (1) = 2452061.500106
   Region (2) = 2452061.500183
   Region (3) = 2452061.500259
   Region (4) = 2452061.500336
   Region (5) = 2452061.500412



************************************************************
*  Example 5:   ( ONLY AVAILABLE on ES-8 products with a   *
*                 configuration code greater than 021018)  *
************************************************************
************************************************************
*               ( ONLY PRINTING MAX OF 25 REGIONS)         *
************************************************************
Spectral Response Functions
SW spectral response values
   Index (1) = 0.000000
   Index (2) = 0.018000
   Index (3) = 0.009000
   Index (4) = 0.006000
   Index (5) = 0.008000
   Index (6) = 0.014000
   Index (7) = 0.027000
   Index (8) = 0.043000
   Index (9) = 0.057000
   Index (10) = 0.065000
   Index (11) = 0.059000
   Index (12) = 0.036000
   Index (13) = 0.006000
   Index (14) = 0.341000
   Index (15) = 0.619000
   Index (16) = 0.689000
   Index (17) = 0.693000
   Index (18) = 0.678000
   Index (19) = 0.679000
   Index (20) = 0.725000
   Index (21) = 0.763000
   Index (22) = 0.786000
   Index (23) = 0.799000
   Index (24) = 0.800000
   Index (25) = 0.799000
   Index (26) = 0.795000
   Index (27) = 0.794000
   Index (28) = 0.795000
   Index (29) = 0.799000
   Index (30) = 0.799000

Spectral Response Functions
SW spectral response values (Continued)
   Index (31) = 0.803000
   Index (32) = 0.803000
   Index (33) = 0.805000
   Index (34) = 0.802000
   Index (35) = 0.802000
   Index (36) = 0.801000
   Index (37) = 0.799000
   Index (38) = 0.797000
   Index (39) = 0.796000
   Index (40) = 0.796000
   Index (41) = 0.795000
   Index (42) = 0.795000
   Index (43) = 0.794000
   Index (44) = 0.793000
   Index (45) = 0.790000
   Index (46) = 0.790000
   Index (47) = 0.788000
   Index (48) = 0.789000
   Index (49) = 0.787000
   Index (50) = 0.788000
   Index (51) = 0.786000
   Index (52) = 0.785000
   Index (53) = 0.786000
   Index (54) = 0.787000
   Index (55) = 0.787000
   Index (56) = 0.785000
   Index (57) = 0.785000
   Index (58) = 0.785000
   Index (59) = 0.786000
   Index (60) = 0.784000

Spectral Response Functions
SW spectral response values (Continued)
   Index (61) = 0.786000
   Index (62) = 0.787000
   Index (63) = 0.786000
   Index (64) = 0.784000
   Index (65) = 0.786000
   Index (66) = 0.782000
   Index (67) = 0.790000
   Index (68) = 0.787000
   Index (69) = 0.781000
   Index (70) = 0.783000
   Index (71) = 0.786000
   Index (72) = 0.786000
   Index (73) = 0.786000
   Index (74) = 0.786000
   Index (75) = 0.787000
   Index (76) = 0.785000
   Index (77) = 0.786000
   Index (78) = 0.787000
   Index (79) = 0.786000
   Index (80) = 0.785000
   Index (81) = 0.785000
   Index (82) = 0.786000
   Index (83) = 0.786000
   Index (84) = 0.789000
   Index (85) = 0.786000
   Index (86) = 0.789000
   Index (87) = 0.789000
   Index (88) = 0.788000
   Index (89) = 0.789000
   Index (90) = 0.789000

Spectral Response Functions
SW spectral response values (Continued)
   Index (91) = 0.791000
   Index (92) = 0.791000
   Index (93) = 0.791000
   Index (94) = 0.792000
   Index (95) = 0.793000
   Index (96) = 0.793000
   Index (97) = 0.794000
   Index (98) = 0.794000
   Index (99) = 0.794000
   Index (100) = 0.794000
   Index (101) = 0.794000
   Index (102) = 0.795000
   Index (103) = 0.795000
   Index (104) = 0.794000
   Index (105) = 0.796000
   Index (106) = 0.794000
   Index (107) = 0.795000
   Index (108) = 0.797000
   Index (109) = 0.797000
   Index (110) = 0.797000
   Index (111) = 0.799000
   Index (112) = 0.799000
   Index (113) = 0.798000
   Index (114) = 0.799000
   Index (115) = 0.802000
   Index (116) = 0.799000
   Index (117) = 0.802000
   Index (118) = 0.803000
   Index (119) = 0.803000
   Index (120) = 0.802000

Spectral Response Functions
SW spectral response values (Continued)
   Index (121) = 0.800000
   Index (122) = 0.802000
   Index (123) = 0.803000
   Index (124) = 0.803000
   Index (125) = 0.805000
   Index (126) = 0.805000
   Index (127) = 0.806000
   Index (128) = 0.807000
   Index (129) = 0.807000
   Index (130) = 0.808000
   Index (131) = 0.807000
   Index (132) = 0.808000
   Index (133) = 0.808000
   Index (134) = 0.808000
   Index (135) = 0.807000
   Index (136) = 0.807000
   Index (137) = 0.809000
   Index (138) = 0.810000
   Index (139) = 0.809000
   Index (140) = 0.809000
   Index (141) = 0.810000
   Index (142) = 0.809000
   Index (143) = 0.808000
   Index (144) = 0.810000
   Index (145) = 0.809000
   Index (146) = 0.809000
   Index (147) = 0.809000
   Index (148) = 0.808000
   Index (149) = 0.809000
   Index (150) = 0.810000

Spectral Response Functions
SW spectral response values (Continued)
   Index (151) = 0.809000
   Index (152) = 0.809000
   Index (153) = 0.808000
   Index (154) = 0.810000
   Index (155) = 0.811000
   Index (156) = 0.809000
   Index (157) = 0.810000
   Index (158) = 0.810000
   Index (159) = 0.810000
   Index (160) = 0.810000
   Index (161) = 0.810000
   Index (162) = 0.809000
   Index (163) = 0.807000
   Index (164) = 0.810000
   Index (165) = 0.811000
   Index (166) = 0.807000
   Index (167) = 0.807000
   Index (168) = 0.808000
   Index (169) = 0.809000
   Index (170) = 0.807000
   Index (171) = 0.808000
   Index (172) = 0.809000
   Index (173) = 0.804000
   Index (174) = 0.808000
   Index (175) = 0.809000
   Index (176) = 0.808000
   Index (177) = 0.808000
   Index (178) = 0.809000
   Index (179) = 0.809000
   Index (180) = 0.808000

Spectral Response Functions
SW spectral response values (Continued)
   Index (181) = 0.806000
   Index (182) = 0.807000
   Index (183) = 0.807000
   Index (184) = 0.808000
   Index (185) = 0.807000
   Index (186) = 0.807000
   Index (187) = 0.807000
   Index (188) = 0.807000
   Index (189) = 0.807000
   Index (190) = 0.807000
   Index (191) = 0.807000
   Index (192) = 0.808000
   Index (193) = 0.808000
   Index (194) = 0.805000
   Index (195) = 0.808000
   Index (196) = 0.806000
   Index (197) = 0.807000
   Index (198) = 0.807000
   Index (199) = 0.808000
   Index (200) = 0.809000
   Index (201) = 0.807000
   Index (202) = 0.808000
   Index (203) = 0.808000
   Index (204) = 0.808000
   Index (205) = 0.808000
   Index (206) = 0.808000
   Index (207) = 0.808000
   Index (208) = 0.809000
   Index (209) = 0.807000
   Index (210) = 0.809000

Spectral Response Functions
SW spectral response values (Continued)
   Index (211) = 0.808000
   Index (212) = 0.809000
   Index (213) = 0.810000
   Index (214) = 0.809000
   Index (215) = 0.811000
   Index (216) = 0.809000
   Index (217) = 0.813000
   Index (218) = 0.812000
   Index (219) = 0.809000
   Index (220) = 0.812000
   Index (221) = 0.811000
   Index (222) = 0.812000
   Index (223) = 0.812000
   Index (224) = 0.813000
   Index (225) = 0.810000
   Index (226) = 0.811000
   Index (227) = 0.810000
   Index (228) = 0.810000
   Index (229) = 0.812000
   Index (230) = 0.811000
   Index (231) = 0.811000
   Index (232) = 0.811000
   Index (233) = 0.812000
   Index (234) = 0.811000
   Index (235) = 0.810000
   Index (236) = 0.811000
   Index (237) = 0.813000
   Index (238) = 0.810000
   Index (239) = 0.809000
   Index (240) = 0.810000

Spectral Response Functions
SW spectral response values (Continued)
   Index (241) = 0.811000
   Index (242) = 0.812000
   Index (243) = 0.808000
   Index (244) = 0.811000
   Index (245) = 0.811000
   Index (246) = 0.811000
   Index (247) = 0.810000
   Index (248) = 0.813000
   Index (249) = 0.807000
   Index (250) = 0.799000
   Index (251) = 0.783000
   Index (252) = 0.772000
   Index (253) = 0.766000
   Index (254) = 0.765000
   Index (255) = 0.765000
   Index (256) = 0.767000
   Index (257) = 0.768000
   Index (258) = 0.759000
   Index (259) = 0.759000
   Index (260) = 0.771000
   Index (261) = 0.765000
   Index (262) = 0.768000
   Index (263) = 0.772000
   Index (264) = 0.770000
   Index (265) = 0.777000
   Index (266) = 0.772000
   Index (267) = 0.772000
   Index (268) = 0.777000
   Index (269) = 0.772000
   Index (270) = 0.775000

Spectral Response Functions
SW spectral response values (Continued)
   Index (271) = 0.771000
   Index (272) = 0.774000
   Index (273) = 0.775000
   Index (274) = 0.769000
   Index (275) = 0.770000
   Index (276) = 0.767000
   Index (277) = 0.763000
   Index (278) = 0.770000
   Index (279) = 0.769000
   Index (280) = 0.766000
   Index (281) = 0.766000
   Index (282) = 0.765000
   Index (283) = 0.767000
   Index (284) = 0.763000
   Index (285) = 0.764000
   Index (286) = 0.768000
   Index (287) = 0.760000
   Index (288) = 0.767000
   Index (289) = 0.764000
   Index (290) = 0.762000
   Index (291) = 0.770000
   Index (292) = 0.757000
   Index (293) = 0.760000
   Index (294) = 0.761000
   Index (295) = 0.766000
   Index (296) = 0.761000
   Index (297) = 0.765000
   Index (298) = 0.760000
   Index (299) = 0.770000
   Index (300) = 0.769000

Spectral Response Functions
SW spectral response values (Continued)
   Index (301) = 0.765000
   Index (302) = 0.763000
   Index (303) = 0.765000
   Index (304) = 0.770000
   Index (305) = 0.767000
   Index (306) = 0.764000
   Index (307) = 0.772000
   Index (308) = 0.768000
   Index (309) = 0.771000
   Index (310) = 0.769000
   Index (311) = 0.767000
   Index (312) = 0.767000
   Index (313) = 0.770000
   Index (314) = 0.773000
   Index (315) = 0.771000
   Index (316) = 0.769000
   Index (317) = 0.768000
   Index (318) = 0.778000
   Index (319) = 0.770000
   Index (320) = 0.772000
   Index (321) = 0.767000
   Index (322) = 0.767000
   Index (323) = 0.769000
   Index (324) = 0.762000
   Index (325) = 0.766000
   Index (326) = 0.755000
   Index (327) = 0.755000
   Index (328) = 0.754000
   Index (329) = 0.750000
   Index (330) = 0.744000

Spectral Response Functions
SW spectral response values (Continued)
   Index (331) = 0.741000
   Index (332) = 0.741000
   Index (333) = 0.732000
   Index (334) = 0.730000
   Index (335) = 0.724000
   Index (336) = 0.722000
   Index (337) = 0.714000
   Index (338) = 0.711000
   Index (339) = 0.708000
   Index (340) = 0.709000
   Index (341) = 0.706000
   Index (342) = 0.703000
   Index (343) = 0.690000
   Index (344) = 0.688000
   Index (345) = 0.677000
   Index (346) = 0.674000
   Index (347) = 0.670000
   Index (348) = 0.663000
   Index (349) = 0.647000
   Index (350) = 0.651000
   Index (351) = 0.642000
   Index (352) = 0.632000
   Index (353) = 0.622000
   Index (354) = 0.623000
   Index (355) = 0.619000
   Index (356) = 0.611000
   Index (357) = 0.608000
   Index (358) = 0.616000
   Index (359) = 0.611000
   Index (360) = 0.607000

Spectral Response Functions
SW spectral response values (Continued)
   Index (361) = 0.611000
   Index (362) = 0.611000
   Index (363) = 0.608000
   Index (364) = 0.608000
   Index (365) = 0.605000
   Index (366) = 0.613000
   Index (367) = 0.605000
   Index (368) = 0.600000
   Index (369) = 0.597000
   Index (370) = 0.595000
   Index (371) = 0.602000
   Index (372) = 0.597000
   Index (373) = 0.607000
   Index (374) = 0.596000
   Index (375) = 0.596000
   Index (376) = 0.597000
   Index (377) = 0.595000
   Index (378) = 0.597000
   Index (379) = 0.605000
   Index (380) = 0.590000
   Index (381) = 0.591000
   Index (382) = 0.586000
   Index (383) = 0.573000
   Index (384) = 0.574000
   Index (385) = 0.568000
   Index (386) = 0.558000
   Index (387) = 0.546000
   Index (388) = 0.536000
   Index (389) = 0.523000
   Index (390) = 0.520000

Spectral Response Functions
SW spectral response values (Continued)
   Index (391) = 0.507000
   Index (392) = 0.499000
   Index (393) = 0.491000
   Index (394) = 0.481000
   Index (395) = 0.473000
   Index (396) = 0.463000
   Index (397) = 0.465000
   Index (398) = 0.452000
   Index (399) = 0.449000
   Index (400) = 0.448000
   Index (401) = 0.438000
   Index (402) = 0.434000
   Index (403) = 0.426000
   Index (404) = 0.423000
   Index (405) = 0.413000
   Index (406) = 0.402000
   Index (407) = 0.394000
   Index (408) = 0.376000
   Index (409) = 0.365000
   Index (410) = 0.347000
   Index (411) = 0.334000
   Index (412) = 0.312000
   Index (413) = 0.306000
   Index (414) = 0.284000
   Index (415) = 0.270000
   Index (416) = 0.257000
   Index (417) = 0.244000
   Index (418) = 0.239000
   Index (419) = 0.227000
   Index (420) = 0.223000

Spectral Response Functions
SW spectral response values (Continued)
   Index (421) = 0.215000
   Index (422) = 0.205000
   Index (423) = 0.201000
   Index (424) = 0.203000
   Index (425) = 0.207000
   Index (426) = 0.207000
   Index (427) = 0.213000
   Index (428) = 0.221000
   Index (429) = 0.225000
   Index (430) = 0.232000
   Index (431) = 0.237000
   Index (432) = 0.248000
   Index (433) = 0.249000
   Index (434) = 0.266000
   Index (435) = 0.263000
   Index (436) = 0.267000
   Index (437) = 0.274000
   Index (438) = 0.269000
   Index (439) = 0.276000
   Index (440) = 0.276000
   Index (441) = 0.269000
   Index (442) = 0.261000
   Index (443) = 0.253000
   Index (444) = 0.250000
   Index (445) = 0.242000
   Index (446) = 0.224000
   Index (447) = 0.218000
   Index (448) = 0.204000
   Index (449) = 0.193000
   Index (450) = 0.178000

Spectral Response Functions
SW spectral response values (Continued)
   Index (451) = 0.175000
   Index (452) = 0.151000
   Index (453) = 0.133000
   Index (454) = 0.122000
   Index (455) = 0.110000
   Index (456) = 0.085000
   Index (457) = 0.074000
   Index (458) = 0.046000
   Index (459) = 0.039000
   Index (460) = 0.030000
   Index (461) = 0.019000
   Index (462) = 0.003000
   Index (463) = 0.008000
   Index (464) = 0.004000
   Index (465) = 0.006000
   Index (466) = 0.001000
   Index (467) = -0.002000
   Index (468) = 0.000000
   Index (469) = 0.005000
   Index (470) = 0.000000
   Index (471) = 0.000000
   Index (472) = 0.000000
   Index (473) = 0.001000
   Index (474) = 0.000000
   Index (475) = 0.002000
   Index (476) = -0.001000
   Index (477) = -0.002000
   Index (478) = -0.002000
   Index (479) = -0.005000
   Index (480) = -0.006000

Spectral Response Functions
SW spectral response values (Continued)
   Index (481) = 0.001000
   Index (482) = 0.001000
   Index (483) = 0.002000
   Index (484) = 0.002000
   Index (485) = 0.002000
   Index (486) = 0.002000
   Index (487) = 0.002000
   Index (488) = 0.003000
   Index (489) = 0.003000
   Index (490) = 0.005000
   Index (491) = 0.007000
   Index (492) = 0.010000
   Index (493) = 0.012000
   Index (494) = 0.012000
   Index (495) = 0.015000
   Index (496) = 0.016000
   Index (497) = 0.017000
   Index (498) = 0.023000
   Index (499) = 0.026000
   Index (500) = 0.030000
   Index (501) = 0.033000
   Index (502) = 0.036000
   Index (503) = 0.039000
   Index (504) = 0.041000
   Index (505) = 0.045000
   Index (506) = 0.049000
   Index (507) = 0.053000
   Index (508) = 0.058000
   Index (509) = 0.062000
   Index (510) = 0.066000

Spectral Response Functions
SW spectral response values (Continued)
   Index (511) = 0.068000
   Index (512) = 0.073000
   Index (513) = 0.077000
   Index (514) = 0.089000
   Index (515) = 0.083000
   Index (516) = 0.095000
   Index (517) = 0.091000
   Index (518) = 0.103000
   Index (519) = 0.114000
   Index (520) = 0.113000
   Index (521) = 0.115000
   Index (522) = 0.115000
   Index (523) = 0.121000
   Index (524) = 0.129000
   Index (525) = 0.135000
   Index (526) = 0.149000
   Index (527) = 0.142000
   Index (528) = 0.130000
   Index (529) = 0.161000
   Index (530) = 0.160000
   Index (531) = 0.169000
   Index (532) = 0.159000
   Index (533) = 0.161000
   Index (534) = 0.185000
   Index (535) = 0.191000
   Index (536) = 0.197000
   Index (537) = 0.193000
   Index (538) = 0.201000
   Index (539) = 0.219000
   Index (540) = 0.201000

Spectral Response Functions
SW spectral response values (Continued)
   Index (541) = 0.202000
   Index (542) = 0.231000
   Index (543) = 0.228000
   Index (544) = 0.237000
   Index (545) = 0.237000
   Index (546) = 0.229000
   Index (547) = 0.239000
   Index (548) = 0.261000
   Index (549) = 0.271000
   Index (550) = 0.263000
   Index (551) = 0.273000
   Index (552) = 0.289000
   Index (553) = 0.277000
   Index (554) = 0.289000
   Index (555) = 0.298000
   Index (556) = 0.268000
   Index (557) = 0.290000
   Index (558) = 0.289000
   Index (559) = 0.283000
   Index (560) = 0.280000
   Index (561) = 0.299000
   Index (562) = 0.307000
   Index (563) = 0.307000
   Index (564) = 0.301000
   Index (565) = 0.314000
   Index (566) = 0.330000
   Index (567) = 0.327000
   Index (568) = 0.347000
   Index (569) = 0.320000
   Index (570) = 0.341000

Spectral Response Functions
SW spectral response values (Continued)
   Index (571) = 0.352000
   Index (572) = 0.343000
   Index (573) = 0.327000
   Index (574) = 0.364000
   Index (575) = 0.366000
   Index (576) = 0.383000
   Index (577) = 0.373000
   Index (578) = 0.355000
   Index (579) = 0.357000
   Index (580) = 0.377000
   Index (581) = 0.399000
   Index (582) = 0.393000
   Index (583) = 0.379000
   Index (584) = 0.376000
   Index (585) = 0.386000
   Index (586) = 0.396000
   Index (587) = 0.396000
   Index (588) = 0.424000
   Index (589) = 0.403000
   Index (590) = 0.388000
   Index (591) = 0.403000
   Index (592) = 0.419000
   Index (593) = 0.435000
   Index (594) = 0.435000
   Index (595) = 0.426000
   Index (596) = 0.408000
   Index (597) = 0.414000
   Index (598) = 0.419000
   Index (599) = 0.450000
   Index (600) = 0.459000

Spectral Response Functions
SW spectral response values (Continued)
   Index (601) = 0.468000
   Index (602) = 0.479000
   Index (603) = 0.452000
   Index (604) = 0.424000
   Index (605) = 0.431000
   Index (606) = 0.467000
   Index (607) = 0.479000
   Index (608) = 0.492000
   Index (609) = 0.476000
   Index (610) = 0.481000
   Index (611) = 0.486000
   Index (612) = 0.440000
   Index (613) = 0.456000
   Index (614) = 0.472000
   Index (615) = 0.493000
   Index (616) = 0.486000
   Index (617) = 0.479000
   Index (618) = 0.474000
   Index (619) = 0.469000
   Index (620) = 0.493000
   Index (621) = 0.517000
   Index (622) = 0.491000
   Index (623) = 0.510000
   Index (624) = 0.529000
   Index (625) = 0.531000
   Index (626) = 0.517000
   Index (627) = 0.502000
   Index (628) = 0.491000
   Index (629) = 0.480000
   Index (630) = 0.500000

Spectral Response Functions
SW spectral response values (Continued)
   Index (631) = 0.521000
   Index (632) = 0.542000



 8.0 Document history

     Version 0.00
        UnKnown
     Version 1.00
        Dale Walikainen @SAIC
        March 05, 2004
     Version 2.00
        Updated for new data sets (Edition3, FM5), websites, contacts
        Jeremie Lande @SSAI
        20 Apr 2012