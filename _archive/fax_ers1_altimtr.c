#include <stdio.h>          /* standard C library call  */
#include <ctype.h>
#define SIZE   24 
#define HEADER   1
typedef char * string;
void main()
{
  char file_name[140];       /* string to store the filename that is requested */
  char file_nam[140];       /* string to store the filename that is requested */
  char ans;
  unsigned char buf [SIZE];
char hold[20];
  int amount;
  int rec_count ,j ,i;         /* increment record count   */
  int count;
  int parm[6];             /* array to hold the parameter indexs  */
  double data[6];
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
  /*******************************************************************************/
  
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
            for (i=1; i <= 4; i++)
              {
                parm[i] = 0;
              }
	    rec_count = 0;
            wrong_num:
            printf("\n\n");
            printf("Here is the ERS1_ALTIMTR parameter list: \n");
            printf("\n");
            printf("        NAME                UNITS   \n");
            printf("----------------------------------  \n");
            printf("    1. LATITUDE           DEGREES\n");
            printf("    2. LONGITUDE          DEGREES\n");
            printf("    3. WIND SPEED         (M/S)\n");
            printf("    4. WAVE HEIGHT        (M)\n");
            printf("\n");
            printf("Enter 'a'   to process all the data parameters.\n");
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
             if  ((ans == 'X') || (ans == 'x'))
              {
                exit(0); 
              }
            if ((ans == 'A') || (ans == 'a'))
              {
                for (i=1; i<= 4; i++)
                  {
                    parm [i] = i;
                    amount = i;
                  }
                goto begin_process;
              }
            else if((ans == 'S') || (ans == 's'))
               {
                try_again:
                  printf("   How many parameters would you like printed out with\n");
                  printf("   each record? NOTE: maximum number of parameters is '4' !\n");
                  printf("   Please enter your selection now (1 - 4)\n");
                  wrong_val:
                  scanf("%s",&ans);
                     if((ans == 'X') || (ans == 'x'))
                       {
                         exit(0);
                       }
                     else if((ans != 'Q') && (ans != 'q'))
                       {
                        amount = atoi (&ans);
                        if ((amount > 4) || (amount < 0))
                          {
                            printf("There are only 4 values for you to select\n");
                            printf("Please enter a valid choice:\n");
                            goto wrong_val;
                          }
                        for (i=1; i<= amount; i++)
                           {
                             printf("Enter the parameter(number) you wish for column #%d.  \n ",i);
                             scanf("%s",&ans);
                             j = atoi(&ans);
                             if ((j <= 0) || (j > 4))
                               {
                                 printf("\nError : you have entered an invalid number for this seletion\n"); 
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
                            printf("You have requested the following parameters to process: \n");
                            for (i=1; i<= amount; i++)
                              {
                                if (parm[i] == 1)
                                  {
                                    printf("LATITUDE            %d\n",parm[i]);
                                  }
                                else if(parm[i] == 2)
                                  {
                                    printf("LONGITUDE           %d\n",parm[i]);
                                  }
                                else if(parm[i] == 3)
                                  {
                                    printf("WIND SPEED          %d\n",parm[i]);
                                  }
                                else if(parm[i] == 4)
                                  {
                                    printf("WAVE HEIGHT         %d\n",parm[i]);
                                  }
                              }
                            printf("\nIf you are dissatisfied with your selections\n");
                            printf("enter any value other than 'q' or 'x' to reenter your selections.\n");
                            printf("\n\nEnter 'q' or 'quit' to begin processing your selections\n");
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
                        }  /* End of the else */
                       }  /* End of the else loop  */      
                        goto wrong_num;
                        begin_process:
                for (i=0; i < HEADER; i++)
                  {
                    fread((char *) &hold[0], sizeof(unsigned char), SIZE ,fp);
                    printf("\n");
                    printf("%s\n",hold);
                  }
                for (count = 1; count <= amount; count++)
                  {
                   switch(parm[count])
                     {
                       case 0:
                         break;
                       case 1: 
                         printf("%-20s"," LATITUDE          ");
                         break;         
                       case 2:
                         printf("%-20s"," LONGITUDE         ");
                         break;
                       case 3:
                         printf("%-20s"," WIND SPEED        ");
                         break;
                       case 4:
                         printf("%-20s","WAVE HEIGHT       ");
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
                         printf(" %-20s"," DEGREES         ");
                         break;         
                       case 2:
                         printf("%-20s"," DEGREES         ");
                         break;
                       case 3:
                         printf("%-20s"," (M/S)             ");
                         break;
                       case 4:
                         printf("%-20s"," (M)              ");
                         break;
                       default:
                         break;
                    }                         /* End of switch */
                 }                     /* End of for */
                  printf("\n\n");
                    
                    while ((feof(fp)) == 0)
                      {
                        data[1]  = 0.0;
                        data[2]  = 0.0;
                        data[3]  = 0.0;
                        data[4]  = 0.0;
                        fscanf(fp,"%lf %lf %lf %lf", &data[1] ,&data[2] ,&data[3] ,&data[4]);
                        rec_count++;
               if (feof(fp) != 0)
                 {
                   printf("-------------------------------------------------------------------\n");
                   printf("~ End of file has been reached with %d records                   ~\n",rec_count);
                   printf("~ read in from the current data file ------> %s     \n",file_name);
                   printf("~ Program has completed successfully!                             ~\n");
                   printf("-------------------------------------------------------------------\n");
		   close(fp);
                   exit(0);
                 }
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
                                case 3:
                                  printf(" %10.4lf       ",data[parm[count]]);
                                  break;
                                case 4:
                                  printf(" %10.4lf       ",data[parm[count]]);
                                  break;
                                default:
                                  break;
                              } /* End of switch */
                          } /* End of for */
                          printf("\n"); 
                      } /* End of while loop */

}    /* End of main */ 
