#include <stdio.h>          /* standard C library call  */
#include <ctype.h>
#define SIZE    40
#define HEADER   1
typedef char * string;
main()
{
  char file_name[140];       /* string to store the filename that is requested */
  char file_nam[140];       /* string to store the filename that is requested */
  char file_out[140];       /* string to store the filename that is requested */
  char comma, ans;
  char ans2[3];
  char a, b;
  char unit[1][4];
  unsigned char buf [SIZE];
  int amount;
  int ifile;
  int icount;
  int rec_count ,j ,i;         /* increment record count   */
  int count;
  int done;
  int parm[10];             /* array to hold the parameter indices  */
  double data [10];
  FILE *fp;                /*  input file pointer       */
  FILE *fpo;                /* output file pointer       */

			   /*****************************/
			   /*   MAIN PROGRAM BEGINS     */
			   /*****************************/  
  printf("\n\n\n\n");  
  printf("\n\n\n\n");  
  printf("\n\n\n\n");  
  printf(" ****************************************************************\n");
  printf(" *                                                              *\n");
  printf(" *                                                              *\n");
  printf(" *            FIRE_ACE_UWCV580_GMETER Read Software             *\n");
  printf(" *                                                              *\n");
  printf(" *  Version: 1.0                                                *\n");
  printf(" *                                                              *\n");
  printf(" *  Date:    August 10, 1999                                    *\n");
  printf(" *                                                              *\n");
  printf(" *  Contact: Science User and Data Services Office              *\n");
  printf(" *           Langley Atmospheric Sciences Data Center           *\n");
  printf(" *           NASA Langley Research Center                       *\n");
  printf(" *           Mail Stop 157D, 2 S. Wright St.                    *\n");
  printf(" *           Hampton, VA 23681-2199                             *\n");
  printf(" *           USA                                                *\n");
  printf(" *                                                              *\n");
  printf(" *           E-mail: larc@eos.nasa.gov                          *\n");
  printf(" *           Phone:  (757)864-8656                              *\n");            
  printf(" *           FAX:    (757)864-8807                              *\n");
  printf(" *                                                              *\n");  
  printf(" *                                                              *\n");
  printf(" ****************************************************************\n");

  /*******************************************************************************/
  printf("\n \n");
  printf("Please enter the name of the INPUT file you wish to read : \n");
  printf(" NOTE: Data file may be in the current working directory,\n");
  printf("       or enter the complete pathname for the data filename.\n");
  printf("\n \n");
  scanf("%s",file_name);
  strcpy(file_nam,file_name);
  fp = fopen(file_name, "r");
  if (fp == NULL)
    {
      printf("***************************************************************************\n");
      printf("* ERROR - Unable to open the data file for this filename -->  %s\n",file_name);
      printf("*                  Program has terminated.                                *\n");
      printf("***************************************************************************\n");
      exit ( -1 );
    }

  ifile=atoi(&file_name[24]);
/*  printf("%i\n",ifile);*/
icount = 5;
  if(ifile > 19980605)
    {
      icount = 3;
    }
 while(done != 1)
   {
     printf("\nPlease enter the name of the OUTPUT file: \n \n");
     scanf("%s",file_out);
     fpo = fopen(file_out, "r");
     if (fpo != NULL)
       {
          printf("\n");
          printf("*************************************************************\n");
          printf("* WARNING: This filename exists: %s\n", file_out);
          printf("*          Do you wish to overwrite it?  'y' yes  'n' no    *\n");
          printf("*************************************************************\n");
          printf("\n");
          scanf("%s",ans2);
          if(tolower(ans2[0]) == 'y')
            {
              fpo = fopen(file_out, "w");
              break;
            }
          continue;
       }
          fpo = fopen(file_out,"w");
          break;
   }

/* prompt for the processing options */
            begin_loop:
            for (i=0; i < icount; i++)
              {
                parm[i] = 0;
              }
	    rec_count = 0;
            fp = fopen(file_name, "r");
            wrong_num:
            printf("\n\n");
            printf("Here is the GMETER parameter list: \n");
            printf("\n");
            if(icount == 5)
              {
                printf("  NUM.        NAME              UNITS  \n");
                printf("-------------------------------------- \n");
                printf("    1.        T                  Sec.  \n");
                printf("    2.        G                  n/a   \n");
                printf("    3.        E                 (1/Km) \n");
                printf("    4.        E/B                n/a   \n");
              }
            else
              {
                printf("  NUM.        NAME              UNITS   \n");
                printf("--------------------------------------  \n");
                printf("    1.        UW time            UTC    \n");
                printf("    2.        E                 (1/Km)  \n");
              }
            printf("\n");
            printf("Enter 'a'   to process all the data parameters.\n");
            printf("Enter 's'   to select the number(s) for the parameter(s) to process. \n");
            printf("Enter 'h'   to receive help with commands.\n");
            printf("Enter 'x'   to exit or terminate processing. \n \n");

            scanf("%s",&ans);
            if ((ans == 'H') || (ans == 'h'))
               {
                 printf("\n\n\n\n");
                 printf("\n\n\n\n");
                 printf("\n\n\n\n");
                 printf("\n\n\n\n");
                 printf("*******************************************************************\n");
                 printf("*                                                                 *\n");
                 printf("*                       HELP COMMAND                              *\n");
                 printf("*                                                                 *\n");
                 printf("*******************************************************************\n"); 
                 printf("Option selection 'a' is the 'all' option.  Select this option\n");
                 printf("to print out all parameters for all data records.\n\n");
                 printf("Option selection 's' is the option to select particular parameters\n");
                 printf("to be printed out.  This option allows you to print out those\n");
                 printf("parameters in any order you wish.  The first thing the program prompts\n"); 
                 printf("for is the total number of parameters from the menu list you wish to\n");
                 printf("have printed.  Then you are prompted for each parameter to be printed\n");
                 printf("in a given column.  This allows you to print the parameters in any \n"); 
                 printf("order.");
                 printf("\n");
                 goto begin_loop;
               }
            if ((ans == 'X') || (ans == 'x'))
              {
                exit (0);
              }
            if ((ans == 'A') || (ans == 'a'))
              {
                for (i=0; i< icount; i++)
                  {
                    parm [i] = i;
                    amount = i;
                  }
                goto begin_process;
              }
            else if ((ans == 'S') || (ans == 's'))
/*select parameters*/
              {
                try_again:
                    printf("   How many parameters would you like printed out with\n");
                    printf("   each record? NOTE: maximum number of parameters is %d\n",icount-1);
                    printf("   Please enter your selection now: (1 - %d",icount-1);
                    printf(")\n");
                    wrong_val:
                    scanf("%s",&ans);
                    if ((ans == 'X') || (ans == 'x'))
                      {
                        exit(0);
                      }
                    else if ((ans != 'Q') && (ans != 'q'))
                      { 
                        amount = atoi(&ans); 
                        if ((amount > icount-1) || (amount < 0))
                          {
                            printf("There are only %d ",icount-1, " values for you to select\n");
                            printf("Please enter a valid choice:\n"); 
                            goto wrong_val;
                          }
                        for (i=1; i<= amount; i++)
                          {
                            printf("Enter the parameter(number) you wish for column #%d.   ",i);
                            scanf("%s",&ans);
                            j = atoi(&ans);
                            if ((j <= 0) || (j > icount-1))
                              {
                                printf("\nError : you have entered an invalid number for this selection!!\n");
                                printf("\nIf you wish to exit enter 'x' otherwise hit any \n");
                                printf("key to redo your selections!!\n");
                                scanf("%s",&ans);
                                if ((ans == 'X') || (ans == 'x'))
                                  {
                                    exit(0);
                                  }
                                    goto try_again;
                               }
                            parm[i] = j;
                          }
                           printf("\n");
                            printf("You have requested the following parameters to process: \n\n");
                            for (i=1; i<= amount; i++)
                              {
                                if(icount == 5)
                                  {
                                    if (parm[i] == 1)
                                      {
                                        printf("T(S)                      %d\n",parm[i]);
                                      }
                                    else if(parm[i] == 2)
                                      {
                                        printf("G                         %d\n",parm[i]);
                                      }
                                    else if(parm[i] == 3)
                                      {
                                        printf("E                   %d\n",parm[i]);
                                      }
                                    else if(parm[i] == 4)
                                      {
                                        printf("E/B                       %d\n",parm[i]);
                                      }
                                  }
                                 else
                                  {
                                    if (parm[i] == 1)
                                      {
                                        printf("UW time                    %d\n",parm[i]);
                                      }
                                    if (parm[i] == 2)
                                      {
                                        printf("E                    %d\n",parm[i]);
                                      }
                                   }
                                 }
                            printf("\nIf you are dissatisfied with your selections\n");
                            printf("enter any value other than 'q' or 'x' to reenter your selection\n"); 
                            printf("\nEnter 'x' to exit the program.\n");
                            printf("\n\nEnter 'q' or 'quit' to begin processing your selections.\n \n");
                            scanf("%s",&ans);
                            if ((ans == 'X') || (ans == 'x'))
                              {
                                exit(0);
                              }
                            if ((ans != 'q') && (ans != 'Q'))
                              {
                                goto try_again;
                              } 
                            goto begin_process;
                         }  /*End of the else   */
                      }  /* End of the else loop   */
                       goto wrong_num;
                       begin_process:
                 for (i=0; i < HEADER; i++)
                   {
                     if(icount == 5)
                       {
                          fread((char *) &buf[0] ,sizeof(unsigned char), SIZE ,fp);
                       }
                     else
                       {
                          fread((char *) &buf[0] ,sizeof(unsigned char), 25 ,fp);
                       } 
                                                 /*  print out the output names */
                   }
                    for (count = 1; count <= amount; count++)
                      {
                         switch (parm[count])
                            {
                            case 1:
                              if(icount == 5)
                                {
                                  fprintf(fpo,"  %9s","T(Sec)"); 
                                }
                              else
                                {
                                  fprintf(fpo,"  %9s","UW time"); 
                                }
                              break;
                            case 2:
                              if(icount == 5)
                                {
                                  fprintf(fpo,"  %9s","G");
                                }
                              else
                                {
                                  fprintf(fpo,"  %9s","E (1/Km)"); 
                                }
                              break;
                            case 3:
                              fprintf(fpo,"  %10s","E(1K/m)");
                              break;
                            case 4:
                              fprintf(fpo,"  %10s","E/B");
                              break;
                            default:
                              break; 
                          }          /* End of switch */
                      }       /* End of for */ 
                    fprintf(fpo,"\n");

                    for (count = 1; count <= amount; count++)
                      {
                        switch (parm[count])
                          {
                            case 1:
                              fprintf(fpo,"  %9s","Sec."); 
                              break;
                            case 2:
                              fprintf(fpo,"  %9s","n/a");
                              break;
                            case 3:
                              fprintf(fpo,"  %10s","(1/km).");
                              break;
                            case 4:
                              fprintf(fpo,"  %10s","    n/a");
                              break;
                            default:
                              break; 
                          }          /* End of switch */
                      }       /* End of for */ 
                    fprintf(fpo,"\n\n");
                    while ((feof(fp)) == 0)
                      {
                        data[1] = 0.0;
                        data[2] = 0.0;
                        data[3] = 0.0;
                        data[4] = 0.0;
                        if (icount == 5)
                          {
                            fscanf(fp,"%lf %lf %lf %lf ",
                                       &data[1] ,&data[2] ,&data[3] ,&data[4]);
                          }
                        else
                          {
                            fscanf(fp,"%lf %lf ",
                                       &data[1] ,&data[2]);
                          }
/**/
                        rec_count++;
/**/
                       for (count = 1; count <= amount; count++)
                          {
                            switch (parm[count])
                              {
                                case 1:
                                  fprintf(fpo," %10.4lf ",data[parm[count]]);
                                  break;
                                case 2:
                                  fprintf(fpo," %10.4lf ",data[parm[count]]);
                                  break;
                                case 3:
                                  fprintf(fpo," %10.4lf ",data[parm[count]]);
                                  break;
                                case 4:
                                  fprintf(fpo," %10.4lf ",data[parm[count]]);
                                  break;
                                default:
                                  break;
                              } /* End of switch */
                           } /* End of for */ 
                          fprintf(fpo,"\n"); 
               if (feof(fp) != 0)
               {
                 printf("---------------------------------------------------------\n");
                 printf("~ End of file has been reached with %d records      \n",rec_count);
                 printf("~ read in from the data file: %s     \n",file_name);
                 printf("~ Program has completed successfully!               \n");
                 printf("---------------------------------------------------------\n");
		 close(fp);
		 close(fpo);
                 exit(0);
               }
            } /* End of the while loop  */
}    /* End of main */ 
