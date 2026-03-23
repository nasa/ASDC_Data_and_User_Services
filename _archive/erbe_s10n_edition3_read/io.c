#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
/*----------------------------------------------*/
int bit_(bitnum,word)
int *bitnum;
int *word;
  {
    int bit;
    bit = findit(bitnum,word);
    return(bit);
  }
/*----------------------------------------------*/
int findit(bitnum,word)
int *bitnum;
int *word;
  {
     int bit=0;
     static int flag = 0;
     static int ifirst = 0;
     #define mask7 (1<<7)
     char wrd0=0;
     char wrd1=0;
     char wrd2=0;

     char wrd3=0;
     wrd3 = *word >> 24;
     wrd2 = *word >> 16;
     if((flag != 1) && (ifirst != 1))
       {
         if(wrd3 == 0 && wrd2 == 0 && *word < 50000)
           {
             flag = 1;
           }
       }
     if(flag == 1)
       {
         wrd1=*word >>8;
         wrd0=*word & 0x0ff;
         *bitnum = 15;
       }
     else
       {
         ifirst=1;
         *bitnum = 31;
         wrd2 = wrd2 & 0x0ff;
         wrd1 = *word >> 8;

         wrd1 = wrd1 & 0x0ff;
         wrd0 = *word & 0x0ff;
       }
     switch (*bitnum)
       {
          case 15:
            if((wrd1 & mask7) != 0)
              {
                bit=1;
              }
            break;
         case 31:
           if((wrd3 & mask7) != 0)
             {
               bit=1;
             }
           break;
       }
   return(bit);
 }
/*----------------------------------------------*/
void setbit_(bitnum,word,msbit)
int *bitnum;
int *word;
int *msbit;
  {
    #define mask0 (1<<0)
    static int flag = 0;
    static int mode = 0;
    if(flag != 1)
      {
        if(*word > 99999)
          {
            mode = 32;
          }
        flag = 1;
      }
    if(*msbit > 0)
      {
        if((*word & mask0) == 0)
          {
            *word |= mask0;
          }
      }
    else
      {
        if((*word & mask0) != 0)
          {
           *word &= ~mask0;
          }
      }
     if(mode == 0)
       {
         *word=*word & 0x0ffff;
       }
  }
/*----------------------------------------------*/
   void syserr (msg)
   char *msg;
   {
      extern int errno,sys_nerr;
      extern char *sys_errlist[];

      fprintf (stderr," ERROR:%s,(%d",msg,errno);
      if (errno>0 && errno<sys_nerr)
         fprintf (stderr,";%s)\n",sys_errlist[errno]);
      else
         fprintf (stderr,")\n");
      exit(1);
   }

 void   copen_ (name,length,unit)
   /*   FORTRAN callable routine to "open" a file for read/write  */
   char name[];
   int *length;
   int *unit ;
   {
      int fd;
      char file[80];
      int i,n;

      n = (int) *length;
      for (i=0; i<=n; i++)
          file[i] = name[i];
      file[n] = '\0';
      if ( (fd = open (&file[0],O_RDONLY)) == -1)
         syserr ("copen");
      *unit = (int) fd;
   }

   void  ccreat_ (name,length,unit)
   /*   FORTRAN callable routine to "create" a file   */
   /*   (Permissions defaulted to 644 (octal))        */
   char name[];
   long int length[];
   int *unit;
   {
      int fd;
      char file[80];
      int i,n;

      n = (int) length[0];
      for (i=0; i<=n; i++)
          file[i] = name[i];
      file[n] = '\0';

      if ( (fd = creat (file,0644)) == -1)
         syserr ("ccreat");
      *unit = (int) fd;
   }

   cclos_ (unit,ioerr)
   /*   FORTRAN callable routine to "close" a file   */
   int *unit;
   int *ioerr;
   {
      int fd,ierr;

      fd = (int) *unit;
      if ( (ierr = close (fd)) == -1)
         syserr ("cclos");
      *ioerr = (int) ierr;
   }

   void crewnd_ (unit,ioerr)
   /*   FORTRAN callable routine to "rewind" a file   */
   int *unit;
   int *ioerr;
   {
      int fd,ierr;

      fd = (int) *unit;
      if ( (ierr = lseek (fd,0L,0)) == -1)
         syserr ("crewnd");
      *ioerr = (int) ierr;
   }

    void cwrit_ (unit,nbuf,buf,ioerr)
   /*   FORTRAN callable routine to "write" character buffer   */
   long int unit[1],nbuf[1];
   char *buf;
   int *ioerr;
   {
      int fd,n,ierr;

      fd = (int) unit[0];
      n = (int) nbuf[0];
      if ( (ierr = write (fd,buf,n)) == -1)
         syserr ("cwrit");
      *ioerr = (int) ierr;
   } 

   void cread_ (unit,nbuf,buf,ioerr)
   /*   FORTRAN callable routine to "read" character buffer   */
   int *unit;
   int *nbuf;
   char *buf;
   int *ioerr;
   {
      int fd,ni,nr;
   /*   printf("cread() : on unit %d\n",*unit) ;*/
      fd = (int) *unit;
      ni = (int) *nbuf;
   /*   printf("cread() : to read %d bytes\n",ni) ;*/
      if ( (nr = read (fd,buf,ni)) == -1)
          syserr ("cread");
      *ioerr = (int) nr;
   }
    void pcwrit_ (unit,nbuf,buf,spos,ioerr)
   /*   FORTRAN callable routine to "write" character buffer   */
   long int unit[1],nbuf[1];
   char *buf;
   int *spos;
   int *ioerr;
   {
      int fd,n,ierr;

      fd = (int) unit[0];
      n = (int) nbuf[0];
      if ( (ierr = write (fd,&buf[*spos],n)) == -1)
         syserr ("pcwrit");
      *ioerr = (int) ierr;
   } 

