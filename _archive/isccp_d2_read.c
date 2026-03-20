/*************************************************************************/
/*  name - isccp_d2_read.c                     
 *
 *  Purpose - This program reads ISCCP D2 HDF formatted data files
 *
 *  Inputs - HDF file name, output file name, subsetting information
 *  Output - Report file 
 *
 *  Key Local Parameters - maxrank - max rank of HDF SDS
 *                         hdfname - name of HDF file
 *                         begin1 - beginning cell number
 *                         end1 - ending cell number
 *                         dataitems - data items to be read
 *                         status - input ok or not
 *
 * Functions Called -     
 *                        getinput - gets data date and file type
 *                        readhdf - reads file id and description, etc.
 *                        getdata - gets parameters to subset
 *                        readdata - reads data, max, mins, etc
 *************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hdf.h"

#define MAXRANK    2
#define LISTSIZE   55
#define FIRST      1
#define MAXLEN     256
#define MAXLNG     132
#define STARTPOS   1

int main(argc,argv)

int argc;
char **argv;

{

 char hdfname[MAXLEN], outfile[MAXLEN];
 char  subset_ans[MAXLEN];
 char  param_ans[MAXLEN];
 int32 begin1, end1, begin2, end2;
 uint16 dataitems[LISTSIZE];
 FILE *out;

 /*  function prototypes */

 char *getinput(); /* gets name of input file */
 void readhdf();   /* reads 5 different files from HDF file */
 void getparam(); /* get user selected parameter(s) to read */
 void getarea();   /* gets area based on cell number */
 void getdata();   /* retrieves data based on cell number */
 void readdata();  /* reads data applying conversion values */

/* Try to extract date and file type from command line */
 if (argc == 3){
   strcpy(hdfname,argv[1]);
   if (strcmp(argv[2],"-") == 0) {
     strcpy(outfile,"stdout");
   } else {
     strcpy(outfile,argv[2]);
   }
 }

 else  /*  prompt for info if necessary */

 {
   printf("This program reads the ISCCP D2 HDF formatted data files \n ");
   getinput(hdfname,outfile);
  }


  printf("Input file is %s\n\n",hdfname);
  printf("Output file is %s\n\n",outfile);

/* open output file */

  if (strcmp(outfile,"stdout") == 0) {
    out = stdout;
  }
  else
  {
    if ((out=fopen(outfile,"wt")) == NULL)
    {
      printf("Cannot open output file %s\n\n",outfile);
      exit(1);
    }
  }
  
/*  call function to read file id */

  readhdf(out,hdfname);

/* call funtion to select parameter */
  
  getparam(param_ans,&begin2,&end2);

/*  call function to subset area */

  getarea(subset_ans,&begin1,&end1);

/*  call function to get parameters */

  getdata(out,hdfname,dataitems);

/*  call function to read data */

  printf("\n\nGetting data, Please wait ...\n");
  readdata(out,hdfname,dataitems,subset_ans,begin1,end1,begin2,end2);
  fprintf(stderr,
          "\n***************************************************************\n");
  fprintf(stderr,
          "*                                                             *\n");
  fprintf(stderr,
          "*         Program has completed processing                    *\n");
  fprintf(stderr,
          "*                                                             *\n");
  fprintf(stderr,
          "***************************************************************\n");

}
/***********************************************************************/
/*   name - getinput  
 *
 *   Purpose - This subroutine accepts user input for the input & output
 *             file names
 *
 *   Input parameters -  none
 *   Output parameters - hdfname - input file name
 *                       outfile - output file name. If = '-' then prints
 *                                 to standard output (stdout)
 *
 *  Key Local Parameters - NONE
 *  Functions Called - NONE
 ************************************************************************/
  
 char *getinput(hdfname,outfile)
 char *hdfname,*outfile;

 {
  int out_control;
  do {
    printf("Please enter file name to read: ");
    scanf("%s",hdfname);
    printf("\n\n");
  } while (strlen(hdfname) == 0);

  
  do {
    printf("Please enter report file name (use `-' to print to screen): ");
    scanf("%s",outfile);
    if (*outfile == '-') {
       strcpy(outfile,"stdout");
     }
    else
     {
        if (access(outfile,2) == 0) {
           printf(
           "******************************************************************\n");
           printf(
           "*                                                                *\n");
           printf(
           "*    WARNING FILE EXISTS - PLEASE CHOOSE OPTION BELOW            *\n");
           printf(
           "*                                                                *\n");
           printf(
           "*    1 - stop now                                                *\n");
           printf(
           "*    2 - overwrite file %s                  *\n", outfile);
           printf(
           "*                                                                *\n");
           printf(
           "******************************************************************\n");
            out_control = 0;
          while ((out_control <1) || (out_control > 2)) {
          printf("\n\t Please enter number of action to take (1-2): ");
          scanf("%d",&out_control);
        }
          if (out_control == 1) {
             fprintf(stderr,"EXITING PROGRAM\n");
             exit(0);
           }
        } /* end of access */
    printf("\n\n");
   } /* end of else */
  } while (strlen(outfile) == 0);
   printf("\n");
  printf("************************************************************************\n");
  printf("ISCCP_D2 HDF formatted files include five file descriptions - they are:\n");
  printf("1) README SECTION - including information the contributing satellites,\n");
  printf(" processing centers, file format description, etc.\n");
  printf("2) TABLE OF CONTENTS - Lists the date and spatial coverage of each \n");
  printf("data object in the HDF file\nin ASCII text tabular form and cell stats\n");
  printf("3) FORTRAN READ SOFTWARE\n");
  printf("4) ANCILLARY DATA TABLE - Lists characteristics of each ISCCP grid \n");
  printf("box in tabular form\n");
  printf("5) CALIBRATION - Calibration correction factor\n");
  printf("***********************************************************************\n\n");
  printf("\n\n This program can write these descriptors into a set of files\n");
  return 0;
 } /* end of get input */

/************************************************************************
 *  name - readhdf                              
 *
 * Purpose - This subroutine reads the file id and file description
 *
 * Input -   hdfname - the name of the HDF input file
 *
 * Key Local Parameters :  iret - return code from HDF routines
 *                         dfile - pointer to HDF file
 *                         id - file id
 *                         filedesc - file description
 *
 * Functions Called :  HDF functions to get file id, file description
 *******************************************************************/

void readhdf(out,hdfname)

 char *hdfname;
 FILE *out;

 {
  int32 iret,dfile,idlen,deslen,out_control;
  int    vdata_ref=-1,
         vdata_id,
         i, 
         iref;
  float32 buf1[6596],buf2[6596],buf3[6596],buf4[6596],buf5[6596],buf6[6596],buf7[6596],buf8[6596],buf9[6596],
          buf10[6596],buf11[6596];
  char   str[7];
  char *fileid,                   /*pointer for file identifier */
       *filedes;                  /*pointer for file description */
  char kind[MAXLEN],              /* input kind */
       name1[MAXLEN],             /* name for Read me */
       name2[MAXLEN],             /* name for table of contents */
       name3[MAXLEN],             /* name for FORTRAN READ SOFTWARE */
       name4[MAXLEN],             /* name for ancillary file */
       name5[MAXLEN];             /* name for calibration correction factor */
  FILE *des1, *des2, *des3, *des4,*des5; /* five pointers for 5 HDF files */

  dfile = Hopen(hdfname,DFACC_READ,0);
  if (dfile == -1){
    fprintf(out,"%s %s\n\n","Error in opening HDF file",hdfname);
    exit(1);
  }

 /* get file id */

  idlen = DFANgetfidlen(dfile,FIRST);
  if (idlen == (int32) -1) {
    printf("File doesn't have a file identifier\n");
  }
  else
  {
    fileid = malloc(idlen+1);
    *(fileid+idlen) = NULL;
    iret = DFANgetfid(dfile,fileid,idlen+1,(intn)FIRST);
    if (iret == -1){
      fprintf(out,"%s%d\n\n","Error in getting file id: id = ",dfile);
      exit(1);
    }
    else
    {
      fprintf(out,"%s %s\n\n","File ID: ",fileid);
      idlen = DFANgetfidlen(dfile,(int32) 0);
      while (idlen != (int32) -1) {
        free (fileid);
        fileid = malloc(idlen);
        iret = DFANgetfid(dfile,fileid,idlen,(intn)0);
        if (iret == -1){
          fprintf(out,"%s%d\n\n","Error in getting file id: id = ",dfile);
          free(fileid);
          exit(1);
        }
        fprintf(out,"%s %s\n\n","File ID: ",fileid);
        idlen = DFANgetfidlen(dfile,(intn) 0);
      } /* end while */

      free(fileid);
    } /* end else process file ids */

  } /* end else we have a file id */

  /* PROCESS FILE DESCRIPTIONS */

 printf("Would you like to have the README SECTION, TABLE OF CONTENTS,\n");
 printf("FORTRAN READ software and ANCILLARY FILE written to local files? (Y or N): ");
 scanf("%s",kind);
 printf("\n");
 kind[0] = toupper(kind[0]);
 if (kind[0] == 'N') {
                        iret = Hclose(dfile);
                        return;
                     }
  else
   {
          /* kind[0] != 'N' so Generate FILES */

    do {
         printf("\nPlease enter file name to write the README data into: ");
         scanf("%s",&name1[0]);
         printf("\n");
         if (access(&name1[0],2) == 0) {
        printf(
        "******************************************************************\n");
        printf(
        "*                                                                *\n");
        printf(
        "*    WARNING FILE EXISTS - PLEASE CHOOSE OPTION BELOW            *\n");
        printf(
        "*                                                                *\n");
        printf(
        "*    1 - stop now                                                *\n");
        printf(
        "*    2 - overwrite file %s                  *\n", &name1[0]);
        printf(
        "*                                                                *\n");
        printf(
         "******************************************************************\n");

          out_control = 0;
          while ((out_control <1) || (out_control > 2)) {
            printf("\n\t Please enter number of action to take (1-2): ");
            scanf("%d",&out_control);
           }
           if (out_control == 1) {
           fprintf(stderr,"EXITING PROGRAM\n");
           exit(0);
        }
 
        printf("\n");
      } /*end access */
 
    } while (strlen(&name1[0]) == 0);
     if ((des1=fopen(&name1[0],"wt")) == NULL)
      {
        fprintf(stderr,"Cannot open output file %s\n\n",&name1[0]);
        iret = Hclose(dfile);
        exit(1);
      }
     deslen = DFANgetfdslen(dfile,(intn)FIRST);
    if (deslen == (int32) -1) {
      fprintf(stderr,
        "ERROR - no length for file description 1 - trying next section\n");
    }
    else
     {
       filedes = malloc(deslen);
      iret = DFANgetfds(dfile,filedes,deslen,(intn)FIRST);
      if (iret == -1) {
        fprintf(stderr,
          "ERROR - read for file description failed - trying next section\n");
        free(filedes);
      }
    else
      {
        fprintf(des1,"%s",filedes);
        fprintf(des1,"\n");
        free(filedes);
        fclose(des1);
      }
    } /* end des1 processing */

     /*  GET NAME FOR Table Of Contents FILE   */

   do {
         printf("Please enter file name to write the Table Of Contents data into: ");
      scanf("%s",&name2[0]);
      printf("\n");
      if (access(&name2[0],2) == 0) {
        printf(
        "******************************************************************\n");
        printf(
        "*                                                                *\n");
        printf(
        "*    WARNING FILE EXISTS - PLEASE CHOOSE OPTION BELOW            *\n");
        printf(
        "*                                                                *\n");
        printf(
        "*    1 - stop now                                                *\n");
        printf(
        "*    2 - overwrite file %s                  *\n", &name2[0]);
        printf(
        "*                                                                *\n");
        printf(
         "******************************************************************\n");
         out_control = 0;
        while ((out_control <1) || (out_control > 2)) {
          printf("\n\t Please enter number of action to take (1-2): ");
          scanf("%d",&out_control);
        }
        if (out_control == 1) {
           fprintf(stderr,"EXITING PROGRAM\n");
           exit(0);
        }
 
        printf("\n");
      } /*end access */
    } while (strlen(&name2[0]) == 0);
  
     if ((des2=fopen(&name2[0],"wt")) == NULL)
    {
      fprintf(stderr,"Cannot open output file %s\n\n",&name2[0]);
      iret = Hclose(dfile);
      exit(1);
    }
    deslen = DFANgetfdslen(dfile,(intn)0);
    if (deslen != (int32) -1) {
      filedes = malloc(deslen);
      DFANgetfds(dfile,filedes,deslen,(intn)0);
      fprintf(des2,"%s",filedes);
      fprintf(des2,"\n");
      free(filedes);
      fclose(des2);
    }
   else
    {
      fprintf(stderr,
             "ERROR - could not read file descriptor 2 - trying next one\n");
    } /*end des2 processing */

/******************************************************************************/

    do {
         printf("Please enter file name to write the FORTRAN READ SOFTWARE into: ");
         scanf("%s",&name3[0]);
         printf("\n");
       if (access(&name3[0],2) == 0) {
          printf(
        "******************************************************************\n");
        printf(
        "*                                                                *\n");
        printf(
        "*    WARNING FILE EXISTS - PLEASE CHOOSE OPTION BELOW            *\n");
        printf(
        "*                                                                *\n");
        printf(
        "*    1 - stop now                                                *\n");
        printf(
        "*    2 - overwrite file %s                  *\n", &name3[0]);
        printf(
        "*                                                                *\n");
        printf(
         "******************************************************************\n");

          out_control = 0;
        while ((out_control <1) || (out_control > 2)) {
          printf("\n\t Please enter number of action to take (1-2): ");
          scanf("%d",&out_control);
        }
        if (out_control == 1) {
           fprintf(stderr,"EXITING PROGRAM\n");
           exit(0);
        }
 
        printf("\n");
      } /*end access */
    } while (strlen(&name3[0]) == 0);


    if ((des3=fopen(&name3[0],"wt")) == NULL)
    {
      fprintf(stderr,"Cannot open output file FORTRAN READ SOFTWARE file\n\n");
      iret = Hclose(dfile);
      exit(1);
    }
    deslen = DFANgetfdslen(dfile,(intn)0);
    if (deslen != (int32) -1) {
      filedes = malloc(deslen);
      DFANgetfds(dfile,filedes,deslen,(intn)0);
      fprintf(des3,"%s",filedes);
      fprintf(des3,"\n");
      free(filedes);
      fclose(des3);
    }
   else
    {
      fprintf(stderr,
          "ERROR - could not read file descriptor 3 - trying next one\n");
    }
 
/*************************************************************************
        PROCESS ANN DATA FILE ANYWAY
*************************************************************************/

  do {
       printf("Please enter file name to write the Ancillary file into: ");
       scanf("%s",&name4[0]);
       printf("\n");
       if (access(&name4[0],2) == 0) {
          printf(
        "******************************************************************\n");
        printf(
        "*                                                                *\n");
        printf(
        "*    WARNING FILE EXISTS - PLEASE CHOOSE OPTION BELOW            *\n");
        printf(
        "*                                                                *\n");
        printf(
        "*    1 - stop now                                                *\n");
        printf(
        "*    2 - overwrite file %s                  *\n", &name4[0]);
        printf(
        "*                                                                *\n");
        printf(
         "******************************************************************\n");
         out_control = 0;
        while ((out_control <1) || (out_control > 2)) {
          printf("\n\t Please enter number of action to take (1-2): ");
          scanf("%d",&out_control);
        }
        if (out_control == 1) {
           fprintf(stderr,"EXITING PROGRAM\n");
           exit(0);
        }
 
        printf("\n");
      } /*end access */
    } while (strlen(&name4[0]) == 0);

 
  if ((des4=fopen(&name4[0],"wt")) == NULL)
  {
    fprintf(stderr,"Cannot open ancillary data file %s - job aborted\n\n",&name4[0]);
    iret = Hclose(dfile);
    exit(3);
  }
               /* find out if this is a VData record or not */

  deslen = DFANgetfdslen(dfile,(intn)0);
  if (deslen != (int32) -1) {
    filedes = malloc(deslen);
    DFANgetfds(dfile,filedes,deslen,(intn)0);
    printf("%s",filedes);
    printf("\n");
    fprintf(des4,"%s",filedes);
    fprintf(des4,"\n");
    free(filedes); 
    if(deslen < 100)
      {
      printf("The VData records will now be read!\n");
      printf("\n");
      Vstart(dfile);
      iref = VSgetid(dfile,vdata_ref);
/*  THIS ROUTINE DECODES 11 FIELDS FROM THE HDF VDATA RECORD.
    THESE ARE THE ANCILLARY FILE FIELD NAMES.
   GRD BOX */
       strcpy(str,"IBOX");
       vdata_id = VSattach(dfile, iref, "r");
       next_vd_field(hdfname,vdata_id,str,buf1);
/* LAT IND */
       strcpy(str,"J");
       vdata_id = VSattach(dfile, iref, "r");
    next_vd_field(hdfname,vdata_id,str,buf2);
/* LON IND*/
       strcpy(str,"I");
       vdata_id = VSattach(dfile, iref, "r");
    next_vd_field(hdfname,vdata_id,str,buf3);
/* SQ1 IND*/
       strcpy(str,"LONBEG");
       vdata_id = VSattach(dfile, iref, "r");
    next_vd_field(hdfname,vdata_id,str,buf4);
/* SQ2 IND*/
       strcpy(str,"LONEND");
       vdata_id = VSattach(dfile, iref, "r");
    next_vd_field(hdfname,vdata_id,str,buf5);
/* LAT CENTER*/
       strcpy(str,"CENLAT");
       vdata_id = VSattach(dfile, iref, "r");
    next_vd_field(hdfname,vdata_id,str,buf6);
/* LON CENTER*/
       strcpy(str,"CENLON");
       vdata_id = VSattach(dfile, iref, "r");
    next_vd_field(hdfname,vdata_id,str,buf7);
/* BOX AREA*/
       strcpy(str,"IAREA");
       vdata_id = VSattach(dfile, iref, "r");
    next_vd_field(hdfname,vdata_id,str,buf8);
 
/* PER LAND*/
       strcpy(str,"LANDFR");
       vdata_id = VSattach(dfile, iref, "r");
    next_vd_field(hdfname,vdata_id,str,buf9);
/* TOPOG HEIGHT*/
       strcpy(str,"ITOPOG");
       vdata_id = VSattach(dfile, iref, "r");
    next_vd_field(hdfname,vdata_id,str,buf10);
/* VEG TYPE*/
       strcpy(str,"IVEG");
       vdata_id = VSattach(dfile, iref, "r");
       next_vd_field(hdfname,vdata_id,str,buf11);
/* These are the only VData records that are used at this time. */

    fprintf(des4,"***************************************************************\n");
    fprintf(des4,"          Vdata values from file\n");
    fprintf(des4,"***************************************************************\n");
    for (i=0; i<6596; i++)
      {
        if(i == 0)
          {
            fprintf(des4," GRD LAT LON SQ1 SQ2     LAT      LON      BOX  PERC  TOPOG  VEG\n");
            fprintf(des4," BOX IND IND IND IND   CENTER   CENTER    AREA  LAND HEIGHT TYPE\n");
            fprintf(des4,"%4d  %2d   %1d %3d %3d     %4.2f  %7.2f  %6d   %3d   %4d   %1d\n",(int)buf1[i],(int)buf2[i],
                   (int)buf3[i],(int)buf4[i],(int)buf5[i],buf6[i],buf7[i],(int)buf8[i],(int)buf9[i],(int)buf10[i],
                   (int)buf11[i]);
          }
        else
          {
            fprintf(des4,"%4d  %2d   %1d %3d %3d     %4.2f  %7.2f  %6d   %3d   %4d   %1d\n",(int)buf1[i],(int)buf2[i],
                   (int)buf3[i],(int)buf4[i],(int)buf5[i],buf6[i],buf7[i],(int)buf8[i],(int)buf9[i],(int)buf10[i],
                   (int)buf11[i]);
          }
      }
       fclose(des4);
      }
  }
  else
  {
    fprintf(stderr,"ERROR - could not read file descriptor 4 - ANNCILLARY");
    fprintf(stderr," DATA \n This data is required to calculate cloud height");
    fprintf(stderr,"\n\t- data file may be corrupted - job aborted\n");
    exit(3);
  } /* end processing description 4 */

/*****************************************************************************/

    do {
         printf("Please enter file name to write the Calibration Factor into: ");
         scanf("%s",&name5[0]);
         printf("\n");
         if (access(&name5[0],2) == 0) {
           printf(
        "******************************************************************\n");
        printf(
        "*                                                                *\n");
        printf(
        "*    WARNING FILE EXISTS - PLEASE CHOOSE OPTION BELOW            *\n");
        printf(
        "*                                                                *\n");
        printf(
        "*    1 - stop now                                                *\n");
        printf(
        "*    2 - overwrite file %s                  *\n", &name5[0]);
        printf(
        "*                                                                *\n");
        printf(
         "******************************************************************\n");
            out_control = 0;
        while ((out_control <1) || (out_control > 2)) {
          printf("\n\t Please enter number of action to take (1-2): ");
          scanf("%d",&out_control);
        }
        if (out_control == 1) {
           fprintf(stderr,"EXITING PROGRAM\n");
           exit(0);
        }
 
        printf("\n");
      } /*end access */
    } while (strlen(&name5[0]) == 0);


    if ((des5=fopen(&name5[0],"wt")) == NULL)
     {
       printf("Cannot open output file CALIBRATION CORRECTION FACTOR\n\n");
       iret = Hclose(dfile);
       exit(1);
     }
     deslen = DFANgetfdslen(dfile,(intn)0);
     if (deslen != (int32) -1) 
      {
        filedes = malloc(deslen);
        DFANgetfds(dfile,filedes,deslen,(intn)0);
        fprintf(des5,"%s",filedes);
        fprintf(des5,"\n");
        free(filedes);
        fclose(des5);
      }
     else
       {
          printf("ERROR - could not read file descriptor 5 - continuing processing\n");
       } /* end processing description 5 */
 }  /* end of writting out files */

/*************************************************************************
        DONE PROCESSING ANN DATA FILE
*************************************************************************/
 

  iret = Hclose(dfile);
  return;
}
next_vd_field(hdfname,vdata_id,str,buffer)
  int vdata_id;
  char str[7],
       hdfname[256];
  float32 buffer[6596];
{
  int nrec,
      status,
      vsize,
      interlace;
  char fields[80],
       vclass[7],
       vdata_name[7];
 
       status = VSinquire(vdata_id,&nrec,&interlace,fields,&vsize,vdata_name);
       if(status == -1)
         {
            printf("Inquire failed will stop.\n");
            exit(0);
         }
       status = VSelts(vdata_id);
       status = VSgetclass(vdata_id,vclass);
       status = VSsetfields(vdata_id,str);
       status = DFSDndatasets(hdfname);
       status = VSread(vdata_id,(uint8 *)buffer,nrec,interlace);
      VSdetach(vdata_id);
      return(0);
}

/********************************************************************/
/*  Name - getparam                       type - function                    
 *                                                                       
 * Purpose - This functions asks the user to enter the parameter      
 *           number(s) to subset data          
 *                                                                      
 * Input Parameters - NONE                                              
 * Output Parameters -param_ans - enter parameter number(P) or none
 *                    begin2-beginning parameter number               
 *                    end2 - ending parameter number                       
 *                                                                      
 * Key Local Parameters -NONE
 *
 * Functions Called - NONE
 *****************************************************************/

 void getparam(param_ans,begin2,end2)

 char *param_ans;
 int32 *begin2, *end2;
 {
  char kind[MAXLEN];

  printf("Enter whether you would like to select a particular parameter?\n");
  printf("yes (Y) or no subsetting (N): ");
  scanf("%s",kind);
  *param_ans = toupper(kind[0]);
  
  if (param_ans[0] == 'N'){
    *begin2 = 1;
    *end2 = 130;
  }
  else {
     printf("\n\nEnter beginning and end parameter numbers separated by a space\n) ");
    printf(" ( 1 <= parameter number <= 130): ");
    printf("\n\nYou may enter just one parameter (e.g., 8 8) ");
    printf("\n or a first and last parameter (e.g., 32 40)");
    scanf("%d %d",begin2,end2);
    while ((*begin2<1) || (*begin2 > 130) || (*end2 > 130) || (*end2 < 1) || 
            (*end2<*begin2)) {
       printf(" Incorreect entry - please try again.\n");
       printf("\n\nEnter beginning and end parameter numbers (e.g., 8 9) ");
       scanf("%d %d",begin2,end2);
     } 
   }  
  }


/********************************************************************/
/*  Name - getarea                       type - function                    
 *                                                                       
 * Purpose - This functions asks the user to enter the cell      
 *           numbers to subset data          
 *                                                                      
 * Input Parameters - NONE                                              
 * Output Parameters -subset_ans - enter cell number(C) or none
 *                    begin1-beginning cell number               
 *                    end1 - ending cell number                       
 *                                                                      
 * Key Local Parameters -NONE
 *
 * Functions Called - NONE
 *****************************************************************/

 void getarea(subset_ans,begin1,end1)

 char *subset_ans;
 int32 *begin1, *end1;
 {
  char kind[MAXLEN];

  printf("Enter whether you would like to further subset based on lat/lon?\n");
  printf("yes (Y) or no subsetting (N): ");
  scanf("%s",kind);
  *subset_ans = toupper(kind[0]);
  
  if (subset_ans[0] == 'N') {
    *begin1 = 1;
    *end1 = 6596;
    
  }
  else {
     printf("\n\nEnter beginning and end cell numbers separated by a space\n) ");
     printf(" (1 <= cell no. <= 6596):  ");
     scanf("%d %d",begin1,end1);
     while ((*begin1<1) || (*begin1 > 6596) || (*end1 > 6596) || (*end1 < 1) || 
            (*end1<*begin1)) {
       printf(" Incorreect entry - please try again.\n");
       printf("\n\nEnter beginning and end cell numbers (e.g., 1250 3649) ");
       scanf("%d %d",begin1,end1);
     }  
  }
}


/*********************************************************************/
/*  Name - getdata
 *
 * Purpose - This subroutine allows the user to choose from the annotation
 *           labels, which data parameters to dump
 *
 * Input -   hdfname - the number of HDF file
 * Output -  dataitems - list of reference numbers of data to dump 
 *
 * Key Local Parameters - nlabels - number of reference labels
 *                        i,j - counters
 *                        k - index to dataitems
 *                        itemnum - number for user to choose
 *                        startpos - starting position in HDF file
 *                        reflist - list of reference numbers
 *                        labellist - list of annotation labels
 *            
 * Functions called - annotation label routines from HDF
 *********************************************************************/

void getdata(out,hdfname,dataitems)

char *hdfname;
uint16 dataitems[LISTSIZE];
FILE *out;

{
  int32 i,j,k,itemnum,nlabels,iret;
  char labellist[LISTSIZE*MAXLNG];
  uint16 reflist[LISTSIZE];
  char dataunt[25], datafmt[10], coordsys[25];
  int32 dimsize[MAXRANK], numdims;

/*  initialize dataitems array */

  for (i=0; i<LISTSIZE; i++){
     dataitems[i] = 65535;
  }

/*  get annotation labels */

  nlabels  = DFANlablist(hdfname,DFTAG_SD,reflist,labellist,LISTSIZE,
                         MAXLNG,STARTPOS);

  if (nlabels == -1){
    fprintf(out,"Error in getting annotation labels\n\n");
    exit(1);
  }
  
  for (i=0; i<nlabels; ++i){
    if (strcmp(labellist + (i*MAXLNG),"\0") == 0) {
      DFSDgetdims(hdfname,(intn*) &numdims,dimsize,MAXRANK);
      iret = DFSDgetdatastrs(labellist+(i*MAXLNG),dataunt,datafmt,coordsys);
      if(iret == -1)
        {
        }
    }
  }
      
  printf("\n");

  j=1;
  k=0;
  for (i=0; i<nlabels; ++i){
    if (j == 1)
    {
      itemnum = 1;
      printf("Item\tDims\t\tLabel\n\n");
    }
    DFSDgetdims(hdfname,(intn*) &numdims,dimsize,MAXRANK);
    if (strcmp(labellist + (i*MAXLNG),"\0") == 0) {
      iret = DFSDgetdatastrs(labellist+(i*MAXLNG),dataunt,datafmt,coordsys);
    }
    printf(" %2d\t(%dX%d)\t %s\n",i+1,dimsize[0],dimsize[1],labellist+(i*MAXLNG));
    j++;
    if (j>15 || (i == nlabels - 1))
    {
      printf("\n");
      printf("Please choose the numbers of the data items to dump ");
      printf("separated by a space.\n");
      printf("Enter 0 to quit ");
      printf("entering parameters\n\n");
      
      while (itemnum >0)
      {
         scanf("%d",&itemnum);
         if (itemnum == -1)
           break;
         if (itemnum == 0)
         {
           dataitems[k] = 0;
           return;
         }
         dataitems[k] = reflist[itemnum-1];
         k++;
       } /* itemnum > 0 */
       j = 1; 
     }
   } /* end for i<numitems */

   dataitems[k] = 0;
}


/***********************************************************************/
/* Name - readdata
 *                                                                         
 * Purpose - This function determines the data slice to retrieve and using
 *           the reference numbers gets the appropriate data, data        
 *           attributes, max, and min  and then prints the data.          
 *                                                                        
 * Input Parameters - hdfname - name of hdf file to read                  
 *                    dataitems- stores reference numbers of data chosen  
 *                    subset_ans-type of subsetting (cell # or none) 
 *                    begin1- beginning cell number                   
 *                    end1- ending cell number                         
 *                                                                       
 *  Output Parameters - none                                               
 *                                                                         
 * Key Local Parameters - winst - stores starting dimensions of data slice
 *                        windims- stores length of slice                 
 *                        dims - stores dimension of data array
 *                        data - stores data gotten from HDF file               
 *                        datalabel - stores label for data               
 *                        dataunit- stores units for data
 *                        datafmt - stores format for data                
 *                        max,imax - stores maximum value                      
 *                        min,imin - stores minimum value                      
 *                        dataval,idataval - stores data values 
 *                        numdims - number of dimensions of SDS
 *                        dimsize - dimension sizes
 *                        listsize - the max number of parameters
 *                        numtype - HDF number type, 5 floating, 24 -
 *                                  32 bit integer  
 *                                                                        
 * Subroutines Called - HDF routines to read the SDS data slice, max,min  
 *                       data attributes, etc.
 **************************************************************************/          

 void readdata(out,hdfname,dataitems,subset_ans,begin1,end1, begin2, end2)

 char *hdfname,           /* hdf file name */
      *subset_ans;        /* cell number requested */
 int32 begin1, end1, begin2, end2;
 FILE *out;
 uint16 dataitems[LISTSIZE];

{

 char datalabel[260], dataunt[25], datafmt[10], coord_sys[25];
 int32 iret, winst[MAXRANK], winend[MAXRANK], windims[MAXRANK];
 int32 dims[MAXRANK],dimsize[MAXRANK], numdims,numtype,param_6;
 float max, min, datavals[6596*130],fill_value;
 int32 k=0,i,j;
 uint8 * c;
 uint8 lat,lon;
 float32 param_116,param_23,HGT,TS,TC,param_120,param_125;
 int32 LYRTRP,ii,LYSRF,PTROP;


#define UNDEF_INT 255
#define UNDEF_FLOAT -1000.0
#define PATHW 6.292
#define PATHI 10.5
#define RLAPSE 6.5
#define NUM_LAYERS 7


float32 PBOUND[8] = {1000.00, 800.00, 680.00, 560.00, 440.00,
                     310.00, 180.00, 30.00};


float32 TMPTAB[256] = {
         -100.000,165.000,169.000,172.000,175.000,177.800,180.500,
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
float32 TMPVAR[256] = {
      -100.000, 0.075, 0.300,0.600,0.900,1.200,1.500,1.800,2.100,2.400,
         2.700, 3.000, 3.300,3.600,3.900,4.200,4.500,4.800,5.100,5.400,
         5.700, 6.000, 6.300,6.600,6.900,7.200,7.500,7.800,8.100,8.400,
         8.700, 9.000, 9.300, 9.600, 9.900,10.200,10.500,10.800,11.100,
        11.400,11.700,12.000,12.300,12.600,12.900,13.200,13.500,13.800,
        14.100,14.400,14.700,15.000,15.300,15.600,15.900,16.200,16.500,
        16.800,17.100,17.400,17.700,18.000,18.300,18.600,18.900,19.200,
        19.500,19.800,20.100,20.400,20.700,21.000,21.300,21.600,21.900,
        22.200,22.500,22.800,23.100,23.400,23.700,24.000,24.300,24.600,
        24.900,25.200,25.500,25.800,26.100,26.400,26.700,27.000,27.300,
        27.600,27.900,28.200,28.500,28.800,29.100,29.400,29.700,30.000,
        30.300,30.600,30.900,31.200,31.500,31.800,32.100,32.400,32.700,
        33.000,33.300,33.600,33.900,34.200,34.500,34.800,35.100,35.400,
        35.700,36.000,36.300,36.600,36.900,37.200,37.500,37.800,38.100,
        38.400,38.700,39.000,39.300,39.600,39.900,40.200,40.500,40.800,
        41.100,41.400,41.700,42.000,42.300,42.600,42.900,43.200,43.500,
        43.800,44.100,44.400,44.700,45.000,45.300,45.600,45.900,46.200,
        46.500,46.800,47.100,47.400,47.700,48.000,48.300,48.600,48.900,
        49.200,49.500,49.800,50.100,50.400,50.700,51.000,51.300,51.600,
        51.900,52.200,52.500,52.800,53.100,53.400,53.700,54.000,54.300,
        54.600,54.900,55.200,55.500,55.800,56.100,56.400,56.700,57.000,
        57.300,57.600,57.900,58.200,58.500,58.800,59.100,59.400,59.700,
        60.000,60.300,60.600,60.900,61.200,61.500,61.800,62.100,62.400,
        62.700,63.000,63.300,63.600,63.900,64.200,64.500,64.800,65.100,
        65.400,65.700,66.000,66.300,66.600,66.900,67.200,67.500,67.800,
        68.100,68.400,68.700,69.000,69.300,69.600,69.900,70.200,70.500,
        70.800,71.100,71.400,71.700,72.000,72.300,72.600,72.900,73.200,
        73.500,73.800,74.100,74.400,74.700,75.400,78.000,85.000,
        -200.000,-1000.000};
float32 PRETAB[256] = {
       -100.00,  1.00, 5.00, 10.00,15.00,20.00,25.00,30.00,35.00,40.00,
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
        -200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,
        -200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,
        -200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,
        -200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,
        -200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,
        -200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,
        -200.00,-200.00,-200.00,-200.00,-200.00,-1000.00};
float32 RFLTAB[256] = {
       -100.000,0.000,0.008,0.012,0.016,0.020,0.024,0.028,0.032,0.036,
          0.040,0.044,0.048,0.052,0.056,0.060,0.064,0.068,0.072,0.076,
          0.080,0.084,0.088,0.092,0.096,0.100,0.104,0.108,0.112,0.116,
          0.120,0.124,0.128,0.132,0.136,0.140,0.144,0.148,0.152,0.156,
          0.160,0.164,0.168,0.172,0.176,0.180,0.184,0.188,0.192,0.196,
          0.200,0.204,0.208,0.212,0.216,0.220,0.224,0.228,0.232,0.236,
          0.240,0.244,0.248,0.252,0.256,0.260,0.264,0.268,0.272,0.276,
          0.280,0.284,0.288,0.292,0.296,0.300,0.304,0.308,0.312,0.316,
          0.320,0.324,0.328,0.332,0.336,0.340,0.344,0.348,0.352,0.356,
          0.360,0.364,0.368,0.372,0.376,0.380,0.384,0.388,0.392,0.396,
          0.400,0.404,0.408,0.412,0.416,0.420,0.424,0.428,0.432,0.436,
          0.440,0.444,0.448,0.452,0.456,0.460,0.464,0.468,0.472,0.476,
          0.480,0.484,0.488,0.492,0.496,0.500,0.504,0.508,0.512,0.516,
          0.520,0.524,0.528,0.532,0.536,0.540,0.544,0.548,0.552,0.556,
          0.560,0.564,0.568,0.572,0.576,0.580,0.584,0.588,0.592,0.596,
          0.600,0.604,0.608,0.612,0.616,0.620,0.624,0.628,0.632,0.636,
          0.640,0.644,0.648,0.652,0.656,0.660,0.664,0.668,0.672,0.676,
          0.680,0.684,0.688,0.692,0.696,0.700,0.704,0.708,0.712,0.716,
          0.720,0.724,0.728,0.732,0.736,0.740,0.744,0.748,0.752,0.756,
          0.760,0.764,0.768,0.772,0.776,0.780,0.784,0.788,0.792,0.796,
          0.800,0.804,0.808,0.812,0.816,0.820,0.824,0.828,0.832,0.836,
          0.840,0.844,0.848,0.852,0.856,0.860,0.864,0.868,0.872,0.876,
          0.880,0.884,0.888,0.892,0.896,0.900,0.904,0.908,0.912,0.916,
          0.920,0.924,0.928,0.932,0.936,0.940,0.944,0.948,0.952,0.956,
          0.960,0.964,0.968,0.972,0.976,0.980,0.984,0.988,0.992,1.000,
          1.016,1.040,1.072,1.108,-200.000,-1000.000};
float32 TAUTAB[256] = {
       -100.000,0.020,0.040,0.060,0.090,0.110,0.140,0.160,0.190,0.220,
          0.240,0.270,0.300,0.330,0.370,0.400,0.430,0.460,0.500,0.530,
          0.570,0.600,0.640,0.680,0.720,0.750,0.790,0.830,0.870,0.920,
          0.960,1.000,1.040,1.090,1.130,1.180,1.220,1.270,1.320,1.370,
          1.420,1.470,1.520,1.570,1.620,1.670,1.730,1.780,1.830,1.890,
          1.950,2.000,2.060,2.120,2.180,2.240,2.300,2.360,2.430,2.490,
          2.550,2.620,2.690,2.750,2.820,2.890,2.960,3.030,3.100,3.180,
          3.250,3.320,3.400,3.480,3.550,3.630,3.710,3.790,3.880,3.960,
          4.040,4.130,4.220,4.300,4.390,4.480,4.570,4.670,4.760,4.850,
          4.950,5.050,5.150,5.250,5.350,5.450,5.560,5.660,5.770,5.880,
          5.990,6.110,6.220,6.340,6.450,6.570,6.690,6.820,6.940,7.070,
          7.190,7.330,7.460,7.590,7.730,7.870,8.010,8.150,8.300,8.440,
          8.590,8.740,8.900,9.060,9.220,9.380,9.540,9.710,9.880,10.050,
        10.230,10.410,10.590,10.780,10.970,11.160,11.350,11.550,11.760,
        11.960,12.170,12.390,12.600,12.830,13.050,13.280,13.520,13.760,
        14.000,14.250,14.510,14.770,15.030,15.300,15.580,15.860,16.150,
        16.440,16.740,17.050,17.360,17.690,18.020,18.350,18.700,19.050,
        19.410,19.780,20.160,20.540,20.940,21.350,21.770,22.200,22.630,
        23.080,23.550,24.030,24.520,25.020,25.540,26.070,26.620,27.190,
        27.770,28.370,28.990,29.630,30.290,30.970,31.670,32.400,33.160,
        33.940,34.740,35.580,36.450,37.350,38.290,39.260,40.260,41.320,
        42.420,43.570,44.760,46.000,47.310,48.680,50.110,51.600,53.170,
        54.840,56.590,58.430,60.360,62.400,64.590,66.900,69.360,71.960,
        74.720,77.730,80.940,84.380,88.060,92.020,96.400,101.010,
        105.510,109.870,114.330,119.590,125.920,133.660,143.120,
        154.650, 169.560, 187.490, 207.200, 228.130, 250.440, 282.780,
        323.920, 378.650,-200.000,-200.000,-200.000,-200.000,-200.000,
       -200.000,-200.000,-200.000,-200.000,-200.000,-200.000,
       -1000.000};
float32 PRWTAB[256] = {
       -100.000,0.000,0.030,0.060,0.090,0.120,0.150,0.180,0.210,0.240,
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
float32 OZNTAB[256] = {
       -100.0,0.0,2.0,4.0,6.0,8.0,10.0,12.0,14.0,16.0,18.0,20.0,22.0,
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

int32 NCELLS[72] = {
           0,   3,  12,  28,  50,  78, 112, 152, 198, 250,
         308, 372, 441, 516, 596, 681, 771, 866, 966,1070,
        1178,1290,1406,1526,1649,1775,1904,2036,2170,2306,
        2444,2584,2725,2867,3010,3154,3298,3442,3586,3729,
        3871,4012,4152,4290,4426,4560,4692,4821,4947,5070,
        5190,5306,5418,5526,5630,5730,5825,5915,6000,6080,
        6155,6224,6288,6346,6398,6444,6484,6518,6546,6568,
        6584,6593};

int32 ICELLS[72] = {
           3,   9,  16,  22,  28,  34,  40,  46,  52,  58,
          64,  69,  75,  80,  85,  90,  95, 100, 104, 108,
         112, 116, 120, 123, 126, 129, 132, 134, 136, 138,
         140, 141, 142, 143, 144, 144, 144, 144, 143, 142,
         141, 140, 138, 136, 134, 132, 129, 126, 123, 120,
         116, 112, 108, 104, 100,  95,  90,  85,  80,  75,
          69,  64,  58,  52,  46,  40,  34,  28,  22,  16,
           9,   3};


 /*  set up slice of data */
/*begin2 = 1;*/
/*  end2 = 130;*/

  winst[1] = begin2;
  winst[0] = begin1;
  winend[1] = end2;
  winend[0] = end1;
  windims[1] = (end2- begin2 + 1);
  windims[0] = (end1-begin1 + 1);
  dims[0] = windims[0];
  dims[1] = windims[1];
  if(NCELLS[0] == 0)
    {
    }
 /* get fill value - currently only in ref #2 - but intended to
    apply to all data in file */
 iret = DFSDreadref(hdfname,(uint16) 2);
 iret = DFSDgetdims(hdfname,(intn*) &numdims,dimsize,MAXRANK);
 iret = DFSDgetfillvalue((VOIDP) &fill_value);
 if (iret == -1) {
   printf("FAILED TO GET FILL VALUE - will use 999.989990\n");
   fill_value = 999.989990;
 }

 /*  while we have data items to get */
 k=0;
 while (dataitems[k] != 0)
 {

   /*  go to desired data reference */

   iret = DFSDreadref(hdfname,dataitems[k]);
   if (iret == -1)
   {
     fprintf(out,"Error on going to specified reference number %d\n\n",
            dataitems[k]);
     exit(1);
    }

   /* get data dimensions */

    iret = DFSDgetdims(hdfname,(intn*) &numdims,dimsize,MAXRANK);

    if (iret !=0)
    {
      fprintf(out,"Error on getting data dimensions\n\n");
      exit(1);
    }
    fprintf(out,"Original data dimensions are %d by %d\n\n",dimsize[0],dimsize[1]);

    /*  get number type */

    iret = DFSDgetNT(&numtype);
    if (iret != 0) {
      fprintf(out,"Error on getting number type\n\n");
      exit(1);
    }

    /*  get data attributes */

    iret = DFSDgetdatastrs(datalabel,dataunt,datafmt,coord_sys);
    if (iret != 0) {
      fprintf(out,"Error on getting data attributes\n\n");
      exit(1);
    }
    fprintf(out,"Data attributes are as follows:\n\n");
    fprintf(out,"Label            : %s\n\n",datalabel);
    fprintf(out,"Units            : %s\n",dataunt);
    fprintf(out,"Format           : %s\n",datafmt);
    fprintf(out,"Fill Value       : %f\n\n",fill_value);
    fprintf(out,"Coordinate System: %s\n\n",coord_sys);

    iret = DFSDgetrange((VOIDP) &max,(VOIDP) &min);
    if (iret == -1) {
      fprintf(out," No min/max values set\n\n");
    }
    else
    {
      fprintf(out,"Data max: %f\n",max);
      fprintf(out,"Data min: %f\n\n",min);
    }

    /*  get data slice */

  
    /*iret = DFSDgetslice(hdfname,winst,windims,(VOIDP) datavals,dims);*/
    iret = DFSDreadslab(hdfname,winst,windims,winst,(VOIDP) datavals,dims);
    if (iret == -1) {
      fprintf(out,"Error on getting data slice\n\n");
      exit(1);
    }
    fprintf(out,
       "Data values for each ISCCP cell from %d to %d \n",winst[0],winend[0]);
    fprintf(out, "for variables %d to %d are:\n\n", winst[1],winend[1]);
    c = (uint8 *)&datavals[0];

    /* print data */
    for (i=winst[0]; i<=winend[0]; i++){
      fprintf(out,"********** CELL NO. %d **********\n\n",i);
      fprintf(out,"\tCOUNT VALUES FOR EACH VARIABLE:\n");
      fprintf(out,"( 1)");
      for (j=winst[1]; j<=winend[1]; j++){
        fprintf(out,"%8d",*(c+(j-1)));
        if ((j%10) == 0){
          fprintf(out,"\n(%3d)",j+1);
        }
      }
   
      /* end of integer counts - now go back and generate physical values */

      fprintf(out,"\n\n\tPHYSICAL VALUES:\n");
      fprintf(out,"( 1)");
      for (j=winst[1]; j<=winend[1]; j++){/* for each variable 130 */
        switch (j) {
       /*****************************************************************
         convert integers to real values with conversion tables and
         multipliers copied from data providers FORTRAN Program
        *****************************************************************/
        case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 119:
        
               if(param_6 == 0)
                 {
                 }
               if ( j == 1) { lat = *(c+(j-1));}
               if ( j == 2) { lon = *(c+(j-1));}
               if ( j == 6) { param_6 = *(c+(j-1));}
               if (*(c+(j-1)) == UNDEF_INT){
                  fprintf (out,"%8.2f",UNDEF_FLOAT);
                 }
               else
                 {
                   fprintf(out,"%8.2f",(float32)*(c+(j-1)));
                 }
               break;
         case 8: case 9: case 10:
        case 11: case 12: case 13: case 14: case 15: case 16: case 17:
        case 18: case 19:       
              
               if (*(c+(j-1)) == UNDEF_INT){
                  fprintf(out,"%8.2f",UNDEF_FLOAT); 
                 }
               else
                 {
                   fprintf(out,"%8.2f",(((float32)*(c+(j-1))) *0.5));
                 }
               break;
          case 20: case 21: case 22:
                 { 
                  fprintf(out,"%8.2f",PRETAB[*(c+(j-1))]);
                 }
                  break;
          case 23: case 116: case 121: case 122: case 123: case 124:
          case 126: case 127:

                   if ( j == 23)
                      { 
                       param_23 = TMPTAB[*(c+(j-1))];
                      }
                   if ( j == 116 )
                      {
                        param_116 = TMPTAB[*(c+(j-1))];
                      }
                 {
                   fprintf(out,"%8.2f",TMPTAB[*(c+(j-1))]);
                 }
                 break;
           case 24: case 25: case 117:
                 {
                   fprintf(out,"%8.2f",TMPVAR[*(c+(j-1))]);
                 }
                 break;
           case 26: case 27: case 28: case 29: case 30: case 31:
                 {
                   if ( j== 29 || j == 30 || j == 31)
                    if (*(c+(j-1)) == UNDEF_INT)
                       {
                        fprintf (out,"%8.2f",UNDEF_FLOAT);
                       }
                    else
                      {
                       fprintf(out,"%8.2f",TAUTAB[*(c+(j-1))] * PATHW );
                      }
                    else
                     { 
                      fprintf(out,"%8.2f",TAUTAB[*(c+(j-1))]);
                     }
                 }
                 break; 
            case 32: case 35: case 38:
                {
                  if (*(c+(j-1)) == UNDEF_INT)
                       {
                        fprintf (out,"%8.2f",UNDEF_FLOAT);
                       }
                  else
                      {
                        fprintf(out,"%8.2f",(((float32)*(c+(j-1))) *0.5));
                        
                      }
                }
                  break;  
             case 33: case 36: case 39:
                 /*if (PRETAB[*(c+(j-1))] >= (float32)0.0)*/
                       {
                         fprintf(out,"%8.2f",PRETAB[*(c+(j-1))]);
                       }
                       
                  break;
              case 34: case 37: case 40:
                      {
                        fprintf(out,"%8.2f",TMPTAB[*(c+(j-1))]);
                      }
                  break; 
              case 41: case 46: case 51: case 56: case 61: case 66:
              case 71: case 76: case 81: case 86: case 91: case 96:
              case 101: case 106: case 111:
                   {
                      if (*(c+(j-1)) == UNDEF_INT)
                       {
                        fprintf (out,"%8.2f",UNDEF_FLOAT);
                       }
                  else
                      {
                        fprintf(out,"%8.2f",(((float32)*(c+(j-1))) *0.5));
 
                      }
                }
                  break;
              case 42: case 47: case 52: case 57: case 62: case 67:
              case 72: case 77: case 82: case 87: case 92: case 97: 
              case 102: case 107: case 112: case 120: case 125:
                       if ( j == 120)
                         {
                           param_120 = PRETAB[*(c+(j-1))];
                         }
                       if ( j == 125)
                         {
                           param_125 = PRETAB[*(c+(j-1))];
                         }
                       {
                         fprintf(out,"%8.2f",PRETAB[*(c+(j-1))]);
                       }
                  break;
              case 43: case 48: case 53: case 58: case 63: case 68:
              case 73: case 78: case 83: case 88: case 93: case 98:
              case 103: case 108: case 113:
                      {
                        fprintf(out,"%8.2f",TMPTAB[*(c+(j-1))]);
                      }
                  break;
              case 44: case 49: case 54: case 59: case 64: case 69:
              case 74: case 79: case 84: case 89: case 94: case 99:
              case 104: case 109: case 114:
                      {
                         fprintf(out,"%8.2f",TAUTAB[*(c+(j-1))]);
                      }
                break;
	      
              case 45: case 50: case 55: 
              case 75: case 80: case 85: 
                     if (TAUTAB[*(c+(j-1))] >= (float32)0.0)
                       { 
                      /*  CONDITION CHANGED TO ONLY APPLY TO LIQUID -CHANGED 12/99*/
                        
                          
                           fprintf(out,"%8.2f",(TAUTAB[*(c+(j-1))] * PATHW));
                       }
		     else{ 
                          fprintf(out,"%8.2f",(TAUTAB[*(c+(j-1))]));
		     }
                      break;   

		case 60: case 65: case 70:case 90: case 95: case 100:
              case 105: case 110: case 115: 
                      if (TAUTAB[*(c+(j-1))] >= (float32)0.0)
                       
                          {
                           fprintf(out,"%8.2f",(TAUTAB[*(c+(j-1))] * PATHI));
                          }
		      else{ 
                          fprintf(out,"%8.2f",(TAUTAB[*(c+(j-1))]));
		     }
		    
                break;

              case 118:
                       {
                         fprintf(out,"%8.2f",RFLTAB[*(c+(j-1))]);
                       }
               break;
              case 128: case 129:
                       {
                         fprintf(out,"%8.2f",PRWTAB[*(c+(j-1))]);
                       }
               break;
              case 130:
                       {
                         fprintf(out,"%8.2f",OZNTAB[*(c+(j-1))]);
                       }
               break;
                         
          } /* with switch j*/
          if ((j%10) == 0){
             fprintf(out,"\n(%3d)",j+1);
            }
         } /*end of for j loop */
           fprintf(out,"\n\n");
           c += winend[1]; /* move buffer pointer to start of next cell data */

           /* print out cell unique info as detailed below */
           fprintf(out," CENTER:  CENTER LON/LAT   %5.2f  %5.2f\n\n",
              (float32)(((float32)lon-(float32)1) *
              ((float32)360.0/(float32)ICELLS[lat-1]) +
              (((float32)360.0/(float32)ICELLS[lat-1])/(float32)2.0)),
              (float32)((((float32)lat-(float32)1)*(float32)2.5) +
              ((float32)2.5/ (float32)2.0) - (float32)90.0));

/***************************************************************************
* PRESSURE LAYER MIDPOINT CALCULATION AS DERIVED FROM FORTRAN CODE AS      *
* PROVIDED BY DATA PRODUCER                                                *
***************************************************************************/

         PTROP = param_125;
         if(PTROP == 0)
            {
            }
         LYSRF = 0;
         LYRTRP = NUM_LAYERS;

         for ( ii = 1; ii <= NUM_LAYERS; ii++)
            {
               if (param_120 <= PBOUND[ii])
                 {
                    LYSRF = ii;
                 }
               if (param_125 > PBOUND[ii])
                 {
                    LYRTRP = ii - 1;
                 }
            } /* end of for statement */ 
           fprintf(out," MIDPRS:  ACTUAL PRESSURE LAYER MID-POINTS (MB)");
    
         for ( ii = 0; ii < NUM_LAYERS; ii++)
            {
              if (( ii < LYSRF ) || ( ii > LYRTRP))
                {
                  fprintf(out,"%8.2f",UNDEF_FLOAT);
                }
              else
                {
                  if ( ii == LYSRF)
                    {
                      fprintf(out,"%8.2f",((param_120 + PBOUND[ii+1]) * (float32) 0.5));
                    }        
                  else
                     {
                        if ( ii == LYRTRP)
                          {
                            fprintf(out,"%8.2f",((param_125 + PBOUND[ii]) * (float32) 0.5));
                          }
                      else
                         {
                           fprintf(out,"%8.2f",
                             ((PBOUND[ii] + PBOUND[ii+1]) * (float32)0.5));
                         } /* end of last else */
                     } /* end of second last else */
                } /* end of else first */
           } /* end of for loop */
         fprintf (out,"\n\n");

/*************************************************************************
* CLOUD TOP HEIGHT CALCULATED AS CODE PROVIDED BY DATA PRODUCER          *
*************************************************************************/


           /* print out cloud height */
           TS = param_116;
           TC = param_23;
           HGT = ( TS - TC)/ RLAPSE * 1000.0;
           fprintf(out," CLDHGT:  CLOUD TOP HEIGHT (M) %9.0f\n\n",HGT);
    }
    k++;
  }  /*  while (dataitems[k] != 0) */
}  /* end readdata */

/*****************************************************************************/
