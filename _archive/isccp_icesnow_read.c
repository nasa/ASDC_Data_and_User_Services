/*****************************************************************************/
/*  Program name: isccp_icesnow_read.c                                       */
/*  Purpose: To read ISCCP Ice/Snow information from data files.             */
/*           File 4 is a ancillary file which must be accessed first,        */
/*           then files 5 through the last file are data files which         */
/*           are access seperately. This is in part a "C" version of         */
/*           the fortran version supplied with the data files, but in        */
/*           this file contains code to allow the user to see the icesnow    */
/*           data from the data files and the land cover fractions from the  */
/*           ancillary data file. It also contains changes made to error     */
/*           check user input of latitude and longitude values.  The user    */
/*           can choose to see the center values with the icesnow codes or   */
/*           a table that contains the icesnow codes or the land cover       */
/*           fraction over a range of latitudes, or both tables.             */
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

#define maxbox 41252
#define maxlon 360
#define maxlat 180
#define iundef 255
#define SIZE   80
#define numbox 10400
#define ioff   88 

      
static int ncells [180]=
           {0,    3,   12,   28,   50,   78,  113,  154,  201,  254,
          313,  379,  451,  529,  613,  703,  799,  901, 1009, 1123,
         1243, 1369, 1501, 1639, 1783, 1932, 2087, 2248, 2414, 2586,
         2763, 2946, 3134, 3327, 3526, 3730, 3939, 4153, 4372, 4596,
         4825, 5059, 5298, 5541, 5789, 6041, 6298, 6559, 6824, 7094,
         7368, 7646, 7928, 8214, 8503, 8796, 9093, 9393, 9697,10004,
        10314,10627,10943,11262,11584,11909,12237,12567,12900,13235,
        13572,13911,14252,14595,14940,15287,15636,15986,16337,16690,
        17044,17399,17755,18112,18470,18828,19187,19546,19906,20266,
        20626,20986,21346,21706,22065,22424,22782,23140,23497,23853,
        24208,24562,24915,25266,25616,25965,26312,26657,27000,27341,
        27680,28017,28352,28685,29015,29343,29668,29990,30309,30625,
        30938,31248,31555,31859,32159,32456,32749,33038,33324,33606,
        33884,34158,34428,34693,34954,35211,35463,35711,35954,36193,
        36427,36656,36880,37099,37313,37522,37726,37925,38118,38306,
        38489,38666,38838,39004,39165,39320,39469,39613,39751,39883,
        40009,40129,40243,40351,40453,40549,40639,40723,40801,40873,
        40939,40998,41051,41098,41139,41174,41202,41224,41240,41249};

      /* This ncells array corresponds to the first index in a latitude band */

static int icells [180] = 
           {3,    9,   16,   22,   28,   35,   41,   47,   53,   59,
           66,   72,   78,   84,   90,   96,  102,  108,  114,  120,
          126,  132,  138,  144,  149,  155,  161,  166,  172,  177,
          183,  188,  193,  199,  204,  209,  214,  219,  224,  229,
          234,  239,  243,  248,  252,  257,  261,  265,  270,  274,
          278,  282,  286,  289,  293,  297,  300,  304,  307,  310,
          313,  316,  319,  322,  325,  328,  330,  333,  335,  337,
          339,  341,  343,  345,  347,  349,  350,  351,  353,  354,
          355,  356,  357,  358,  358,  359,  359,  360,  360,  360,
          360,  360,  360,  359,  359,  358,  358,  357,  356,  355,
          354,  353,  351,  350,  349,  347,  345,  343,  341,  339,
          337,  335,  333,  330,  328,  325,  322,  319,  316,  313,
          310,  307,  304,  300,  297,  293,  289,  286,  282,  278,
          274,  270,  265,  261,  257,  252,  248,  243,  239,  234,
          229,  224,  219,  214,  209,  204,  199,  193,  188,  183,
          177,  172,  166,  161,  155,  149,  144,  138,  132,  126,
          120,  114,  108,  102,   96,   90,   84,   78,   72,   66,
           59,   53,   47,   41,   35,   28,   22,   16,    9,    3};


     /*  This icells array corresponds to the number of segments in a */      
     /*   latitude band                                                 */

FILE *anfp;
   char anfilename[256];     
FILE *dfp;
char filename[256];

FILE *ofp;
char outputfile[256];

  char buf[SIZE]; /*for reading header from the ancillary file */
unsigned char chrbuf[numbox]; /* for reading data block from the data files */

typedef struct ivalues{
        int centervalue;
        int landcover;
      }value;

value eqmap[maxbox];            /* Equal-area grid map                    */
value sqmap[maxlat][maxlon];    /* Square map                               */
value ivar[maxlon];

int lonlim[maxbox][2];        
int ifile, jrec, idatyp, latbeg, latend, iyear, month, iday, isisrc, 
    isiyy1,isimm1,isidd1,isiyy2,isimm2,isidd2,isiyy3,isimm3,isidd3,
    isiyy4,isimm4,isidd4, isnsrc,isnyy1,isnmm1,isndd1, lonbeg,lonend,
    nprev,ibxbeg, ibxend, nlon, irec,ibox,lat,lon;
char choice;
int beglon,beglat,endlat,endlon;

  

void main()
{ 
  void display_main();
  void display_menu();
  
  display_main();
  display_menu();
  printf("\n");
  printf("PROGRAM HAS ENDED \n\n");
  close(outputfile);
  close(filename);
}

void display_main()
 {
  /* Purpose: This section display a menu which contains general information about  */
  /*          the providers of the program.                                         */

  printf("\n\n\n\n");
  printf("\n\n\n\n");
  printf("\n\n\n\n");
  printf("\n\n\n\n");
  printf("        *********************************************************************\n");
  printf("        *                                                                   *\n");
  printf("        *                    Langley DAAC                                   *\n");
  printf("        *        PROJECT_LEVEL/DATA_SET_LEVEL READ SOFTWARE                 *\n");
  printf("        *  VERSION NUMBER:                                                  *\n");
  printf("        *  Language: C                                                      *\n");
  printf("        *  Platforms supported:  SUN                                        *\n");
  printf("        *                        HP                                         *\n");
  printf("        *                        SGI                                        *\n");
  printf("        *                        Solaris 2.3                                *\n");
  printf("        * Contact:  Langley DAAC user and Data Services Office              *\n");
  printf("        *           NASA Langley Research Center                            *\n");
  printf("        *           Mail Stop 157D                                          *\n");
  printf("        *           Hampton, VA 23681-0001                                  *\n");
  printf("        *           PHONE:   (757)864-8656            FAX: (757)864-8807    *\n");
  printf("        *                                                                   *\n");
  printf("        *********************************************************************\n");

  printf("\n\n\n\n\n");
  
 }

void display_menu()
  { 
    /* Purpose: allow the user to choose which type of information they want */
    
  void process_files();
  void display_help();

    char ans;
   
     printf("\n\n");
     printf("Enter 'c' to view the ice and snow values for a specified latitude band. \n");
     printf("Enter 's' to view a square map of the ice and snow values for a specified \n");
     printf("          latitude and longitude region. \n"); 
     printf("Enter 'h' to receive help with commands. \n");
     printf("Enter 'x' to exit or terminate processing. \n");
     printf("\n\n");
     scanf("%s",&ans);
     choice = ans;
   
    switch (choice)
      {
      case 'C':
      case 'c':
      case 'S':
      case 's': process_files();
                break;
      case 'h':
      case 'H': display_help();
                break;
      case 'x':
      case 'X': printf("Termination requested - ");
                break;
      default: 
              printf("Invalid menu entry! Please try again.\n");
              display_menu();
              break;
      }
  }

void display_help()
 { /* Purpose: To aid the user in making a choice */
     

     printf("\n\n\n\n");
     printf("\n\n\n\n");
     printf("\n\n\n\n");
     printf("*********************************************************************\n");
     printf("*                                                                   *\n");
     printf("*                       HELP COMMAND                                *\n");
     printf("*                                                                   *\n");
     printf("*********************************************************************\n"); 
     printf("Option selection 'c' allows you to print out the ice snow code and   \n");
     printf("the land cover fraction value for a user specified latitude.         \n");
     printf("Option selection 's' allows you to view a square map of the ice snow \n");
     printf("values for a user specified latitude and longitude range.            \n");     
     
     display_menu();
  }

void process_files()
{ 
  /* Purpose: process the data files for user determined output */
  void rdanc();
  void isopen();
  void openout();
  void isread();
  void center();
  void eq2sq();
  void display_tables_menu();
 
  int ifull,itotal,i,j,k,code,res,ans,index;
  
  char userlat[4];
  char cont;
  int done=0;
  

  rdanc();
  
  printf("\n");
  printf("Read software will process data files until you wish to stop.\n");
  printf("Press 'Y' or 'y' to continue or 'x' to stop.              \n");
  scanf("%s",&cont); 
  while ((cont== 'Y') || (cont == 'y'))
    {
     isopen();
     openout();
     printf("\nOpened data file: %s \n",filename);
     fprintf(ofp,"\n\n     The data file used is %s",filename);
     ibxbeg=0;
     ibxend=0;
     
     irec=0;
     ifull=0;
     itotal=0;
  
     if ((choice=='c')||(choice=='C'))
      {  /* prompt user for latitude index and verify input is in correct range. */
       while (!done)
        {
         printf("\n");
         printf("Enter desired latitude band: \n");
         printf("(Value should be between 1 -> 180; Latitude\n");
         printf(" bands are in 1 degree increments, starting \n");
         printf(" at the South Pole.)\n");
         
         scanf("%s",userlat);
         res = isalpha(userlat[0]); 
         if (res !=0)
           { printf("Your selection of %s is invalid. \n",userlat);
             printf("Please try again.\n");}
         else{
            ans = atoi(userlat);   
            if ((ans < 1) || (ans > 180))
             { printf("\n");
               printf("Latitude entered is not within the proper range \n");
               printf("Please try again.n");
             }
             else
              done = 1;
	  }
         ans = ans-1;
       }/* end while */
      
        
        
      }
 

     for (lat=0; lat <= maxlat-1; lat++)
      { 
       isread();
       
       for (lon=1;lon <= nlon; lon++)
	 { ibox = nprev + lon;
           itotal++;
           if (ivar[lon].centervalue != 255) ifull++;

           if ((choice=='c')||(choice=='C'))
            {  
             /*output center value for latitude requested by user */
             
             if ((lat == ans)&&(lon <= icells[ans])) 
              {
               fprintf(ofp,"*******************************************************\n");
               fprintf(ofp,"  processing equal-area lon/lat %d %d\n",lon,ans+1);
               center(lon-1);
               if (lat ==0)
                  fprintf(ofp,"    Ice Snow Cover code value: %d \n",ivar[lon].centervalue);
               else
                  fprintf(ofp,"    Ice Snow Cover code value: %d \n",ivar[lon-1].centervalue);
               index = ncells[ans]+lon;
               fprintf(ofp,"  Land Cover percent value : %d \n",eqmap[index].landcover);
              }
            }
           eqmap[ibox].centervalue = ivar[lon].centervalue;
         }
     
     }/* end for */
    printf("Number of full boxes in data file: %6d out of %6d\n",ifull,itotal);
    eq2sq(1);

 /* user can select a latitude and longitude range to view the ice/snow codes */
    if ((choice == 's')||(choice =='S'))
     { display_tables_menu();
     
                 
      }/* end if*/
    printf("\n");
    printf("Do you wish to process another data file? ");
    scanf("%s",&cont);  
   }/* end while */
  }/* end process_files */
 
  /*******************************************************************************/
  /* THIS SECTION CONTAINS THE FUNCTIONS USED TO INPUT THE DATA FILES            */
  /*******************************************************************************/
void rdanc()
   /* read icesnow ancillary data file; */
   /* begin and end longitude values are taken from this file */

  { int p,j,i,cenlat,cenlon,iarea,landfr,l;
    
   

         /* These used to read variables from the buffer buf[]  */
   char sibox[6];
   char sj[6];
   char si[6];
   char slonbeg[6];
   char slonend[6]; 
   char sland[6];

  /* initialize lonlim array */
    for (l =0; l <=maxbox;l++)
      { lonlim[l][0]=iundef;
        lonlim[l][1]=iundef;
      }
                                             
   printf ("Enter ancillary filename:");
   scanf ("%s",anfilename);
    
    anfp = fopen(anfilename,"r");
    printf("Ancillary data file is %s\n", anfilename);
    if (anfp == NULL)
     { 
      printf("************************************************************\n");
      printf("*  ERROR - Unable to open the data file  %s *\n",anfilename);
      printf("*           Program has terminated                         *\n");
      printf("************************************************************\n");
      exit(-1);
     }
    fread ((char *) &buf[0],sizeof(unsigned char), SIZE, anfp);
    /*printf("%s\n",buf);*/
    fread ((char *) &buf[0],sizeof(unsigned char), SIZE, anfp);
    /*printf("%s\n",buf);*/

    for (irec =1; irec <= maxbox; irec++){
       fread((char *) &buf[0],sizeof (unsigned char), SIZE, anfp);
         
        /* following commented printf lines are for debugging */
     /*  printf("%s\n",buf);*/
       strncpy(sibox,buf,6);
       ibox = atoi(sibox);
    /*   printf("%d ",ibox);*/
     /*  strncpy(sj,buf+6,8);*/
     /*  printf("%d ",atoi(sj));*/
     /*  strncpy(si,buf+14,6);*/
     /*  printf("%d ",atoi(si));*/
       strncpy(slonbeg,buf+20,6);
       lonbeg = atoi(slonbeg);
     /*  printf("%d \n ",lonbeg);*/
       strncpy(slonend,buf+26,6);
       lonend = atoi(slonend);
     /*  printf("%d\n",lonend);*/
        lonlim[ibox-1][0] = lonbeg;
        lonlim[ibox-1][1] = lonend;
        strncpy(sland,buf+63,10);
        landfr = atoi(sland);
        eqmap[ibox-1].landcover = landfr; 
                
      }
 
    close(anfilename);
  }

void isopen()
  {
    /*open icesnow data file and output file*/
   
  printf("Enter data file to be used: ");
  scanf("%s",filename);
 
  dfp = fopen(filename, "r");
  if (dfp == NULL)
    {
      printf("************************************************************\n");
      printf("* ERROR - Unable to open the data file %s *\n",filename);
      printf("*           Program has terminated                         *\n");
      printf("************************************************************\n");
      exit(-1);
    }
  
  }

void openout()
  {/* Purpose: to open an output file*/ 
   char ans;
  printf("\n\n");
  printf("Enter a name for the output file: ");
  scanf("%s",outputfile);
  ofp=fopen(outputfile,"r");
  if (ofp != NULL)
    {
     printf("***************************************************************\n");   
     printf("     WARNING: This filename exists -- %s\n",outputfile);
     printf("      Do you wish to overwrite it? 'y' yes 'n' no             \n");
     printf("***************************************************************\n");
     scanf("%s",&ans);
     if ((ans == 'n') || ( ans == 'N')) openout();
    }
  ofp = fopen(outputfile,"w");

 /* ofp=fopen("isout", "w");*/
 /* printf(" Output file is isout.\n");*/
   
  }


void isread()
  { 
    /* Purpose: read an ice/snow data file and store values in ivar array*/
    
    void isrec();

    int  nbox, idecod;
   
  /* initialize the output arrays ivar */
    for (lon=0; lon <=maxlon;lon++)
       { ivar[lon].centervalue = iundef;
         ivar[lon].landcover = iundef;}
   
    /* loop over all boxes for this lat */
    
    nlon = icells[lat];
   
    nprev = ncells[lat];
    for (lon=0;lon<=nlon;lon++)
       {
         if (lon==0) 
           nbox = nprev + lon+1;
         else
           nbox = nprev + lon;
         if (nbox < ibxbeg)
                {printf ("read past latitude zone\n"); }
         
         if (nbox <= ibxend)
              { 
                idecod=(nbox-ibxbeg)+ioff;
                ivar[lon].centervalue = (chrbuf[idecod]);
               /* if(lat ==179) printf ("lon=%d, value=%d",lon,ivar[lon].centervalue);*/
              }
            else
               isrec();
        
	  
       }
  }

void isrec()
  { /* read an ice/snow data record and unpack record prefix */

    int lon1, lon2,q;
    
   
     irec++;

     fread((char *) &chrbuf[0], sizeof(unsigned char), numbox,dfp);
    
     if (feof(dfp) == 0)
       { 
      
       
    /* decode the prefix information for this record */

      ifile  = (chrbuf[0]);
      jrec   = (chrbuf[1]);
      idatyp = (chrbuf[2]);
      latbeg = (chrbuf[3]);
      latend = (chrbuf[4]);
      iyear  = (chrbuf[5]);
      month  = (chrbuf[6]);
      iday   = (chrbuf[7]);
      isisrc = (chrbuf[8]);
      isiyy1 = (chrbuf[9]);
      isimm1 = (chrbuf[10]);
      isidd1 = (chrbuf[11]);
      isiyy2 = (chrbuf[12]);
      isimm2 = (chrbuf[13]);
      isidd2 = (chrbuf[14]);
      isiyy3 = (chrbuf[15]);  
      isimm3 = (chrbuf[16]);
      isidd3 = (chrbuf[17]);
      isiyy4 = (chrbuf[18]);
      isimm4 = (chrbuf[19]);
      isidd4 = (chrbuf[20]);
      isnsrc = (chrbuf[21]);
      isnyy1 = (chrbuf[22]);
      isnmm1 = (chrbuf[23]);
      isndd1 = (chrbuf[24]);
      lon1   = (chrbuf[25]);
      lon2   = (chrbuf[26]);
      lonbeg = lon1+lon2;
      lon1   = (chrbuf[27]);
      lon2   = (chrbuf[28]);
      lonend = lon1 + lon2;
  /* following printf is for debug purposes; */
  /* it list the header infor for each record in a file */
  /*   printf("data from chrbuf %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n\n",ifile,jrec,idatyp,latbeg,latend,iyear,month,iday,isisrc,isiyy1,isimm1,isidd1,isiyy2,isimm2,isidd2,isiyy3,isimm3,isidd3,isiyy4,isimm4,isidd4,isnsrc,isnyy1,isnmm1,isndd1,chrbuf[25],chrbuf[26],chrbuf[27],chrbuf[28]);*/
 
    
      fprintf(ofp,"\n");
      if (irec == 1){
          if (isisrc == 1){
            fprintf(ofp, "IS PREFIX INFORMATION: YEAR %5d MONTH %3d DAY %3d\n",iyear,month,  iday);
            fprintf(ofp," SEAICE: SOURCE USJC \n");
            fprintf(ofp,"   NORTH-EAST: YEAR %5d MONTH %3d DAY %3d \n", isiyy1,isimm1, isidd1);
            fprintf(ofp,"   NORTH-WEST: YEAR %5d MONTH %3d DAY %3d \n",isiyy2,isimm2, isidd2);
            fprintf(ofp,"   SOUTH:      YEAR %5d MONTH %3d DAY %3d \n ",isiyy3,isimm3, isidd3);
            fprintf(ofp,"SNOW: SOURCE NOAA  YEAR %5d MONTH %3d DAY %3d\n",isnyy1,isnmm1,isndd1);}
          else
           {fprintf(ofp, "IS PREFIX INFORMATION:  ");
            fprintf(ofp,"   DATE: YEAR %5d MONTH %3d DAY %3d\n",iyear,month, iday); 
            fprintf(ofp," SEAICE: SOURCE SSMI \n");
            fprintf(ofp,"   NORTH: YEAR %5d MONTH %3d DAY %3d TO  YEAR %5d MONTH %3d DAY %3d\n", isiyy1,isimm1, isidd1,isiyy1,isimm2, isidd2);
            fprintf(ofp,"   SOUTH: YEAR %5d MONTH %3d DAY %3d TO YEAR %5d MONTH %3d DAY %3d \n",isiyy3,isimm3, isidd3,isiyy4,isimm4,isidd4);
            fprintf(ofp,"SNOW: SOURCE NOAA  YEAR %5d MONTH %3d DAY %3d\n",isnyy1,isnmm1,isndd1); } 
     if (idatyp != 0) printf("is data record - data type is not valid\n");
     if ((isisrc != 1)&&(isisrc != 2)) printf("SEAICE source is not valid for file %s\n",filename);
     if (isnsrc != 1) printf("SNOW source is not valid for file %s\n",filename);
                 
       }
     ibxbeg = ncells[latbeg-1]+lonbeg;
     ibxend = ncells[latend-1]+lonend;
  };
}

void center(lon)
   /* calculate center lon/lat of box (equal-area grid) */

  {float dlon;
   float cenlat,cenlon;
   int dlat=1;

   dlon = 360.0/nlon;
   
   cenlat = (lat ) * dlat + dlat/2.0 - 90.0;
   cenlon = (lon ) * dlon + dlon/2.0;
   fprintf(ofp,"CENTER: CENTER LON/LAT %8.2f%8.2f\n",cenlon,cenlat);

  }

void eq2sq( ishift)
  /* convert equal area map to square lat/lon map for display purposes */
  /* ishift = 1 => shift longitudes to be in range -180 to +180        */
  /* ishift any other value => keep longitudes in range 0 to 360       */

  { int lonsq1, lonsq2,ilon,lonsq,abox;

   abox=0;
   for (lat=0;lat < maxlat; lat++){
      for (lon=0; lon < icells[lat]; lon++){
         
         
         lonsq1 = lonlim[abox][0];
         lonsq2 = lonlim[abox][1];
        
         for (ilon=lonsq1; ilon<=lonsq2;ilon++){
            lonsq = ilon;
            if (ishift == 1){
               lonsq = lonsq + (maxlon/2);
               if (lonsq >= maxlon)
                  lonsq = lonsq - maxlon;
	     }/*endif*/
           
            sqmap[lat][lonsq] = eqmap[abox];
          
         }/*end for ilon*/
         abox++;
       }/*end for lon*/
    }/*end for lat*/
  }



  /***************************************************************************/
  /*     THIS SECTION CONTAINS THE FUNCTION USED TO PRODUCE OUTPUT           */
  /***************************************************************************/

void display_tables_menu()
{   void get_range();
    void display_IS_table();
    void display_LC_table();
   
    char codetype;
   
      printf("\n");
      printf("For Ice/Snow codes enter : I \n");
      printf("For Land Cover values enter : L\n");
      printf("For Both enter : B \n");
     
      scanf("%s",&codetype);
      
      switch(codetype)
	{
	case 'I':
        case 'i': get_range();
                  display_IS_table();
                  break;
        case 'L':
        case 'l': get_range();
                  display_LC_table();
                  break;
        case 'B':
        case 'b': get_range();
                  display_IS_table();
                  display_LC_table();
                  break;
        
        default: printf(" You have entered an erroneous selection.\n");
                 printf(" Please try again.\n");
                 display_tables_menu();
                 break;
	}
} 




void get_range()
{  
   char usertude[4];
   int done1=0;
   int done2=0;
   int done3=0;
   int done4=0;
   int res;
    
   printf("                                                            \n");
   printf("For readability, ranges should be kept small; i.e., 0 to 10.\n");
   printf("                                                            \n");
   printf("Latitude bands are in 1 degree increments starting at the   \n");
   printf("South Pole; longitude bands are 1 degree increments starting\n");
   printf("at the Greenwich meridian.\n");
   while (!done1)
    { printf("\n");
      printf("Enter beginning longitude band:(1-360 degrees) ");
      scanf("%s",usertude);
      res = isalpha(usertude[0]); 
      if (res !=0)
         { printf("Your selection of %s is invalid. \n",usertude);
           printf("Please try again.\n");}
       else{
          beglon = atoi(usertude);   
          if ((beglon < 1) || (beglon > 360))
            { printf("\n");
              printf("Longitude entered is not within the proper range \n");
              printf("Please try again.\n");
             }
           else
             done1 = 1;
	  }
         beglon= beglon-1;
       }/* end while */
    while (!done2)
    { printf("\n");
      printf("Enter ending longitude band:(1-360 degrees) ");
      scanf("%s",usertude);
      res = isalpha(usertude[0]); 
      if (res !=0)
         { printf("Your selection of %s is invalid. \n",usertude);
           printf("Please try again.\n");}
       else{
          endlon = atoi(usertude);   
          if ((endlon < 0) || (endlon > 360))
            { printf("\n");
              printf("Longitude entered is not within the proper range \n");
              printf("Please try again.\n");
             }
           else
             done2 = 1;
	  }
        endlon = endlon-1;
       }/* end while */

    while (!done3)
    { printf("\n");
      printf("Enter beginning latitude band:(1-180 degrees) \n");
      
      scanf("%s",usertude);
      res = isalpha(usertude[0]); 
      if (res !=0)
         { printf("Your selection of %s is invalid. \n",usertude);
           printf("Please try again.\n");}
       else{
          beglat = atoi(usertude);   
          if ((beglat < 0) || (beglat > 179))
            { printf("\n");
              printf("Latitude entered is not within the proper range \n");
              printf("Please try again.\n");
             }
           else
             done3 = 1;
	  }
        beglat = beglat-1;
       }/* end while */

    while (!done4)
    { printf("\n");
      printf("Enter ending latitude band:(1-180 degrees) ");
      scanf("%s",usertude);
      res = isalpha(usertude[0]); 
      if (res !=0)
         { printf("Your selection of %s is invalid. \n",usertude);
           printf("Please try again.\n");}
       else{
          endlat = atoi(usertude);   
          if ((endlat < 1) || (endlat > 180))
            { printf("\n");
              printf("Latitude entered is not within the proper range \n");
              printf("Please try again.\n");
             }
           else
             done4 = 1;
	  }
        endlat = endlat-1;
       }/* end while */
      
          
                  
    }

void display_IS_table()
{ int i,k,j;
       
      fprintf(ofp,"\n");
      fprintf(ofp,"Ice/Snow Cover codes \n");
      fprintf(ofp,"latitude /longitude index numbers\n");
      fprintf(ofp,"          ");
      for (i=beglon;i<=endlon;i++)
        {fprintf(ofp," %4d  ",i+1);}
      fprintf(ofp,"\n");
      fprintf(ofp,"          ");
      for (i=1;i<=((endlon-beglon+1)*7);i++)
         fprintf(ofp,"-");
      fprintf(ofp,"\n");
      for (k=beglat;k<=endlat;k++)
         {
           fprintf(ofp," %3d    | ",k+1);
           for (j=beglon;j<=endlon;j++)
              { fprintf(ofp," %4d  ",sqmap[k][j].centervalue);}
           fprintf(ofp,"\n");                                                     
         }  
    }

void display_LC_table()
{ int i,k,j;

      fprintf(ofp,"\n");
      fprintf(ofp,"Land Cover fraction values \n");
      fprintf(ofp,"latitude /longitude index numbers\n");
      fprintf(ofp,"          ");
      for (i=beglon;i<=endlon;i++)
        {fprintf(ofp," %4d  ",i+1);}
      fprintf(ofp,"\n");
      fprintf(ofp,"          ");
      for (i=1;i<=((endlon-beglon+1)*7);i++)
         fprintf(ofp,"-");
      fprintf(ofp,"\n");
      for (k=beglat;k<=endlat;k++)
         {
           fprintf(ofp," %3d    | ",k+1);
           for (j=beglon;j<=endlon;j++)
              { fprintf(ofp," %4d  ",sqmap[k][j].landcover);}
           fprintf(ofp,"\n");                                                     
         }  
}
