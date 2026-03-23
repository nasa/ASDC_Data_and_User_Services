C****************************************************************************
C* NCSA HDF                                                                 *
C* Software Development Group                                               *
C* National Center for Supercomputing Applications                          *
C* University of Illinois at Urbana-Champaign                               *
C* 605 E. Springfield, Champaign IL 61820                                   *
C*                                                                          *
C* For conditions of distribution and use, see the accompanying             *
C* hdf/COPYING file.                                                        *
C*                                                                          *
C****************************************************************************
C
C constant.i,v 1.4.4.1 1993/10/26 19:22:02 georgev Exp
C
C
C
C *-----------------------------------------------------------------------------
C * File: 	constant.f
C * Purpose:	Fortran header file for HDF routines
C * Contents: 
C *     Tag definitions
C *     Error return codes
C *    	Logical constants
C * Remarks: This file can be included with Fortran user programs.  As a
C *          general rule, don't use DFNT constants that don't include a
C *          number in their name.  E.g., don't use DFNT_FLOAT, use
C *          DFNT_FLOAT32 or DFNT_FLOAT64.  The DFNT constants that don't
C *          include numbers are for backward compatibility only.  Also,
C *          there are no current plans to support 128-bit number types.
C *          For more information about constants in this file, see the
C *          equivalent constant declarations in the C include file 'hdf.h'
C *--&-------------------------------------------------------------------XXXXXX*


C	Error Return Codes 

      integer 	DFE_NOERROR,   DFE_NONE,        DFE_FNF,
     *          DFE_DENIED,    DFE_ALROPEN,     DFE_TOOMANY,
     *          DFE_BADNAME,   DFE_BADACC,      DFE_BADOPEN,
     *		DFE_NOTOPEN,   DFE_CANTCLOSE,   DFE_DFNULL,  
     *		DFE_ILLTYPE,   DFE_UNSUPPORTED, DFE_BADDDLIST,
     *		DFE_NOTDFFILE, DFE_SEEDTWICE,   DFE_NOSPACE,
     *		DFE_NOSUCHTAG, DFE_READERROR

      parameter(DFE_NOERROR     	 =   0,
     *		DFE_NONE         	 =   0,
     *		DFE_FNF         	 =  -1,
     *		DFE_DENIED      	 =  -2,
     *		DFE_ALROPEN     	 =  -3,
     *		DFE_TOOMANY     	 =  -4,
     *		DFE_BADNAME     	 =  -5,
     *		DFE_BADACC      	 =  -6,
     *		DFE_BADOPEN      	 =  -7,
     *		DFE_NOTOPEN     	 =  -8,
     *		DFE_CANTCLOSE   	 =  -9,
     *		DFE_DFNULL        	 = -10,
     *		DFE_ILLTYPE     	 = -11,
     *		DFE_UNSUPPORTED 	 = -12,
     *		DFE_BADDDLIST   	 = -13,
     *		DFE_NOTDFFILE   	 = -14,
     *		DFE_SEEDTWICE   	 = -15,
     *		DFE_NOSPACE     	 = -16,
     *		DFE_NOSUCHTAG     	 = -17,
     *		DFE_READERROR   	 = -18)

      integer	DFE_WRITEERROR,  DFE_SEEKERROR,   DFE_NOFREEDD,
     *          DFE_BADTAG,      DFE_BADREF,      DFE_RDONLY,
     *          DFE_BADCALL,     DFE_BADPTR,      DFE_BADLEN,
     *          DFE_BADSEEK,     DFE_NOMATCH,     DFE_NOTINSET,
     *          DFE_BADDIM,      DFE_BADOFFSET,   DFE_BADSCHEME,
     *          DFE_NODIM,       DFE_NOTENOUGH,   DFE_NOVALS,
     *          DFE_CORRUPT,     DFE_BADFP
 
      parameter(DFE_WRITEERROR           = -19,
     *          DFE_SEEKERROR            = -20,
     *          DFE_NOFREEDD    	 = -21,
     *          DFE_BADTAG      	 = -22,
     *          DFE_BADREF      	 = -23,
     *          DFE_RDONLY      	 = -24,
     *          DFE_BADCALL     	 = -25,
     *          DFE_BADPTR      	 = -26,
     *          DFE_BADLEN      	 = -27,
     *          DFE_BADSEEK     	 = -28,
     *          DFE_NOMATCH     	 = -29,
     *          DFE_NOTINSET    	 = -30,
     *          DFE_BADDIM      	 = -31,
     *          DFE_BADOFFSET   	 = -32,
     *          DFE_BADSCHEME   	 = -33,
     *          DFE_NODIM       	 = -34,
     *          DFE_NOTENOUGH   	 = -35,
     *          DFE_NOVALS               = -36,
     *          DFE_CORRUPT              = -37,
     *          DFE_BADFP                = -38)

      integer	DFE_NOREF,       DFE_BADDATATYPE, DFE_BADMCTYPE,
     *          DFE_BADNUMTYPE,  DFE_BADORDER,    DFE_ARGS,
     *          DFE_INTERNAL,    DFE_DUPDD,       DFE_CANTMOD,
     *          DFE_RANGE,       DFE_BADTABLE,    DFE_BADSDG,
     *          DFE_BADNDG,      DFE_BADFIELDS,   DFE_NORESET,
     *          DFE_NOVS,        DFE_VGSIZE,      DFE_DIFFFILES,
     *          DFE_VTAB,        DFE_BADAID

      parameter(DFE_NOREF                = -39,
     *          DFE_BADDATATYPE          = -40,
     *          DFE_BADMCTYPE            = -41,
     *          DFE_BADNUMTYPE           = -42,
     *          DFE_BADORDER             = -43,
     *          DFE_ARGS                 = -44,
     *          DFE_INTERNAL             = -45,
     *          DFE_DUPDD                = -46,
     *          DFE_CANTMOD              = -47,
     *          DFE_RANGE                = -48,
     *          DFE_BADTABLE             = -49,
     *          DFE_BADSDG               = -50,
     *          DFE_BADNDG               = -51,
     *          DFE_BADFIELDS            = -52,
     *          DFE_NORESET              = -53,
     *          DFE_NOVS                 = -54,
     *          DFE_VGSIZE               = -55,
     *          DFE_DIFFFILES            = -56,
     *          DFE_VTAB                 = -57,
     *          DFE_BADAID               = -58)

      integer   DFE_OPENAID, DFE_BADCONV, DFE_GENAPP, DFE_CANTFLUSH,
     *          DFE_BADTYPE, DFE_SYMSIZE, DFE_BADATTACH, DFE_CANTDETACH

      parameter(DFE_OPENAID              = -59,
     *          DFE_BADCONV              = -60,
     *          DFE_GENAPP               = -61,
     *          DFE_CANTFLUSH            = -62,
     *          DFE_BADTYPE              = -63,
     *          DFE_SYMSIZE              = -64,
     *          DFE_BADATTACH            = -65,
     *          DFE_CANTDETACH           = -66)

C	Logical Constants

      integer 	DFACC_READ, DFACC_WRITE, DFACC_CREATE, DFACC_ALL  

      parameter(DFACC_READ       	 = 1,
     *		DFACC_WRITE      	 = 2,
     *		DFACC_CREATE     	 = 4,
     *		DFACC_ALL        	 = 7)

c	Constants for DFSDsetorder

      integer 	DFO_FORTRAN, DFO_C
     *		
 
      parameter(DFO_FORTRAN            = 1,
     *		DFO_C                  = 2)

c	Definitions of storage convention

      integer	DFNTF_IEEE, DFNTF_VAX, DFNTF_CRAY, DFNTF_PC,   
     *		DFNTF_CONVEX, DFNTF_VP
 
      parameter(DFNTF_IEEE             = 1,
     *		DFNTF_VAX              = 2,
     *		DFNTF_CRAY             = 3,
     *		DFNTF_PC               = 4,
     *		DFNTF_CONVEX           = 5,
     *		DFNTF_VP               = 6)

c       Masks for types

      integer   DFNT_HDF, DFNT_NATIVE, DFNT_CUSTOM

      parameter(DFNT_HDF                 = 0,
     *          DFNT_NATIVE              = 4096,
     *          DFNT_CUSTOM              = 8192,
     *          DFNT_LITEND              = 16384)

c	Number type info codes 

      integer	DFNT_NONE, DFNT_QUERY, DFNT_VERSION
 
      parameter(DFNT_NONE	= 0,
     *		DFNT_QUERY	= 0,
     *          DFNT_VERSION    = 1)

      integer	DFNT_FLOAT32, DFNT_FLOAT, DFNT_FLOAT64,
     *		DFNT_DOUBLE,  DFNT_FLOAT128

      parameter(DFNT_FLOAT32	= 5,
     *		DFNT_FLOAT	= 5,
     *		DFNT_FLOAT64	= 6,
     *		DFNT_DOUBLE	= 6,
     *		DFNT_FLOAT128	= 7)

      integer 	DFNT_INT8,  DFNT_UINT8, 
     *		DFNT_INT16, DFNT_UINT16,
     *		DFNT_INT32, DFNT_UINT32, 
     *      DFNT_INT64, DFNT_UINT64,
     *      DFNT_INT128,DFNT_UINT128
 
      parameter(DFNT_INT8	= 20,
     *		DFNT_UINT8	= 21,
     *		DFNT_INT16	= 22,
     *		DFNT_UINT16	= 23,
     *		DFNT_INT32	= 24,
     *		DFNT_UINT32	= 25,
     *		DFNT_INT64	= 26,
     *		DFNT_UINT64	= 27,
     *		DFNT_INT128	= 28,
     *		DFNT_UINT128	= 29)

      integer 	DFNT_UCHAR8, DFNT_UCHAR, DFNT_CHAR8, 
     *      DFNT_CHAR,   DFNT_CHAR16, DFNT_UCHAR16
 
      parameter(DFNT_UCHAR8	= 3,
     *		DFNT_UCHAR	= 3,
     *		DFNT_CHAR8	= 4,
     *		DFNT_CHAR	= 4,
     *		DFNT_CHAR16	= 42,
     *		DFNT_UCHAR16	= 43)

      integer	DFNT_NFLOAT32, DFNT_NFLOAT, DFNT_NFLOAT64,
     *		DFNT_NDOUBLE,  DFNT_NFLOAT128

      parameter(DFNT_NFLOAT32	= 4101,
     *		DFNT_NFLOAT	= 4101,
     *		DFNT_NFLOAT64	= 4102,
     *		DFNT_NDOUBLE	= 4102,
     *		DFNT_NFLOAT128	= 4103)

      integer 	DFNT_NINT8,  DFNT_NUINT8, 
     *		DFNT_NINT16, DFNT_NUINT16,
     *		DFNT_NINT32, DFNT_NUINT32, 
     *		DFNT_NINT64, DFNT_NUINT64,
     *		DFNT_NINT128,DFNT_NUINT128
 
      parameter(DFNT_NINT8	= 4116,
     *		DFNT_NUINT8	= 4117,
     *		DFNT_NINT16	= 4118,
     *		DFNT_NUINT16	= 4119,
     *		DFNT_NINT32	= 4120,
     *		DFNT_NUINT32	= 4121,
     *		DFNT_NINT64	= 4122,
     *		DFNT_NUINT64	= 4123,
     *		DFNT_NINT128	= 4124,
     *		DFNT_NUINT128	= 4125)

      integer 	DFNT_NUCHAR8, DFNT_NUCHAR, DFNT_NCHAR8, 
     *		DFNT_NCHAR,   DFNT_NCHAR16, DFNT_NUCHAR16
 
      parameter(DFNT_NUCHAR8	= 4099,
     *		DFNT_NUCHAR	= 4099,
     *		DFNT_NCHAR8	= 4100,
     *		DFNT_NCHAR	= 4100,
     *		DFNT_NCHAR16	= 4138,
     *		DFNT_NUCHAR16	= 4139)

      integer	DFNT_LFLOAT32, DFNT_LFLOAT, DFNT_LFLOAT64,
     *		DFNT_LDOUBLE,  DFNT_LFLOAT128

      parameter(DFNT_LFLOAT32	= 16389,
     *		DFNT_LFLOAT	= 16389,
     *		DFNT_LFLOAT64	= 16390,
     *		DFNT_LDOUBLE	= 16390,
     *		DFNT_LFLOAT128	= 16391)

      integer 	DFNT_LINT8,  DFNT_LUINT8, 
     *		DFNT_LINT16, DFNT_LUINT16,
     *		DFNT_LINT32, DFNT_LUINT32, 
     *		DFNT_LINT64, DFNT_LUINT64,
     *		DFNT_LINT128,DFNT_LUINT128
 
      parameter(DFNT_LINT8	= 16404,
     *		DFNT_LUINT8	= 16405,
     *		DFNT_LINT16	= 16406,
     *		DFNT_LUINT16	= 16407,
     *		DFNT_LINT32	= 16408,
     *		DFNT_LUINT32	= 16409,
     *		DFNT_LINT64	= 16410,
     *		DFNT_LUINT64	= 16411,
     *		DFNT_LINT128	= 16412,
     *		DFNT_LUINT128	= 16413)

      integer 	DFNT_LUCHAR8, DFNT_LUCHAR, DFNT_LCHAR8, 
     *		DFNT_LCHAR,   DFNT_LCHAR16, DFNT_LUCHAR16
 
      parameter(DFNT_LUCHAR8	= 16387,
     *		DFNT_LUCHAR	= 16387,
     *		DFNT_LCHAR8	= 16388,
     *		DFNT_LCHAR	= 16388,
     *		DFNT_LCHAR16	= 16426,
     *		DFNT_LUCHAR16	= 16427)

c	tags and refs

      integer 	DFREF_WILDCARD, DFTAG_WILDCARD, DFTAG_NULL,
     *		DFTAG_LINKED,   DFTAG_VERSION
 
      parameter(DFREF_WILDCARD	= 0,
     *		DFTAG_WILDCARD	= 0,
     *		DFTAG_NULL	= 1,
     *		DFTAG_LINKED	= 20,
     *		DFTAG_VERSION	= 30)

c	utility set

      integer	DFTAG_FID, DFTAG_FD,  DFTAG_TID, DFTAG_TD,
     *		DFTAG_DIL, DFTAG_DIA, DFTAG_NT,  DFTAG_MT
 
      parameter(DFTAG_FID	= 100,
     *		DFTAG_FD	= 101,
     *		DFTAG_TID	= 102,
     *		DFTAG_TD	= 103,
     *		DFTAG_DIL	= 104,
     *		DFTAG_DIA	= 105,
     *		DFTAG_NT	= 106,
     *		DFTAG_MT	= 107)

c 	raster-8 set 

      integer	DFTAG_ID8, DFTAG_IP8, DFTAG_RI8,
     *		DFTAG_CI8, DFTAG_II8
 
      parameter(DFTAG_ID8	= 200,
     *		DFTAG_IP8	= 201,
     *		DFTAG_RI8	= 202,
     *		DFTAG_CI8	= 203,
     *		DFTAG_II8	= 204)

c	Raster Image set

      integer	DFTAG_ID, DFTAG_LUT, DFTAG_RI, DFTAG_CI
 
      parameter(DFTAG_ID	= 300,
     *		DFTAG_LUT	= 301,
     *		DFTAG_RI	= 302,
     *		DFTAG_CI	= 303)

      integer	DFTAG_RIG, DFTAG_LD,  DFTAG_MD, DFTAG_MA,
     *		DFTAG_CCN, DFTAG_CFM, DFTAG_AR
  
      parameter(DFTAG_RIG	= 306,
     *		DFTAG_LD	= 307,
     *		DFTAG_MD	= 308,
     *		DFTAG_MA	= 309,
     *		DFTAG_CCN	= 310,
     *		DFTAG_CFM	= 311,
     *		DFTAG_AR	= 312)

      integer	DFTAG_DRAW, DFTAG_RUN, DFTAG_XYP, DFTAG_MTO
 
      parameter(DFTAG_DRAW	= 400,
     * 		DFTAG_RUN	= 401,
     *		DFTAG_XYP	= 500,
     *		DFTAG_MTO	= 501)

c	Tektronix 

      integer	DFTAG_T14, DFTAG_T105
 
      parameter(DFTAG_T14	= 602,
     *		DFTAG_T105	= 603)

c	Scientific Data set 

      integer	DFTAG_SDG, DFTAG_SDD, DFTAG_SDS, DFTAG_SDL,
     *      DFTAG_SDU, DFTAG_SDF, DFTAG_SDM, DFTAG_SDC,
     *		DFTAG_SDT,DFTAG_SDLNK,DFTAG_NDG, 
     *		DFTAG_BREQ,DFTAG_EREQ
 
      parameter(DFTAG_SDG	= 700,
     *		DFTAG_SDD	= 701,
     *		DFTAG_SD	= 702,
     *		DFTAG_SDS	= 703,
     *		DFTAG_SDL	= 704,
     *		DFTAG_SDU	= 705,
     *		DFTAG_SDF	= 706,
     *		DFTAG_SDM	= 707,
     *		DFTAG_SDC	= 708,
     *		DFTAG_SDT	= 709,
     *		DFTAG_SDLNK	= 710,
     *		DFTAG_NDG	= 720,
     *		DFTAG_BREQ	= 799,
     *		DFTAG_EREQ	= 780)

c	VSets 

      integer	DFTAG_VG, DFTAG_VH, DFTAG_VS
 
      parameter(DFTAG_VG     	= 1965,
     *		DFTAG_VH	= 1962,
     *		DFTAG_VS	= 1963)

c	compression schemes 

      integer	DFTAG_RLE, DFTAG_IMC, DFTAG_IMCOMP
 
      parameter(DFTAG_RLE   	=11,
     *		DFTAG_IMC   	=12,
     *		DFTAG_IMCOMP   	=12,
     *		DFTAG_JPEG   	=13,
     *		DFTAG_GREYJPEG 	=14)

c	SPECIAL CODES 

      integer	SPECIAL_LINKED, SPECIAL_EXT
 
      parameter(SPECIAL_LINKED 	= 1,
     *		SPECIAL_EXT 	= 2)

c	PARAMETERS 

      integer	DF_MAXFNLEN

      parameter(DF_MAXFNLEN	= 256)


C       Statndard return codes       
      integer	SUCCEED, FAIL
 
      parameter(SUCCEED         = 0,
     *		FAIL     	= -1)


c	Compression Types 

      integer	COMP_NONE, COMP_RLE, COMP_IMCOMP, COMP_JPEG

      parameter(COMP_NONE	= 0,
     *		COMP_RLE	= 11,
     *		COMP_IMCOMP	= 12,
     * 		COMP_JPEG	= 2)


c******************End of dfconstants.f***************************



