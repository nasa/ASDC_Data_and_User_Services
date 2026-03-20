#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "HDFread.h"

/*
---------------------------------------------------------------
	list_sds
---------------------------------------------------------------
*/
void	list_sds(char *filename, int32 *field_quantity, char *sds_array[])
{

	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
	int32		dimsizes[MAX_SDS_DIMS];
   	int		i;
	int32		nattrs;
   	int32		ndatasets;
   	int32		nglobal_attr;
	int32		number_type;
	int32		rank;
	int32		sd_id;
	int32		sds_id;
	char		sds_name[MAX_CHAR_LENGTH];
   	int32		status;

	/*
   	----------------------------------------
		Process the Data	
   	----------------------------------------
   	*/

	/* First Look at the SDS */
	sd_id=SDstart(filename,DFACC_RDONLY);
	status=SDfileinfo(sd_id,&ndatasets,&nglobal_attr);

	*field_quantity=ndatasets;
	for (i=0; i<ndatasets; i++)
	{
		sds_id=SDselect(sd_id,i);
		status=SDgetinfo(sds_id,sds_name,&rank,dimsizes,&number_type,
                                 &nattrs);

                        strcpy(sds_array[i],sds_name);

                /*
		fprintf(stdout,"Name=%s\n",sds_name);
		fprintf(stdout,"Rank=%i\n",rank);
		for (j=0; j<rank; j++)
		{
			fprintf(stdout,"Dimsizes[%i]=%i\n",j,dimsizes[j]);
		}
		fprintf(stdout,"Number Type=%i\n",number_type);
		fprintf(stdout,"Attrs=%i\n",nattrs);
                */
	
	}
	status=SDend(sd_id);
}
/*
---------------------------------------------------------------
	list_vdata
---------------------------------------------------------------
*/
void	list_vdata(char *filename, int32 *field_quantity, char *vdata_array[])
{

	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
	int		continue_flag;
   	int32		current_vid;
   	int32		current_vref;
   	int32        	file_id;
   	int32		number_of_vdatas;
   	int32		previous_vref;
	char		vdata_name[MAX_CHAR_LENGTH];
	char		vdata_class[MAX_CHAR_LENGTH];

	/*
   	----------------------------------------
		Process the Data	
   	----------------------------------------
   	*/


   	file_id=Hopen(filename,DFACC_RDONLY,0);
	if (file_id!=-1)
	{
		Vstart(file_id);
		continue_flag=1;
		previous_vref=-1;
		number_of_vdatas=0;
		while (continue_flag) 
		{
			current_vref=VSgetid(file_id,previous_vref);
			if (current_vref!=-1)
			{
		   		/* Loop through the vdatas in this file */

		   		current_vid=VSattach(file_id,current_vref,"r");
		   		if (current_vid!=-1)
		   		{
					previous_vref=current_vref;
                                        if( VSisattr(current_vid)==1 )
                                        {
					   VSdetach(current_vid);
                                           continue;
                                        }

					VSgetname(current_vid,vdata_name);
/*    fprintf(stdout,"current_vid=%i   vdata name=%s\n",current_vid,vdata_name);   */

					/* Omit the fakennn vdata sets */
					if (strncmp(vdata_name,"fake",4)!=0)
					{
                                           VSgetclass(current_vid,vdata_class);
 /*     fprintf(stdout,"current_vid=%i   vdata class=%s\n",current_vid,vdata_class);  */
			     if (strncmp(vdata_class,"DimVal",6)!=0)
                                           {
                                                strcpy(vdata_array[number_of_vdatas],vdata_name);
						number_of_vdatas++;
                                           }
					}
					VSdetach(current_vid);
		   		}
                   		else
                   		{
                       			fprintf(stdout,
                                           "\tCurrent Vid is bad!\n");
					continue_flag=0;
                   		}
                	}
                	else
                	{
                            /* No more Vdatas in the file */
                       	    /* fprintf(stdout,"\tCurrent Vref is bad!\n"); */
				continue_flag=0;
                	}
		}	/* End while */
		Vend(file_id);
   		Hclose(file_id);
        }
        else
        {
		/* Bad filename */
                fprintf(stdout,"\tBad Filename in list_vdata\n");
        }
	*field_quantity=number_of_vdatas;

}
/*
---------------------------------------------------------------
	get_vdata_fields
---------------------------------------------------------------
*/
char	*get_vdata_fields(char *filename, char *data_name,
                          int32 *field_quantity)
{

	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
   	int32		current_vid;
   	int32		current_vref;
   	char		field_names[MAX_FNAMES];
   	int32        	file_id;
   	int32		number_of_fields;
   	char		result[MAX_CHAR_LENGTH];
   	char		*the_fields[MAX_VDATA_FIELDS];
	char		*the_result;

	/*
   	----------------------------------------
		Process the Data	
   	----------------------------------------
   	*/
   	file_id=Hopen(filename,DFACC_RDONLY,0);
	/* fprintf(stdout,"\tThe file id is: \t%i\n"); */

	if (file_id!=-1)
	{
		Vstart(file_id);

		current_vref=VSfind(file_id, data_name);
		if (current_vref)
		{
		   current_vid=VSattach(file_id,current_vref,"r");
		   if (current_vid!=-1)
		   {
			number_of_fields=VSgetfields(current_vid, 
					field_names);
			*field_quantity=number_of_fields;
                   /*   fprintf(stdout,
                           "\tNumber of Fields = \t%i\n",number_of_fields);  */
		        /*         
			for (i=0; i<number_of_fields; i++) 
			{
				the_fields[i]=VFfieldname(current_vid,i);
				fprintf(stdout,"\tFields[%i]=%s\n",i,
                                        the_fields[i]);
			}
                        */		  

			VSdetach(current_vid);
			Vend(file_id);
   			Hclose(file_id);
	                the_result=malloc(strlen(field_names)+1);
			strcpy(the_result, field_names);
		   }
                   else
                   {
                        fprintf(stdout,"\tCurrent Vid is bad!\n");
                        Vend(file_id);
                        Hclose(file_id);
			strcpy(result,"BAD");
			the_result = result;
                   }
                }
                else
                {
                        fprintf(stdout,"\tCurrent Vref is bad!\n");
                        Vend(file_id);
                        Hclose(file_id);
			strcpy(result,"BAD");
			the_result = result;
                }
        }
        else
        {
		strcpy(result,"BAD");
		the_result = result;
        }

   return(the_result);
}

/*
---------------------------------------------------------------
	get_sds_dims
---------------------------------------------------------------
*/
void     get_sds_dims(char *filename, char *data_name, int32 *output_rank,
                      int32 *output_dimensions, int32 *output_number_type)
{

	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
	int32		dimsizes[MAX_SDS_DIMS];
   	int		i;
	int32		index;
	int32		nattrs;
	int32		number_type;
	int32		rank;
	int32		sd_id;
	int32		sds_id;
	char		sds_name[MAX_CHAR_LENGTH];
   	int32		status;

	/*
   	----------------------------------------
		Start of Code	
   	----------------------------------------
   	*/


	/* First Look at the SDS */
	sd_id=SDstart(filename,DFACC_RDONLY);
	index=SDnametoindex(sd_id,data_name);
	sds_id=SDselect(sd_id,index);
	status=SDgetinfo(sds_id,sds_name,&rank,dimsizes,&number_type,&nattrs);
        *output_rank=rank;
        *output_number_type=number_type;
	for (i=0; i<rank; i++)
	{
		output_dimensions[i]=dimsizes[i];
	}
	status=SDend(sd_id);

   return;
}
/*
---------------------------------------------------------------
	read_sds
---------------------------------------------------------------
*/
int32	read_sds(char *filename, char *data_name, int32 rank,
                 int32 number_type, int32 *start, int32 *stride,
                 int32 *edge, void *sds_data)
                 
{

	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
	int32		index;
   	int32		result;
	int32		sd_id;
	int32		sds_id;
   	int32		status;

	/*
   	----------------------------------------
		Start of Code	
   	----------------------------------------
   	*/

	/* First Look at the SDS */
	sd_id=SDstart(filename,DFACC_RDONLY);
	if(sd_id == -1)
	{
	   fprintf(stderr,"Unable to open HDF file for SDS.\n");
	   result = -1;
	   return result;
	}

	index=SDnametoindex(sd_id,data_name);
	if(index == -1)
	{
	   fprintf(stderr,"Could not find %s SDS\n", data_name);
	   status = SDend(sd_id);
	   result = -1;
	   return result;
	}

	sds_id=SDselect(sd_id,index);
	if(sds_id == -1)
	{
	   fprintf(stderr,"Could not get sds_id for %s\n", data_name);
	   status = SDend(sd_id);
	   result = -1;
	   return result;
	}

        status = SDreaddata(sds_id, start, stride, edge,
                            (void *) sds_data);
	if(status == -1)
        {
	   fprintf(stderr, "ERROR for reading %s SDS.\n", data_name);
	   result = -1;
        }
        else
        {
           result = 0;
        }
	status = SDendaccess(sds_id);
	status = SDend(sd_id);

   return(result);
}
/*
---------------------------------------------------------------
	set_sds_data
---------------------------------------------------------------
*/

void  *set_sds_data(int32 number_type, int32 rank, int32 *dimensions)
{

	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
   	int		i;
	int		data_size;
	int		stor_size;
        void           *ptr;

	/*
   	----------------------------------------
		Start of Code	
   	----------------------------------------
   	*/

     if ( rank == 1 ) data_size = dimensions[0];
     else
     {
        data_size = dimensions[1];
        for (i=2; i<rank; i++) data_size *= dimensions[i];
     }

     switch(number_type)
     {
        case DFNT_UCHAR8:
               {
	          stor_size = data_size * sizeof(uchar8);
                  if ( rank == 1 ) ptr = (uchar8 *) malloc(stor_size);
                  else ptr = (uchar8 *) calloc(dimensions[0],stor_size);
		  break;
               }
        case DFNT_CHAR8:
               {
                  stor_size = data_size * sizeof(char8);
                  if ( rank == 1 ) ptr = (char8 *) malloc(stor_size);
                  else ptr = (char8 *) calloc(dimensions[0],stor_size);
		  break;
               }
        case DFNT_FLOAT32:
               {
                  stor_size = data_size * sizeof(float32);
                  if ( rank == 1 ) ptr = (float32 *) malloc(stor_size);
                  else ptr = (float32 *) calloc(dimensions[0],stor_size);
		  break;
               }
	case DFNT_FLOAT64:
               {
                  stor_size = data_size * sizeof(float64);
                  if ( rank == 1 ) ptr = (float64 *) malloc(stor_size);
                  else ptr = (float64 *) calloc(dimensions[0],stor_size);
		  break;
               }
	case DFNT_INT8:
               {
                  stor_size = data_size * sizeof(int8);
                  if ( rank == 1 ) ptr = (int8 *) malloc(stor_size);
                  else ptr = (int8 *) calloc(dimensions[0],stor_size);
		  break;
               }
	case DFNT_UINT8:
               {
                  stor_size = data_size * sizeof(uint8);
                  if ( rank == 1 ) ptr = (uint8 *) malloc(stor_size);
                  else ptr = (uint8 *) calloc(dimensions[0],stor_size);
		  break;
               }
	case DFNT_INT16:
               {
                  stor_size = data_size * sizeof(int16);
                  if ( rank == 1 ) ptr = (int16 *) malloc(stor_size);
                  else ptr = (int16 *) calloc(dimensions[0],stor_size);
		  break;
               }
	case DFNT_UINT16:
               {
                  stor_size = data_size * sizeof(uint16);
                  if ( rank == 1 ) ptr = (uint16 *) malloc(stor_size);
                  else ptr = (uint16 *) calloc(dimensions[0],stor_size);
		  break;
               }
	case DFNT_INT32:
               {
                  stor_size = data_size * sizeof(int32);
                  if ( rank == 1 ) ptr = (int32 *) malloc(stor_size);
                  else ptr = (int32 *) calloc(dimensions[0],stor_size);
		  break;
               }
	case DFNT_UINT32:
	       {
                  stor_size = data_size * sizeof(uint32);
                  if ( rank == 1 ) ptr = (uint32 *) malloc(stor_size);
                  else ptr = (uint32 *) calloc(dimensions[0],stor_size);
		  break;
	       }
     /*
        case DFNT_INT64:
	       {
                  stor_size = data_size * sizeof(int64);
                  if ( rank == 1 ) ptr = (int64 *) malloc(stor_size);
                  else ptr = (int64 *) calloc(dimensions[0],stor_size);
		  break;
	       }
	case DFNT_UINT64:
	       {
                  stor_size = data_size * sizeof(uint64);
                  if ( rank == 1 ) ptr = (uint64 *) malloc(stor_size);
                  else ptr = (uint64 *) calloc(dimensions[0],stor_size);
		  break;
	       }
     */
	default:
	       {
		  fprintf(stderr, "Unknown type for SDS: %d.\n", number_type);
                  ptr = NULL;
	       }
     }
     return ptr;
}

/*
---------------------------------------------------------------
	print_sds
---------------------------------------------------------------
*/

int32  print_sds(char *data_name, int32 rank, int32 number_type, int32 *start,
                 int32 *stride, int32 *edge, void *sds_data)
{

	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
   	int32	i1, i2, i3, i4, i5, i6, i7, i8;
	int32 	status;
	int32   index;
	int32   item_count;
	int32   max_item=5;

	/*
   	----------------------------------------
		Start of Code	
   	----------------------------------------
   	*/

     fprintf(stdout,"\nSDS data set: \"%s\"\n\n", data_name);
     status = 0;
     switch(rank)
     {
        case  1:
        {
           item_count = 1;
   	   for (i1 = start[0]; i1 < edge[0]; i1+=stride[0])
	   {
	      index = i1;
	      status = print_value(number_type, index, sds_data);
              if (item_count++ == max_item)
              {
                 fprintf (stdout,"\n");
                 item_count = 1;
              }
	      if(status != 0)
	      {
	         printf("\n***Error printing SDS - %s.***\n\n", data_name);
		 return status;
	      }
	   }
           fprintf (stdout,"\n");
	   break;
        }
        case  2:
        {
   	   for (i1 = start[0]; i1 < edge[0]; i1+=stride[0])
           {
              item_count = 1;
   	      for (i2 = start[1]; i2 < edge[1]; i2+=stride[1])
	      {
	         index = i1*edge[1] + i2;
	         status = print_value(number_type, index, sds_data);
                 if (item_count++ == max_item)
                 {
                    fprintf (stdout,"\n");
                    item_count = 1;
                 }
	         if(status != 0)
	         {
	            printf("\n***Error printing SDS - %s.***\n\n", data_name);
		    return status;
	         }
	      }
              fprintf (stdout,"\n");
           } 
           fprintf (stdout,"\n");
	   break;
        }
        case  3:
        {
   	   for (i1 = start[0]; i1 < edge[0]; i1+=stride[0])
           {
   	      for (i2 = start[1]; i2 < edge[1]; i2+=stride[1])
	      {
                 item_count = 1;
   	         for (i3 = start[2]; i3 < edge[2]; i3+=stride[2])
		 {
		    index = i1*edge[1]*edge[2] + i2*edge[2] + i3;
	            status = print_value(number_type, index, sds_data);
                    if (item_count++ == max_item)
                    {
                       fprintf (stdout,"\n");
                       item_count = 1;
                    }
	            if(status != 0)
	            {
	               printf("\n***Error printing SDS - %s.***\n\n", data_name);
		       return status;
	            }
		 }
                 fprintf (stdout,"\n");
	      }
              fprintf (stdout,"\n");
           }
           fprintf (stdout,"\n");
	   break;
        }
	case  4:
        {
   	   for (i1 = start[0]; i1 < edge[0]; i1+=stride[0])
           {
   	      for (i2 = start[1]; i2 < edge[1]; i2+=stride[1])
	      {
   	         for (i3 = start[2]; i3 < edge[2]; i3+=stride[2])
		 {
                    item_count = 1;
   	   	    for (i4 = start[3]; i4 < edge[3]; i4+=stride[3])
		    {
		       index = i1*edge[1]*edge[2]*edge[3] + \
			       i2*edge[2]*edge[3] + i3*edge[3] + i4;
	               status = print_value(number_type, index, sds_data);
                       if (item_count++ == max_item)
                       {
                          fprintf (stdout,"\n");
                          item_count = 1;
                       }
	               if(status != 0)
	               {
	                  printf("\n***Error printing SDS - %s.***\n\n", data_name);
		          return status;
	               }
		    }
                    fprintf (stdout,"\n");
		 }
                 fprintf (stdout,"\n");
	      }
              fprintf (stdout,"\n");
           }
           fprintf (stdout,"\n");
	   break;
        }
	case 5:
        {
   	   for (i1 = start[0]; i1 < edge[0]; i1+=stride[0])
           {
   	      for (i2 = start[1]; i2 < edge[1]; i2+=stride[1])
              {
   	         for (i3 = start[2]; i3 < edge[2]; i3+=stride[2])
		 {
   	            for (i4 = start[3]; i4 < edge[3]; i4+=stride[3])
		    {
                       item_count = 1;
		       for(i5 = start[4]; i5 < edge[4]; i5+=stride[4])
		       {
		          index = i1*edge[1]*edge[2]*edge[3]*edge[4] + \
			    i2*edge[2]*edge[3]*edge[4] + \
			    i3*edge[3]*edge[4] + i4*edge[4] + i5;
	                    status = print_value(number_type, index, sds_data);
                          if (item_count++ == max_item)
                          {
                             fprintf (stdout,"\n");
                             item_count = 1;
                          }
	                    if(status != 0)
	                    {
	                       printf("\n***Error printing SDS - %s.***\n\n", data_name);
		               return status;
	                    }
		       }
                       fprintf (stdout,"\n");
		    }
                    fprintf (stdout,"\n");
		 }
                 fprintf (stdout,"\n");
	      }
              fprintf (stdout,"\n");
	   }
           fprintf (stdout,"\n");
	   break;
        }
	case 6:
        {
   	   for (i1 = start[0]; i1 < edge[0]; i1+=stride[0])
           {
   	      for (i2 = start[1]; i2 < edge[1]; i2+=stride[1])
              {
   	         for (i3 = start[2]; i3 < edge[2]; i3+=stride[2])
		 {
   	            for (i4 = start[3]; i4 < edge[3]; i4+=stride[3])
		    {
		       for(i5 = start[4]; i5 < edge[4]; i5+=stride[4])
		       {
                          item_count = 1;
		          for(i6 = start[5]; i6 < edge[5]; i6+=stride[5])
		          {
		            index = i1*edge[1]*edge[2]*edge[3]*edge[4]*edge[5] + \
			    i2*edge[2]*edge[3]*edge[4]*edge[5] + \
			    i3*edge[3]*edge[4]*edge[5] + i4*edge[4]*edge[5] + \
			    i5*edge[5] + i6;
	                    status = print_value(number_type, index, sds_data);
                             if (item_count++ == max_item)
                             {
                                fprintf (stdout,"\n");
                                item_count = 1;
                             }
	                    if(status != 0)
	                    {
	                       printf("\n***Error printing SDS - %s.***\n\n", data_name);
		               return status;
	                    }
	                 }
                         fprintf (stdout,"\n");
	              }
                      fprintf (stdout,"\n");
	           }
                   fprintf (stdout,"\n");
	        }
                fprintf (stdout,"\n");
	     }
             fprintf (stdout,"\n");
	  }
          fprintf (stdout,"\n");
	  break;
        }
	case 7:
        {
   	   for (i1 = start[0]; i1 < edge[0]; i1+=stride[0])
           {
   	      for (i2 = start[1]; i2 < edge[1]; i2+=stride[1])
              {
   	         for (i3 = start[2]; i3 < edge[2]; i3+=stride[2])
		 {
   	            for (i4 = start[3]; i4 < edge[3]; i4+=stride[3])
		    {
		       for(i5 = start[4]; i5 < edge[4]; i5+=stride[4])
		       {
		          for(i6 = start[5]; i6 < edge[5]; i6+=stride[5])
		          {
                             item_count = 1;
		             for(i7 = start[6]; i7 < edge[6]; i7+=stride[6])
		             {
		              index = i1*edge[1]*edge[2]*edge[3]*edge[4]*edge[5]*edge[6] + \
			      i2*edge[2]*edge[3]*edge[4]*edge[5]*edge[6] + \
			      i3*edge[3]*edge[4]*edge[5]*edge[6] + i4*edge[4]*edge[5]*edge[6] + \
			      i5*edge[5]*edge[6] + i6*edge[6] + i7;
	                      status = print_value(number_type, index, sds_data);
                                if (item_count++ == max_item)
                                {
                                   fprintf (stdout,"\n");
                                   item_count = 1;
                                }
	                      if(status != 0)
	                      {
	                         printf("\n***Error printing SDS - %s.***\n\n", data_name);
		                 return status;
	                      }
	                   }
                           fprintf (stdout,"\n");
	                }
                        fprintf (stdout,"\n");
	             }
                     fprintf (stdout,"\n");
	          }
                  fprintf (stdout,"\n");
	       }
               fprintf (stdout,"\n");
	    }
            fprintf (stdout,"\n");
	 }
         fprintf (stdout,"\n");
	 break;
        }
	case 8:
        {
   	   for (i1 = start[0]; i1 < edge[0]; i1+=stride[0])
           {
   	      for (i2 = start[1]; i2 < edge[1]; i2+=stride[1])
              {
   	         for (i3 = start[2]; i3 < edge[2]; i3+=stride[2])
		 {
   	            for (i4 = start[3]; i4 < edge[3]; i4+=stride[3])
		    {
		       for(i5 = start[4]; i5 < edge[4]; i5+=stride[4])
		       {
		          for(i6 = start[5]; i6 < edge[5]; i6+=stride[5])
		          {
		             for(i7 = start[6]; i7 < edge[6]; i7+=stride[6])
		             {
                                item_count = 1;
		                for(i8 = start[7]; i8 < edge[7]; i8+=stride[7])
		                {
		                index=i1*edge[1]*edge[2]*edge[3]*edge[4]*edge[5]*edge[6]*edge[7] + \
			        i2*edge[2]*edge[3]*edge[4]*edge[5]*edge[6]*edge[7] + \
			        i3*edge[3]*edge[4]*edge[5]*edge[6]*edge[7] + \
				i4*edge[4]*edge[5]*edge[6]*edge[7] + \
			        i5*edge[5]*edge[6]*edge[7] + i6*edge[6]*edge[7] + \
				i7*edge[7] + i8;
	                        status = print_value(number_type, index, sds_data);
                                   if (item_count++ == max_item)
                                   {
                                      fprintf (stdout,"\n");
                                      item_count = 1;
                                   }
	                        if(status != 0)
	                        {
	                           printf("\n***Error printing SDS - %s.***\n\n", data_name);
		                   return status;
	                        }
	                     }
                             fprintf (stdout,"\n");
	                  }
                          fprintf (stdout,"\n");
	               }
                       fprintf (stdout,"\n");
	            }
                    fprintf (stdout,"\n");
	         }
                 fprintf (stdout,"\n");
	      }
              fprintf (stdout,"\n");
	   }
           fprintf (stdout,"\n");
	}
        fprintf (stdout,"\n");
	break;
        }
	default:
	       {
		  fprintf(stderr, "\n***Rank %d not supported.***\n\n", rank);
                  status = -1;
	       }
     }
     return status;
}

/*
---------------------------------------------------------------
	print_value
---------------------------------------------------------------
*/

int32  print_value(int32 number_type, int32 index, void *ptr)
{

	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
   	int32		status;

	/*
   	----------------------------------------
		Start of Code	
   	----------------------------------------
   	*/

     status = 0;
     switch(number_type)
     {
        case DFNT_UCHAR8:
               {
		  if ( *((uchar8 *)ptr+index) == NULL ) break;
                  fprintf (stdout,"%c",*((uchar8 *)ptr+index));
		  break;
               }
        case DFNT_CHAR8:
               {
		  if ( *((char8 *)ptr+index) == NULL ) break;
                  fprintf (stdout,"%c",*((char8 *)ptr+index));
		  break;
               }
        case DFNT_FLOAT32:
               {
                  fprintf (stdout," %f",*((float32 *)ptr+index));
		  break;
               }
	case DFNT_FLOAT64:
               {
                  fprintf (stdout," %f",*((float64 *)ptr+index));
		  break;
               }
	case DFNT_INT8:
               {
                  fprintf (stdout," %d",*((int8 *)ptr+index));
		  break;
               }
	case DFNT_UINT8:
               {
                  fprintf (stdout," %u",*((uint8 *)ptr+index));
		  break;
               }
	case DFNT_INT16:
               {
                  fprintf (stdout," %d",*((int16 *)ptr+index));
		  break;
               }
	case DFNT_UINT16:
               {
                  fprintf (stdout," %u",*((uint16 *)ptr+index));
		  break;
               }
	case DFNT_INT32:
               {
                  fprintf (stdout," %d",*((int32 *)ptr+index));
		  break;
               }
	case DFNT_UINT32:
	       {
                  fprintf (stdout," %u",*((uint32 *)ptr+index));
		  break;
	       }
     /*
        case DFNT_INT64:
	       {
                  fprintf (stdout," %d",*((int64 *)ptr+index));
		  break;
	       }
	case DFNT_UINT64:
	       {
                  fprintf (stdout," %u",*((uint64 *)ptr+index));
		  break;
	       }
     */
	default:
	       {
		  fprintf(stderr, "Unknown type for SDS: %d.\n", number_type);
                  status = -1;
	       }
     }
     return status;
}

/*
---------------------------------------------------------------
	get_vfield_types
---------------------------------------------------------------
*/

int32 get_vfield_types(char *filename, char *data_name, int vfield_count,
                       int32 *output_types, int32 *output_order)
{
	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
   	int32		current_vfield_index;
   	int32		current_vid;
   	int32		current_vref;
   	int32        	file_id;
	int		i;
   	int32		result;
   	int32		status;

	/*
   	----------------------------------------
		Start of Code	
   	----------------------------------------
	*/

        file_id=Hopen(filename,DFACC_RDONLY,0);
        if (file_id!=-1)
        {
                Vstart(file_id);
                current_vref=VSfind(file_id, data_name);
                if (current_vref)
                {
                   current_vid=VSattach(file_id,current_vref,"r");
                   if (current_vid!=-1)
                   {
			for (i=0; i<vfield_count; i++)
			{
				/* Convert name to field_index */
				current_vfield_index=i;
				status=VFfieldtype(current_vid,
                                                   current_vfield_index);
				if (status!=-1)
				{
					output_types[i]=status;
/*			printf("The field type is %d\n",
                                               output_types[i]);    */
					result=0;
				}
				else
				{
					fprintf(stdout,
                                                "\tVFfieldtype is bad!\n");
					result=-1;
					break;
				}
				status=VFfieldorder(current_vid,
                                                    current_vfield_index);
				if (status!=-1)
				{
					output_order[i]=status;
					result=0;
				}
				else
				{
					fprintf(stdout,
                                                "\tVFfieldorder is bad!\n");
					result=-1;
					break;
				}
			}
			VSdetach(current_vid);
			Vend(file_id);
			Hclose(file_id);
		   }
		   else
		   {
			fprintf(stdout,"\tCurrent Vid is bad!\n");
			Vend(file_id);
   			Hclose(file_id);
			result=-1;
	   }		
		}
		else
		{
			fprintf(stdout,"\tCurrent Vref is bad!\n");
			Vend(file_id);
   			Hclose(file_id);
			result=-1;
		}
	}
	else
	{
		result=-1;
	}
	return(result);
}

/*
---------------------------------------------------------------
	get_vdata_n_rec
---------------------------------------------------------------
*/

int32 get_vdata_n_rec(char *filename, char *data_name)
{
	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
   	int32		current_vid;
   	int32		current_vref;
   	int32        	file_id;
	int32		n_records;
   	int32		result;
   	int32		status;

	/*
   	----------------------------------------
		Start of Code	
   	----------------------------------------
	*/

        file_id=Hopen(filename,DFACC_RDONLY,0);
        if (file_id!=-1)
        {
                Vstart(file_id);
                current_vref=VSfind(file_id, data_name);
                if (current_vref)
                {
                   current_vid=VSattach(file_id,current_vref,"r");
                   if (current_vid!=-1)
                   {

			status=VSinquire(current_vid,&n_records,
				NULL,NULL,NULL,NULL);
			if (status!=-1)
			{
				VSdetach(current_vid);
				Vend(file_id);
				Hclose(file_id);
				result=n_records;
			}
			else
			{
				VSdetach(current_vid);
				Vend(file_id);
				Hclose(file_id);
				result=-1;
			}
		   }
		   else
		   {
			fprintf(stdout,"\tCurrent Vid is bad!\n");
			Vend(file_id);
   			Hclose(file_id);
			result=-1;
		   }		
		}
		else
		{
			fprintf(stdout,"\tCurrent Vref is bad!\n");
			Vend(file_id);
   			Hclose(file_id);
			result=-1;
		}
	}
	else
	{
		result=-1;
	}
	return(result);
}

/*
---------------------------------------------------------------
	read_vdata
---------------------------------------------------------------
*/
int32 read_vdata(char *filename, char *vdata_name, int field_count,
                int32 number_of_records, char *field_list, int32 *number_types,
                int32 *number_order, int32 *number_index, void *output_data[])
                
{

	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
   	uint8   	*buffer;
   	uint8   	*buffer_ptr;
   	int32		buffer_size;
	int32		data_size;
	int32		current_order;
   	int32		current_vid;
   	int32		current_vref;
   	int32        	file_id;

	int32		size_of_single_record;
	char8		*tempC8;
	uchar8		*tempUC8;
	int8            *tempI8;
	uint8		*tempU8;
	int16           *tempI16;
	uint16		*tempU16;
	int32           *tempI32;
	uint32		*tempU32;
   	float32		*tempF32;
   	float64		*tempF64;
	char		*ptr_field;
        char		*ptr_start;
	char		*ptr_temp;
        char		temp_field[MAX_CHAR_LENGTH];
   	int32		i,k;
   	int32		result;
   	int32		status;
	int32		this_index;

	/*
   	----------------------------------------
		read_vdata Processing
   	----------------------------------------
   	*/
/*
	printf("vdata_name = %s.\n", vdata_name);
	printf("field_list = %s.\n", field_list);
        printf("field_count = %d.\n", field_count);
*/
   	file_id=Hopen(filename,DFACC_RDONLY,0);
	if (file_id!=-1)
	{
		Vstart(file_id);

		current_vref=VSfind(file_id, vdata_name);
		if (current_vref)
		{
		   current_vid=VSattach(file_id,current_vref,"r");
		   if (current_vid!=-1)
		   {
			if (field_count!=-1)
			{
			   status=VSseek(current_vid,0);
			   if (status==-1)
	  		   {
			   	fprintf(stdout,
                                  "\tVSseek failed - No Data in Vdata %s \n",
                                  vdata_name);
				return(-1);
			   }
			   status=VSsetfields(current_vid, field_list);
			   if (status!=-1)
			   {
				/* get the size_of_single_record from the HDF vsize field INFO */
				size_of_single_record=VSsizeof(current_vid,field_list);
				buffer_size=number_of_records * size_of_single_record;
				if ((buffer_size>0) && (size_of_single_record>0))
				{
				      /* Added check on malloc return.  Cast result to HDF type.   */
				      if ( (buffer=(uint8 *) malloc(buffer_size)) == NULL)
				      {
					   printf("OUT OF MEMORY FOR VDATA BUFFER, %d BYTES\n",buffer_size);
					   return(-1);
				      }
				      /* Cast pointer to buffer to HDF type.  The vdata must have FULL_INTERLACE to be */
				      /* read field by field, so it is hard-coded here.   */
				      status=VSread(current_vid,(uint8 *) buffer,
						number_of_records,FULL_INTERLACE); 
				      if (status!=-1)
				      {
					ptr_temp = malloc(strlen(field_list)+1);
					if(ptr_temp == NULL)
					{
					   fprintf(stdout, "ERROR -- Unable to allocate memory for ptr_temp");
					   return(-1);
					}
					strcpy(ptr_temp, field_list);
					ptr_start = ptr_temp;
					   for (k=0; k<field_count; k++)	
					   {	
					      ptr_field = strchr(ptr_start, ',');
					      if(ptr_field != NULL) *ptr_field = '\0';
					      strcpy(temp_field, ptr_start);
					      ptr_start = ptr_field + 1;
					      status=VSfindex(current_vid,temp_field,&this_index);
					      if(status == -1)
					      {
					         fprintf(stdout, "ERROR -- Cannot find the index for %s\n", temp_field);
					         free(ptr_temp);
					         return(-1);
					      }
					      number_index[k] = this_index; 
					   }
					   free(ptr_temp);
					   buffer_ptr=buffer;
					   for (i=0; i<number_of_records; i++)
					   {
					        for (k=0; k<field_count; k++)	
					 	{	
						   this_index = number_index[k];
					           current_order=number_order[this_index];
						      switch (number_types[this_index])
						      {
							case DFNT_CHAR8:
								data_size = current_order * sizeof(char8);
								tempC8 = (char8 *)output_data[k] +  i * current_order;
								memcpy((uint8 *)tempC8, buffer_ptr, data_size);
								buffer_ptr += data_size;
								break;
							case DFNT_UCHAR8:
								data_size = current_order * sizeof(uchar8);
								tempUC8 = (uchar8 *)output_data[k] + i * current_order;
								memcpy((uint8 *)tempUC8, buffer_ptr, data_size);
								buffer_ptr += data_size;
								break;
							case DFNT_INT8:
								data_size = current_order * sizeof(int8);
								tempI8 = (int8 *)output_data[k] +  i * current_order;
								memcpy((uint8 *)tempI8, buffer_ptr, data_size);
								buffer_ptr += data_size;
								break;
							case DFNT_UINT8:
								data_size = current_order * sizeof(uint8);
								tempU8 = (uint8 *)output_data[k] + i * current_order;
								memcpy((uint8 *)tempU8, buffer_ptr, data_size);
								buffer_ptr += data_size;
								break;
							case DFNT_INT16:
								data_size = current_order * sizeof(int16);
								tempI16 = (int16 *)output_data[k] + i * current_order;
								memcpy((uint8 *)tempI16, buffer_ptr, data_size);
								buffer_ptr += data_size;
								break;
							case DFNT_UINT16:
								data_size = current_order * sizeof(uint16);
								tempU16 = (uint16 *)output_data[k] + i * current_order;
								memcpy((uint8 *)tempU16, buffer_ptr, data_size);
								/* printf("output_data[i][0] = %d.\n", *((uint16 *)output_data[k] + i * current_order)); */
								buffer_ptr += data_size;
								break;
							case DFNT_INT32:
								data_size = current_order * sizeof(int32);
								tempI32 = (int32 *)output_data[k] + i * current_order;
								memcpy((uint8 *)tempI32, buffer_ptr, data_size);
								buffer_ptr += data_size;
								break;
							case DFNT_UINT32:
								data_size = current_order * sizeof(uint32);
								tempU32 = (uint32 *)output_data[k] + i * current_order;
								memcpy((uint8 *)tempU32, buffer_ptr, data_size);
								/* printf("output_data[i][0] = %d.\n", *((uint32 *)output_data[k] + i * current_order)); */
								buffer_ptr += data_size;
								break;
							case DFNT_FLOAT32:
								data_size = current_order * sizeof(float32);
								tempF32 = (float32 *)output_data[k] + i * current_order;
								memcpy((uint8 *)tempF32, buffer_ptr, data_size);
								/* printf("output_data[i][0] = %lf.\n", *((float32 *)output_data[k] + i * current_order)); */
								buffer_ptr += data_size;
								break;
							case DFNT_FLOAT64:
								data_size = current_order * sizeof(float64);
								tempF64 = (float64 *)output_data[k] + i * current_order;
								memcpy((uint8 *)tempF64, buffer_ptr,data_size);
								/* printf("output_data[i][0] = %lf.\n", *((float64 *)output_data[k] + i * current_order)); */
					   	      		buffer_ptr += data_size;
								break;
							}
					      }	
					   }

					   /* Done processing */
					   /* Check pointer before freeing memory.   */
					   if (buffer != NULL) free(buffer);
					   VSdetach(current_vid);
					   Vend(file_id);
   					   Hclose(file_id);
					   result=0;

				      }
				      else
				      {
					   fprintf(stdout,"\tVSread is bad!\n");
					   /* Check pointer before freeing memory.   */
					   if (buffer != NULL) free(buffer);
					   VSdetach(current_vid);
					   Vend(file_id);
   					   Hclose(file_id);
					   result=-1;
				      }
				}
				else
				{
				   fprintf(stdout,"\tBuffer size is bad!\n");
				   VSdetach(current_vid);
				   Vend(file_id);
   				   Hclose(file_id);
				   result=-1;
				}
			   }
			   else
			   {
				fprintf(stdout,"\tVSsetfields is bad!\n");
			   	Vend(file_id);
   			   	Hclose(file_id);
			   	result=-1;
			   }
			}
			else
			{
				fprintf(stdout,"\tNumber of fields is bad!\n");
				Vend(file_id);
   				Hclose(file_id);
				result=-1;
			}
		   }
		   else
		   {
			fprintf(stdout,"\tCurrent Vid is bad!\n");
			Vend(file_id);
   			Hclose(file_id);
			result=-1;
		   }		
		}
		else
		{
			fprintf(stdout,"\tCurrent Vref is bad!\n");
			Vend(file_id);
   			Hclose(file_id);
			result=-1;
		}
	}
	else
	{
		result=-1;
	}

/*
** Now print a line to seperate the output from this function and others
*/
   /* 
   fprintf(stdout,"\r\n-----------------------------------------------------\r\n");
   fprintf(stdout,"Leaving C routine with result=%i\n",result);
   */

   return(result);
}
/*
---------------------------------------------------------------
	set_vdata_data
---------------------------------------------------------------
*/

void  set_vdata_data(char *filename, char *vdata_name, int32 field_count,
                     char *field_list, int32 *number_type, int32 *order,
                     int32 n_records, int32 *number_index, void *ptr[])
                     
{

	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
        char		*ptr_field;
        char		*ptr_start;
        char		*ptr_temp;
        char		temp_field[MAX_CHAR_LENGTH];
   	int32		current_vid;
   	int32		current_vref;
   	int32        	file_id;
   	int32           i;
   	int32           k;
	int32           status;
	int32           data_size;
	int32           stor_size;
	int32		this_index;

	/*
   	----------------------------------------
		Start of Code	
   	----------------------------------------
   	*/

   	file_id=Hopen(filename,DFACC_RDONLY,0);
	if (file_id!=-1)
	{
           Vstart(file_id);
	   current_vref=VSfind(file_id, vdata_name);
	   if (current_vref)
	   {
	      current_vid=VSattach(file_id,current_vref,"r");
	      if (current_vid!=-1)
	      {
	         if (field_count!=-1)
		 {
	         ptr_temp = malloc(strlen(field_list)+1);
	         if(ptr_temp == NULL)
	         {
	            fprintf(stdout, "ERROR -- Unable to allocate memory for ptr_temp");
                    exit(-1);
	         }
	         strcpy(ptr_temp, field_list);
	         ptr_start = ptr_temp;
	         for (k=0; k<field_count; k++)	
	         {	
	            ptr_field = strchr(ptr_start, ',');
	            if(ptr_field != NULL) *ptr_field = '\0';
	            strcpy(temp_field, ptr_start);
	            ptr_start = ptr_field + 1;
	            status=VSfindex(current_vid,temp_field,&this_index);
	            if(status == -1)
	            {
	               fprintf(stdout, "ERROR -- Cannot find the index for %s\n", temp_field);
	               free(ptr_temp);
                       exit(-1);
	             }
	             number_index[k] = this_index; 
	         }
	         free(ptr_temp);

           for (k=0; k<field_count; k++)
           {
              i = number_index[k];
               
              if ( order[i] == 1 ) data_size = n_records;
              else data_size = order[i];
               
              switch(number_type[i])
              {
                 case DFNT_UCHAR8:
                        {
	                   stor_size = data_size * sizeof(uchar8);
                           if ( order[i] == 1 ) ptr[k] = (uchar8 *) malloc(stor_size);
                           else ptr[k] = (uchar8 *) calloc(n_records,stor_size);
		           break;
                        }
                 case DFNT_CHAR8:
                        {
                           stor_size = data_size * sizeof(char8);
                           if ( order[i] == 1 ) ptr[k] = (char8 *) malloc(stor_size);
                           else ptr[k] = (char8 *) calloc(n_records,stor_size);
		           break;
                        }
                 case DFNT_FLOAT32:
                        {
                           stor_size = data_size * sizeof(float32);
                           if ( order[i] == 1 ) ptr[k] = (float32 *) malloc(stor_size);
                           else ptr[k] = (float32 *) calloc(n_records,stor_size);
		           break;
                        }
	         case DFNT_FLOAT64:
                        {
                           stor_size = data_size * sizeof(float64);
                           if ( order[i] == 1 ) ptr[k] = (float64 *) malloc(stor_size);
                           else ptr[k] = (float64 *) calloc(n_records,stor_size);
		           break;
                        }
	         case DFNT_INT8:
                        {
                           stor_size = data_size * sizeof(int8);
                           if ( order[i] == 1 ) ptr[k] = (int8 *) malloc(stor_size);
                           else ptr[k] = (int8 *) calloc(n_records,stor_size);
		           break;
                        }
	         case DFNT_UINT8:
                        {
                           stor_size = data_size * sizeof(uint8);
                           if ( order[i] == 1 ) ptr[k] = (uint8 *) malloc(stor_size);
                           else ptr[k] = (uint8 *) calloc(n_records,stor_size);
		           break;
                        }
	         case DFNT_INT16:
                        {
                           stor_size = data_size * sizeof(int16);
                           if ( order[i] == 1 ) ptr[k] = (int16 *) malloc(stor_size);
                           else ptr[k] = (int16 *) calloc(n_records,stor_size);
		           break;
                        }
	         case DFNT_UINT16:
                        {
                           stor_size = data_size * sizeof(uint16);
                           if ( order[i] == 1 ) ptr[k] = (uint16 *) malloc(stor_size);
                           else ptr[k] = (uint16 *) calloc(n_records,stor_size);
		           break;
                        }
	         case DFNT_INT32:
                        {
                           stor_size = data_size * sizeof(int32);
                           if ( order[i] == 1 ) ptr[k] = (int32 *) malloc(stor_size);
                           else ptr[k] = (int32 *) calloc(n_records,stor_size);
		           break;
                        }
	         case DFNT_UINT32:
	                {
                           stor_size = data_size * sizeof(uint32);
                           if ( order[i] == 1 ) ptr[k] = (uint32 *) malloc(stor_size);
                           else ptr[k] = (uint32 *) calloc(n_records,stor_size);
		           break;
	                }
              /*
                 case DFNT_INT64:
	                {
                           stor_size = data_size * sizeof(int64);
                           if ( order[i] == 1 ) ptr[k] = (int64 *) malloc(stor_size);
                           else ptr[k] = (int64 *) calloc(n_records,stor_size);
		           break;
	                }
	         case DFNT_UINT64:
	                {
                           stor_size = data_size * sizeof(uint64);
                           if ( order[i] == 1 ) ptr[k] = (uint64 *) malloc(stor_size);
                           else ptr[k] = (uint64 *) calloc(n_records,stor_size);
		           break;
	                }
              */
	         default:
	                {
		           fprintf(stderr, "Unknown type for Vdata: %d.\n",
                                   number_type[i]);
                           ptr[k] = NULL;
	                }
              }
           }
                 }
                 else
                 {
                    fprintf(stdout,"\tNumber of fields is bad!\n");
		    VSdetach(current_vid);
                    Vend(file_id);
                    Hclose(file_id);
                 }
              }
              else
              {
                 fprintf(stdout,"\tCurrent Vid is bad!\n");
                 Vend(file_id);
                 Hclose(file_id);
              }		
           }
           else
           {
              fprintf(stdout,"\tCurrent Vref is bad!\n");
              Vend(file_id);
              Hclose(file_id);
           }
        }
        else
        {
           fprintf(stdout,"\tError opening HDF file %s, in set_vdata_data\n",filename);
           exit(-1);
        }
}
/*
---------------------------------------------------------------
	print_vdata
---------------------------------------------------------------
*/

int32  print_vdata(char *vdata_name, int32 field_count, int32 n_records,
                   char *field_list, int32 *number_type, int32 *order,
                   int32 *number_index, void *ptr[])
{

	/*
   	----------------------------------------
		Local variables
   	----------------------------------------
   	*/
        char		*ptr_field;
        char		*ptr_start;
        char		*ptr_temp;
        char		temp_field[MAX_CHAR_LENGTH];
   	int32           i,j,k,l;
   	int32           index;

	/*
   	----------------------------------------
		Start of Code	
   	----------------------------------------
   	*/

        fprintf (stdout,"\nVdata name: %s\n",vdata_name);

	ptr_temp = malloc(strlen(field_list)+1);
	if(ptr_temp == NULL)
	{
	   fprintf(stdout, "ERROR -- Unable to allocate memory for ptr_temp");
           return(-1);
	}
	strcpy(ptr_temp, field_list);
	ptr_start = ptr_temp;
	for (k=0; k<field_count; k++)	
	{	
	   ptr_field = strchr(ptr_start, ',');
	   if(ptr_field != NULL) *ptr_field = '\0';
	   strcpy(temp_field, ptr_start);
	   ptr_start = ptr_field + 1;

	   i = number_index[k];
           fprintf (stdout,"\n Vdata field name: %s\n",temp_field);
               
              switch(number_type[i])
              {
                 case DFNT_UCHAR8:
                        {
           		   for (j=0; j<n_records; j++)
           		   {
              		      index = j*order[i];
                              for (l=0; l<order[i]; l++)
                              {
				 if ( *((uchar8 *)ptr[k]+index+l) == NULL ) break;
                                 fprintf (stdout,"%c",
			         *((uchar8 *)ptr[k]+index+l));
                              }
              		      fprintf (stdout,"\n");
           		   }
		           break;
                        }
                 case DFNT_CHAR8:
                        {
           		   for (j=0; j<n_records; j++)
           		   {
              		      index = j*order[i];
                              for (l=0; l<order[i]; l++)
                              {
				 if ( *((char8 *)ptr[k]+index+l) == NULL ) break;
                                 fprintf (stdout,"%c",
			         *((char8 *)ptr[k]+index+l));
                              }
              		      fprintf (stdout,"\n");
           		   }
		           break;
                        }
                 case DFNT_FLOAT32:
                        {
           		   for (j=0; j<n_records; j++)
           		   {
              		      index = j*order[i];
                              for (l=0; l<order[i]; l++)
                                 fprintf (stdout," %f",
			         *((float32 *)ptr[k]+index+l));
              		      fprintf (stdout,"\n");
           		   }
		           break;
                        }
	         case DFNT_FLOAT64:
                        {
           		   for (j=0; j<n_records; j++)
           		   {
              		      index = j*order[i];
                              for (l=0; l<order[i]; l++)
                                 fprintf (stdout," %f",
			         *((float64 *)ptr[k]+index+l));
              		      fprintf (stdout,"\n");
           		   }
		           break;
                        }
	         case DFNT_INT8:
                        {
           		   for (j=0; j<n_records; j++)
           		   {
              		      index = j*order[i];
                              for (l=0; l<order[i]; l++)
                                 fprintf (stdout," %d",
			         *((int8 *)ptr[k]+index+l));
              		      fprintf (stdout,"\n");
           		   }
		           break;
                        }
	         case DFNT_UINT8:
                        {
           		   for (j=0; j<n_records; j++)
           		   {
              		      index = j*order[i];
                              for (l=0; l<order[i]; l++)
                                 fprintf (stdout," %u",
			         *((uint8 *)ptr[k]+index+l));
              		      fprintf (stdout,"\n");
           		   }
		           break;
                        }
	         case DFNT_INT16:
                        {
           		   for (j=0; j<n_records; j++)
           		   {
              		      index = j*order[i];
                              for (l=0; l<order[i]; l++)
                                 fprintf (stdout," %d",
			         *((int16 *)ptr[k]+index+l));
              		      fprintf (stdout,"\n");
           		   }
		           break;
                        }
	         case DFNT_UINT16:
                        {
           		   for (j=0; j<n_records; j++)
           		   {
              		      index = j*order[i];
                              for (l=0; l<order[i]; l++)
                                 fprintf (stdout," %u",
			         *((uint16 *)ptr[k]+index+l));
              		      fprintf (stdout,"\n");
           		   }
		           break;
                        }
	         case DFNT_INT32:
                        {
           		   for (j=0; j<n_records; j++)
           		   {
              		      index = j*order[i];
                              for (l=0; l<order[i]; l++)
                                 fprintf (stdout," %d",
			         *((int32 *)ptr[k]+index+l));
              		      fprintf (stdout,"\n");
           		   }
		           break;
                        }
	         case DFNT_UINT32:
	                {
           		   for (j=0; j<n_records; j++)
           		   {
              		      index = j*order[i];
                              for (l=0; l<order[i]; l++)
                                 fprintf (stdout," %u",
			         *((uint32 *)ptr[k]+index+l));
              		      fprintf (stdout,"\n");
           		   }
		           break;
	                }
              /*
                 case DFNT_INT64:
	                {
           		   for (j=0; j<n_records; j++)
           		   {
              		      index = j*order[i];
                              for (l=0; l<order[i]; l++)
                                 fprintf (stdout," %d",
			         *((int64 *)ptr[k]+index+l));
              		      fprintf (stdout,"\n");
           		   }
		           break;
	                }
	         case DFNT_UINT64:
	                {
           		   for (j=0; j<n_records; j++)
           		   {
              		      index = j*order[i];
                              for (l=0; l<order[i]; l++)
                                 fprintf (stdout," %u",
			         *((uint64 *)ptr[k]+index+l));
              		      fprintf (stdout,"\n");
           		   }
		           break;
	                }
              */
	         default:
	                {
		           fprintf(stderr, "Unknown type for Vdata: %d.\n",
                                   number_type[i]);
                           ptr[k] = NULL;
	                }
              }
  	}
        free(ptr_temp);
        return(0);
}
         
         
void free_ptr_array(int32 n_elemts, void *ptr_array[])
{
 
   int32 i;

   for(i = 0; i < n_elemts; i++)
   {
      free(ptr_array[i]);
   }
}
