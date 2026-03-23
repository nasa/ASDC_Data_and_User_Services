
/************************************************************************************
*                                                                                   *
* Program - rd_ax_sofia_ptu.c                                                       *
*                                                                                   *
* AUTHOR: Mandira Mukherjee                                                         *
*                                                                                   *
* Date: 10/14/94                                                                    *
*                                                                                   *
*PURPOSE/DESCRIPTION:                                                               *
*                      The data file is in ASCII format. The first line of the prog *
*                      ram outputs the Version number(V), second line indicates the *
*                      identification of sounding(A), third line indicates adjustme *
*                      nt(d). Fourth line shows the name of the raw file associated *
*                      with the sounding(b), fifth line indicates parameter to      *
*                      compute the median of PTU Vaisala data(m), Sixth line indicat*
*                      es file of raw wind data associated with the radiosounding(c)*
*                      Seventh line shows the criterium of segmentation used (s),   *
*                      Eigth line indicates the start time(t), ninth line shows the *
*                      surface data (a). The output of the program is based on the  *
*                      Selected points PTU (B) or Selected points WIND (C) or both  *
*                      the points(A).                                               *
************************************************************************************/

#include  <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/param.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define  RECSIZE    80


main (argc,argv)
int argc;
char *argv[];



{
  void Print_heading();               /* prints the heading of the data set */
  void Get_par_type();                /* displays the choice of parameters */
  char filename1[MAXPATHLEN+1];       /* name of the file as first argv */
  char buf[256];                      /* buffer to read strings at a time */
 
  FILE *fptr;                         /* file pointer */
  int i,k,k1,amount,count;            /* count */
  char pts[3],                        /* stores variables points */
       ch[4],                         /* stores channel num */
       par_type[10];                  /* stores parameter type requested */
  int time,z,u,dd,ff;                
  double p,t;




/*********************************************************************************/

 Print_heading();
if (argc == 1)
   {
     printf("Please enter the datafile name:      ");
     scanf("%s",filename1);
   }
 else
   {
     strcpy(filename1,argv[1]);
   }

(void) Get_par_type(par_type);
/*printf("%s\n",par_type);*/


printf("DATA FILENAME REQUESTED IS %s\n",filename1);

if ((fptr = fopen(filename1,"r"))==NULL)
     {
       printf("***************************************************\n");
       printf("ERROR-UNABLE TO OPEN DATA FILE. PROGRAM HAS TERMINATED\n");
       printf("******************************************************\n");
       exit(-1);
     }
 for (i =0; i< 22; i++)
  { 
    fgets(buf,80,fptr);
    printf("%s\n",buf);
  }
 while ((feof(fptr)) ==0)
     {
       fgets(buf,81,fptr);
      /* printf("\n");*/
       sscanf(buf,"%s",pts);
       /*printf("selected point = %s\n",pts);*/
           if ((feof(fptr)!=0))
             {
               printf("\n\n\n");
               printf("----------------------------------------------------\n");
               printf("*THE END OF FILE HAS BEEN REACHED........           \n");
               printf(" THIS IS A NORMAL TERMINATION...........            \n");
               printf("-----------------------------------------------------\n");
               exit(0);
             }
          if (pts[0] == 'B')
               {
                    if ((par_type[0] == 'B')||(par_type[0] == 'b') ||(par_type[0] == 'A')||
                         (par_type[0] == 'a'))
                       {
                           printf("%s ",pts);
                           sscanf(&buf[2],"%d%d%lf%lf%d%s",&time,&z,&p,&t,&u,ch);
                           printf(" %d  %d   %.1lf   %.1lf    %d    %s \n",time,z,p,t,u,ch);
                           fgets(buf,81,fptr);
                       }
               }
          if(pts[0] == 'C')
               {
                     if ((par_type[0] == 'C')||(par_type[0] == 'c') ||(par_type[0] == 'A') ||
                           (par_type[0] == 'a'))
                         {
                            printf("%s ",pts);
                            sscanf(&buf[2],"%d%d%d%d",&time,&z,&dd,&ff);
                            printf(" %d   %d   %d     %d\n",time,z,dd,ff);
                            fgets(buf,81,fptr);
                         }
               }
                      
               
     } /* End of while */
   fclose (fptr);

} /* End of main */

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
           printf("*                 FIRE_ASTEX_NAT_SOFIA_PTU                *\n");
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
*                     Requests user for the desired data for the selected points *
*                    PTU or the selected points WINDS or both the selected points*
*                    For PTU entry is (P) , for WINDS entry is (C), and for both *
*                    entry is (A).                                               *
*                                                                                *
* INVOCATION:                                                                    *
*                     (void) Get_par_type                                        *
*                                                                                *
* INTERNALROUTINES:                                                              *
*                  NONE:                                                         *
*********************************************************************************/

void Get_par_type(par_type)

char *par_type;
  
  {
       char par[5]; /* TEMP var to store par type */
    while ((par_type[0] != 'a')&&(par_type[0]!='A')&&(par_type[0]!='B')&&
           (par_type[0]!= 'b')&&(par_type[0]!= 'C')&&(par_type[0]!='c')&&
           (par_type[0]!= 'X')&&(par_type[0]!= 'x'))
      {
           printf("\n");
           printf("PLEASE ENTER IF YOU WISH TO CHOOSE DATA FOR \n");
           printf("SELECTED POINTS PTU (B) or SELECTED POINTS WINDS (C)\n");
           printf("OR FOR BOTH THE SELECTED POINTS (A)\n");
           printf("1) IF SELECTED POINTS PTU PLEASE ENTER (B) .\n");
           printf("2) IF SELECTED POINTS WINDS PLEASE ENTER (C).\n");
           printf("3) IF BOTH THE SELECTED POINTS PLEASE ENTER (A).\n");
           printf("4) X or x TO EXIT PROGRAM\n");
           printf("\n");

           scanf("%s",par);
           strcpy(par_type,par);
      }
      printf("\n");
         if ((strcmp(par_type,"X")==0) || (strcmp(par_type,"x")==0))
           {
             printf("\n\n");
             printf("**********************EXIT PROGRAM**********************\n");
             exit(0);
           }
  }

/***********************************************************************************/
        

    
                                                            
