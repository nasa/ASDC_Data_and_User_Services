/************************************************************************************************/
/*                                                                                              */
/* This is the read program for reading the FIRE Sabreliner and Kingair Microphysical data sets.*/
/* This program was written and modified by Syamala Sistla on MAY 26, 1997.                     */
/*                                                                                              */
/************************************************************************************************/

#include <stdio.h>

FILE *fp;
FILE *tp;
#define MAXLEN 100
char buf1[MAXLEN];
char buf2[MAXLEN];
char buf3[MAXLEN];
char buf4[MAXLEN];
char buf5[MAXLEN];
char buf6[MAXLEN];
char buf7[MAXLEN];
char buf8[MAXLEN];
char buf9[MAXLEN];
char buf9[MAXLEN];
char buf10[MAXLEN];
char buf11[MAXLEN];
char buf12[MAXLEN];
char buf13[MAXLEN];
char buf14[MAXLEN];
char buf15[MAXLEN];
char buf16[MAXLEN];
char buf17[MAXLEN];
char buf18[MAXLEN];
char buf19[MAXLEN];
char buf20[MAXLEN];
char buf21[MAXLEN];
char buf22[MAXLEN];
char buf23[MAXLEN];/*36*/


/* GLOBAL PARAMETERS*/
char TIMES[10];
char PRES[10];
char TEMP[10];
char ALT[10];
char LAT[10];
char LON[10];
char USTAR[10];
char DBARM[10];
char DMAX[10];
char W1[10];
char W2[10];
char W3[10];
char W4[10];/*54*/

char WTOTAL[8];
char DT8[5];
char DT12[8];
char M1[5];
char M2[5];
char M3[5];
char M4[5];
char DPCRC[8];
char RH[5];
char IWC[5];
char XM1[5];
char XM2[5];
char XM3[5];/*68*/

char PRECIP[8];
char DBZ[5];
char VBAR[8];
char C_TOTAL[10];
char LE[5];
int i1;
char pcon1[8];
char pcon2[8];
char GT[5];
int i2;
char TIMP[8];/*80*/

char RHORH[8];
char SI[5];
char SW[5];
char LAMBDA[8];
char NZER[8];
char RSQ[6];
char XSAREA[8];

int times1;
int times2;
char pres[5];
float temp;
char alt[8];
float lat;
float lon;
float ustar;
float dbarm;
float dmax;
float w1;
float w2;
float w3;
float w4;/*105*/

float wtotal;
float dt8;
float dt12;
float m1;
float m2;
float m3;
float m4;
float dpcrc;
char rh[5];
float iwc;
float xm1;
float xm2;
float xm3;/*119*/

float precip;
float dbz;
float vbar;
float ctotal;/*124*/
float le;
float X;
float Y;
float Z;
float timp;

float rhorh;
float si;
float sw;
float lambda;
float nzer;
float rsq;
float xsarea;
char probe[5];

void display_main();
void get_file();
void Read_file_header();
void Print_file_header();
void Read_parms();
void Print_parms(); 
void Parameter_description();
void Print_original_format();
void Print_custom_format();
void Print_all_data();
int n_blocks;


/************************************************************************************/
/*                                                                                  */
/*  Purpose: This section displays a menu which contains general information about  */
/*           the providers of the program.                                          */
/*                                                                                  */
/************************************************************************************/

void display_main()
{ 
                                           
   printf("\n\n\n\n");

   printf(" ******************************************************************\n");
   printf(" *                                                                *\n");
   printf(" *                     Langley DAAC                               *\n");
   printf(" *       PROJECT_LEVEL/DATA_SET_LEVEL READ SOFTWARE               *\n");
   printf(" *   VERSION NUMBER:                                              *\n");
   printf(" *   Language: C                                                  *\n");
   printf(" *   Platforms supported: SUN                                     *\n");
   printf(" *                        HP                                      *\n");
   printf(" *                        SGI                                     *\n");
   printf(" *                        Dec Alpha                               *\n");
   printf(" *   Contact: Langley DAAC User and Data Services Office          *\n");
   printf(" *            NASA Langley Research Center                        *\n");   
   printf(" *            Mail Stop 157D                                      *\n");
   printf(" *            Hampton, VA 23681-0001                              *\n");
   printf(" *            PHONE:  (757)864-8656        FAX: (757)864-8807     *\n");
   printf(" *                                                                *\n");
   printf(" ******************************************************************\n");

   printf("\n\n\n\n");

}



/*******************************************************************/
/*                                                                 */
/*                     Custom_Output_Menu                          */
/*                                                                 */
/*******************************************************************/

void Custom_Output_Menu ()
{
int choice;

printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
printf("                Customized Output Menu                           \n");
printf("                ======================                           \n");
printf("  1...File Header                                                \n");
printf("  2...Listing of Parameters                                      \n");
printf("  3...Parameter Description                                      \n");
printf("  4...Display all data and number of blocks.                     \n");
printf("  5...Raw data - selected no. of blocks, in original format      \n");
printf("  6...Raw data - selected no. of blocks, with parameter name     \n");
printf("  0...To Exit                                                    \n");

printf("Enter your choice: \n");
scanf("%d",&choice);
  switch(choice)
  {
     case 1:
          Print_file_header();
          break;
     case 2:
          Print_parms();
          break;
     case 3:
          Parameter_description();
          break;
     case 4:
          Print_all_data();
          break;
     case 5:
          Print_original_format();
          break;
     case 6:
          Print_custom_format();
          break;
     case 0:
          break;
     default: break;
  } 


}
 

/************************************************************************************/
/*                                                                                  */
/* Purpose: To open a user specified file for data input.                           */
/* Prints the output to an output file.                                             */
/*                                                                                  */
/************************************************************************************/ 


void get_file()
{
  char filename[50];
  char outfile[50];
  
  printf("Enter name of input file: \n");
  printf("   NOTE: Data file may be in the current working directory.\n");
  printf("   NOTE: Or enter the complete pathname for the data filename.\n");
  printf("\n \n");
  scanf("%s", filename);
  
  fp = fopen(filename, "r");
  if (fp == NULL)
  { 
    printf("*********************************************************\n");
    printf("ERROR-UNABLE TO OPEN DATA FILE \"%s\". PROGRAM TERMINATED\n",
            filename);
    printf("*********************************************************\n");
    exit(-1);
  }

/*    **************************************************
 *  --> Open output report file....get name of file 
 *    **************************************************/

  printf("Enter name of output file: ");
  scanf("%s", outfile);
  tp = fopen(outfile, "w");

  if (tp == NULL) 
       {
         printf("EXITING - FATAL ERROR opening file %s\n",outfile);
         exit(1);
       }
  
 }

/***********************************************************************/
/*                                                                     */
/*                        Read and print the file header.              */
/*                                                                     */
/***********************************************************************/

void Read_file_header()
{ 
  fgets(buf1,MAXLEN,fp);
  fgets(buf2,MAXLEN,fp);
  fgets(buf3,MAXLEN,fp);
  fgets(buf4,MAXLEN,fp);
  fgets(buf5,MAXLEN,fp);
  fgets(buf6,MAXLEN,fp);
  fgets(buf7,MAXLEN,fp);
  fgets(buf8,MAXLEN,fp);
  fgets(buf9,MAXLEN,fp);
  fgets(buf10,MAXLEN,fp);
  fgets(buf11,MAXLEN,fp);
  fgets(buf12,MAXLEN,fp);
  fgets(buf13,MAXLEN,fp);
  fgets(buf14,MAXLEN,fp);
  fgets(buf15,MAXLEN,fp);
  fgets(buf16,MAXLEN,fp);
  fgets(buf17,MAXLEN,fp);
  fgets(buf18,MAXLEN,fp);
  fgets(buf19,MAXLEN,fp);
  fgets(buf20,MAXLEN,fp);
  fgets(buf21,MAXLEN,fp);
  fgets(buf22,MAXLEN,fp);
  fgets(buf23,MAXLEN,fp);
}

void Print_file_header()
{
  Read_file_header();
  fputs(buf1,tp);
  fputs(buf2,tp);
  fputs(buf3,tp);
  fputs(buf4,tp);
  fputs(buf5,tp);
  fputs(buf6,tp);
  fputs(buf7,tp);
  fputs(buf8,tp);
  fputs(buf9,tp);
  fputs(buf10,tp);
  fputs(buf11,tp);
  fputs(buf12,tp);
  fputs(buf13,tp);
  fputs(buf14,tp);
  fputs(buf15,tp);
  fputs(buf16,tp);
  fputs(buf17,tp);
  fputs(buf18,tp);
  fputs(buf19,tp);
  fputs(buf20,tp);
  fputs(buf21,tp);
  fputs(buf22,tp);
  fputs(buf23,tp);

}
   
/***********************************************************************/
/*                                                                     */
/*      Read the names of the parameters and print them out.           */
/*                                                                     */
/***********************************************************************/                                               

void Read_parms()
{  
  Read_file_header();
  fscanf(fp,"%s  %s  %s  %s  %s  %s  %s  %s  %s  %s  %s  %s  %s\n",TIMES,PRES,TEMP,ALT,LAT,LON,USTAR,DBARM,DMAX,W1,W2,W3,W4);
  fscanf(fp,"%s  %s  %s  %s  %s  %s  %s  %s  %s  %s  %s  %s  %s\n",WTOTAL,DT8,DT12,M1,M2,M3,M4,DPCRC,RH,IWC,XM1,XM2,XM3);
  fscanf(fp,"%s %s %s %s %s %d %s %s %s %d %s\n",PRECIP,DBZ,VBAR,C_TOTAL,LE,&i1,pcon1,pcon2,GT,&i2,TIMP);
  fscanf(fp,"%s  %s  %s  %s  %s  %s  %s\n",RHORH,SI,SW,LAMBDA,NZER,RSQ,XSAREA);
}

/***********************************************************************/
/*                                                                     */
/*    Display parameters and their descriptions.                       */
/*                                                                     */
/***********************************************************************/
  

void Parameter_description()
{

  fprintf(tp,"Parameter Description                            Parameter\n");
  fprintf(tp,"\n");
  fprintf(tp,"TIME INTERVAL.......................................TIMES\n");
  fprintf(tp,"STATIC PRESSURE.....................................PRES\n");
  fprintf(tp,"AMBIENT TEMPERATURE.................................TEMP\n");
  fprintf(tp,"PRESSURE ALTITUDE...................................ALT\n");
  fprintf(tp,"VERTICAL VELOCITY REQUIRED TO KEEP                       \n");
  fprintf(tp,"THE RELATIVE HUMIDITY CONSTANT......................USTAR\n");
  fprintf(tp,"MEDIAN PARTICLE MASS WEIGHTED DIAMETER..............DBARM\n");
  fprintf(tp,"MAXIMUM PARTICLE DIAMETER...........................DMAX\n");
  fprintf(tp,"DIFFERENTIAL GROWTH RATE IN CHANNEL 1...............W1\n");
  fprintf(tp,"DIFFERENTIAL GROWTH RATE IN CHANNEL 2...............W2\n");
  fprintf(tp,"DIFFERENTIAL GROWTH RATE IN CHANNEL 3...............W3\n");
  fprintf(tp,"DIFFERENTIAL GROWTH RATE IN CHANNEL 4...............W4\n");
  fprintf(tp,"TOTAL DIFFERENTIAL GROWTH RATE......................W-TOTAL\n");
  fprintf(tp,"DEPLETION TIME (8 MICRON DROPLETS)..................DT8\n");
  fprintf(tp,"DEPLETION TIME (12 MICRON DROPLETS).................DT12\n");
  fprintf(tp,"IWC IN CHANNEL 1....................................M1\n");
  fprintf(tp,"IWC IN CHANNEL 2....................................M2\n");
  fprintf(tp,"CRYOGENIC DEW POINT TEMPERATURE.....................DPCRC\n");
  fprintf(tp,"CRYOGENIC RELATIVE HUMIDITY.........................RH\n");
  fprintf(tp,"ICE WATER CONTENT...................................IWC\n");
  fprintf(tp,"ICE WATER CONTENT BASED ON SNOW HABIT...............XM1\n");
  fprintf(tp,"ICE WATER CONTENT BASED ON SMALL SNOW HABIT.........XM2\n");
  fprintf(tp,"ICE WATER CONTENT BASED ON LARGE SNOW HABIT.........XM3\n");
  fprintf(tp,"PRECIPITATION RATE..................................PRECIP\n");
  fprintf(tp,"RADAR REFECTIVITY FACTOR............................DBZ\n");
  fprintf(tp,"MEAN REFECTIVITY WEIGHTED WITH THE                       \n");
  fprintf(tp,"TERMINAL VELOCITY...................................VBAR\n");
  fprintf(tp,"TOTAL PARTICLE CONCENTRATION........................C-TOTAL\n");
  fprintf(tp,"PARTICLE CONCENTRATION LE 200.......................LE 200\n");
  fprintf(tp,"PARTICLE CONCENTRATION LE 200.......................200-500\n");
  fprintf(tp,"PARTICLE CONCENTRATION LE 200.......................500-800\n");
  fprintf(tp,"PARTICLE CONCENTRATION LE 200.......................GT 800\n");
  fprintf(tp,"IWC IN CHANNEL 3....................................M3\n");
  fprintf(tp,"IWC IN CHANNEL 4....................................M4\n");
  fprintf(tp,"NUMBER OF CRYSTAL-CRYSTAL COLLISIONS................TIMP\n");
  fprintf(tp,"WATER VAPOR DENSITY.................................RHORH\n");
  fprintf(tp,"SUPERSATURATION WITH RESPECT TO ICE.................SI\n");
  fprintf(tp,"SUPERSATURATION WITH RESPECT TO WATER...............SW\n");
  fprintf(tp,"COEFFICIENTS USED TO FIT THE EQUATION                    \n");
  fprintf(tp,"  LAMBDA............................................LAMBDA\n");
  fprintf(tp,"  NZERO.............................................NZER\n");
  fprintf(tp,"SOLE COEFFICIENT OF THE FIT.........................RSQ\n");
  fprintf(tp,"CROSS-SECTIONAL AREA UNITS..........................XSAREA\n");

}


void Print_parms()
{ 
  Read_parms();
  fprintf(tp,"%10s %10s %5s %5s %9s %8s %9s %6s %5s %7s %7s %7s %7s\n",TIMES,PRES,TEMP,ALT,LAT,LON,USTAR,DBARM,DMAX,W1,W2,W3,W4);
  fprintf(tp,"%14s %5s %7s %7s %7s %7s %7s %6s %3s %7s %6s %6s %6s\n",WTOTAL,DT8,DT12,M1,M2,M3,M4,DPCRC,RH,IWC,XM1,XM2,XM3);
  fprintf(tp,"%16s %5s %5s %8s %3s %3d %7s %7s %3s %3d %7s\n",PRECIP,DBZ,VBAR,C_TOTAL,LE,i1,pcon1,pcon2,GT,i2,TIMP);
  fprintf(tp,"%24s  %7s  %7s  %9s  %9s  %6s  %9s\n",RHORH,SI,SW,LAMBDA,NZER,RSQ,XSAREA); 
}
   

void Print_all_data()
{ 
  int n;
  n=0;
  Read_parms();
  while(!feof(fp))
  {
       fscanf(fp,"\n");
       fscanf(fp,"%d %d %s %f %s %f %f %f %f %f %f %f %f %f\n",&times1,&times2,pres,&temp,alt,&lat,&lon,
              &ustar,&dbarm,&dmax,&w1,&w2,&w3,&w4); 

       fscanf(fp,"%f %f %f %f %f %f %f %f %s %f %f %f %f\n",&wtotal,&dt8,&dt12,&m1,&m2,&m3,&m4,&dpcrc,rh,&iwc,&xm1,&xm2,&xm3);
       fscanf(fp,"%f %f %f %f %f %f %f %f %f\n",&precip,&dbz,&vbar,&ctotal,&le,&X,&Y,&Z,&timp);
       fscanf(fp,"%f %f %f %f %f %f %f %s\n",&rhorh,&si,&sw,&lambda,&nzer,&rsq,&xsarea,probe);
    
    
       fprintf(tp,"\n");
       fprintf(tp,"     ");
       fprintf(tp,"%6d %6d %5s %5.1f %6s %8.3f %8.3f %8.1E %6.3f %6.3f %7.1E %7.1E %7.1E %7.1E\n",times1,times2,pres,temp,alt,lat,lon,
              ustar,dbarm,dmax,w1,w2,w3,w4); 
       fprintf(tp,"        ");

       fprintf(tp,"%7.1E %7.1E %7.1E %7.1E %7.1E %7.1E %7.1E %5.1f %3s %6.4f %6.4f %6.4f %6.4f\n",wtotal,dt8,dt12,m1,m2,m3,m4,dpcrc,rh,                   iwc,xm1,xm2,xm3);
       fprintf(tp,"          ");
       fprintf(tp,"%7.1E %5.1f %4.1f %7.1E %7.1E %7.1E %7.1E %7.1E %7.1E\n",precip,dbz,vbar,ctotal,le,X,Y,Z,timp);
       fprintf(tp,"               " );
       fprintf(tp,"%7.1E %7.1E %7.1E %9.3E %9.3E %6.4f %9.3E %3s\n",rhorh,si,sw,lambda,nzer,rsq,xsarea,probe);
       n++;
  }
    printf("Number of blocks of data in file = %d\n",n);
}


/**************************************************************************/
/*                                                                        */
/*  Print selected blocks of data in the same format as seen in data file.*/
/*                                                                        */
/**************************************************************************/

void Print_original_format()
{ 
  int num_blocks;
  int k;
  Read_parms();
  printf("Please enter the number of blocks of data you would like to view: ");
  scanf("%d",&num_blocks);
    for(k=0;k<num_blocks;k++)
    { 
       fscanf(fp,"\n");
       fscanf(fp,"%d %d %s %f %s %f %f %f %f %f %f %f %f %f\n",&times1,&times2,pres,&temp,alt,&lat,&lon,
              &ustar,&dbarm,&dmax,&w1,&w2,&w3,&w4); 

       fscanf(fp,"%f %f %f %f %f %f %f %f %s %f %f %f %f\n",&wtotal,&dt8,&dt12,&m1,&m2,&m3,&m4,&dpcrc,rh,&iwc,&xm1,&xm2,&xm3);
       fscanf(fp,"%f %f %f %f %f %f %f %f %f\n",&precip,&dbz,&vbar,&ctotal,&le,&X,&Y,&Z,&timp);
       fscanf(fp,"%f %f %f %f %f %f %f %s\n",&rhorh,&si,&sw,&lambda,&nzer,&rsq,&xsarea,probe);
    
    
       fprintf(tp,"\n");
       fprintf(tp,"     ");
       fprintf(tp,"%6d %6d %5s %5.1f %6s %8.3f %8.3f %8.1E %6.3f %6.3f %7.1E %7.1E %7.1E %7.1E\n",times1,times2,pres,temp,alt,lat,lon,
              ustar,dbarm,dmax,w1,w2,w3,w4); 
       fprintf(tp,"        ");

       fprintf(tp,"%7.1E %7.1E %7.1E %7.1E %7.1E %7.1E %7.1E %5.1f %3s %6.4f %6.4f %6.4f %6.4f\n",wtotal,dt8,dt12,m1,m2,m3,m4,dpcrc,rh,                   iwc,xm1,xm2,xm3);
       fprintf(tp,"          ");
       fprintf(tp,"%7.1E %5.1f %4.1f %7.1E %7.1E %7.1E %7.1E %7.1E %7.1E\n",precip,dbz,vbar,ctotal,le,X,Y,Z,timp);
       fprintf(tp,"               " );
       fprintf(tp,"%7.1E %7.1E %7.1E %9.3E %9.3E %6.4f %9.3E %3s\n",rhorh,si,sw,lambda,nzer,rsq,xsarea,probe);
     }
     
}


/****************************************************************************/
/*                                                                          */
/*  Print data in a format that shows the parameter and the value.          */
/*                                                                          */
/****************************************************************************/


void Print_custom_format()
{ 
  int num_blocks;
  int k;
  Read_parms();
  printf("Please enter the number of blocks of data you would like to view: ");
  scanf("%d",&num_blocks);
  for(k=0;k<num_blocks;k++)
  { 
    fscanf(fp,"\n");
    fscanf(fp,"%d %d %s %f %s %f %f %f %f %f %f %f %f %f\n",&times1,&times2,pres,&temp,alt,&lat,&lon,
              &ustar,&dbarm,&dmax,&w1,&w2,&w3,&w4); 

    fscanf(fp,"%f %f %f %f %f %f %f %f %s %f %f %f %f\n",&wtotal,&dt8,&dt12,&m1,&m2,&m3,&m4,&dpcrc,rh,&iwc,&xm1,&xm2,&xm3);
    fscanf(fp,"%f %f %f %f %f %f %f %f %f\n",&precip,&dbz,&vbar,&ctotal,&le,&X,&Y,&Z,&timp);
    fscanf(fp,"%f %f %f %f %f %f %f %s\n",&rhorh,&si,&sw,&lambda,&nzer,&rsq,&xsarea,probe);
   
    
    fprintf(tp,"\n");

    fprintf(tp,"TIMES......................%6d   %6d\n",times1,times2);
    fprintf(tp,"PRES.......................%5s\n",pres );
    fprintf(tp,"TEMP...................... %5.1f\n",temp);
    fprintf(tp,"ALT........................%6s\n",alt);
    fprintf(tp,"LAT........................%8.3f\n",lat);
    fprintf(tp,"LON........................%8.3f\n",lon);
    fprintf(tp,"USTAR......................%8.1E\n",ustar);
    fprintf(tp,"DBARM......................%6.3f\n",dbarm);
    fprintf(tp,"DMAX.......................%6.3f\n",dmax);
    fprintf(tp,"W1.........................%7.1E\n",w1);
    fprintf(tp,"W1.........................%7.1E\n",w2);
    fprintf(tp,"W1.........................%7.1E\n",w3);
    fprintf(tp,"W1.........................%7.1E\n",w4);

    fprintf(tp,"WTOTAL.....................%7.1E\n",wtotal);
    fprintf(tp,"DT8........................%7.1E\n",dt8);
    fprintf(tp,"DT12.......................%7.1E\n",dt12);
    fprintf(tp,"M1.........................%7.1E\n",m1);
    fprintf(tp,"M2.........................%7.1E\n",m2);
    fprintf(tp,"M3.........................%7.1E\n",m3); 
    fprintf(tp,"M4.........................%7.1E\n",m4);
    fprintf(tp,"DPCRC......................%5.1f\n",dpcrc);
    fprintf(tp,"RH.........................%3s\n",rh);
    fprintf(tp,"IWC........................%6.4f\n",iwc);
    fprintf(tp,"XM1........................%6.4f\n",xm1);
    fprintf(tp,"XM2........................%6.4f\n",xm2);
    fprintf(tp,"XM3........................%6.4f\n",xm3);
   
    fprintf(tp,"PRECIP.....................%7.1E\n",precip);
    fprintf(tp,"DBZ........................%5.1f\n",dbz);
    fprintf(tp,"VBAR.......................%4.1f\n",vbar);
    fprintf(tp,"C-TOTAL....................%7.1E\n",ctotal);
    fprintf(tp,"LE 200.....................%7.1E\n",le);
    fprintf(tp,"200-500................... %7.1E\n",X);
    fprintf(tp,"500-800....................%7.1E\n",Y);
    fprintf(tp,"GT 800.................... %7.1E\n",Z);
    fprintf(tp,"TIMP.......................%7.1E\n",timp);

    fprintf(tp,"RHORH......................%7.1E\n",rhorh);
    fprintf(tp,"SI.........................%7.1E\n",si);
    fprintf(tp,"SW.........................%7.1E\n",sw);
    fprintf(tp,"LAMBDA.....................%9.3E\n",lambda);
    fprintf(tp,"NZERO......................%9.3E\n",nzer);
    fprintf(tp,"RSQ........................%6.4f\n",rsq);
    fprintf(tp,"XSAREA.....................%9.3E\n",xsarea);
    fprintf(tp,"PROBE TYPE.................%3s\n",probe);
    
  }
}


main()
{
  display_main();
  get_file();
  Custom_Output_Menu();  
  fclose(fp);
  fclose(tp);
  exit(0);
}



