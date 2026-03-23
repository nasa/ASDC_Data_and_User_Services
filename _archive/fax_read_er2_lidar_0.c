/*****************************************************************
*                                                                *
*   FILE: read_lidar.c    Version: 1.0  Date: 060195             *
*                                                                *
* AUTHOR: Tom Feltman, Langley DAAC                              *
*                                                                *
* PURPOSE/DESCRIPTION:                                           *
*	This program is designed to read the FIRE ASTEX ER2 LIDAR*
*	data granules.  The file is set up to contain a single   *
*	header record, followed by pairs of CLS Calibrated and   *
*	Analyzed data records.                                   *
*                                                                *
* INVOCATION:                                                    *
*	ax_er2_lidar (if Makefile.fax is used to compile program)*
*                                                                *
* INTERNALROUTINES:                                              *
*	NONE                                                     *
*****************************************************************/

#ifndef HPUX
#include <sys/ieeefp.h>
#include <floatingpoint.h>
#else
#define _HPUX_SOURCE
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

#define MAXLINE 132
#define BSIZE  26680
#define file_header 1
#define ana_data_header 2
#define signal_header 4
#define channel_1 8
#define channel_2 16
#define channel_3 32
#define channel_4 64 

#define rec_size 26680

extern int errno;
#ifdef PROTO
main(int argc, char **argv)
#else
main(argc, argv)
int argc;
char **argv;
#endif
{

FILE *inf, *outf;
int in_file_handle;
char buf[BSIZE];
float fbuf[(BSIZE/4)+1],*fptr;
double dbl[15];
char *cptr;
char tmp[81],fmt[MAXLINE];
char line[MAXLINE],inname[MAXLINE], outname[MAXLINE];
int i,j,k,*iptr;
int out_control=0,out_flags=0;
int start_rec=0,end_rec=0,num_recs;
/*
#ifdef HPUX
extern struct stat file_status;
#else
*/
struct stat file_status;
/*
#endif
*/

printf("Enter Lidar file name: ");
gets(inname);
if ((inf=fopen(inname,"r")) == NULL) {
  printf("error openning file %s\n",inname);
  exit(1);
  }
printf("Enter Output file name ( a dash defaults to screen) :\t");
gets(outname);
if (outname[0] == '-') outf = stdout;
  else
    if (access(outname,2) == 0) {
     printf(
     "*******************************************************************\n");
     printf(
     "*                                                                 *\n");
     printf(
     "*    WARNING FILE EXISTS - PLEASE CHOOSE OPTION BELOW             *\n");
     printf(
     "*                                                                 *\n");
     printf(
     "*    1 - stop now                                                 *\n");
     printf(
     "*    2 - append to existing file %s          *\n", outname);
     printf(
     "*    3 - overwrite file %s                   *\n", outname);
     printf(
     "*                                                                 *\n");
     printf(
     "*******************************************************************\n");
     
     while ((out_control <1) || (out_control > 3)) {
       printf("\n\t Please enter number of action to take (1-3): ");
       scanf("%d",&out_control);
     }
     switch (out_control) {
       case 1: /*exit now */
               exit(0);
               break;
       case 2: /* open for append */
               if ((outf = fopen(outname,"a")) == NULL) {
                 printf("error opening file %s\n",outname);
                 exit(1);
               }
               break;
       case 3: /* overwrite file */
               if ((outf = fopen(outname,"w")) == NULL) {
                 printf("error opening file %s\n",outname);
                 exit(1);
               }
               break;
     default : printf("Help - bad action value %d \n",out_control);
               exit(1);
               break;
     } /* end switch */
   } /* end access == 0 test */
   else
     if ((outf = fopen(outname,"w")) == NULL) {
       printf("error opening file %s\n",outname);
       exit(1);
     }

printf("*******************************************************************\n");
printf("*                                                                 *\n");
printf("*                Customized Output Menu                           *\n");
printf("*                ======================                           *\n");
printf("*                                                                 *\n");
printf("*  1) File Header                                                 *\n");
printf("*  2) CLS Analyzed Data Header                                    *\n");
printf("*  3) CLS Calibrated Signal Header                                *\n");
printf("*  4) Channel 1 data                                              *\n");
printf("*  5) Channel 2 data                                              *\n");
printf("*  6) Channel 3 data                                              *\n");
printf("*  7) Channel 4 data                                              *\n");
printf("*                                                                 *\n");
printf("*                                                                 *\n");
printf("*  enter 0 (zero) to indicate selection process complete          *\n");
printf("*                                                                 *\n");
printf("*******************************************************************\n");

out_control = -1;
while (out_control != 0) {
  if ((out_control < 0) || (out_control > 7))
    printf("\n\n\tPlease enter number of desired selection (1-7) (0 = done): ");
  scanf("%d",&out_control);
  switch (out_control) {
    case 0 : /* user finished selecting options */
             break;
    case 1 : /* File Header */
             out_flags = out_flags | file_header;
             break; 
    case 2 : /* data header */
             out_flags = out_flags | ana_data_header;
             break;
    case 3 : out_flags = out_flags | signal_header;
             break;
    case 4 : out_flags = out_flags | channel_1;
             break;
    case 5 : out_flags = out_flags | channel_2;
             break;
    case 6 : out_flags = out_flags | channel_3;
             break;
    case 7 : out_flags = out_flags | channel_4;
             break;
    default: printf(
             "\tERROR - selected value out of range = %d\n",out_control);
             printf(
               "\tPlease enter number of desired selection (1-7) (0 = done): ");
             break;
  } /* end switch */

} /* end while */

in_file_handle = fileno(inf);
/* temp. use num_recs for return status */
num_recs = fstat(in_file_handle,&file_status);
if (num_recs == -1) {
  printf("ERROR DETECTED - cann't status input file - error = %d\n",errno);
  exit(1);
}
num_recs = ((int)file_status.st_size/rec_size) - 1; /* Subtract one BECAUSE last 
                                                  record in file was written 
                                                  out twice */
printf("\n\n There are %d records in this file. The first record is the file\n",        num_recs);
printf(" header.  This is followed by pairs of CLS Calibrated Signal & CLS\n");
printf(" Analyzed Data Headers.  Please indicate start and end record\n");
printf(" numbers to extract.  If you enter -1 for the starting record, all\n");
printf(" records will be examined and extracted in accordance with the\n");
printf(" previously specified criteria. NOTE - if the File Header was\n");
printf(
     " requested, it will be output reguardless of starting record number\n\n");
while (end_rec == 0) {
  printf(" Please enter starting record number (-1 or %d >= number >= 1): ",
         num_recs);
  scanf("%d",&start_rec);
  if ((start_rec > 0 ) && (start_rec <= num_recs)) {
    while ((end_rec < start_rec) || (end_rec > num_recs)) {
      printf("enter last record to examine ( %d >= number >= %d): ",num_recs,
             start_rec);
      scanf("%d",&end_rec);
    }
  }
  else
  {
    if (start_rec == -1) {
      start_rec = 1;
      end_rec = num_recs;
    }
  }
}

/* read Lidar file header */
  if ((j=fread((void *)buf,sizeof(char),BSIZE,inf)) == NULL) {
    printf(" read failed on first read attempt\n");
    exit(1);
  }
  cptr = buf;
  if ((out_flags & file_header) == file_header) {
    memcpy(tmp,cptr,9);
    tmp[9] = NULL;
    cptr += 9;
    fprintf(outf,
     "\t\n\n************** CLS DATA FILE HEADER RECORD  **************\n\n");
    fprintf(outf,"\t%s\t= CLS DATA START TIME(UT)\n",tmp);
    memcpy(tmp,cptr,8);
    tmp[8] = NULL;
    cptr +=8;
    fprintf(outf,"\t%s\t= CLS DATA END TIME(UT)\n",tmp);
    memcpy(tmp,cptr,8);
    cptr +=8;
    fprintf(outf,"\t%s\t= ER2 SORTIE START JULDAY(UT)\n",tmp); 
    memcpy(tmp,cptr,8);
    cptr +=8;
    fprintf(outf,"\t%s\t=  ER2 SORTIE END JULDAY(UT)\n",tmp);
    memcpy(tmp,cptr,8);
    cptr +=8;
    fprintf(outf,"\t%s\t=  ER2 SORTIE NUMBER\n",tmp);
    memcpy(tmp,cptr,5);
    tmp[5] = NULL;
    cptr +=5;
    fprintf(outf,"\t%s\t= DETECTOR NUMBER IN CHANNEL 1 POSITION\n",tmp);
    memcpy(tmp,cptr,5);
    tmp[5] = NULL;
    cptr +=5;
    fprintf(outf,"\t%s\t= DETECTOR NUMBER IN CHANNEL 2 POSITION\n",tmp);
    memcpy(tmp,cptr,5);
    cptr +=5;
    fprintf(outf,"\t%s\t= DETECTOR NUMBER IN CHANNEL 3 POSITION\n",tmp);
    memcpy(tmp,cptr,5);
    cptr +=5;
    fprintf(outf,"\t%s\t= DETECTOR NUMBER IN CHANNEL 4(LINEAR AMP)\n",tmp);
    /* move to "English text" */
    cptr = buf + 1024;
    tmp[80] = NULL;
    for (i=0;i<23;i++) {
      memcpy(tmp,cptr,80);
      cptr += 80;
      fprintf(outf,"%s\n",tmp);
    }
  } /* end of out_control < 8 */
  
/*  Now we're ready to start read in "pairs of 26680 byte records of 32 bit
    floating point data  */

  if (start_rec > 2)  {  /* need to position to correct location */
    i = start_rec;
    if ((fseek(inf,(long)((start_rec - 1) * rec_size),SEEK_SET)) != 0) {
       printf("seek to record %d of file %s failed -- exiting \n",i,inname);
       exit(1);
    }
    if (start_rec%2) { /* user's asking to start with data record first */
      if (fread((void *)buf,sizeof(char),BSIZE,inf) == NULL) {
        fprintf(outf," read failed on read of RECORD #%d\n",i);
        exit(1);
      }
      if (((out_flags & ana_data_header) == ana_data_header) && (i <= end_rec)){
        fprintf(outf,
         "\t\n\n************** CLS ANALYZED DATA RECORD #%d **************\n\n",
         i);
      
        cptr = buf;
        memcpy(tmp,cptr,8);
        tmp[8] = NULL;
        cptr += 8;
        fprintf(outf,"\t%s = time in hours, UT from ER2 INS\n",tmp);
        memcpy(tmp,cptr,8);
        cptr += 8;
        fprintf(outf,"\t%s = decimal latitude; to 1/100 degree;INS\n",tmp);
        memcpy(tmp,cptr,8);
        cptr += 8;
        fprintf(outf,"\t%s = decimal longitude;l/100 degress;INS\n",tmp);
        memcpy(tmp,cptr,7);
        tmp[7] = NULL;
        cptr += 7;
        fprintf(outf,"\t%s = ER2 pressure altitude;l/100 kms. INS\n",tmp);
        memcpy(tmp,cptr,7);
        cptr += 7;
        fprintf(outf,"\t%s = ER2 aircraft rool; 1/100 degrees, INS\n",tmp);
        memcpy(tmp,cptr,2);
        cptr += 2;
        tmp[2] = NULL;
        fprintf(outf,"\t%s = Number of cloud layers detected;<=5~\n",tmp);
        memcpy(tmp,cptr,2);
        cptr += 2;
        tmp[2] = NULL;
        fprintf(outf,"\t%s = Ground detection flag;l=TRUE;0=FALSE\n",tmp);
        memcpy(tmp,cptr,14);
        cptr += 14;
        tmp[14] = NULL;
        fprintf(outf,"\t%s = top and bottom of detected layer 1\n",tmp);
        memcpy(tmp,cptr,14);
        cptr += 14;
        fprintf(outf,"\t%s = top and bottom of detected layer 2\n",tmp);
        memcpy(tmp,cptr,7);
        cptr += 7;
        tmp[7] = NULL;
        fprintf(outf,"\t%s = occasional annotation of time,hhmmss\n",tmp);
        memcpy(tmp,cptr,8);
        tmp[8] = NULL;
        cptr += 8;
        fprintf(outf,"\t%s = time in hours;matches time UT from ER2 INS above\n",
                tmp);
        memcpy(tmp,cptr,15);
        cptr += 15;
        tmp[15] = NULL;
        fprintf(outf,"\t%s = top and bottom of detected layer 3\n",tmp);
        memcpy(tmp,cptr,14);
        cptr += 14;
        tmp[14] = NULL;
        fprintf(outf,"\t%s = top and bottom of detected layer 4\n",tmp);
        memcpy(tmp,cptr,14);
        cptr += 14;
        fprintf(outf,"\t%s = top and bottom of detected layer 5\n",tmp);
        memcpy(tmp,cptr,9);
        tmp[9] = NULL;
        cptr += 9;
        fprintf(outf,"\t%s = departure of height of earth's surface from 0\n",
                tmp);
      }
    } /* start_rec%2  first record  processed was a Analyzed data record */
    else
    {
     i = start_rec - 1;  /* subtract 1 since i is increment at top of other loop*/
    }
  } /* end of start_rec > 2 */
  else
    i = 1;

  while ((feof(inf) == 0) && (i <= end_rec)) { /* read in data */
    i++;
    if (i > end_rec) {
fprintf(stdout,"*******************************************************************\n");
fprintf(stdout,"*                                                                 *\n");
fprintf(stdout,"*                      program complete                           *\n");
fprintf(stdout,"*                                                                 *\n");
fprintf(stdout,"*******************************************************************\n");
      exit(0);
    }
    /* read CLS Calibrated Signal Record */
/*  The first 200 bytes comprise a header */

    if ((j = fread((void*)fbuf,sizeof(char),BSIZE,inf)) == NULL) {
      if (feof(inf) != 0) printf ("*** END OF FILE DETECTED ***\n");
      else
        fprintf(outf," read failed on read attempt for record %d\n",i);
      exit(1);
    }
    iptr = (int*) fbuf;
    
    /* print CLS Calibrated Signal header info out */
    if ((out_flags & signal_header) == signal_header) {
      fprintf(outf,"\n*********** CLS Calibrated Signal Header Info RECORD #%d***********\n\n",i);
      fprintf(outf,"\t%d = NAVIGATION SERIAL NUMBER\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = AM OR PM INDICATOR\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = TIME STATUS\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = JULIAN DATE\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = TIME OF DAY\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = LATITUDE IN 1/100 DEGREE\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = LONGITUDE IN 1/100 DEGREE\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = NORTH SOUTH SPEED IN 1/10 KNOTS\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = EAST  WEST  SPEED IN 1/10 KNOTS\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = TRUE HEADING IN 1/100 DEGREES\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = GROUND SPEED IN 1/10 KNOTS\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = TOTAL TEMPERATURE (CELSIUS)\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = ER2 ALTITUDE IN FEET\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = TRUE AIR SPEED IN 1/10 KNOTS\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = ER2 PITCH IN 1/100 DEGREES\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = ER2 ROLL IN 1/100 DEGREES\n",*iptr);
      iptr++;
      fprintf(outf,"\t\t ===== 15 CLS ENGINEERING VALUES =====\n"); 
      fprintf(outf,"\t%d\t%d\t%d\t%d\t%d\n",*iptr,*(iptr+1),*(iptr+2),*(iptr+3),*(iptr+4)); 
      iptr += 5;
      fprintf(outf,"\t%d\t%d\t%d\t%d\t%d\n",*iptr,*(iptr+1),*(iptr+2),*(iptr+3),*(iptr+4)); 
      iptr += 5;
      fprintf(outf,"\t%d\t%d\t%d\t%d\t%d\n",*iptr,*(iptr+1),*(iptr+2),*(iptr+3),*(iptr+4)); 
      iptr += 5;
      fprintf(outf,"\t%d = 1064NM SHOT ENERGY\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = POD ROLL IN 1/100 DEGREES\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = POD PITCH IN 1/100 DEGREES\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = SHOT NUMBER\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = 532NUM ENERGY\n",*iptr);
      iptr++;
      fprintf(outf,"\t%d = CLS OPERATIONS FLAGS\n",*iptr);
      iptr++;
  /*  SPARE LOCATION - NOT USED so printf commented out
      fprintf(outf,"\t%d = 
  */
      iptr++;
      fprintf(outf,"\t\t ===== INVALID SHOT AVERAGE - ONE VALUE PER CHANNEL =====\n");
      fprintf(outf,"\t%d\t%d\t%d\t%d\n",*iptr,*(iptr+1),*(iptr+2),*(iptr+3));
      iptr += 4;
      fptr = (float*) iptr;
      fprintf(outf,"\t\t ===== AVERAGED PRETRIGGER SIGNALS =====\n");
      for (k=0;k<4;k++) dbl[k] = (double) *(fptr+k);
        fprintf(outf,"\t%13.4E%13.4E%13.4E%13.4E\n",
                 dbl[0],dbl[1],dbl[2],dbl[3]);
      fptr += 4;
      fprintf(outf,"\t\t ===== AVERAGED BACKGROUND SIGNALS =====\n");
        for (k=0;k<4;k++) dbl[k] = (double) *(fptr+k);
        fprintf(outf,"\t%13.4E%13.4E%13.4E%13.4E\n",
                 dbl[0],dbl[1],dbl[2],dbl[3]);
      fptr += 4;
    } 
    else
    {  /* have to move pointer 200 bytes in case user only wants data values */
      fptr = (float *) (iptr + 200);
    }
    /* Now print out 6620 values for AVERAGED CLS Backscatter Values */ 
    if (out_flags >= 8) fprintf(outf,
      "\n\t\t ===== 1665 Values per channel for AVERAGED CLS Backscatter =====\n"); 
    if ((out_flags & channel_1) == channel_1) {
      fprintf(outf,"\nindex\t\t CHANNEL 1 \n\n");
      for (j=0;j<1655;j+=5,fptr+=5) {
        for (k=0;k<5;k++) dbl[k] = (double) *(fptr+k);
        fprintf(outf,"%d\t%13.4E%13.4E%13.4E%13.4E%13.4E\n",
                 j,dbl[0],dbl[1],dbl[2],dbl[3],dbl[4]);
      }
    }
    else
      fptr+=1655; /* skip over channel 1 data */
    if ((out_flags & channel_2) == channel_2) {
      fprintf(outf,"\nindex\t\t CHANNEL 2 \n\n");
      for (j=0;j<1655;j+=5,fptr+=5) {
        for (k=0;k<5;k++) dbl[k] = (double) *(fptr+k);
        fprintf(outf,"%d\t%13.4E%13.4E%13.4E%13.4E%13.4E\n",
                 j,dbl[0],dbl[1],dbl[2],dbl[3],dbl[4]);
      }
    }
    else
      fptr+=1655; /* skip over channel 2 data */
    if ((out_flags & channel_3) == channel_3) {
      fprintf(outf,"\nindex\t\t CHANNEL 3 \n\n");
      for (j=0;j<1655;j+=5,fptr+=5) {
        for (k=0;k<5;k++) dbl[k] = (double) *(fptr+k);
        fprintf(outf,"%d\t%13.4E%13.4E%13.4E%13.4E%13.4E\n",
                 j,dbl[0],dbl[1],dbl[2],dbl[3],dbl[4]);
      }
    }
    else
      fptr+=1655; /* skip over channel 3 data */
    if ((out_flags & channel_4) == channel_4) {
      fprintf(outf,"\nindex\t\t CHANNEL 4 \n\n");
      for (j=0;j<1655;j+=5,fptr+=5) {
        for (k=0;k<5;k++) dbl[k] = (double) *(fptr+k);
        fprintf(outf,"%d\t%13.4E%13.4E%13.4E%13.4E%13.4E\n",
                 j,dbl[0],dbl[1],dbl[2],dbl[3],dbl[4]);
      }
    }
    else
      fptr+=1655; /* skip over channel 4 data */
  
  /****  END OF THE FIRST 26680 BYTE RECORD OF THE TWO RECORD SET ****/
  
/*     CLS ANALYZED DATA RECORD       */
    i++;
    if (i > end_rec) {
fprintf(stdout,"*******************************************************************\n");
fprintf(stdout,"*                                                                 *\n");
fprintf(stdout,"*                      program complete                           *\n");
fprintf(stdout,"*                                                                 *\n");
fprintf(stdout,"*******************************************************************\n");
      exit(0);
    }
    if (fread((void *)buf,sizeof(char),BSIZE,inf) == NULL) {
      fprintf(outf," read failed on read of RECORD #%d\n",i);
      exit(1);
    }
    if ((out_flags & ana_data_header) == ana_data_header) {
      fprintf(outf,
       "\t\n\n************** CLS ANALYZED DATA RECORD #%d **************\n\n",i);
    
      cptr = buf;
      memcpy(tmp,cptr,8);
      tmp[8] = NULL;
      cptr += 8;
      fprintf(outf,"\t%s = time in hours, UT from ER2 INS\n",tmp);
      memcpy(tmp,cptr,8);
      cptr += 8;
      fprintf(outf,"\t%s = decimal latitude; to 1/100 degree;INS\n",tmp);
      memcpy(tmp,cptr,8);
      cptr += 8;
      fprintf(outf,"\t%s =decimal longitude;l/100 degress;INS\n",tmp);
      memcpy(tmp,cptr,7);
      tmp[7] = NULL;
      cptr += 7;
      fprintf(outf,"\t%s = ER2 pressure altitude;l/100 kms. INS\n",tmp);
      memcpy(tmp,cptr,7);
      cptr += 7;
      fprintf(outf,"\t%s = ER2 aircraft rool; 1/100 degrees, INS\n",tmp);
      memcpy(tmp,cptr,2);
      cptr += 2;
      tmp[2] = NULL;
      fprintf(outf,"\t%s = Number of cloud layers detected;<=5~\n",tmp);
      memcpy(tmp,cptr,2);
      cptr += 2;
      tmp[2] = NULL;
      fprintf(outf,"\t%s = Ground detection flag;l=TRUE;0=FALSE\n",tmp);
      memcpy(tmp,cptr,14);
      cptr += 14;
      tmp[14] = NULL;
      fprintf(outf,"\t%s = top and bottom of detected layer 1\n",tmp);
      memcpy(tmp,cptr,14);
      cptr += 14;
      fprintf(outf,"\t%s = top and bottom of detected layer 2\n",tmp);
      memcpy(tmp,cptr,7);
      cptr += 7;
      tmp[7] = NULL;
      fprintf(outf,"\t%s = occasional annotation of time,hhmmss\n",tmp);
      memcpy(tmp,cptr,8);
      tmp[8] = NULL;
      cptr += 8;
      fprintf(outf,"\t%s = time in hours;matches time UT from ER2 INS above\n",tmp);
      memcpy(tmp,cptr,15);
      cptr += 15;
      tmp[15] = NULL;
      fprintf(outf,"\t%s = top and bottom of detected layer 3\n",tmp);
      memcpy(tmp,cptr,14);
      cptr += 14;
      tmp[14] = NULL;
      fprintf(outf,"\t%s = top and bottom of detected layer 4\n",tmp);
      memcpy(tmp,cptr,14);
      cptr += 14;
      fprintf(outf,"\t%s = top and bottom of detected layer 5\n",tmp);
      memcpy(tmp,cptr,9);
      tmp[9] = NULL;
      cptr += 9;
      fprintf(outf,"\t%s = departure of height of earth's surface from 0\n",tmp);
    }
      
  } /* end of while not EOF */  

fprintf(stdout,"*******************************************************************\n");
fprintf(stdout,"*                                                                 *\n");
fprintf(stdout,"*                      program complete                           *\n");
fprintf(stdout,"*                                                                 *\n");
fprintf(stdout,"*******************************************************************\n");
}
