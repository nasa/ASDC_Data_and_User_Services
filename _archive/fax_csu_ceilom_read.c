#include <stdio.h>          /* standard C library call  */
#include <ctype.h>
void main()
{
  char file_nam[140];       /* string to store the filename that is requested */
  char file_name[140];       /* string to store the filename that is requested */
  char comma, ans;
  int amount;
  int rec_count ,j ,i;         /* increment record count   */
  int count;
  int parm[10];             /* array to hold the parameter indexs  */
  double data [3];
  FILE *fp;                /*  input file pointer       */

			   /*****************************/
			   /*   MAIN PROGRAM BEGINS     */
			   /*****************************/
  
  printf("\n\n\n\n\n");
  printf("\n\n\n\n\n");
  printf("\n\n\n\n\n");
  printf("\n\n\n\n\n");
  printf("\n\n\n\n\n");
  printf("\n\n\n\n\n");
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
            for (i=0; i < 3; i++)
              {
                parm[i] = 0;
              }
	    rec_count = 0;
            fp = fopen(file_name, "r");
            wrong_num:
            printf("\n\n");
            printf("Here is the CEILOMETER parameter list: \n");
            printf("\n");
            printf("  NUM.        NAME              UNITS   \n");
            printf("--------------------------------------  \n");
            printf("    1.    JULIAN DATE            NUM.     \n");
            printf("    2.  CLOUD BASE HEIGHT      METERS    \n");
            printf("\n");
            printf("Enter 'a'   to process all the data paramters.\n");
            printf("Enter 's'   to select the number(s) for the parameter(s) to process. \n");
            printf("Enter 'h'   to receive help with commands.\n"); 
            printf("Enter 'x'   to exit or terminate processing. \n");

            scanf("%s",&ans);
            if((ans == 'H') || (ans =='h'))
              {
                printf("\n\n\n\n");
                printf("\n\n\n\n");
                printf("\n\n\n\n");
                printf("\n\n\n\n");
                printf("\n\n\n\n");
                printf("\n\n\n\n");
                printf("**********************************************************\n");
                printf("*                                                        *\n");
                printf("*                  HELP    COMMAND                       *\n");
                printf("*                                                        *\n");
                printf("**********************************************************\n"); 
                printf("Option selection 'a' is the 'all' option.  Select this option\n");
                printf("to print out all parameters for all data records.\n\n");
                printf("Option selection 's' is the I want to print out only\n");
                printf("selected parameters for all data records option.\n");
                printf("This option lets you print those parameters out in any\n");
                printf("order.  The first thing the program prompts for is the\n");
                printf("total number of parameters from the menu list you wish\n");
                printf("to have printed.  Then you are prompted for each parameter\n");
                printf("number to be printed for a given column. This allows you\n");
                printf("to print the parameters in any column order.\n");
                goto begin_loop;
              }
              
            if ((ans == 'X') || (ans == 'x'))
              {
                exit (0);
              }
            if ((ans == 'A') || (ans == 'a'))
              {
                for (i=0; i< 3; i++)
                  {
                    parm [i] = i;
                    amount = i;
                  }
                goto begin_process;
              }
            else if ((ans == 'S') || (ans == 's'))
              {
                 try_again:
                    printf("   How many parameters from the menu would you like printed out with\n");
                    printf("   each record? NOTE: the maximum number of parameters is '2' !\n");
                    printf("   Please enter your selection now: (1 or 2)                    \n");
                    wrong_val:
                    scanf("%s",&ans);
                    if ((ans == 'X') || (ans == 'x'))
                      {
                        exit(0);
                      }
                    else if ((ans != 'Q') && (ans != 'q'))
                      { 
                        amount = atoi(&ans); 
                        if ((amount > 2) || (amount < 0)) 
                          {
                            printf("There are only two values for you to select\n");
                            printf("Please enter a valid choice:\n");
                            goto wrong_val;
                          }
                        for (i=1; i<= amount; i++)
                          {
                            printf("Enter the parameter(number) you wish for column #%d.   ",i);
                            scanf("%s",&ans);
                            if((ans < '0') || (ans > '2'))
                              {
                                printf("\nError : you have entered an invalid number for this column!!!\n");
                                printf("Continue your selections you can redo your selections before processing.\n\n"); 
                              }
                            parm[i] = atoi(&ans);
                          }
                            printf("\n");
                            printf("You have requested the following parameters to process: \n\n");
                            for (i=1; i<= amount; i++)
                              {
                                if (parm[i] == 1)
                                  {
                                    printf("JULIAN DATE        %d\n",parm[i]);
                                  }
                                else if(parm[i] == 2)
                                  {
                                    printf("CLOUD BASE HEIGHT  %d\n",parm[i]);
                                  }
                              }
                               printf("\nIf you are dissatisfied with your selections\n");
                               printf("enter any value other than 'q' or 'x' to reenter your selections:\n");
                               printf("\nEnter 'x' to exit the program.\n");
                               printf("\n\nEnter 'q' or 'quit' to begin processing your selections!\n");
                               scanf("%s",&ans);
                               if ((ans == 'X') || (ans == 'x'))
                                 {
                                   exit (0);
                                 }
                               if((ans != 'q') && (ans != 'Q'))
                                 {
                                   goto try_again;
                                 }
                                  goto begin_process;
                      }  /* End of the else */
               }    /* End of the else loop */   
                goto wrong_num;
                      begin_process:
                      for(count = 1; count <= amount; count++)
                       {
                         switch (parm[count])
                          {
                            case 1:
                              printf("%-20s","   JULIAN          "); 
                              break;
                            case 2:
                              printf("%-20s","   CLOUD           ");
                              break;
                            default:
                              break; 
                           }          /* End of switch */
                       }       /* End of for */ 
                    printf("\n");
                    for (count = 1; count <= amount; count++)
                      {
                        switch (parm[count])
                          {
                            case 1:
                              printf("%-20s","   NUM.             "); 
                              break;
                            case 2:
                              printf("%-20s","   METERS           ");
                              break;
                            default:
                              break; 
                           }          /* End of switch */
                      }       /* End of for */ 
                    printf("\n\n");
                    while ((feof(fp)) == 0)
                      {
                        data[1] = 0.0;
                        data[2] = 0.0;
                        fscanf(fp,"%lf %c %lf \n", &data[1] ,&comma ,&data[2]); 
                        rec_count++;
                        for (count = 1; count <= amount; count++)
                          {
                            switch (parm[count])
                              {
                                case 1:
                                  printf(" %10.4lf       ",data[parm[count]]);
                                  break;
                                case 2:
                                  printf(" %10.4lf       ",data[parm[count]]);
                                  break;
                                default:
                                  break;
                              } /* End of switch */
                           } /* End of for */ 
                          printf("\n"); 
                      if (feof(fp) != 0)
                        {
                 printf("-------------------------------------------------------------------\n");
                 printf("~ End of file has been reached with %d records                   ~\n",rec_count);
                 printf("~ read in from the current data file ------> %s     \n",file_nam);
                 printf("~ Program has completed successfully!                             ~\n");
                 printf("-------------------------------------------------------------------\n");
		 close(fp);
                        }
                     }
}    /* End of main */ 
