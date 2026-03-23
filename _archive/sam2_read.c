/**********************************************************************
*                                                                     *
*  PROGRAM: sam2_read.c                                               *
*                                                                     *
*  VERSION: 1.1                         March 30, 1994                *
*  VERSION: 2.0                         January 29, 1999              *
*                                                                     *
*                                                                     *
*  PURPOSE: The purpose of this code is to output the selected        *
*           altitudes and data parameters that the user has requested.*
*           This code works only with the SAM II Aerosol Extinction   *
*           ascii data files.                                         *
*                                                                     *
*  09-01-1994 Modified code to be ANSI compliant.                     *
*  01-29-1999 Modified code to prompt user for year in the four digit *
*             format.  However, within the program, must continue     *
*             with the two digit format because that is the format    *
*             used within each data file.                             *
*                                                                     *
*  ALGORITHM: None.                                                   *
*                                                                     *
*  INVOCATION:                                                        *
*        sam2_read                                                    *
*                                                                     *
*  FILE/RECORD REFERENCES:                                            *
*           The code has been designed to print out all data as well  *
*           as messages to the screen.  The user has the option to    *
*           redirect all output to another device or to a file.       *
*                                                                     *
*  EXTERNAL ROUTINES: None.                                           *
*                                                                     *
*  INTERNAL ROUTINES:                                                 *
*    Check_data_date:  Verifies data date.                            *
*    Check_year:       Verifies data year.                            *
*    Check_month:      Verifies data month.                           *
*                                                                     *
*                                                                     *
*  NOTES: THIS IS AN INTERACTIVE PROGRAM.  IT WILL PROMPT THE USER    *
*         FOR ALL INFORMATION.                                        *
*         To compile this program, just type in ...                   *
*         cc -o sam2_read sam2_read.c                                 *
*                                                                     *
*  RANGES FOR DATA PARAMETERS:                                        *
*     Record information:                                             *
*         GMT - Julian day                                            *
*               min = 1.0          max = 366.999                      *
*         YEAR - Data year                                            *
*               min = 78.0         max = 93.0                         *
*         MONTH - Data month                                          *
*               min = 01.0         max = 12.0                         *
*         DAY - Data day                                              *
*               min = 01.0         max = 31.0                         *
*         HOUR - Data hour                                            *
*               min = 0.0          max = 23.0                         *
*         MINUTES - Data minutes                                      *
*               min = 0.0          max = 59.0                         *
*         SECONDS - Data seconds                                      *
*               min = 0.0          max = 59.99                        *
*         EVENT - Sunrise or sunset                                   *
*               min = 0.0          max = 1.0                          *
*         ORBIT - Satellite orbit number                              *
*               min = 1.0          max = Greater than 0.0             *
*         LAT - Latitude location, degrees                            *
*               When EVENT = 0.0 (Sunrise)                            *
*               min = -81.0        max = -49.0                        *
*               When EVENT = 1.0 (Sunset)                             *
*               min = 64.0         max = 87.0                         *
*         LON - Longitude location, degrees                           *
*               min = -180.0       max = 180.0                        *
*         TROP - Tropopause, km                                       *
*               min = 5.0          max = 17.0                         *
*         HEIGHT - Altitude of minimum temperature, km                *
*               min = -0.14        max = 44.0                         *
*                                                                     *
*     Data information:                                               *
*         ALT - Altitude, km                                          *
*               min = 4.0          max = 39.0                         *
*         EXT - Aerosol extinction coefficient, 1.0/km                *
*               min = 0.0          max = 0.042                        *
*         EXTERR - Aerosol extinction coef. uncertainty, 1.0/km       *
*               min = 0.0000002    max = 0.029                        *
*         EXTR - Total extinction ratio, nondimensional               *
*               min = 1.0          max = 1057.0                       *
*         EXTRERR - Total extinction ratio uncertainty, nondimensional*
*               min = 0.03         max = 1015.0                       *
*         TMPMET - NMC temperature in degrees, Kelvin                 *
*               min = 174.0        max = 288.0                        *
*         TMPERR - NMC temperature uncertainty in degrees, Kelvin     *
*               min = 2.0          max = 11.0                         *
*         PRESS - NMC pressure, mb                                    *
*               min = 1.69         max = 631.0                        *
*                                                                     *
*                                                                     *
*  NOTE:  All of these paramters may contain a "fill"                 *
*         value which is 9999.0.                                      *
*         TROP, HEIGHT, ALT, EXT, EXTERR, EXTR, EXTRERR, TMPMET,      *
*         TMPERR, PRESS.                                              *
*                                                                     *
***********************************************************************/
#include <stdio.h>         /* Standard C library calls */
#include <string.h>        /* Standard string function calls */

#define HEADER    44       /* 44 lines of data info at top of each file */
#define SIZE      81       /* Number of characters on a line */
#define FILL_VAL 9999.0    /* Assigned fill value for data set */


main(argc,argv)
int argc;
char *argv[];
{
  unsigned char  buf[SIZE];/* String used to read in data */

  FILE     *fptr;          /* Input file pointer */

  int      amount,         /* Number of parameters to be printed to screen */
           param[9],       /* Array which stores parameter numbers selection */
           altamount,      /* Number of altitudes users wants printed to screen */
           altitude[40],   /* Only working with altitudes 4 through 39 */
           i, j,           /* Counter */
           label_month,    /* Month passed in at argv[3] */
           label_year,     /* Year passed in at argv[2] */
           rec_count=1;    /* Record counter */

/* Declaration of variables for record information */
  double   gmt,            /* Julian day - position 1 */
           year,           /* Data year - position 2 */
           month,          /* Data month - position 3 */
           day,            /* Data day - position 4 */
           hour,           /* Data hour - position 5 */
           minute,         /* Data minute - postion 6 */
           seconds,        /* Data seconds - position 7 */
           event,          /* 0.0 = sunrise, 1.0 = sunset - position 8 */
           orbit,          /* Orbit number - position 9 */
           lat,            /* Latitude of data collection - position 10 */
           lon,            /* Longitude of data collection - position 11 */
           trop,           /* Tropopause height range - position 12 */
           height;         /* Height of minimum temperature - position 13 */

/* Declaration of variables for actual data information */
  double   data[9],        /* Where data[0] - not in use
                                    data[1] - altitude
                                    data[2] - aerosol extinction coefficient
                                    data[3] - aerosol extinction uncertainty
                                    data[4] - total extinction ratio
                                    data[5] - total extinction ratio uncertainty
                                    data[6] - nmc temperature
                                    data[7] - nmc temperature uncertainty
                                    data[8] - nmc pressure                  */
           value;          /* Used to store data values */

  char     filename[20],   /* String to store filenames */
           teststring[132]; /* Strings passed in from keyboard */

/* Declaration of functions to verify the record information */
     void Check_data_date ();
     void Check_year ();
     void Check_month ();

                        /*************************/
                        /*  MAIN PROGRAM BEGINS  */
                        /*************************/

  printf("**********************************************************\n");
  printf("*                                                        *\n");
  printf("*                        SAM II                          *\n");
  printf("*                     Read Program                       *\n");
  printf("*                                                        *\n");
  printf("*  January 29, 1999                         Version 2.0  *\n");
  printf("**********************************************************\n");
  printf("\n\n\n");

/*  Get data set year.  */
  printf("Please enter the data set year in YYYY form.   ");
  scanf("%s",teststring);
  label_year = atoi(teststring);    /* data set year (four digits form) */
  label_year = label_year - 1900;

  printf("\n\n");

/*  Get data set month.  */
  printf("Please enter the data set month in MM form.   ");
  scanf("%s",teststring);
  label_month = atoi(teststring);   /* data set month (two digits form) */
 
  (void) Check_data_date(label_year,label_month);   /* call to function */

  printf("\n\n");

/*  Get data set filename.  */
  printf("Enter the data set file name.  \n");
  printf("NOTE:  Data are expected to be in current working directory!\n");
  scanf("%s",filename);

  fptr = fopen (filename,"r");

  if  (fptr == NULL) 
  {
     printf("*************************************************\n");
     printf("* ERROR - Unable to open the data file for this *\n");
     printf("*         SAM II Aerosol Extinction data set of *\n");
     printf("*         %d",label_year);
     if (label_month < 10 )
     {
        printf("0");
     }
     printf("%d.                                 *\n",label_month);
     printf("*         Program has terminated.               *\n");
     printf("*************************************************\n");
     exit ( -1 );
  }


  for (i=0; i < 9; i++)
     param[i] = 0;

  printf("\n\n");
  printf("Parameters available with each data record / event...\n");
  printf("\n");
  printf("1.  Altitude\n");
  printf("2.  Aerosol extinction coefficient, 1/km\n");
  printf("3.  Aerosol extinction coefficient uncertainty, 1/km\n");
  printf("4.  Total extinction ratio, nondimensional\n");
  printf("5.  Total extinction ratio uncertainty, nondimensional\n");
  printf("6.  NMC temperature in degrees Kelvin, interpolated to altitude\n");
  printf("    from 18 standard pressure levels\n");
  printf("7.  NMC temperature uncertainty in degrees Kelvin, interpolated\n");
  printf("    to altitude from 18 standard pressure levels\n");
  printf("8.  NMC pressure, mb, interpolated to altitude from 18 standard\n");
  printf("    pressure levels\n");

  printf("\n");
  printf("How many parameters would you like to have printed out with\n");
  printf("each record / event?  ");
  scanf("%s",teststring);

  amount = atoi(teststring);

  printf("\n\n");

  if (amount == 8)
  {
     for (i=1; i<= 8; i++)
        param[i] = i;
  }
  else
  {
     for (i=1; i <= amount; i++)
     {
        printf("Enter parameter #%d.   ",i);
        scanf("%s",teststring);
        param[i] = atoi(teststring);
        printf("\n");
     }
  }

  printf("\n\n");
  printf("There are data values for altitudes from 4 km to 39 km.\n");
  printf("How many altitudes would you like to print out parameters to \n");
  printf("the screen?  If you wish to print out parameters for all\n");
  printf("altitudes through 39 km, enter 39.   \n");
  scanf("%s",teststring);
  altamount = atoi(teststring);

  printf("\n\n");

  for (i=0; i < 40; i++)
    altitude[i] = 0;

  if (altamount == 39)
  {
     for (i=4; i <= 39; i++)
        altitude[i] = i;
  }
  else
  {
     for (i=1; i <= altamount; i++)
     {
        printf("NOTE: Altitude range must be entered in increasing order!\n");
        printf("Please enter altitude #%d (range 4 km to 39 km).  ",i);
        scanf("%s",teststring);
        j = atoi(teststring) ;
        altitude[j] = j;
        if (altitude[j] < 4)
        {
           printf("\n");
           printf("****************************************************\n");
           printf("* ERROR - Altitude range is between 4 km to 39 km. *\n");
           printf("*         Program has terminated.                  *\n");
           printf("****************************************************\n");
           exit ( -1 );
        }
        printf("\n");
     }
  }

  for (i=0; i<HEADER; i++)       /* Read and print first 44 lines of data file */
  {
     fread((char *) &buf[0], sizeof(unsigned char), SIZE, fptr);
     printf("%s",buf);
  }

  while ((feof(fptr)) == 0)
  {
      gmt = 0.0;                /* initialize record variables */
      year = 0.0;
      month = 0.0;
      day = 0.0;
      hour = 0.0;
      minute = 0.0;
      seconds = 0.0;
      event = 0.0;
      orbit = 0.0;
      lat = 0.0;
      lon = 0.0;
      trop = 0.0;
      height = 0.0;

      fscanf(fptr,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
             &gmt,&year,&month,&day,&hour,&minute,&seconds,&event,
             &orbit,&lat,&lon,&trop,&height);

      if (feof(fptr) != 0)
      {
         printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
         printf("~ End of file has been reached with %d records ~\n",rec_count);
         printf("~ read in from the current SAM II data file.     ~\n");
         printf("~ Program has completed successfully!            ~\n");
         printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
         exit ( 0 );
      }
  
      rec_count++;            /*  increment record count  */

      printf("\n\n");
      printf("%.4lf %.1lf %.1lf %.1lf %.1lf %.1lf %.1lf %.2lf %.2lf %.2lf %.2lf %12.5e %12.5e\n",
            gmt,year,month,day,hour,minute,seconds,event,orbit,lat,lon,trop,height);

      /*  Perform error checking on each parameter  */
      (void) Check_year (year, label_year, rec_count);  /* call to function */
      (void) Check_month (month, label_month, rec_count);  /* call to function */

      for (i=4; i<=39; i++)    /* Read data values at each altitude */
      {
         /*  initialize data variables */
         data[1] = 0.0;
         data[2] = 0.0;
         data[3] = 0.0;
         data[4] = 0.0;
         data[5] = 0.0;
         data[6] = 0.0;
         data[7] = 0.0;
         data[8] = 0.0;

         /*  read in data variables */
         fscanf(fptr,"%lf  %lf  %lf  %lf  %lf  %lf  %lf  %lf",
                &data[1],&data[2],&data[3],&data[4],&data[5],&data[6],
                &data[7],&data[8]);
         if (altitude[i] != 0)
         {
             if (altitude[i] == i)
             {
                for (j = 1; j <= amount; j++)
                {
                   if (param[j] != 0)
                     printf("%12.5e ",data[param[j]]);
                }
                printf("\n");
             }
             else
              ;
         }


      }       /*  End of for loop  */
   }      /*  End of main while loop  */
}    /*  End of main routine */



/***********************************************************
*                                                          *
*  FUNCTION:  Check_data_date                              *
*                                                          *
*  PURPOSE:  Verify that the data date requested by the    *
*            user falls in the range of October 1978       *
*            through December 1993.  However, there are    *
*            no data files for the months of February 1993 *
*            through October 1993.                         *
*                                                          *
*  ALGORITHM:  None.                                       *
*                                                          *
*  INVOCATION:                                             *
*       (void) Check_data_date (label_year, label_month);  *
*    WHERE:                                                *
*       <label_year> - requested data year in two digit    *
*                      form                                *
*       <label_month> - requested data month in two digit  *
*                       form                               *
*                                                          *
*  FILE/RECORD REFERENCES:  None.                          *
*                                                          *
*  EXTERNAL ROUTINES:  None.                               *
*                                                          *
*  INTERNAL ROUTINES:  None.                               *
*                                                          *
*  NOTES:  If the function finds an error, the code will   *
*          print an error message to the screen and will   *
*          terminate.                                      *
*                                                          *
************************************************************/
void Check_data_date (label_year, label_month)
int label_year, label_month;
{
   if (label_year < 78)
   {
      printf("*********************************************\n");
      printf("* ERROR - There was no SAM II data produced *\n");
      printf("*         before the year 1978.             *\n");
      printf("*         Program has terminated.           *\n");
      printf("*********************************************\n");
      exit ( -1 );
   }
   else if (label_year > 93)
   {
      printf("*********************************************\n");
      printf("* ERROR - There was no SAM II data produced *\n");
      printf("*         after the year 1993.              *\n");
      printf("*         Program has terminated.           *\n");
      printf("*********************************************\n");
      exit ( -1 );
   }
   else if (label_year == 78)
   {
      if (label_month < 10)
      {
         printf("****************************************************\n");
         printf("* ERROR - There was no SAM II data produced before *\n");
         printf("*         October 1978 (7810).                     *\n");
         printf("*         Program has terminated.                  *\n");
         printf("****************************************************\n");
         exit ( -1 );
      }
   }
   else if (label_year == 93)
   {
      if ((label_month == 1) || (label_month == 11) || (label_month == 12))
         ;
      else
      {
         printf("**************************************************\n");
         printf("* ERROR - There was no SAM II data produced from *\n");
         printf("*         9302 thru 9310.                        *\n");
         printf("*         Program has terminated.                *\n");
         printf("**************************************************\n");
         exit ( -1 );
      }
   }
}   /*  End of function, Check_data_date  */

/***********************************************************
*                                                          *
*  FUNCTION:  Check_year                                   *
*                                                          *
*  PURPOSE:  To verify that the year as read in from the   *
*            data record matches that of what was entered  *
*            in by the user.  If it does not, an           *
*            error message will be printed to the screen   *
*            and terminate the program.                    *
*                                                          *
*  ALGORITHM:  None.                                       *
*                                                          *
*  INVOCATION:                                             *
*         (void) Check_year (year, label_year, rec_count); *
*    WHERE:                                                *
*         <year> - data year from input data file.         *
*         <label_year> - as passed in from the user.       *
*         <rec_count> - contains which record year is      *
*                       located.                           *
*                                                          *
*  FILE/RECORD REFERENCES:  None.                          *
*                                                          *
*  EXTERNAL ROUTINES:  None.                               *
*                                                          *
*  INTERNAL ROUTINES:  None.                               *
*                                                          *
*  NOTES:  None.                                           *
*                                                          *
************************************************************/
void Check_year (year, label_year, rec_count)
double year;
int label_year,
    rec_count;
{
   if (year != ((double)label_year))
   {
      printf("%.1lf %.1lf\n",year,(double)label_year);
      printf("***********************************************\n");
      printf("* ERROR - The year read in from this data set *\n");
      printf("*         does not match by what was entered  *\n");
      printf("*         in by the user.  This was found at  *\n");
      printf("*         record %d.                        *\n",rec_count);
      printf("*         Program has terminated.             *\n");
      printf("***********************************************\n");
/*    exit ( -1 );   */
   }
}    /*  End of function, Check_year   */

/***********************************************************
*                                                          *
*  FUNCTION:  Check_month                                  *
*                                                          *
*  PURPOSE:  To verify that the month as read in from the  *
*            data record matches that of what was entered  *
*            in by the user.  If it does not, an           *
*            error message will be printed to the screen   *
*            and terminate the program.                    *
*                                                          *
*  ALGORITHM:  None.                                       *
*                                                          *
*  INVOCATION:                                             *
*      (void) Check_month (month, label_month, rec_count); *
*    WHERE:                                                *
*         <month> - data month from input data file.       *
*         <label_month> - month as passed in from the user.*
*         <rec_count> - contains which record month is     *
*                       located.                           *
*                                                          *
*  FILE/RECORD REFERENCES:  None.                          *
*                                                          *
*  EXTERNAL ROUTINES:  None.                               *
*                                                          *
*  INTERNAL ROUTINES:  None.                               *
*                                                          *
*  NOTES:  None.                                           *
*                                                          *
************************************************************/
void Check_month (month, label_month, rec_count)
double month;
int label_month,
    rec_count;
{
   if (month != ((double)label_month))
   {
      printf("%.1lf\n",month);
      printf("************************************************\n");
      printf("* ERROR - The month read in from this data set *\n");
      printf("*         does not match by what was entered   *\n");
      printf("*         in by the user.  This was found at   *\n");
      printf("*         record %d.                         *\n",rec_count);
      printf("*         Program has terminated.              *\n");
      printf("************************************************\n");
/*    exit ( -1 );   */
   }
}    /*  End of function, Check_month   */
