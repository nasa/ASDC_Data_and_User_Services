#include <stdio.h>
#define SIZE 80 
#define HEADER 18
void main()
{
 char file_name[256];
 unsigned char *buf;
 unsigned char buff [SIZE];
 FILE *fp;
 int nob = 1961216;
 int nbp = 490304;
 int i;
 int mask;
 int v;
 int w;
 int irow;
 int inew;
 int count;
 float value;
 float lat;
 float lon;
 printf("\n\n");
 printf("\n\n");
 printf("\n\n");
 printf("\n\n");
 printf("\n\n");
 printf("\n\n");
 printf("\n\n");
 printf("\n\n");
 printf("\n\n");
 printf("\n\n");
  
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

 printf("   ************************************************************************\n");
 printf("   * Please enter the name of the INPUT file you wish to read :           *\n");
 printf("   *        NOTE: Data file may be in the current working directory.      *  \n");
 printf("   *        NOTE: Or enter the complete pathname for the data filename.   * \n");
 printf("   ************************************************************************\n");

 buf = (unsigned char *)malloc ((unsigned) (1 * nob)); 

 scanf("%s",file_name);

 fp =  fopen(file_name, "r");
 if (fp == NULL)
   { 
     printf("  ERROR on the file\n");
   }

 for (i=0; i<HEADER; i++)
   {
    fread ((char *) &buff[0], sizeof(unsigned char), SIZE ,fp);
   }

/*          Now Read The File         */

 fseek(fp,1128,0);
 fread((char *)buf,1,nob,fp); 

 inew=1;
 mask = 1;
 mask <<= 31;
 v=buf[1];
 for(irow=1;irow<=163;irow++)
   {
    printf("\n");
    printf("Now do the processing for row %d\n",irow);
    lat = (39.99 - ((irow - 1) * 0.08));
    printf("  Latitude :  %5.2f\n",lat);
    printf("\n");
                     printf("       These are the data values from the file\n"); 
                     printf("pixel                                                       starting pixel count\n");
                     printf("count    CLOUD_DATA   CLOUD_CODE  TYPE           Longitude  in the data for row %d\n",irow);
                     printf("-------------------------------------------------------------------------------------------------------\n");
                     for(count=1;count<=188;count++)
                       { 
/*      Now shift the data four bits to the right             */
/*      This will give you the cloud type                     */
                          w=buf[inew] >> 4;
                          lon = (-29.97 + ((count - 1) * 0.08));
                              switch(w)
                                 {
                                    case 1 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  clear                 %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 2 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  cloud edges           %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 3 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  cloud edges           %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 4 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  thin cirrus           %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 5 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  thick cirrus          %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 6 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  thick cirrus          %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 7 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  perturbation          %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 8 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  low cumuliform        %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 9 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  low cumuliform        %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 10 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  low cumuliform        %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 11 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  low stratiform        %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 12 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  low stratiform        %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 13 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  medium                %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 14 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  medium                %5.2f     %3d\n",lon,inew);
                                              break;
                                    case 15 :
                                              printf("%3d       %3d           %3d",count,buf[inew],w);
                                              printf("  medium                %5.2f     %3d\n",lon,inew);
                                              break;
                                    default:
                                              break;
                                 }
		       inew++;
   }
}
  printf("\n");
  printf("*************************************************************\n");
  printf("The programs has completed processing for file : %s\n",file_name);
  printf("A total of %d",irow-1);
  printf(" rows were processed.\n");
  printf("*************************************************************\n");
}
