/************************************************************************
*                                                                       *
* PROGRAM:   fax_psu_malbal_read.c                                      *
*                                                                       *
* VERSION:   1.0                       DATE:  February 9, 1995          *
*                                                                       *
* PURPOSE:   The purpose of this code is to read the FIRE ASTEX         *
*            Malcolm Baldrige data set.                                 *
*                                                                       *
* ALGORITHM: None.                                                      *
*                                                                       *
* INVOCATION:    fax_psu_malbal_read.c datafile                         *
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
*           Pennsylvania State University Malcolm Baldrige data set     *
*           file.                                                       *
*                                                                       *
*                                                                       *
************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 256
#define NUM_PARAMS 42

main(argc,argv)
int argc;
char *argv[];
{
  char in_file[LINESIZE],       /* data set to be read                       */
       fname[LINESIZE],         /* data set name given in command line       */
       buf[LINESIZE],
       tmp[4],                  /* place holding for the param selection    */
       list[4],
       month[4],
       gmt[6],lon[9],ct[8],
       cq[8],cu[8],cw[8],
       end[3];

  int  count, counter,          /* counter                                  */
       params[100],             /* integer format for params selected       */
       i;

  int  day,us,tru,rel,hed,hsc,hsi,hsb,
       hlc,hli,hlb,rs,rl,rain,j,
       oph,tlt,lat;

  double u10,ts,t,qs,q,tuc,
         tui,tub,to,qo,
         tr,re,ab,tau,f0,w,
         zb,zt;
    
  FILE *fptr;                   /* file pointer to input data set            */

  void Display_greeting();

/* Initialize the params array  */

   for (count=0; count <= 100; count++)
      params[count] = -99;
   (void) Display_greeting();

/* Get name of data set to be read */

  if (argc == 1) 
  {
    printf("Please enter the name of the data set to be read:  ");
    scanf("%s",fname);
    strcpy(in_file,fname);
  }
  else
  {
    strcpy(in_file,argv[1]);
  }

  printf("\n\n");
  printf("The data set to be read is %s \n", in_file);

/* Open data set to read */

  fptr = fopen(in_file,"r");

/* Check to see if eof encountered  */

  if(fptr == NULL)
  {
    printf("***********************************************************\n");
    printf("*                                                         *\n");
    printf("*           ERROR -- UNABLE TO OPEN THE DATA SET          *\n");
    printf("*                   PROGRAM TERMINATED                    *\n");
    printf("*                                                         *\n");
    printf("***********************************************************\n");
    exit(-1);
  }

/* Print the parameters available on this data set  */

  printf("\n\n");
  printf("The following are the parameters on this data set: \n");
  printf("\n\n");
  printf("No.  Parameter/Description                    Units \n");
  printf("---  ---------------------                    ----- \n");
  printf("\n");
  printf(" 1.  Date                                  Day and Month\n");
  printf(" 2.  GMT                                      hr:min    \n");
  printf(" 3.  Us:  Ship speed                           m/s      \n");
  printf(" 4.  U10: True ship speed at 10m height        m/s      \n");
  printf(" 5.  Tru: True wind direction rel. to N        deg      \n");
  printf(" 6.  Rel: Relative wind direction              deg      \n");
  printf(" 7.  Hed: Direction of ships bow               deg      \n");
  printf(" 8.  Ts:  Sea surface temperature               C       \n");
  printf(" 9.  T:   Temperature                           C       \n");
  printf("10.  qs:  Sea surface specific humidity        g/kg     \n");
  printf("11.  q:   Vaisala air specific humidity        g/kg     \n");
  printf("12.  Hsc: Covariance sensible heat flux       W/m^2     \n");
  printf("13.  Hsi: Initial sensible heat flux          W/m^2     \n");
  printf("14.  Hsb: Bulk sensible heat flux             W/m^2     \n");
  printf("15.  Hlc: Covariance latent heat flux         W/m^2     \n");
  printf("16.  Hli: Inertial latent heat flux           W/m^2     \n");
  printf("17.  Hlb: Bulk latent heat flux               W/m^2     \n");
  printf("18.  Tuc: Covariance surface stress           N/m^2     \n");
  printf("19.  Tui: Inertial-dissipation surface stress N/m^2     \n");
  printf("20.  Tub: Bulk surface stress                 N/m^2     \n");
  printf("21.  Rs:  Solar irradiance                    W/m^2     \n");
  printf("22.  Rl:  Longwave irradiance                 W/m^2     \n");
  printf("23.  Rain:Rain rate                           mm/hr     \n");
  printf("24.  J:   Ship maneuver/contamination index             \n");
  printf("25.  Oph: Stddev OPHIR hygrometer clear channel counts  \n");
  printf("26.  Tlt: Mean wind vector tilt                         \n");
  printf("27.  Ct:  Sonic temperature                k^2/m^0.667  \n");
  printf("28.  Cq:  Water vapor                 (g/m^3)^2/m^0.667)\n");
  printf("29.  Cu:  Streamwise velocity          (m/s/^2/m^0.667) \n");
  printf("30.  Cw:  Vertical velocity            (m/s/^2/m^0.667) \n");
  printf("31.  To:  OPHIR air temperature                C        \n");
  printf("32.  Qo:  OPHIR specific humidity             g/kg      \n");
  printf("33.  Tr:  solar transmittance                           \n");
  printf("34.  Re:  solar reflectance                             \n");
  printf("35.  Ab:  Solar absorbtance                             \n");
  printf("36.  Tau: System optical depth                          \n");
  printf("37.  f(0):Overhead cloud fraction                       \n");
  printf("38.  W:   Liquid water path                  g/m^2      \n");
  printf("39.  Zb:  Cloud base                           m        \n");
  printf("40.  Zt:  Cloud top                            m        \n");
  printf("41.  Lat: Latitude                                      \n");
  printf("42.  Lon: Longitude                                     \n");
  printf("\n\n");
  
  /*  Selection of parameters whether all or some  */

  list[0] = 'Q';
  while ((list[0]!='a') && (list[0]!='A') && (list[0]!='s') && (list[0]!='S') && (list[0]!='x') && (list[0]!='X'))
  {
    printf(" Do you want all parameters to be printed \n");
    printf(" Please select: a or A for All; S for Some; X to eXit: ");
    scanf("%s",tmp); 
    strcpy(list,tmp); 
  }                                          /* end of while statement */

    if ((list[0] == 'a') || (list[0] == 'A'))
    {
      for(count=0; count <= NUM_PARAMS; count++)
        params[count] = count;
    }
    else if ((list[0] == 's') || (list[0] == 'S'))
    { 
      printf("****  Selection of parameters  ***\n");
      tmp[0] = 'S';
      count = 1;
      while(( tmp[0] != 'q') && (tmp[0] != 'Q') && (tmp[0] != 'x') && (tmp[0] != 'X'))
      {
         printf("Please enter parameter# you wish to view \n");
         printf("Enter q to end selection process or X to exit the program --> \n");
         scanf("%s",tmp);
         strcpy(list,tmp);
         if ((list[0] == 'q') || (list[0] == 'Q'))
	 {
           if(params[0] == -99)
           {
             strcpy(tmp,"    ");             
             printf("\n");
             printf(" Selection of parameters have not yet been made \n");
             printf(" Do you want to exit the program -- enter x or X \n");
             printf(" OR Enter parameters numbers \n");
             scanf("%s",tmp);
             strcpy(list,tmp);
           }
           else break;
	 }

         if((list[0] == 'x') || (list[0] == 'X'))
         {
           printf("\n\n");
           printf("**** EXITING THE PROGRAM PER YOUR REQUEST ****\n");
           exit(0);
	 }

         if((atoi(tmp) > NUM_PARAMS) || (atoi(tmp) <= 0))
	 {
           printf("\n");
           printf("A NUMBER NOT ALLOWED FOR PARAMETER SELECTION HAS BEEN INPUT\n");
           strcpy(tmp,"    ");
         }

         params[count-1] = atoi(tmp) - 1;
         count++;
      }                                      /* end of the while statement */
    }                                        /* end of the if-else statement */
    else if ((list[0] = 'x') || (list[0] = 'X'))
    {
     printf("\n\n");
     printf(" **** EXITING THE PROGRAM PER YOUR REQUEST *****\n");
     exit(0);
    }

 /* Read the data set and print the parameters requested by the user */

    counter = 0;         /* counter is for # of records read */

/* Print the headers for the requested parameters */

  for (i = 1; i <= count; i++)
  {
     switch (params[i-1])
     {
	case 0:
          printf("  Date  ");
          break;
        case 1:
          printf("  GMT  ");
          break;
        case 2:
          printf("  Us   ");
          break;
	case 3:
          printf("  U10  ");
          break;
	case 4:
          printf("  Tru  ");
          break;
        case 5:
          printf("  Rel  ");
          break;
        case 6:
          printf("  Hed  ");
          break;
        case 7:
          printf("  Ts   ");
          break;
        case 8:
          printf("  T    ");
          break;
        case 9:
          printf("  qs   ");
          break;
        case 10:
          printf("  q    ");
          break;
        case 11:
          printf("  Hsc  ");
          break;
        case 12:
          printf("  Hsi  ");
          break;
        case 13:
          printf("  Hsb  ");
          break;
        case 14:
          printf("  Hlc  ");
          break;
        case 15:
          printf("  Hli  ");
          break;
        case 16:
          printf("  Hlb  ");
          break;
        case 17:
          printf("  Tuc  ");
          break;
        case 18:
          printf("  Tui  ");
          break;
        case 19:
          printf("  Tub  ");
          break;
        case 20:
          printf("  Rs   ");
          break;
        case 21:
          printf("  Rl   ");
          break;
        case 22:
          printf("  Rain ");
          break;
        case 23:
          printf("  J    ");
          break;
        case 24:
          printf("  Oph  ");
          break;
        case 25:
          printf("  Tlt  ");
          break;
        case 26:
          printf("  Ct   ");
          break;
        case 27:
          printf("  Cq   ");
          break;
        case 28:
          printf("  Cu   ");
          break;
        case 29:
          printf("  Cw   ");
          break;
        case 30:
          printf("  To   ");
          break;
        case 31:
          printf("  Qo   ");
          break;
        case 32:
          printf("  Tr   ");
          break;
        case 33:
          printf("  Re   ");
          break;
        case 34:
          printf("  Ab   ");
          break;
        case 35:
          printf("  Tau  ");
          break;
        case 36:
          printf("  f(0) ");
          break;
        case 37:
          printf("  W    ");
          break;
        case 38:
          printf("  Zb   ");
          break;
        case 39:
          printf("  Zt   ");
          break;
        case 40:
          printf("  Lat   ");
          break;
        case 41:
          printf("  Lon   ");
          break;
        default:
          break;
     }         /* end of case statement */
   } /* end of the for loop */
  printf("\n");

/* Put the header in the buffer area */

  fgets(&buf[0],LINESIZE,fptr);
  
  strcpy(end,"^Z");
  counter = 0;
  while ((feof(fptr) == 0)) 
  {
    strcpy(month,"    ");
    strcpy(gmt,"      ");
    strcpy(lon,"         ");
    strcpy(ct,"        ");
    strcpy(cq,"        ");
    strcpy(cu,"        ");
    strcpy(cw,"        ");

    day=us=tru=rel=hed=hsc=hsi=hsb= 0;
    hlc=hli=hlb=rs=rl=rain=j=oph=tlt=lat= 0;

    u10=ts=t=qs=q=tuc=0.0;
    tui=tub=to=qo=0.0;
    tr=re=ab=tau=f0=w=zb=zt=0.0;
    
    fscanf(fptr,"%d%s%s%d%lf%d%d%d%lf%lf%lf%lf%d%d%d%d%d%d%lf%lf%lf%d%d%d%d%d%d%s%s%s%s%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%d%s",&day,month,gmt,&us,&u10,&tru,&rel,&hed,&ts,&t,&qs,&q,&hsc,&hsi,&hsb,&hlc,&hli,&hlb,&tuc,&tui,&tub,&rs,&rl,&rain,&j,&oph,&tlt,ct,cq,cu,cw,&to,&qo,&tr,&re,&ab,&tau,&f0,&w,&zb,&zt,&lat,lon);

    
/*  End of file encountered */

    if ((feof(fptr) != 0) || (day == atoi(end)))
    {
      printf("\n\n");
      printf("****************************************************\n");
      printf("*                                                  *\n");
      printf("*              END OF FILE ENCOUNTERED             *\n");
      printf("*                                                  *\n");
      printf("*          %d records of data were read          *\n",counter);
      printf("*                                                  *\n");
      printf("*  EXECUTION OF PROGRAM COMPLETED AND SUCCESSFUL   *\n");
      printf("*                                                  *\n");
      printf("****************************************************\n");
      exit(0);
    }


/*  Print the parameters selected to the screen */

    for (i= 1; i <= count; i++)
    {
         switch (params[i-1])
	   {
	     case 0:
               printf(" %d %s   ",day,month);
               break;
             case 1:
               printf(" %s  ",gmt);
               break;
	     case 2:
               printf(" %d  ",us);
               break;
	     case 3:
               printf("%.1lf",u10);
               break;
	     case 4:
               printf(" %d  ",tru);
               break;
             case 5:
	       printf(" %d  ",rel);
	       break;
             case 6:
               printf(" %d  ",hed);
               break;
             case 7:
               printf(" %.1lf",ts);
               break;
             case 8:
               printf(" %.1lf",t);
               break;
             case 9:
               printf(" %.1lf",qs);
               break;
             case 10:
               printf(" %.1lf",q);
               break;
             case 11:
               printf(" %d   ",hsc);
               break;
             case 12:
               printf(" %d   ",hsi);
               break;
             case 13:
               printf(" %d   ",hsb);
               break;
             case 14:
               printf(" %d   ",hlc);
               break;
             case 15:
               printf(" %d   ",hli);
               break;
             case 16:
               printf(" %d   ",hlb);
               break;
             case 17:
               printf(" %.3lf",tuc);
               break;
             case 18:
               printf(" %.3lf",tui);
               break;
             case 19:
               printf(" %.3lf",tub);
               break;
             case 20:
               printf(" %d   ",rs);
               break;
             case 21:
               printf(" %d   ",rl);
               break;
             case 22:
               printf(" %d   ",rain);
               break;
             case 23:
               printf(" %d   ",j);
               break;
             case 24:
               printf(" %d   ",oph);
               break;
             case 25:
               printf(" %d   ",tlt);
               break;
             case 26:
               printf(" %s   ",ct);
               break;
             case 27:
               printf(" %s   ",cq);
               break;
             case 28:
               printf(" %s   ",cu);
               break;
             case 29:
               printf(" %s   ",cw);
               break;
             case 30:
               printf(" %.1lf",to);
               break;
             case 31:
               printf(" %.1lf",qo);
               break;
             case 32:
               printf(" %.2lf",tr);
               break;
             case 33:
               printf(" %.2lf",re);
               break;
             case 34:
               printf(" %.2lf",ab);
               break;
             case 35:
               printf(" %.2lf",tau);
               break;
             case 36:
               printf(" %.2lf",f0);
               break;
             case 37:
               printf(" %d   ",w);
               break;
             case 38:
               printf(" %lf  ",zb);
               break;
             case 39:
               printf(" %lf  ",zt);
               break;
             case 40:
               printf(" %d   ",lat);
               break;
             case 41:
               printf(" %s   ",lon);
               break;
             default:
               break;
	     }         /* end of case statement */
       }    /* end of the for-statement  */
     printf("\n");
     counter++;
  }  /* end of the while statement */
} /* end of the main program */


void Display_greeting()
  {
   printf("*************************************************************\n");
   printf("*                                                           *\n");
   printf("* FIRE ASTEX Pennsylvania State University Malcolm Baldrige *\n");
   printf("*                                                           *\n");
   printf("*                        READ PROGRAM                       *\n");
   printf("*                                                           *\n");
   printf("* Version 1.0                            September 19, 1995 *\n");
   printf("*************************************************************\n");
   printf("\n\n");
 }  /* end of the display greeting */
