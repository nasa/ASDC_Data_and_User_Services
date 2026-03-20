/*
@(#) File name: fhdf_netcdf.c      Release: 1.1    Date: 11/18/94, 07:05:35
*/
/****************************************************************************
*                                                                           *
* FILE: fhdf_netcdf                                                         *
* Version: 1.0                                                              *
* AUTHOR:  Tom Feltman, Langley DAAC                                        *
*   note:                                                                   *
*   This program is heavily based upon software developed by the            *
*   University Corporation for Atmospheric Research and NCSA. Please        *
*   refer to your system's  netcdf/README file for copying and              *
*   redistribution conditions.                                              *
*   This program is provided as sample read software for netcdf formatted   *
*   science data sets.  It may require modifications to execute on your     *
*   system.  You can refer to your system's hdf/src/mfhdf/ncdump directory  *
*   for certain include files and modifications necessary for platform      *
*   specific requirements.                                                  *
* *                                                                           *
* PURPOSE/DESCRIPTION:                                                      *
*   The purpose of thios program is to produce a general read program*	    *
*   for users to access data sets written in NCSA's HDF netCDF format.      *
*   Our goal at Langley is to provide the user the ability to easily        *
*   determine the contents of these data files and to extract only the      *
*   data they are interested in.                                            *
*                                                                           *
* INVOCATION:                                                               *
*   This program is designed to be run interactively. Since there are a     *
*   well defined set of inputs, a skillfully designed shell script could    *
*   be used to control the program in a batch ebnvironment.                 * 
*   The following line will begin program execution:                        *
*                                                                           *
*         % fhdf_netcdf                                                     *
*                                                                           *
* INPUTS:                                                                   *
*   The program will request the name of the file which is to be read.  It  *
*   will open the file and display a list of variables (SDSs) to the user.  *
*   The user responds with the displayed ordinal of the variable(s) that    *
*   are to be extracted.  A -1 will display the next screen of variables    *
*   which are stored in the file.  A 0 (zero) indicates that the selection  *
*   process has been completed and the program should write the selected    *
*   to the output file.  The user is also given the choice of including the *
*   global variables in the report file.                                    *
*                                                                           *
* OUTPUTS:                                                                  *
*   Output is written to a file with same name as the input file, with an   *
*   ".rpt" suffix appended to the name.                                     *
*****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <netcdf.h>

extern char *progname;		/* for error messages */

#ifndef EXIT_FAILURE
#ifndef vms
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#else
#define EXIT_SUCCESS 1
#define EXIT_FAILURE 0
#endif
#endif

typedef enum {false=0, true=1} bool;

struct ncdim {			/* dimension */
    char name[MAX_NC_NAME];
    long size;
};

struct ncvar {			/* variable */
    char name[MAX_NC_NAME];
    nc_type type;
    int ndims;
    int dims[MAX_VAR_DIMS];
    int natts;
};

struct ncatt {			/* attribute */
    int var;
    char name[MAX_NC_NAME];
    nc_type type;
    int len;
    void *val;
};

typedef
enum {LANG_NONE, LANG_C, LANG_F} Nclang; 

struct fspec {			/* specification for how to format dump */
    char *name;			/*
				 * name specified with -n or derived from file
				 * name
				 */
    bool header_only;		/*
				 * if true, don't print any variable data
				 */
    bool coord_vals;		/*
				 * if true, print header and coordinate
				 * dimension values (values of variables that
				 * are also dimensions), but no other variable
				 * data
				 */
    bool brief_data_cmnts;	/*
				 * if true, put // comments in data section
				 * identifying variable and indices, useful for
				 * navigating through large multi-dimensional
				 * data lists.
				 */
    bool full_data_cmnts;	/*
				 * if true, put // comments in data section
				 * identifying every value, useful for
				 * navigating through large multi-dimensional
				 * data lists.
				 */
    Nclang data_lang;		/*
				 * Specifies index conventions used in data
				 * comments, either LANG_C (C, 0-based, column
				 * major) or LANG_F (Fortran, 1-based, row
				 * major)
				 */
    int nlvars;			/*
				 * Number of variables specified with -v option
				 * on command line
				 */
    char** lvars;		/*
				 * list of variable names specified with -v
				 * option on command line
				 */
};

#define NVARS 37

#ifndef NO_STDARG
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#undef PROTO
#ifndef NO_HAVE_PROTOTYPES 
#define	PROTO(x)	x
#else
#define	PROTO(x)	()
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Print error message to stderr, don't exit */
extern void	error		PROTO((
				       char *fmt,
                                       ...
				       ));

/* set position in line before lput() calls */
extern void	set_indent	PROTO((
				       int indent
				       ));

/* set maximum line length */
extern void	set_max_len	PROTO((
				       int len
				       ));

/* splits lines to keep them short */
extern void	lput		PROTO((
                                       FILE *out,
				       char *string
				       ));

/* In case different formats specified with -d option, set them here. */
extern void	set_formats	PROTO((
				       char *flt_fmt,
				       char *dbl_fmt
				       ));

/* Determine print format to use for each value for this variable. */
char *		get_fmt		PROTO((
				       int ncid,
				       int varid,
				       nc_type type,
                                       FILE *out
				       ));

/* structure for list of variables specified by user */
struct vnode
{
    struct vnode* next;
    int id;
};
typedef struct vnode vnode;

/* Get new variable list */
extern vnode*	newvlist	PROTO((
				       void
				       ));

/* Add a variable id to variable list */
extern void	varadd		PROTO((
				       vnode* vlist,
				       int varid
				       ));

/* Test if a variable id is in variable list */
extern int	varmember	PROTO((
				       vnode* vlist,
				       int varid
				       ));

#ifdef __cplusplus
}
#endif



/* Output the data for a single variable, in CDL syntax. */
extern int	vardata		PROTO((
				       struct ncvar*, /* variable */
				       long [], /* variable dimension sizes */
				       int, /* netcdf id */
				       int, /* variable id */
				       FILE * /* out put file handle */
				       ));

#ifdef __cplusplus
}
#endif


/*
 * Print error message to stderr, don't exit
 */
#ifndef NO_STDARG
void error(char *fmt, ...)
#else
/*VARARGS1*/
void error(fmt, va_alist)
     char *fmt ;
    va_dcl 
#endif
{
    va_list args ;

    (void) fprintf(stderr,"*** %s: ", progname);

#ifndef NO_STDARG
/*    va_start(args, fmt) ; */
#else
    va_start(args) ;
#endif
    (void) vfprintf(stderr,fmt,args) ;
    va_end(args) ;

    (void) fprintf(stderr, "\n") ;
    (void) fflush(stderr);	/* to ensure log files are current */
}

#define LINEPIND	"    "	/* indent of continued lines */

static int linep;
static int max_line_len;

void
set_indent(in)
     int in;
{
    linep = in;
}


void
set_max_len(len)
     int len;
{
    max_line_len = len-2;
}


void
lput(out,cp)
     char *cp;
     FILE *out;
{
    int nn = strlen(cp);

    if (nn+linep > max_line_len && nn > 2) {
	(void) fputs("\n", out);
	(void) fputs(LINEPIND, out);
	linep = strlen(LINEPIND);
    }
    (void) fputs(cp,out);
    linep += nn;
}


static char *formats[] =
{
    "%d",			/* bytes, shorts */
    "%s",			/* char arrays as strings */
    "%ld",			/* longs */
    "%.7g ",			/* floats */
    "%.15g"			/* doubles */
};


/* In case different formats specified with -d option, set them here. */
void
set_formats(flt, dbl)
     char *flt;
     char *dbl;
{
    strcpy(formats[3], flt);
    strcpy(formats[4], dbl);
}


static char *
has_c_format_att(ncid, varid)
    int ncid;			/* netcdf id */
    int varid;			/* variable id */
{
    nc_type cfmt_type;
    int cfmt_len;
    int savopts;
#define C_FMT_NAME	"C_format" /* name of C format attribute */
#define	MAX_CFMT_LEN	100	/* max length of C format attribute */
    static char cfmt[MAX_CFMT_LEN];
    
    /*
     * turn off error handling, we expect ncattinq to fail if there is no
     * C_format attribute
     */
    savopts = ncopts;
    ncopts = 0;
    if (ncattinq(ncid, varid, "C_format", &cfmt_type, &cfmt_len) != -1) {
	ncopts = savopts;	/* restore error handling */
	if (cfmt_type == NC_CHAR && cfmt_len > 0 && cfmt_len < MAX_CFMT_LEN) {
	    if (ncattget(ncid, varid, "C_format", (void *)cfmt) != -1)
	      return &cfmt[0];
	}
    }
    ncopts = savopts;		/* restore error handling */
    return 0;
}


/*
 * Determine print format to use for each value for this variable.  Use value
 * of attribute C_format if it exists, otherwise a sensible default.
 */
char *
get_fmt(ncid, varid, type,out)
     int ncid;			/* netcdf id */
     int varid;			/* variable id */
     nc_type type;		/* netCDF data type */
     FILE *out;                 /* output file */
{
    char *c_format_att = has_c_format_att(ncid, varid);

    /* If C_format attribute exists, return it */
    if (c_format_att)
      return c_format_att;    

    /* Otherwise return sensible default. */
    switch (type) {
      case NC_BYTE:
	return formats[0];
      case NC_CHAR:
	return formats[1];
      case NC_SHORT:
	return formats[0];
      case NC_LONG:
 	return formats[2];
      case NC_FLOAT:
	return formats[3];
      case NC_DOUBLE:
	return formats[4];
      default:
	error("pr_vals: bad type");
    }
}


static vnode *
newvnode()
{
    vnode *newvp = (vnode*) malloc(sizeof(vnode));
    
    if (!newvp) {
	error("out of memory!");
	exit(EXIT_FAILURE);
    }
    return newvp;
}


/*  simply get memory for another link to add to the list */
vnode*
newvlist()
{
    vnode *vp = newvnode();

    vp -> next = 0;
    vp -> id = -1;		/* bad id */

    return vp;
}


/*  add this to the list to print out */
void
varadd(vlist, varid)
     vnode* vlist;
     int varid;
{
    vnode *newvp = newvnode();
    
    newvp -> next = vlist -> next;
    newvp -> id = varid;
    vlist -> next = newvp;
}


/*  determine if this variable is one the user selected */
int
varmember(vlist, varid)
     vnode* vlist;
     int varid;
{
    vnode *vp = vlist -> next;

    for (; vp ; vp = vp->next)
      if (vp->id == varid)
	return 1;
    return 0;    
}

char *progname;

/*  find the "type" of variable we have so we can print in the  */
/*  proper format                                               */
static char *
type_name(type,out)
     nc_type type;
     FILE *out;
{
    switch (type) {
      case NC_BYTE:
	return "byte";
      case NC_CHAR:
	return "char";
      case NC_SHORT:
	return "short";
      case NC_LONG:
	return "long";
      case NC_FLOAT:
	return "float";
      case NC_DOUBLE:
	return "double";
      default:
        fprintf(out,"type_name: bad type %d", type);
	error("type_name: bad type %d", type);
	return "bogus";
    }
}

 
/*
 * Print list of attribute values.  Attribute values must be printed with
 * explicit type tags, because their types are not declared.
 */
static void
pr_att_vals(type, len, vals, nl,out)
     nc_type type;
     int len;
     void *vals;
     int nl;
     FILE *out;
{
    int iel;
    union {
	char *cp;
	short *sp;
	nclong *lp;
	float *fp;
	double *dp;
    } gp;
    char *sp;
    unsigned char uc;
    char gps[30];		/* for ascii of a float or double precision */
    char *f_fmt = "%#.8g";
    char *d_fmt = "%#.16g";

    nl += 3; /* add in 3 characters for the " = " leader */
    switch (type) {
      case NC_BYTE:
	gp.cp = (char *) vals;
	for (iel = 0; iel < len; iel++)
	  if (isprint(uc = *gp.cp++ & 0377)) {
	    fprintf (out,"'%c'%s", uc, iel<len-1 ? ", " : "");
	  } else {
	    fprintf (out,"'\\%o'%s", uc, iel<len-1 ? ", " : "");
	  }
	break;
      case NC_CHAR:
	gp.cp = (char *) vals;
	/* adjust len so trailing nulls don't get printed */
	sp = gp.cp + len - 1;
	while (*sp-- == '\0' && len > 0)
	    len--;
	for (iel = 0; iel < len; iel++) {
	  nl++;
	  switch (uc = *gp.cp++ & 0377) {
	    case '\b':
	      fprintf (out,"\\b");
	      break;
	    case '\f':
	      fprintf (out,"\\f");
	      break;
	    case '\n':		/* generate linebreaks after new-lines */
	      fprintf (out,"\\n,\n\t");
              nl = 8;
	      break;
	    case '\r':
	      fprintf (out,"\\r");
	      break;
	    case '\t':
	      fprintf (out,"\\t");
	      break;
	    case '\v':
	      fprintf (out,"\\v");
	      break;
	    case '\\':
	      fprintf (out,"\\\\");
	      break;
	    case '\'':
	      fprintf (out,"\\'");
	      break;
	    case '\"':
	      fprintf (out,"\\\"");
	      break;
	    default:
              if ((uc == ' ') && (nl >= 65))  {
                fprintf(out,"\n\t");
                nl = 8;
               }
               else
               if (((uc == '.') || (uc == ';') || (uc == '-') || (uc == '?')) 
                  && (nl >= 70)){
                 fprintf(out,"%c\n\t",uc);
                 nl = 8;
               }
               else
	      fprintf (out,"%c",uc);
	      break;
	  }
        }
	break;
      case NC_SHORT:
	gp.sp = (short *) vals;
	for (iel = 0; iel < len; iel++)
	  fprintf (out,"%ds%s",*gp.sp++,iel<len-1 ? ", " : "");
	break;
      case NC_LONG:
	gp.lp = (nclong *) vals;
	for (iel = 0; iel < len; iel++)
	  fprintf (out,"%ld%s",*gp.lp++,iel<len-1 ? ", " : "");
	break;
      case NC_FLOAT:
	gp.fp = (float *) vals;
	for (iel = 0; iel < len; iel++) {
	    int ll;
	    (void) sprintf(gps, f_fmt, * gp.fp++);
	    /* append a trailing "f" for floating-point attributes */
	    ll = strlen(gps);
	    gps[ll + 1] = '\0';
	    gps[ll] = 'f';
            nl += (ll + 1 + 2);
	    fprintf (out,"%s%s", gps, iel<len-1 ? ", " : "");
            if (nl >= 65) {
              fprintf(out,"\n\t    ");
              nl = 12;
            }
	}
	break;
      case NC_DOUBLE:
	gp.dp = (double *) vals;
	for (iel = 0; iel < len; iel++) {
	    int ll;
	    (void) sprintf(gps, d_fmt, *gp.dp++);
	    fprintf (out,"%s%s", gps, iel<len-1 ? ", " : "");
            ll = strlen(gps);
            nl += ll + 2;
            fprintf (out,"%s%s", gps, iel<len-1 ? ", " : "");
            if (nl >= 65) {
              fprintf(out,"\n\t    ");
              nl = 12;
            }
	}
	break;
      default:
	error("pr_att_vals: bad type");
	fprintf(out,"pr_att_vals: bad type");
    }
}


/****************************************************************************/
/*                                                                          */
/* PROGRAM:                                                                 */
/*	do_ncdump                                                           */
/*                                                                          */
/* PURPOSE/DESCRIPTION:                                                     */
/*   	Display variable selection list and call routines to write select   */
/*	variables to an output file.                                        */
/*                                                                          */
/* ALGORITHM:                                                               */
/*  	get number of dimensions, number of variables, number of global     */  
/*      atts, and dimension id of unlimited dimension                       */  
/*      If NVARS is too small, tell user size needed and abort              */
/*      Let user select variables to print into report file                 */
/*      Let user decide if they want to include the global attributes in    */
/*	report file.                                                        */
/* 	Print selected data and write to output file.                       */
/*                                                                          */
/* INVOCATION:                                                              */
/*   	do_ncdump(input_filename,output_file_stream)                        */
/*                                                                          */
/* 	Where:                                                              */
/*	     input_filename = name of file to read from                     */
/*	     output_file_stream = value returned from fopen request         */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

static void
do_ncdump(filename,out)
    char filename[132];              /* max filename */
    FILE *out;
{
    int ndims;			/* number of dimensions */
    int nvars;			/* number of variables */
    int ngatts;			/* number of global attributes */
    int xdimid;			/* id of unlimited dimension */
    int dimid;			/* dimension id */
    int varid;			/* variable id */
    int itemnumber;		/* selection of items to print out */
    struct ncdim dims[MAX_NC_DIMS]; /* dimensions */
    long vdims[MAX_NC_DIMS];	/* dimension sizes for a single variable */
    struct ncvar var[NVARS];		/* variable */
    struct ncatt att;		/* attribute */
    int id;			/* dimension number per variable */
    int ia;			/* attribute number */
    int iv;			/* variable number */
    int is_coord;		/* true if variable is a coordinate variable */
    int nlvars;			/* number of variables user selected */


    int ncid = ncopen(filename, NC_NOWRITE); /* netCDF id */
    char ans[80];
    vnode* vlist = newvlist();	/* list for vars elected by user */

    

    /* don't crash on error */
    ncopts = 0;

    if (ncid == -1) { 
        fprintf(out,"ncopen failed on %s", filename);
        error("ncopen failed on %s", filename);
        return;
    }
    /*
     * get number of dimensions, number of variables, number of global
     * atts, and dimension id of unlimited dimension, if any
     */
    (void) ncinquire(ncid, &ndims, &nvars, &ngatts, &xdimid);

          if (NVARS < nvars) {
fprintf(out,"NUMBER OF SDSs IN FILE = %d.  This should be less than or equal to %d.\n ",nvars,NVARS);
fprintf(out,"  If not please change NVARS from 37 to number shown and remake program\n");
      exit(1);
    }
    
    /* get dimension info */
    for (dimid = 0; dimid < ndims; dimid++) 
	(void) ncdiminq(ncid, dimid, dims[dimid].name, &dims[dimid].size);

    /* get variable info */
    for (varid = 0; varid < nvars; varid++) {
	(void) ncvarinq(ncid, varid, var[varid].name, &var[varid].type, 
        &var[varid].ndims, var[varid].dims, &var[varid].natts);
	/* get variable attributes */
	for (ia = 0; ia < var[varid].natts; ia++) {
	    (void) ncattname(ncid, varid, ia, att.name);
	    (void) ncattinq(ncid, varid, att.name, &att.type, &att.len);
	    att.val = (void *) malloc((unsigned)att.len*nctypelen(att.type));
	    if (!att.val) {
                fprintf(out,"ran out of memory trying to get variable attributes\n");
		error("Out of memory!");
		(void) ncclose(ncid);
		return;
	    }
	    (void) ncattget(ncid, varid, att.name, att.val);
	    free ((char *) att.val);
	}

    }

/********  let user select variables to print out ************/
        printf("\n  Item      Data Type     Label(dimension lengths)\n");
        printf("  ====      =========     ========================\n\n");

        fprintf(out,"Original data file contained the following data:\n\n");
        fprintf(out,"\n  Item      Data Type     Label(dimension lengths)\n");
        fprintf(out,"  ====      =========     ========================\n\n");
        for (varid = 0; varid < nvars; varid++) {
          printf("  %3d      %8s       %s(", 
                  varid+1,type_name(var[varid].type),var[varid].name);
          fprintf(out,"  %3d      %8s       %s(", 
                  varid+1,type_name(var[varid].type),var[varid].name);
          for (id = 0; id < var[varid].ndims; id++) {
              printf("%d%s",dims[var[varid].dims[id]].size,id<var[varid].ndims - 1 ? " X " : ")\n");
              fprintf(out,"%d%s",dims[var[varid].dims[id]].size,id<var[varid].ndims - 1 ? " X " : ")\n");
          }
          if (((varid != 0) && (((varid+1)%15) == 0)) || varid == (nvars - 1)) {
            printf("\nPlease choose the numbers of the data items you wish to dump\n");
            if (varid == (nvars - 1)){
               printf("Enter -1 or zero to terminate selection process\n");
            } else {
            printf("Enter -1 to display additional items or zero to end selection process\n\n"); }
            if (varid == 14)
            printf("NOTE: if you enter 0 as very first entry, all variables will be displayed\n\n");
            while (itemnumber != 0 || itemnumber != -1)
         {
            /* read the user's selection */
 
            scanf("%d", &itemnumber);
            /* if user's selection equals -1, then */
            /* display 15 more parameters          */
 
            if (itemnumber == -1) {
               if (varid != (nvars - 1)){
                 printf("\n  Item      Data Type     Label(dimension lengths)\n");
                 printf("  ====      =========     ========================\n\n");
               }
	       break;
            }
            if (itemnumber == 0) {
               varid = NVARS;
               break;
            }
            if ((itemnumber > NVARS +1) || (itemnumber < -1)) {
              printf("bad selection - try again\n");
            } else {
              varadd(vlist,itemnumber-1);
              nlvars++;
            } /* end of adding to print list */


          } /* end of while */
 
         } /* end of if */
        } /* end of for */

    /* get global attributes */
    ans[0] = ' ';
    while ( (ans[0] != 'Y') && (ans[0] != 'y') && (ans[0] != 'N') && (ans[0] != 'n') ) {
    printf("Would you like the global attributes included in the report? (Y/N)");
    scanf("%s",ans);
    printf("\n");
    }
    if (( ans[0] != 'N') && (ans[0] != 'n')) { 
    fprintf(out,
      "\n****************** START GLOBAL ATTRIBUTE SECTION ******************\n\n");
    for (ia = 0; ia < ngatts; ia++) {
	(void) ncattname(ncid, NC_GLOBAL, ia, att.name);
	
	fprintf (out,"%s = ", att.name);
	
	(void) ncattinq(ncid, NC_GLOBAL, att.name, &att.type, &att.len);
	att.val = (void *) malloc((unsigned) (att.len*nctypelen(att.type)));
	if (!att.val) {
           fprintf(out,"ran out of memory trying to get global attributes\n");
	    error("Out of memory!");
	    (void) ncclose(ncid);
	    return;
	}
	(void) ncattget(ncid, NC_GLOBAL, att.name, att.val);

	pr_att_vals(att.type, att.len, att.val, strlen(att.name),out);
	fprintf (out,"\n");
	free ((char *) att.val);
    } /* end of printing attributes  values */
    fprintf(out,
      "\n****************** END GLOBAL ATTRIBUTE SECTION ******************\n\n");
    }
/*dummy*/
	                                               /* output variable data */
	for (varid = 0; varid < nvars; varid++) 
          {

       	                                              /* if var list specified, test for membership */
	    if (nlvars > 0 && ! varmember(vlist, varid))
	      continue;
	    (void) ncvarinq(ncid, varid, var[varid].name, &(var[varid].type),
                      &(var[varid].ndims), var[varid].dims, &(var[varid].natts));
	    /*
	     * Only get data for variable if it is not a record variable,
	     * or if it is a record variable and at least one record has
	     * been written.
	     */
	    if (var[varid].ndims == 0 || var[varid].dims[0] != xdimid
		                      || dims[xdimid].size != 0)
              {
	           	                     /* Collect variable's dim sizes */
		for (id = 0; id < var[varid].ndims; id++)
                  {
		    vdims[id] = dims[var[varid].dims[id]].size;

                  }

		 if (vardata(&var[varid], vdims, ncid, varid,out) == -1)
                   {
		     /*error("can't output data for variable %s", var[varid].name)/*;
		     /*fprintf(out,"can't output data for variable %s", var[varid].name)/*;
		     /*(void) ncclose(ncid);*/
                   /*   return;*/
		   }
	      }
         }
    fprintf (out,"\n");
    (void) ncclose(ncid);
}

#define	STREQ(a, b)	(*(a) == *(b) && strcmp((a), (b)) == 0)

/*
 * Print a row of variable values.  Makes sure output lines aren't too long
 * by judiciously inserting newlines. 
 */
static void
pr_vals(vp, len, fmt, more, lastrow, vals, out)
     struct ncvar *vp;		/* variable */
     long len;			/* number of values to print */
     char *fmt;			/*
				 * printf format used for each value.  If
				 * nc_type is NC_CHAR and this is NULL,
				 * character arrays will be printed as strings
				 * enclosed in quotes.
				 */
     bool more;			/*
				 * true if more data will follow, so add
				 * trailing comma
				 */
     bool lastrow;		/*
				 * true if this is the last row for this
				 * variable, so terminate with ";" instead of
				 * ","
				 */
     void *vals;		/* pointer to block of values */
     FILE *out;			/* report file */
{
    long iel;
    union {
	char *cp;
	short *sp;
	nclong *lp;
	float *fp;
	double *dp;
    } gp;
    char *sp;
    unsigned char uc;
    float fill_float;
    double fill_double;
    char sout[100];		/* temporary string for each encoded output */

    fill_float = FILL_FLOAT;	/* static initialization hits ultrix cc bug */
    fill_double = FILL_DOUBLE;
    switch (vp->type) {
      case NC_BYTE:
	gp.cp = (char *) vals;
	for (iel = 0; iel < len-1; iel++) {
	    (void) sprintf(sout, fmt, *gp.cp++);
	    (void) strcat(sout, ", ");
	    lput(out, sout);
	}
		(void) sprintf(sout, fmt, *gp.cp++);
		lput(out, sout);
		break;
	      case NC_CHAR:
		gp.cp = (char *) vals;
	if (fmt == 0 || STREQ(fmt,"%s") || STREQ(fmt,"")) { /* as string */
	    fprintf(out,"\n");
	    /* adjust len so trailing nulls don't get printed */
	    sp = gp.cp + len;
	    while (len > 0 && *--sp == '\0')

	      len--;
	    for (iel = 0; iel < len; iel++) {
              if (iel%80 == 0) {
                fprintf(out,"\n");
              }
	      switch (uc = *gp.cp++ & 0377) {
		case '\b':
		  fprintf(out,"\\b");
		  break;
		case '\f':
		  fprintf(out,"\\f");
		  break;
		case '\n':	/* generate linebreaks after new-lines */
		  fprintf(out,"\\n\",\n    \"");
		  break;
		case '\r':
		  fprintf(out,"\\r");
		  break;
		case '\t':
		  fprintf(out,"\\t");
		  break;
		case '\v':
		  fprintf(out,"\\v");
		  break;
		case '\\':
		  fprintf(out,"\\\\");
		  break;
		case '\'':
		  fprintf(out,"\\\'");
		  break;
		case '\"':
		  fprintf(out,"\\\"");
		  break;
		default:
		  if (isprint(uc)) {
		    fprintf(out,"%c",uc);
                  }
		  else
		  {
                    fprintf(out,"\\%.3o",uc);
                  }
		  break;
                }
	      }
	} else {		/* use format from C_format attribute */
	    for (iel = 0; iel < len-1; iel++) {
		(void) sprintf(sout, fmt, *gp.cp++);
		(void) strcat(sout, ", ");
		lput(out, sout);
	    }
	    (void) sprintf(sout, fmt, *gp.cp++);
	    lput(out, sout);
	}
	break;
      case NC_SHORT:
	gp.sp = (short *) vals;
	for (iel = 0; iel < len-1; iel++) {
	    (void) sprintf(sout, fmt, *gp.sp++);
	    (void) strcat(sout, ", ");
	    lput(out, sout);
	}
	(void) sprintf(sout, fmt, *gp.sp++);
	lput(out, sout);
	break;
      case NC_LONG:
	gp.lp = (nclong *) vals;
	for (iel = 0; iel < len-1; iel++) {
	    (void) sprintf(sout, fmt, *gp.lp++);
	    (void) strcat(sout, ", ");
	    lput(out, sout);
	}
	(void) sprintf(sout, fmt, *gp.lp++);
	lput(out, sout);
	break;
      case NC_FLOAT:
	gp.fp = (float *) vals;
	for (iel = 0; iel < len-1; iel++) {
	    if (*gp.fp >= fill_float)
	      (void) sprintf(sout, "FloatInf, ");
	    else {
		(void) sprintf(sout, fmt, *gp.fp);
		(void) strcat(sout, ", ");
	    }
	    lput(out, sout);
	    gp.fp++;
	}
	if (*gp.fp >= fill_float)
	  (void) sprintf(sout, "FloatInf");
	else
	  (void) sprintf(sout, fmt, *gp.fp);
	lput(out, sout);
	gp.fp++;
	break;
      case NC_DOUBLE:
	gp.dp = (double *) vals;
	for (iel = 0; iel < len-1; iel++) {
	    if (*gp.dp >= fill_double)
	      (void) sprintf(sout, "DoubleInf, ");
	    else {
		(void) sprintf(sout, fmt, *gp.dp);
		(void) strcat(sout, ", ");
	    }
	    lput(out, sout);
	    gp.dp++;
	}
	if (*gp.dp >= fill_double)
	  (void) sprintf(sout, "DoubleInf");
	else
	  (void) sprintf(sout, fmt, *gp.dp);
	lput(out, sout);
	gp.dp++;
	break;
      default:
        fprintf(out,"pr_vals: bad type");
	error("pr_vals: bad type");
    }
    if (more) {
	lput(out, ", ");
    } else {
	if(lastrow) {
	    lput(out, " ;");
	    lput(out, "\n");
	} else {
	    lput(out, ",\n");
	    lput(out, "  ");
	}
    }
}


/*
 * print last delimiter in each line before annotation (, or ;)
 */
static void
lastdelim (more, lastrow, out)
     bool more;
     bool lastrow;
     FILE *out;
{
    if (more) {
	fprintf(out,", ");
    } else {
	if(lastrow) {
	    fprintf(out,";");
	} else {
	    fprintf(out,",");
	}
    }
}


/*
 * Updates a vector of ints, odometer style.  Returns 0 if odometer
 * overflowed, else 1.
 */
static int
upcorner(dims,ndims,odom,add)
     long *dims;		/* The "odometer" limits for each dimension  */
     int ndims;			/* Number of dimensions */
     long* odom;		/* The "odometer" vector to be updated */
     long* add;			/* A vector to "add" to odom on each update */
{
    int id;
    int ret = 1;

    for (id = ndims-1; id > 0; id--) {
	odom[id] += add[id];
	if(odom[id] >= dims[id]) {
	    odom[id-1]++;
	    odom[id] -= dims[id];
	}
    }
    odom[0] += add[0];
    if (odom[0] >= dims[0])
      ret = 0;
    return ret;
}


int
vardata(vp, vdims, ncid, varid, out)
     struct ncvar *vp;		/* variable */
     long vdims[];		/* variable dimension sizes */
     int ncid;			/* netcdf id */
     int varid;			/* variable id */
     FILE *out;			/* report file */
{
    long cor[MAX_VAR_DIMS];	/* corner coordinates */
    long edg[MAX_VAR_DIMS];	/* edges of hypercube */
    long add[MAX_VAR_DIMS];	/* "odometer" increment to next "row"  */
#define VALBUFSIZ 8192
    double vals[VALBUFSIZ/sizeof(double)] ; /* aligned buffer */
    int gulp = VALBUFSIZ/nctypelen(vp->type);

    int id;
    int ia;
    int ir;
    long nels;
    long ncols;
    long nrows;
    struct ncatt att;		/* attribute info for variable */
    int vrank = vp->ndims;

    /* printf format used to print each value */
    char *fmt = get_fmt(ncid, varid, vp->type, out);

    nels = 1;
    for (id = 0; id < vrank; id++) {
	cor[id] = 0;
	edg[id] = 1;
	nels *= vdims[id];	/* total number of values for variable */
    }

	fprintf(out,"\n %s \n  ", vp->name);
        /* get variable attributes */
        for (ia = 0; ia < vp->natts; ia++) {
            (void) ncattname(ncid, varid, ia, att.name);
 
            fprintf (out,"\t%s = ", att.name);
 
            (void) ncattinq(ncid, varid, att.name, &att.type, &att.len);
 
            att.val = (void *) malloc((unsigned)att.len*nctypelen(att.type));
            if (!att.val) {
                fprintf(out,"vardata: Out of memory trying to get attribute values!");
                error("Out of memory!");
                (void) ncclose(ncid);
                return;
            }
            (void) ncattget(ncid, varid, att.name, att.val);
            pr_att_vals(att.type, att.len, att.val, strlen(att.name)+8, out);
            fprintf (out,"\n");
            free ((char *) att.val);
        }

    if (vrank < 1) {
	ncols = 1;
    } else {
	ncols = vdims[vrank-1];	/* size of "row" along last dimension */
	edg[vrank-1] = vdims[vrank-1];
	for (id = 0; id < vrank; id++)
	  add[id] = 0;
	if (vrank > 1)
	  add[vrank-2] = 1;
    }
    nrows = nels/ncols;		/* number of "rows" */
    
    for (ir = 0; ir < nrows; ir++) {
	/*
	 * rather than just printing a whole row at once (which might exceed
	 * the capacity of MSDOS platforms, for example), we break each row
	 * into smaller chunks, if necessary.
	 */
	long corsav;
	long left = ncols;
	bool lastrow;

	lastrow = (ir == nrows-1);
	while (left > 0) {
	    long toget = left < gulp ? left : gulp;
	    if (vrank > 0)
	      edg[vrank-1] = toget;
	    (void) ncvarget (ncid, varid, cor, edg, (void *) vals);
	      pr_vals(vp, toget, fmt, left > toget, lastrow, (void *) vals, out); 
	    left -= toget;
	    if (vrank > 0) {
	      cor[vrank-1] += toget;
            }
	}
	if (vrank > 0)
	  cor[vrank-1] = corsav;
	if (ir < nrows-1)
	  if (!upcorner(vdims,vp->ndims,cor,add)) {
	    fprintf(out,"vardata: odometer overflowed!");
	    error("vardata: odometer overflowed!");
          }
        fprintf(out,"\n");
	set_indent(2);
    }

    return 0;
}
/**************************************************************/
/*                                                            */
/* PROGRAM:                                                   */
/*        fread_netCDF.c                                      */
/*                                                            */
/* PURPOSE/DESCRIPTION:                                       */
/* 	This program will read a user specified netCDF        */
/*	formatted files and allow a user to select the        */
/*	data variables that should be extracted and written   */
/*      to a report file.                                     */
/*                                                            */
/* ALGORITHM:                                                 */
/*	The program requests a user to input the name of a    */
/*      netCDF data file.  An output file name is generated   */
/*      by appending ".rpt" to the input file's name. A       */
/*      routine do_ncdump is then called to display a list    */
/* 	of variables that the file contains.  The user then   */
/*      selects the variables they want to have written to    */
/* 	the report file.  Users are also given the option of  */
/*	including the "global variables" in the output        */
/*      report.  The report is then written.                  */
/*                                                            */
/* INVOCATION:                                                */
/*      fread_netCDF                                          */
/*                                                            */
/* FILE/RECORD REFERENCES:                                    */
/*        granule_name    -       input file                  */
/*        granule_name.rpt-       output file                 */
/*                                                            */
/* EXTERNAL ROUTINES:                                         */
/*	Assorted HDF netCDF library routines.                 */
/*                                                            */
/* NOTES:  This program was designed as sample read software  */
/*	   for specific data sets.  However, it may be used   */
/*         to read most netCDF formatted files.  The user may */
/*         need to adjust the define statement for  NVARS     */
/*         to accomodate files which contain more than 32     */
/*         data variables.  In those cases the program will   */
/* 	   remind the user to change the NVARS to a value     */
/*         necessary to safely execute the program.           */
/*                                                            */
/**************************************************************/
int
main(argc, argv)
int argc;
char *argv[];
{
    extern int optind;
    extern int opterr;
    extern char *optarg;
    int c;
    int i;
    int max_len = 80;		/* default maximum line length */
    char filename[132];		/* full file name */
    char ans[80];
    char outfile[132];          /* report file name */
    FILE *out;

    opterr = 1;
    progname = argv[0];


    set_max_len(max_len);
    
    argc -= optind;
    argv += optind;

    i = 0;


    printf("This program reads the FIRE_ASTEX and FIRE_MAS netCDF data.\n");
    printf("  Please enter Q to Quit or C to continue ");
    scanf("%s", ans);
 
    if (ans[0] == 'Q' || ans[0] == 'q')       /* the user chose to 'quit' */
     {
       printf("\n\nExiting Program ...\n");
       exit(1);
     }/* end of if */
    printf("\n\tPlease enter netCDF file name to read: \n");

    scanf("%s",filename);
    strcpy(outfile,filename);
    strcat(outfile,".rpt");
    /* open output file */
 
    if ((out = fopen(outfile,"wt")) == NULL){
      printf("\n!! Cannot open output file");
      exit(1);
    }
    fprintf(out," This is the report file for the %s file\n\n",filename);
    printf("Report file name is: %s\n",outfile);
	  do_ncdump(filename,out);
    return EXIT_SUCCESS;
}

