/*
 *****************************************************************************
 *  Program: ES9_readhdf.c
 *  
 *  Purpose: To provide examples of how to read data from the ES9 HDF file.
 *  
 *  There are 3 examples of how to read data from the SDSs and on the
 *  ES9 HDF file.
 *
 *  Example 1) Illustrates reading real data from an SDS of rank 1
 *  Example 2) Illustrates reading real data from an SDS of rank 2
 *  Example 3) Illustrates reading string data from a Vdata field and 
 *             storing the data in an array. 
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
 *  value in an HDF include file.  Function sds_get_dims_by_index is used to
 *  return the value of the 'type', sds_type, which is used in the calls to 
 *  read the SDS. 
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
 *   HDF_File - ES9 HDF file name - modify this if a different ES9 HDF file is
 *              being read
 *   sds_index - SDS index - see the Data Products Catalog ("DPC_ES9_R4V1.pdf") or 
 *              the provided dump of the HDF file 
 *              ("CER_ES9_Aqua-FM3_Sample_024024.200306.dump") 
 *              for valid SDS indices in the ES9 product
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
 *   Example 1.
*/

   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <math.h>
   #include "ES9_HDFread.h"

/*
 *******************************************************************************
 *  MAIN PROGRAM
 *******************************************************************************
*/

   void main( int argc, char *argv[])
   {

     char *HDF_File;
     char sds_name[MAX_CHAR_LENGTH];
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
     int32 i,j,k;
     int32 sds_index;
     int stor_size, index_max;
     void *vdata_data[MAX_VDATA_FIELDS];

/*
 *******************************************************************************
 *  Arrays assigned for Examples 1, 2, and 3.
 *  In order to read hourly data, the array sizes of 
 *  sds_2d_data_array will change dynamically.
 *******************************************************************************
*/

     float32 *sds_1d_data_array;
     float32 *sds_2d_data_array;

     char    string_data[32];

/*
 *******************************************************************************
 *  Assign the HDF file to be read.
 *******************************************************************************
*/

     if(argc != 2)
  {
     fprintf ( stderr, "Usage: rd HDFfilename\n");
     exit(1);
  }

     HDF_File = argv[1];

/*
 *******************************************************************************
 *  Print the name of the file being read and check the validity of the file. 
 *******************************************************************************
*/
     printf("\nName of ES9 HDF file to be read:\n< %s\n",HDF_File);

     if (!Hishdf(HDF_File))
     {
       fprintf(stderr, "ERROR - specified file is not an HDF file.\n");
       exit (1);
     } 

/*
 *******************************************************************************
 *  Example 1) Example of how to extract an SDS of rank 1 ( 1-dimensional ) 
 *             from the HDF file and store it in memory.  The SDS, 
 *             "Longwave flux", will be extracted and stored 
 *             in the 1-D array, "sds_1d_data_array".
 *******************************************************************************
 * This example uses function 'read_sds_by_index' and 'get_sds_dims_by_index'.
 * 
 *******************************************************************************
*/

     sds_index = 132;
     get_sds_dims_by_index(HDF_File,sds_index,sds_name,&rank,sds_dims,&type);
     sds_rank = rank;
     sds_type = type;
     sds_edge[0] = sds_dims[0]; 
     sds_strt[0] = 0;
     sds_incr[0] = 1;

     stor_size = sds_dims[0] * sizeof(float32);
     sds_1d_data_array = (float32 *) malloc(stor_size);

/*
 *******************************************************************************
 *  Read the "Longwave flux" SDS into the 1-D 
 *     "sds_1d_data_array" array. 
 *******************************************************************************
*/
     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_1d_data_array);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%i\" ", sds_index);
        fprintf (stderr,"\nsds_dims[0] = %i.  Make sure sds_1d_data_array size reflects this.\n", sds_dims[0]);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "Longwave flux" SDS, stored in 
 *  the 1-D array, "sds_1d_data_array".
 *******************************************************************************
*/
     index_max = 5;
     if ( sds_edge[0] < 5)
     {
        index_max = sds_edge[0];
     }

     printf ("\n\n\n************************************************************\n");
     printf ("*    Example 1:  ( ONLY PRINTING MAX OF 5 HOUR BOX DATAS)   *\n");
     printf ("*************************************************************");
     for (i=0; i<index_max; i++)
       {
	 printf ("\nHourbox (%i) for %s = %f",
		 i+1,sds_name,sds_1d_data_array[i]);
       }
     printf ("\n\n");

     free(sds_1d_data_array);

/*
 *******************************************************************************
 *  Example 2) Example of how to extract an SDS of rank 2 ( 2-dimensional ) 
 *             from the HDF file and store it in memory.  The SDS, 
 *             "Longwave flux standard deviation", will be extracted and stored 
 *             in the 2-D array, "sds_2d_data_array".
 *******************************************************************************
 * This example uses function 'read_sds_by_index' and 'get_sds_dims_by_index'.
 * 
 *******************************************************************************
*/

     sds_index = 65;
     get_sds_dims_by_index(HDF_File,sds_index,sds_name,&rank,sds_dims,&type);
     sds_rank = rank;
     sds_type = type;
     sds_edge[0] = sds_dims[0]; 
     sds_edge[1] = sds_dims[1];
     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;

     sds_2d_data_array = calloc(sds_dims[0], sds_dims[1]*sizeof(float32));


/*
 *******************************************************************************
 *  Read the "Longwave flux standard deviation" SDS into the 2-D 
 *     "sds_2d_data_array" array. 
 *******************************************************************************
*/
     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_2d_data_array);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%i\" ", sds_index);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "Longwave flux standard deviation" SDS, stored in 
 *  the 2-D array, "sds_2d_data_array".
 *******************************************************************************
*/

     index_max = 5;
     if ( sds_edge[1] < 5)
     {
        index_max = sds_edge[1];
     }

     printf ("\n\n\n*********************************************************\n");
     printf ("*    Example 2:   ( ONLY PRINTING MAX OF 5 PER DAY)     *\n");
     printf ("*********************************************************");

     for (i=0; i<sds_edge[0]; i++)
     {
        printf ("\n\n%s for day %i\n",sds_name,i+1);
        for (j=0; j<index_max; j++)
        {
           k = i * sds_edge[1] + j;
           printf ("\nRegion (%i) = %f",
                      j+1,sds_2d_data_array[k]);

        }
     }
     printf ("\n\n");

     free(sds_2d_data_array);

/*
 *******************************************************************************
 *  Example 3) This example extracts string data from a Vdata field in the
 *             "CERES_metadata" and prints it out in ascii text.
 * Note: string_data was declared an array above.
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

     vdata_data[0]=string_data;
     rd_num_fields = 1;
     rd_num_recs   = 1;
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
 *  "string_data" array.
 *******************************************************************************
*/
/*
    Print each character ( ie,order ) in the vfield separately.  The order is
    also used ( 32, in this example ) to dimension the size of the array at the
    beginning of the code.
*/
     printf ("\n\n\n*********************************************************\n");
     printf ("*    Example 3:                                         *\n");
     printf ("*********************************************************\n");
     printf ("\nASSOCIATEDPLATFORMSHORTNAME = ");
     for(i=0; i<32; i++)
     {
       if(string_data[i] == NULL)break;
         printf("%c",string_data[i]);
     }
     printf("\n\n");

}
