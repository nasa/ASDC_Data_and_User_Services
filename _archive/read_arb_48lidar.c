/************************************************************************
*                                                                       *
* PROGRAM:   read_arb_48lidar.c                                         *
*                                                                       *
* VERSION:   1.0                       DATE:  May 30, 1996              *
* VERSION:   2.0                       DATE:  January 31, 1999          *
*                                                                       *
* PURPOSE:   The purpose of this code is to output the ARB 48 inch      *
*            aerosol lidar data set as requested by the user.           *
*                                                                       *
* ALGORITHM: None.                                                      *
*                                                                       *
* INVOCATION:    read_arb48lidar                                        *
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
* NOTES: There are three parameters of in this data set; altitude,      *
*        scattering ratio, and aerosol backscattering coefficient.      *
*                                                                       *
*        The user will be presented all of the days of data that were   *
*        collected for the year.  From there, the user will be able to  *
*        select in which order he/she would like the parameters         *
*        written to the standard output.  In this case, it will be to   *
*        the screen.                                                    *
*                                                                       *
*        The parameters for this data set are as follows:               *
*        Parameter        Minimum        Maximum         Format         *
*        ---------        -------        -------         ------         *
*        Altitude          0.00           33.0            Real          *
*        Scattering                                                     *
*             Ratio       -1000.0        1000.0           Real          *
*        Aerosol                                                        *
*             Coefficient  -0.1           0.1             Real          *
*                                                                       *
*        There is a fill value for all of the parameters is 9999.9.     *
*                                                                       *
* QUESTIONS: If you have any questions, please contact:                 *
*            EOSDIS Langley DAAC                                        *
*            Science, Users and Data Services                           *
*            NASA Langley Research Center                               *
*            Mail Stop 157D                                             *
*            2 South Wright Street                                      *
*            Hampton, Virginia 23681-2199                               *
*            U.S.A.                                                     *
*                                                                       *
*            Telephone: (757) 864-8656                                  *
*            FAX:       (757) 864-8807                                  *
*            E-mail:    support-asdc@earthdata.nasa.gov                 *
*                                                                       *
* REVISION INFORMATION:                                                 *
* 01-31-1999 Updated code with new address change wording of some code. *
*                                                                       *
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  SIZE   45        /* Number of characters on a line */


main(argc,argv)
int argc;
char *argv[];
{

  void Greetings();            /* Declare function */

  unsigned char   buf[SIZE],  /* String used to read in data */
                  temp_buf[SIZE];

  char     filename[30],      /* Input file name */
           temp_name[30];     /* Input temporary name */

  FILE     *fptr;             /* Input file pointer */

  int      amount,            /* Number of parameters selected */
           count,             /* Counter */
           day,               /* Data collection day */
           flag,              /* Loop flag */
           num_alts,          /* Total number of altitudes for each day */
           param[3],          /* Stores selection order */
           param_count;       /* Counter */

  double   alt,               /* Stores Altitude value for each day */
           backscat,          /* Stores Backscattering for each altitude */
           coeff,             /* Stores Coefficient value for each day */
           irr,               /* Solar Irradiance */
           scatt,             /* Stores Scattering value for each day */
           std;               /* Standard Deviation */


          /*************************************/
          /*                                   */
          /*       Begin Read Program          */
          /*                                   */
          /*************************************/

  (void) Greetings();

  /* Initialize array */
  for (count = 1; count <= 3; count++)
     param[count-1] = -1;

  /* Prompt for file name */
  if (argc == 1)
  {
     printf("Please enter the input data file name. Include\n");
     printf("the entire path. --> ");
     scanf("%s",temp_name);
     strcpy(filename,temp_name);
  }
  else if (argc != 0)  /* File name included at command line */
  {
    strcpy(filename,argv[1]);
  }

  /* Open input file name */
  fptr = fopen(filename,"r");

  if (fptr == NULL)
  {
     printf("*****************************************\n");
     printf("* ERROR - Unable to open the data file. *\n");
     printf("*         Program has terminated.       *\n");
     printf("*****************************************\n");
     printf("\n");
     exit ( -1 );
  }

  printf("\n\nThe filename for this input data file is %s.\n",filename);


  /* Loop flag set to off */
  flag = 0;
  
  while (flag == 0)
  {
  printf("\n\n");
  printf("There are three parameters of information that were\n");
  printf("collected at each event.  These parameters are:\n");
  printf("\n");
  printf("    No.      Parameter                                Units\n");
  printf("    ---      ---------                                -----\n");
  printf("     1.      Altitude                                Kilometers\n");
  printf("     2.      Scattering Ratio                        Unitless\n");
  printf("     3.      Aerosol Backscattering Coefficient      1/(km-sr)\n");
  printf("\n\n");

  printf("Enter 'a' or 'A' to output all parameters in the above format.\n");
  printf("Enter 'x' or 'X' to eXit this program.\n");
  printf("Enter the total number of parameters to output (1-3). --> ");
  scanf("%s",temp_name);

  if ((strcmp("a",temp_name)) == 0)
  {
     for (count = 0; count < 3; count++)
        param[count] = count+1;
     amount = 3;
     flag = 1;
  }
  else if ((strcmp("A",temp_name)) == 0)
  {
     for (count = 0; count < 3; count++)
        param[count] = count+1;
     amount = 3;
     flag = 1;
  }
  else if ((strcmp("x",temp_name)) == 0)
  {
     printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
     printf("~ You have requested to exit this program.  ~\n");
     printf("~ This read program has ended.              ~\n");
     printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
     exit(0);
  }
  else if ((strcmp("X",temp_name)) == 0)
  {
     printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
     printf("~ You have requested to exit this program.  ~\n");
     printf("~ This read program has ended.              ~\n");
     printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
     exit(0);
  }
  else 
  {
     amount = atoi(temp_name);
     if ((amount < 1) || (amount > 3))
     {
        printf("The amount should be between 1-3.\n");
        printf("Please try again.\n\n");
        flag = 0;
     }
     else
        flag = 2;
  }
} /* End of while */

  printf("\n\n");

  if (flag == 2)
  {
     for (count = 0; count < amount; count++)
     {
        flag = 2;
        while (flag == 2)
        {
         printf("Enter parameter number %1d --> ",count+1);
         scanf("%s",temp_name);
         if ((strcmp("x",temp_name)) == 0)
         {
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printf("~ You have requested to exit this program.  ~\n");
            printf("~ This read program has ended.              ~\n");
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            exit(0);
         }
         else if ((strcmp("X",temp_name)) == 0)
         {
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printf("~ You have requested to exit this program.  ~\n");
            printf("~ This read program has ended.              ~\n");
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            exit(0);
         }
         else
         {
            param[count] = atoi(temp_name);
            if ((param[count] >= 1) && (param[count] <= 3))
               flag = 0;
            else
            { 
               printf("The amount should be between 1-3.\n");
               printf("Please try again.\n\n");
               flag = 2;
            }
          }
         } /* End of while */
     } /* End of for */
  } /* End of if */

  printf("\n");

  fgets((char *) &buf[0], SIZE, fptr);

  while ((feof(fptr)) == 0)
  {

     /* Code searches for string that starts with the word DATA. */
     /* This states that a new day of data is available in this  */
     /* this file.                                               */

     if (buf[0] == 'D')
     {
       fscanf(fptr,"%d %d %lf",&day,&num_alts,&backscat);
       printf("\n\n");
       printf("DATE:  %d\n",day);
       printf("Total number of Altitudes: %d \n",num_alts);
       printf("Integrated Backscattered: %lf\n\n",backscat);

       for (count = 1; count <= 5; count++)
          fscanf(fptr,"%s",temp_buf);


       for (count = 1; count <= num_alts; count++)
       {
         fscanf(fptr,"%lf %lf %lf",&alt,&scatt,&coeff);

         for (param_count = 0; param_count < amount; param_count++)
         {
          if (param[param_count] == 1)
              printf("%lf ",alt);
          else if (param[param_count] == 2)
              printf("%lf ",scatt);
          else if (param[param_count] == 3)
              printf("%.10lf ",coeff);
         } /* end of for param_count loop */

          printf("\n");

       } /* end of for count loop */

     } /* end of if buf[0] = 'D' loop */

     fgets((char *) &buf[0], SIZE, fptr);

   } /* end of while loop */

   exit(0);
}   /* End of main function */

/****************************************************************************
*                                                                           *
* FUNCTION:  Greetings                                                      *
*                                                                           *
* PURPOSE:   To print a banner at the beginning of the execution to inform  *
*            the users what version of this read program he or she is       *
*            working with.                                                  *
*                                                                           *
****************************************************************************/
void Greetings()
{   /* Begin Greetings function */
   printf("\n\n\n");
   printf("   *****************************************************\n");
   printf("   *                                                   *\n");
   printf("   *             ARB 48 inch Aerosol Lidar             *\n");
   printf("   *                   Read Program                    *\n");
   printf("   *                                                   *\n");
   printf("   *                                                   *\n");
   printf("   *  Version 2.0                     January 31, 1999 *\n");
   printf("   *****************************************************\n");
   printf("\n");
}   /* End of Greetings function */
