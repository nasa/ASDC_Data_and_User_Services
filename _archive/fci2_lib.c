/* 
@(#) File name: fci2_lib.c	Release: 1.1	Date: 1/13/94, 08:39:14
*/

/****************************************************************************
PROGRAM:
	fci2_lib

PURPOSE/DESCRIPTION:
	Program fci2_lib has a collection of library routines that are 
	used by more than one read routines for the FIRE Cirrus II data 
	sets.  This program contains four libray functions: str_trim(), 
	hd_sondes(), julian_day(), and interrupt_cat().

INTERNAL ROUTINES: 
	str_trim - trims the trailing blanks, tabs, and the new-line 
	    characters from a string.  The function returns the length
	    of the new trimmed string.
	hd_sondes - prints the header and the column headings for the class 
	    sondes, NWS_KS sondes, and the NWS_out_KS sondes data files.  
	    The output were sent to the standard output device.
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
static char sccsid[] = "File: fci2_lib.c  Release: 1.1  Date: 1/13/94, 08:39:14"
#endif

#include  <stdlib.h>
#include  <stdio.h>
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

    while ((s-- != string) &&
           ((*s == ' ') || (*s == '\t') || (*s == '\n')))
        new_length-- ;

    if (length == 0)  *++s = '\0' ;

    return (new_length) ;

}


/*****************************************************************************
Function:

    hd_sondes ()

Purpose:

    HD_SONDES prints out the header and the column headings to the screen.  
    This function is used by the class sondes, NWS_KS sondes, and the 
    NWS_out_KS sondes data files. 

Invocation:

    (void) hd_sondes (fname, nline, line_ptr, ncol, col_ptr);
    
Where:
    <fname>
        is the sondes class or sondes NWS_KS or sondes NWS_out_KS file name 
        to be processed.
    <nline>
        is the number of lines in the header section of the data file.
    <line_ptr>
        is the address of the starting header lines.
    <ncol>
        is the number of the fields.
    <col_ptr>
        is the address of the starting field headings.

****************************************************************************/


void  hd_sondes (fname, nline, line_ptr, ncol, col_ptr)
char  *fname;
char  **line_ptr, **col_ptr;
int  nline, ncol;
{

    int  i;
    
    
    printf ("\fFile name: %s\n\n", fname);
    
    for (i = 0; i < nline; i++) 
        printf ("%s\n", line_ptr[i]);
    
    for (i = 0; i < ncol; i++) 
        printf ("%s\n", col_ptr[i]);
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
