
/************************************************************************************
*                                                                                   *
* Program - rd_ax_sprdat.c                                                          *
*                                                                                   *
* AUTHOR: Mandira Mukherjee                                                         *
*                                                                                   *
* Date: 11/18/94                                                                    *
*                                                                                   *
* PURPOSE/DESCRIPTION: The purpose of the program is to read the SOFIA_BUOY_SPEAR   *
*                      dataset.This dataset has 39  parameters.                     *
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
#define PARAMS      39

main (argc,argv)
int argc;
char *argv[];



{
  void Print_heading();              /* prints the heading of the data set */
  int Get_par_type();                /* displays the choice of parameters */
  int Get_rec_hd_type();             /* prints the requested header */
  char filename1[MAXPATHLEN+1];       /* name of the file as first argv */
  char buf[256];                      /* buffer to read strings at a time */
  char par_type[10],tmp[10];          /* storage for requested parameter */
 
  FILE *fptr;                         /* file pointer */
  int i,rec_count,count,              /* counters */
      param[PARAMS],                  /* array of parameters */
      day,hour;
  int col1,col2,col3,col4,col5,col37,col38,col39; /* var 1 thru 5 and 37 thru 39 */
  double  col6,col7,col8,col9,col10,col11,col12,col13,col14,
         col15,col16,col17,col18,col19,col20,col21,col22,col23,
         col24,col25,col26,col27,col28,col29,col30,col31,col32,
         col33,col34,col35,col36;     /* variables 6 thru 36 */ 

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

/********************************************************************************/
if ((fptr = fopen(filename1,"r"))==NULL)
     {
       printf("***************************************************\n");
       printf("ERROR-UNABLE TO OPEN DATA FILE. PROGRAM HAS TERMINATED\n");
       printf("******************************************************\n");
       exit(-1);
     }
  

rec_count = 0;

while ((feof(fptr)) ==0)
     {
       col1 = 0;col2 = 0;col3 = 0;col4 = 0;col5 = 0;
       col6 = 0.0;col7 = 0.0;col8 = 0.0;col9 = 0.0; col10 = 0.0;col11 = 0.0;
       col12 = 0.0; col13 = 0.0; col14 = 0.0; col15 = 0.0; col16 = 0.0;col17 = 0.0;
       col18 = 0.0; col19 = 0.0; col20 = 0.0; col21 = 0.0; col22 = 0.0; col23 = 0.0;
       col24 = 0.0;col25 = 0.0; col26 = 0.0; col27 = 0.0; col28 = 0.0; col29 = 0.0;
       col30 =0.0; col31 = 0.0; col32 = 0.0; col33 = 0.0; col34 = 0.0; col35 = 0.0;
       col36 = 0.0;col37 = 0;col38 = 0;col39 = 0;

       fgets(buf,254,fptr);
       sscanf(buf,"%d %d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d %d %d ",&col1,&col2,&col3,&col4,&col5,&col6,&col7,&col8,&col9,&col10,&col11,&col12,&col13,&col14,&col15,&col16,&col17,&col18,&col19,&col20,&col21,&col22,&col23,&col24,&col25,&col26,&col27,&col28,&col29,&col30,&col31,&col32,&col33,&col34,&col35,&col36,&col37,&col38,&col39);

        fgets(buf,80,fptr);
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
                 printf("%d  ",col1);
                 break;
               case 1:
                 printf ("%d  ",col2);
                 break;
               case 2:
                 printf ("  %d  ",col3);
                 break;
               case 3:
                 printf ("%d  ", col4);
                 break;
               case 4:
                 printf ("%d  ",col5);
                 break;
               case 5:
                 printf ("%.3lf  ",col6);
                 break;
               case 6:
                 printf("%.3lf  ",col7);
                 break;
               case 7:
                 printf ("%.3lf  ",col8);
                 break;
               case 8:
                 printf ("%.3lf  ",col9);
                 break;
               case 9:
                 printf ("%.3lf  ", col10);
                 break;
               case 10:
                 printf ("%.3lf  ",col11);
                 break;
               case 11:
                 printf ("%.3lf  ",col12);
                 break;
               case 12:
                 printf("%.3lf  ",col13);
                 break;
               case 13:
                 printf ("%.3lf  ",col14);
                 break;
               case 14:
                 printf ("%.3lf  ",col15);
                 break;
               case 15:
                 printf ("%.3lf  ", col16);
                 break;
               case 16:
                 printf ("%.3lf  ",col17);
                 break;
               case 17:
                 printf ("%.3lf  ",col18);
                 break;
               case 18:
                 printf ("%.3lf  ",col19);
                 break;
               case 19:
                 printf ("%.3lf  ",col20);
                 break;
               case 20:
                 printf ("%.3lf  ",col21);
                 break;
               case 21:
                 printf ("%.3lf  ",col22);
                 break;
               case 22:
                 printf ("%.3lf  ", col23);
                 break;
               case 23:
                 printf ("%.3lf  ",col24);
                 break;
               case 24:
                 printf ("%.3lf  ",col25);
                 break;
               case 25:
                 printf ("%.3lf  ",col26);
                 break;
               case 26:
                 printf ("%.3lf  ",col27);
                 break;
               case 27:
                 printf ("%.3lf  ",col28);
                 break;
               case 28:
                 printf ("%.3lf  ", col29);
                 break;
               case 29:
                 printf ("%.3lf  ",col30);
                 break;
               case 30:
                 printf ("%.3lf  ",col31);
                 break;
               case 31:
                 printf ("%.3lf  ",col32);
                 break;
               case 32:
                 printf ("%.3lf  ",col33);
                 break;
               case 33:
                 printf ("%.3lf  ",col34);
                 break;
               case 34:
                 printf ("%.3lf  ", col35);
                 break;
               case 35:
                 printf ("%.3lf  ",col36);
                 break;
               case 36:
                 printf ("%d  ",col37);
                 break;
               case 37:
                 printf("%d  ",col38);
                 break;
               case 38:
                 printf ("%d  ",col39);
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
           printf("*          FIRE_ASTEX_NAT_SOFIA_BUOY_SPEAR                *\n");
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
*                      There are 39 parameters of information that was collected *
*                      each event. They are:                                     *
*                      Buoy number id, Year, Day, Hour/minute, Number of identical*
*                      consecutive messages, Values of 31 energy bands e(f)/emax *
*                      (non dimentional units), value of emax in dm2.s, Significa*
*                      nt wave height (H 1/3) in tenth meters, Mean Period (Tm)  *
*                      in tenth seconds.                                         *
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
        printf("There are five  parameters of information that were \n");
        printf("collected at each event. These parameters are: \n");
        printf("\n");
        printf("    NO.     Parameter                               Units        \n");
        printf("  -----     -----------                             -----        \n");
        printf("    1.      Buoy number id                                       \n");
        printf("    2.      Year                                    yy           \n");
        printf("    3.      Julian Day                              ddd          \n");
        printf("    4.      Hour/minute                             hhmm         \n");
        printf("    5.      No. of identical consecutive messages                \n");
        printf("    6 -36.  Values of the 31 energy bands e(f)/emax              \n");
        printf("    37.     Value of emax                           dm2.s        \n");
        printf("    38.     Significant wave height (H1/3)          tenth meters  \n");
        printf("    39.     Mean Period (Tm)                        tenth seconds \n");
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
                                printf(" THE TOTAL PARAMETERS AVAILABLE ARE 1 THRU 39\n");
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
*                                                                                *
* FILE: Get_rec_hd_type                                                          *
*                                                                                *
* AUTHOR: Mandira Mukherjee.                                                     *
*                                                                                *
* PURPOSE/DESCRIPTION:                                                           *
*                      There are 39 parameters of information that was collected *
*                      each event. They are:                                     *
*                      Buoy number id, Year, Day, Hour/minute, Number of identical*
*                      consecutive messages, Values of 31 energy bands e(f)/emax *
*                      (non dimentional units), value of emax in dm2.s, Significa*
*                      nt wave height (H 1/3) in tenth meters, Mean Period (Tm)  *
*                      in tenth seconds.                                         *
*                                                                                *
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
                 printf("ID    ");
                 break;
               case 1:
                 printf ("YR  ");
                 break;
               case 2:
                 printf ("JUL  ");
                 break;
               case 3:
                 printf ("MIN ");
                 break;
               case 4:
                 printf ("MESGS  ");
                 break;
               case 5:
                 printf ("BAND1  ");
                 break;
               case 6:
                 printf("BAND2  ");
                 break;
               case 7:
                 printf ("BAND3  ");
                 break;
               case 8:
                 printf ("BAND4  ");
                 break;
               case 9:
                 printf ("BAND5  ");
                 break;
               case 10:
                 printf ("BAND6  ");
                 break;
               case 11:
                 printf ("BAND7  ");
                 break;
               case 12:
                 printf("BAND8   ");
                 break;
               case 13:
                 printf ("BAND9  ");
                 break;
               case 14:
                 printf ("BAND10  ");
                 break;
               case 15:
                 printf ("BAND11  ");
                 break;
               case 16:
                 printf ("BAND12  ");
                 break;
               case 17:
                 printf ("BAND13  ");
                 break;
               case 18:
                 printf ("BAND14  ");
                 break;
               case 19:
                 printf ("BAND15  ");
                 break;
               case 20:
                 printf ("BAND16  ");
                 break;
               case 21:
                 printf ("BAND17  ");
                 break;
               case 22:
                 printf ("BAND18  ");
                 break;
               case 23:
                 printf ("BAND19  ");
                 break;
               case 24:
                 printf ("BAND20  ");
                 break;
               case 25:
                 printf ("BAND21  ");
                 break;
               case 26:
                 printf ("BAND22  ");
                 break;
               case 27:
                 printf ("BAND23  ");
                 break;
               case 28:
                 printf ("BAND24  ");
                 break;
               case 29:
                 printf ("BAND25  ");
                 break;
               case 30:
                 printf ("BAND26  ");
                 break;
               case 31:
                 printf ("BAND27  ");
                 break;
               case 32:
                 printf ("BAND28  ");
                 break;
               case 33:
                 printf ("BAND29  ");
                 break;
               case 34:
                 printf ("BAND30  ");
                 break;
               case 35:
                 printf ("BAND31  ");
                 break;
               case 36:
                 printf ("EMAX   ");
                 break;
               case 37:
                 printf("WAVE_HT  ");
                 break;
               case 38:
                 printf ("Tm   ");
                 break;
                 default:
                 break;
             }  /* End of Switch */
         }  /*End of for */
    }  /* End of Get_rec_hd_type function */

/***************************************************************************************/
 
 
 
 
                  


 
