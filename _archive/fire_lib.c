/* 
@(#) File name: fsdf_lib.c	Release: 1.1	Date: 7/28/94, 14:00:09
*/

/****************************************************************************
FILE:
	fsdf_lib

AUTHOR: Meng-chun Lin, Mail stop 157B, EOSDIS LANGLEY DAAC, Hampton, VA 23666
	(804)864-8657.  e-mail: M.LIN@LaRC.NASA.GOV

PURPOSE/DESCRIPTION:
	Program fsdf_lib has a collection of library routines that are 
	used by the read routines for the FIRE Cirrus I, Cirrus II, and FIRE
	MS data sets.  This program contains five libray functions: str_trim(), 
	month_day (), swap_byte (), julian_day(), and interrupt_cat().

INTERNAL ROUTINES: 
	str_trim - trims the trailing blanks, tabs, and the new-line 
	    characters from a string.  The function returns the length
	    of the new trimmed string.
	month_day () - returns month and day of the month from the elapsed 
            number of days of the year.
	swap_byte () - reverses the order of bytes in the input unsigned 
	    character array. 
       	julian_day - returns the elapsed number of days of the year.  Since 
	    the leap year has 29 days in February, the julian day will be 
	    different after February 28 between leap year and non-leap year.
	interrupt_cat - provides means to gracefully exit when the interrupt 
	    signals were caught.

NOTES:
	Function dy_size() is the support routine for julian_day(), which 
	simulates dysize() function.  It is here because some machines do 
	not support libray function dysize().
	
*****************************************************************************/
#ifdef sccs
static char sccsid[] = "File: fsdf_lib.c  Release: 1.1  Date: 7/28/94, 14:00:09"
#endif

#include  <stdlib.h>
#include  <stdio.h>
#include  <malloc.h>
#include  <sys/types.h>
#include  <sys/param.h>
#include  <string.h>


/****************************************************************************
FUNCTION:
	str_trim ()

PURPOSE/DESCRIPTION:
    STR_TRIM trims trailing white space (blanks, tabs, and new-line characters)
    from a string.  If the length argument is zero, the string is assumed to be
    null-terminated; after trimming trailing white space, the null terminator
    is relocated to the new end of the string.  If the length argument is NOT
    zero, the string does NOT need to be null-terminated; after trimming
    trailing white space, the null terminator is NOT relocated.  In either
    case, STR_TRIM returns the length of the new string.


    Invocation:

            trimmed_length = str_trim (string, length) ;

    WHERE 
        <string>
            is the string to be trimmed.  If the length argument is zero,
            STRING is assumed to be null-terminated and STR_TRIM will
            ***** relocate the null terminator *****.  If LENGTH is not
            zero, STR_TRIM will not relocate the null terminator; it
            simply computes the trimmed length.
        <length>
            is the length, before trimming, of STRING.  If LENGTH is zero,
            STRING is assumed to be null-terminated.
        <trimmed_length>
            returns the length of STRING with trailing blanks, tabs, and
            new-line characters removed.

FILE/RECORD REFERENCES:

EXTERNAL ROUTINES: 

NOTES:


*******************************************************************************/

int  str_trim (string, length)
    char  *string ;
    int  length ;
{
    char  *s ;
    int  new_length ;

    new_length = length ;
    if (length == 0)  new_length = strlen (string) ;
    s = string + new_length ;

    while ((s-- != string) && ((isspace (*s) != 0) || (*s == '\n')))
        new_length-- ;

    if (length == 0)  *++s = '\0' ;

    return (new_length) ;

}

/****************************************************************************
FUNCTION:
	byte_swap
	
	reverse the order of bytes between DEC VAX byte-order convention and 
	other platform's byte-order convention, and vice versa

PURPOSE/DESCRIPTION:
	Function byte_swap reverse the order of bytes in the input unsigned
        character array.  For example, if an input array is a 32-bit word with
        the four 8-bit bytes ABCD, it will be re-ordered to DCBA on the 4-byte
        reversal, or it will be re-ordered to BADC on the 2-byte reversal.
        This function can be used to reverse the VAX byte order convention to
        regular byte order convention or vice versa.  It can be worked on
        either ASCII data or binary data.  The calling routine can convert the 
        returned character array to the proper values.
        
        NOTE that because on Unix system using function fread() to retrieve
        the binary data is not necessarily null-terminated, the out of bounds 
        (i.e., the total size to be reversed is greater than the size of 
        character array) condition in the input array can not be detected by 
        this function.
         
INVOCATION:
	(void) byte_swap (buffer, size, nitems);

   WHERE 
	<buffer> - I, O
	   is the input & output buffer which contains an array of 'unsigned 
	   char' data to be reversed. The reversed byte string is also stored 
	   in this array and returned to the calling routine.  In other words,
           the original string is replaced by the new one.
	<size> - I
	   size of data to be reversed, e.g., every 2, 3, 4, ... bytes. 
	<nitems> - I
	   total number of elements (not total bytes) to be reversed.  For 
	   example, if there are five words (each is 4 bytes) to be reversed, 
	   the nitems will be 5.

FILE/RECORD REFERENCES:	None.

EXTERNAL ROUTINES: None.

NOTES:
	Data size of 1 (e.g., char) is allowed, but meaningless in byte 
	reversal function.  The returned string will be the same as the 
	orginal input string.

*****************************************************************************/

void  byte_swap (buffer, size, nitems)
unsigned char  *buffer;
int  size, nitems;
{

    int  i, j, nbytes;
    unsigned char  *outbuf;
    
    
    outbuf = (unsigned char *) calloc (nitems*size, sizeof (unsigned char));
    
    for (i = 0, nbytes = 0; i < nitems; i++, nbytes = i*size)    {
        for (j = 0; j < size; j++)    {
            *(outbuf + nbytes + j) = buffer [nbytes + size - 1 - j];
        }
    }
    
    for (i = 0; i < (nitems * size); i++)    { /* Copy back to the buffer */
        buffer [i] = *(outbuf + i);
    }
    free (outbuf);
}


/*****************************************************************************
Function:

    month_day ()

Purpose:

    MONTH_DAY set month, and day from the elapsed number of days of the year. 

Invocation:

    status = month_day (year, num_days, pmonth, pday)

Where:
    <year>
        is the year the month and day to be calculated.
    <num_days>
        is the elapsed days of the year.
    <pmonth>
        returns the month of the argument year YEAR.
    <pday>
        returns the day of the month.
    <status>
        returns -1 if an error occurs; otherwise, a 0 is returned.

****************************************************************************/

int month_day (year, num_days, pmonth, pday)
int  year, num_days;
int  *pmonth, *pday;              /* Returned pointer to month, day of a year */
{

    int i, leap_year;
    static char daytab[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}   };


    leap_year = (dy_size (year) == 366);
    for (i = 1; num_days > daytab[leap_year][i]; i++)
        num_days -= daytab[leap_year][i];
    
    if (i > 12)   {
        printf ("Elapsed days (%d) is too big for the year (%d).\n", num_days, year);
        return (-1);
    }

    *pmonth = i;
    *pday = num_days;
    return (0);
}


/*****************************************************************************
Function:

    dy_size () 

Purpose:

    DY_SIZE returns number of days in the argument year (either 365, or 366 
    for learp year).  
    THIS FUNCTION IS THE SAME AS dysize() in SUN OS library function.  Some 
    Unix (e.g., HP-UX) system does not have this function in the library.  

Invocation:

    return_value = dy_size (year)

Where:
    <year>
        is the year the month and day to be calculated.
    <return_value>
        returns 365 for non-leap year, and 366 for leap year.

****************************************************************************/

int dy_size (year)
int  year;
{

    int leap_year;

    leap_year = (year % 4 == 0 && year % 100 != 0) ||  (year % 400 == 0);
    return ( (leap_year) ? 366 : 365);

}

/*****************************************************************************
Function:

    julian_day ()

Purpose:

    JULIAN_DAY returns the elapsed number of days of the year.  Since the 
    leap year has 29 days in February, the julian day will be different 
    after February 28 between leap year and non-leap year.  

Invocation:

    status = julian_day (year, month, day);

Where:
    <year>
        is the year the month and day to be calculated.
    <month>
        is the month of the argument YEAR.
    <day>
        is the day of the month.
    <status>
        returns the elapsed days of the year (julian day), or -1 if there is 
        an error.

FILE/RECORD REFERENCES: None.

EXTERNAL ROUTINES: None.

NOTES: None.

****************************************************************************/
int julian_day (year, month, day)
int  year;
int  month, day;              /* Returned pointer to month, day of a year */
{

    int i, leap_year, num_days=0;
    static char daytab[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}   };


    leap_year = (dy_size (year) == 366);
    if ((month > 12) || (day > daytab[leap_year][month])) {
        printf ("Month (=%d) and/or day (=%d) is wrong.\n", month, day);
        return (-1);
    }
    
    for (i = 1; i < month; i++)
        num_days += daytab[leap_year][i];
    
    return ((num_days+day));
}
/****************************************************************************
Function:

    interrupt_cat(): Interrupt catching routine

Purpose:

    INTERRUPT_CAT makes program exit gracefully when the interrupt signals 
    such as SIGTERM, or SIGINT is received. 
    
INVOCATION:

        status = interrupt_cat();

  WHERE 
        <status> - O
           returns -1 when the function completes execution.

FILE/RECORD REFERENCES: None.

EXTERNAL ROUTINES: None.

NOTES: None.
*****************************************************************************/

void  interrupt_cat()
{

    printf ("**** Interrupted **** \n");
    exit (-1);
}
