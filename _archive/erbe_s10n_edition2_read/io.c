/************************************sccsidkeywordsstart****************
*     *** This file is under configuration control
*     *** sccs keywords--do not modify these values!!!
*     @(#)io.c    93.1    94/04/20
*************************************sccsidkeywordsend*****************/
#include <stdio.h>
#include <errno.h>


void syserr(msg)
char *msg;
{
   extern int errno, sys_nerr;
   extern char *sys_errlist[];

   fprintf(stderr, " ERROR:%s,(%d", msg, errno);
   if (errno > 0 && errno < sys_nerr)
      fprintf(stderr, ";%s)\n", sys_errlist[errno]);
   else
      fprintf(stderr, ")\n");
   exit(1);
}

void copen_(name, length, unit)
 /* FORTRAN callable routine to "open" a file for read/write  */
char name[];
int length[1];
int *unit;
{
   int fd;
   char file[80];
   int i, n;

   n = (int) length[0];
   for (i = 0; i <= n; i++)
      file[i] = name[i];
   file[n] = '\0';

   if ((fd = open(file, 2)) == -1)
      syserr("copen");
   *unit = (int) fd;
   /* printf("Unit opened %d\n",unit[0]) ; */
}

void ccreat_(name, length, unit)
 /* FORTRAN callable routine to "create" a file   */
 /* (Permissions defaulted to 644 (octal))        */
char name[];
long int length[];
int *unit;
{
   int fd;
   char file[80];
   int i, n;

   n = (int) length[0];
   for (i = 0; i <= n; i++)
      file[i] = name[i];
   file[n] = '\0';

   if ((fd = creat(file, 0644)) == -1)
      syserr("ccreat");
   *unit = (int) fd;
}

cclos_(unit, ioerr)
 /* FORTRAN callable routine to "close" a file   */
int *unit;
int *ioerr;
{
   int fd, ierr;

   fd = (int) *unit;
   if ((ierr = close(fd)) == -1)
      syserr("cclos");
   *ioerr = (int) ierr;
}

void crewnd_(unit, ioerr)
 /* FORTRAN callable routine to "rewind" a file   */
int *unit;
int *ioerr;
{
   int fd, ierr;

   fd = (int) *unit;
   if ((ierr = lseek(fd, 0L, 0)) == -1)
      syserr("crewnd");
   *ioerr = (int) ierr;
}

void cwrit_(unit, nbuf, buf, ioerr)
 /* FORTRAN callable routine to "write" character buffer   */
long int unit[1], nbuf[1];
char *buf;
int *ioerr;
{
   int fd, n, ierr;

   fd = (int) unit[0];
   n = (int) nbuf[0];
   if ((ierr = write(fd, buf, n)) == -1)
      syserr("cwrit");
   *ioerr = (int) ierr;
}

void cread_(unit, nbuf, buf, ioerr)
 /* FORTRAN callable routine to "read" character buffer   */
int *unit;
int *nbuf;
char *buf;
int *ioerr;
{
   int fd, ni, nr;
   /* printf("cread() : on unit %d\n",*unit) ; */
   fd = (int) *unit;
   ni = (int) *nbuf;
   /* printf("cread() : to read %d bytes\n",ni) ; */
   if ((nr = read(fd, buf, ni)) == -1)
      syserr("cread");
   *ioerr = (int) nr;
}
void pcwrit_(unit, nbuf, buf, spos, ioerr)
 /* FORTRAN callable routine to "write" character buffer   */
long int unit[1], nbuf[1];
char *buf;
int *spos;
int *ioerr;
{
   int fd, n, ierr;

   fd = (int) unit[0];
   n = (int) nbuf[0];
   if ((ierr = write(fd, &buf[*spos], n)) == -1)
      syserr("pcwrit");
   *ioerr = (int) ierr;
}
