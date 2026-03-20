
/************************************************************************************
*                                                                                   *
* Program - nav_suroit.c                                                            *
*                                                                                   *
* AUTHOR: Mandira Mukherjee                                                         *
*                                                                                   *
* Date: 10/14/94                                                                    *
*                                                                                   *
* PURPOSE/DESCRIPTION: The purpose of the program is to read the SOFIA-SUROIT_NAV   *
*                      FIRE_ASTEX_NAT data file. The data file contains 11 parameters*
*                                                                                   *
************************************************************************************/

#include  <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/param.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define  RECSIZE    80
#define  PARAMS     11

main (argc,argv)
int argc;
char *argv[];



{
  void Print_heading();              /* prints the heading of the data set */
  int Get_par_type();                /* displays the choice of parameters */
  int Get_hd_rec_type();             /* prints the requested header */
  char filename1[MAXPATHLEN+1];      /* name of the file as first argv */
  char buf[256];                     /* buffer to read strings at a time */
  char par_type[10],tmp[10];         /* storage of requested parameter */
 
  FILE *fptr;                         /* file pointer */
  int i,rec_count,count,             /* counters */
      param[PARAMS],                 /* array of requested parameters */
      day,hour;
  double col3,col4,col8,col9,col10,col11;
  int col1,col2,col5,col6,col7;           /* varibles 1 thru 11 */

/*********************************************************************************/


void Print_heading();

for (count = 0;count < PARAMS; count++)
  {
        param[count] = -99;
  }
(void) Print_heading();
 
if (argc == 1)
  {
    printf("Please enter the datafile name:     ");
    scanf("%s",filename1);
  }
else
  {
    strcpy (filename1,argv[1]);
  }
 printf ("\n\n");
 printf("The datafile to be read is %s \n", filename1);
 

Get_par_type(tmp,par_type,count,param);
Get_hd_rec_type(tmp,par_type,count,param);

/*************************************************************************/


if ((fptr = fopen(filename1,"r"))==NULL)
     {
       printf("***************************************************\n");
       printf("ERROR-UNABLE TO OPEN DATA FILE. PROGRAM HAS TERMINATED\n");
       printf("******************************************************\n");
       exit(-1);
     }
  

rec_count = 0;

for (i= 0;i < 5 ; i++)
   {
     fgets(buf,80,fptr);
   } 

while ((feof(fptr)) ==0)
     {
       col1 = 0;  col5 = 0; col10 = 0.0;
       col2 = 0;  col6 = 0; col11 = 0.0;
       col3 = 0.0; col7 =0;
       col4 = 0.0; col8 = 0.0;
       col9 = 0.0;

       fgets(buf,80,fptr);
       sscanf(buf,"%d%d%lf%lf%d%d%d%lf%lf%lf%lf",&col1,&col2,&col3,&col4,
              &col5,&col6,&col7,&col8,&col9,&col10,&col11);
       printf("\n");
           if ((feof(fptr)!=0))
             {
               printf("\n\n\n");
               printf("----------------------------------------------------\n");
               printf("*THE END OF FILE HAS BEEN REACHED........           \n");
               printf(" THIS IS A NORMAL TERMINATION...........            \n");
               printf(" THE TOTAL NUMBER OF RECORDS: %d\n", rec_count);    
               printf("-----------------------------------------------------\n");
               exit(0);
             }
       ++rec_count;

      for ( count = 1; count <= PARAMS; count++)
        {
          switch (param[count -1])
             {
               case 0:
                 printf("%d     ",col1);
                 break;
               case 1:
                 printf ("%d    ",col2);
                 break;
               case 2:
                 printf ("%.1lf   ",col3);
                 break;
               case 3:
                 printf ("%.1lf   ", col4);
                 break;
               case 4:
                 printf ("%d             ",col5);
                 break;
               case 5:
                 printf ("%d          ",col6);
                 break;
               case 6:
                 printf ("%d        ",col7);
                 break;
               case 7:
                 printf ("%.1lf     ",col8);
                 break;
               case 8:
                 printf ("%.1lf        ",col9);
                 break;
               case 9:
                 printf ("%.1lf      ",col10);
                 break;
               case 10:
                 printf ("%.1lf          ",col11);
                 break; 
             }  /* End of Switch */
         }  /*End of for */
     printf ("\n");
     } /* End of while */
 fclose(fptr);

}  /* End of main */

/********************************************************************************
*                                                                               *
* FILE: Print_heading                                                           *
*                                                                               *
* AUTHOR: Mandira Mukherjee                                                     *
*                                                                               *
* PURPOSE/DESCRIPTION: Prints the heading for the FIRE_ASTEX_NAT_SOFIA program. *
*                                                                               *
* INVOCATION:                                                                   *
*            (void) Print_heading()                                             *
*                                                                               *
* INTERNAL ROUTINES:                                                            *
*                    NONE.                                                      *
*                                                                               *
* EXTERNAL ROUTINES:                                                            *
*                    NONE                                                       *
*                                                                               *
*********************************************************************************/

void Print_heading()
    {
           printf("\n\n");
           printf("***********************************************************\n");
           printf("*                                                         *\n");
           printf("*       FIRE_ASTEX_NAT_SOFIA_SUROIT_NAVIGATION            *\n");
           printf("*                     Read Program                        *\n");
           printf("*                                                         *\n");
           printf("* Version 1.00                       September 30, 1994   *\n");
           printf("*                                                         *\n");
           printf("***********************************************************\n");
           printf("\n");
     } /* End of Printing */

/*********************************************************************************
*                                                                                *
* FILE: Get_par_type                                                             *
*                                                                                *
* AUTHOR: Mandira Mukherjee.                                                     *
*                                                                                *
* PURPOSE/DESCRIPTION:                                                           *
*                There are eleven parameters in FIRE_ASTEX_NAT_SOFIA_SUROIT_MET  *
*                dataset. They are Day, Hour, Lat, Lon, Cloudiness, Wind azimuth,*
*                Speed, air temperature, Pressure, SST and Relative humidity.    * 
*                                                                                *
* INVOCATION:                                                                    *
*                     (void) Get_par_type                                        *
*                                                                                *
* INTERNALROUTINES:                                                              *
*                  NONE:                                                         *
*********************************************************************************/

int Get_par_type(tmp,par_type,count,param)

int count,param[PARAMS]; 
char *par_type,*tmp;
 {
                 char par[15]; /* Temp var to store type */
                 int pe;
        printf("\n");
        printf("There are eleven parameters of information that were \n");
        printf("collected at each event. These parameters are: \n");
        printf("\n");
        printf("    NO.             Parimeter              Units    \n"); 
        printf("  -----            -----------             -------  \n");
        printf("    1.               Day                     day    \n");
        printf("    2.               Hour                    hr     \n");
        printf("    3.               Lat                     deg    \n");
        printf("    4.               Lon                     deg    \n");
        printf("    5.               Cloudiness              unitless       \n");
        printf("    6.               Wind azimuth            deg    \n");
        printf("    7.               Speed                   knots  \n");
        printf("    8.               Air Temperature         C      \n");
        printf("    9.               Pressure                hPa    \n");
        printf("   10.               SST                     C      \n");
        printf("   11.               Relative Humidity       percent      \n");
            
        printf("\n\n");

        /* SELECT PARAMETERS */
 
       par_type[0]= 'q';
       while ((par_type[0]!='a') && (par_type[0] != 'A') && (par_type[0] != 's') && (par_type[0]!= 'S') && (par_type[0]!= 'X') && (par_type[0] != 'x'))
         {
           printf(" Please indicate the number of parameters to be printed \n");
           printf(" Please select: a or A for ALL parameters\n");
           printf(" S or s for some and X or x to exit the program\n\n\n");
           scanf("%s",par_type);
           strcpy(tmp,par_type);
         }
        if ((strcmp(par_type,"a")== 0) || (strcmp(par_type,"A")== 0))
          {
            for (count =0; count <= PARAMS; count++)
               param[count]= count;
          }
         else if ((strcmp(par_type, "s")== 0) || (strcmp(par_type,"S")== 0))
           {
               printf ("**********Parameters selection Requested*************\n");
 
               for (count =1; count <= PARAMS;count++)
                  {
                     printf("Enter q to end selection or X to exit program\n");
                    printf("Please enter parameter #%d-------->",count);
                    scanf("%s",par_type);
                    printf("%s\n",par_type);
                    strcpy(tmp,par_type);
                      if ((strcmp(par_type,"X")==0) || (strcmp(par_type,"x")== 0))
                         {
                            printf ("\n\n");
                            printf("*******EXIT PROGRAM***********\n");
                            exit(-1);
                         }
                       if(((strcmp(par_type,"q")== 0) || (strcmp(par_type,"Q")== 0))&&
                          (count == 1))
                         {
                           printf("**********NO SELECTION OF PARAMETERS**********\n");
                           printf("*********EXIT PROGRAM*************************\n");
                           exit(-1);
                         }

                        if ((strcmp(par_type,"q")== 0) || (strcmp(par_type,"Q")== 0))
                         {
                           break;
                         }
                            if ((atoi(tmp) > PARAMS) || (atoi(tmp) <= 0))
                              {
                                printf("\n");
                                printf(" THE TOTAL PARAMETERS AVAILABLE ARE 1 THRU 11\n");
                                printf(" THE REQUESTED PARAMETER IS OUT OF RANGE\n");
                                strcpy(tmp,"   ");
                               }
                          pe = atoi(par_type);
                        param[count-1]= pe -1;
                     }    /*end of for statement */
               }         /* end of else if */
         else if ((strcmp(par_type,"x")== 0) || (strcmp(par_type,"X")== 0))
                  {
                     printf ("\n\n");
                     printf("****************EXIT PROGAM*****************\n");
                     exit(0);
                  }
 
   } /* end of the get_type function */

 

/*********************************************************************************/
       
int Get_hd_rec_type(tmp,par_type,count,param)

int count,param[PARAMS];
char *par_type,*tmp;
  
    {
    count = 0;
 
      for ( count = 1; count <= PARAMS; count++)
        {
          switch (param[count -1])
             {
               case 0:
                 printf("DAY   ");
                 break;
               case 1:
                 printf ("HR   ");
                 break;
               case 2:
                 printf ("LAT    ");
                 break;
               case 3:
                 printf ("LON   ");
                 break;
               case 4:
                 printf ("CLOUD   ");
                 break;
               case 5:
                 printf ("WIND AZIMUTH      ");
                 break;
               case 6:
                 printf ("SPEED    ");
                 break;
               case 7:
                 printf("AIR TEMP   ");
                 break;
               case 8:
                 printf ("PRESSURE      ");
                 break;
               case 9:
                 printf ("SST      ");
                 break;
               case 10:
                 printf ("HUMIDITY  ");
                 break;
                 default:
                 break;
             }  /* End of Switch */
         }  /*End of for */
    }  /* End of Get_rec_hd_type function */
 
/***************************************************************************************/
 
 
               
