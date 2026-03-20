#include <stdio.h>          /* standard C library call  */
#include <ctype.h>
#define SIZE    91
#define HEADER   1
typedef char * string;
void main()
{
  char file_nam[140];       /* string to store the filename that is requested */
  char file_name[140];       /* string to store the filename that is requested */
  char comma, ans;
  char ans2[2];
  unsigned char buf [SIZE];
  int amount;
  int rec_count ,j ,i;         /* increment record count   */
  int count;
  int parm[12];             /* array to hold the parameter indexs  */
  double data[12];
  FILE *fp;                /*  input file pointer       */

			   /*****************************/
			   /*   MAIN PROGRAM BEGINS     */
			   /*****************************/

  
  printf("\n\n\n\n");
  printf("\n\n\n\n");
  printf("\n\n\n\n");
  printf("\n\n\n\n");
  printf("\n\n\n\n");
  printf("\n\n\n\n");
  printf("   *****************************************************************************\n");
  printf("   *                                                                           *\n");
  printf("   *                       Langley DAAC                                        *\n");
  printf("   *                                                                           *\n");
  printf("   *        PROJECT_LEVEL/DATA_SET_LEVEL READ SOFTWARE                         *\n");
  printf("   *  VERSION NUMBER:                                                          *\n");
  printf("   *  Language: C                                                              *\n");
  printf("   *  Platforms supported:  SUN                                                *\n");
  printf("   *                        HP                                                 *\n");
  printf("   *                        SGI                                                *\n");
  printf("   *                        Dec Alpha                                          *\n");
  printf("   * Contact:  Langley DAAC user and Data Services Office                      *\n");
  printf("   *           NASA Langley Research Center                                    *\n");
  printf("   *           Mail Stop 157D                                                  *\n");
  printf("   *           Hampton, VA 23681-0001                                          *\n");
  printf("   *           PHONE:   (804)864-8656            FAX: (804)864-8807            *\n");
  printf("   *                                                                           *\n");
  printf("   *****************************************************************************\n");
  /*******************************************************************************/

  printf("Please enter the name of the INPUT file you wish to read : \n");
  printf("       NOTE: Data file may be in the current working directory.\n");
  printf("       NOTE: Or enter the complete pathname for the data filename.\n");
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
             					     /* prompt for the processing options */
            begin_loop:
            for (i=0; i <= 11; i++)
              {
                parm[i] = 0;
              }
	    rec_count = 0;
            fp = fopen(file_name, "r");
            wrong_num:
            printf("\n\n");
            printf("Here is the FUNCHAL parameter list: \n");
            printf("\n");
            printf("  NUM.      NAME                UNITS   \n");
            printf("---------------------------------------  \n");
            printf("    1. STATION NUMBER           NUM.\n");
            printf("    2. YEAR                     NUM.\n");
            printf("    3. MONTH                    NUM.\n");
            printf("    4. DAY                      NUM.\n");
            printf("    5. HOUR                     NUM. \n");
            printf("    6. ATMOSPHERIC PRESSURE     HPA\n");
            printf("    7. GEOPOTENTIAL HEIGHT      M\n");
            printf("    8. AIR TEMPERATURE          1/10 DEGREES C\n");
            printf("    9. RELATIVE HUMIDITY        PERCENT\n");
            printf("   10. WIND DIRECTION           DEGREES\n");
            printf("   11. WIND SPEED               KNOTS\n");
            printf("\n");
            printf("Enter 'a'   to process all the data paramters.\n");
            printf("Enter 's'   to select the parameter(s) to process. \n");
            printf("Enter 'h'   to receive help with commands.\n");
            printf("Enter 'x'   to exit or terminate processing. \n");

            scanf("%s",&ans);
            if ((ans == 'H') || (ans == 'h'))
               {
                 printf("\n\n\n\n");
                 printf("\n\n\n\n");
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
                 goto begin_loop;
               }
            if ((ans == 'X') || (ans == 'x'))
              {
                exit(0);
              }
            if ((ans == 'A') || (ans == 'a'))
              {
                for (i=1; i<= 11; i++)
                  {
                    parm [i] = i;
                    amount = i;
                  }
                goto begin_process;
              }
            else if((ans == 'S') || (ans == 's'))
               {
                 try_again:
                     printf("  How many parameters would you like printed out with\n");
                     printf("  each record? NOTE: the maximum number of parameters is '11'! \n");
                     printf("Please enter your selection now (1 - 11)\n");
                     wrong_val:
                     scanf("%s",&ans);
                     if ((ans == 'X') || (ans == 'x'))
                       {
                         exit(0);
                       }
                     else if ((ans != 'Q') && (ans != 'q'))
                       {
                         amount = atoi(&ans);
                         if ((amount > 11) || (amount < 0))
                           {
                             printf("There are only 11 values for you to select\n");
                             printf("Please enter a valid choice:\n");
                             goto wrong_val;
                           }
                         for (i=1; i<= amount; i++)
                           {
                             printf("Enter the parameter(number) you wish for column #%d.   ",i);
                             scanf("%s",ans2);
                               j = atoi(ans2);
                             if ((j <= 0) || (j > 11))
                               {
                                 printf("\nError : you have entered an invalid number for this selection!!\n");
                                 printf("\nIf you wish to exit enter 'x' otherwise hit any \n");
                                 printf("key to redo your selections!!\n");
                                 scanf("%s",&ans);
                                 if((ans == 'X') || (ans == 'x'))
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
                             if (parm[i] == 1)
                               {
                                 printf("STATION NUMBER              %d\n",parm[i]);
                               }
                             else if(parm[i] == 2) 
                               {
                                 printf("YEAR                        %d\n",parm[i]);
                               }
                             else if(parm[i] == 3) 
                               {
                                 printf("MONTH                       %d\n",parm[i]);
                               }
                             else if(parm[i] == 4) 
                               {
                                 printf("DAY                         %d\n",parm[i]);
                               }
                             else if(parm[i] == 5) 
                               {
                                 printf("HOUR                        %d\n",parm[i]);
                               }
                             else if(parm[i] == 6) 
                               {
                                 printf("ATMOSPHERIC PRESSURE        %d\n",parm[i]);
                               }
                             else if(parm[i] == 7) 
                               {
                                 printf("GEOPOTENTIAL HEIGHT         %d\n",parm[i]);
                               }
                             else if(parm[i] == 8) 
                               {
                                 printf("AIR PRESSURE                %d\n",parm[i]);
                               } 
                             else if(parm[i] == 9) 
                               {
                                 printf("RELATIVE HUMIDITY           %d\n",parm[i]);
                               }
                             else if(parm[i] == 10) 
                               {
                                 printf("WIND DIRECTION              %d\n",parm[i]);
                               }
                             else if(parm[i] == 11) 
                               {
                                 printf("WIND SPEED                  %d\n",parm[i]);
                               }
                           }  
                           printf("\nIf you are dissatisfied with your selections\n");
                           printf("enter any value other than 'q' or 'x' to reenter your selections:\n");
                           printf("\nEnter 'x' to exit the program.\n");
                           printf("\n\nEnter 'q' or 'quit' to begin processing your selections.\n");
                           scanf("%s",&ans);
                           if((ans == 'X') || (ans == 'x'))
                             {
                               exit(0);
                             }
                           if ((ans != 'q') && (ans != 'Q'))
                             {
                               goto try_again;
                             }
                           goto begin_process;
                   }  /*End of the else  */
              }   /*  End of the else loop */
               goto wrong_num;
                begin_process:
                for (count = 1; count <= amount; count++)
                  {
                   switch(parm[count])
                     {
                       case 0:
                         break;
                       case 1: 
                         printf("%10s","   STATION");
                         break;         
                       case 2:
                         printf("%10s","      YEAR");
                         break;
                       case 3:
                         printf("%10s","     MONTH");
                         break;
                       case 4:
                         printf("%10s","       DAY");
                         break;
                       case 5:
                         printf("%10s","      HOUR");
                         break;
                       case 6: 
                         printf("%10s"," ATM/PRESS");
                         break;
                       case 7:
                         printf("%11s"," GEO/HEIGHT");
                         break;
                       case 8:
                         printf("%10s"," AIR/TEMP");
                         break;
                       case 9:
                         printf("%10s"," REL/HUMID");
                         break;
                       case 10:
                         printf("%10s"," WIND/DIR");
                         break;
                       case 11:
                         printf("%10s"," WIND/SPEED");
                         break;
                       default:
                         break;
                    }                         /* End of switch */
                 }                     /* End of for */
                  printf("\n");
                    
                for (count = 1; count <= amount; count++)
                  {
                   switch(parm[count])
                     {
                       case 0:
                         break;
                       case 1: 
                         printf("%10s","       NUM");
                         break;         
                       case 2:
                         printf("%10s","       NUM");
                         break;
                       case 3:
                         printf("%10s","       NUM");
                         break;
                       case 4:
                         printf("%10s","       NUM");
                         break;
                       case 5:
                         printf("%10s","       NUM");
                         break;
                       case 6: 
                         printf("%10s","       HPA");
                         break;
                       case 7:
                         printf("%10s","       M  ");
                         break;
                       case 8:
                         printf("%11s"," 1/10 DEG_C");
                         break;
                       case 9:
                         printf("%10s","   PERCENT");
                         break;
                       case 10:
                         printf("%10s","   DEGREES");
                         break;
                       case 11:
                         printf("%10s","      KNOT");
                         break;
                       default:
                         break;
                    }                         /* End of switch */
                 }                     /* End of for */
                  printf("\n\n");
                    while ((feof(fp)) == 0)
                      {
                        data[1]  = 0;
                        data[2]  = 0;
                        data[3]  = 0;
                        data[4]  = 0;
                        data[5]  = 0;
                        data[6]  = 0;
                        data[7]  = 0;
                        data[8]  = 0;
                        data[9]  = 0;
                        data[10] = 0;
                        data[11] = 0;
                        data[12] = 0;
                        fscanf(fp,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                                  &data[1] ,&data[2] ,&data[3] ,&data[4] ,&data[5] ,&data[6] ,&data[7]
                                 ,&data[8] ,&data[9] ,&data[10] ,&data[11]);
                    if (feof(fp) != 0)
                      {
                        printf("-------------------------------------------------------------------\n");
                        printf("~ End of file has been reached with %d records                     \n",rec_count);
                        printf("~ read in from the current data file ------> %s     \n",file_nam);
                        printf("~ Program has completed successfully!                              \n");
                        printf("-------------------------------------------------------------------\n");
		        close(fp);
                        exit(0);
                      }
                        rec_count++;
                        for (count = 1; count <= amount; count++)
                          {
                            switch (parm[count])
                              {
                                case 1:
                                  printf("%10.1lf",data[parm[count]]);
                                  break;
                                case 2:
                                  printf("%10.1lf",data[parm[count]]);
                                  break;
                                case 3:
                                  printf("%10.1lf",data[parm[count]]);
                                  break;
                                case 4:
                                  printf("%10.1lf",data[parm[count]]);
                                  break;
                                case 5:
                                  printf("%10.1lf",data[parm[count]]);
                                  break;
                                case 6:
                                  printf("%10.1lf",data[parm[count]]);
                                  break;
                                case 7:
                                  printf("%10.1lf",data[parm[count]]);
                                  break;
                                case 8:
                                  printf("%10.1lf",data[parm[count]]);
                                  break;
                                case 9:
                                  printf("%10.1lf",data[parm[count]]);
                                  break;
                                case 10:
                                  printf("%10.1lf",data[parm[count]]);
                                  break;
                                case 11:
                                  printf("%10.1lf",data[parm[count]]);
                                  break;
                                default:
                                  break;
                              } /* End of switch */
                          } /* End of for */
                          printf("\n"); 
                      } /* End of while loop */
}    /* End of main */ 
