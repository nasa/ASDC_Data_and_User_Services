/************************************************************************
*                                                                       *
* PROGRAM: read_lase_gh.c                                               *
*                                                                       *
* VERSION  1.0                      DATE: 20 October 2000               * 
*                                                                       *
* PURPOSE: The purpose of this code is to demonstrate reading           *
*          LASE SOLVE/AFWEX data files.  It is intended as a            *
*          starting point from which a program can be developed         *
*          to process LASE data.  In it's distributed form, it          *
*          reads in the file header and profile data and writes it      *
*          to the screen.                                               *
*                                                                       *
* ALGORITHM: None.                                                      *
*                                                                       *
* INVOCATION: read_lase_gc <datafilename>                               *
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
* MODIFICATION HISTORY:                                                 *
* 10/22/2002 - Changed the name from read_lase_solve.c                  *
*              to read_lase_gh.c to be more general.                    *
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILENAME 51
FILE  *f1;
main (int argc,char *argv[])
{
 int NV,
     flg, 
     NAUXV;

 char filename[MAX_FILENAME],
      ANAME[9][80],
      VNAME[80];

 float  ASCAL[20],
        VSCAL,
        VMISS;

banner(&flg);
  if (argc == 1)
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
  read_header(f1,&NV,&NAUXV,ASCAL,&VSCAL,&VMISS,ANAME,VNAME);
  read_data_records(f1,&NV,&NAUXV,ASCAL,&VSCAL,&VMISS,ANAME,VNAME);
}
banner(flg)
int *flg;
{
  char contkey;

  printf("**********************************************************\n\n");
  printf("                LASE_SOLVE READ PROGRAM \n\n");
  printf("  Version 1.0                               20 DEC 2000 \n\n");
  printf("  This program reads the LASE SOLVE/AFWEX data in the Gaines-Hipskind\n");
  printf("  formatting convention.The input file name is built according to the\n");
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
  printf("  The program also accepts input by way of the link command:\n");
  printf("  example\n");
  printf("  ln -s /fs1/LASE/SOLVE/archive_data/lase_solve_aer_nadir_19991130 LASE_LINK\n");
  printf("  This command will make a soft link to the file where it is located.\n");
  printf("  When the program needs a filename as input give it the LASE_LINK name.\n");
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
  *flg=1;
  return(0);
}
read_header(f1,NV,NAUXV,ASCAL,VSCAL,VMISS,ANAME,VNAME)
FILE *f1;
int *NV,
    *NAUXV;
float ASCAL[20],
      *VSCAL,
      *VMISS;
char ANAME[9][80],
     VNAME[80];
{
  int i,
      nv,
      nauxv,
      NLHEAD,
      FFI,
      IVOL,
      NVOL,
      NSCOML,
      NNCOML,
      AMISS[20],
      syr,
      smon,
      sday,
      ryr,
      rmon,
      rday;
  float DX,
        CAL,
        vscal,
        vmiss;
  char temp[80],
       ONAME[60],
       ORG[80],
       SNAME[80],
       MNAME[80],
       XNAMES[120],
       vname[80],
       aname[80],
       NCOM[120];
  fscanf(f1,"%d %d", &NLHEAD,&FFI);
  fgets(temp,79,f1);
  fgets(ONAME,sizeof(ONAME),f1);
  printf("\n\n\nInvestigator: %s",ONAME);
  fgets(ORG,sizeof(ORG),f1);
  printf("Originators: %s",ORG);
  fgets(SNAME,sizeof(SNAME),f1);
  printf("Source of measurments: %s",SNAME);
  fgets(MNAME,sizeof(MNAME),f1);
  printf("Mission: %s",MNAME);
  fscanf(f1,"%d %d",&IVOL,&NVOL);
  fscanf(f1,"%d %d %d %d %d %d",&syr,&smon,&sday,&ryr,&rmon,&rday);
  printf("Start date: %d/%d/%d \n",smon,sday,syr);
  printf("Date processed: %d/%d/%d \n",rmon,rday,ryr);
  fscanf(f1,"%f",&DX);
  fgets(temp,79,f1);
  printf("\nIndependent variables descriptions:\n");
  printf("------------------------------------------------------------\n");
  for (i=0;i<2;i++)
    {
      fgets(XNAMES,sizeof(XNAMES),f1);
      printf("%s",XNAMES);
    }
  printf("------------------------------------------------------------\n");
  fscanf(f1,"%d", &nv);
  *NV = nv;
  fscanf(f1,"%f",&vscal);
  *VSCAL = vscal;
  fscanf(f1,"%f",&vmiss);
  *VMISS = vmiss;
  fgets(temp,79,f1);
  fgets(vname,sizeof(vname),f1);
  (void)strcpy(VNAME,vname);
  fscanf(f1,"%d",&nauxv);
  *NAUXV = nauxv;
  fgets(temp,79,f1);
  for (i=0;i<*NAUXV;i++)
    {
     fscanf(f1,"%f",&CAL);
     ASCAL[i] = CAL;
    }
  for (i=0;i<*NAUXV;i++)
    {
     fscanf(f1,"%d",&AMISS[i]);
    }
  fgets(temp,79,f1);
  for (i=0;i<*NAUXV;i++)
    {
      fgets(aname,sizeof(aname),f1);
      (void)strcpy(ANAME[i],aname);
    }
  printf("\n");
  printf("Special comment lines:\n");
  fscanf(f1,"%d",&NSCOML);
  fgets(temp,79,f1);
  fscanf(f1,"%d",&NNCOML);
  fgets(temp,79,f1);
  for (i=0;i<NNCOML;i++)
    {
      fgets(NCOM,sizeof(NCOM),f1);
      printf("%s",NCOM);
    }
  printf("\n");
  return(0);
}
read_data_records(f1,NV,NAUXV,ASCAL,VSCAL,VMISS,ANAME,VNAME)
FILE *f1;
int *NV,
    *NAUXV;
float ASCAL[20],
      *VSCAL,
      *VMISS;
char ANAME[9][80],
     VNAME[80];
{
  int val,
      i=0,
      j=0,
      k=0,
      l=0,
      ihead=0,
      nvv,
      NX=0;
  float x,
       vscal,
       vmiss;
  char contkey,
       cname[80];
  headers(&contkey,&ihead,&NX,&NV);
  while (1)
    {
      read_values(f1,&val);
      printf("%4.3f",*VSCAL);
      printf(" <--Scale factor by which one multiplies recorded\n");
      printf("      values of the primary variables to convert them\n");
      printf("      to units specified in VNAME(n).\n");
      printf("\nVNAME(n) <-- ");
      printf("%s",VNAME);
      printf("\n");
      printf("%d  ",val);
      printf("<--The Bounded Independent variable.\n");
      read_values(f1,&val);
      printf("%d  ",val);
      NX = val;
      if((NX < 0) || (NX > 9999))
	{
	  for(i=3;i<=*NAUXV+1;i++)
	    {
              read_values(f1,&val);  /*throw away overscale data records.*/
	      printf("%d  ",val);
	    }
          continue;
	}
      read_values(f1,&val);
      printf("%d  ",val);
      read_values(f1,&val);
      printf("%d  ",val);
      ihead++;
      printf("<--The first three auxiliary variables are:\n");
      strcpy(cname,ANAME[0]);
      cname[strlen(cname)-1] = '\0';
      printf("%s\n",cname);
      strcpy(cname,ANAME[1]);
      cname[strlen(cname)-1] = '\0';
      printf("%s\n",cname);
      strcpy(cname,ANAME[2]);
      cname[strlen(cname)-1] = '\0';
      printf("%s\n",cname);
      headers(&contkey,&ihead,&NX,&NV);
      printf("            auxiliary variables\n");
      printf("            -------------------\n");
      for (j=3;j<=*NAUXV;j++)
        {
          switch (j+1)
            {
              case 3:
                     read_values(f1,&val);
                     x = ASCAL[j] * val;
		     strcpy(cname,ANAME[j]);
		     cname[strlen(cname)-1] = '\0';
		     l = strlen(cname);
		     l= 40 -l;
                     printf("%s:",cname);
                     for (k=0;k<l;k++)
                       {
		         printf(" ");
                       }
                     printf("%8.2f\n",x);
                     break;
              case 4:
                     read_values(f1,&val);
                     x = ASCAL[j] * val;
		     strcpy(cname,ANAME[j]);
		     cname[strlen(cname)-1] = '\0';
		     l = strlen(cname);
		     l= 40 -l;
                     printf("%s:",cname);
                     for (k=0;k<l;k++)
                       {
		         printf(" ");
                       }
                     printf("%8.2f\n",x);
                     break;
              case 5:
                     read_values(f1,&val);
                     x = ASCAL[j] * val;
		     strcpy(cname,ANAME[j]);
		     cname[strlen(cname)-1] = '\0';
		     l = strlen(cname);
		     l= 40 -l;
                     printf("%s:",cname);
                     for (k=0;k<l;k++)
                       {
		         printf(" ");
                       }
                     printf("%8.2f\n",x);
                     break;
              case 6:
                     read_values(f1,&val);
                     x = ASCAL[j] * val;
		     strcpy(cname,ANAME[j]);
		     cname[strlen(cname)-1] = '\0';
		     l = strlen(cname);
		     l= 40 -l;
                     printf("%s:",cname);
                     for (k=0;k<l;k++)
                       {
		         printf(" ");
                       }
                     printf("%8.2f\n",x);
                     break;
              case 7:
                     read_values(f1,&val);
                     x = ASCAL[j] * val;
		     strcpy(cname,ANAME[j]);
		     cname[strlen(cname)-1] = '\0';
		     l = strlen(cname);
		     l= 40 -l;
                     printf("%s:",cname);
                     for (k=0;k<l;k++)
                       {
		         printf(" ");
                       }
                     printf("%8.2f\n",x);
                     break;
              case 8:
                     read_values(f1,&val);
                     x = ASCAL[j] * val;
		     strcpy(cname,ANAME[j]);
		     cname[strlen(cname)-1] = '\0';
		     l = strlen(cname);
		     l= 40 -l;
                     printf("%s:",cname);
                     for (k=0;k<l;k++)
                       {
		         printf(" ");
                       }
                     printf("%8.2f\n",x);
                     break;
              case 9:
                     read_values(f1,&val);
                     x = ASCAL[j] * val;
		     strcpy(cname,ANAME[j]);
		     cname[strlen(cname)-1] = '\0';
		     l = strlen(cname);
		     l= 40 -l;
                     printf("%s:",cname);
                     for (k=0;k<l;k++)
                       {
		         printf(" ");
                       }
                     printf("%8.2f\n",x);
                     break;
              default:
                     break;
                 
            }
        }
      ihead++;
      nvv = *NV;
      headers(&contkey,&ihead,&NX,&nvv);
      ihead=0;
      nvv = *NV;
      vscal = *VSCAL;
      vmiss = *VMISS;
      process_records(f1,&NX,&nvv,&val,&vscal,&vmiss);
    }
}
headers(contkey,ihead,NX,NV)
char contkey;
int *ihead,
    *NX,
    *NV;
{
  if(*ihead == 0)
    {
      char ckey;
      printf("------------------------------------------------------------\n");
      printf("This program as delivered will write all the data values\n");
      printf("to the screen until the end-of-file is reached.\n\n");
      printf("The program will display a screen of values and then\n");
      printf("prompt you for a response. To skip the prompt enter\n");
      printf("a <y> for yes when prompted.\n\n");
      printf("Then if you wish to interrupt the program use CTRL C.\n");
      printf("(Press the C key while holding down the Ctrl or Control key).\n");
      printf("\nPress Return or Enter to continue.\n");
      printf("-------------------------------------------------------------\n");
      scanf("%c",&ckey);
      contkey=ckey;
    }
  else
    {
      if(*ihead == 1)
        {
          printf("\nThe Bounded Independent variable:\n");
          printf("[Has its constant values defined in the file header.]\n");
          printf("\nX(i,1), i=1, NX(1)   NX = %d\n",*NX); 
          printf("-----------------------------------------------------\n");
        }
    }
  if(*ihead == 2)
    {
      printf("\nThe Unbounded Independent variable:\n");
      printf("[The index m is always used to count the independent]\n");
      printf("[variable marks. The implied loop over m is unbounded.]\n");
      printf("\nV(i,m,n), i=1,NX n=1,NV      NX = %d NV = %d\n",*NX,*NV);
      printf("----------------------------------------------------------------------------\n");
    }
return(0);
}
void get_info(iskip)
int *iskip;
{
  char contkey;
  if(*iskip == 0)
    {
      printf("--------------------------------------\n");
      printf("Please enter a <cr> to continue.\n");
      printf("If you wish to end having this message\n");
      printf("shown to you enter 'y' for yes.\n");
      printf("--------------------------------------\n");
      scanf("%c",&contkey);
      if(contkey == 'y')
        {
          (*iskip)++;
        }
    }
}
read_values(f1,val)
FILE *f1;
int *val;
{
  int value,
      rec=0;
  rec = fscanf(f1,"%d",&value);
  if(rec < 0)
    {
      printf("\nProgram terminated normally!\n\n");
      exit(0);
    }
  *val=value;
return(0);
}
process_records(f1,NX,NV,val,VSCAL,VMISS)
FILE *f1;
int *NX,
    *NV,
    *val;
float *VSCAL,
      *VMISS;
{
  int i=0,
      m=0,
      n=0,
      value=0;
  static int iskip=0;
  int cnt=0;
  float data;
  char ans;
  for (i=1;i<=*NX;i++)         /*bounded independent variable*/
    {
      for (m=1;m<=*NV;m++)     /*unbounded independent variable*/
        {
          for (n=1;n<=*NV;n++) /*primary variable*/
            {
              (void) read_values(f1,&value);
              cnt++;
              *val = value;
              if(*val < *VMISS)
                {
                  data = (*val) * (*VSCAL);
                  printf("%10.4lf ",data);
                }
              else
                {
                  printf("  Overscale");
                }
              if(cnt > 6)
                {
                  cnt=0;
                  printf("\n");
                }
            }
        }
    }
  printf("\n\n");
  if(iskip == 0)
    {
      printf("\nDo you wish to skip this prompt?\n");
      printf("If so enter <y> for yes.\n");
      printf("------------------------------------------------------------\n");
      printf("Then if you wish to interrupt the program use CTRL C.\n");
      printf("[Press the C key while holding down the Ctrl or Control key].\n\n");
      printf("------------------------------------------------------------\n");
      printf("Else press Return or Enter to continue\n\n");
      scanf("%c",&ans);
      if(ans == 'y')
        {
          iskip++;
        }
    }
 return(0);
}
