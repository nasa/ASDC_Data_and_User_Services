 
#include <hdf.h>

#define MAX_SDS           150
#define MAX_VDATA         150
#define MAX_VDATA_FIELDS  150
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
int32  get_vfield_types(char *filename, char *data_name, int vfield_count,
                        int32 *output_types, int32 *output_order);
int32  get_vdata_n_rec(char *filename, char *data_name);
void   get_sds_dims(char *filename, char *data_name, int32 *output_rank,
                    int32 *output_dimensions, int32 *output_number_type);
void  *set_sds_data(int32 number_type, int32 rank, int32 *output_dimensions);
void  set_vdata_data(char *filename, char *vdata_data, int32 field_count,
                     char *field_list, int32 *number_types, int32 *number_order,
                     int32 number_of_records, int32 *number_index,
                     void *output_ptr[]);
int32  read_sds(char *filename, char *data_name, int32 rank,
               int32 number_type, int32 *starting_record,
               int32 *increment, int32 *the_count, void *sds_data);
int32  read_vdata(char *filename, char *vdata_name, int field_count,
                int32 number_of_records, char *field_list, int32 *number_types,
                int32 *number_order, int32 *number_index, void *output_data[]);
int32  print_vdata(char *vdata_name, int32 field_count, int32 number_of_records,
                   char *field_list, int32 *number_types, int32 *number_order,
                   int32 *number_index, void *output_data[]);
int32  print_sds(char *data_name, int32 rank, int32 number_type,
                 int32 *starting_record, int32 *increment, int32 *the_count,
                 void *sds_data);
int32  print_value(int32 number_type, int32 index, void *ptr);
void free_ptr_array(int32 n_elemts, void *ptr_array[]);
