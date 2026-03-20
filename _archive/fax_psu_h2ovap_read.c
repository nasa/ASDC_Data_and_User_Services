/************************************************************************
*                                                                       *
* PROGRAM:   fastxh2ovap_read.c                                         *
*                                                                       *
* VERSION:   1.0                       DATE:  September 27, 1995        *
*                                                                       *
* PURPOSE:   The purpose of this code is to read the FIRE ASTEX         *
*            water vapor and liquid vapor (H2OVAP) data set.            *
*                                                                       *
* ALGORITHM: None.                                                      *
*                                                                       *
* INVOCATION:    fastxh2ovap_read.c datafile                            *
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
*           Pennsylvania State University Water Vapor data set.         *
*                                                                       *
*                                                                       *
************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 81
#define NUM_PARAMS 7

main(argc,argv)
int argc;
char *argv[];
{
  char in_file[LINESIZE],       /* data set to be read                       */
       fname[LINESIZE],         /* data set name given in command line       */
       buf[LINESIZE],           /* holds the first line which is the header */
       tmp[4],                  /* place holding for the param selection    */
       list[4];
  int  count, counter,          /* counter                                  */
       params[100],             /* integer format for params selected       */
       i;
  double hour,                  /* hour                                     */
         vapcm,                 /* vapor -- cm units                        */
         liqcm,                 /* liquid -- cm units                       */
         vap1m,                 /* vapor -- 1m units                        */
         liq1m,                 /* liquid -- 1m units                       */
         vap5m,                 /* vapor -- 5m units                        */
         liq5m;                 /* liquid -- 5m units                       */

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
  printf("The data set to be read is %s \n\n", in_file);

/* Open data set to read */

  fptr = fopen(in_file,"r");

/* Check to see if eof encountered  */

  if(fptr == NULL)
  {
    printf("***********************************************************\n");
    printf("*                                                         *\n");
    printf("*           ERROR -- UNABLE TO OPEN THE DATA SET          *\n");
    printf("*                   PROGRAM TERMINATED                    *\n");
    printf("*                                                         *\n");
    printf("***********************************************************\n");
    exit(-1);
  }

/* Print the parameters available on this data set  */

  printf("\n\n");
  printf("The following are the parameters on this data set: \n");
  printf("\n\n");
  printf("No.   Parameter                               Units \n");
  printf("---   ---------                               ----- \n");
  printf("\n");
  printf(" 1.   Hour                                     GMT  \n");
  printf(" 2.   Vapor                                    CM   \n");
  printf(" 3.   Liquid                                   CM   \n");
  printf(" 4.   Vapor-1 minute                           CM   \n");
  printf(" 5.   Liquid-1 minute                          CM   \n");
  printf(" 6.   Vapor-5 minute                           CM   \n");
  printf(" 7.   Liquid-5 minute                          CM   \n");
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
      count=1;
      tmp[0] = 'S';
      while((tmp[0] != 'x') && (tmp[0] != 'X') && (tmp[0] != 'q') && (tmp[0] != 'Q'))
      {
         printf("Please enter parameter# you wish to view \n");
         printf("Enter q to end selection process or X to exit the program --> \n");
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
          printf("**** EXITING THE PROGRAM PER YOUR REQUEST ****\n");
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

/* Read the first line of the program which contains the header  */

   fgets(&buf[0],LINESIZE,fptr);

/* Read the data set and print the parameters requested by the user */

   counter = 0;                 /* counter is for # of records read */

/* Print the headers for the requested parameters */

   for (i = 1; i <= count; i++)
   {
     switch (params[i-1])
     {
	case 0:
          printf("  Hour    ");
          break;
        case 1:
          printf("  VAP(CM) ");
          break;
        case 2:
          printf("  LIQ(CM) ");
          break;
        case 3:
          printf("  VAP(1M) ");
          break;
        case 4:
          printf("  LIQ(1M) ");
          break;
        case 5:
          printf("  VAP(5M) ");
          break;
        case 6:
          printf("  LIQ(5M) ");
          break;
        default:
          break;
     }         /* end of case statement */
   } /* end of the for loop */
  printf("\n\n");

  counter = 0;
  while ((feof(fptr) == 0))
  {
    hour  = 0.0;
    vapcm = liqcm = 0.0;
    vap1m = liq1m = 0.0;
    vap5m = liq5m = 0.0;
    
    fscanf(fptr,"%lf%lf%lf%lf%lf%lf%lf",&hour,&vapcm,&liqcm,&vap1m,&liq1m,&vap5m,&liq5m);

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
               printf(" %.4lf  ",hour);
               break;
             case 1:
               printf(" %.4lf  ",vapcm);
               break;
             case 2:
               printf(" %.4lf  ",liqcm);
               break;
             case 3:
               printf(" %.4lf  ",vap1m);
               break;
             case 4:
               printf(" %.4lf  ",liq1m);
               break;
             case 5:
               printf(" %.4lf  ",vap5m);
               break;
             case 6:
               printf(" %.4lf  ",liq5m);
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
   printf("***************************************************************\n");
   printf("*                                                             *\n");
   printf("*     FIRE ASTEX Pennsylvania State University Water Vapor    *\n");
   printf("*                                                             *\n");
   printf("*                        READ PROGRAM                         *\n");
   printf("*                                                             *\n");
   printf("* Version 1.0                              September 27, 1995 *\n");
   printf("***************************************************************\n");
   printf("\n\n");
 }  /* end of the display greeting */
