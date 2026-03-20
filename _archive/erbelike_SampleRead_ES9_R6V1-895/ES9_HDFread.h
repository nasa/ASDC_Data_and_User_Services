 
#include <hdf.h>

#define MAX_SDS           450
#define MAX_VDATA          50
#define MAX_VDATA_FIELDS   50
#define MAX_CHAR_LENGTH   256
#define MAX_SDS_DIMS        8
#define MAX_FNAMES       4096

/* ===========================================================================
FUNCTION PROTOTYPING
============================================================================*/
void   list_sds (char *filename, int32 *field_quantity, char *sds_array[]);
void   list_vdata (char *filename, int32 *field_quantity, char *vdata_array[]);
char  *get_vdata_fields (char *filename, char *data_name,
                         int32 *field_quantity);
void   set_field_names(char *the_names, int32 count, int32 vid);
int32  get_vfield_types(char *filename, char *data_name, int vfield_count,
                        int32 *output_types, int32 *output_order);
long   get_vdata_n_rec(char *filename, char *data_name);
void   get_sds_dims_by_index(char *filename, int32 index, char *data_name, int32 *output_rank, int32 *output_dimensions, int32 *output_number_type);
void  *set_sds_data(int32 number_type, int32 rank, int32 *output_dimensions);
void   set_vdata_data(int32 field_count, int32 *number_types,
                      int32 *number_order, int32 number_of_records,
                      void *output_ptr[]);
int32  read_sds_by_index(char *filename, int32 index, int32 rank,
               int32 number_type, int32 *starting_record,
               int32 *increment, int32 *the_count, void *sds_data);
long read_vdata(char *filename, char *vdata_name, int field_count,
                int32 number_of_records, char *field_list, int32 *number_types,
                int32 *number_order, void *output_data[]);
void free_ptr_array(int32 n_elemts, void *ptr_array[]);

