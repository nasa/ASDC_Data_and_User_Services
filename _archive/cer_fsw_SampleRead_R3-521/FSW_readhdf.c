/*
 *****************************************************************************
 *  Program: FSW_readhdf.c
 *
 *  Purpose: To provide examples of how to read data from the FSW HDF file.
 *           The HDF product for TRMM consists of eight files, each containing
 *           data for ten 1.0-degree equal-angle zones.  The HDF product for 
 *           Terra and Aqua consists of 60 files, each containing data for 
 *           three 1.0-degree equal-angle zones.  Each record contains 
 *           spatially averaged data for an individual region. 
 *           The hours are set to maximum hours a region has in the file.
 *           The SDSs in the file are declared as either 3 or 4 dimensional 
 *           arrays. 
 *           3-D (#hours,zones,regions); 4-D (3,#hours,zones,regions) 
 *                                        where 3 = {mean,std,num_obs}
 *
 *           The Cloud Layers (High,UpperMid,LowerMid,Low) SDS names are same
 *           for all four cloud layers. Using the SDS name to read data will 
 *           read only HIGH Cloud Layer SDSs. Therefore Example 5 illustrates
 *           how to read Cloud Layer SDS using SDS index. 
 *
 *            
 *  There are 6 examples of how to read data from the SDSs and Vdatas on the
 *  FSW HDF file.
 *
 *  Example 1) Illustrates reading real data from an SDS of rank 3
 *             (3-dimensional) and storing the data in an array and
 *             writing the data to the standard output file.
 *  Example 2) Illustrates reading real data from an SDS of rank 4 
 *             (4-dimensional) and storing the data in an array.
 *  Example 3) Illustrates reading real data from an SDS of rank 3 and 
 *             writing the data to the standard output file.
 *  Example 4) Illustrates reading real data from an SDS of rank 3 and 
 *             writing the data to the standard output file.
 *  Example 5) Illustrates reading Cloud Layer SDS data of rank 4 using SDS 
 *             index and 
 *             writing the data to the standard output file.
 *  Example 6) Illustrates reading string data from a Vdata field and 
 *             storing the data in an array. 
 *
 *
 *  An alternate way of extracting the data from an HDF file is through the 
 *  "hdp" utility which is a part of the HDF libraries.  The "hdp" utility
 *  will give information as to the size, type, order, and names of the 
 *  HDF data structures in addition to the data itself.  Some examples are:
 *
 *        "hdp -H"      - provides "help" on how to use the "hdp" utility
 *        "hdp dumpvd"  - lists the options and format for viewing Vdatas
 *        "hdp dumpsds" - lists the options and format for viewing SDSs
 *
 *  The examples require specification of the 'type' of data to be read
 *  passed as a parameter to the HDF routine. The HDF data types are
 *  defined in the following table. The names (first column) or numeric
 *  values (center column) are valid parameter values; either can be
 *  used in C programs since the names are defined with the appropriate
 *  value in an HDF include file.  Function sds_get_dims is used to return 
 *  the value of the 'type', sds_type, which is used in the calls to read 
 *  the SDS. 
 *
 *                         HDF Data Types
 *
 *  Definition Name   |   Definition Value   |       Description
 *  ------------------|----------------------|--------------------------------
 *  DFNT_CHAR8        |          4           |   8-bit character type
 *  DFNT_CHAR         |          4           |   Same as DFNT_CHAR8
 *  DFNT_UCHAR8       |          3           |   8-bit unsigned character type
 *  DFNT_UCHAR        |          3           |   Same as DFNT_UCHAR8
 *  DFNT_INT8         |         20           |   8-bit integer type
 *  DFNT_UINT8        |         21           |   8-bit unsigned integer type
 *  DFNT_INT16        |         22           |   16-bit integer type
 *  DFNT_UINT16       |         23           |   16-bit unsigned integer type
 *  DFNT_INT32        |         24           |   32-bit integer type
 *  DFNT_UINT32       |         25           |   32-bit unsigned integer type
 *  DFNT_INT64        |         26           |   64-bit integer type
 *  DFNT_UINT64       |         27           |   64-bit unsigned integer type
 *  DFNT_FLOAT32      |          5           |   32-bit floating-point type
 *  DFNT_FLOAT64      |          6           |   64-bit floating-point type
 *
 *  To modify the code to extract different SDSs than what are in the examples, 
 *  the following variables must be changed:
 *   HDF_File - FSW HDF file name - modify this if a different FSW HDF file is
 *              being read
 *   sds_name - SDS name - see the Data Products Catalog ("FSW_dpc_r3.ps") or 
 *              the provided dump of the HDF file ("CER_FSW_Sample_R3V4.dump") 
 *              for valid SDS names in the FSW product
 *   sds_strt - array containing the starting location in each dimension 
 *              at which the data read begins.  It is zero based.  In all
 *              examples, sds_strt for each dimension is set to zero so
 *              reads start at the beginning element of each dimension.  
 *              To read in a partial number of footprints, change sds_strt[0] 
 *              to the number of the first footprint to be read.
 *   sds_edge - sds_edge is set to the number of values to be read in each
 *              dimension.  This sample program sets them to the total
 *              number of elements in each dimension, so all data are read.  
 *              To read in a partial number of footprints, set sds_edge[0]
 *              to the number of the footprints to be read.   
 *   sds_incr - array containing the increments along the dimensions for which
 *              data is to be read.  For these examples, the values for the
 *              sds_incr array are set to one, so no data are skipped.  To
 *              read every third value along a dimension, skipping the two
 *              values in between, set sds_incr for that dimension to 3.
 *   The array to hold the values read from an SDS must be defined with the
 *   correct dimension and this array must be used in place of the one used in
 *   Example 1 or 2.  For Example 3, the same pointer, sds_data, can be used, no
 *   matter which SDS is requested.
*/

   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <math.h>
   #include <float.h>
   #include "FSW_HDFread.h"

/*
 *******************************************************************************
 *  MAIN PROGRAM
 *******************************************************************************
*/

  /* void main()*/
   void main( int argc, char *argv[] )

   {

     char *HDF_File;
     char *sds_name;
     char *data_name;
     char *vdata_name;
     char *vfield_name;
     int32 rank;
     int32 type;
     int32 sds_status;
     int32 sds_rank;
     int32 sds_type;
     int32 sds_dims[MAX_SDS_DIMS];
     int32 sds_strt[MAX_SDS_DIMS];
     int32 sds_incr[MAX_SDS_DIMS];
     int32 sds_edge[MAX_SDS_DIMS];
     int32 num_fields;
     int32 rd_num_fields;
     int32 rd_num_recs;
     int32 vfield_types[MAX_VDATA_FIELDS];
     int32 vfield_order[MAX_VDATA_FIELDS];
     int32 vfield_status;
     long  vdata_status;
     int32 i,j,k,l,hr,reg,zn,indx,indx2,region_num;
     int32 index,stats,typ,hour;
     long  vdata_n_rec;
     void *sds_data;
     void *vdata_data[MAX_VDATA_FIELDS];

/*
 *******************************************************************************
 *  Arrays assigned for Examples 1, 2, 4, and 5.
 *******************************************************************************
*/
     float32 * hour_box;
     float32 * region;
     float32 * surface_types;
     char    platform[32]; 
     float32 * total_sky;
     float32 *eff_pres_H;
     float32 *eff_pres_UM;
     float32 *eff_pres_LM;
     float32 *eff_pres_L;
/*
 *******************************************************************************
 *  Assign the 5-footprint test HDF file to be read.  If another HDF file is to
 *  be read, enter its name here.
 *******************************************************************************

     HDF_File = "CER_FSW_TRMM_Sample_R3V4"; 
*/
/* ===========================================================================
Assign the HDF file to be read
============================================================================*/

  if(argc != 2)
  {
     fprintf ( stderr, "Usage: FSW_rd HDFfilename\n");
     exit(1);
  }

  HDF_File = argv[1];

  printf("\nName of HDF file to be read:\n< %s\n",HDF_File);

/*
 *******************************************************************************
 *  Print the name of the file being read and check the validity of the file. 
 *******************************************************************************
*/

     if (!Hishdf(HDF_File))
     {
       fprintf(stderr, "ERROR - specified file is not an HDF file.\n");
       exit (1);
     } 
  
/*
 *******************************************************************************
 *  Example 1)  Example of how to extract an SDS of rank 3 (3-dimensional)
 *              from the HDF file and store it in memory.  The SDS, 
 *             "Hour box Number", will be extracted and stored in 
 *             the array "hour_box".  
 *******************************************************************************
 *
 * This example uses function 'read_sds' and 'get_sds_dims'.
 * 
 * 'get_sds_dims'
 *    Function - Return the values of sds_type, sds_rank, and sds_edge given
 *               the sds_name and HDF_File names.
 *    Input Variables:
 *       HDF_File - character pointer to the name of the HDF file
 *       sds_name - character pointer to the name of the SDS data set
 *    Output Variable:
 *       rank     - integer defining rank of the SDS data set
 *       type     - integer defining type of SDS data set (see table above)
 *       sds_dims - integer array of number of values of each dimension
 *                  of the SDS.
 *
 * 'read_sds'
 *    Function - Reads an SDS data set from an HDF file.
 *    Input Variables:
 *       HDF_File - character pointer to the name of the HDF file
 *       sds_name - character pointer to the name of the SDS data set
 *       sds_rank - integer defining rank of the SDS data set
 *       sds_type - integer defining type of SDS data set (see table above)
 *       sds_strt - integer array of indices where read will start for each 
 *                  dimension of the SDS
 *       sds_incr - integer array of increments for each dimension of the SDS
 *       sds_edge - integer array of number of values to be read in each 
 *                  dimension of the SDS
 *    Output Variable:
 *       hour_box - output array of type 'sds_type' and rank 'sds_rank'
 *
 * Note: This SDS data can also be accessed using SDS index (7) (See Example 5)
 *
 *****************************************************************************
*/
     sds_name    = "Hour Box Number";
     get_sds_dims(HDF_File,sds_name,&rank,sds_dims,&type);
     sds_rank = rank;
     sds_type = type;

/*
   In all examples, sds_edge is set to the number of values in each dimension,
   so all data is read.  To read in partial hour boxes of data, 
   set sds_edge[0] to the number of hour boxes to be read and change 
   sds_strt[0] to the number of the first hour box to be read.   
*/
     
     sds_edge[0] = sds_dims[0]; 
     sds_edge[1] = sds_dims[1];
     sds_edge[2] = sds_dims[2];     
     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_strt[2] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;
     sds_incr[2] = 1;
     hour_box = (float32 *)malloc (sds_dims[0] * sds_dims[1] * 
                   sds_dims[2] * sizeof(float32));

/*
 *****************************************************************************
 *  Read the "hour box numbers" ( Observation Hours ) SDS into the 3-D
 *  "hour_box" array.
 *****************************************************************************
*/
      sds_status = read_sds(HDF_File, sds_name, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, hour_box);

     if ( sds_status != 0 )
     {
        fprintf (stderr, "\nError in reading SDS: \"%s\" ", sds_name);
        exit(1);
     }
     fprintf (stdout,
       "\n\n\n*********************************************************\n");
     fprintf (stdout,
       "*    Example 1:                                         *\n");
     fprintf (stdout,
       "*********************************************************");
     for (zn=0;zn<sds_dims[1];zn++)
       {
        for (reg=0;reg<sds_dims[2]; reg++)
          {
           for (hr=0; hr<sds_dims[0]; hr++)
             {
              indx = hr*sds_dims[1]*sds_dims[2] + zn * sds_dims[2]+ reg;
                         
              printf ("\nHour Box %i\n",hr+1);
              printf ("Hour Box Number  = %i\n",(int) hour_box[indx]);    
             }
          }

       }


/*
 *******************************************************************************
 *  Example 2) Example of how to extract an SDS of rank 4 ( 4-dimensional )
 *             from the HDF file and store it in memory.  The SDS,
 *             "Surface type percent coverage", will be extracted and stored
 *             in the 4-D array, "surface_types".  The differences between
 *             Example 2 and Example 1 are:
 *                1.  The values sds_incr[1], sds_edge[1], and sds_strt[1]
 *                    must be set for this 4-D SDS.
 *                2.  The values for sds_name, sds_rank, and sds_type are now
 *                    set for the 4-D SDS, "Surface type percent coverage".
 *******************************************************************************
 * This example uses function 'read_sds' and 'get_sds_dims'.
 *
 * See example 1 for information on these functions.
 *
 * Note: This SDS data can also be accessed using SDS index (12) (See Example 5)
 *******************************************************************************
*/  
     sds_name = "Surface Type Percentage";
     get_sds_dims(HDF_File,sds_name,&rank,sds_dims,&type);
     sds_rank = rank;
     sds_type = type;
     sds_edge[0] = sds_dims[0];
     sds_edge[1] = sds_dims[1];
     sds_edge[2] = sds_dims[2];
     sds_edge[3] = sds_dims[3];
     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_strt[2] = 0;
     sds_strt[3] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;
     sds_incr[2] = 1;
     sds_incr[3] = 1;

/*
 *****************************************************************************
 *  Read the "Surface Type Percentage coverage" SDS into the 4-D 
 *  "surface_types" array.
 *****************************************************************************
*/
     surface_types = (float32 *)malloc ( sds_dims[0] * sds_dims[1] * 
                     sds_dims[2] * sds_dims[3] * sizeof(float32));
     sds_status = read_sds(HDF_File, sds_name, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, surface_types);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%s\" ", sds_name);
        exit(1);
     }

/*
 *****************************************************************************
 *  Print the values in the "Surface type percent coverage" SDS, stored in
 *  the 4-D array, "surface_types".
 *****************************************************************************
*/
     printf ("\n\n\n*********************************************************\n");
     printf ("*    Example 2:                                         *\n");
     printf ("*********************************************************");

      for (zn=0;zn<sds_dims[2];zn++)
       {
         for (reg=0;reg<sds_dims[3]; reg++)
          {
            for (hr=0; hr<sds_dims[1]; hr++)
             {
              printf ("\n\nHour box %i\n",hr+1);

              for (typ=0;typ<sds_dims[0];typ++)
                {
                indx2 = typ*sds_dims[1]*sds_dims[2]*sds_dims[3]+
                       hr*sds_dims[2]*sds_dims[3] +
                        zn * sds_dims[3] + reg ;
                   printf ("\nSurface type percent coverage(%i) = %f",
                   typ+1,surface_types[indx2]);
                }
             }
          }

       }


/*
 *******************************************************************************
 *  Example 3) Example of how to extract an SDS of rank 3 (3-dimensional) from
 *             the HDF file using pointers and print the data as ascii text to
 *             standard output.  Memory is allocated to hold the data being read
 *             and is released after the data is printed to the output file.
 *             The SDS, "Region Number",
 *             will be extracted and written to the output file.  To write the
 *             data to a different file, add in a call to "fopen", similar to
 *             the following, to open the output file:
 *                     outfile = fopen(filename, "w");
 *             Change "stdout" to "outfile" in the "fprintf" commands.
 *
 *  This example illustrates the use of functions 'get_sds_dims',
 *    'set_sds_data' and 'read_sds'.
 *
 * 'set_sds_data'
 *     Function - Returns a void pointer to the address to write the SDS data
 *                set.
 *    Input Variables:
 *       sds_rank - integer defining rank of the SDS data set.
 *       sds_type - integer defining type of SDS data set (see table above).
 *       sds_dims - integer array of rank 'sds_rank' containing dimensions
 *                  of the SDS data set.
 *
 * 'read_sds'     - See Example 1 for an explanation.
 * 'get_sds_dims' - See Example 1 for an explanation.
 *
 *  Note: This SDS data can also be accessed using SDS index (6) (See Example 5)
 *******************************************************************************
*/
     sds_name    = "Region Number";
     get_sds_dims(HDF_File,sds_name,&rank,sds_dims,&type);
     sds_rank = rank;
     sds_type = type;
     sds_edge[0] = sds_dims[0];
     sds_strt[0] = 0;
     sds_incr[0] = 1;
     sds_edge[1] = sds_dims[1];
     sds_strt[1] = 0;
     sds_incr[1] = 1;
     sds_edge[2] = sds_dims[2];
     sds_strt[2] = 0;
     sds_incr[2] = 1;

/*
 *******************************************************************************
 *  Returns a void pointer to the address to write the SDS data set.
 *******************************************************************************
*/
     sds_data = set_sds_data(sds_type, sds_rank, sds_dims);
    
/*
 *******************************************************************************
 * Read an SDS data set from an HDF file.
 *******************************************************************************
*/
     sds_status = read_sds(HDF_File, sds_name, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_data);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%s\" ", sds_name);
        exit(1);
     }
     fprintf (stdout,
       "\n\n\n*********************************************************\n");
     fprintf (stdout,
       "*    Example 3:                                         *\n");
     fprintf (stdout,
       "*********************************************************");
     for (zn=0;zn<sds_dims[1];zn++)
       {
        for (reg=0;reg<sds_dims[2]; reg++)
          {
           for (hr=0; hr<sds_dims[0]; hr++)
             {
              indx = hr*sds_dims[1]*sds_dims[2] + zn * sds_dims[2]+ reg;
                         
              printf ("\nHour Box %i\n",hr+1);
              printf ("Region Number  = %i\n",
                   (int) *((float32 *)sds_data+indx));
                   
             }
          }

       }

/*
 *******************************************************************************
 * Free the space allocated to the sds data array.
 *******************************************************************************
*/
     free(sds_data);

/*
 *******************************************************************************
 *  Example 4) Example of how to extract an SDS of rank 4 ( 4-dimensional )
 *             from the HDF file and store it in memory.  The SDS,
 *             "SW TOA Total-Sky", will be extracted and stored
 *             in the 4-D array, "total_sky".  The differences between
 *             Example 2 and Example 1 are:
 *                1.  The values sds_incr[1], sds_edge[1], and sds_strt[1]
 *                    must be set for this 4-D SDS.
 *                2.  The values for sds_name, sds_rank, and sds_type are now
 *                    set for the 4-D SDS, "SW TOA Total-Sky".
 *
 * This example uses function 'read_sds' and 'get_sds_dims'.
 *
 * See example 1 for information on these functions.
 *
 * Note: This SDS data can also be accessed using SDS index (42) (See Example 5)
 *
 *******************************************************************************
*/
     sds_name = "SW TOA Total-Sky";
     get_sds_dims(HDF_File,sds_name,&rank,sds_dims,&type);
     sds_rank = rank;
     sds_type = type;
     sds_edge[0] = sds_dims[0];
     sds_edge[1] = sds_dims[1];
     sds_edge[2] = sds_dims[2];
     sds_edge[3] = sds_dims[3];
     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_strt[2] = 0;
     sds_strt[3] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;
     sds_incr[2] = 1;
     sds_incr[3] = 1;

/*
 *****************************************************************************
 *  Read the "SW TOA Total-Sky" SDS into the 4-D 
 *  "total_sky" array. 
 *****************************************************************************
*/
     total_sky = (float32 *)malloc ( sds_dims[0] * sds_dims[1] * 
                  sds_dims[2] * sds_dims[3] * sizeof(float32));
     sds_status = read_sds(HDF_File, sds_name, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, total_sky);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%s\" ", sds_name);
        exit(1);
     }
/*
 *****************************************************************************
 *  Print the values in the "SW TOA Total-Sky" SDS, stored in
 *  the 4-D array, "total_sky" for all hours of each zonal region.
 *****************************************************************************
*/
     printf ("\n\n\n*********************************************************\n");
     printf ("*    Example 4:                                         *\n");
     printf ("*********************************************************");

     for (zn=0;zn<sds_dims[2];zn++) 
       {
         for (reg=0;reg<sds_dims[3]; reg++)
          {
            for (hr=0; hr<sds_dims[1]; hr++) 
             {
               
               printf ("\n\nHour box %i\n",hr+1);
               printf ("\nTOA SW TotalSky \n");
               
               for (stats=0;stats<sds_dims[0];stats++)
                 {

                  indx2 = stats*sds_dims[1]*sds_dims[2]*sds_dims[3]+
                       hr*sds_dims[2]*sds_dims[3] + 
                       zn * sds_dims[3] + reg;
 
                  if (stats == 0) printf ("\n Mean : %f",total_sky[indx2]);
                  if (stats == 1) printf ("\n Std  : %f",total_sky[indx2]); 
                  if (stats == 2) printf ("\n NumObs : %i",(int) total_sky[indx2]); 
                 
                 }
             }
          }

       }

/*
****************************************************************************
*     Example 5: The names of cloud layer properties are same on all 4 
*     cloud layers (High,UpperMid,LowerMid,Low). Using the SDS name to read data
*     will read only High cloud layer category SDSs. Therefore Example 5 
*     illustrates how to read Cloud Layer SDS (Effective Pressure ) from
*     Terra product using SDS index.
*   
****************************************************************************  
*/
     index = 218;
     data_name = calloc(MAX_CHAR_LENGTH,sizeof(char));
     get_sds_dims_by_index(HDF_File,index,data_name,&rank,sds_dims,&type);

     sds_rank = rank;
     sds_type = type;

     sds_edge[0] = sds_dims[0];
     sds_edge[1] = sds_dims[1];
     sds_edge[2] = sds_dims[2];
     sds_edge[3] = sds_dims[3];
     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_strt[2] = 0;
     sds_strt[3] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;
     sds_incr[2] = 1;
     sds_incr[3] = 1;
 
     eff_pres_L = (float32 *)malloc ( sds_dims[0] * sds_dims[1] * 
                  sds_dims[2] * sds_dims[3] * sizeof(float32));

     sds_status = read_sds_by_index(HDF_File, index,sds_name,sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, eff_pres_L);

     if ( sds_status != 0 )
        {
          fprintf (stderr, "\nError in reading SDS: \"%i\" ", index);
          exit(1);
        } 

/*
 *****************************************************************************
 *  Print the values in the "Effective Pressure" SDS of the Low Cloud Layer 
 *  category, stored in the 4-D array, "eff_pres_L" for all hours of 
 *  each zonal region.
 *****************************************************************************
*/
     printf ("\n\n\n*********************************************************\n");
     printf ("*    Example 5:                                         *\n");
     printf ("*********************************************************"); 

   for (zn=0;zn<sds_dims[2];zn++) 
      {
        for (reg=0;reg<sds_dims[3]; reg++)
          {
            for (hr=0; hr<sds_dims[1]; hr++) 
             {
              indx = hr*sds_dims[2]*sds_dims[3] + zn * sds_dims[3]+ reg;
               printf ("\n\nHour box %i\n",hr+1);
               printf ("\nLow Cloud Layer %s \n",data_name);
                 for (stats=0;stats<sds_dims[0];stats++)
                   {
                     indx2 = stats*sds_dims[1]*sds_dims[2]*sds_dims[3]+
                              hr*sds_dims[2]*sds_dims[3] + 
                                      zn * sds_dims[3]+ reg;

                     if (stats == 0) printf ("\n Mean : %f",eff_pres_L[indx2]);
                     if (stats == 1) printf ("\n Std  : %f",eff_pres_L[indx2]); 
                     if (stats == 2) printf ("\n NumObs  : %i",
                             (int) eff_pres_L[indx2]);  
                   }
              }
           }
 
      }

     free(eff_pres_L);
/*
 *******************************************************************************
 *  Example 6) This example extracts string data from a Vdata field in the
 *             "CERES_metadata" and prints it out in ascii text. 
 * Note: platform was declared an array above.
 *******************************************************************************
*/
     num_fields    = 14;
     vdata_name    = "CERES_metadata";
     vfield_name   = "ASSOCIATEDPLATFORMSHORTNAME";
/*
 *******************************************************************************
 * Gets arrays of types and orders of the fields within a Vdata.
 *******************************************************************************
*/
     vfield_status = get_vfield_types(HDF_File, vdata_name,
                     num_fields, vfield_types, vfield_order);
     if ( vfield_status != 0 )
     {
        fprintf (stdout,"\nError in getting Vdata field types or orders\n");
        exit(1);
     }

     vdata_data[0]=platform;
     rd_num_fields = 1;
     rd_num_recs   = 1;
/*
 *******************************************************************************
/*
 *******************************************************************************
 * Read a Vdata data set from a HDF file.
 *******************************************************************************
*/
     vdata_status = read_vdata(HDF_File, vdata_name, rd_num_fields,
                    rd_num_recs, vfield_name, vfield_types, vfield_order,
                    vdata_data);

     if ( vdata_status != 0 )
     {
        fprintf (stderr,"\nError in reading Vdata: \"%s\" ", vdata_name);
        exit(1);
     }
/*
 *******************************************************************************
 *  Print the value of the "ASSOCIATEDPLATFORMSHORTNAME" vfield, stored in the
 *  "platform" array.
 *******************************************************************************
*/
/*
    Print each character ( ie,order ) in the vfield separately.  The order is
    also used ( 32, in this example ) to dimension the size of the array at the
    beginning of the code.
*/
     printf ("\n\n\n*********************************************************\n");
     printf ("*    Example 6:                                         *\n");
     printf ("*********************************************************\n");
     printf ("\nASSOCIATEDPLATFORMSHORTNAME = ");
     for(i=0; i<32; i++)
     {
       if(platform[i] == NULL)break;
         printf("%c",platform[i]);
     }
     printf ("\n");
/*
 *******************************************************************************
 *  Example 7) This example extracts integer data from the Vdata field,
 *             "NUMBEROFRECORDS" using pointers and prints it out as ascii text
 *             to the standard output file, stdout.  Memory is allocated to hold
 *             the data being read and is released after the data is printed to
 *             the output file.  To read in a different vfield, change the
 *             vfield_name (and the vdata_name and num_fields, if the vfield is
 *             in a different vdata).
 *******************************************************************************
 * This example illustrates the use of functions 'get_vfield_types',
 * 'get_vdata_n_rec', 'set_vdata_data', and 'read_vdata'.
 * 
 * 'get_vfield_types' - see Example 4 for description of this function
 *
 * 'get_vdata_n_rec'
 *    Function - Returns the number of records in the Vdata.
 *    Input Variables:
 *       HDF_File      - character pointer to the name of the HDF file.
 *       vdata_name    - character pointer to the name of the Vdata data set.
 *
 * 'set_vdata_data'
 *    Function - Returns a void pointer to the address to write the Vdata
 *               data set.
 *    Input Variables:
 *       num_fields    - integer defining number of fields in the Vdata.
 *       vfield_types  - integer array containing the types of the fields within
 *                       the Vdata data set (see table above).
 *       vfield_order  - integer array containing the order of the fields within
 *                       the Vdata data set.
 *       vdata_n_rec   - integer number of records in the Vdata.
 *    Output Variable:
 *       vdata_data    - void pointer
 *
 * 'read_vdata' - see Example 4 for description of this function
 *******************************************************************************
*/

     num_fields    = 14;
     vdata_name    = "CERES_metadata";
     vfield_name   = "NUMBEROFRECORDS";
/*
 *******************************************************************************
 * Gets arrays of types and orders of the fields within a Vdata.
 *******************************************************************************
*/
     vfield_status = get_vfield_types(HDF_File, vdata_name,
                     num_fields, vfield_types, vfield_order);
     if ( vfield_status != 0 )
     {
        fprintf (stderr,"\nError in getting Vdata field types or orders");
        exit(1);
     }
/*
 *******************************************************************************
 * Get the number of records in the Vdata, vdata_n_rec
 *******************************************************************************
*/
     vdata_n_rec = get_vdata_n_rec(HDF_File, vdata_name);
/*
 *******************************************************************************
 * Allocate memory and set the pointer, vdata_data, to point to the address
 * to write the Vdata data set.
 *******************************************************************************
*/
     set_vdata_data(num_fields, vfield_types, vfield_order, (int32)vdata_n_rec,
                    vdata_data);

     rd_num_fields = 1;
     rd_num_recs = 1;
/*
 *******************************************************************************
 * Read a Vdata data set from a HDF file.
 *******************************************************************************
*/
     vdata_status = read_vdata(HDF_File, vdata_name, rd_num_fields,
                    rd_num_recs, vfield_name, vfield_types, vfield_order,
                    vdata_data);
     if ( vdata_status != 0 )
     {
        fprintf (stderr,"\nError in reading Vdata: \"%s\" ", vdata_name);
        free_ptr_array(num_fields, vdata_data);
        exit(1);
     }

free_ptr_array(num_fields, vdata_data);



}
