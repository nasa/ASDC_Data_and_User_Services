/************************************************************************************
* Program - rd_ax_tetroon.c                                                         *
*                                                                                   *
* AUTHOR: Mandira Mukherjee                                                         *
*                                                                                   *
* Date: 11/22/94                                                                    *
* PURPOSE/DESCRIPTION: The purpose of the program is to produce a  Read Software    *
*                      for FIRE_ASTEX_NAT_MAGE_TETROON data set. The data files are *
*                      in ASCII format. The program displays the parameters in the  *
*                      data set and requests the user for the desired input. The    *
*                      output is printed to the screen , if desired the output can  *
*                      be redirected to a file.                                     *
* FIRE_ASTEX_NAT_MAGE_TETROON:                                                      *
*                     This is the Tetroon location type of data. First and second   *
*                     Lagrangian Experiments June 12 - 13 , June 18 - 20 1992.      *
*                     Filename format is based on lagr(Expt #).(Tetroon #)          *
*                     where (Expt #) is 1 or 2, (Tetroon #) for Lagrangian 1 is     *
*                    [2,4,6,8,12,14,3] and for Lagrangian 2 is [15,7,11,5,13].      *
*                    The program in the first output line prints out the parameter  *
*                    names. Lat and Lon are expressed as the pair (whole degrees,   *
*                    decimal minutes).                                              *
*                    #sats gives the number of GPS satellites available for positio *
*                    ning. Four satellites are necessary determine altitude,        *
*                    otherwise the last available altitude from four satellites     *
*                    is assumed to remain constant, so that the horizontal location *
*                    can be triangulated from three satellites. An altitude is      *
*                   always given in the file, so care should be taken as to its use.*
************************************************************************************/

#include  <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/param.h>
#include <string.h>
#include <limits.h>
#include <math.h>
 
#define  RECSIZE    80
#define  PARAMS     9
 
main (argc,argv)
int argc;
char *argv[];
 
 
 
{
  void Print_heading();               /* prints heading of the data set */
  void Get_par_type();                /* dispplays parameter type */
  char filename1[MAXPATHLEN+1];       /* name of the file requested */
  char buf[256];                      /* buffer to read strings at a time */
  char par_type[10],tmp[10];          /* temporary storage of parameter requested */
 
  FILE *fptr;                         /* file pointer */
  int i,rec_count,count,              /* counter */
      param[PARAMS],                  /* array stores the requested number of parameter */
      day,hour;        
  int col1,col8,col9;                 /* variable storage */
  double col5,col7;                   /* var storage */
  char col2[12],col3[12],col4[5],col6[5],dot;    /* var storage */
 
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
 
/******************************************************************************/

count = 0;
 
              /*Print header of requested Parameters*/
for ( count =1; count <= PARAMS;count++)
    {
       switch (param[count - 1])
        {
          case 0:
           printf("BALLOON ");
           break;
          case 1:
           printf ("dd-mm-yy ");
           break;
          case 2:
           printf("hh:mm:ss ");
           break;
          case 3:
           printf("Lat  ");
           break;
          case 4:
           printf("Lat/dec ");
           break;
          case 5:
           printf("Lon  ");
           break;
          case 6:
           printf("Lon/dec ");
           break;
          case 7:
           printf("Altitude  ");
           break;
          case 8:
           printf("GPS Sat    ");
           break;
           default:
           break;
       }    /* end of case statement */
      }       /* end of for loop */
    printf("\n");

/*******************************************************************************/
if ((fptr = fopen(filename1,"r"))==NULL)
     {
       printf("***************************************************\n");
       printf("ERROR-UNABLE TO OPEN DATA FILE. PROGRAM HAS TERMINATED\n");
       printf("******************************************************\n");
       exit(-1);
     }
 
 
rec_count = 0;
for (i =0; i< 2; i++)
  {
         fgets(buf,80,fptr);
        /* printf("%s\n",buf);*/
  }
while ((feof(fptr)) ==0)
     {
       col1 = 0;
       col5 = 0.0;
       col7 = 0.0;
       col8 = 0;
       col9 =0;
 
 
 fgets(buf,80,fptr);
sscanf(buf,"%d%s%s%s%lf%c%c%s%lf%c%c%d%c%c%d",&col1,col2,col3,col4,&col5,&dot,&dot,col6,&col7,&dot,&dot,&col8,&dot,&dot,&col9);
 /*printf("%d   %s   %s   %s   %.2lf   %s   %.2lf   %d    %d\n",col1,col2,col3,col4,col5,col6,col7,col8,col9);*/
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
                 printf("  %d ",col1);
                 break;
               case 1:
                 printf ("   %s",col2);
                 break;
               case 2:
                 printf ("   %s",col3);
                 break;
               case 3:
                 printf ("   %s", col4);
                 break;
               case 4:
                 printf (" %.2lf",col5);
                 break;
               case 5:
                 printf ("   %s",col6);
                 break;
               case 6:
                 printf ("   %.2lf",col7);
                 break;
               case 7:
                 printf ("   %d",col8);
                 break;
               case 8:
                 printf ("   %d ",col9);
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
* PURPOSE/DESCRIPTION: Prints the heading for the FIRE_AX_MAGE_TETROON data set.*
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
           printf("*                 FIRE_AX_MAGE_TETROON                    *\n");
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
*                     There are nine parameters in FIRE_AX_MAGE_TETROON data set.*
*                     They are Balloon, date/month/year, hour/minute/seconds,    *
*                     Latitude in degrees, latitude, Longitude in degrees, Longi *
*                     tude, Altitude, GPS satellite.                             * 
*                                                                                *
* INVOCATION:                                                                    *
*                     (void) Get_par_type                                        *
*                                                                                *
* INTERNALROUTINES:                                                              *
*                  NONE:                                                         *
*********************************************************************************/

void Get_par_type(tmp,par_type,count,param)
 
int count,param[25];
char *par_type,*tmp;
 {
                 char par[15]; /* Temp var to store type */
                 int pe;
        printf("\n");
        printf("There are five  parameters of information that was \n");
        printf("collected at each event. These parameters are: \n");
        printf("\n");
        printf("    NO.             Parameter                            Units        \n");
        printf("  -----            -----------                           -----        \n");
        printf("    1.              Balloon                              number       \n");
        printf("    2.              dd-mm-yy                             day          \n");
        printf("    3.              hh:mm:ss                             hour/min/sec \n");
        printf("    4.              Latitude (whole degree)              degree       \n");
        printf("    5.              Latitude (minute)                    min          \n");
        printf("    6.              Longitude (whole degree)             degree       \n");
        printf("    7.              Longitude (minute)                   min          \n");
        printf("    8.              Altitude                             meters       \n");
        printf("    9.              GPS Satellite                        number       \n");
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
                                printf(" THE TOTAL PARAMETERS AVAILABLE ARE 1 THRU 9\n");
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
 
