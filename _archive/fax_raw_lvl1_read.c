/************************************************************************
*                                                                       *
* PROGRAM:   fax_raw_lvl1_read.c                                        *
*                                                                       *
* VERSION:   1.0                       DATE:  September 20, 1995        *
*                                                                       *
* PURPOSE:   The purpose of this code is to provide a sample read       *
*            program for users who order the FIRE ASTEX Rawinsonde      *
*            Level 1 data set.                                          *
*                                                                       *
* ALGORITHM: None.                                                      *
*                                                                       *
* INVOCATION:    fax_raw_lvl1_read datafile                             *
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
*           Rawinsonde Level 1 data set files.                          *
*           The first line contains the variable names.                 *
*           The second line through to the end of the file contains     *
*           eleven columns of data.                                     *
*                                                                       *
*                                                                       *
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  SIZE   93        /* Number of characters on a line */


main(argc,argv)
int argc;
char *argv[];
{
  char     buf[SIZE],         /* String used to read in data */
           filename[30],      /* Input file name */
           temp_name[30],     /* Input temporary name */
           time[12];          /* TIME - first parameter in data file */

  FILE     *fptr;             /* Input file pointer */

  int      geopm,             /* Number of parameters user dumps */ 
           count,             /* counter */
           dir;               /* Direction in deg */
  int      param[11];
  int      rec_count;
  int      amount;

  double   azdeg,             /* AscRate in m/s */
           eldeg,             /* Dewp in degC */
           etime,             /* Mix R. in g/kg */
           pmb,               /* Pressure in hPa */
           rh,                /* RH in % */
           speed,             /* Speed in m/s */
           tdew,              /* Temp in degC */
           temp;              /* Temperature */


  void     Print_greeting ();

          /*************************************/
          /*                                   */
          /*       Begin Read Program          */
          /*                                   */
          /*************************************/

  for (count = 0; count < 11; count++)
      param[count] = -1;

  (void) Print_greeting();

  if (argc == 1)                  /* Interactive Mode */
  {
     printf("Please enter the input data file name --> ");
     scanf("%s",temp_name);
     strcpy(filename,temp_name);
  }
  else
  {
      strcpy(filename,argv[1]);
  }
  printf("\n\n");

  fptr = fopen(filename,"r");
 printf("The input filename is %s\n",filename);

  if (fptr == NULL)
  {
     printf("*****************************************\n");
     printf("* ERROR - Unable to open the data file. *\n");
     printf("*         Program has terminated.       *\n");
     printf("*****************************************\n");
     printf("\n");
     exit ( -1 );
  }

  /* Reads the first three lines of data file and prints to screen */

  fgets((char *) &buf[0], SIZE, fptr);
  printf("The first line of this data file consists of:\n");
  printf(" %s",buf);


  /* Reads next two lines of data file and continues */

  printf("\n\n");
  printf("There are eleven parameters of information\n");
  printf("for each event.  These parameters are:\n");
  printf("\n");
  printf("    No.      Parameter        Units\n");
  printf("    ---      ---------        -----\n");
  printf("     1.      TIME             min  s\n");
  printf("     2.      PMB              mb\n");
  printf("     3.      TEMP             C\n");
  printf("     4.      TDEW             C\n");
  printf("     5.      RH               percent\n");
  printf("     6.      GEOPM            m\n");
  printf("     7.      AZDEG            degrees\n");
  printf("     8.      ELDEG            degrees\n");
  printf("     9.      SPEED            m/s\n");
  printf("    10.      DIR              degrees\n");
  printf("    11.      E.TIME           degrees\n");
  printf("\n\n");

  printf("How many parameters would you like to have printed out\n");
  printf("with each event/record?  ");
  scanf("%s",temp_name);

  amount = atoi(temp_name);

  printf("%d\n",amount);

  if (amount == 11)
  {
     for (count = 0; count < 11; count++)
        param[count] = count;
  }
  else if (amount < 11)
  {
     for (count = 1; count <= amount; count++)
     {
        printf("Enter parameter #%d --> ",count);
        scanf("%s",temp_name);
        param[count-1] = atoi(temp_name) - 1;
        printf("\n");
     }
  }

  rec_count = 0;

  while ((feof(fptr)) == 0)
  {
     pmb      = 0.0;
     temp     = 0.0;
     tdew     = 0.0;
     rh       = 0.0;
     geopm    = 0;
     azdeg    = 0.0;
     eldeg    = 0.0;
     dir      = 0;
     etime    = 0.0;
     speed    = 0.0;

     fscanf(fptr,"%s%lf%lf%lf%lf%d%lf%lf%lf%d%lf",time,&pmb,&temp,
            &tdew,&rh,&geopm,&azdeg,&eldeg,&speed,&dir,&etime);

     if (feof(fptr) != 0)
     {
        printf("\n\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("~ End of file has been reached.        ~\n");
        printf("~ There are a total of %d records in  ~\n",rec_count);
        printf("~ this file.                           ~\n");
        printf("~ Program has completed successfully!  ~\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        exit ( 0 );
     }

     ++rec_count;

     for (count = 1; count <= amount; count++)
     {

       switch (param[count-1])
       {
          case 0:
             printf("%s  ",time);
             break;
          case 1:
             printf(" %.2lf  ",pmb);
             break;
          case 2:
             printf(" %.2lf  ",temp);
             break;
          case 3:
             printf(" %.2lf  ",tdew);
             break;
          case 4:
             printf(" %.2lf  ",rh);
             break;
          case 5:
             printf(" %d  ",geopm);
             break;
          case 6:
             printf(" %.2lf  ",azdeg);
             break;
          case 7:
             printf(" %.2lf  ",eldeg);
             break;
          case 8:
             printf(" %.1lf  ",speed);
             break;
          case 9:
             printf(" %d  ",dir);
             break;
          case 10:
             printf(" %.2lf  ",etime);
             break;
          default:
             break;
        }   /* End of switch */
    }   /*  End of for */
    printf("\n");
  }   /* End of while */
}   /* End of main function */

void Print_greeting()
{
  printf("*****************************************************\n");
  printf("*                                                   *\n");
  printf("*      FIRE ASTEX Rawinsonde Read Program           *\n");
  printf("*             for the two levels:                   *\n");
  printf("*                                                   *\n");
  printf("*                  LEVEL I                          *\n");
  printf("*                  LEVEL II                         *\n");
  printf("*                                                   *\n");
  printf("* Version 1.0                     December 01, 1994 *\n");
  printf("*****************************************************\n");
  printf("\n\n");
}   /* End of Print_greeting function */
