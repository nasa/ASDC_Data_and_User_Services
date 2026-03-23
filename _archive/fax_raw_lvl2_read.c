/************************************************************************
*                                                                       *
* PROGRAM:   fax_raw_lvl2_read.c                                        *
*                                                                       *
* VERSION:   1.0                       DATE:  September 20, 1995        *
*                                                                       *
* PURPOSE:   The purpose of this code is to read the FIRE ASTEX         *
*            Rawinsonde data which is broken up into two levels:        *
*            LEVEL I and LEVEL II.                                      *
*            This sample read software concentrates on the Level 2      *
*            data set.                                                  *
*                                                                       *
* ALGORITHM: None.                                                      *
*                                                                       *
* INVOCATION:    fax_raw_lvl2_read datafile                             *
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
*           Rawinsonde Level 2 data set files.                          *
*           The first three lines contain the variable names.           *
*           The fourth line through to the end of the file contain      *
*           the data.                                                   *
*                                                                       *
*                                                                       *
************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 81
#define NUM_PARAMS 6

main(argc,argv)
int argc;
char *argv[];
{
  char in_file[LINESIZE],       /* data set to be read                       */
       fname[LINESIZE],         /* data set name given in command line       */
       tmp[3],                  /* place holding for the param selection    */
       list[3],
       buf[LINESIZE];           /* buffer to hold the three lines of header */
  int  count, counter,          /* counter                                  */
       params[100],             /* integer format for params selected       */
       i;
  double geopot,                /* geopotential height                      */
         pressure,              /* pressure                                 */
         temp,                  /* temperature                              */
         dewpt_temp,            /* dew point temperature                    */
         wnd_speed,             /* wind speed                               */
         wnd_dir;               /* wind direction                           */
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
    printf("*           ERROR -- UNABLE TO OPEN THE DATA SET         *\n");
    printf("*                   PROGRAM TERMINATED                   *\n");
    printf("*                                                        *\n");
    printf("**********************************************************\n");
    exit(-1);
  }

/* Print the parameters available on this data set  */

  printf("\n\n");
  printf("The following are the parameters on this data set: \n");
  printf("\n\n");
  printf("No.   Parameter               Units \n");
  printf("---   ---------               ----- \n");
  printf("\n");
  printf(" 1.   Geopotential Height       m    \n");
  printf(" 2.   Pressure                  mb   \n");
  printf(" 3.   Temperature               C    \n");
  printf(" 4.   Dew point temperature     C    \n");
  printf(" 5.   Wind speed                m/s  \n");
  printf(" 6.   Wind direction            deg  \n");
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
      while((tmp[0] != 'x') && (tmp[0] != 'X') || (tmp[0] != 'q') && (tmp[0] != 'Q'))
      {
         printf("Please enter parameter# you wish to view \n");
         printf("Enter q to end selection process or X to exit the program--> ");
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

         if ((list[0] == 'x') || (list[0] == 'X'))
         {
           printf("\n\n");
           printf(" **** EXITING THE PROGRAM PER YOUR REQUEST ****\n");
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
      }                                      /* end of the for statement */
    }                                        /* end of the if-else statement */
    else if ((list[0] = 'x') || (list[0] = 'X'))
    {
     printf("\n\n");
     printf(" **** EXITING THE PROGRAM PER YOUR REQUEST *****\n");
     exit(0);
    }


/* Read the first three lines of the data set since this is header info  */

    for(i=1; i<=3; i++)
       fgets(&buf[0],LINESIZE,fptr);

 /* Read the data set and print the parameters requested by the user */

    counter = 0;         /* counter is for # of records read */
                               /* count is for the parameter selected */

/* Print the headers for the requested parameters */
   printf("        ");   /* this is for the counter record */

  for (i = 1; i <= count; i++)
  {
     switch (params[i-1])
     {
	case 0:
          printf("  z(m)  ");
          break;
        case 1:
          printf("  p(mb) ");
          break;
        case 2:
          printf("   t(c) ");
          break;
	case 3:
          printf("   td(c)");
          break;
	case 4:
          printf("   m/s  ");
          break;
        case 5:
          printf("   degrees");
          break;
        default:
          break;
     }         /* end of case statement */
   } /* end of the for loop */
  printf("\n");

  while ((feof(fptr) == 0))
  {
    geopot     = 0.0;
    pressure   = 0.0;
    temp       = 0.0;
    dewpt_temp = 0.0;
    wnd_speed  = 0.0;
    wnd_dir    = 0.0;
    
    fscanf(fptr,"%d%lf%lf%lf%lf%lf%lf",&counter,&geopot,&pressure,&temp,&dewpt_temp,&wnd_speed,&wnd_dir);

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

    printf("   %d    ",counter);

/*  Print the parameters selected to the screen */

    for (i = 1; i <= count; i++)
    {
         switch (params[i-1])
	   {
	     case 0:
               printf(" %.2lf  ",geopot);
               break;
             case 1:
               printf(" %.2lf  ",pressure);
               break;
	     case 2:
               printf(" %.2lf  ",temp);
               break;
	     case 3:
               printf(" %.2lf  ",dewpt_temp);
               break;
	     case 4:
               printf(" %.2lf  ",wnd_speed);
               break;
             case 5:
	       printf(" %.2lf  ",wnd_dir);
	       break;
             default:
               break;
	     }         /* end of case statement */
       }    /* end of the for-statement  */
     printf("\n");
  }  /* end of the while statement */
} /* end of the main program */


void Display_greeting()
  {
   printf("***************************************************************\n");
   printf("*                                                             *\n");
   printf("*               FIRE ASTEX RAWINSONDE LEVEL II                *\n");
   printf("*                                                             *\n");
   printf("*                        READ PROGRAM                         *\n");
   printf("*                                                             *\n");
   printf("* Version 1.0                              September 20, 1995 *\n");
   printf("***************************************************************\n");
   printf("\n\n");
 }  /* end of the display greeting */
