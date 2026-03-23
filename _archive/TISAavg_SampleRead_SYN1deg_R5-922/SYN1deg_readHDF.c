/*
 *****************************************************************************
 *  Program: SYN1deg_readHDF.c
 *
 *  Purpose: To provide examples of how to read data from the SYN1deg, synoptic
 *           3Hour, Month, and M3Hour files.
 *
 *  SYN1deg-3Hour HDF file:
 *  There are 3 examples of how to read data from the SDSs on the SYN 1deg 3 Hour HDF
 *  file.  The purpose of the examples is to show how to get the exact desired SDS.
 *
 *  Example 1) Illustrates reading real data from an SDS of rank 2
 *             (2-dimensional) and storing the data in an array 
 *             and writing the data to the standard output file.
 *  Example 2) Illustrates reading real data from an SDS of rank 3 
 *             (3-dimensional) and storing the data in an array
 *             and writing the data to the standard output file.
 *  Example 3) Illustrates reading real data from an SDS of rank 4
 *            (4-dimensinoal) and storing the data in an array
 *             and writing the data to the standard output file. 
 *             This example is the 3-hourly synoptic data of the
 *             regional data with the 5 profile pressure levels read.
 *
 *  SYN1deg-Month HDF file
 *  There are 4 examples of how to read data from the SDSs and Vdatas on the
 *  SYN 1deg Month HDF file.
 *
 *  Example 1) Illustrates reading real data from an SDS of rank 2
 *             (2-dimensional) and storing the data in an array
 *             and writing the data to the standard output file.
 *  Example 2) Illustrates reading real data from an SDS of rank 3
 *             (3-dimensional) and storing the data in an array
 *             and writing the data to the standard output file.
 *  Example 3) Illustrates reading real data from an SDS of rank 4
 *             (4-dimensional) and storing the data in an array.
 *             and writing the data to the standard output file.
 *  Example 4) Illustrates reading string data from a Vdata field and
 *             storing the data in an array.
 *             and writing the data to the standard output file.
 *
 *  SYN1deg-M3Hour HDF file
 *  There are 5 examples of how to read data from the SDSs and Vdatas on the
 *  M3Hour HDF file.
 *
 *  Example 1) Illustrates reading real data from an SDS of rank 2
 *             (2-dimensional) and storing the data in an array
 *             and writing the data to the standard output file.
 *  Example 2) Illustrates reading real data from an SDS of rank 3
 *             (3-dimensional) and storing the data in an array
 *             and writing the data to the standard output file.
 *             It also provides an example of reading only a portion
 *             of the data.
 *  Example 3) Illustrates reading real data from an SDS of rank 4
 *             (4-dimensional) and storing the data in an array.
 *             and writing the data to the standard output file.
 *  Example 4) Illustrates reading real data from an SDS of rank 5
 *             (5-dimensional) and storing the data in an array.
 *             and writing the data to the standard output file.
 *  Example 5) Illustrates reading string data from a Vdata field and
 *             storing the data in an array.
 *             and writing the data to the standard output file.
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
 *   HDF_File  - SYN1deg-3Hour, SYN1deg-Month, or SYN1deg-M3Hour HDF file name - modify this if 
 *               a different HDF file is being read.
 *   sds_index - SDS index - see the Data Products Catalog for SYN1deg-3Hour (SYN1deg-3Hour_dpc_r5.ps"),
 *               see the Data Products Catalog for AVG ("SYN1deg-Month_dpc_r5.ps") or
 *               see the Data Products Catalog for ZAVG ("SYN1deg-M3Hour_dpc_r5.ps").
 *   sds_strt  - array containing the starting location in each dimension
 *               at which the data read begins.  It is zero based.  In all
 *               examples, sds_strt for each dimension is set to zero so
 *               reads start at the beginning element of each dimension.
 *               To read in a partial number of regions, change sds_strt[0]
 *               to the number of the first region to be read.
 *   sds_edge  - sds_edge is set to the number of values to be read in each
 *               dimension.  This sample program sets them to the total
 *               number of elements in each dimension, so all data are read.
 *               To read in a partial number of regions, set sds_edge[0]
 *               to the number of the regions to be read.
 *   sds_incr  - array containing the increments along the dimensions for which
 *               data is to be read.  For these examples, the values for the
 *               sds_incr array are set to one, so no data are skipped.  To
 *               read every third value along a dimension, skipping the two
 *               values in between, set sds_incr for that dimension to 3.
 *   The array to hold the values read from an SDS must be defined with the
 *   correct dimension and type and this array must be used in place of the
 *   one used in Example 1.
*/

   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <math.h>
   #include "SYN1deg_HDFread.h"

/*
 *******************************************************************************
 *  MAIN PROGRAM
 *******************************************************************************
*/

int main( int argc, char *argv[])
{

     char *HDF_File;
     char *AVG_f1   = "CER_SYN1deg-3Hour_";  // 18
     char *AVG_f2   = "CER_SYN1deg-Month_";  // 18
     char *AVG_f3   = "CER_SYN1deg-M3Hour_"; //19
     char part_hdf[19];
     char sds_name[MAX_CHAR_LENGTH];
     char *vdata_name;
     char *vfield_name;
     int32 rank;
     int32 type;
     int32 sds_index;
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
     int32 i,j,k,m,n,p,q;
     int index_max, index_max0, index_max1, index_max2, index_max3, index_max4;
     void *vdata_data[MAX_VDATA_FIELDS];

     /* HDF error Handling */
     int32 e, sLen;
     const char *str;
     
/*
 *******************************************************************************
 *  Arrays assigned for Examples 1, 2, 3 and 4.
 *******************************************************************************
*/
     float32   *sds_2d_data_array;
     float32   *sds_3d_data_array;
     float32   *sds_4d_data_array;
     float32   *sds_5d_data_array;
     char      string_data[32];

// variable to control the number of output for each dimension
     int32     maxOut;
     int32     maxOutReg;

    maxOut = 2;
    maxOutReg = 5;

/*
 *******************************************************************************
 *  Assign the 5-region test HDF file to be read.  If another HDF file is to
 *  be read, enter its name here.
 *******************************************************************************
*/
    
    if(argc != 2)
    {
     fprintf ( stderr, "Usage: SYN1deg_rd HDFfilename\n");
     exit(1);
    }

     HDF_File = argv[1];

     printf("\nName of HDF file to be read:\n< %s\n",HDF_File);

     if (!Hishdf(HDF_File))
     {
       fprintf(stderr, "ERROR - specified file is not an HDF file.\n");
       exit (1);
     }

     strncpy(part_hdf,HDF_File,18);
     part_hdf[19] = 0;

     /* ==SYN1deg-3Hour== */
/* ********************************************************************************************************* */
/* ******************************* SYN1deg-3Hour *********************************************************** */ 
   if (strncmp(AVG_f1,part_hdf,18) == 0 )
   {
    /*
     *******************************************************************************
     *  Print the name of the file being read and check the validity of the file. 
     ****************************************************************************** 
     */
 
     printf("\nName of 5-zone 5-region SYN1deg-3Hour HDF file:\n< %s\n",HDF_File);

     if (!Hishdf(HDF_File))
     {
       fprintf(stderr, "ERROR - specified file is not an HDF4 file.\n");
       exit (1);
     } 

/*
 *******************************************************************************
 *  Example 1) Example of how to extract an SDS of rank 2 ( 2-dimensional ) 
 *             from the HDF file and store it in memory.  The SDS, 
 *             "Region Number"   , with index number 0 will be read and stored 
 *             in the 2-D array, "sds_2d_data_array".  The values for sds_name, 
 *             sds_rank, and sds_type are now determined by the SDS index, 
 *             set for the 2-D SDS, "Region Number"   .
 *******************************************************************************
 * This example uses function 'get_sds_dims_by_index' and 'read_sds_by_index'
 * 
 *******************************************************************************
 *
 *******************************************************************************
 *  To retrieve another SDS change sds_index to the index of the SDS you want
 *     to retrieve
 *******************************************************************************
*/
     printf ("\n\n\n*********************************************************\n");
     printf ("*        Start Example 1   (2 dimensional data)         *\n");
     printf ("*********************************************************");


     sds_index = 0;
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
 *  Read the "Region Number"   SDS into the 2-D "sds_2d_data_array" array. 
 *******************************************************************************
*/
     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_2d_data_array);

        fprintf (stdout,"\n SDS name: \"%s\" \n", sds_name);
        fprintf (stdout,"Rank: \"%i\" \n", rank);
        fprintf (stdout,"type: \"%i\" \n", type);
        fprintf (stdout,"sds_dims[0]: \"%i\" \n", sds_dims[0]);
        fprintf (stdout,"sds_dims[1]: \"%i\" \n", sds_dims[1]);

        fprintf (stdout,"sds_strt[0]: \"%i\" \n", sds_strt[0]);
        fprintf (stdout,"sds_strt[1]: \"%i\" \n", sds_strt[1]);
        fprintf (stdout,"sds_incr[0]: \"%i\" \n", sds_incr[0]);
        fprintf (stdout,"sds_incr[1]: \"%i\" \n", sds_incr[1]);
        fprintf (stdout,"sds_edge[0]: \"%i\" \n", sds_edge[0]);
        fprintf (stdout,"sds_edge[1]: \"%i\" \n", sds_edge[1]);

     if ( sds_status != 0 )
     {
        i = 0;
        while((e=HEvalue(i)) !=DFE_NONE)
        {
          str=HEstring(e);
          sLen = strlen(str);
          fprintf(stderr,"strLen = %i \n", sLen);
          fprintf(stderr,"%s",*str);
          fprintf(stderr,"%s",str);
          i++;
        } 
        fprintf (stderr,"\nError %i in reading SDS: \"%i\" \n", sds_status,sds_index);
       
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "Region Number"   SDS, stored in 
 *  the 2-D array, "sds_2d_data_array".
 *******************************************************************************
*/

     index_max = maxOutReg;
     if ( sds_edge[1] < maxOutReg)
     {
        index_max = sds_edge[1];
     }

     printf ("\n\n*********************************************************\n");
     printf ("*  Example 1:   ( ONLY PRINTING MAX OF 5 PER ZONE)      *\n");
     printf ("*********************************************************");

     for (i=0; i<sds_edge[0]; i++)
     {
        printf ("\n\n%s for Zone %i\n",sds_name,i+1);
        for (j=0; j<index_max; j++)
        {
           k = i * sds_edge[1] + j;
           printf ("\n   Longitude Region (%i)[%i] = %f",
                      j+1,k+1,sds_2d_data_array[k]);
        }
     }
     printf ("\n\n");

     free(sds_2d_data_array);

     fprintf (stderr,"\n At end of Example 1 with SDS  \"%i\" \n", sds_index);

 /*
 *******************************************************************************
 *  Example 2) Example of how to extract an SDS of rank 3 ( 3-dimensional ) 
 *             from the HDF file and store it in memory.  The SDS, 
 *             "LW TOA Total-Sky" with index number 6 will be extracted and stored 
 *             in the 3-D array, "sds_3d_data_array".  The values for sds_name, 
 *             sds_rank, and sds_type are now determined by the SDS index, 
 *             set for the 3-D SDS, "LW TOA Total-Sky"
 *******************************************************************************
 * This example uses function 'get_sds_dims_by_index' and 'read_sds_by_index'.
 * 
 *******************************************************************************
 *  To retrieve another SDS change sds_index to the index of the SDS you want
 *     to retrieve
 *******************************************************************************
*/

     printf ("\n\n\n*********************************************************\n");
     printf ("*              Start Example 2  (3 Dimensional data)    *\n");
     printf ("*********************************************************");

     sds_index = 6;
     get_sds_dims_by_index(HDF_File,sds_index,sds_name,&rank,sds_dims,&type);

        fprintf (stdout,"\n SDS: \"%s\" \n", sds_name);
        fprintf (stdout,"Rank: \"%i\" \n", rank);
        fprintf (stdout,"type: \"%i\" \n", type);
        fprintf (stdout,"sds_dims[0]: \"%i\" \n", sds_dims[0]);
        fprintf (stdout,"sds_dims[1]: \"%i\" \n", sds_dims[1]);
        fprintf (stdout,"sds_dims[2]: \"%i\" \n", sds_dims[2]);

     sds_rank = rank;
     sds_type = type;
     sds_edge[0] = sds_dims[0]; 
     sds_edge[1] = sds_dims[1];
     sds_edge[2] = sds_dims[2];
     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_strt[2] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;
     sds_incr[2] = 1;

     sds_3d_data_array = calloc(sds_dims[0]*sds_dims[1], sds_dims[2]*sizeof(float32));

/*
 *******************************************************************************
 *  Read the "LW TOA Total-Sky" SDS into the 3-D "sds_3d_data_array" array. 
 *******************************************************************************
*/
     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_3d_data_array);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%i\" ", sds_index);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "LW TOA Total-Sky" SDS, stored in 
 *  the 3-D array, "sds_3d_data_array".
 *******************************************************************************
*/
     index_max0 = maxOut;
     index_max1 = maxOut;
     index_max2 = maxOut;
     if ( sds_edge[2] < maxOut)
     {
        index_max2 = sds_edge[2];
     }
     if ( sds_edge[1] < maxOut)
     {
        index_max1 = sds_edge[1];
     }
     if ( sds_edge[0] < maxOut)
     {
        index_max0 = sds_edge[0];
     }


     printf ("\n\n************************************************************\n");
     printf ("*  Example 2:   ( ONLY PRINTING MAX OF %i PER DIMENSION)    *\n",maxOut);
     printf ("************************************************************");

     for (i=0; i<index_max0; i++)
     {
        printf ("\n\n%s for Synoptic Hour =  %i\n",sds_name,i+1);
        for (k=0; k<index_max1; k++)
        {
            printf ("\n\n%s for Zone %i\n",sds_name,k+1);
            for (m=0; m<index_max2; m++)
            {
                n = i * sds_edge[1] * sds_edge[2] + k * sds_edge[2] + m ;
                printf ("      Longitude Region (%i)[%i] = %f\n",
                              m+1,n+1,sds_3d_data_array[n]);
            }
        }
     }
     printf ("\n\n");

     free(sds_3d_data_array);

     fprintf (stderr,"\n At end of Example 2 with SDS  \"%i\" \n", sds_index);

/*
 *******************************************************************************
 *  Example 3) Example of how to extract an SDS of rank 4 ( 4-dimensional )
 *             from the HDF file and store it in memory.  The SDS,
 *             "Tuned Total-Sky LW Up",         
 *             with index number 110 will be extracted and stored
 *             in the 4-D array, "sds_4d_data_array".  The values for sds_name,
 *             sds_rank, and sds_type are now determined by the SDS index,
 *             set for the 4-D SDS, "Tuned total-Sky LW Up" 
 *******************************************************************************
 * This example uses function 'get_sds_dims_by_index' and 'read_sds_by_index'.
 *
 *******************************************************************************
 *******************************************************************************
 *  To retrieve another SDS change sds_index to the index of the SDS you want
 *     to retrieve
 *******************************************************************************
*/

     printf ("\n\n\n*********************************************************\n");
     printf ("*            Start Example 3  (4 dimensional data)      *\n");
     printf ("*********************************************************");

     sds_index = 110; 
     get_sds_dims_by_index(HDF_File,sds_index,sds_name,&rank,sds_dims,&type);

        fprintf (stdout,"\n SDS: \"%s\" \n", sds_name);
        fprintf (stdout,"Rank: \"%i\" \n", rank);
        fprintf (stdout,"type: \"%i\" \n", type);
        fprintf (stdout,"sds_dims[0]: \"%i\" \n", sds_dims[0]);
        fprintf (stdout,"sds_dims[1]: \"%i\" \n", sds_dims[1]);
        fprintf (stdout,"sds_dims[2]: \"%i\" \n", sds_dims[2]);
        fprintf (stdout,"sds_dims[3]: \"%i\" \n", sds_dims[3]);

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

     sds_4d_data_array = calloc(sds_dims[0]*sds_dims[1]*sds_dims[2], sds_dims[3]*sizeof(float32));

/*
 *******************************************************************************
 *  Read the "Tuned Total-Sky LW Up" SDS into the 4-D "sds_4d_data_array" array.
 *******************************************************************************
*/
     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_4d_data_array);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%i\" ", sds_index);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "Tuned Clear-Sky SW Up" SDS, stored in
 *  the 4-D array, "sds_4d_data_array".
 *******************************************************************************
*/
     index_max0 = maxOut;
     index_max1 = maxOut;
     index_max2 = maxOut;
     index_max3 = maxOut;
     if ( sds_edge[3] < maxOut)
     {
        index_max3 = sds_edge[3];
     }
     if ( sds_edge[2] < maxOut)
     {
        index_max2 = sds_edge[2];
     }
     if ( sds_edge[1] < maxOut)
     {
        index_max1 = sds_edge[1];
     }
     if ( sds_edge[0] < maxOut)
     {
        index_max0 = sds_edge[0];
     }

     printf ("\n\n\n************************************************************\n");
     printf ("*  Example 3:   ( ONLY PRINTING MAX OF %i PER DIMENSION)    *\n",maxOut);
     printf ("************************************************************");

     for (i=0; i<index_max0; i++)
     {
        printf ("\n\n%s Level     =  %i  \n",sds_name,i+1);

        for (k=0; k<index_max1; k++)
        {
            printf ("\n\n%s for Synoptic hour %i\n",sds_name,k+1);
            for (m=0; m<index_max2; m++)
            {
                printf ("\n\n%s for Zone %i\n",sds_name,m+1);
                for (p=0; p<index_max3; p++)
                {
                    n = i * sds_edge[1] * sds_edge[2] * sds_edge[3]
                      + k * sds_edge[2] * sds_edge[3]
                      + m * sds_edge[3] + p ;
                    printf ("      Longitude Region (%i)[%i] = %f\n",
                                  p+1,n+1,sds_4d_data_array[n]);
                }
            }
        }
     }
     free(sds_4d_data_array);
     fprintf (stderr,"\n At end of Example 3 with SDS  \"%i\" \n", sds_index);

   } // end if 3Hour


/* ********************************************************************************************************* */
/* ******************************* SYN1deg-Month *********************************************************** */ 

/*   ==Month==Month==Month==Month==Month==Month==Month==Month== !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*   AVG_f2 = Month    */

   if (strncmp(AVG_f2  ,part_hdf,18) == 0 )
   {
       printf("\nName of 5-zone 5-region SYN1deg-Month HDF file:\n< %s\n",HDF_File);

/*
 *******************************************************************************
 *  Example for SYN1deg-Month
 *******************************************************************************
*/

/*
 *******************************************************************************
 *  Example 1) Example of how to extract an SDS of rank 2 ( 2-dimensional )
 *             from the HDF file and store it in memory.  The SDS,
 *             "Region Number"   , with index number 0 will be read and stored
 *             in the 2-D array, "sds_2d_data_array".  The values for sds_name,
 *             sds_rank, and sds_type are now determined by the SDS index,
 *             set for the 2-D SDS, "Region Number"   .
 *******************************************************************************
 * This example uses function 'get_sds_dims_by_index' amd 'read_sds_by_index''.
 *
 *******************************************************************************
 *  To retrieve another SDS change sds_index to the index of the SDS you want
 *     to retrieve
 *******************************************************************************
*/
     printf ("\n\n\n*********************************************************\n");
     printf ("*      Start Example 1  (2 Dimensional data)            *\n");
     printf ("*********************************************************");

     sds_index = 0;
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
 *  Read the "Region Number"   SDS into the 2-D "sds_2d_data_array" array.
 *******************************************************************************
*/
     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_2d_data_array);

        fprintf (stdout,"\n SDS: \"%s\" \n", sds_name);
        fprintf (stdout,"Rank: \"%i\" \n", rank);
        fprintf (stdout,"type: \"%i\" \n", type);
        fprintf (stdout,"sds_dims[0]: \"%i\" \n", sds_dims[0]);
        fprintf (stdout,"sds_dims[1]: \"%i\" \n", sds_dims[1]);

     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%i\" ", sds_index);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "Region Number"   SDS, stored in
 *  the 2-D array, "sds_2d_data_array".
 *******************************************************************************
*/

     index_max = maxOutReg;
     if ( sds_edge[1] < maxOutReg)
     {
        index_max = sds_edge[1];
     }

     printf ("\n\n\n*********************************************************\n");
     printf ("*  Example 1:   ( ONLY PRINTING MAX OF %i PER ZONE)      *\n",maxOutReg);
     printf ("*********************************************************");

     for (i=0; i<sds_edge[0]; i++)
     {
        printf ("\n\n%s for Zone %i\n",sds_name,i+1);
        for (j=0; j<index_max; j++)
        {
           k = i * sds_edge[1] + j;
           printf ("\n   Longitude Region (%i)[%i] = %f",
                      j+1,k+1,sds_2d_data_array[k]);

        }
     }
     printf ("\n");

     free(sds_2d_data_array);

     fprintf (stderr,"\n At end of Example 1 with SDS  \"%i\" \n", sds_index);

 /*
 *******************************************************************************
 *  Example for SYN1deg-Month
 *******************************************************************************
 *******************************************************************************
 *  Example 2) Example of how to extract an SDS of rank 3 ( 3-dimensional )
 *             from the HDF file and store it in memory.  The SDS,
 *             "LW TOA Total-Sky" with index number 6 will be extracted and stored
 *             in the 3-D array, "sds_3d_data_array".  The values for sds_name,
 *             sds_rank, and sds_type are now determined by the SDS index,
 *             set for the 3-D SDS, "LW TOA Total-Sky"
 *******************************************************************************
 * This example uses function 'get_sds_dims_by_index' and 'read_sds_by_index'.
 *
 *******************************************************************************
 *  To retrieve another SDS change sds_index to the index of the SDS you want
 *     to retrieve
 *******************************************************************************
*/

     printf ("\n\n*********************************************************\n");
     printf ("*             Start Example 2   (3 Dimensional data)    *\n");
     printf ("*********************************************************");

     sds_index = 6;
     get_sds_dims_by_index(HDF_File,sds_index,sds_name,&rank,sds_dims,&type);

        fprintf (stdout,"\n SDS: \"%s\" \n", sds_name);
        fprintf (stdout,"Rank: \"%i\" \n", rank);
        fprintf (stdout,"type: \"%i\" \n", type);
        fprintf (stdout,"sds_dims[0]: \"%i\" \n", sds_dims[0]);
        fprintf (stdout,"sds_dims[1]: \"%i\" \n", sds_dims[1]);
        fprintf (stdout,"sds_dims[2]: \"%i\" \n", sds_dims[2]);

     sds_rank = rank;
     sds_type = type;
     sds_edge[0] = sds_dims[0];
     sds_edge[1] = sds_dims[1];
     sds_edge[2] = sds_dims[2];
     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_strt[2] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;
     sds_incr[2] = 1;

     sds_3d_data_array = calloc(sds_dims[0]*sds_dims[1], sds_dims[2]*sizeof(float32));


/*
 *******************************************************************************
 *  Read the "LW TOA TOTAL-Sky" SDS into the 3-D "sds_3d_data_array" array.
 *******************************************************************************
*/
     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_3d_data_array);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%i\" ", sds_index);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "LW TOA TOTAL-Sky" SDS, stored in
 *  the 3-D array, "sds_3d_data_array".
 *******************************************************************************
*/
     index_max1 = maxOut;
     index_max2 = maxOut;
     if ( sds_edge[2] < maxOut)
     {
        index_max2 = sds_edge[2];
     }
     if ( sds_edge[1] < maxOut)
     {
        index_max1 = sds_edge[1];
     }


     printf ("\n\n\n************************************************************\n");
     printf ("*  Example 2:   ( ONLY PRINTING MAX OF %i PER DIMENSION)    *\n",maxOut);
     printf ("************************************************************");

     for (i=0; i<sds_edge[0]; i++)
     {
        if (i==0)
          printf ("\n\n%s Stat type =  %i  MEAN \n",sds_name,i+1);
        if (i==1)
          printf ("\n\n%s Stat type =  %i  STANDARD DEVIATION \n",sds_name,i+1);
 
        for (k=0; k<index_max1; k++)
        {
            printf ("\n\n%s for Zone %i\n",sds_name,k+1);
            for (m=0; m<index_max2; m++)
            {
                n = i * sds_edge[1] * sds_edge[2] + k * sds_edge[2] + m ;
                printf ("      Longitude Region (%i)[%i] = %f\n",
                              m+1,n+1,sds_3d_data_array[n]);
            }
        }
     }
     printf ("\n");

     free(sds_3d_data_array);

     fprintf (stderr,"\n At end of Example 2 with SDS  \"%i\" \n", sds_index);

/*
 *******************************************************************************
 *  Example for SYN1deg-Month
 *******************************************************************************
 *******************************************************************************
 *  Example 3) Example of how to extract an SDS of rank 4 ( 4-dimensional )
 *             from the HDF file and store it in memory.  The SDS,
 *             "Tuned Total-Sky LW Up",
 *             with index number 110 will be extracted and stored
 *             in the 4-D array, "sds_4d_data_array".  The values for sds_name,
 *             sds_rank, and sds_type are now determined by the SDS index,
 *             set for the 4-D SDS, "Tuned Total-Sky LW up"
 *******************************************************************************
 * This example uses function 'get_sds_dims_by_index' and 'read_sds_by_index'.
 *
 *******************************************************************************
 *  To retrieve another SDS change sds_index to the index of the SDS you want
 *     to retrieve
 *******************************************************************************
*/

     printf ("\n\n*********************************************************\n");
     printf ("*               Start Example 3   (4 Dimensional data)  *\n");
     printf ("*********************************************************");

     sds_index = 110;

     get_sds_dims_by_index(HDF_File,sds_index,sds_name,&rank,sds_dims,&type);

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

        fprintf (stdout,"\n SDS: \"%s\" \n", sds_name);
        fprintf (stdout,"Rank: \"%i\" \n", rank);
        fprintf (stdout,"type: \"%i\" \n", type);
        fprintf (stdout,"sds_dims[0]: \"%i\" \n", sds_dims[0]);
        fprintf (stdout,"sds_dims[1]: \"%i\" \n", sds_dims[1]);
        fprintf (stdout,"sds_dims[2]: \"%i\" \n", sds_dims[2]);
        fprintf (stdout,"sds_dims[3]: \"%i\" \n", sds_dims[3]);

     sds_4d_data_array = calloc(sds_dims[0]*sds_dims[1]*sds_dims[2], sds_dims[3]*sizeof(float32));

/*
 *******************************************************************************
 *  Read the "Tuned Pristine SW Surface Up" SDS into the 4-D "sds_4d_data_array" array.
 *******************************************************************************
*/
     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_4d_data_array);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%i\" ", sds_index);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "Tuned Total-Sky LW Up" SDS, stored in
 *  the 4-D array, "sds_4d_data_array".
 *******************************************************************************
*/
     index_max0 = maxOut;
     index_max1 = maxOut;
     index_max2 = maxOut;
     index_max3 = maxOut;
     if ( sds_edge[3] < maxOut)
     {
        index_max3 = sds_edge[3];
     }
     if ( sds_edge[2] < maxOut)
     {
        index_max2 = sds_edge[2];
     }
     if ( sds_edge[1] < maxOut)
     {
        index_max1 = sds_edge[1];
     }
     if ( sds_edge[0] < maxOut)
     {
        index_max0 = sds_edge[0];
     }

     printf ("\n\n\n************************************************************\n");
     printf ("*  Example 3:   ( ONLY PRINTING MAX OF %i PER DIMENSION)    *\n",maxOut);
     printf ("************************************************************");

     for (i=0; i<index_max0; i++)
     {

       printf ("\n\n%s Flux Profile Level =  %i  \n",sds_name,i+1);

        for (k=0; k<index_max1; k++)
        {
          if (k==0)
          printf ("\n\n%s Stat type =  %i  MEAN \n",sds_name,k+1);
          if (k==1)
          printf ("\n\n%s Stat type =  %i  STANDARD DEVIATION \n",sds_name,k+1);
 
          for (m=0; m<index_max2; m++)
          {
            printf ("\n\n%s for ZONE %i\n",sds_name,m+1);
            for (p=0; p<index_max3; p++)
            {
                    n = i * sds_edge[1] * sds_edge[2] * sds_edge[3]
                      + k * sds_edge[2] * sds_edge[3]
                      + m * sds_edge[3] + p ;
                    printf ("      Longitude Region (%i)[%i] = %f\n",
                                  p+1,n+1,sds_4d_data_array[n]);
            } //index_max3
          } //index_max2
        } //index_max1
     } // index_max0
     printf ("\n");

     free(sds_4d_data_array);

     fprintf (stderr,"\n At end of Example 3 with SDS  \"%i\" \n\n", sds_index);


/*
 *******************************************************************************
 *  Example 4) This example extracts string data from a Vdata field in the
 *             "CERES_metadata" and prints it out in ascii text.
 * Note: string_data was declared an array above.
 *******************************************************************************
*/
     num_fields    = 10;
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
     printf ("\n\n*********************************************************\n");
     printf ("*    Example 4:      (vData )                           *\n");
     printf ("*********************************************************\n");
     printf ("\nASSOCIATEDPLATFORMSHORTNAME = ");
     for(i=0; i<32; i++)
     {
        if(string_data[i] == 0)break;
        printf("%c",string_data[i]);
     }
     printf ("\n");

     fprintf (stderr," At end of example 4 with vdata \n\n");

   } // End of Month



/* ********************************************************************************************************* */
/* ******************************* SYN1deg-M3Hour ********************************************************** */ 
/* ********************************************************************************************************* */
   /*   AVG_f3 = SYN1deg-3Hour    */

   if (strncmp(AVG_f3  ,part_hdf,18) == 0 )
   {
      printf("\n Name of 5-zone 5-region SYN1deg-M3Hour file :%s\n",HDF_File);

/*
 *******************************************************************************
 *  Example for SYN1deg-M3Hour
 *******************************************************************************
/*
 *******************************************************************************
 *  Example 1) Example of how to extract an SDS of rank 2 ( 2-dimensional ) 
 *             from the HDF file and store it in memory.  The SDS, 
 *             "Region Number"   , with index number 0 will be read and stored 
 *             in the 2-D array, "sds_2d_data_array".  The values for sds_name, 
 *             sds_rank, and sds_type are now determined by the SDS index, 
 *             set for the 2-D SDS, "Region Number"   .
 *******************************************************************************
 * This example uses function 'get_sds_dims_by_index' and 'read_sds_by_index'
 * 
 *******************************************************************************
 *
 *******************************************************************************
 *  To retrieve another SDS change sds_index to the index of the SDS you want
 *     to retrieve
 *******************************************************************************
*/
     printf ("\n\n*********************************************************\n");
     printf ("*        Start Example 1   (2 dimensional data)         *\n");
     printf ("*********************************************************");


     sds_index = 0;
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
 *  Read the "Region Number"   SDS into the 2-D "sds_2d_data_array" array. 
 *******************************************************************************
*/
     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_2d_data_array);

        fprintf (stdout,"\n SDS name: \"%s\" \n", sds_name);
        fprintf (stdout,"Rank: \"%i\" \n", rank);
        fprintf (stdout,"type: \"%i\" \n", type);
        fprintf (stdout,"sds_dims[0]: \"%i\" \n", sds_dims[0]);
        fprintf (stdout,"sds_dims[1]: \"%i\" \n", sds_dims[1]);

        fprintf (stdout,"sds_strt[0]: \"%i\" \n", sds_strt[0]);
        fprintf (stdout,"sds_strt[1]: \"%i\" \n", sds_strt[1]);
        fprintf (stdout,"sds_incr[0]: \"%i\" \n", sds_incr[0]);
        fprintf (stdout,"sds_incr[1]: \"%i\" \n", sds_incr[1]);
        fprintf (stdout,"sds_edge[0]: \"%i\" \n", sds_edge[0]);
        fprintf (stdout,"sds_edge[1]: \"%i\" \n", sds_edge[1]);

     if ( sds_status != 0 )
     {
        i = 0;
        while((e=HEvalue(i)) !=DFE_NONE)
        {
          str=HEstring(e);
          sLen = strlen(str);
          fprintf(stderr,"strLen = %i \n", sLen);
          fprintf(stderr,"%s",*str);
          fprintf(stderr,"%s",str);
          i++;
        } 
        fprintf (stderr,"\nError %i in reading SDS: \"%i\" \n", sds_status,sds_index);
       
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "Region Number"   SDS, stored in 
 *  the 2-D array, "sds_2d_data_array".
 *******************************************************************************
*/

     index_max = maxOutReg;
     if ( sds_edge[1] < maxOutReg)
     {
        index_max = sds_edge[1];
     }

     printf ("\n\n*********************************************************\n");
     printf ("*  Example 1:   ( ONLY PRINTING MAX OF %i PER ZONE)      *\n",maxOutReg);
     printf ("*********************************************************");

     for (i=0; i<sds_edge[0]; i++)
     {
        printf ("\n\n%s for Zone %i\n",sds_name,i+1);
        for (j=0; j<index_max; j++)
        {
           k = i * sds_edge[1] + j;
           printf ("\n   Longitude Region (%i)[%i] = %f",
                      j+1,k+1,sds_2d_data_array[k]);
        }
     }
     printf ("\n");

     free(sds_2d_data_array);

     fprintf (stderr,"\n At end of Example 1 with SDS  \"%i\" \n", sds_index);


 /*
 *******************************************************************************
 *  Example for SYN1deg-M3Hour
 *******************************************************************************
 *******************************************************************************
 *  Example 2) Example of how to extract an SDS of rank 3 ( 3-dimensional )
 *             from the HDF file and store it in memory. In this example the start point for
 *             reading (sds_strt) and the number of reads (sds_edge) have been altered 
 *             so that the surface type for the example is provided in the printout. 
 *             The SDS, "Surface type percent coverage" with index number 4 will be extracted and stored
 *             in the 3-D array, "sds_3d_data_array".  The values for sds_name,
 *             sds_rank, and sds_type are now determined by the SDS index,
 *             set for the 3-D SDS, "Surface type percent coverage"
 *******************************************************************************
 * This example uses function 'get_sds_dims_by_index' and 'read_sds_by_index'.
 *
 *******************************************************************************
 *  To retrieve another SDS change sds_index to the index of the SDS you want
 *     to retrieve
 *******************************************************************************
*/

     printf ("\n\n*********************************************************\n");
     printf ("*             Start Example 2   (3 Dimensional data)    *\n");
     printf ("*********************************************************");

     sds_index = 4;
     get_sds_dims_by_index(HDF_File,sds_index,sds_name,&rank,sds_dims,&type);

        fprintf (stdout,"\n SDS: \"%s\" \n", sds_name);
        fprintf (stdout,"Rank: \"%i\" \n", rank);
        fprintf (stdout,"type: \"%i\" \n", type);
        fprintf (stdout,"sds_dims[0]: \"%i\" \n", sds_dims[0]);
        fprintf (stdout,"sds_dims[1]: \"%i\" \n", sds_dims[1]);
        fprintf (stdout,"sds_dims[2]: \"%i\" \n", sds_dims[2]);

     q = 15;
     sds_rank = rank;
     sds_type = type;
     sds_edge[0] = 5; // sds_dims[0];
     sds_edge[1] = sds_dims[1];
     sds_edge[2] = sds_dims[2];
     sds_strt[0] = q; // 0;
     sds_strt[1] = 0;
     sds_strt[2] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;
     sds_incr[2] = 1;

     sds_3d_data_array = calloc(sds_dims[0]*sds_dims[1], sds_dims[2]*sizeof(float32));

/*
 *******************************************************************************
 *  Read the "Surface type percent coverage" SDS into the 3-D "sds_3d_data_array" array.
 *******************************************************************************
*/
     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_3d_data_array);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%i\" ", sds_index);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "Surface type percent coverage" SDS, stored in
 *  the 3-D array, "sds_3d_data_array".
 *******************************************************************************
*/
     index_max1 = maxOut;
     index_max2 = maxOut;
     index_max0 = maxOut;
     if ( sds_edge[0] < maxOut)
     {
        index_max0 = sds_edge[0];
     }
     if ( sds_edge[2] < maxOut)
     {
        index_max2 = sds_edge[2];
     }
     if ( sds_edge[1] < maxOut)
     {
        index_max1 = sds_edge[1];
     }


     printf ("\n\n\n************************************************************\n");
     printf ("*  Example 2:   ( ONLY PRINTING MAX OF %i PER DIMENSION)    *\n",maxOut);
     printf ("************************************************************");

     for (i=0; i<index_max0; i++)
     {
        printf ("\n\n%s for surface type =  %i\n",sds_name,q+i+1);
 
        for (k=0; k<index_max1; k++)
        {
            printf ("\n\n%s for Zone %i\n",sds_name,k+1);
            for (m=0; m<index_max2; m++)
            {
                n = i * sds_edge[1] * sds_edge[2] + k * sds_edge[2] + m ;
                printf ("      Longitude Region (%i)[%i] = %f\n",
                              m+1,n+1,sds_3d_data_array[n]);
            }
        }
     }
     printf ("\n");

     free(sds_3d_data_array);

     fprintf (stderr,"\n At end of Example 2 with SDS  \"%i\" \n", sds_index);


/*
 *******************************************************************************
 *  Example for SYN1deg-M3Hour
 *******************************************************************************
 *******************************************************************************
 *  Example 3) Example of how to extract an SDS of rank 4 ( 4-dimensional )
 *             from the HDF file and store it in memory.  The SDS,
 *             "Aerosol Opt. Depth at 0.66um"   , with index number 79 will be read and stored
 *             in the 4-D array, "sds_4d_data_array".  The values for sds_name,
 *             sds_rank, and sds_type are now determined by the SDS index,
 *             set for the 4-D SDS, "Aerosol Opt. Depth at 0.66um" monthly hourly average.
 *******************************************************************************
 * This example uses function 'get_sds_dims_by_index' and 'read_sds_by_index'.
 *
 *******************************************************************************
 *  To retrieve another SDS change sds_index to the index of the SDS you want
 *     to retrieve
 *******************************************************************************
*/

     printf ("\n\n*********************************************************\n");
     printf ("*                      Start Example 3  (4 Dimensions)  *\n");
     printf ("*********************************************************");

     sds_index = 79;
     get_sds_dims_by_index(HDF_File,sds_index,sds_name,&rank,sds_dims,&type);
        fprintf (stdout,"\n SDS: \"%s\" \n", sds_name);
        fprintf (stdout,"Rank: \"%i\" \n", rank);
        fprintf (stdout,"type: \"%i\" \n", type);
        fprintf (stdout,"sds_dims[0]: \"%i\" \n", sds_dims[0]);
        fprintf (stdout,"sds_dims[1]: \"%i\" \n", sds_dims[1]);
        fprintf (stdout,"sds_dims[2]: \"%i\" \n", sds_dims[2]);
        fprintf (stdout,"sds_dims[3]: \"%i\" \n", sds_dims[3]);

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

     sds_4d_data_array = calloc(sds_dims[0]*sds_dims[1]*sds_dims[2], sds_dims[3]*sizeof(float32));

/*
 *******************************************************************************
 *  Read the "Aerosol Opt. Depth at 0.66um" SDS into the 4-D "sds_4d_data_array" array.
 *******************************************************************************
*/
     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_4d_data_array);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%i\" ", sds_index);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "Aerosol Opt. Depth at 0.63um" SDS, stored in
 *  the 4-D array, "sds_4d_data_array".
 *******************************************************************************
*/
     index_max1 = maxOut;
     index_max2 = maxOut;
     index_max3 = maxOut;
     if ( sds_edge[3] < maxOut)
     {
        index_max3 = sds_edge[3];
     }
     if ( sds_edge[2] < maxOut)
     {
        index_max2 = sds_edge[2];
     }
     if ( sds_edge[1] < maxOut)
     {
        index_max1 = sds_edge[1];
     }

     printf ("\n\n************************************************************\n");
     printf ("*  Example 3:   ( ONLY PRINTING MAX OF %i PER DIMENSION)    *\n",maxOut);
     printf ("************************************************************");

     for (i=0; i<sds_edge[0]; i++)
     {
        if (i==0)
        printf ("\n\n%s Stat type =  %i  MEAN \n",sds_name,i+1);
        if (i==1)
        printf ("\n\n%s Stat type =  %i  STANDARD DEVIATION \n",sds_name,i+1);

        for (k=0; k<index_max1; k++)
        {
            printf ("\n\n%s for Hour %i\n",sds_name,k+1);
            for (m=0; m<index_max2; m++)
            {
                printf ("\n\n%s for Zone %i\n",sds_name,m+1);
                for (p=0; p<index_max3; p++)
                {
                    n = i * sds_edge[1] * sds_edge[2] * sds_edge[3]
                      + k * sds_edge[2] * sds_edge[3]
                      + m * sds_edge[3] + p ;
                    printf ("      Longitude Region (%i)[%i] = %f\n",
                                  p+1,n+1,sds_4d_data_array[n]);
                }
            }
        }
     }
     printf ("\n");
     free(sds_4d_data_array);

     fprintf (stderr,"\n At end of Example 3 with SDS  \"%i\" \n", sds_index);

/*
 *******************************************************************************
 *  Example for SYN1deg-M3Hour
 *******************************************************************************
 *******************************************************************************
 *  Example 4) Example of how to extract an SDS of rank 5 ( 5-dimensional )
 *             from the HDF file and store it in memory.  The SDS,
 *             "Tuned Total-Sky LW Up" monthly 3 hour data,
 *             with index number 110 will be extracted and stored
 *             in the 5-D array, "sds_5d_data_array".  The values for sds_name,
 *             sds_rank, and sds_type are now determined by the SDS index,
 *             set for the 5-D SDS, "Tuned Total-Sky LW Up"
 *******************************************************************************
 * This example uses function 'get_sds_dims_by_index' and 'read_sds_by_index'.
 *
 *******************************************************************************
 *  To retrieve another SDS change sds_index to the index of the SDS you want
 *     to retrieve
 *******************************************************************************
*/

     printf ("\n\n\n*********************************************************\n");
     printf ("*                      Start Example 4   (5 dimensions) *\n");
     printf ("*********************************************************");

     sds_index = 110;
     get_sds_dims_by_index(HDF_File,sds_index,sds_name,&rank,sds_dims,&type);

        fprintf (stdout,"\n SDS: \"%s\" \n", sds_name);
        fprintf (stdout,"Rank: \"%i\" \n", rank);
        fprintf (stdout,"type: \"%i\" \n", type);
        fprintf (stdout,"sds_dims[0]: \"%i\" \n", sds_dims[0]);
        fprintf (stdout,"sds_dims[1]: \"%i\" \n", sds_dims[1]);
        fprintf (stdout,"sds_dims[2]: \"%i\" \n", sds_dims[2]);
        fprintf (stdout,"sds_dims[3]: \"%i\" \n", sds_dims[3]);
        fprintf (stdout,"sds_dims[4]: \"%i\" \n", sds_dims[4]);

     sds_rank = rank;
     sds_type = type;
     sds_edge[0] = sds_dims[0];
     sds_edge[1] = sds_dims[1];
     sds_edge[2] = sds_dims[2];
     sds_edge[3] = sds_dims[3];
     sds_edge[4] = sds_dims[4];
     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_strt[2] = 0;
     sds_strt[3] = 0;
     sds_strt[4] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;
     sds_incr[2] = 1;
     sds_incr[3] = 1;
     sds_incr[4] = 1;

     sds_5d_data_array = calloc(sds_dims[0]*sds_dims[1]*sds_dims[2]*sds_dims[3],
                                sds_dims[4]*sizeof(float32));

/*
 *******************************************************************************
 *  Read the "Longwave flux" SDS into the 5-D "sds_5d_data_array" array.
 *******************************************************************************
*/
     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_5d_data_array);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%i\" ", sds_index);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "Longwave flux" SDS, stored in
 *  the 5-D array, "sds_5d_data_array".
 *******************************************************************************
*/
     index_max0 = maxOut;
     index_max1 = maxOut;
     index_max2 = maxOut;
     index_max3 = maxOut;
     index_max4 = maxOut;
     if ( sds_edge[4] < maxOut)
     {
        index_max4 = sds_edge[4];
     }
     if ( sds_edge[3] < maxOut)
     {
        index_max3 = sds_edge[3];
     }
     if ( sds_edge[2] < maxOut)
     {
        index_max2 = sds_edge[2];
     }
     if ( sds_edge[1] < maxOut)
     {
        index_max1 = sds_edge[1];
     }
     if ( sds_edge[0] < maxOut)
     {
        index_max0 = sds_edge[0];
     }

     printf ("\n\n\n************************************************************\n");
     printf ("*  Example 4:   ( ONLY PRINTING MAX OF %i PER DIMENSION)    *\n",maxOut);
     printf ("************************************************************");

  for (q=0; q<index_max0; q++)    /* 5 pressure levels */
  {
     printf ("\n\n%s:   Profile level =  %i  \n",sds_name,q+1);

     for (i=0; i<index_max1; i++)
     {
        if (i==0)
        printf ("\n\n%s:   Stat type =  %i  MEAN \n",sds_name,i+1);
        if (i==1)
        printf ("\n\n%s:   Stat type =  %i  STANDARD DEVIATION \n",sds_name,i+1);

        for (k=0; k<index_max2; k++)
        {
            printf ("\n\n%s:  Monthly 3 hour avg for hour %i\n",sds_name,k+1);
            for (m=0; m<index_max3; m++)
            {
                printf ("\n\n%s for Zone %i\n",sds_name,m+1);
                for (p=0; p<index_max4; p++)
                {
                    n = q * sds_edge[1] * sds_edge[2] * sds_edge[3] * sds_edge[4]
                      + i * sds_edge[2] * sds_edge[3] * sds_edge[4]
                      + k * sds_edge[3] * sds_edge[4]
                      + m * sds_edge[4] + p ;
                    printf ("      Longitude Region (%i)[%i] = %f\n",
                                  p+1,n+1,sds_5d_data_array[n]);
                }
            }
        }
     }
   }
     printf ("\n");

     free(sds_5d_data_array);

     fprintf (stderr,"\n At end of Example 4 with SDS  \"%i\" \n", sds_index);


/*
 *******************************************************************************
 *  Example for SYN1deg-M3Hour
 *******************************************************************************
 *******************************************************************************
 *  Example 5) This example extracts string data from a Vdata field in the
 *             "CERES_metadata" and prints it out in ascii text.
 * Note: string_data was declared an array above.
 *******************************************************************************
*/
     num_fields    = 10;
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
     printf ("\n\n*********************************************************\n");
     printf ("*    Example 5:   (vdata)                                      *\n");
     printf ("*********************************************************\n");
     printf ("\nASSOCIATEDPLATFORMSHORTNAME = ");
     for(i=0; i<32; i++)
     {
        if(string_data[i] == 0)break;
         printf("%c",string_data[i]);
     }
     printf("\n");
     fprintf (stderr,"\n At end of Example 5 vdata  \n");
   } // Endif M3H
 
   exit(0);
}
