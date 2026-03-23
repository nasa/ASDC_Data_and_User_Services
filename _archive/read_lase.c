/************************************************************************
*                                                                       *
* PROGRAM:   read_lase.c                                         	*
*                                                                       *
* VERSION:   2.0                       DATE:  17 February 2000          *
*                                                                       *
* PURPOSE:   The purpose of this code is to demonstrate reading         *
*            LASE data files.  It is intended as a starting point       *
*            from which a program can be developed to process LASE      *
*            data.  In its distributed form, it reads in the file       *
*            header and profile data and writes it to the screen.       * 
*                                                                       *
* ALGORITHM: None.                                                      *
*                                                                       *
* INVOCATION:    read_lase <datafilename>                               *
*      WHERE:                                                           *
*          <datafilename> - optional                                    *
*                           name of (including path if not in current   *
*                             directory) LASE file to be read           *
*                                                                       *
*                                                                       *
* EXTERNAL ROUTINES:                                                    *
*            #include <stdio.h>                                         *
*            #include <stdlib.h>                                        *
*            #include <string.h>                                        *
*                                                                       *
* INTERNAL ROUTINES:  None.                                             *
*                                                                       *
* NOTES:   None.							*
*									*
* MODIFICATION HISTORY:							*
* 2/17/2000 -	Modified to work with new format for LASE_CAMEX3        *
*		data set, including change to floating point for        *
*		lat/lon and profile values; with these changes,		*
*		program will still work with files in original LASE	*
*		format							*
*									*                                                                       *
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATBUF 600
#define MAXBUF 80
#define MAX_FILENAME 51
#define MAX_NUM_VAR 9

void banner(int x);

main (argc,argv)
 int argc;
 char *argv[];

{
 char filename[MAX_FILENAME],
      dataname[30],
      investigator[60],
      species[60],
      mission[30],
      temp[MAXBUF],
      varname[MAX_NUM_VAR][30],
      units[9][30],
      contkey;
 
 int i,
     line_count,
     num_hdr_lines,
     syr,
     smon,
     sday,
     ryr,
     rmon,
     rday,
     flight_num,
     num_var,
     num_comment_lines,
     data_type,
     avg_period,
     freq,
     jday,
     time,
     alt,
     balt,
     alt_inc,
     num_pts,
     pcount;

 float scale[MAX_NUM_VAR],     
       offset[MAX_NUM_VAR],
       min[MAX_NUM_VAR],
       max[MAX_NUM_VAR],
       missing[MAX_NUM_VAR],
       lod[MAX_NUM_VAR],
       lat,
       lon,
       profile[DATBUF];
  
 FILE  *f1,
       *fout; 

  banner(argc);
  if (argc ==1) 
  {
    printf("\nPlease enter the LASE data file name\n\n"); 
    gets(filename);
  }
  else
    strcpy(filename,argv[1]);
   
   
  if ((f1 = fopen(filename,"r")) == NULL)
  {  
     printf("Can't open LASE data file %s\n",filename);
     exit(-1);
  }
  
  line_count = 0;
  fscanf(f1,"%d", &num_hdr_lines);
  fgets(temp,79,f1);	  /*  read to end of previous line  */
  line_count +=1;
  fgets(dataname,29,f1);  /*  original file name  */
  line_count +=1;
  fgets(investigator,59,f1);
  printf("\nInvestigator: %s",investigator);
  line_count +=1;
  fgets(species,59,f1);
  printf("Data: %s",species);
  line_count +=1;
  fgets(mission,29,f1);
  printf("Mission: %s",mission);
  line_count +=1;
   
  fscanf(f1,"%d %d %d %d %d %d",&syr,&smon,&sday,&ryr,&rmon,&rday);
  printf("Start date: %d/%d/%d \n",smon,sday,syr);
  printf("Date processed: %d/%d/%d \n",rmon,rday,ryr);
  line_count +=1;
   
  fscanf(f1,"%d",&flight_num); 
  line_count +=1;
  printf("Flight Number: %d\n",flight_num);
  fscanf(f1,"%d",&num_var);
  line_count +=1;
  fscanf(f1,"%d",&num_comment_lines);
  line_count +=1;
  fscanf(f1,"%d",&data_type);
  line_count +=1;
  fscanf(f1,"%d",&avg_period);
  line_count +=1;
  fscanf(f1,"%d",&freq);
  line_count +=1;
  printf("\nNum  Variable                 Units       Scale    Offset  Min     Max   Missing\n"); 
  printf("--------------------------------------------------------------------------------\n");
  for (i=0;i<num_var;i++)
  {
    fgets(temp,79,f1);  /*  read to end of previous line  */
    fscanf(f1,"%[^,],%[^,],%f,%f,%f,%f,%f,%f",varname[i],units[i],
      &scale[i],&offset[i],&min[i],&max[i],&missing[i],&lod[i]);
    line_count += 1;
    printf("%2d)  %-22s %-12s %6.2f %8.2f %9.2f %9.2f %9.2f \n",
      i+1,varname[i],units[i],scale[i],offset[i],min[i],max[i],missing[i]); 
  }
   
/*  print the comment lines from the header file  */
    
  for (i=line_count; i<= num_hdr_lines; i++) 
  {
    fgets((char *) &temp[0],MAXBUF,f1);
    printf("%s\n",temp);
  }
  
  printf("This program as delivered will write all the profiles\n");
  printf("to the screen until the end-of-file is reached.\n");
  printf("\nTo interrupt the program use CTRL C (press the C key\n");
  printf("while holding down the Ctrl or Control key).\n");
  printf("\nPress Return or Enter to continue\n");
  scanf("%c",&contkey);

/*  Read in the data and write it to the screen
*** Note that latitude, longitude and profile values written to the screen
*** are multiplied by their corresponding scale value to convert them 
*** from the scaled integer value stored in the file to their actual value.
*/

  pcount = 0;
  
  while (feof(f1)==0)
  {
    fscanf(f1,"%d,%d,%d,%d,%d,%f,%f,%d",
      &jday,&time,&alt,&balt,&alt_inc,&lat,&lon,&num_pts);
    pcount +=1;
    printf("\nProfile %d:\n",pcount);
    printf("Day: %d   Time: %d   Lat: %f   Lon: %f \n",
      jday,time,lat*scale[5],lon*scale[6]);
    printf("Altitude info--Aircraft: %d @Begn:  %d  Increment: %d\n",
      alt,balt,alt_inc);
    printf("Total Scattering Ratio Profile (%3d points):\n",num_pts);
    for (i=0;i<num_pts;i++) fscanf(f1,"%f,",&profile[i]);
    for (i=0;i<num_pts;i++) 
    { 
      if (i%6 == 0) printf("\n");  /* print six profile values per line  */ 
      printf("%7.2f ",profile[i]*scale[8]);
      
    }
    printf("\n");
  }
}
void banner(argc)
{
  char contkey;

  printf("**********************************************************\n\n");
  printf("                LASE READ PROGRAM \n\n");
  printf("  Version 2.0                               17 FEB 2000 \n\n");
  printf("  This program reads the LASE data in the GTE Data Archive \n");
  printf("  Format. The input file name is built according to the  \n");
  printf("  naming convention formed by the Langley DAAC.  Currently \n");
  printf("  the input file name is expected to be no more than 50 characters \n");
  printf("  in length. If you have changed the name of the \n");
  printf("  input file to greater than 50 characters, you will need to \n");
  printf("  modify this program, changing the value of the MAX_FILENAME \n");
  printf("  parameter in the #define statement near the beginning of the file\n");
  printf("  to be at least the maximum length needed + 1.\n\n");
  printf("  All data files are assumed to be in the current working directory. \n");
  printf("  If the files are located in another directory, please be sure \n");
  printf("  to include the complete path with the file name.\n");
  printf("\n\n Platforms tested:    SUN (Solaris 2.5 and SunOS 4.1.3) \n");
  printf("                      HP (HP-UX 10.10) \n");
  printf("                      SGI (IRIX 6.4) \n");
  printf("                      Dec Alpha (Digital UNIX V4.0A)\n");
  printf(" \n\n  Contact: Langley DAAC User and Data Services Office \n");
  printf("           NASA Langley Research Center \n");
  printf("           Mail Stop 157D \n");
  printf("           Hampton, VA 23681-2199 \n");
  printf("           Phone (757)864-8656   Fax (757)864-8807 \n");
  printf("           e-mail:  support-asdc@earthdata.nasa.gov \n");
  printf("           http://eosweb.larc.nasa.gov \n");
  printf("**********************************************************\n");
  printf("\n\n Press Return or Enter to continue\n");
  scanf("%c",&contkey);
}
