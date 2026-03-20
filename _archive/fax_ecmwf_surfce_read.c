#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

#define MAXBUF 81

main (argc,argv)
 int argc;
 char *argv[];
{  char fldabbr[4],trange,field[40],units[10];
   char filename[15];
   char id[4],sp[4];
   char t2[5],title[6],mon[3],day[3],hr[3],t3[6];
   char temp[MAXBUF],temp2[MAXBUF],t4[5];
   char loni[7],lonf[6],dlon[5],lati[6],latf[6],dlat[5];
   char  nlon[3],nlat[3],surp[4],surt[4];
   float longit[82],latit[50],fdata;
   float data[82][50];
   char test[13];
   int i,j,rec;
   FILE *f1;

   strcpy(surp,"134");
   strcpy(surt,"139");
/* the latitude and longitude range is from 85W to 10E, 70N to 15N at surface pressure 
   SP represents Surface pressure in Pascals and ST is the Sea Surface Temperature 
   in degrees Kelvin */

   banner(argc);
   if (argc == 1) {
     printf("\nPlease enter in the input file name\n\n"); 
     scanf("%s",filename);
   } 
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
   i=0; 
   j=0;
   for (i=0;i<atoi(nlon);i++) {
   printf("\n\n     Longitude           Latitude");
   if (strcmp(id,surp)==0) 
     printf("           Surface Pressure (Pa)\n");
   else 
     printf("           Sea Surface Temperature (K)\n");
   printf("---------------------------------------------------------------\n");
    for (j=0;j<atoi(nlat);j++) {
     fscanf(f1,"%s",test);
     data[i][j] = atof(test);
     printf( "     %f         %f         %13.6f\n", longit[i],latit[j], data[i][j]);
     latit[j+1] = latit[j]-atof(dlat); 
    }     
   longit[i+1]=longit[i] + atof(dlon);
   
   }

     printf(" \n\nThe Read Program for ASTEX Data Set from ECMWF has completed.\n");
     rec = atoi(nlat)*atoi(nlon);
     printf(" There were %d records displayed\n", rec);

}


banner(argc)
{
  char cont;

  printf("**********************************************************\n\n");
  printf("  FIRE ASTEX Surface from ECMWF READ PROGRAM \n\n");
  printf("  Version 1.0                                  July 1995 \n\n");
  printf("  This program reads the Surface Data generated in June 1992. The\n");
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

