/* ===========================================================================
    READ HDF FILE
    98/06/01 Kam-Pui Lee (757) 827-4864 k.lee@larc.nasa.gov
             Pete Spence (757) 825-7024 p.l.spence@larc.nasa.gov


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
provide the same information. Please read the 'README_contents'
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


In the four examples presented, the number of records is analogous to a scan.
The CERES instrument takes 660 samples taken per scan. Hence for the BDS
many of the SDS's are of rank 2 (2 dimensional) with the first dimension
being number of records (scans) and the second dimension being number of
samples. Examples 1 and 2 simply store/print two samples of the first 10
records.


============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "HDFread.h"

/* ===========================================================================
MAIN PROGRAM
============================================================================*/
void main( int argc, char *argv[] )
{

  char *HDF_File;
  char *sds_name;
  char *vdata_name;
  char *vfield_name;
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
  int32 vfield_index[MAX_VDATA_FIELDS];
  int32 vfield_status;
  int32 vdata_status;
  int32 vprint_status;
  int32 i,k;
  int32 index;
  int32 vdata_n_rec;
  void *sds_data;
  void *vdata_data[MAX_VDATA_FIELDS];




/* ===========================================================================
Arrays assigned for the examples.
============================================================================*/
  float32 SW_rad[10][660];
  float64 sat_vel[10][3];

/* ===========================================================================
Assign the HDF file to be read
============================================================================*/

  if(argc != 2)
  {
     fprintf ( stderr, "Usage: rd HDFfilename\n");
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

  The SDS "CERES SW Filtered Radiances Upwards" will be extracted
  and stored in the array SW_rad. This example illustrates the use
  of function 'read_sds'. The first 10 records are read and printed.

  'read_sds'
     Function - Reads a SDS data set from a HDF file.
     Input Variables:
        HDF_File - character pointer to the name of the HDF file.
        sds_name - character pointer to the name of the SDS data set.
        sds_rank - integer defining rank of the SDS data set.
        sds_type - integer defining type of SDS data set (see table above).
        sds_strt - integer array of starting index of each dimension of the SDS.
        sds_incr - integer array of increment of each dimension of the SDS.
        sds_edge - integer array of number of values of each dimension
                   of the SDS.
     Output Variable:
        SW_rad   - output array of type 'sds_type' and rank 'sds_rank'.

  Note: SW_rad was declared an array above.
============================================================================*/

     sds_name = "CERES SW Filtered Radiances Upwards";
     sds_rank = 2;
     sds_type = 5;
     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;
     sds_edge[0] = 7;
     sds_edge[1] = 660;

     sds_status = read_sds(HDF_File, sds_name, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, SW_rad);
                  
     if ( sds_status != 0 )
     {
        printf ("\nError in reading SDS: \"%s\" ", sds_name);
        exit(1);
     }

     fprintf (stdout,"\nSDS data set: \"%s\" \n\n", sds_name);
     for (i=0; i<7; i++)
     {
        printf ("i = %i\n",i);
        printf ("SW_rad @[200],[400] = %f %f\n",SW_rad[i][200],
                 SW_rad[i][400]);
     }





/* ===========================================================================
Example 2) Extract a SDS from the HDF file and print out data in ascii text.

  The SDS "CERES SW Filtered Radiances Upwards" will be extracted
  and written out to a file. This example illustrates the use of functions
  'get_sds_dims', 'set_sds_data', 'read_sds', and 'print_sds'. The user may
  uncomment a print loop below the 'print_sds' function to compare results
  with Example 1.

  'get_sds_dims'
     Function - Gets the rank, type, and dimensions of the SDS data set.
     Input Variables:
        HDF_File - character pointer to the name of the HDF file.
        sds_name - character pointer to the name of the SDS data set.
     Output Variables:
        sds_rank - integer defining rank of the SDS data set.
        sds_type - integer defining type of SDS data set (see table above).
        sds_dims - integer array of rank 'sds_rank' containing dimensions
                   of the SDS data set.

  'set_sds_data'
     Function - Returns a void pointer to the address to write the SDS data set.
     Input Variables:
        sds_type - integer defining type of SDS data set (see table above).
        sds_rank - integer defining rank of the SDS data set.
        sds_dims - integer array of rank 'sds_rank' containing dimensions
                   of the SDS data set.

  'read_sds'
     Function - Reads a SDS data set from a HDF file.
     Input Variables:
        HDF_File - character pointer to the name of the HDF file.
        sds_name - character pointer to the name of the SDS data set.
        sds_rank - integer defining rank of the SDS data set.
        sds_type - integer defining type of SDS data set (see table above).
        sds_strt - integer array of starting index of each dimension of the SDS.
        sds_incr - integer array of increment of each dimension of the SDS.
        sds_edge - integer array of number of values of each dimension
                   of the SDS.
     Output Variables:
        sds_data - output void pointer later cast to type 'sds_type'

  'print_sds'
     Function - Prints a SDS data set. Values are printed by looping though
                highest dimention first and progressively looping back though
                the lower dimensions. See the following algorithm where R is
                rank of the SDS data set...

        for ( i=sds_strt[0]; i<sds_edge[0]; i+=sds_incr[0] )
        {
           for ( j=sds_strt[1]; j<sds_edge[1]; j+=sds_incr[1] )
           {
			.
			.
			.
              for ( m=sds_strt[R-2]; m<sds_edge[R-2]; m+=sds_incr[R-2] )
              {
                 for ( n=sds_strt[R-1]; n<sds_edge[R-1]; n+=sds_incr[R-1] )
                 {
                    *** print sds_data value ***
                 }
              }
			.
			.
			.
           }
        }

                5 values are printed per line. A line feed is executed at the
                completion of each loop.

     Input Variables:
        sds_name - character pointer to the name of the SDS data set.
        sds_rank - integer defining rank of the SDS data set.
        sds_type - integer defining type of SDS data set (see table above).
        sds_strt - integer array of starting index of each dimension of the SDS.
        sds_incr - integer array of increment of each dimension of the SDS.
        sds_edge - integer array of number of values of each dimension
                   of the SDS.
        sds_data - output void pointer later cast to type 'sds_type'
============================================================================*/

     sds_name = "CERES SW Filtered Radiances Upwards";

     get_sds_dims(HDF_File, sds_name, &sds_rank,
                  sds_dims, &sds_type);

     sds_data = set_sds_data(sds_type, sds_rank, sds_dims);

     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;
     sds_edge[0] = sds_dims[0];
     sds_edge[1] = sds_dims[1];

     sds_status = read_sds(HDF_File, sds_name, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, sds_data);
     if ( sds_status != 0 )
     {
        printf ("\nError in reading SDS: \"%s\" ", sds_name);
     }

     sds_status = print_sds(sds_name, sds_rank, sds_type, sds_strt, sds_incr,
                            sds_edge, sds_data);
     if ( sds_status != 0 )
     {
        printf ("\nError in printing SDS: \"%s\" ", sds_name);
     }
                           
/*
     for (i=0; i<10; i++)
     {
        printf ("i = %i\n",i);
        index = i*sds_dims[1];
        fprintf (stdout,"SW_rad @[200],[400] = %f %f\n",
                 *((float32 *)sds_data+index+200),
                 *((float32 *)sds_data+index+400));
     }
*/
   
     free(sds_data);

 






/* ===========================================================================
Example 3) Extract a Vdata field from the HDF file and store in memory.

  The Vdata field "Satellite Velocity at record start" in Vdata
  "Satellite-Celestial Data" will be extracted and stored in the array
  'sat_vel'. This example illustrates the use of functions 'get_vfield_types'
  and 'read_vdata'.

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
        vfield_name   - character pointer to the list of names of the Vdata
                        fields separated by commas ','.
        vfield_types  - integer array containing the types of the fields within
                        the Vdata data set (see table above).
        vfield_order  - integer array containing the order of the fields within
                        the Vdata data set.
     Output Variable:
        vfield_index  - integer array containing the indicies of the fields
                        within the Vdata data set. (needed for 'print_vdata'
                        function -- see Example 4)
        vdata_data    - void pointer (cast to the array 'sat_vel' in this
                        example).
  
  Note: sat_vel was declared an array above.
============================================================================*/
 
     num_fields = 18;
     vdata_name = "Satellite-Celestial Data";
     vfield_name = "Satellite Velocity at record start";
     vfield_status = get_vfield_types(HDF_File, vdata_name,
                     num_fields, vfield_types, vfield_order);
     if ( vfield_status != 0 )
     {
        printf ("\nError in getting Vdata field types or orders\n");
        exit(1);
     }

     vdata_data[0]=sat_vel;
     rd_num_fields = 1;
     rd_num_recs = 7;
     vdata_status = read_vdata(HDF_File, vdata_name, rd_num_fields,
                    rd_num_recs, vfield_name, vfield_types, vfield_order,
                    vfield_index, vdata_data);
     if ( vdata_status != 0 )
     {
        printf ("\nError in reading Vdata: \"%s\" ", vdata_name);
        exit(1);
     }

     fprintf (stdout,"\nVdata name: \"%s\" \n", vdata_name);
     fprintf (stdout,"\n Vdata field name: \"%s\" \n\n", vfield_name);
     for (k=0; k<7; k++)
     {
         printf ("k=%i\nsat_vel = %f   %f   %f\n",
                 k, sat_vel[k][0], sat_vel[k][1], sat_vel[k][2]);
     }







/* ===========================================================================
Example 4) Extract more than one Vdata field from the HDF file and print out
           data in ascii text.

  The Vdata fields "Earth-Sun Distance", "Satellite Velocity at record start",
  and "Longitude of Sun at Observation" in Vdata "Satellite-Celestial Data"
  will be extracted and written to a file. This example illustrates the use of
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
        HDF_File      - character pointer to the name of the HDF file.
        vdata_name    - character pointer to the name of the Vdata data set.
        rd_num_fields - integer defining number of fields in the Vdata to be
                        read.
        vfield_name   - character pointer to the list of names of the Vdata
                        fields separated by commas ','.
        vfield_types  - integer array containing the types of the fields within
                        the Vdata data set (see table above).
        vfield_order  - integer array containing the order of the fields within
                        the Vdata data set.
        vdata_n_rec   - integer number of records in the Vdata.
     Output Variable:
        vfield_index  - integer array containing the indicies of the fields
                        within the Vdata data set. (needed for 'print_vdata'
                        function)
        vdata_data    - void pointer

  'read_vdata'
     Function - Reads a Vdata data set from a HDF file.
     Input Variables:
        HDF_File      - character pointer to the name of the HDF file.
        vdata_name    - character pointer to the name of the Vdata data set.
        rd_num_fields - integer defining number of fields to read.
        rd_num_recs   - integer defining number of records to read.
        vfield_name   - character pointer to the list of names of the Vdata
                        fields separated by commas ','.
        vfield_types  - integer array containing the types of the fields within
                        the Vdata data set (see table above).
        vfield_order  - integer array containing the order of the fields within
                        the Vdata data set.
     Output Variable:
        vfield_index  - integer array containing the indicies of the fields
                        within the Vdata data set. (needed for 'print_vdata'
                        function)
        vdata_data    - void pointer to the address of the Vdata vfield data
                        set.
  'print_vdata'
     Function - Prints fields within a Vdata. Returns 0 when successful.
                Columns are the order of the Vdata field.
                Rows (lines) are the records of the Vdata field.
     Input Variables:
        vdata_name    - character pointer to the name of the Vdata data set.
        rd_num_fields - integer defining number of fields to read.
        rd_num_recs   - integer defining number of records to read.
        vfield_name   - character pointer to the list of names of the Vdata
                        fields separated by commas ','.
        vfield_types  - integer array containing the types of the fields within
                        the Vdata data set (see table above).
        vfield_order  - integer array containing the order of the fields within
                        the Vdata data set.
        vfield_index  - integer array containing the indicies of the fields
                        within the Vdata data set.
        vdata_data    - void pointer to the address of the Vdata vfield data
                        set.
============================================================================*/
 
     num_fields = 18;
     rd_num_fields = 3;
     rd_num_recs = 7;
  
     vdata_name = "Satellite-Celestial Data";
     vfield_name = "Earth-Sun Distance,Satellite Velocity at record start,Longitude of Sun at Observation";

     vfield_status = get_vfield_types(HDF_File, vdata_name,
                     num_fields, vfield_types, vfield_order);
     if ( vfield_status != 0 )
     {
        printf ("\nError in getting Vdata field types or orders");
        exit(1);
     }
     vdata_n_rec = get_vdata_n_rec(HDF_File, vdata_name);
     set_vdata_data(HDF_File, vdata_name, rd_num_fields, vfield_name,
                    vfield_types, vfield_order, vdata_n_rec,
                    vfield_index, vdata_data);

     vdata_status = read_vdata(HDF_File, vdata_name, rd_num_fields,
                    rd_num_recs, vfield_name, vfield_types, vfield_order,
                    vfield_index, vdata_data);
     if ( vdata_status != 0 )
     {
        printf ("\nError in reading Vdata: \"%s\" ", vdata_name);
        free_ptr_array(rd_num_fields, vdata_data);
        exit(1);
     }

     vprint_status = print_vdata(vdata_name, rd_num_fields, rd_num_recs,
                     vfield_name, vfield_types, vfield_order, vfield_index,
                     vdata_data);
     if ( vprint_status != 0 )
     {
        printf ("\nError in printing Vdata: \"%s\" ", vdata_name);
        free_ptr_array(rd_num_fields, vdata_data);
        exit(1);
     }

     free_ptr_array(rd_num_fields, vdata_data);

}
