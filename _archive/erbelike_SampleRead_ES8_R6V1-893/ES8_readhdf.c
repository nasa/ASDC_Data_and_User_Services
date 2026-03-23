/* ===========================================================================
    READ HDF FILE
    original 98.01.16 Kam-Pui Lee (757) 827-4864 k.lee@larc.nasa.gov
                      Pete Spence (757) 825-7024 p.l.spence@larc.nasa.gov
    modified 98.02.17 Susan Snell (757) 827-4889 s.h.snell@larc.nasa.gov
    modified 02/28/2000 Sandra Nolan (757) 827-4652 s.k.nolan@larc.nasa.gov


Please refer to the following examples in the body of this program.

Example 1) Illustrates reading from a SDS and storing the data to an array.
Example 2) Illustrates reading from a SDS and writing the data to a file.
Example 3) Illustrates reading from a Vdata and storing the data to an array.
Example 4) Illustrates reading from a Vdata and writing the data to a file.


A utility (hdp) is provided in the HDF libraries which will
extract information about the contents of the HDF file. The
commands:

hdp -H
hdp dumpsds [-options] <HDF file name>
hdp dumpvd [-options] <HDF file name>
vshow <HDF file name> [+|+vdata_id]

These will give information as to the size, type, and names of
the HDF data structures. The [-H] option will give help information.

Another utility is provided with this package which will
provide the same information. Please read the README_contents
file.




The four examples require the 'type' of data to be read. The HDF data types
are defined in the following table. Although names (first column) are
permitted, only the values (center column) are used in the examples.

   Definition Name   |   Definition Value   |       Description
---------------------|----------------------|--------------------------------
   DFNT_CHAR8        |          4           |   8-bit character type
   DFNT_CHAR         |          4           |   Same as DFNT_CHAR8
   DFNT_UCHAR8       |          3           |   8-bit unsigned character type
   DFNT_UCHAR        |          3           |   Same as DFNT_UCHAR8
   DFNT_INT8         |         20           |   8-bit integer type
   DFNT_UINT8        |         21           |   8-bit unsigned integer type
   DFNT_INT16        |         22           |   16-bit integer type
   DFNT_UINT16       |         23           |   16-bit unsigned integer type
   DFNT_INT32        |         24           |   32-bit integer type
   DFNT_UINT32       |         25           |   32-bit unsigned integer type
   DFNT_INT64        |         26           |   64-bit integer type
   DFNT_UINT64       |         27           |   64-bit unsigned integer type
   DFNT_FLOAT32      |          5           |   32-bit floating-point type
   DFNT_FLOAT64      |          6           |   64-bit floating-point type



============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ES8_HDFread.h"

/* ===========================================================================
MAIN PROGRAM
============================================================================*/
void main( int argc, char *argv[])
{

  char *HDF_File;
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
  int32 num_records;
  int32 rd_num_fields;
  int32 rd_num_recs;
  int32 vfield_types[MAX_VDATA_FIELDS];
  int32 vfield_order[MAX_VDATA_FIELDS];
  int32 vfield_status;
  long  vdata_status;
  int32 index;
  int32 i,j,k;
  int   index_max;
  long  vdata_n_rec;
  void *vdata_data[MAX_VDATA_FIELDS];


/* ===========================================================================
Arrays assigned for the examples.
============================================================================*/

  float32   *sds_2d_data32_array;
  float64   *sds_2d_data64_array;

/* ===========================================================================
Assign the HDF file to be read
============================================================================*/

  if(argc != 2)
  {
     fprintf ( stderr, "Usage: ES8_rd HDFfilename\n");
     exit(1);
  }

  HDF_File = argv[1];

  printf("\nName of HDF file to be read:\n< %s\n",HDF_File);

  if (!Hishdf(HDF_File))
  {
    fprintf(stderr, "ERROR - specified file is not an HDF file.\n");
    exit (1);
  } 


/* ===========================================================================
Example 1) Extract a SDS from the HDF file and store in memory.

  The SDS "CERES SW filtered radiance" will be extracted
  and written out to a file. This example illustrates the use of functions
  'get_sds_dims_by_index' and 'read_sds_by_index'.

  'get_sds_dims_by_index'
     Function - Returns a void pointer to the address to write the SDS data set.
     Input Variables:
        HDF_File - character pointer to the name of the HDF file.
        sds_rank - integer defining index of the SDS data set.
     Output Variable:
        sds_rank - integer defining rank of the SDS data set.
        sds_type - integer defining type of SDS data set (see table above).
        sds_dims - integer array of rank 'sds_rank' containing dimensions
                   of the SDS data set.
        sds_name - character pointer to the name of the SDS data set.

  'read_sds_by_index'
     Function - Reads a SDS data set from a HDF file.
     Input Variables:
        HDF_File - character pointer to the name of the HDF file.
        sds_index- integer defining the SDS_index of the data set.
        sds_rank - integer defining rank of the SDS data set.
        sds_type - integer defining type of SDS data set (see table above).
        sds_strt - integer array of starting index of each dimension of the SDS.
        sds_incr - integer array of increment of each dimension of the SDS.
        sds_edge - integer array of number of values of each dimension
                   of the SDS.  For this example, 100 is used to represent the
                   number of records to be read and 660 will represent the number
                   of elements within one record of the SDS chosen.
     Output Variable:
        sds_2d_data32_array - output void pointer later cast to type 'sds_type'

  Note: sds_2d_data32_array was declared an array above.
============================================================================*/

     sds_index = 8;
     get_sds_dims_by_index(HDF_File,sds_index,sds_name,&rank,sds_dims,&type);

     sds_rank = rank;
     sds_type = type;
     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;
     sds_edge[0] = sds_dims[0];
     sds_edge[1] = sds_dims[1];

num_records = sds_dims[0];

/*    num_records = sds_dims[0]
      num_scans = sds_dims[1]  */


     sds_2d_data32_array = calloc(sds_dims[0], sds_dims[1]*sizeof(float32));

     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_2d_data32_array);

     if ( sds_status != 0 )
     {
        printf ("\nError in reading SDS: \"%i\" ", sds_index);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the SDS, stored in
 *  the 2-D array, "sds_2d_data32_array".
 *******************************************************************************
*/

     index_max = 5;
     if ( sds_edge[0] < index_max)
     {
        index_max = sds_edge[0];
     }

     printf ("\n\n\n*********************************************************\n");
     printf ("*  Example 1:   ( ONLY PRINTING MAX OF 5 REGIONS)       *\n");
     printf ("*********************************************************");

     for (i=0; i<index_max; i++)
     {
        printf ("\n\n%s for Region %i\n",sds_name,i+1);
        for (j=0; j<sds_edge[1]; j++)
        {
           if ( j % 30 == 0 && j != 0)
           {
               printf ("\n%s for Region %i (Continued)\n",sds_name,i+1);
           }
           k = i * sds_edge[1] + j;
           printf ("   Scan (%i) = %f\n",
                      j+1,sds_2d_data32_array[k]);

        }
     }
     printf ("\n\n");

     free(sds_2d_data32_array);



/* ===========================================================================
Example 2) Extract a SDS from the HDF file and print out data in ascii text

  The SDS "CERES SW unfiltered radiance" will be extracted
  and written out to a file. This example illustrates the use of functions
  'get_sds_dims_by_index' and 'read_sds_by_index'.

  'get_sds_dims_by_index'
     Function - Returns a void pointer to the address to write the SDS data set.
     Input Variables:
        HDF_File - character pointer to the name of the HDF file.
        sds_rank - integer defining index of the SDS data set.
     Output Variable:
        sds_rank - integer defining rank of the SDS data set.
        sds_type - integer defining type of SDS data set (see table above).
        sds_dims - integer array of rank 'sds_rank' containing dimensions
                   of the SDS data set.
        sds_name - character pointer to the name of the SDS data set.

  'read_sds_by_index'
     Function - Reads a SDS data set from a HDF file.
     Input Variables:
        HDF_File - character pointer to the name of the HDF file.
        sds_index- integer defining the SDS_index of the data set.
        sds_rank - integer defining rank of the SDS data set.
        sds_type - integer defining type of SDS data set (see table above).
        sds_strt - integer array of starting index of each dimension of the SDS.
        sds_incr - integer array of increment of each dimension of the SDS.
        sds_edge - integer array of number of values of each dimension
                   of the SDS.  For this example, 100 is used to represent the
                   number of records to be read and 660 will represent the number 
                   of elements within one record of the SDS chosen.
     Output Variable:
        sds_2d_data32_array - output void pointer later cast to type 'sds_type'
============================================================================*/

     sds_index = 8;
     get_sds_dims_by_index(HDF_File,sds_index,sds_name,&rank,sds_dims,&type);

     sds_rank = rank;
     sds_type = type;
     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;
     sds_edge[0] = sds_dims[0];
     sds_edge[1] = sds_dims[1];

num_records = sds_dims[0];

/*    num_records = sds_dims[0]
      num_scans = sds_dims[1]  */


     sds_2d_data32_array = calloc(sds_dims[0], sds_dims[1]*sizeof(float32));

     sds_status = read_sds_by_index(HDF_File, sds_index, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_2d_data32_array);

     if ( sds_status != 0 )
     {
        printf ("\nError in reading SDS: \"%i\" ", sds_index);
        exit(1);
     }



/*
 *******************************************************************************
 *  Write the values in the SDS, stored in
 *  the 2-D array, "sds_2d_data32_array".
 *******************************************************************************
*/

     index_max = 25;
     if ( sds_edge[0] < index_max)
     {
        index_max = sds_edge[0];
     }

     printf ("\n\n\n************************************************************\n");
     printf ("*  Example 2:   ( ONLY PRINTING MAX OF 25 REGIONS)         *\n");
     printf ("************************************************************");


     for (i=0; i<index_max; i++)
  /*    The values print to the output file will be of "CERES SW unfiltered 
        radiance", record numbers 0 - 99, element numbers 200 and 400.     */
     {
        index = i*sds_dims[1] - 1;
        fprintf (stdout,"\nRegion (%i),  %s @[200],[400] = %f %f", i+1, sds_name,
                 *((float32 *)sds_2d_data32_array+index+200),
                 *((float32 *)sds_2d_data32_array+index+400));
     }
     free(sds_2d_data32_array);




/* ===========================================================================
Example 3) Extract a Vdata field from the HDF file and store in memory.

  The Vdata field "Time of observation" in Vdata "Time of observation" will 
  be extracted and stored in the array sds_2d_data64_array. This example
  illustrates the use of functions 'get_vfield_types' and 'read_vdata'.

  'get_vfield_types'
     Function - Gets arrays of types and orders of the fields within a Vdata.
     Input Variables:
        HDF_File      - character pointer to the name of the HDF file.
        vdata_name    - character pointer to the name of the Vdata data set.
        num_fields    - integer defining number of fields in the Vdata.
     Output Variable:
        vfield_types  - integer array containing the types of the fields within
                        the Vdata data set (see table above).
        vfield_order  - integer array containing the order of the fields within
                        the Vdata data set.

  'read_vdata'
     Function - Reads a Vdata data set from a HDF file.
     Input Variables:
        HDF_File      - character pointer to the name of the HDF file.
        vdata_name    - character pointer to the name of the Vdata data set.
        rd_num_fields - integer defining number of fields to read.
        rd_num_recs   - integer defining number of records to read.
        vfield_name   - character pointer to the name of the vfield data set.
        vfield_types  - integer array containing the types of the fields within
                        the Vdata data set (see table above).
        vfield_order  - integer array containing the order of the fields within
                        the Vdata data set.
     Output Variable:
        vdata_data    - void pointer (cast to the array 'sds_2d_data64_array' 
                        in this example).
  
  Note: sds_2d_data64_array was declared an array above.
============================================================================*/
 
     num_fields = 1;
     vdata_name = "Time of observation";
     vfield_name = "Time of observation";

     sds_2d_data64_array = malloc(num_records*sizeof(float64));

     vfield_status = get_vfield_types(HDF_File, vdata_name,
                     num_fields, vfield_types, vfield_order);
     if ( vfield_status != 0 )
     {
        printf ("\nError in getting Vdata field types or orders\n");
        exit(1);
     }

     vdata_data[0]=sds_2d_data64_array;
     rd_num_fields = 1;
     rd_num_recs = num_records;
     vdata_status = read_vdata(HDF_File, vdata_name, rd_num_fields,
                    rd_num_recs, vfield_name, vfield_types, vfield_order,
                    vdata_data);
                    
     if ( vdata_status != 0 )
     {
        printf ("\nError in reading Vdata: \"%s\" ", vdata_name);
        exit(1);
     }

     index_max = 25;
     if ( num_records < index_max)
     {
        index_max = num_records;
     }

     printf ("\n\n\n************************************************************\n");
     printf ("*  Example 3:   ( ONLY PRINTING MAX OF 25 REGIONS)         *\n");
     printf ("************************************************************");

     printf ("\n%s\n",vdata_name);
     for (k=0; k<index_max; k++)
     {
         if ( k % 30 == 0 && k != 0)
         {
             printf ("\n%s (Continued)\n",vdata_name);
         }
         printf ("   Region (%i) = %f\n", 
                    k+1,sds_2d_data64_array[k]);
     }




/* ===========================================================================
Example 4) Extract a Vdata field from the HDF file and print out data in ascii
           text

  The Vdata field "Time of observation" in Vdata "Time of observation" will 
  be extracted and written to a file.  This example illustrates the use of 
  functions 'get_vfield_types', 'get_vdata_n_rec', 'set_vdata_data', and 
  'read_vdata'.
  
  'get_vfield_types'
     Function - Gets arrays of types and orders of the fields within a Vdata.
     Input Variables:
        HDF_File      - character pointer to the name of the HDF file.
        vdata_name    - character pointer to the name of the Vdata data set.
        num_fields    - integer defining number of fields in the Vdata.
     Output Variable:
        vfield_types  - integer array containing the types of the fields within
                        the Vdata data set (see table above).
        vfield_order  - integer array containing the order of the fields within
                        the Vdata data set.

  'get_vdata_n_rec'
     Function - Returns the number of records in the Vdata.
     Input Variables:
        HDF_File      - character pointer to the name of the HDF file.
        vdata_name    - character pointer to the name of the Vdata data set.

  'set_vdata_data'
     Function - Returns a void pointer to the address to write the Vdata
                data set.
     Input Variables:
        num_fields    - integer defining number of fields in the Vdata.
        vfield_types  - integer array containing the types of the fields within
                        the Vdata data set (see table above).
        vfield_order  - integer array containing the order of the fields within
                        the Vdata data set.
        vdata_n_rec   - integer number of records in the Vdata.
     Output Variable:
        vdata_data    - void pointer

  'read_vdata'
     Function - Reads a Vdata data set from a HDF file.
     Input Variables:
        HDF_File      - character pointer to the name of the HDF file.
        vdata_name    - character pointer to the name of the Vdata data set.
        rd_num_fields - integer defining number of fields to read.
        rd_num_recs   - integer defining number of records to read.
        vfield_name   - character pointer to the name of the vfield data set.
        vfield_types  - integer array containing the types of the fields within
                        the Vdata data set (see table above).
        vfield_order  - integer array containing the order of the fields within
                        the Vdata data set.
     Output Variable:
        vdata_data    - void pointer to the address of the Vdata vfield data
                        set.
============================================================================*/
 
     num_fields = 1;
     vdata_name = "Time of observation";
     vfield_name = "Time of observation";
     vfield_status = get_vfield_types(HDF_File, vdata_name,
                     num_fields, vfield_types, vfield_order);
     if ( vfield_status != 0 )
     {
        printf ("\nError in getting Vdata field types or orders");
        exit(1);
     }
     vdata_n_rec = get_vdata_n_rec(HDF_File, vdata_name);
     set_vdata_data(num_fields, vfield_types, vfield_order, (int32)vdata_n_rec,
                    vdata_data);

     rd_num_fields = 1;
     rd_num_recs = num_records;
     vdata_status = read_vdata(HDF_File, vdata_name, rd_num_fields,
                    rd_num_recs, vfield_name, vfield_types, vfield_order,
                    vdata_data);
     if ( vdata_status != 0 )
     {
        printf ("\nError in reading Vdata: \"%s\" ", vdata_name);
        free_vdata(num_fields, vdata_data);
        exit(1);
     }


     index_max = 25;
     if ( num_records < index_max)
     {
        index_max = num_records;
     }

     printf ("\n\n\n************************************************************\n");
     printf ("*  Example 4:   ( ONLY PRINTING MAX OF 25 REGIONS)         *\n");
     printf ("************************************************************");

     printf ("\n%s\n",vdata_name);
     for (i=0; i<index_max; i++)
     {
        if ( i % 30 == 0 && i != 0)
        {
            printf ("\n%s (Continued)\n",vdata_name);
        }
        fprintf (stdout,"   Region (%i) = %f\n",i+1,
                  *((float64 *)vdata_data[0]+i));
     }
     free_vdata(num_fields, vdata_data);

/* ===========================================================================
Example 5) Extract a Vdata field from the HDF file and print out data in ascii
           text

  The Vdata field "SW spectral response values" in Vdata
  "Spectral Response Functions" will
  be extracted and written to a file.  This example illustrates the use of
  functions 'get_vfield_types', 'get_vdata_n_rec', 'set_vdata_data', and
  'read_vdata'.

  'get_vfield_types'
     Function - Gets arrays of types and orders of the fields within a Vdata.
     Input Variables:
        HDF_File      - character pointer to the name of the HDF file.
        vdata_name    - character pointer to the name of the Vdata data set.
        num_fields    - integer defining number of fields in the Vdata.
     Output Variable:
        vfield_types  - integer array containing the types of the fields within
                        the Vdata data set (see table above).
        vfield_order  - integer array containing the order of the fields within
                        the Vdata data set.

  'get_vdata_n_rec'
     Function - Returns the number of records in the Vdata.
     Input Variables:
        HDF_File      - character pointer to the name of the HDF file.
        vdata_name    - character pointer to the name of the Vdata data set.

  'set_vdata_data'
     Function - Returns a void pointer to the address to write the Vdata
                data set.
     Input Variables:
        num_fields    - integer defining number of fields in the Vdata.
        vfield_types  - integer array containing the types of the fields within
                        the Vdata data set (see table above).
        vfield_order  - integer array containing the order of the fields within
                        the Vdata data set.
        vdata_n_rec   - integer number of records in the Vdata.
     Output Variable:
        vdata_data    - void pointer

  'read_vdata'
     Function - Reads a Vdata data set from a HDF file.
     Input Variables:
        HDF_File      - character pointer to the name of the HDF file.
        vdata_name    - character pointer to the name of the Vdata data set.
        rd_num_fields - integer defining number of fields to read.
        rd_num_recs   - integer defining number of records to read.
        vfield_name   - character pointer to the name of the vfield data set.
        vfield_types  - integer array containing the types of the fields within
                        the Vdata data set (see table above).
        vfield_order  - integer array containing the order of the fields within
                        the Vdata data set.
     Output Variable:
        vdata_data    - void pointer to the address of the Vdata vfield data
                        set.
============================================================================*/

     printf ("\n\n\n************************************************************\n");
     printf ("*  Example 5:   ( ONLY AVAILABLE on ES-8 products with a   *\n");
     printf ("*                 configuration code greater than 021018)  *\n");
     printf ("************************************************************\n");

     num_fields = 6;
     vdata_name = "Spectral Response Functions";
     vfield_name = "SW spectral response values";
     vfield_status = get_vfield_types(HDF_File, vdata_name,
                     num_fields, vfield_types, vfield_order);

     if ( vfield_status != 0 )
     {
        printf ("\nError in getting Vdata field types or orders");
        exit(1);
     }

     vdata_n_rec = get_vdata_n_rec(HDF_File, vdata_name);

     set_vdata_data(num_fields, vfield_types, vfield_order, vdata_n_rec,
                    vdata_data);

     rd_num_fields = 1;
     rd_num_recs = 1;
     vdata_status = read_vdata(HDF_File, vdata_name, rd_num_fields,
                    rd_num_recs, vfield_name, vfield_types, vfield_order,
                    vdata_data);
     if ( vdata_status != 0 )
     {
        printf ("\nError in reading Vdata: \"%s\" ", vdata_name);
        free_vdata(num_fields, vdata_data);
        exit(1);
     }

     index_max = vfield_order[0];

     printf ("************************************************************\n");
     printf ("*               ( ONLY PRINTING MAX OF 25 REGIONS)         *\n");
     printf ("************************************************************\n");

     printf ("%s\n",vdata_name);
     printf ("%s\n",vfield_name);

     for (i=0; i<index_max; i++)
     {
        if ( i % 30 == 0 && i != 0)
        {
            printf ("\n%s\n",vdata_name);
            printf ("%s (Continued)\n",vfield_name);
        }
        fprintf (stdout,"   Index (%i) = %f\n",i+1,
                  *((float32 *)vdata_data[0]+i));
     }
     free_vdata(num_fields, vdata_data);

}
