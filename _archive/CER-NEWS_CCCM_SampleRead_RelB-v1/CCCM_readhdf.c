/*
 *****************************************************************************
 *  Program: CCCM_readhdf.c
 *  
 *  Purpose: To provide examples of how to read data from the CCCM HDF file.
 *  
 *  There are 6 examples of how to read data from the SDSs and Vdatas on the
 *  CCCM HDF file.
 *
 *  Example 1) Illustrates reading real data from an SDS of rank 1
 *             (1-dimensional) and storing the data in an array.
 *  Example 2) Illustrates reading integer data from an SDS of rank 2 
 *             (2-dimensional) and storing the data in an array.
 *  Example 3) Illustrates reading real data from an SDS of rank 3 and 
 *             writing the data to the standard output file.
 *  Example 4) Illustrates reading real data from a Vdata field and 
 *             storing the data in an array.
 *  Example 5) Illustrates reading string data from a Vdata field and 
 *             storing the data in an array. 
 *  Example 6) Illustrates reading integer data from a Vdata field and 
 *             writing the data to the standard output file.
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
 *   HDF_File - CCCM HDF file name - modify this if a different CCCM HDF file is
 *              being read
 *   sds_name - SDS name - see the Data Products Catalog ("DPC_SSF_CRS_C3M.pdf") or 
 *              the provided dump of the HDF file ("CER-NEWS_CCCM_Sample_RelA2.hdf.dump") 
 *              for valid SDS names in the CCCM product
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
   #include "CCCM_HDFread.h"

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
     int32 i,j,k,l;
     int32 index;
     long  vdata_n_rec;
     void *sds_data;
     void *vdata_data[MAX_VDATA_FIELDS];

/*
 *******************************************************************************
 *  Arrays assigned for Examples 1, 2, 4, and 5.
 *******************************************************************************

     int16 *  surface_types;
     float32 wavelength[20];
     char    platform[32];
     float32   wavelength[20];
*/
     float64 * velocity;
     int16   * surface_types;
     float32 wavelength[20];
     char    platform[32];


/*
 *******************************************************************************
 *  Assign the 5-footprint test HDF file to be read.  If another HDF file is to
 *  be read, enter its name here.
 *******************************************************************************

     HDF_File = "CER-NEWS_CCCM_SampleRead_RelA2.hdf"; 
*/
/* ===========================================================================
Assign the HDF file to be read
============================================================================*/

  if(argc != 2)
  {
     fprintf ( stderr, "Usage: cccm_rd HDFfilename\n");
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
 *  Example 1) Example of how to extract an SDS of rank 1 (1-dimensional) from 
 *             the HDF file and store it in memory.  The SDS, 
 *             "X component of satellite inertial velocity", will 
 *             be extracted and stored in the array "velocity".  
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
 *       velocity - output array of type 'sds_type' and rank 'sds_rank'
 *
 * Note: velocity has been declared as an array.
 *******************************************************************************
*/

     sds_name    = "X component of satellite inertial velocity";
     get_sds_dims(HDF_File,sds_name,&rank,sds_dims,&type);
     sds_rank = rank;
     sds_type = type;
/*
   Reading small numbers of footprints at a time is not advantageous.  In all
   examples, sds_edge is set to the number of values in each dimension, so all
   data is read.  To read in partial footprints of data, set sds_edge[0] to 
   the number of footprints to be read and change sds_strt[0] to the 
   number of the first footprint to be read.   
*/
     sds_edge[0] = sds_dims[0]; 
     sds_strt[0] = 0;
     sds_incr[0] = 1;
     velocity = (float64 *)malloc (sds_dims[0] * sizeof(float64));

/*
 *******************************************************************************
 *  Read the "X component of satellite inertial velocity" SDS into the 1-D 
 *  "velocity" array. 
 *******************************************************************************
*/
     sds_status = read_sds(HDF_File, sds_name, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, velocity);

     if ( sds_status != 0 )
     {
        fprintf (stderr, "\nError in reading SDS: \"%s\" ", sds_name);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "X component of satellite inertial velocity" SDS, 
 *  stored in the 1-D array, "velocity" 
*******************************************************************************
*/
     printf ("\n\n\n*********************************************************\n");
     printf ("*    Example 1:                                         *\n");
     printf ("*********************************************************");
     for (i=0; i<sds_dims[0]; i++)
     {
        printf ("\n\nFootprint %i\n",i+1);
        printf ("\nX component of satellite inertial velocity = %f", 
                   velocity[i]);
     }
/*
 *******************************************************************************
 *  Example 2) Example of how to extract an SDS of rank 2 ( 2-dimensional ) 
 *             from the HDF file and store it in memory.  The SDS, 
 *             "Number of imager pixels in CERES FOV", will be extracted and stored 
 *             in the 1-D array, "surface_types".  The differences between 
 *             Example 2 and Example 1 are:
 *                1.  The values sds_incr[1], sds_edge[1], and sds_strt[1] 
 *                    must be set for this 2-D SDS. 
 *                2.  The values for sds_name, sds_rank, and sds_type are now
 *                    set for the 2-D SDS, "Surface type percent coverage".
 *******************************************************************************
 * This example uses function 'read_sds' and 'get_sds_dims'.
 * 
 * See example 1 for information on these functions.
 *******************************************************************************
*/

     sds_name = "Number of imager pixels in CERES FOV";
     get_sds_dims(HDF_File,sds_name,&rank,sds_dims,&type);
     sds_rank = rank;
     sds_type = type;
     sds_edge[0] = sds_dims[0]; 
     sds_edge[1] = sds_dims[1];
     sds_strt[0] = 0;
     sds_strt[1] = 0;
     sds_incr[0] = 1;
     sds_incr[1] = 1;

/*
 *******************************************************************************
 *  Read the "Number of imager pixels in CERES FOV" SDS into the 1-D "surface_types" 
 *  array. 
 *******************************************************************************
*/
     surface_types = (int16 *)malloc ( sds_dims[0] * sds_dims[1] * sizeof(int16));
     sds_status = read_sds(HDF_File, sds_name, sds_rank, sds_type,
                           sds_strt, sds_incr, sds_edge, surface_types);
     if ( sds_status != 0 )
     {
        fprintf (stderr,"\nError in reading SDS: \"%s\" ", sds_name);
        exit(1);
     }

/*
 *******************************************************************************
 *  Print the values in the "Number of imager pixels in CERES FOV" SDS, stored in 
 *  the 2-D array, "surface_types".
 *******************************************************************************
*/
     printf ("\n\n\n*********************************************************\n");
     printf ("*    Example 2:                                         *\n");
     printf ("*********************************************************");
     for (i=0; i<sds_dims[0]; i++)
     {
        printf ("\n\nFootprint %i\n",i+1);
        for (j=0; j<sds_dims[1];j++)
        {
           k = i * sds_dims[1] + j;
           printf ("\nNumber of imager pixels in CERES FOV(%i) = %i",
                      j+1,surface_types[k]);

        }
     }
/*
 *******************************************************************************
 *  Example 3) Example of how to extract an SDS of rank 3 (3-dimensional) from 
 *             the HDF file using pointers and print the data as ascii text to 
 *             standard output.  Memory is allocated to hold the data being read 
 *             and is released after the data is printed to the output file.  
 *             The SDS, "Percentiles of visible optical depth for cloud layer",
 *             will be extracted and written to the output file.  To write the 
 *             data to a different file, add in a call to "fopen", similar to 
 *             the following, to open the output file:
 *                     outfile = fopen(filename, "w");
 *             Change "stdout" to "outfile" in the "fprintf" commands.
 *******************************************************************************
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
 *******************************************************************************
*/
     sds_name    = "Cloud layer top level height";
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
     for (i=0; i<sds_dims[0]; i++)
     {

        index = i*sds_dims[1]*sds_dims[2]; 
        fprintf (stdout,"\n\nFootprint %i\n",i+1);
        fprintf (stdout,"\nCloud layer top level height ");

        for (j=0; j<16; j++)

           { for (k=0; k<6; k++)
			   
		      { fprintf (stdout,"\nCloud Group %i Layer %i cloud top height = %f", j+1, k+1,
					  *((float32 *)sds_data+index+j*6+k)); }		   

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
 *  Example 4) This example extracts the Vdata field, "WAVELENGTH", of order 20,
 *             (ie., there are 20 values of wavelength) from the "CCCM_Header" 
 *             Vdata and stores it in the array, "wavelength".  To read in a 
 *             different Vdata field, change the name of the "vfield_name" (and 
 *             the name of the "vdata_name" if reading from a different Vdata). 
 *             The two valid Vdatas in the CCCM are "CCCM_Header" and 
 *             "CERES_metadata".  The variable "num_fields" must be set to 49 
 *             for the "CCCM_Header" Vdata and 14 for the "CERES_metadata" Vdata.
 *             An array must be defined to hold the values of the field and that
 *             array must be used in place of the "wavelength" array.
 *******************************************************************************
 * This example illustrates the use of functions 'get_vfield_types'
 * and 'read_vdata'.
 *
 * 'get_vfield_types'
 *    Function - Gets arrays of types and orders of the fields within a Vdata.
 *    Input Variables:
 *       HDF_File      - character pointer to the name of the HDF file.
 *        vdata_name    - character pointer to the name of the Vdata data set.
 *       num_fields    - integer defining number of fields in the Vdata.
 *    Output Variable:
 *       vfield_types  - integer array containing the types of the fields within
 *                       the Vdata data set (see table above).
 *       vfield_order  - integer array containing the order of the fields within
 *                       the Vdata data set.
 *
 *  'read_vdata'
 *     Function - Reads a Vdata data set from a HDF file.
 *    Input Variables:
 *       HDF_File      - character pointer to the name of the HDF file.
 *       vdata_name    - character pointer to the name of the Vdata data set.
 *       rd_num_fields - integer defining number of fields to read.
 *       rd_num_recs   - integer defining number of records to read.
 *       vfield_name   - character pointer to the name of the vfield data set.
 *       vfield_types  - integer array containing the types of the fields within
 *                       the Vdata data set (see table above).
 *       vfield_order  - integer array containing the order of the fields within
 *                       the Vdata data set.
 *    Output Variable:
 *       vdata_data    - void pointer (cast to the array 'wavelength' in this
 *                       example).
 *  
 * Note: wavelength was declared an array above.
 *******************************************************************************
*/
 
     num_fields    = 49;
     vdata_name    = "CCCM_Header";
     vfield_name   = "WAVELENGTH";
/*
 *******************************************************************************
 * Gets arrays of types and orders of the fields within a Vdata.
 *******************************************************************************
*/
     vfield_status = get_vfield_types(HDF_File, vdata_name,
                     num_fields, vfield_types, vfield_order);
     if ( vfield_status != 0 )
     {
        fprintf (stderr,"\nError in getting Vdata field types or orders\n");
        exit(1);
     }

     vdata_data[0]=wavelength;
/*
 *******************************************************************************
 * The Vdatas in the CCCM contain only one record each and in these examples, 
 * only one field is being read.  Therefore, rd_num_recs = 1 and 
 * rd_num_fields = 1. 
 *******************************************************************************
*/
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
 *  Print the 20 values in the "WAVELENGTH" vfield, stored in the "wavelength"
 *  array.
 *******************************************************************************
*/
     printf ("\n\n\n*********************************************************\n");
     printf ("*    Example 4:                                         *\n");
     printf ("*********************************************************\n");
     printf ("\nCentral wavelengths of imager channels\n");
     for (k=0; k<20; k++)
     {
        printf ("\n%i.\t%f",k+1,wavelength[k]);
     }

/*
 *******************************************************************************
 *  Example 5) This example extracts string data from a Vdata field in the
 *             "CERES_metadata" and prints it out in ascii text.  See Example 4
 *             on how to extract other Vdata fields from the "CERES_metadata"
 *             Vdata.
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
     printf ("*    Example 5:                                         *\n");
     printf ("*********************************************************\n");
     printf ("\nASSOCIATEDPLATFORMSHORTNAME = ");
     for(i=0; i<32; i++)
     {
       if(platform[i] == NULL)break;
         printf("%c",platform[i]);
     }
/*
 *******************************************************************************
 *  Example 6) This example extracts integer data from the Vdata field,
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
/*
 *******************************************************************************
 *  Print the value of the "NUMBEROFRECORDS" vfield, stored in the allocated
 *  array, vdata_data.
 *******************************************************************************
*/
     fprintf (stdout,
               "\n\n\n*********************************************************\n");
     fprintf (stdout,"*    Example 6:                                         *\n");
     fprintf (stdout,"*********************************************************\n");
     fprintf (stdout,"\nNUMBEROFRECORDS = %i\n",
              *((int32 *)vdata_data[0]));
/*
 *******************************************************************************
 * Free the space allocated to the vdata_data array. 
 *******************************************************************************
*/
free_ptr_array(num_fields, vdata_data);
}
