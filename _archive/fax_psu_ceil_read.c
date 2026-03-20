/************************************************************************
*                                                                       *
* PROGRAM:   fax_ceilmal_read.c                                         *
*                                                                       *
* VERSION:   1.0                       DATE:  September 18, 1995        *
*                                                                       *
* PURPOSE:   The purpose of this code is to provide a sample read       *
*            program for users who order the FIRE ASTEX Pennsylvania    *
*            State University Ceilometer data sets which is able to     *
*            read all three sites: Malcolm Baldrige, Santa Maria, and   *
*            Valdivia.                                                  *
*                                                                       *
* CONTACT:   If you have any questions, please contact the Langley DAAC.*
*                                                                       *
*            Langley DAAC User and Data Services Office                 *
*            NASA Langley Research Center                               *
*            Mail Stop 157D                                             *
*            Hampton, VA 23681-0001                                     *
*            USA                                                        *
*                                                                       *
*            Phone:  (804)864-8656                                      *
*            FAX:    (804)864-8807                                      *
*            E-Mail: userserv@eosdis.larc.nasa.gov                      *
*                                                                       *
*                                                                       *
* ALGORITHM: None.                                                      *
*                                                                       *
* INVOCATION:    fax_ceilmal_read                                       *
*                                                                       *
* FILE/RECORD REFERENCES:                                               *
*            This is an interactive program.  Currently, this program   *
*            will produce no output files.  ALL output will be printed  *
*            to the screen.                                             *
*                                                                       *
* EXTERNAL ROUTINES:  None.                                             *
*                                                                       *
* INTERNAL ROUTINES:                                                    *
*            #include <stdio.h>                                         *
*            #include <stdlib.h>                                        *
*            #include <string.h>                                        *
*                                                                       *
* NOTES:    This program has been set up to read the FIRE ASTEX         *
*           Pennsylvania State University Ceilometer data sets.         *
*           The program prompts the user for the granule name.          *
*           The program then asks which variables he wishes to output   *
*           to the screen.  The output then scrolls onto the screen     *
*           and then a closing statement is printed to the screen when  *
*           the end of the file has been reached.
*                                                                       *
*           The TIME parameter is the parameter users should be aware   *
*           of.  The format for the value is an integer format of       *
*           hhmmss where hh represents hours, mm represents minutes,    *
*           ss represents seconds.  There may be instances when any of  *
*           these values may be less than 10.  If so, instead of the    *
*           value being represented as "01," the user will see " 1"     *
*           where the 0 - the first digit has been left blank.          *
*                                                                       *
************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_PARAMS 3
#define LINESIZE 81
#define TIMESIZE 8

main(argc,argv)
int argc;
char *argv[];
{
  char in_file[LINESIZE],       /* data set to be read                       */
       fname[LINESIZE],         /* data set name given in command line       */
       buf[LINESIZE],
       tmp[4],                  /* place holding for the param selection    */
       list[4],
       time[8],
       cloud_base[8],           /* cloud base                               */
       ceil_code[3],            /* ceilometer data code                     */
       blank[2];

  int  count, counter,          /* counter                                  */
       params[NUM_PARAMS],      /* integer format for params selected       */
       i;

  FILE *fptr;                   /* file pointer to input data set            */

  void Display_greeting();

/* Initialize the params array to 0 */

   for (count=0; count < NUM_PARAMS; count++)
      params[count] = -99;
   strcpy(blank," ");
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
  printf("No.   Parameter                               Units \n");
  printf("---   ---------                               ----- \n");
  printf("\n");
  printf(" 1.   Time                                    hhmmss\n");
  printf(" 2.   Cloud Base                                m   \n");
  printf(" 3.   Ceilometer code                          n/a  \n");
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
      for(count=0; count < NUM_PARAMS; count++)
        params[count] = count;
    }
    else if ((list[0] == 's') || (list[0] == 'S'))
    { 
      printf("****  Selection of parameters  ***\n");

      for(count=1; count <= NUM_PARAMS; count++)
      {
         printf("Please enter parameter# you wish to view \n");
         printf("Enter q to end selection process or X to exit the program --> ");
         scanf("%s",tmp);
         strcpy(list,tmp);
         if((list[0] == 'x') || (list[0] == 'X'))
	 {
           printf("\n");
           printf("**** EXITING THE PROGRAM PER YOUR REQUEST ****\n");
           exit(0);
         }  
         if ((list[0] == 'q') || (list[0] == 'Q')) break;
         if(atoi(tmp) > NUM_PARAMS)
	 {
           printf("\n");
           printf("A NUMBER GREATER THAN THE NUMBER OF PARAMETERS AVAILABLE HAS BEEN INPUT\n");
          strcpy(tmp,"    ");
         }
       


         params[count-1] = atoi(tmp) - 1;
      }                                      /* end of the for statement */
    }                                        /* end of the if-else statement */
    else if ((list[0] = 'x') || (list[0] = 'X'))
    {
     printf("\n\n");
     printf(" **** EXITING THE PROGRAM PER YOUR REQUEST *****\n");
     exit(0);
    }


 /* Read the data set and print the parameters requested by the user */

  counter = count = 0;         /* counter is for # of records read */
                               /* count is for the parameter selected */

/* Print the headers for the requested parameters */

  for (count = 1; count <= NUM_PARAMS; count++)
  {
     switch (params[count-1])
     {
	case 0:
          printf("  Time      ");
          break;
        case 1:
          printf("  Cloud Base  ");
          break;
        case 2:
          printf("  Ceil. Code  ");
          break;
        default:
          break;
     }         /* end of case statement */
   } /* end of the for loop */
  printf("\n\n");

  counter = 0;
  while ((feof(fptr) == 0))
  {
    strcpy(buf,blank);
    strcpy(time,blank);
    strcpy(cloud_base,blank);
    strcpy(ceil_code,blank);  
    fgets(&buf[0],TIMESIZE,fptr);
    strncpy(time,buf,8);
    strcpy(buf,blank);
    fgets(&buf[0],TIMESIZE,fptr);
    strncpy(cloud_base,buf,7);
    strcpy(buf,blank);
    fgets(&buf[0],3,fptr);
    strncpy(ceil_code,buf,3);
    fgets(&buf[0],LINESIZE,fptr);
    
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

    for (count = 0; count< NUM_PARAMS; count++)
    {
         switch (params[count])
	   {
	     case 0:
               printf(" %6s    ",time);
               break;
             case 1:
               printf("    %4s  ",cloud_base);
               break;
             case 2:
               printf("    %1s  ",ceil_code);
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
   printf("*        FIRE ASTEX PENNSYLVANIA STATE UNIVERSITY (PSU)       *\n");
   printf("*               CEILOMETER DATA READ PROGRAM FOR              *\n");
   printf("*               THE FOLLOWING LOCATIONS:                      *\n");
   printf("*                                                             *\n");
   printf("*                    Malcolm Baldrige                         *\n");
   printf("*                    Santa Maria                              *\n");
   printf("*                    Valdivia                                 *\n");
   printf("*                                                             *\n");
   printf("* Version 1.0                                      09-18-95   *\n");
   printf("***************************************************************\n");
   printf("\n\n");
 }  /* end of the display greeting */
