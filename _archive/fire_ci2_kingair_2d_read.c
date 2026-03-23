/*************************************************************************************/
/*                                                                                   */
/* This is the read software for reading the FIRE_CI2_KINGAIR_2D data sets.          */
/* This program was written and modified by Syamala Sistla on MARCH 10, 1997         */
/*                                                                                   */
/*************************************************************************************/

#include <stdio.h>

FILE *fp;
FILE *tp;
#define MAXLEN 100
float *data;


void display_main();
void Print_file_header();
void Read_file_header();
void Print_block_header();
void Print_data();
void Print_min_max();
  
char probe[20];
char resln[20];
char micron[20];
char typec[20];
char cres[30];
char typep[30];
char pres[30]; 
    
char area[10];
char ratio[10]; 
char bin[10];
char endpt[20];
float pts[11];

char fro[10];
int p1;
int p2;
int numbins;
char bins[10];
char dash[10];
char endpts[10];
char points[15];
char in[10];
char microns[20];
char are[30];

char symbol[20];
int one;
int time1;
int time2;
float sec;
float speed;
float value;
char buf[MAXLEN];
char buf1[MAXLEN];
char buf2[MAXLEN];
char buf3[MAXLEN];
char buf4[MAXLEN];
char buf5[MAXLEN];

int probe_type;
int type_num;

float min;
float max;


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
printf("  2...Block Header  - block headers only                         \n");
printf("      1 = INDICATES 2D-C PROBE CONCENTRATION MEASUREMENT         \n");
printf("      2 = INDICATES 2D-P PROBE CONCENTRATION MEASUREMENT         \n");
printf("  3...Raw data - data only                                       \n");
printf("  4...Minimum and maximum values - for each block                \n");
printf("  0...To Exit                                                    \n");

printf("Enter your choice: \n");
scanf("%d",&choice);
  switch(choice)
  {
     case 1:
          Print_file_header();
          break;
     case 2:
          Print_block_header();
          break;
     case 3:
          Print_data();
          break;
     case 4:
          Print_min_max();
          break;
     case 0:
          break;
     default: break;
  } 


}
 

/***********************************************************************/
/*                                                                     */
/*                        Read the file header.                        */
/*                                                                     */
/***********************************************************************/

void Read_file_header()
{ 
  int i;   

  fgets(buf,MAXLEN,fp);
  fscanf(fp,"%s %s %s %s %s %s %s\n",probe,resln,micron,typec,cres,typep,pres);
  fscanf(fp,"%s %s %s %s\n",area,ratio,bin,endpt);

  for (i=0;i<11;i++)
    fscanf(fp,"%f",&pts[i]);
  fscanf(fp,"\n");

 
  fscanf(fp,"\n");
  fscanf(fp,"%s %s %d %d %s %s %s %s %s %s %s\n",fro,probe,&p1,&numbins,bins,
                                             dash,endpts,points,in,microns,are);
  fgets(buf1,MAXLEN,fp);
  fscanf(fp,"\n");
  fscanf(fp,"%s %s %d %d %s %s %s %s %s %s %s\n",fro,probe,&p2,&numbins,bins,
                                             dash,endpts,points,in,microns,are);
  fgets(buf2,MAXLEN,fp);
  fgets(buf3,MAXLEN,fp);
  fgets(buf4,MAXLEN,fp);
  fgets(buf5,MAXLEN,fp);
}


/********************************************************************/
/*                                                                  */
/*                      Print the file header.                      */
/*                                                                  */
/********************************************************************/

void Print_file_header()
{
  int i;

  Read_file_header(); 
  fputs(buf,tp);
  fprintf(tp,"%6s %s %s %s %5s %11s %5s\n",probe,resln,micron,typec,cres,typep,pres);
  fprintf(tp,"%5s %s %s %s\n",area,ratio,bin,endpt);
  fprintf(tp," ");

  for(i=0;i<11;i++)
    fprintf(tp," %4.2f",pts[i]);
  fprintf(tp,"\n");
   
  fprintf(tp,"%s %s %1d %2d %s %s %s %s %s %s %s\n",fro,probe,p1,numbins,bins,
                                              dash,endpts,points,in,microns,are);
  fprintf(tp,"             ");
  fputs(buf1,tp);
  fprintf(tp,"%s %s %1d %2d %s %s %s %s %s %s %s\n",fro,probe,p2,numbins,bins,
                                              dash,endpts,points,in,microns,are);
  fprintf(tp,"            ");
  fputs(buf2,tp);
  fputs(buf3,tp);
  fputs(buf4,tp);
  fputs(buf5,tp);  
  fprintf(tp,"\n\n");
}


/********************************************************************************/
/*                                                                              */
/*                   Print record headers only.                                 */
/*                                                                              */
/********************************************************************************/

void Print_block_header()
{ 
  int i;
  int k;
  int x;
  int n;   
  n = 0;  
  x = 0;
  printf("Please enter probe type (1 or 2): ");
  scanf("%d",&type_num);
  while(type_num>2)
  {
    printf("Not a valid entry, entry should be 1 or 2\n");
    printf("Enter probe type: ");
    scanf("%d",&type_num);
  }
  
  Read_file_header();
  printf("Please be patient:\n");
  while(!feof(fp)) 
  {
  
     fscanf(fp,"%s %d %d %d %f %f %f %s\n",symbol,&probe_type,&time1,&time2,
                                        &sec,&speed,&value,symbol);
     if (probe_type==type_num)
     {
          fprintf(tp," %s %d %d %d   %7.5f %9.5f   %7.5E %s\n",symbol,probe_type,
                                                time1,time2,sec,speed,value,symbol);
          x++;
     }

     data = (float *) malloc(120 * sizeof(float));
  
     for(k=0;k<10;k++)                    /*Counting number of rows.*/
     {
         for(i=0;i<12;i++)                /*Counting number of columns.*/
	 {  
            n++;                          /*Counting number of data values.*/
            fscanf(fp,"%f",&data[i]);
         }
     fscanf(fp,"\n");
     }
   }
   fprintf(tp,"\n");
   fprintf(tp,"No. of blocks: %d\n",x);
   printf("No. of blocks: %d\n",x);
}  


/**********************************************************************/
/*                                                                    */
/*                      Print raw data values only.                   */
/*                                                                    */
/**********************************************************************/                                      

                          
void Print_data()
{
  int i;
  int k; 
  int n;
  int x;
  x = 0;
  n = 0;

  Read_file_header();
  printf("Please be patient: \n");
  while(!feof(fp)) 
  {
     fprintf(tp," ");                                     
     fscanf(fp,"%s %d %d %d %f %f %f %s\n",symbol,&probe_type,&time1,&time2,
                                        &sec,&speed,&value,symbol);

     fprintf(tp," %s %d %d %d   %7.5f %9.5f   %7.5E %s\n",symbol,probe_type,
                                                time1,time2,sec,speed,value,symbol);
     x++;
    
     data = (float *) malloc(120 * sizeof(float));

     for(k=0;k<10;k++)                    /*Counting number of rows.*/
     { fprintf(tp," ");
         for(i=0;i<12;i++)                /*Counting number of columns.*/
	 {  
            n++;                          /*Counting number of data values.*/
            fscanf(fp,"%f",&data[i]);             
            fprintf(tp," %6.4E",data[i]);
            }
     fscanf(fp,"\n");
     fprintf(tp,"\n");
      }
     
   }
   fprintf(tp,"\n\n");
   fprintf(tp,"No. of blocks: %d\n",x);
   fprintf(tp,"No. of data values: %d\n",n);
   printf("No. of blocks: %d\n",x);
   printf("No. of data values: %d\n",n);  
   fprintf(tp,"\n\n"); 
}


/***************************************************************************/
/*                                                                         */
/*        Print the minimum and maximum values for each record.            */
/*                                                                         */
/***************************************************************************/

void Print_min_max()
{
  int i;
  int k;
  int n;
  int x;
  x = 0;
  n = 0;
  
  Read_file_header();
  

  fprintf(tp,"|     Record No.  |      Minimum       |       Maximum     |\n");
  fprintf(tp,"|==========================================================|\n");
  printf("Please be patient:\n");
  while(!feof(fp)) 
  {
      fscanf(fp,"%s %d %d %d %f %f %f %s\n",symbol,&probe_type,&time1,&time2,
                                        &sec,&speed,&value,symbol);
      x++;
      min = 0;
      max = 0;
      data = (float *) malloc(120 * sizeof(float));
   
     for(k=0;k<10;k++)                    /*Counting number of rows.*/
     {/* fprintf(tp," ");*/
         for(i=0;i<12;i++)                /*Counting number of columns.*/
	 {  
            n++;                          /*Counting number of data values.*/
            fscanf(fp,"%f",&data[i]); 
             if(data[i]<=min)
              min = data[i];
            if (data[i]>=max)
              max = data[i];
  	 }
      fscanf(fp,"\n");

      }
       fprintf(tp,"|      %4d       |    %6.4E      |     %6.4E    |\n",x,min,max);
   }

   fprintf(tp,"\n\n");
   fprintf(tp,"No. of blocks: %d\n",x);
   printf("No. of blocks: %d\n",x);
   fprintf(tp,"\n\n"); 
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

  





