#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

#define MAXBUF 81

main (argc,argv)
 int argc;
 char *argv[];
{  
   char filename[15];
   char id[4],sp[5];
   char t2[5],title[6],mon[3],day[3],hr[3],t3[6];
   char temp[MAXBUF],t4[5];
   char loni[7],lonf[6],dlon[5],lati[6],latf[6],dlat[5];
   char  nlon[3],nlat[3];
   float longit[82],latit[50];
   float data[82][50];
   char test[14];
   double holdit[3970];
   int rec,i,j,k,l,kk,m;
   FILE *f1;

/* the latitude and longitude range is from 85W to 10E, 70N to 15N at surface pressure 
   SP represents Surface pressure in Pascals and ST is the Sea Surface Temperature 
   in degrees Kelvin */

   banner(argc);
   if (argc ==1) {
     printf("\nPlease enter in the input file name\n\n"); 
     scanf("%s",filename);}
   else
     strcpy(filename,argv[1]);
   
   
   if((f1 = fopen(filename,"r")) == NULL)
   {  printf("Can't open the input file\n");
      exit(1);
   }
   fscanf(f1,"%s%s%s%s%s%s%s%s%s%s",title,t2,mon,day,hr,t3,t3,id,t3,sp);
   
   printf("Month is %s day %s hour %s ",mon,day,hr);
   printf("and the surface pressure is %s Pascal.\n",sp);

   fscanf(f1,"%s%s%s%s%s%s%s%s",t4,t4,t4,t4,t4,t4,t4,t4);
   fscanf(f1,"%7s%7s%7s%7s%7s%7s%7s%7s", loni,lonf,dlon,nlon,lati,latf,dlat,nlat);
   printf("Initial longitude = %s and the final longitude = %s.\n\n\n", loni,lonf);
   printf("The longitude delta is %s and the number of longitude positions are %s.\n\n",dlon,nlon);
   printf("Initial latitude = %s and the final latitude = %s with a delta of %s\n",lati,latf,dlat);
   printf("providing %s latitude postions.\n",nlat);

/* The remaining data is a sweep of latitude numbers per longitude position */
   

   longit[0] = atof(loni);
   latit[0]  = atof(lati);
   fgets(&temp[0],MAXBUF,f1);
/*
   get all of the data from the file and store in array holdit
*/
   m = 0;
   while (feof(f1) ==0) {
      fgets(&temp[0],MAXBUF,f1);
      kk =1; 
      for (l=0;l<6;l++) {
        strncpy(test,&temp[kk],13);
        holdit[m] = atof(test);
        kk=kk+13;
        m++;
      }   
   }
/*
  break the holdit array up into data sections and display
*/ 
   m=0;
   for (i=0;i<atoi(nlon);i++) {
     printf("\n\n     Longitude           Latitude");
     switch (atoi(id)) {
       case 212:
         printf("  Cloud Liquid Water (kg/kg)  \n");
         break;
       case 213:
         printf("  Cloud Fraction    \n");
         break;
       default:
         printf("\n\n Error in the field identifier \n");
         exit(0);
      } 
     printf("---------------------------------------------------------------\n");
     for (j=0; j < atoi(nlat);j++) {
        printf( "     %f         %f         %13.6e\n", longit[i],latit[j], holdit[m]);
        m++;
        latit[j+1] = latit[j]-atof(dlat); 
      }

     longit[i+1]=longit[i] + atof(dlon);
   }
     printf(" \n\nThe Read Program for ASTEX Data Set from ECMWF has completed.\n");
     rec = atoi(nlon)*atoi(nlat);
     printf(" There are a %d records displayed.\n", rec);
 }

banner(argc)
{
  char cont;
 
  printf("**********************************************************\n\n");
  printf("  FIRE ASTEX Cloud from ECMWF READ PROGRAM \n\n");
  printf("  Version 1.0                                  July 1995 \n\n");
  printf("  This program reads the Basic Data generated in June 1992. The\n");
  printf("  input file name is built according to the naming convention formed\n");
  printf("  by the Langley DACC.  If you have changed the name of the\n");
  printf("  input file or need to specify a path, you will need to   \n");
  printf("  modify this program.  All data is expected to be in the  \n");
  printf("  current working directory.                               \n");
  printf("\n\n Platforms supported: SUN \n");
  printf("                      HP \n");
  printf("                      SGI \n");
  printf("                      Dec Alpha \n");
  printf(" \n\n  Contact: Langley DAAC User and Data Services Office \n");
  printf("           NASA Langley Research Center \n");
  printf("           Mail Stop 157D \n");
  printf("           Hampton, VA 23681-0001 \n");
  printf("           Phone (804)864-8656   Fax (804)864-8807 \n");
  printf("**********************************************************\n\n\n");
    printf("\n\n Hit a carriage control to continue. \n");
    scanf("%c",&cont);
return;
}

