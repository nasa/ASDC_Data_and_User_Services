
/************************************************************************************
*                                                                                   *
* Program - rd_ax_flt#_10s.c                                                        *
*                                                                                   *
* AUTHOR: Mandira Mukherjee                                                         *
*                                                                                   *
* Date: 10/21/94                                                                    *
*                                                                                   *
* PURPOSE/DESCRIPTION: The purpose of the program is to read the SOFIA-ARAT_10seconds*
*                     FICHER MOYEN data file or ARAT_F27 data file.                 *
*                     Measurement of cloud radiative properties,remote sensing with *
*                     LIDAR , and coordination with ER-2 and C-131 with NOAA-12.    *
*                     Flight involved two orthogonal axes 60nm long at 15,000.00ft  *
*                     centered at 35.40N 23.00W.                                    *
*                                                                                   *
*                     The FOKKER F27 (ARAT) with flux measurement package and the *
*                       airborne Lidar Leandre is used during ASTEX. The FOKKER 27  *
*                       ARAT capabilities are the following:                        *
*                         . Turbulence measurements of wind, temperature and moistu-*
*                           re. Fast response sensors are located on a nose boom 5m *
*                           long, on which are measured :                           *
*                              -attack and sideslip angles by mobile vanes and by a *
*                               five hole probe(Rosemound 858).                     *
*                              -true airspeed by a Pitot probe                      *
*                              - temperature by a fast response "INSU" probe        *
*                              - humidity by a Lyman-alpha humidity meter           *
*                         . Mean state sensors                                      *
*                              -Rosemount temperature probe                         *
*                              -Reverse-flow temperature probe                      *
*                              -General Eastern dew point sensor                    *
*                         . Aerosols and cloud microphysics:                        *
*                              -1-D drop size measurements from 0-6000 microns by   *
*                                four Knollenberg sensors                           *
*                              -2-D sensor OAP 2DC for drop sizes between 25 and 800*
*                               microns.                                            *
*                         . Liquid water content                                    *
*                         .Radiative measurements, up- and downward                 *
*                         . Chemical measurements(isokinetic veins)                 *
*                         .Upward or downward pointing lidar                        *
*                                                                                   *
*                                   There are 60 parameters in ARAT ax_flt#_10sec   *
*                    data files. According to the request of the user the parameters*
*                     will be printed to the screen.                                *
*                                                                                   *
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
#define  PARAMS     60

main (argc,argv)
int argc;
char *argv[];



{
  void Print_heading();               /* prints the heading of the data set */
  int Get_par_type();                 /* displays the choice of parameters */
  int Get_rec_hd_type();              /* prints the requested header for the choosen par*/
  char filename1[MAXPATHLEN+1];       /* name of the file as first argv */
  char buf[256],star[4];              /* buffer to read strings at a time */
  char par_type[10],                  /* storage for requested parameter */
       st[12],dot,st2[12],st3[12],st4[12],col66[12],col67[12],
          col62[12],tmp[10],line[50],arat[12];  /* variables stored in strings */
 
  FILE *fptr;                         /* file pointer */
  int iln,n,i,i1,rec_count,count,     /* counter */
      param[PARAMS],                  /* array for the requested parameter */
      day,hour,num;
  double col1,col2,col3,col4,col5,col6,col8,
             col9,col10,col11,col12,col13,col15,
             col16,col17,col18,col19,col22,col23,
             col24,col25,col26,col27,col29,col30,
             col31,col32,col33,col34,col36,
             col37,col38,col39,col40,col41,
             col43,col44,col45,col46,col47,col48,
             col50,col51,col52,col53,col54,col55,
             col57,col58,col59,col60,col61,
             col64,col65,col68,col69;           /* variables in floating points */
                                          /* column 1 thru 69 skipping each 7th column */

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
Get_rec_hd_type(tmp,par_type,count,param);


/*********************************************************************************/
if ((fptr = fopen(filename1,"r"))==NULL)
     {
       printf("***************************************************\n");
       printf("ERROR-UNABLE TO OPEN DATA FILE. PROGRAM HAS TERMINATED\n");
       printf("******************************************************\n");
       exit(-1);
     }

   fgets(buf,120,fptr);
    /* printf("%s\n",buf);*/
         sscanf(buf,"%s %s",line,arat);
                  if (arat[0] == 'A')        /* reads in the symbol for ARAT F27 FILES */
                     {
                         iln = 87;
                     }
                  else if(arat[0] == 'F')    /* reads in the symbol for FICHIER MOYEN */
                      {
                          iln = 67;
                      }
             rec_count = 0;
                for (i = 0; i< iln;i++)
                     {
                         fgets(buf,120,fptr);
                     }

 while ((feof(fptr)) ==0)

     {
       col1 =0.0;col2 = 0.0;col3 = 0.0;col4 = 0.0;col5 = 0.0;col6 = 0.0;
       col8 = 0.0;col9 = 0.0; col10= 0.0;col11= 0.0;col12=0.0;col13 = 0.0;
       col15 = 0.0;col16 = 0.0;col17 = 0.0;col18=0.0;col19 = 0.0;
       col22=0.0;col23 =0.0;col24 =0.0;col25 =0.0;col26 =0.0;col27 =0.0;
       col29 =0.0;col30 =0.0;col31=0.0;col32 =0.0;col33 =0.0;col34 =0.0;
       col36 =0.0;col37 =0.0;col38=0.0;col39 =0.0;col40 =0.0;col41 =0.0;
       col43 =0.0;col44 =0.0;col45=0.0;col46 =0.0;col47 =0.0;col48 =0.0;
       col50 =0.0;col51 =0.0;col52=0.0;col53 =0.0;col54 =0.0;col55 =0.0;
       col57 =0.0;col58 =0.0;col59=0.0;col60 =0.0;col61 =0.0;
       col64=0.0;col65 =0.0;col68=0.0;col69 =0.0;
       

  fgets(buf,120,fptr);
  /*printf("%s\n",buf);*/
  sscanf(buf,"%2s%d",star,&num);
  for (i1 = 0; i1<31;i1++)
   {                               /*begin for loop*/
       fgets(buf,80,fptr);
        if(i1== 1)
           {                             /*begin first scan*/
              sscanf(buf,"%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%s",&col1,&dot,&col2,&dot,&col3,&dot,&col4,&dot,&col5,&dot,&col6,&dot,st);
     }                              /*end of first scan*/
        else if (i1==4)
           {                        /*begin second scan*/
              sscanf(buf,"%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%s",&col8,&dot,&col9,&dot,&col10,&dot,&col11,&dot,&col12,&dot,&col13,&dot,st);
           }                      /* end of second scan */

    else if (i1 == 7)
          {                            /* begin third scan */
            sscanf(buf,"%lf%c%lf%c%lf%c%lf%c%lf%c%s%s",&col15,&dot,&col16,&dot,&col17,&dot,&col18,&dot,&col19,&dot,st4,st);
          }                            /*end of third scan*/
    else if (i1 == 10)
          {                            /* begin fourth scan*/
            sscanf(buf,"%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%s",&col22,&dot,&col23,&dot,&col24,&dot,&col25,&dot,&col26,&dot,&col27,&dot,st);
          }                            /* end of fourth scan*/
    else if (i1 == 13)
          {
            sscanf(buf,"%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%s",&col29,&dot,&col30,&dot,&col31,&dot,&col32,
&dot,&col33,&dot,&col34,&dot,st);
          }                             /* end of fifth scan*/
    else if (i1 == 16)
          {
                 sscanf(buf,"%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%s",&col36,&dot,&col37,&dot,&col38,&dot,&col39,
&dot,&col40,&dot,&col41,&dot,st);
          }                             /* end of sixth scan*/
    else if (i1 == 19)
          {
            sscanf(buf,"%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%s",&col43,&dot,&col44,&dot,&col45,&dot,&col46,&dot,&col47,&dot,&col48,&dot,st);
          }                             /* end of seventh scan*/
     else if (i1 == 22)
          {
                 sscanf(buf,"%lf%c%lf%c%lf%c%lf%c%lf%c%lf%c%s",&col50,&dot,&col51,&dot,&col52,&dot,&col53,&dot,&col54,&dot,&col55,&dot,st);
          }                              /* end of eigth scan*/
     else if (i1 == 25)
          {
           sscanf(buf,"%lf%c%lf%c%lf%c%lf%c%lf%c%s%s",&col57,&dot,&col58,&dot,&col59,&dot,&col60,
&dot,&col61,&dot,col62,st);
          }                              /* end of ninth scan*/
     else if (i1 == 28)
          {
            sscanf(buf,"%lf%c%lf%c%s%s%lf%c%lf%c%s",&col64,&dot,&col65,&dot,col66,col67,&col68,&dot,&col69,&dot,st);
          }                              /* end of tenth scan */
  } /* end of 13 for loops*/ 
       printf("\n");
           if ((feof(fptr)!=0)|| (num  != rec_count))
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
                 printf("%6.4lf  ",col1);
                 break;
               case 1:
                 printf ("%6.3lf   ",col2);
                 break;
               case 2:
                 printf ("%6.3lf   ",col3);
                 break;
               case 3:
                 printf ("%6.3lf   ", col4);
                 break;
               case 4:
                 printf ("%6.3lf   ",col5);
                 break;
               case 5:
                 printf ("%6.3lf   ",col6);
                 break;
               case 6:
                 printf ("%6.3lf   ",col8);
                 break;
                case 7:
                 printf("%4.6lf  ",col9);
                 break;
               case 8:
                 printf ("%6.4lf  ",col10);
                 break;
               case 9:
                 printf ("%6.3lf   ",col11);
                 break;
               case 10:
                 printf ("%5.5lf  ", col12);
                 break;
               case 11:
                 printf ("%6.2lf    ",col13);
                 break;
               case 12:
                 printf ("%6.2lf    ",col15);
                 break;
               case 13:
                 printf ("%6.2lf    ",col16);
                 break;
               case 14:
                 printf ("%4.6lf  ",col17);
                 break;
               case 15:
                 printf ("%6.3lf   ", col18);
                 break;
               case 16:
                 printf ("%6.3lf   ",col19);
                 break;
               case 17:
                 printf ("   %s",st4);
                 break;
               case 18:
                 printf ("%6.3lf   ",col22);
                 break;
               case 19:
                 printf ("%6.4lf  ",col23);
                 break;
               case 20:
                 printf ("%6.4lf  ", col24);
                 break;
               case 21:
                 printf ("%4.6lf  ",col25);
                 break;
               case 22:
                 printf ("%6.2lf    ",col26);
                 break;
               case 23:
                 printf ("%4.6lf  ",col27);
                 break;
               case 24:
                 printf("%4.6lf  ",col29);
                 break;
               case 25:
                 printf ("%4.6lf  ",col30);
                 break;
               case 26:
                 printf ("%5.5lf  ",col31);
                 break;
               case 27:
                 printf ("%5.5lf  ",col32);
                 break;
               case 28:
                 printf ("%5.5lf  ",col33);
                 break;
               case 29:
                 printf ("%5.5lf  ",col34);
                 break;
               case 30:
                 printf ("%5.5lf  ",col36);
                 break;
               case 31:
                 printf ("%5.5lf  ",col37);
                 break;
               case 32:
                 printf ("%5.5lf  ",col38);
                 break;
               case 33:
                 printf ("%5.5lf  ",col39);
                 break;
               case 34:
                 printf ("%5.5lf  ",col40);
                 break;
               case 35:
                 printf ("%5.5lf  ",col41);
                 break;
               case 36:
                 printf ("%5.5lf  ",col43);
                 break;
               case 37:
                 printf ("%5.5lf  ",col44);
                 break;
               case 38:
                 printf ("%5.5lf  ",col45);
                 break;
               case 39:
                 printf ("%5.5lf  ",col46);
                 break;
               case 40:
                 printf ("%5.5lf  ",col47);
                 break;
               case 41:
                 printf ("%5.5lf  ",col48);
                 break;
               case 42:
                 printf ("%5.5lf  ",col50);
                 break;
               case 43:
                 printf ("%5.5lf  ",col51);
                 break;
               case 44:
                 printf ("%5.5lf  ",col52);
                 break;
               case 45:
                 printf ("%5.5lf  ",col53);
                 break;
               case 46:
                 printf ("%5.5lf  ",col54);
                 break;
               case 47:
                 printf ("%5.5lf  ",col55);
                 break;
               case 48:
                 printf ("%5.5lf  ",col57);
                 break;
               case 49:
                 printf ("%5.5lf  ",col58);
                 break;
               case 50:
                 printf ("%5.5lf  ",col59);
                 break;
               case 51:
                 printf ("%5.5lf  ",col60);
                 break;
               case 52:
                 printf ("%5.5lf  ",col61);
                 break;
               case 53:
                 printf ("   %s",col62);
                 break;
               case 54:
                 printf ("%5.5lf  ",col64);
                 break;
               case 55:
                 printf ("%5.5lf  ",col65);
                 break;
               case 56:
                 printf ("  %s",col66);
                 break;
               case 57:
                 printf ("  %s",col67);
                 break;
                case 58:
                 printf ("%5.5lf  ",col68);
                 break;
                case 59:
                 printf ("%5.5lf  ",col69);
                 break; 
             } /* End of Switch */
         } /*End of for */
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
* PURPOSE/DESCRIPTION: Prints the heading for the ASTEX_SOFIA_ARAT_10sec program*
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
           printf("*              FIRE_ASTEX_SOFIA_ARAT_10_SEC               *\n");
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
*                      There are 60 parameters in ASTEX_SOFIA_ARAT_10SEC data    *
*                      files. They are:                                          *
*        ----------|-----------|-----------|-----------|-----------|-----------| *
*            1     |     2     |     3     |     4     |     5     |     6     | *
*                                                                                * 
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Static     |Static     |Reverse    |Deiced     |Potential  |Virtual    | *
*       Pressure   |Temperature|Flow       |Temperature|Temperature|Potential  | * 
*                  |           |Temperature|           |           |Temperature| *
*        hPa       |degrees C  |degrees C  |degrees C  |degrees C  |degrees C  | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Equivalent |Dew Point  |Water Vapor|Relative   |Upward     |Downward   | *
*       Potential  |Temperature|Mixing     |Humidity   |Visible    |Visible    | *
*       Temperature|           |Ratio      |           |Flux       |Flux       | *
*       degrees C  |degrees C  |g/kg       |%          |W/m^2      |W/m^2      | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Upward     |Downward   |Surface    |Downward   |Dew Point  |Liq. Water | *
*       Infra Red  |Infra Red  |Temperature|Radiance   |Temperature|Content    | * 
*       Flux       |Flux       |           |           |EGG        |(J-W)      | *
*       W/m^2      |W/m^2      |degrees C  |W/m^2/sr   |degrees C  |g/kg       | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Liq. Water |Droplet    |Aircraft   |Aircraft   |Aircraft   |Velocity   | *
*       Content    |Liq. Water |Roll       |Pitch      |Heading    |North      | *
*       FSSP       |Content    |           |           |           |           | *
*       g/kg       |g/kg       |degrees    |degrees    |degrees    |m/s        | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Velocity   |Vertical   |Doppler    |Doppler    |Doppler    |Ice        | *
*       East       |Velocity   |Speed x    |Speed y    |Speed z    |Detector   | * 
*                  |           |           |           |           |           | *
*       m/s        |m/s        |m/s        |m/s        |m/s        |mm         | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*                  |Wind       |Wind       |Wind       |Wind       |           | *
*       Airspeed   |Velocity  D|Direction D|Velocity  I|Direction I|W simple   | *
*                  |           |           |           |           |           | *
*       m/s        |m/s        |degrees    |m/s        |degrees    |m/s        | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Latitude   |Longitude  |Latitude   |Longitude  |Latitude   |Longitude  | *
*       Inertial   |Inertial   |INS/hybrid |INS/hybrid |North D    |East  D    | * 
*       Nav System |Nav System |           |           |(GPS)      |(GPS)      | *
*       degrees    |degrees    |degrees    |degrees    |degrees    |degrees    | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Pressure   |Radar      |Radar      |Particle   |Upward     |Particle   | *
*       Altitude   |Altimeter  |Altimeter  |Scattering |Radiance   |Concen.    | * 
*                  |Altitude   |Altitude   |Nephlometer|           |ASASP      | *
*       m          |(0-1500 m) |(0-6000 m) |1/m        |W/m^2/sr   |part/cc    | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Particle   |           |Variance of|Variance of|Variance of|Variance of| *
*       Concen.    |Temperature|Vertical   |Transverse |Potential  |Water Vapor| *
*       CCN        |EGG        |Velocity   |Velocity   |Temperature|Mixing Rat.| *
*       part/cc    |degrees C  |(m/s)^2    |(m/s)^2    |(deg. C)^2 |(g/kg)^2   | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Variance of|Variance of|Variance of|Variance of|Relative   |Time       | *
*       Airspeed   |Nephlometer|Upward     |Downward   |Humidity   |Fractional | *
*                  |           |Radiance   |Radiance   |           |Hours      | *
*       (m/s)^2    |(1/m)^2    |(W/m2/sr)^2|(W/m2/sr)^2|%          |h          | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*                                                                                *
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
        printf("There are five  parameters of information that was \n");
        printf("collected at each event. These parameters are: \n");
        printf("\n");
        printf("    NO.          Parameter                           Units       \n");
        printf("    ---          ---------                           -----       \n");
        printf("     1.          Static Pressure                     hPa         \n");
        printf("     2.          Static Temperature                  C           \n");
        printf("     3.          Reverse Flow Temp.                  C           \n");
        printf("     4.          Deiced Temperature                  C           \n");
        printf("     5.          Potential Temperature               C           \n");
        printf("     6.          Virtual Pot. Temp.                  C           \n");
        printf("     7.          Equivalent Pot. Temp.               C           \n");
        printf("     8.          Dew Point Temperature               C           \n");
        printf("     9.          Water Vapor Mixing Ratio            g/kg        \n");
        printf("    10.          Relative Humidity                   percent     \n");
        printf("    11.          Upward Visible Flux                 W/m^2       \n");
        printf("    12.          Downward Visible Flux               W/m^2       \n");
        printf("    13.          Upward IR Flux                      W/m^2       \n");
        printf("    14.          Downward IR Flux                    W/m^2       \n");
        printf("    15.          Surface Temperature                 C           \n");
        printf("    16.          Downward Radiance                   W/m^2-sr    \n");
        printf("    17.          Dew Point Temp. EGG                 C           \n");
        printf("    18.          Liquid Water Content (J-W)          g/kg        \n");
        printf("    19.          Liquid Water Content (FSSP)         g/kg        \n");
        printf("    20.          Droplet Liquid Water Content        g/kg        \n");
        printf("    21.          Aircraft Roll                       deg         \n");
        printf("    22.          Aircraft Pitch                      deg         \n");
        printf("    23.          Aircraft Heading                    deg         \n");
        printf("    24.          Velocity North                      m/s         \n");
        printf("    25.          Velocity Est                        m/s         \n");
        printf("    26.          Vertical Velocity                   m/s         \n");
        printf("    27.          Doppler Speed x                     m/s         \n");
        printf("    28.          Doppler Speed y                     m/s         \n");
        printf("    29.          Doppler Speed z                     m/s         \n");
        printf("    30.          Ice Detector                        mm          \n");
        printf("    31.          Airspeed                            m/s         \n");
        printf("    32.          Wind Velocity D                     m/s         \n");
        printf("    33.          Wind Direction D                    deg         \n");
        printf("    34.          Wind Velocity I                     m/s         \n");
        printf("    35.          Wind Direction I                    deg         \n");
        printf("    36.          W Simple                            m/s         \n");
        printf("    37.          Latitude INS                        deg         \n");
        printf("    38.          Longitude INS                       deg         \n"); 
        printf("    39.          Latitude INS/hybrid                 deg         \n");
        printf("    40.          Longitude INS/hybrid                deg         \n");
        printf("    41.          Latitude North D (GPS)              deg         \n");
        printf("    42.          Longitude East D (GPS)              deg         \n");
        printf("    43.          Pressure Altitude                   m           \n");
        printf("    44.          Radar Altimeter Altitude 1          (0-1500m)   \n");
        printf("    45.          Radar Altimeter Altitude 2          (0-6000m)   \n");
        printf("    46.          Part. Scattering Neph.              1/m         \n");
        printf("    47.          Upward Radiance                     W/m^2-sr    \n");
        printf("    48.          Part. Conc. ASASP                   part/cc     \n");
        printf("    49.          Part. Conc. CCN                     part/cc     \n");
        printf("    50.          Temperature EGG                     C           \n");
        printf("    51.          Variance of Vert. Velocity          (m/s)^2     \n");
        printf("    52.          Variance of Transverse Velocity     (m/s)^2     \n");
        printf("    53.          Variance of Pot. Temp               (deg C)^2   \n");
        printf("    54.          Variance of Water Vap. Mix. Ratio   (g/kg)^2    \n");
        printf("    55.          Variance of Wind Speed              (m/s)^2     \n");
        printf("    56.          Variance of Nephelometer            (1/m)^2     \n");
        printf("    57.          Variance of Upward Radiance         (W/m^2-sr)^2 \n");
        printf("    58.          Variance of Downward Radiance       (W/m^2-sr)^2 \n");
        printf("    59.          Relative Humidity                   Percent     \n");
        printf("    60.          Time                                Hours       \n");
        printf("\n\n");

           /* SELECT PARAMETERS */
 
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
                                printf(" THE TOTAL PARAMETERS AVAILABLE ARE 1 THRU 60\n");
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
                     printf("****************EXIT PROGRAM*****************\n");
                     exit(0);
                  }
 
   } /* end of the get_type function */
 
 
                    
/*********************************************************************************
* FILE: Get_rec_hd_type                                                          *
*                                                                                *
*AUTHOR: Mandira Mukherjee.                                                      *
*                                                                                *
* PURPOSE/DESCRIPTION:                                                           *
*                        There are 60 parameters of information collected at     *
*                        each event. They are:                                   *
*        ----------|-----------|-----------|-----------|-----------|-----------| *
*            1     |     2     |     3     |     4     |     5     |     6     | *
*                                                                                *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Static     |Static     |Reverse    |Deiced     |Potential  |Virtual    | *
*       Pressure   |Temperature|Flow       |Temperature|Temperature|Potential  | *
*                  |           |Temperature|           |           |Temperature| *
*        hPa       |degrees C  |degrees C  |degrees C  |degrees C  |degrees C  | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Equivalent |Dew Point  |Water Vapor|Relative   |Upward     |Downward   | *
*       Potential  |Temperature|Mixing     |Humidity   |Visible    |Visible    | *
*       Temperature|           |Ratio      |           |Flux       |Flux       | *
*       degrees C  |degrees C  |g/kg       |%          |W/m^2      |W/m^2      | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Upward     |Downward   |Surface    |Downward   |Dew Point  |Liq. Water | *
*       Infra Red  |Infra Red  |Temperature|Radiance   |Temperature|Content    | *
*       Flux       |Flux       |           |           |EGG        |(J-W)      | *
*       W/m^2      |W/m^2      |degrees C  |W/m^2/sr   |degrees C  |g/kg       | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Liq. Water |Droplet    |Aircraft   |Aircraft   |Aircraft   |Velocity   | *
*       Content    |Liq. Water |Roll       |Pitch      |Heading    |North      | *
*       FSSP       |Content    |           |           |           |           | *
*       g/kg       |g/kg       |degrees    |degrees    |degrees    |m/s        | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Velocity   |Vertical   |Doppler    |Doppler    |Doppler    |Ice        | *
*       East       |Velocity   |Speed x    |Speed y    |Speed z    |Detector   | *
*                  |           |           |           |           |           | *
*       m/s        |m/s        |m/s        |m/s        |m/s        |mm         | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*                  |Wind       |Wind       |Wind       |Wind       |           | *
*       Airspeed   |Velocity  D|Direction D|Velocity  I|Direction I|W simple   | *
*                  |           |           |           |           |           | *
*       m/s        |m/s        |degrees    |m/s        |degrees    |m/s        | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Latitude   |Longitude  |Latitude   |Longitude  |Latitude   |Longitude  | *
*       Inertial   |Inertial   |INS/hybrid |INS/hybrid |North D    |East  D    | *
*       Nav System |Nav System |           |           |(GPS)      |(GPS)      | *
*       degrees    |degrees    |degrees    |degrees    |degrees    |degrees    | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Pressure   |Radar      |Radar      |Particle   |Upward     |Particle   | *
*       Altitude   |Altimeter  |Altimeter  |Scattering |Radiance   |Concen.    | *
*                  |Altitude   |Altitude   |Nephlometer|           |ASASP      | *
*       m          |(0-1500 m) |(0-6000 m) |1/m        |W/m^2/sr   |part/cc    | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Particle   |           |Variance of|Variance of|Variance of|Variance of| *
*       Concen.    |Temperature|Vertical   |Transverse |Potential  |Water Vapor| *
*       CCN        |EGG        |Velocity   |Velocity   |Temperature|Mixing Rat.| *
*       part/cc    |degrees C  |(m/s)^2    |(m/s)^2    |(deg. C)^2 |(g/kg)^2   | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*       Variance of|Variance of|Variance of|Variance of|Relative   |Time       | *
*       Airspeed   |Nephlometer|Upward     |Downward   |Humidity   |Fractional | *
*                  |           |Radiance   |Radiance   |           |Hours      | *
*       (m/s)^2    |(1/m)^2    |(W/m2/sr)^2|(W/m2/sr)^2|%          |h          | *
*       -----------|-----------|-----------|-----------|-----------|-----------| *
*                                                                                *
* INVOCATION:                                                                    *
*                     (void) Get_rec_hd_type                                     *
*INTERNALROUTINES:                                                               *
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
                 printf("St. Pres.");
                 break;
               case 1:
                 printf("St. Temp.");
                 break;
               case 2:
                 printf("Rev Flow Temp.");
                 break;
               case 3:
                 printf("Deiced Temp.");
                 break;
               case 4:
                 printf("Pot. Temp.");
                 break;
               case 5:
                 printf("Virt. Pot. Temp.");
                 break;
               case 6:
                 printf("Equiv. Pot. Temp.");
                 break;
               case 7:
                 printf("Dew Pt. Temp. ");
                 break;
               case 8:
                 printf("Wat Vap Mix Ratio");
                 break;
               case 9:
                 printf("Rel. Hum.");
                 break;
               case 10:
                 printf("Up. Vis. Flux");
                 break;
               case 11:
                 printf("Down. Vis. Flux");
                 break;
               case 12:
                 printf("Up. IR Flux");
                 break;
               case 13:
                 printf("Down. IR Flux");
                 break;
               case 14:
                 printf("Surf. Temp");
                 break;
               case 15:
                 printf("Down. Rad.");
                 break;
               case 16:
                 printf("Dew Pt. Temp. EGG");
                 break;
               case 17:
                 printf("LWC J-W");
                 break;
               case 18:
                 printf("LWC FSSP");
                 break;
               case 19:
                 printf("Droplet LWC");
                 break;
               case 20:
                 printf("Aircraft Roll");
                 break;
               case 21:
                 printf("Aircraft Pitch");
                 break;
               case 22:
                 printf("Aircraft Heading");
                 break;
               case 23:
                 printf("Velocity N");
                 break;
               case 24:
                 printf("Velocity E");
                 break;
               case 25:
                 printf("Vertical Vel.");
                 break;
               case 26:
                 printf("Doppler Speed x");
                 break;
               case 27:
                 printf("Doppler Speed y");
                 break;
               case 28:
                 printf("Doppler Speed z");
                 break;
               case 29:
                 printf("Ice Detector");
                 break;
               case 30:
                 printf("Airspeed");
                 break;
               case 31:
                 printf("Wind Vel. D");
                 break;
               case 32:
                 printf("Wind Dir. D");
                 break;
               case 33:
                 printf("Wind Vel. I ");
                 break;
               case 34:
                 printf("Wind Dir. I" );
                 break;
               case 35:
                 printf("W Simple" );
                 break;
               case 36:
                 printf("Lat. INS" );
                 break;
               case 37:
                 printf("Lon. INS" );
                 break;
               case 38:
                 printf("Lat. INS/hybrid");
                 break;
               case 39:
                 printf("Lon. INS/hybrid");
                 break;
               case 40:
                 printf("Lat. North D (GPS)");
                 break;
               case 41:
                 printf("Lon. East D (GPS)");
                 break;
               case 42:
                 printf("Pres. Alt.");
                 break;
               case 43:
                 printf("Rad. Altim. Alt. 1");
                 break;
               case 44:
                 printf("Rad. Altim. Alt. 2");
                 break;
               case 45:
                 printf("Part. Sc. Neph.");
                 break;
               case 46:
                 printf("Down. Radiance");
                 break;
               case 47:
                 printf("Part. Conc. ASASP");
                 break;
               case 48:
                 printf("Part. Conc. CCN");
                 break;
               case 49:
                 printf("Temp. EGG");
                 break;
               case 50:
                 printf("Var. Vert. Vel.");
                 break;
               case 51:
                 printf("Var. Trans. Vel.");
                 break;
               case 52:
                 printf("Var. Pot. Temp.");
                 break;
               case 53:
                 printf("Var. H2O Vap. Mix. Ratio");
                 break;
               case 54:
                 printf("Var. Airspeed");
                 break;
               case 55:
                 printf("Var. Neph.");
                 break;
               case 56:
                 printf("Var. Up. Rad.");
                 break;
               case 57:
                 printf("Var. Down. Rad.");
                 break;
               case 58:
                 printf("Rel. Hum.");
                 break;
               case 59:
                 printf("Time");
                 break;
                 default:
                 break;
              } /* End of Switch */
         }    /* End of for */
  }    /* End of Get_rec_hd_type function */

/***********************************************************************************/
 

