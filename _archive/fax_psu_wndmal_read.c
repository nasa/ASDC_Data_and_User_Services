/************************************************************************
*                                                                       *
* PROGRAM:   fax_psu_wndmal_read.c                                      *
*                                                                       *
* VERSION:   1.0                       DATE:  September 19, 1995        *
*                                                                       *
* PURPOSE:   The purpose of this code is to read the FIRE ASTEX         *
*            wind profiles on Malcolm Baldrige (WND_MAL) data set.      *
*                                                                       *
* ALGORITHM: None.                                                      *
*                                                                       *
* INVOCATION:    fax_psu_wndmal_read datafile                           *
*      WHERE:                                                           *
*          <datafile> - optional                                        *
*                       input ASCII data file                           *
*                                                                       *
* FILE/RECORD REFERENCES:                                               *
*            Currently, this program will produce no output files.  ALL *
*            output will be printed to the screen.                      *
*                                                                       *
* EXTERNAL ROUTINES:  None.                                             *
*                                                                       *
* INTERNAL ROUTINES:                                                    *
*            #include <stdio.h>                                         *
*            #include <stdlib.h>                                        *
*            #include <string.h>                                        *
*                                                                       *
* NOTES:    This program has been set up to read the FIRE ASTEX         *
*           wind profiles on Malcolm Baldrige (WND_MAL) data set files. *
*           The first line contains a header which consists of the hour,*
*           height, wind speed, and wind direction.                     *
*                                                                       *
*                                                                       *
************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 81
#define NUM_PARAMS 4

main(argc,argv)
int argc;
char *argv[];
{
  char in_file[LINESIZE],       /* data set to be read                       */
       fname[LINESIZE],         /* data set name given in command line       */
       buf[LINESIZE],           /* holds the header                         */
       tmp[4],                  /* place holding for the param selection    */
       list[4];
  int  count, counter,          /* counter                                  */
       params[100],             /* integer format for params selected       */
       i;
  double hour,height,
         wnd_speed,wnd_dir;

  FILE *fptr;                   /* file pointer to input data set            */

  void Display_greeting();

/* Initialize the params array to 0 */

   for (count=0; count <= 100; count++)
      params[count] = -99;
   (void) Display_greeting();

/* Get name of data set to be read */

  if (argc == 1) 
  {
    printf("Please enter the name of the data set to be read:  ");
    scanf("%s",fname);
    strcpy(in_file,fname);
  }
  else
  {
    strcpy(in_file,argv[1]);
  }

  printf("\n\n");
  printf("The data set to be read is %s \n", in_file);

/* Open data set to read */

  fptr = fopen(in_file,"r");

/* Check to see if eof encountered  */

  if(fptr == NULL)
  {
    printf("**********************************************************\n");
    printf("*                                                        *\n");
    printf("*           ERROR -- UNABLE TO OPEN THE DATASET          *\n");
    printf("*                   PROGRAM TERMINATED                   *\n");
    printf("*                                                        *\n");
    printf("**********************************************************\n");
    exit(-1);
  }

/* Print the parameters available on this data set  */

  printf("\n\n");
  printf("The following are the parameters on this data set: \n");
  printf("\n\n");
  printf("No.   Parameter                           Units \n");
  printf("---   ---------                           ----- \n");
  printf("\n");
  printf(" 1.   Hour                                 hour \n");
  printf(" 2.   Height                               m    \n");
  printf(" 3.   Wind speed                           m/s  \n");
  printf(" 4.   Wind direction                       deg  \n");
  printf("\n\n");
  
  /*  Selection of parameters whether all or some  */

  list[0] = 'Q';
  while ((list[0]!='a') && (list[0]!='A') && (list[0]!='s') && (list[0]!='S') && (list[0]!='x') && (list[0]!='X'))
  {
    printf(" Do you want all parameters to be printed \n");
    printf(" Please select: a or A for All; S for Some; X to eXit: ");
    scanf("%s",tmp); 
    strcpy(list,tmp); 
  }                                          /* end of while statement */

    if ((list[0] == 'a') || (list[0] == 'A'))
    {
      for(count=0; count <= NUM_PARAMS; count++)
        params[count] = count;
    }
    else if ((list[0] == 's') || (list[0] == 'S'))
    { 
      printf("****  Selection of parameters  ***\n");
      tmp[0] = 'S';
      count = 1;

      while((tmp[0] != 'x') && (tmp[0] != 'X') && (tmp[0] != 'q') && (tmp[0] != 'Q'))
      {
         printf("Please enter parameter# you wish to view \n");
         printf("Enter q for end of selection process or X to exit program --> ");
         scanf("%s",tmp);
         strcpy(list,tmp);

         if ((list[0] == 'q') || (list[0] == 'Q'))
         {
           if(params[0] == -99)
           {
             strcpy(tmp,"    ");             
             printf("\n");
             printf(" Selection of parameters have not yet been made \n");
             printf(" Do you want to exit the program -- enter x or X \n");
             printf(" OR Enter parameters numbers \n");
             scanf("%s",tmp);
             strcpy(list,tmp);
           }
           else break;
         }

         if ((list[0] =='x') || (list[0] == 'X'))
         {
           printf("\n\n");
           printf(" **** EXITING THE PROGRAM PER YOUR REQUEST *****\n");
           exit(0);
         } 
        
         if((atoi(tmp) > NUM_PARAMS) || (atoi(tmp) <= 0))
	 {
           printf("\n");
           printf("A NUMBER NOT AVAILABLE FOR PARAMETER SELECTION HAS BEEN INPUT\n");
           strcpy(tmp,"    ");
         }

         params[count-1] = atoi(tmp) - 1;
         count++;
      }                                      /* end of the while statement */
    }                                        /* end of the if-else statement */
    else if ((list[0] = 'x') || (list[0] = 'X'))
    {
     printf("\n\n");
     printf(" **** EXITING THE PROGRAM PER YOUR REQUEST *****\n");
     exit(0);
    }


 /* Read the data set and print the parameters requested by the user */

    counter = 0;                 /* counter is for # of records read */

/* The header is read in and printed out */

  fgets(&buf[0],LINESIZE,fptr);

/* Print the headers for the requested parameters */

  for (i = 1; i <= count; i++)
  {
     switch (params[i-1])
     {
	case 0:
          printf("     Hour   ");
          break;
        case 1:
          printf("    Height (m) ");
          break;
        case 2:
          printf("  Wind speed (m/s)");
          break;
	case 3:
          printf("  Wind dir (deg)");
          break;
        default:
          break;
     }         /* end of case statement */
   } /* end of the for loop */
  printf("\n");


  counter = 0;
  while ((feof(fptr) == 0))
  {
    hour=height=wnd_speed=wnd_dir = 0.0;
    
    fscanf(fptr,"%lf%lf%lf%lf",&hour,&height,&wnd_speed,&wnd_dir);

/*  End of file encountered */

    if (feof(fptr) != 0)
    {
      printf("\n\n");
      printf("****************************************************\n");
      printf("*                                                  *\n");
      printf("*              END OF FILE ENCOUNTERED             *\n");
      printf("*                                                  *\n");
      printf("*          %d records of data were read          *\n",counter);
      printf("*                                                  *\n");
      printf("*  EXECUTION OF PROGRAM COMPLETED AND SUCCESSFUL   *\n");
      printf("*                                                  *\n");
      printf("****************************************************\n");
      exit(0);
    }

/*  Print the parameters selected to the screen */

    for (i = 1; i<= count; i++)
    {
         switch (params[i-1])
	   {
	     case 0:
               printf("     %6lf ",hour);
               break;
             case 1:
               printf("    %.4lf  ",height);
               break;
	     case 2:
               printf("     %.4lf ",wnd_speed);
               break;
	     case 3:
               printf("      %.4lf ",wnd_dir);
               break;
             default:
               break;
	     }         /* end of case statement */
       }    /* end of the for-statement  */
     printf("\n");
     counter++;
  }  /* end of the while statement */
} /* end of the main program */


void Display_greeting()
  {
   printf("********************************************************\n");
   printf("*                                                      *\n");
   printf("* FIRE ASTEX WIND PROFILE OF MALCOLM BALDRIGE DATA SET *\n");
   printf("*                                                      *\n");
   printf("*                      READ PROGRAM                    *\n");
   printf("*                                                      *\n");
   printf("* Version 1.0                       September 19, 1995 *\n");
   printf("********************************************************\n");
   printf("\n\n");
 }  /* end of the display greeting */
