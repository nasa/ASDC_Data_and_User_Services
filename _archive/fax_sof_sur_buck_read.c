
/************************************************************************************
*                                                                                   *
* Program - rd_ax_surbuck.c                                                         *
*                                                                                   *
* AUTHOR: Mandira Mukherjee                                                         *
*                                                                                   *
* Date: 10/14/94                                                                    *
*                                                                                   *
*PURPOSE/DESCRIPTION:                                                               *
*                       The goals of ASTEX were pursued by combining measurements   *
*                       from aircraft, satellites, ships and islands. Surface based *
*                       measurements were made from the island of Santa Maria (in   *
*                       the Azores) and Porto Santo (near Madeira) and four research*
*                      vessels- NOAA's Oceanus and Malcolm Baldrige, the German Ship*
*                       Valdivia and the French Ship Le Suroit. Soundings were made *
*                       from Le Suroit and Oceanus.                                 *
*                                                                                   *
*                       Data provided is from a trailing thermistor with bucket     *
*                       measurements. The thermistor data has been calibrated but   *
*                       not quality controlled. There were 4 sensors deployed, the  *
*                       first was deployed for the whole of the first leg and had   *
*                      drifted by approximately 0.3 degC by the port call but seemed*
*                       stable at this value.   The thermistor was out of the water *
*                       whilst the ship was steaming,  hopefully all this data has  *
*                       been removed from this data set  (but may not all have been *
*                       deleted).(Note provided by Data producer)                   *
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
#define  PARAMS     5

main (argc,argv)
int argc;
char *argv[];



{
  void Print_heading();               /* prints the heading of the data set */
  void Get_par_type();                /* displays the choice of parameters */
  char filename1[MAXPATHLEN+1];       /* name of the file as first argv */
  char buf[256];                      /* buffer to read strings at a time */
  char par_type[10],tmp[10];          /* storage of requested parameter */
 
  FILE *fptr;                         /* file pointer */
  int i,rec_count,count,              /* counter */
      param[PARAMS],                  /* array of requested parameters */
      day,hour;                       /* variable day, hour */
  double jday,bsst,msst;              /* variable julianday,bucket SST, mmetsst */

/*********************************************************************************/


 Print_heading();
for (count = 0;count < PARAMS; count++)
  {
        param[count] = -99;
  }
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

/*****************************Read Dataset and Print Heading*********************/
count = 0;
 
              /*Print header of requested Parameters*/
for ( count =1; count <= PARAMS;count++)
    {
       switch (param[count - 1])
        {
          case 0:
           printf("  DAY   ");
           break;
          case 1:
           printf ("   HOUR   ");
           break;
          case 2:
           printf("  JDAY     ");
           break;
          case 3:
           printf("   BUCKET SST    ");
           break;
          case 4:
           printf("   M MET SST    ");
           break;
           default:
           break;
       }    /* end of case statement */
      }       /* end of for loop */
    printf("\n");
 
/******************************************************************/
 

if ((fptr = fopen(filename1,"r"))==NULL)
     {
       printf("***************************************************\n");
       printf("ERROR-UNABLE TO OPEN DATA FILE. PROGRAM HAS TERMINATED\n");
       printf("******************************************************\n");
       exit(-1);
     }
  
fgets(buf,80,fptr);
/*printf("%s\n",buf);*/

rec_count = 0;

while ((feof(fptr)) ==0)
     {
       day = 0;
       hour = 0;
       jday = 0.0;
       bsst = 0.0;
       msst = 0.0;

       fgets(buf,80,fptr);
       sscanf(buf,"%d%d%lf%lf%lf",&day,&hour,&jday,&bsst,&msst);
     /*  printf("%d   %d  .3lf%  .2lf%  .1lf% \n",day,hour,jday,bsst,msst);
       printf("\n");*/
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
       /*printf ("count = %d",count);
       printf ("amount = %d",amount);*/

      for ( count = 1; count <= PARAMS; count++)
        {
          switch (param[count -1])
             {
               case 0:
                 printf(" %d ",day);
                 break;
               case 1:
                 printf ("      %d",hour);
                 break;
               case 2:
                 printf ("      %.3lf",jday);
                 break;
               case 3:
                 printf ("         %.2lf", bsst);
                 break;
               case 4:
                 printf ("         %.1lf",msst);
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
           printf("*           FIRE_ASTEX_NAT_SOFIA_SUROIT_BUCKET            *\n");
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
*                     There are eight parameters in FIRE_ASTEX_NAT_SOFIA_BALLOON *
*                     dataset.They are hour,Height in m(z), Atmospheric pressure *
*                     in Pmb(P), Air temperature in degree centigrade(ta),       *
*                     Relative humidity in %(H%), Wind speed in m/s (V), Wind    *
*                     direction in degree (dir),specific humidity in g/kg (q) .  *
*                                                                                *
* INVOCATION:                                                                    *
*                     (void) Get_par_type                                        *
*                                                                                *
* INTERNALROUTINES:                                                              *
*                  NONE:                                                         *
*********************************************************************************/

void Get_par_type(tmp,par_type,count,param)

int count,param[10]; 
char *par_type,*tmp;
 {
                 char par[15]; /* Temp var to store type */
                 int pe;
        printf("\n");
        printf("There are five parameters of information that were \n");
        printf("collected at each event. These parameters are: \n");
        printf("\n");
        printf("    NO.             Parameter               Units\n");
        printf("  -----            -----------             -------  \n");
        printf("    1.              Day                     day    \n");
        printf("    2.              Hour                    hour    \n");
        printf("    3.              Julian day              day     \n");
        printf("    4.              Bucket SST              C       \n");
        printf("    5.              MMet SST                C       \n");
        printf("\n\n");
         par_type[0]= 'q';
while ((par_type[0]!='a') && (par_type[0] != 'A') && (par_type[0] != 's') && (par_type[0]!= 'S') && (par_type[0] != 'X') && (par_type[0] != 'x'))
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
                                printf(" THE TOTAL PARAMETERS AVAILABLE ARE 1 THRU 5\n");
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
 
/******************************************************************************************/ 
       


