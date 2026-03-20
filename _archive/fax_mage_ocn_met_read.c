/************************************************************************************
* Program - fax_mage_ocn_met.c                                                      *
*                                                                                   *
* AUTHOR: Mandira Mukherjee                                                         *
*                                                                                   *
* Date: 11/22/94                                                                    *
* PURPOSE/DESCRIPTION: The purpose of the program is to produce a  Read Software    *
*                      for FIRE_AX_MAGE_OCEANUS data set. The data files are        *
*                      in ASCII format. The program displays the parameters in the  *
*                      data set and requests the user for the desired input. The    *
*                      output is printed to the screen , if desired the output can  *
*                      be redirected to a file.                                     *
* FIRE_AX_MAGE_OCEANUS:                                                             *
*                      The Oceanus data set, comprising output from standard ship   *
*                      sensors and the standard IMET package, was retrieved as part *
*                      of the ASTEX MAGE field experiment performed in a region of  *
*                      the eastern Atlantic extending southeast from Santa Maria    *
*                     ( 37 deg N, 25 deg W) in the Azores to Porto Santo ( 33 deg N,*
*                      16 deg W)a small island just north of Madeira, during June of*
*                       1992. Study of low-level cloud conditions ranging from solid*
*                      stratocumulus decks to broken trade cumulus, suitable sites  *
*                      for surface measurement and aircraft operations, and the opp-*
*                      ortunity to coordinate with the oceanographic investigation  *
*                      SARE. Spatial coverage is 40 N to 37N; 24W to 21W.           *
*                      Temporal Coverage is 5/26/92 - 6/23/92.                      *
*                          The first 22 lines in the program prints out the informa *
*                      tion of the data and then the next line follows the data.    *
*                      There are 21 parameters in the datafiles and according to the*
*                      requested number of the parameter the data will be printed to*
*                      the screen.                                                  *
************************************************************************************/

#include  <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/param.h>
#include <string.h>
#include <limits.h>
#include <math.h>
 
#define  RECSIZE    80
#define  PARAMS    21 
 
main (argc,argv)
int argc;
char *argv[];
 
 
 
{
  void Print_heading();              /* prints heading of the data set */
  int Get_par_type();                /* displays the choice of parameters */
  int Get_rec_hd_type();             /* prints the requested header */
  char filename1[MAXPATHLEN+1];      /* filename requested */
  char buf[256];                      /* buffer to read strings at a time */
  char par_type[10],tmp[10],dot;     /* storage of requested parameter */
 
  FILE *fptr;                         /* file pointer */
  int i,rec_count,count,              /* counter */
      param[PARAMS],day,hour;         /* storage of parameter type */
  int col1,col2,col3,col4,col5,col6,col7;      /* variable storage */
  double col8,col9,col10,col11,col12,col13,col14,col15,     /* variable storage */
         col16,col17,col18,col19,col20,col21;               /* variable storage */
 
/****************************************************************************/

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
 Get_rec_hd_type(tmp,par_type,count,param);
 

/*******************************************************************************/

if ((fptr = fopen(filename1,"r"))==NULL)
     {
       printf("***************************************************\n");
       printf("ERROR-UNABLE TO OPEN DATA FILE. PROGRAM HAS TERMINATED\n");
       printf("******************************************************\n");
       exit(-1);
     }
 
 
rec_count = 0;
for (i =0; i< 22 ; i++)
  {
    fgets(buf,80,fptr);
   /* printf("%s\n",buf);*/
  }
while ((feof(fptr)) ==0)
     {
       col1 = 0; col10 = 0.0; col19 = 0.0;
       col2 = 0; col11 = 0.0; col20 = 0.0;
       col3 = 0; col12 = 0.0; col21 = 0.0;
       col4 = 0; col13 = 0.0;
       col5 = 0; col14 = 0.0;
       col6 = 0; col15 = 0.0;
       col7 = 0; col16 = 0.0;
       col8 = 0.0; col17 = 0.0;
       col9 = 0.0; col18 = 0.0;

          fgets(buf,80,fptr);
        sscanf(buf,"%d%c%d%c%d%c%d%c%d%c%d%c%d%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf",&col1,&dot,&col2,&dot,&col3,&dot,
        &col4,&dot,&col5,&dot,&col6,&dot,&col7,&dot,&col8,&dot,&col9,&dot,&col10,&dot,&col11,&dot,&col12,&dot,&col13);
          fgets(buf,80,fptr);
        sscanf(buf,"%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%lf",&col14,&dot,&col15,&dot,&col16,
               &dot,&col17,&dot,&col18,&dot,&col19,&dot,&col20,&dot,&col21);

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
        for ( count = 1; count <= PARAMS;count++)
          {
            switch (param[count -1])
              { 
              case 0:
                 printf(" %d ",col1);
                 break;
               case 1:
                 printf ("  %d",col2);
                 break;
               case 2:
                 printf (" %d",col3);
                 break;
               case 3:
                 printf (" %d", col4);
                 break;
               case 4:
                 printf (" %d",col5);
                 break;
               case 5:
                 printf (" %d",col6);
                 break;
               case 6:
                 printf (" %d",col7);
                 break;
               case 7:
                 printf ("  %.3lf",col8);
                 break;
               case 8:
                 printf ("  %.3lf",col9);
                 break;
               case 9:
                 printf ("  %.1lf",col10);
                 break;
               case 10:
                 printf ("  %.3lf",col11);
                 break;
               case 11:
                 printf ("   %.1lf",col12);
                 break;
               case 12:
                 printf ("   %.1lf\n",col13);
                 break;
               case 13:
                 printf ("   %.1lf",col14);
                 break;
               case 14:
                 printf ("   %.1lf",col15);
                 break;
               case 15:
                 printf ("   %.1lf",col16);
                 break;
               case 16:
                 printf ("   %.3lf",col17);
                 break;
               case 17:
                 printf ("   %.3lf",col18);
                 break;
               case 18:
                 printf ("   %.3lf",col19);
                 break;
               case 19:
                 printf ("   %.4lf",col20);
                 break;
               case 20:
                 printf ("   %.4lf",col21);
                 break;
             }  /* End of Switch */
         }  /*End of for */
     printf ("\n");
     } /* End of while */
 fclose(fptr);
 
}  /* End of main */
 
/*******************************************************************************
*                                                                               *
* FILE: Print_heading                                                           *
*                                                                               *
* AUTHOR: Mandira Mukherjee                                                     *
*                                                                               *
* PURPOSE/DESCRIPTION: Prints the heading for the FIRE_AX_MAGE_OCEANUS data set.*
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
           printf("*                 FIRE_AX_MAGE_OCEANUS                    *\n");
           printf("*                     Read Program                        *\n");
           printf("*                                                         *\n");
           printf("* Version 1.00                       September 30, 1994   *\n");
           printf("*                                                         *\n");
           printf("***********************************************************\n");
           printf("\n");
     } /* End of Printing */
 
/*********************************************************************************
**                                                                               *
* FILE: Get_par_type                                                             *
*                                                                                *
* AUTHOR: Mandira Mukherjee.                                                     *
*                                                                                *
* PURPOSE/DESCRIPTION:                                                           *
*                     There are 21 parameters in FIRE_AX_MAGE_OCEANUS   data set.*
*                     They are year, Month, Day,Hour, Minute, Second, GPS time,  *
*                     GPS lat, GPS lon, Sperry Gyro Output, Air temp, Barometric *
*                     pressure, Precipitation, UV, Wind Direction, Wind Speed,   *
*                     LRN lat, LRN lon, Ship Speed, Sea water Surface Conductivi *
*                     ty, SST.                                                   *
*                                                                                *
* INVOCATION:                                                                    *
*                     (void) Get_par_type                                        *
*                                                                                *
* INTERNALROUTINES:                                                              *
*                  NONE:                                                         *
*********************************************************************************/

int Get_par_type(tmp,par_type,count,param)
 
int count,param[25];
char *par_type,*tmp;
 {
                 char par[15]; /* Temp var to store type */
                 int pe;
        printf("\n");
        printf("There are 21 parameters of information that were \n");
        printf("collected at each event. These parameters are: \n");
        printf("\n");
        printf("    NO.             Parameter                             Units\n");
        printf("  -----            -----------                            -------       \n");
        printf("    1.              Year                                  number         \n");
        printf("    2.              Month                                 number         \n");
        printf("    3.              Day                                   day            \n");
        printf("    4.              Hour                                  hour           \n");
        printf("    5.              Minute                                minute         \n");
        printf("    6.              Second                                second         \n");
        printf("    7.              GPS-Time                              hhmmss(Z)      \n");
        printf("    8.              GPS-Lat                               Deg N          \n");
        printf("    9.              GPS-Lon                               Deg W          \n");
        printf("   10.              Sperry Gyro Output                    Degrees        \n");
        printf("   11.              Air Temperature                       Degrees C      \n");
        printf("   12.              Barometric Pressure                   mb             \n");
        printf("   13.              Precipitation                         millimeters    \n");
        printf("   14.              Short Wave Radiation (UV)             watts/sq meters\n");
        printf("   15.              Wind Direction                        Degrees        \n");
        printf("   16.              Wind Speed                            meters/second  \n");
        printf("   17.              LRN-Lat                               Deg N          \n");
        printf("   18.              LRN-Lon                               Deg W          \n");
        printf("   19.              Ship Speed                            Knots          \n");
        printf("   20.              Sea Water Surface Conductivity        S/meter        \n");
        printf("   21.              Sea Surface Temperature               Degrees C      \n");

        printf("\n\n");
 
        par_type[0]= 'q';
while ((par_type[0]!='a') && (par_type[0] != 'A') && (par_type[0] != 's') && (par_type[0]!= 'S') && (par_type[0] != 'X')
 && (par_type[0] != 'x'))
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
                                printf(" THE TOTAL PARAMETERS AVAILABLE ARE 1 THRU 21\n");
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
 
/*********************************************************************************
*FILE: Get_rec_hd_type                                                           *
*                                                                                *
* AUTHOR: Mandira Mukherjee.                                                     *
*                                                                                *
* PURPOSE/DESCRIPTION:                                                           *
*                        There are 21 parameters of information collected at     *
*                        each event. They are:                                   *
*                         
* INVOCATION:                                                                    *
*                     (void) Get_rec_hd_type                                     *
*                                                                                *
* INTERNALROUTINES:                                                              *
*                  NONE:                                                         *
*********************************************************************************/


int Get_rec_hd_type(tmp,par_type,count,param)
 
int count,param[PARAMS];
char *par_type,*tmp;
 
  {
    count = 0;
 
      for ( count = 1; count <= PARAMS; count++)
        {
          switch (param[count -1])
             {
               case 0:
                 printf("Year ");
                 break;
               case 1:
                 printf("Month ");
                 break;
               case 2:
                 printf("Day ");
                 break;
               case 3:
                 printf("Hour ");
                 break;
               case 4:
                 printf("Minute ");
                 break;
               case 5:
                 printf("Second ");
                 break;
               case 6:
                 printf("GPS-Time ");
                 break;
               case 7:
                 printf("GPS-Lat ");
                 break;
               case 8:
                 printf("GPS-Lon ");
                  break;
               case 9:
                 printf("Sp.GY.out ");
                 break;
               case 10:
                 printf("Air temp ");
                 break;
               case 11:
                 printf("Barom. Pres ");
                 break;
               case 12:
                 printf("Precipitation ");
                 break;
               case 13:
                 printf("UV ");
                 break;
               case 14:
                 printf("Wind dir. ");
                 break;
               case 15:
                 printf("Wind spd ");
                 break;
               case 16:
                 printf("LRN-Lat ");
                 break;
               case 17:
                 printf("LRN-Lon ");
                 break;
               case 18:
                 printf("Ship spd ");
                 break;
               case 19:
                 printf("SWSC ");
                 break;
               case 20:
                 printf("SST");
                 break;
                 default:
                 break;
              } /* End of Switch */
         }    /* End of for */
  }    /* End of Get_rec_hd_type function */
 
/***********************************************************************************/
 
                 
 
