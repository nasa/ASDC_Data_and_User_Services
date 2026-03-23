/*****************************************************************************/
/*   Program name: isccp_tovs_read.c                                         */
/*   Purpose: To read ISCCP TOVS data files.                                 */
/*            File 4 is an ancillary file which must be accessed first.      */
/*            Files 5 through the last file are data files; they are divided */
/*            into clim monthly files, tovs monthly files and tovs daily     */
/*            files.  This program is in part a "C" version of the fortran   */
/*            version supplied with the data files.  Added to this file are  */
/*            user interactive coding.                                       */
/*****************************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>


#define maxvar 30  /* number of variables reported in each tovs profile*/
#define maxlon 144 /* grids */
#define maxlat 72
#define maxbox 6596
#define iundef 255         /* undefined value for integer values */
#define rundef -1000.0     /* undefined value for floating point values */
#define maxcnt 255
#define numbox 16530
#define SIZE   80

   /* set arrays */
static float tmptab [256]=
     { -100.000,165.000,169.000,172.000,175.000,177.800,180.500,
        183.000,185.500,187.800,190.000,192.000,194.000,195.700,
        197.500,199.200,201.000,202.700,204.500,206.200,208.000,
        209.700,211.500,212.800,214.100,215.400,216.700,217.900,
        219.200,220.500,221.800,223.100,224.400,225.400,226.500,
        227.500,228.600,229.600,230.600,231.700,232.700,233.800,
        234.800,235.700,236.600,237.500,238.400,239.200,240.100,
        241.000,241.900,242.800,243.700,244.500,245.300,246.100,
        246.900,247.700,248.500,249.300,250.100,250.900,251.700,
        252.400,253.100,253.900,254.600,255.300,256.000,256.700,
        257.500,258.200,258.900,259.500,260.200,260.800,261.500,
        262.100,262.800,263.400,264.100,264.700,265.400,266.000,
        266.600,267.200,267.800,268.400,269.100,269.700,270.300,
        270.900,271.500,272.100,272.700,273.200,273.800,274.400,
        275.000,275.600,276.100,276.700,277.300,277.800,278.400,
        278.900,279.500,280.000,280.500,281.100,281.600,282.200,
        282.700,283.200,283.700,284.200,284.700,285.200,285.800,
        286.300,286.800,287.300,287.800,288.300,288.800,289.300,
        289.800,290.200,290.700,291.200,291.700,292.200,292.700,
        293.200,293.600,294.100,294.600,295.000,295.500,296.000,
        296.500,296.900,297.400,297.800,298.300,298.700,299.200,
        299.600,300.100,300.500,301.000,301.400,301.900,302.300,
        302.800,303.200,303.600,304.000,304.500,304.900,305.300,
        305.800,306.200,306.600,307.000,307.500,307.900,308.300,
        308.700,309.100,309.600,310.000,310.400,310.800,311.200,
        311.600,312.000,312.400,312.900,313.300,313.700,314.100,
        314.500,314.900,315.300,315.700,316.100,316.400,316.800,
        317.200,317.600,318.000,318.400,318.800,319.200,319.500,
        319.900,320.300,320.700,321.100,321.400,321.800,322.200,
        322.600,323.000,323.300,323.700,324.100,324.500,324.900,
        325.200,325.600,326.000,326.400,326.700,327.100,327.400,
        327.800,328.200,328.500,328.900,329.200,329.600,329.900,
        330.300,330.600,331.000,331.300,331.700,332.000,332.400,
        332.700,333.100,333.400,333.800,334.100,334.500,334.800,
        335.200,335.500,335.900,336.200,336.600,336.900,337.300,
        337.600,338.000,338.300,338.600,339.000,339.300,339.700,
        340.000,345.000,-200.000,-1000.000};

 static float pretab[256]=
    {  -100.00,  1.00, 5.00, 10.00,15.00,20.00,25.00,30.00,35.00,40.00,
         45.00, 50.00, 55.00,60.00,65.00,70.00,75.00,80.00,85.00,90.00,
         95.00,100.00,105.00,110.00,115.00,120.00,125.00,130.00,135.00,
        140.00,145.00,150.00,155.00,160.00,165.00,170.00,175.00,180.00,
        185.00,190.00,195.00,200.00,205.00,210.00,215.00,220.00,225.00,
        230.00,235.00,240.00,245.00,250.00,255.00,260.00,265.00,270.00,
        275.00,280.00,285.00,290.00,295.00,300.00,305.00,310.00,315.00,
        320.00,325.00,330.00,335.00,340.00,345.00,350.00,355.00,360.00,
        365.00,370.00,375.00,380.00,385.00,390.00,395.00,400.00,405.00,
        410.00,415.00,420.00,425.00,430.00,435.00,440.00,445.00,450.00,
        455.00,460.00,465.00,470.00,475.00,480.00,485.00,490.00,495.00,
        500.00,505.00,510.00,515.00,520.00,525.00,530.00,535.00,540.00,
        545.00,550.00,555.00,560.00,565.00,570.00,575.00,580.00,585.00,
        590.00,595.00,600.00,605.00,610.00,615.00,620.00,625.00,630.00,
        635.00,640.00,645.00,650.00,655.00,660.00,665.00,670.00,675.00,
        680.00,685.00,690.00,695.00,700.00,705.00,710.00,715.00,720.00,
        725.00,730.00,735.00,740.00,745.00,750.00,755.00,760.00,765.00,
        770.00,775.00,780.00,785.00,790.00,795.00,800.00,805.00,810.00,
        815.00,820.00,825.00,830.00,835.00,840.00,845.00,850.00,855.00,
        860.00,865.00,870.00,875.00,880.00,885.00,890.00,895.00,900.00,
        905.00,910.00,915.00,920.00,925.00,930.00,935.00,940.00,945.00,
        950.00,955.00,960.00,965.00,970.00,975.00,980.00,985.00,990.00,
        995.00,1000.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,
        -200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,
        -200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,
        -200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,
        -200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,
        -200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,
        -200.00,-200.00,-200.00 -200.00,-200.00,-200.00,-200.00,-200.00,
        -1000.00};

static float prwtab[maxcnt+1]=
{    -100.000,0.000,0.030,0.060,0.090,0.120,0.150,0.180,0.210,0.240,
        0.270,0.300,0.330,0.360,0.390,0.420,0.450,0.480,0.510,0.540,
        0.570,0.600,0.630,0.660,0.690,0.720,0.750,0.780,0.810,0.840,
        0.870,0.900,0.930,0.960,0.990,1.020,1.050,1.080,1.110,1.140,
        1.170,1.200,1.230,1.260,1.290,1.320,1.350,1.380,1.410,1.440,
        1.470,1.500,1.530,1.560,1.590,1.620,1.650,1.680,1.710,1.740,
        1.770,1.800,1.830,1.860,1.890,1.920,1.950,1.980,2.010,2.040,
        2.070,2.100,2.130,2.160,2.190,2.220,2.250,2.280,2.310,2.340,
        2.370,2.400,2.430,2.460,2.490,2.520,2.550,2.580,2.610,2.640,
        2.670,2.700,2.730,2.760,2.790,2.820,2.850,2.880,2.910,2.940,
        2.970,3.000,3.030,3.060,3.090,3.120,3.150,3.180,3.210,3.240,
        3.270,3.300,3.330,3.360,3.390,3.420,3.450,3.480,3.510,3.540,
        3.570,3.600,3.630,3.660,3.690,3.720,3.750,3.780,3.810,3.840,
        3.870,3.900,3.930,3.960,3.990,4.020,4.050,4.080,4.110,4.140,
        4.170,4.200,4.230,4.260,4.290,4.320,4.350,4.380,4.410,4.440,
        4.470,4.500,4.530,4.560,4.590,4.620,4.650,4.680,4.710,4.740,
        4.770,4.800,4.830,4.860,4.890,4.920,4.950,4.980,5.010,5.040,
        5.070,5.100,5.130,5.160,5.190,5.220,5.250,5.280,5.310,5.340,
        5.370,5.400,5.430,5.460,5.490,5.520,5.550,5.580,5.610,5.640,
        5.670,5.700,5.730,5.760,5.790,5.820,5.850,5.880,5.910,5.940,
        5.970,6.000,6.030,6.060,6.090,6.120,6.150,6.180,6.210,6.240,
        6.270,6.300,6.330,6.360,6.390,6.420,6.450,6.480,6.510,6.540,
        6.570,6.600,6.630,6.660,6.690,6.720,6.750,6.780,6.810,6.840,
        6.870,6.900,6.930,6.960,6.990,7.020,7.050,7.080,7.110,7.140,
        7.170,7.200,7.230,7.260,7.290,7.320,7.350,7.380,7.410,7.440,
        7.470,7.500,7.650,8.000,-200.000,-1000.000};

static float ozntab[maxcnt+1]=
{     -100.0,0.0,2.0,4.0,6.0,8.0,10.0,12.0,14.0,16.0,18.0,20.0,22.0,
        24.0,26.0,28.0,30.0,32.0,34.0,36.0,38.0,40.0,42.0,44.0,46.0,
        48.0,50.0,52.0,54.0,56.0,58.0,60.0,62.0,64.0,66.0,68.0,70.0,
        72.0,74.0,76.0,78.0,80.0,82.0,84.0,86.0,88.0,90.0,92.0,94.0,
        96.0,98.0,100.0,102.0,104.0,106.0,108.0,110.0,112.0,114.0,
        116.0,118.0,120.0,122.0,124.0,126.0,128.0,130.0,132.0,134.0,
        136.0,138.0,140.0,142.0,144.0,146.0,148.0,150.0,152.0,154.0,
        156.0,158.0,160.0,162.0,164.0,166.0,168.0,170.0,172.0,174.0,
        176.0,178.0,180.0,182.0,184.0,186.0,188.0,190.0,192.0,194.0,
        196.0,198.0,200.0,202.0,204.0,206.0,208.0,210.0,212.0,214.0,
        216.0,218.0,220.0,222.0,224.0,226.0,228.0,230.0,232.0,234.0,
        236.0,238.0,240.0,242.0,244.0,246.0,248.0,250.0,252.0,254.0,
        256.0,258.0,260.0,262.0,264.0,266.0,268.0,270.0,272.0,274.0,
        276.0,278.0,280.0,282.0,284.0,286.0,288.0,290.0,292.0,294.0,
        296.0,298.0,300.0,302.0,304.0,306.0,308.0,310.0,312.0,314.0,
        316.0,318.0,320.0,322.0,324.0,326.0,328.0,330.0,332.0,334.0,
        336.0,338.0,340.0,342.0,344.0,346.0,348.0,350.0,352.0,354.0,
        356.0,358.0,360.0,362.0,364.0,366.0,368.0,370.0,372.0,374.0,
        376.0,378.0,380.0,382.0,384.0,386.0,388.0,390.0,392.0,394.0,
        396.0,398.0,400.0,402.0,404.0,406.0,408.0,410.0,412.0,414.0,
        416.0,418.0,420.0,422.0,424.0,426.0,428.0,430.0,432.0,434.0,
        436.0,438.0,440.0,442.0,444.0,446.0,448.0,450.0,452.0,454.0,
        456.0,458.0,460.0,462.0,464.0,466.0,468.0,470.0,472.0,474.0,
        476.0,478.0,480.0,482.0,484.0,486.0,488.0,490.0,492.0,494.0,
        496.0,498.0,500.0,505.0,515.0,-200.0,-1000.0};

static int ncells[maxlat]=
{    0,   3,  12,  28,  50,  78, 112, 152, 198, 250,
   308, 372, 441, 516, 596, 681, 771, 866, 966,1070,
  1178,1290,1406,1526,1649,1775,1904,2036,2170,2306,
  2444,2584,2725,2867,3010,3154,3298,3442,3586,3729,
  3871,4012,4152,4290,4426,4560,4692,4821,4947,5070,
  5190,5306,5418,5526,5630,5730,5825,5915,6000,6080,
  6155,6224,6288,6346,6398,6444,6484,6518,6546,6568,
  6584,6593 };

static int icells[maxlat]=
{    3,   9,  16,  22,  28,  34,  40,  46,  52,  58,
    64,  69,  75,  80,  85,  90,  95, 100, 104, 108,
   112, 116, 120, 123, 126, 129, 132, 134, 136, 138,
   140, 141, 142, 143, 144, 144, 144, 144, 143, 142,
   141, 140, 138, 136, 134, 132, 129, 126, 123, 120,
   116, 112, 108, 104, 100,  95,  90,  85,  80,  75,
    69,  64,  58,  52,  46,  40,  34,  28,  22,  16,
     9,   3 };

   /* input and output files */
  
FILE *anfp;
char anfilename[256];

FILE *dfp;
char filename[256];

FILE *ofp;
char outputfile[256];


   /* variables */
int lonlim[maxbox][2];
static int irec,ans;
int luntv,ifile,itype,iyear,month,iday,
   latbeg,latend,lonbeg,lonend,ibxbeg,ibxend,indx,
   ibox,lon,jrec,nprev;
static int lat,nlon;
int  ivar[maxlon+1][maxvar+1];
float rvar[maxlon+1][maxvar+1];
int eqmap[maxbox];
int sqmap[maxlat][maxlon];
unsigned char chrbuf[numbox][maxvar]; /* for reading data blck from the data files*/
char buf[SIZE];  /*for reading header from the ancillary file*/
char choice;

void main()
{ void display_main();
  void display_menu();
  
  display_main();
  display_menu();
  printf("\n");
  printf("PROGRAM HAS ENDED !\n\n");
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
     printf(" Enter 'c' to view the TOVS values for a specified latitude band.   \n");
     printf(" Enter 's' to view the ancillary data for a specified latitude band.\n"); 
     printf(" Enter 'b' to view both sets of data for a specified latitude band. \n");
     printf(" Enter 'h' to receive help with commands. \n");
     printf(" Enter 'x' to exit or terminate processing. \n");
    
     scanf("%s",&ans);
     choice = ans;
     printf("\n\n");
    switch (choice)
      {
      case 'C':
      case 'c':
      case 'b':
      case 'B':
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
     printf("Option selection 'c' allows you to print out the TOVS data for a     \n");
     printf("user specified latitude band.                                        \n");
     printf("                                                                     \n");
     printf("Option selection 's' allows you to view the ancillary data (such     \n");
     printf("as percentage of land, topographic height and vegetation type) for   \n");
     printf("a user specified latitude band.                                      \n");  
     printf("                                                                     \n");
     printf("Option selection 'b' will provide both of the above selections for   \n");
     printf("a user specified latitude band.                                      \n");   
     
     display_menu();
  }


void process_files()
{ /* Purpose: process the data files for user determined output */
  void rdanc();
  void tvopen();
  void openout();
  int tvread();
  void tvphys();
  void printi();
  void printr();
  void center();
 
  
 
  int ifull,i,j,k,code,res;
  
  char userlat[4];
  char cont;
  int done=0;
  int g,h;
  int try = 0;
 
  printf("\n");
  printf(" This read software will process data files until you wish to stop.\n");
  printf(" Press 'Y' or 'y' to continue or 'x' to stop.                      \n");
  scanf("%s",&cont); 
  while ((cont== 'Y') || (cont == 'y'))
    {
     
  
     if ((choice=='c')||(choice=='C')||(choice=='b')||(choice=='B')||(choice=='S')||(choice=='s'))
      {  /* prompt user for latitude index and verify input is in correct range. */
       while (!done)
        {
         printf("\n");
         printf(" Enter desired latitude band:                 \n");
         printf(" (Value should be between 1 and 72.           \n");
         printf("  Latitude bands are in 2.5 degree increments \n");
         printf("  beginning at the South Pole.)               \n");
         scanf("%s",userlat);
         res = isalpha(userlat[0]); 
         if (res !=0)
           { printf("Your selection of %s is invalid. \n",userlat);
             printf("Please try again.\n");}
         else{
            ans = atoi(userlat);   
            if ((ans < 1) || (ans > 72))
             { printf("\n");
               printf("Latitude entered is not within the proper range \n");
               printf("Please try again.\n");
             }
             else
              done = 1;
	  }
         
          indx = ans-1;
          
                     
       }/* end while */
      } /* end if c */  
     openout();
     try++;
     if (try ==1) rdanc();
     tvopen();
    
     
     fprintf(ofp,"\n     The data file used is %s\n\n",filename);
     ibxbeg = 0;
     ibxend = -1;
     ibox = 0;
     irec = 0;
     ifull = 0;
     for (lat=0; lat<maxlat; lat++)
      { 
       code=tvread();
      
       tvphys();
       for (lon=1;lon<=nlon; lon++)
	 { ibox++;
            
           if (ivar[lon][2] == 255)
            {
             if (lat==indx)
               {fprintf(ofp,"\n A value of 255 was encountered at longitude band %d in variable 3\n",lon);
                fprintf(ofp,"No data was recorded!\n");}
             continue;}
           ifull++;

           if ((choice=='c')||(choice=='C')||(choice=='b')||(choice=='B'))
            {  
             /*output center value for latitude requested by user */
             
             if ((lat == indx)&&(lon <= icells[indx])) /* make changeable when programs works*/
              {
               fprintf(ofp,"**************************************************************************************************************************************\n");
               fprintf(ofp,"    Processing equal-area lon/lat %d %d\n",lon,indx+1);
               center(lon);
               
               printi(lon);
               printr(lon);
              
              } /* end if */
          } /* end if choice */
       } /* end for lon */
     
     }/* end for lat*/
    printf(" The number of full (not=255) boxes in data file  %s: %6d\n",filename,ifull);
    

 /* user can select a latitude and longitude range to view the ice/snow codes */
   
    printf("\n");
    printf(" Do you wish to process another data file? ");
    scanf("%s",&cont);  
    if ((cont == 'Y')||(cont == 'y'))done=0;
   }/* end while */
  }/* end process_files */
 

  /*******************************************************************************/
  /* THIS SECTION CONTAINS THE FUNCTIONS USED TO INPUT THE DATA FILES            */
  /*******************************************************************************/

void rdanc()
   /* read tovs ancillary data file; */
   /* begin and end longitude values are taken from this file */

  { int p,j,i,iarea,landfr,l,rec,itopog,iveg;
    float cenlat,cenlon;

 

         /* These used to read variables from the buffer buf[]  */
   char sibox[4];
   char sj[4];
   char si[4];
   char slonbeg[4];
   char slonend[4]; 
   char scenlat[9];
   char scenlon[9];
   char siarea[8];
   char sland[6];
   char sitopog[7];
   char siveg[4];

  /* initialize lonlim array */
    for (l =0; l <=maxbox;l++)
      { lonlim[l][0]=iundef;
        lonlim[l][1]=iundef;
      }
                                             
   printf(" Enter filename for ancillary file:\n");
   scanf ("%s",anfilename);
    
    anfp = fopen(anfilename,"r");
    fprintf(ofp,"     Ancillary data file is %s\n", anfilename);
    if (anfp == NULL)
     { 
      printf("************************************************************\n");
      printf("*  ERROR - Unable to open the data file  %s *\n",anfilename);
      printf("*           Program has terminated                         *\n");
      printf("************************************************************\n");
      exit(-1);
     }
    fread ((char *) &buf[0],sizeof(unsigned char), SIZE, anfp);
    if ((choice=='s')||(choice=='S')||(choice=='b')||(choice=='B'))
         fprintf(ofp, "%s\n",buf);
    fread ((char *) &buf[0],sizeof(unsigned char), SIZE, anfp);
    if ((choice=='s')||(choice=='S')||(choice=='b')||(choice=='B'))
         fprintf(ofp, "%s\n",buf);

    for (rec =1; rec <= maxbox; rec++)
      {
       fread((char *) &buf[0],sizeof (unsigned char), SIZE, anfp);
       strncpy(sibox,buf,4); 
       ibox = atoi(sibox);
      
       strncpy(sj,buf+5,3);
        j=atoi(sj);
  
       strncpy(si,buf+8,4);
      
       strncpy(slonbeg,buf+12,4);
       lonbeg = atoi(slonbeg);
      
       strncpy(slonend,buf+16,4);
       lonend = atoi(slonend);
       
       strncpy(scenlat,buf+20,9);
       cenlat = atof(scenlat);
       
       strncpy(scenlon,buf+29,9);
       cenlon = atof(scenlon);
       
       strncpy(siarea,buf+38,8);
       iarea = atoi(siarea);
       
       strncpy(sland,buf+46,6);
       landfr = atoi(sland);
       
       strncpy(sitopog,buf+52,7);
       itopog = atoi(sitopog);
       
       strncpy(siveg,buf+59,4);
       iveg = atoi(siveg);
       
       if ((choice=='s')||(choice=='S')||(choice=='b')||(choice=='B'))
          if (atoi(sj) == indx+1) fprintf(ofp, "%4d%4d%4d%4d%4d%9.2f%9.2f%8d%6d%7d%4d\n",ibox,atoi(sj),atoi(si),lonbeg,lonend,cenlat,cenlon,iarea,landfr,itopog,iveg);     
      
       lonlim[ibox-1][0] = lonbeg;
       lonlim[ibox-1][1] = lonend;
       
      }
 
    close(anfilename);
  }

void tvopen()
  {
    /*open tovs data file and output file*/
   
  printf(" Enter data file to be used:        \n");
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
   char outans;
  printf("\n\n");
  printf(" Enter a name for the output file:   \n");
  scanf("%s",outputfile);
  ofp=fopen(outputfile,"r");
  if (ofp != NULL)
    {
     printf("***************************************************************\n");   
     printf("     WARNING: This filename exist -- %s\n",outputfile);
     printf("      Do you wish to overwrite it? 'y' yes 'n' no             \n");
     printf("***************************************************************\n");
     scanf("%s",&outans);
     if ((outans == 'n') || ( outans == 'N')) openout();
    }
  ofp = fopen(outputfile,"w");

 /* ofp=fopen("tovsout", "w");*/
 /* printf(" Output file is tovsout.\n");*/
   
 }


int tvread()
  { 
    /* Purpose: read an ice/snow data file and store values in ivar array*/
    
    void tvrec();

    int  nbox,i,ilon;
    static int idecod;

  /* initialize the output arrays ivar */
    for (lon=0; lon <maxlon;lon++)
        for (i=0;i<maxvar; i++)
           ivar[lon][i] = iundef;
        
  
    /* loop over all boxes for this lat */
   
    nlon = icells[lat];   
    nprev = ncells[lat];
    
    for (lon=0;lon<=nlon;lon++)
       {
         nbox = nprev + lon;
         
            if (nbox >= ibxbeg)
             {
              if (nbox <= ibxend)
               { 
                 if ((int)chrbuf[idecod+1][0] > lat+1 )  return 0;             
              
                 idecod++;
                 
                 ilon = chrbuf[idecod][1]; 
                  
                 for (i=0;i<maxvar;i++)
                   { ivar[ilon][i]=chrbuf[idecod][i]; }
               
               }
             else
	      {
               tvrec();
               idecod = 0;
	      };
             };/*end if nbox*/   
         
	};/* end for */
      
       return 0;
    }     
       

void tvrec()
  { /* read an ice/snow data record and unpack record prefix */
    int ilat,ilong,next,i;
     irec++;
      
     fread((char *) chrbuf, sizeof(unsigned char), numbox,dfp);
     
     if (feof(dfp) == 0)
       {  ifile  = (chrbuf[0][0]);
          jrec   = (chrbuf[0][1]);
          itype  = (chrbuf[0][2]);      
          iyear  = (chrbuf[0][3]);
          month  = (chrbuf[0][4]);
          iday   = (chrbuf[0][5]);
          latbeg = (chrbuf[0][6]);
          latend = (chrbuf[0][7]);
          lonbeg = (chrbuf[0][8]);
          lonend = (chrbuf[0][9]);
      /*    next   = (chrbuf[0][10]);*/
          ibxbeg = (irec-1)*550+1;
          ibxend = (irec*550);
  
       /*   printf ("%d\n",irec);*/
      /*    printf ("chrbuf= %d %d %d %d %d %d %d %d %d %d %d\n",ifile,jrec,itype,iyear,month,iday,latbeg,latend,lonbeg,lonend,next);*/
         
        /*  printf ("ibxbeg = %d\n",ibxbeg);*/
        /*  printf ("ibxend = %d\n",ibxend);*/
          
        
          if (irec ==1)
            {if (itype !=  1)
	       {fprintf(ofp,"TV PREFIX INFORMATION: \n");
                fprintf(ofp,"data type : %d month: %d \n",itype,month);}
              else
		{
                 fprintf(ofp,"TV PREFIX INFORMATION: \n");
                 fprintf(ofp,"data type: %d year: %d month: %d day: %d\n",itype,iyear,month,iday);}
            }/* end if irec */
       } 
  } 

   

	      
void tvphys()
  { /*  DESCRIPTION:  CONVERT DATA FOR ALL GRID BOXES WITHIN A SINGLE LAT  
                      BAND FROM INTEGER COUNTS TO FLOATING POINT PHYSICAL VALUES */
    
    int ibyte,g,h,i;
 
  
    for (lon=1;lon <=nlon;lon++)
       { for (ibyte =0;ibyte <=7;ibyte++){
             rvar[lon][ibyte+1]=(float)(ivar[lon][ibyte]);
       }
        
        rvar[lon][9]=pretab[ivar[lon][8]];
        if (ivar[lon][9]!=iundef)
           rvar[lon][10]=(float)((ivar[lon][9])/100.0);
         else
           rvar[lon][10] = rundef;  
           
   /*--------TOPOGRAPHICAL HEIGHT------------------------------------*/
  
      rvar[lon][11] = (float)(ivar[lon][10]);
     
  /*----------- SURFACE TEMPERATURE  & PRESSURE--------------------- */
 
       rvar[lon][12] = tmptab[ivar[lon][11]];
       rvar[lon][13] = pretab[ivar[lon][12]];
      
/*--------------------------------------------------------------------*/
/* TROPOPAUSE TEMPERATURE & PRESSURE                                  */
/*--------------------------------------------------------------------*/
        rvar[lon][14] = tmptab[ivar[lon][13]];
        rvar[lon][15] = pretab[ivar[lon][14]];
/*--------------------------------------------------------------------*/
/* PRECIPITABLE WATER                                                 */
/*--------------------------------------------------------------------*/
         for (ibyte=15;ibyte<=19;ibyte++)
             rvar[lon][ibyte+1] = prwtab[ivar[lon][ibyte]];
 
/*--------------------------------------------------------------------*/
/* ATMOSPHERIC TEMPERATURES                                           */
/*--------------------------------------------------------------------*/
         for (ibyte=20;ibyte <=28;ibyte++)
            rvar[lon][ibyte+1] = tmptab[ivar[lon][ibyte]];
  
/*--------------------------------------------------------------------*/
/* OZONE COLUMN ABUNDANCE                                             */
/*--------------------------------------------------------------------*/
        rvar[lon][30] = ozntab[ivar[lon][29]];
/*--------------------------------------------------------------------*/
/* END OF LON LOOP                                                    */
  }
/*  printf("TVPHYS: LAT BAND %d CONVERTED TO PHYSICAL VALUES\n",lat);*/
  }
  
void center(lon)
   /* calculate center lon/lat of box (equal-area grid) */

  {float dlon;
   float cenlat,cenlon;
   float dlat=2.5;

   dlon = 360.0/nlon;
  
   cenlat = lat * dlat + dlat/2.0 - 90.0;
   cenlon = (lon-1 ) * dlon + dlon/2.0;
   fprintf(ofp,"    CENTER: CENTER LON/LAT %8.2f%8.2f\n",cenlon,cenlat);
 
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

void printi()
  {
    /* print count values for the box */
   int i,k,iend;

   fprintf(ofp,"PRINTI: COUNT VALUES FOR ALL VARIABLES\n");
   fprintf(ofp,"                  ");
   for (k=1;k<=10;k++)
     fprintf(ofp,"  %d  ",k);
   fprintf (ofp,"\n");
   for (i=0;i<maxvar;i=i+10)
    {iend = i+9;
     if (iend > maxvar-1) iend = maxvar-1;
     fprintf (ofp,"variable %3d-%3d |",i+1,iend+1);
     for (k=i;k<=iend;k++)
       {
        fprintf(ofp," %3d ",ivar[lon][k]);
       }fprintf (ofp,"\n");
    }/* end for i */
  }/* end printi */
 
void printr()
  {
    /* print physical values for the box */
   int i,k,iend;
   fprintf(ofp,"\n");
   fprintf(ofp,"PRINTR: PHYSICAL VALUES FOR ALL VARIABLES\n");
   fprintf(ofp,"                 ");
   for (k=1;k<=10;k++)
     fprintf(ofp,"     %d     ",k);
   fprintf (ofp,"\n");
   for (i=1;i<=maxvar;i=i+10)
    {iend = i+9;
     if (iend > maxvar) iend = maxvar;
     fprintf (ofp,"variable %3d-%3d |",i,iend);
     for (k=i;k<=iend;k++)
       {
        fprintf(ofp," %-9.3f ",rvar[lon][k]);
       }fprintf (ofp,"\n");
   }
}
  
  
