C**********************************************************************C
C**********************************************************************C
C**********************************************************************C
C* DXREAD MODULE:  SUBROUTINES TO READ DX DATA FROM A DX FILE         *C
C*--------------------------------------------------------------------*C
C* CONTENTS:  SUBROUTINE DRIVER   SAMPLE MAIN PROGRAM FOR DXREAD      *C
C*            SUBROUTINE DXTABS   SET UP DX UNPACKING TABLES          *C
C*            SUBROUTINE DXOPEN   OPEN A DX FILE                      *C
C*            SUBROUTINE DXREAD   READ A DX PIXEL                     *C
C*            BLOCK DATA          DATA FOR CONVERSION TABLES          *C
C*                                                                    *C
C* NOTE 1:  CPU TIME CAN BE DECREASED BY COMMENTING OUT THE PARTS     *C
C* OF DXREAD THAT UNPACK VARIABLES THAT YOU ARE NOT INTERESTED IN.    *C
C*
C* SGI users:
C*      change recl=lrecl to recl=lrecl/4 in open statement
C* DEC users 
C*      change recl=lrecl to recl=lrecl/4 in open statement
C*      add convert='little_endian' to open statement
C*      change each occurance of chars(4) to chars(1)
C**********************************************************************C
C**********************************************************************C
C**********************************************************************C
C*                                                                      
C*              S U B R O U T I N E   D R I V E R                       
C*                                                                    *C
C*--------------------------------------------------------------------*C
C* THIS IS A SAMPLE MAIN PROGRAM ILLUSTRATING THE USE OF THE DXTABS,  *C
C* DXOPEN AND DXREAD ROUTINES.  FIRST CALL DXTABS TO SET UP THE       *C
C* TABLES USED TO UNPACK BIT-PACKED DATA.  THEN CALL DXOPEN TO OPEN   *C
C* THE FILE AND READ THE HEADER INFORMATION.  HEADER VARIABLES THAT   *C
C* IDENTIFY THE IMAGE ARE RETURNED IN THE COMMON BLOCK "HEADER".      *C
C* TO READ THE IMAGE DATA, LOOP OVER PIXELS                           *C
C* AND CALL DXREAD TO UNPACK ONE PIXEL AT A TIME.  THE DATA VALUES    *C
C* FOR THE PIXEL ARE RETURNED IN THE COMMON BLOCK "DXDATA" AS         *C
C* INTEGERS.  FOR PHYSICAL VALUES, APPLY THE APPROPRIATE CONVERSION   *C
C* TO THE INTEGER COUNT VALUE.                                        *C
C* THE MAPPED LOCATION, AND LATITUDE/LONGITUDE OF THE PIXEL ARE       *C
C* RETURNED IN COMMON BLOCK "LOCATE".  CONTINUE CALLING DXREAD        *C
C* UNTIL THE END-OF-IMAGE CONDITION IS REACHED.                       *C
C**********************************************************************C
C**********************************************************************C
C**********************************************************************C
cc      SUBROUTINE DRIVER                                                 
C*--------------------------------------------------------------------*C
C* COMMON BLOCK "HEADER" CONTENTS                                       
C*--------------------------------------------------------------------*C
C* YEAR    YEAR (1983-PRESENT)                                          
C* MONTH   MONTH (1-12)                                                 
C* DAY     DAY (1-31)                                                   
C* GMT     GMT (00,03,06,09,12,15,18,21)                                
C* SATID   CODE NUMBER THAT IDENTIFIES THE SATELLITE (1-99)             
C* SATTYP  CODE NUMBER INDICATING SATELLITE TYPE (0-8), -1 IS SUBSET    
C* NCHANS  NUMBER OF CHANNELS PRESENT (1-5)                             
C* NITIMG  IMAGE LEVEL DAY/NIGHT FLAG (0-1), 1=ENTIRE IMAGE IS NIGHT    
C*--------------------------------------------------------------------*C
      COMMON /HEADER/ YEAR,MONTH,DAY,GMT,SATID,SATTYP,NCHANS,NITIMG     
     $               ,B3VERS                                            
      CHARACTER*8     B3VERS                                            
      INTEGER         YEAR,MONTH,DAY,GMT,SATID,SATTYP,NCHANS,NITIMG     
C*--------------------------------------------------------------------*C
C* COMMON BLOCK "LOCATE" CONTENTS                                       
C*--------------------------------------------------------------------*C
C* LON     LONGITUDE (0-3600)                                           
C* LAT     LATITUDE (0-1800)                                            
C* IPOS    I-POSITION ON MAP PROJECTION (1-1440)                        
C* JPOS    J-POSITION ON MAP PROJECTION (1-550)                         
C*--------------------------------------------------------------------*C
      COMMON /LOCATE/ LON,LAT,IPOS,JPOS                                 
      INTEGER         LON,LAT,IPOS,JPOS                                 
C*--------------------------------------------------------------------*C
C* COMMON BLOCK "DXDATA" CONTENTS (UNDEFINED=255, CT=COUNTS)            
C*--------------------------------------------------------------------*C
C* LNDWTR  LAND/WATER FLAG (0-1):  1 = WATER PIXEL                      
C* HITOPO  TOPOGRAPHY FLAG (0-1):  1 = HIGH TOPOGRAPHY PIXEL            
C* SNOICE  SNOW/ICE CODE (0-3)                                          
C*               OCEAN PIXEL       LAND PIXEL                           
C*            0  NONE              NONE                                 
C*            1  PARTIAL ICE       LAND ICE                             
C*            2  FULL ICE          FULL SNOW                            
C*            3  ICE MARGIN        MARGIN                               
C* MUE     COSINE OF SATELLITE ANGLE * 100 (0-100)                      
C* IRAD    IR RADIANCE (0-254 CT)                                       
C* ARAD(3) UP TO 3 ADDITIONAL CHANNEL RADIANCES (0-254 CT)              
C* GLINT   GLINT FLAG (0-1):  1 = GLINT CONDITION EXISTS                
C* MU0     COSINE OF SUN ANGLE * 100 (0-100)                            
C* PHI     RELATIVE AZIMUTH ANGLE (0-180 DEG)                           
C* VRAD    VIS RADIANCE (0-254 CT)                                      
C* DAYNIT  DAY/NIGHT FLAG (0-1):  1 = NIGHT PIXEL (NO VIS RETRIEVALS)   
C* ITHR    IR THRESHOLD RESULT (0-5)                                    
C* VTHR    VIS THRESHOLD RESULT (0-5)                                   
C* SHORE   SHORE FLAG (0-1):  1 = SHORE PIXEL                           
C* ICSRAD  IR CLEAR SKY COMPOSITE RADIANCE (0-254 CT)                   
C* ITMP    IR RETRIEVED CLOUD TOP OR SURFACE TEMPERATURE (0-254 CT)     
C* IPRS    IR RETRIEVED CLOUD TOP PRESSURE (0-254 CT)                   
C* ICSTMP  IR RETRIEVED CLEAR SKY COMPOSITE TEMPERATURE (0-254 CT)      
C* ICSPRS  IR RETRIEVED CLEAR SKY COMPOSITE PRESSURE (0-254 CT)         
C* NREF    NI REFLECTIVITY (NOAA CHANNEL 3) (0-254 CT)                  
C* NTHR    NI THRESHOLD RESULT (NOAA CHANNEL 3) (0-15)                  
C* NCSREF  NI CLEAR SKY COMPOSITE REFLECTANCE (NOAA CHANNEL 3)(0-254 CT)
C* VCSRAD  VIS CLEAR SKY COMPOSITE RADIANCE (0-254 CT)                  
C* VALBTA  VIS RETRIEVED CLOUD TAU OR SURFACE ALBEDO (0-254 CT)         
C* VCSALB  VIS RETRIEVED CLEAR SKY COMPOSITE ALBEDO (0-254 CT)          
C* VTMP    VIS ADJUSTED CLOUD TOP TEMPERATURE (0-254 CT)                
C* VPRS    VIS ADJUSTED CLOUD TOP PRESSURE (0-254 CT)                   
C* VTAUIC  VIS RETRIEVED ICE CLOUD TAU (0-254 CT)                       
C* VTMPIC  VIS RETRIEVED ICE CLOUD TOP TEMPERATURE (0-254 CT)           
C* VPRSIC  VIS RETRIEVED ICE CLOUD TOP PRESSURE (0-254 CT)              
C* CLOUD   CLOUD FLAG (0-1):  1 = CLOUDY PIXEL                          
C*--------------------------------------------------------------------*C
      COMMON /DXDATA/ LNDWTR,HITOPO,SNOICE,MUE,IRAD                     
     $     ,ARAD,GLINT,MU0,PHI,VRAD,DAYNIT,ITHR,VTHR,SHORE              
     $     ,ICSRAD,ITMP,IPRS,ICSTMP,ICSPRS,NREF,NTHR                    
     $     ,NCSREF,VCSRAD,VALBTA,VCSALB,VTMP,VPRS,VTAUIC                
     $     ,VTMPIC,VPRSIC,CLOUD                                         
      INTEGER         LNDWTR,HITOPO,SNOICE,MUE,IRAD                     
     $     ,ARAD(3),GLINT,MU0,PHI,VRAD,DAYNIT,ITHR,VTHR,SHORE           
     $     ,ICSRAD,ITMP,IPRS,ICSTMP,ICSPRS,NREF,NTHR                    
     $     ,NCSREF,VCSRAD,VALBTA,VCSALB,VTMP,VPRS,VTAUIC                
     $     ,VTMPIC,VPRSIC,CLOUD                                         
C*--------------------------------------------------------------------*C
C* FUNCTION DECLARATIONS                                                
C*--------------------------------------------------------------------*C
      INTEGER DXOPEN, DXREAD                                            
C*--------------------------------------------------------------------*C
C* TABLES TO CONVERT FROM INTEGER COUNTS TO PHYSICAL VALUES             
C*--------------------------------------------------------------------*C
      PARAMETER     ( MAXCNT = 255 )                                    
      COMMON /CNTTAB/ TMPTAB(0:MAXCNT),PRETAB(0:MAXCNT)                 
     $               ,RFLTAB(0:MAXCNT),TAUTAB(0:MAXCNT)                 
C???????????                                                            
C* DX PIXEL DATA - INTERNAL USE ONLY                                    
      COMMON /DXHIDE/ NODAY,BXSHOR,TIMSPA,ICSLOG,BXITHR,BXICSR          
     $     ,VCSLOG,BXVTHR,BXVCSR,IRET,ICSRET,VRET,VCSRET,NDXBYT,DXBYT   
      INTEGER         NODAY,BXSHOR,TIMSPA,ICSLOG,BXITHR,BXICSR          
     $     ,VCSLOG,BXVTHR,BXVCSR,IRET,ICSRET,VRET,VCSRET,NDXBYT         
      PARAMETER     ( MAXDX = 32 )                                      
      CHARACTER*1     DXBYT(MAXDX)                                      
C???????????                                                            
C*--------------------------------------------------------------------*C
C* INIT                                                               *C
C*--------------------------------------------------------------------*C
      LUNDX = 10                                                        
      CALL DXTABS                                                       
C*--------------------------------------------------------------------*C
C* OPEN THE FILE                                                      *C
C*--------------------------------------------------------------------*C
      IRC = DXOPEN(LUNDX)                                               
      IF ( IRC .NE. 0 ) GOTO 90                                         
      PRINT 9,YEAR,MONTH,DAY,GMT,SATID,SATTYP,NCHANS,NITIMG             
    9 FORMAT(1X,'HEADER:  YEAR',I5,' MONTH',I3,' DAY',I3,' GMT',I3,     
     $       ' SATID',I3,' SATTYP',I2,' NCHANS',I2,' NITIMG',I3)        
      IF ( SATTYP .LT. 0 ) THEN                                         
         PRINT *,'THIS IS A SUBSET FILE, IPOS AND JPOS NOT USABLE'      
      END IF                                                            
C*--------------------------------------------------------------------*C
C* LOOP OVER PIXELS                                                   *C
C*--------------------------------------------------------------------*C
      ICTPIX = 0                                                        
   20 CONTINUE                                                          
      IRC = DXREAD(LUNDX)                                               
      IF ( IRC .NE. 0 ) GOTO 90                                         
      ICTPIX = ICTPIX + 1                                               
C*--------------------------------------------------------------------*C
C* PRINT SOME VALUES                                                  *C
C*--------------------------------------------------------------------*C
C     PRINT *,LON,LAT,IPOS,JPOS                                         
      IF ( DAYNIT .EQ. 0 ) THEN                                         
         IF ( MU0 .EQ. 255 ) PRINT *,'BAD',IPOS,JPOS,MU0                
      END IF                                                            
      IF ( IPOS .EQ. 340.AND. JPOS .EQ. 161) THEN                       
         PRINT *,'NDXBYT',NDXBYT,(ICHAR(DXBYT(I)),I=1,NDXBYT)           
         PRINT *,'LON LAT IPOS JPOS',LON,LAT,IPOS,JPOS                  
         PRINT *,'1  ',NODAY,BXSHOR,LNDWTR,HITOPO,SNOICE,TIMSPA         
         PRINT *,'2  ',ICSLOG,BXITHR                                    
         PRINT *,'3  ',MUE                                              
         PRINT *,'4  ',IRAD,TMPTAB(IRAD)                                
         PRINT *,'5  ',BXICSR,TMPTAB(BXICSR)                            
         PRINT *,'6-8',ARAD                                             
         PRINT *,'9  ',GLINT,VCSLOG,BXVTHR                              
         PRINT *,'10 ',MU0                                              
         PRINT *,'11 ',PHI                                              
         PRINT *,'12 ',VRAD,RFLTAB(VRAD)                                
         PRINT *,'13 ',BXVCSR,RFLTAB(BXVCSR)                            
         PRINT *,'14 ',DAYNIT,ITHR,VTHR,SHORE                           
         PRINT *,'15 ',IRET,ICSRET                                      
         PRINT *,'16 ',ICSRAD,TMPTAB(ICSRAD)                            
         PRINT *,'17 ',ITMP,TMPTAB(ITMP)                                
         PRINT *,'18 ',IPRS,PRETAB(IPRS)                                
         PRINT *,'19 ',ICSTMP,TMPTAB(ICSTMP)                            
         PRINT *,'20 ',ICSPRS,PRETAB(ICSPRS)                            
         PRINT *,'21 ',NREF,RFLTAB(NREF)                                
         PRINT *,'22 ',NTHR                                             
         PRINT *,'23 ',NCSREF,RFLTAB(NCSREF)                            
         PRINT *,'24 ',VRET,VCSRET                                      
         PRINT *,'25 ',VCSRAD,RFLTAB(VCSRAD)                            
         PRINT *,'26 ',VALBTA,RFLTAB(VALBTA),TAUTAB(VALBTA)             
         PRINT *,'27 ',VCSALB,RFLTAB(VCSALB)                            
         PRINT *,'28 ',VTMP,TMPTAB(VTMP)                                
         PRINT *,'29 ',VPRS,PRETAB(VPRS)                                
         PRINT *,'30 ',VTAUIC,TAUTAB(VTAUIC)                            
         PRINT *,'31 ',VTMPIC,TMPTAB(VTMPIC)                            
         PRINT *,'32 ',VPRSIC,PRETAB(VPRSIC)                            
         PRINT *,'CLD',CLOUD                                            
      END IF                                                            
C*--------------------------------------------------------------------*C
C* END OF PIXEL LOOP                                                  *C
C*--------------------------------------------------------------------*C
      GOTO 20                                                           
C*--------------------------------------------------------------------*C
C* END OF FILE                                                        *C
C*--------------------------------------------------------------------*C
   90 CONTINUE                                                          
      PRINT *,'ICTPIX=',ICTPIX                                          
      STOP                                                              
      END                                                               
C**********************************************************************C
C**********************************************************************C
C**********************************************************************C
C*                                                                    *C
C*            S U B R O U T I N E   D X T A B S                       *C
C*                                                                    *C
C*--------------------------------------------------------------------*C
C* DESCRIPTION:                                                       *C
C*    THIS SUBROUTINE SETS UP LOOK-UP TABLES USED FOR UNPACKING THE   *C
C*    DATA.  IT IS CALLED ONLY ONCE, AT THE BEGINNING OF EXECUTION.   *C
C*    THE TABLES TAKE AN INTEGER INDEX AND RETURN SELECTED BITS AS    *C
C*    AN INTEGER VALUE.                                               *C
C* ARGUMENTS:                                                         *C
C*    NONE                                                            *C
C* RETURN CODES:                                                      *C
C*    NONE                                                            *C
C**********************************************************************C
C**********************************************************************C
C**********************************************************************C
      SUBROUTINE DXTABS                                                 
      COMMON /DXTAB/  NITTAB,GLNTAB,SHRTAB,LNDTAB,TOPTAB,CLSTAB,SNITAB  
     $     ,IFLTAB,THRTAB,ANGTAB,VFLTAB,ITHRTB,VTHRTB,DRETAB,CRETAB     
     $     ,NSHTAB                                                      
      INTEGER         NITTAB(0:255),GLNTAB(0:255),SHRTAB(0:255)         
     $     ,LNDTAB(0:255),TOPTAB(0:255),CLSTAB(0:255),SNITAB(0:255)     
     $     ,IFLTAB(0:255),THRTAB(0:255),ANGTAB(0:255),VFLTAB(0:255)     
     $     ,ITHRTB(0:255),VTHRTB(0:255),DRETAB(0:255),CRETAB(0:255)     
     $     ,NSHTAB(0:255)                                               
      DO 200 IVAL = 0,255                                               
         NITTAB(IVAL) = IAND(IVAL,128) / 128                            
         GLNTAB(IVAL) = NITTAB(IVAL)                                    
         SHRTAB(IVAL) = IAND(IVAL,64) / 64                              
         LNDTAB(IVAL) = IAND(IVAL,32) / 32                              
         TOPTAB(IVAL) = IAND(IVAL,16) / 16                              
         CLSTAB(IVAL) = IAND(IVAL,2+1)                                  
         SNITAB(IVAL) = IAND(IVAL,8+4) / 4                              
         IFLTAB(IVAL) = IAND(IVAL,128+64+32+16+8) / 8                   
         THRTAB(IVAL) = IAND(IVAL,4+2+1)                                
         ANGTAB(IVAL) = IAND(IVAL,64+32+16+8+4+2+1)                     
         VFLTAB(IVAL) = IAND(IVAL,64+32+16+8) / 8                       
         ITHRTB(IVAL) = IAND(IVAL,64+32+16) / 16                        
         VTHRTB(IVAL) = IAND(IVAL,8+4+2) / 2                            
         DRETAB(IVAL) = IAND(IVAL,128+64+32+16) / 16                    
         CRETAB(IVAL) = IAND(IVAL,8+4+2+1)                              
         NSHTAB(IVAL) = IAND(IVAL,1)                                    
  200 CONTINUE                                                          
      RETURN                                                            
      END                                                               
C**********************************************************************C
C**********************************************************************C
C**********************************************************************C
C*                                                                    *C
C*                F U N C T I O N   D X O P E N                       *C
C*                                                                    *C
C*--------------------------------------------------------------------*C
C* DESCRIPTION:                                                       *C
C*    THIS FUNCTION OPENS A DX FILE, READS AND UNPACKS THE IMAGE      *C
C*    HEADER, AND INITIALIZES IMAGE VARIABLES.  IT IS CALLED ONCE     *C
C*    FOR EACH IMAGE.                                                 *C
C* ARGUMENTS:                                                         *C
C*    LUNIN    FORTRAN LOGICAL UNIT ATTACHED TO DX FILE               *C
C* RETURN CODES:                                                      *C
C*    DXOPEN   0 = SUCCESS                                            *C
C*            -1 = FAILURE                                            *C
C*--------------------------------------------------------------------*C
C* THE FIRST RECORD IN A DX IMAGE FILE IS THE HEADER RECORD.          *C
C* THE HEADER RECORD CONTAINS 384 80-CHARACTER LINES OF ASCII TEXT.   *C
C*                                                                    *C
C* THE FIRST 80-CHAR LINE CONTAINS THE FOLLOWING:                     *C
C*                                                                    *C
C*    HDLINE(1) = YEAR,MONTH,DAY,GMT,SATID,SATTYP,NCHANS,NITIMG       *C
C*                    YEAR,MONTH,DAY = image date                     *C
C*                    GMT = image GMT (0,3,6,9,12,15,18,21)           *C
C*                    SATID = code number to identify satellite       *C
C*                    SATTYP = 0 Geo-stationary, 1-4 Polar (S,N.A,D)  *C
C*                    NCHANS = number of channels present             *C
C*                    NITIMG = 1 if image is entirely night-time      *C
C*                                                                    *C
C* THE REST OF LINES CONTAIN THE FOLLOWING:                           *C
C*                                                                    *C
C*    HDLINE(2)       = Production run date                           *C
C*    HDLINE(3)       = Production run time                           *C
C*    HDLINE(4)       = Input (B3) sequence and version (SSSS.V)      *C
C*    HDLINE(5-100)   = Blank                                         *C
C*    HDLINE(101-135) = Title,units,format for the 39 DXDATA variables*C
C*    HDLINE(136-384) = Blank                                         *C
C**********************************************************************C
C**********************************************************************C
C**********************************************************************C
      INTEGER FUNCTION DXOPEN(LUNIN)                                    
      PARAMETER     ( LRECL = 30720 )                                   
C* IMAGE HEADER                                                         
      COMMON /HEADER/ YEAR,MONTH,DAY,GMT,SATID,SATTYP,NCHANS,NITIMG     
     $               ,B3VERS                                            
      CHARACTER*8     B3VERS                                            
      INTEGER         YEAR,MONTH,DAY,GMT,SATID,SATTYP,NCHANS,NITIMG     
C* DX IMAGE HEADER                                                      
      COMMON /DXHEAD/ HDLINE                                            
      CHARACTER*80    HDLINE(384),EBCLIN                                
C* DX FILE INFO                                                         
      COMMON /DXFILE/ IREC,IPIX,IWEST,IEAST,INORTH,ISOUTH,NPIX,IOUT     
     $               ,INPBYT,BXADDL                                     
      INTEGER         IREC,IWEST,IEAST,INORTH,ISOUTH,NPIX,IOUT          
     $               ,INPBYT,BXADDL                                     
C*--------------------------------------------------------------------*C
C* READ DX IMAGE HEADER                                               *C
C*--------------------------------------------------------------------*C
      IREC = 1                                                          
C* CMS/MVS BEGIN                                                        
c     READ(LUNIN,IOSTAT=IRC) HDLINE                                     
c     DO 100 I=1,384                                                    
c        CALL EBCDIC(HDLINE(I),80,EBCLIN)                               
c        HDLINE(I) = EBCLIN                                             
C        PRINT *,HDLINE(I)                                              
c 100 CONTINUE                                                          
C* CMS/MVS END                                                          

C* UNIX/AIX BEGIN                                                       

C IBM
      OPEN(UNIT=LUNIN,ACCESS='DIRECT',RECL=LRECL,FORM='UNFORMATTED')    
C SGI 
C     OPEN(UNIT=LUNIN,ACCESS='DIRECT',RECL=LRECL/4,FORM='UNFORMATTED')    

C DEC 
C     OPEN(UNIT=LUNIN,ACCESS='DIRECT',RECL=LRECL/4,FORM='UNFORMATTED',
C    $     CONVERT=LITTLE_ENDIAN')    
C     also must change each occurance of chars(4) to chars(1)

      READ(LUNIN,REC=IREC,IOSTAT=IRC) HDLINE                            
C* UNIX/AIX END                                                         
      IF ( IRC .NE. 0 ) GOTO 900                                        
C*--------------------------------------------------------------------*C
C* DECODE IMAGE HEADER LINE 1                                         *C
C*--------------------------------------------------------------------*C
      READ(UNIT=HDLINE(1),FMT='(8I10)') YEAR,MONTH,DAY,GMT,SATID        
     $     ,SATTYP,NCHANS,NITIMG                                        
      BXADDL = NCHANS - 2                                               
      B3VERS = HDLINE(4)(1:8)                                           
C*--------------------------------------------------------------------*C
C* INITIALIZE RECORD VARIABLES                                        *C
C*--------------------------------------------------------------------*C
      NPIX = 0                                                          
      IPIX = 0                                                          
      IOUT = 0                                                          
      INPBYT = 0                                                        
      IWEST = 0                                                         
      IEAST = 0                                                         
      INORTH = 0                                                        
      ISOUTH = 0                                                        
C*--------------------------------------------------------------------*C
C* RETURN SUCCESS                                                     *C
C*--------------------------------------------------------------------*C
      DXOPEN = 0                                                        
      RETURN                                                            
C*--------------------------------------------------------------------*C
C* RETURN FAILURE                                                     *C
C*--------------------------------------------------------------------*C
  900 CONTINUE                                                          
      PRINT *,'IMAGE DOES NOT EXIST'                                    
      DXOPEN = -1                                                       
      RETURN                                                            
      END                                                               
C**********************************************************************C
C**********************************************************************C
C**********************************************************************C
C*                                                                    *C
C*                F U N C T I O N   D X R E A D                       *C
C*                                                                    *C
C*--------------------------------------------------------------------*C
C* DESCRIPTION:                                                       *C
C*    THIS FUNCTION UNPACKS DX DATA RECORDS ONE PIXEL AT A TIME,      *C
C*    READING ADDITIONAL RECORDS WHEN NECESSARY.  IT IS CALLED ONCE   *C
C*    FOR EACH PIXEL.  TO READ THE ENTIRE IMAGE, IT IS CALLED         *C
C*    REPEATEDLY, UNTIL THE END-OF-IMAGE CONDITION IS REACHED.        *C
C* ARGUMENTS:                                                         *C
C*    LUNIN    FORTRAN LOGICAL UNIT ATTACHED TO DX FILE               *C
C* RETURN CODES:                                                      *C
C*    DXREAD   0 = SUCCESS                                            *C
C*            -1 = REACHED END OF IMAGE                               *C
C*--------------------------------------------------------------------*C
C* THERE ARE A VARIABLE NUMBER OF DATA RECORDS IN EACH DX IMAGE.      *C
C* EACH RECORD CONTAINS PACKED BINARY PIXEL DATA AS FOLLOWS:          *C
C*    I*4   IWEST   Western-most longitude (0-3600 degrees*10)          
C*    I*4   IEAST   Eastern-most longitude (0-3600 degrees*10)          
C*    I*4   INORTH  Northern-most latitude (0-1800 degrees*10)          
C*    I*4   ISOUTH  Southern-most latitude (0-1800 degrees*10)          
C*    I*4   NPIX    Number of pixels                                    
C*    I*4   IOUT    Number of bytes in packed data BUFFER               
C*    I*2   LONBUF(NPIX) Longitudes for NPIX pixels (0-3600 degrees*10) 
C*    I*2   LATBUF(NPIX) Latitudes for NPIX pixels (0-1800 degrees*10)  
C*    I*2   XBUF(NPIX)  X-positions for NPIX pixels (1-1440)            
C*    I*2   YBUF(NPIX)  Y-positions for NPIX pixels (1-550)             
C*    I*1   BUFFER(IOUT) Packed data for NPIX pixels                    
C*    remainder of record is filled with I*1 255 values.                
C*                                                                      
C* THE CONTENTS OF THE BUFFER AS IT IS UNPACKED ARE AS FOLLOWS:         
C*                                                                      
C* NOTE 1:  Numbers on the left identify the byte number from 1 to 37.  
C* This is the MAXIMUM number of bytes that a pixel can have.  Not all  
C* bytes are present for all pixels.                                    
C*                                                                      
C* NOTE 2:  Variables marked with "*" are part of the data set but are n
C* returned in common block DXDATA because they are for internal use    
C* only.  They are kept in common block DXHIDE instead.                 
C*                                                                      
C* DATA SECTION 1 - pixel ident. satellite angle, IR cloud detection    
C* 1. NODAY,BXSHOR,LNDWTR,HITOPO,SNOICE,TIMSPA (N,B,L,H,SS,TT)          
C*     *NODAY  : BX day/night flag (0-1), 1 = day sect. 2,4 not present 
C*     *BXSHOR : BX shore flag (0-1), 1 = shore pixel                   
C*      LNDWTR : Land/water flag (0-1), 1 = water pixel                 
C*      HITOPO : Topography flag (0-1), 1 = high topography pixel       
C*      SNOICE : Snow/ice code (0-3)                                    
C*     *TIMSPA : Time/space test result (0-3)                           
C* 2. ICSLOG,BXITHR (IIIII,BBB)                                         
C*     *ICSLOG : IR clear sky composite logic code (0-24)               
C*     *BXITHR : BX IR threshold result ()-5)                           
C* 3. MUE       : Cosine of satellite angle * 100 (0-100)               
C* 4. IRAD      : IR radiance (0-254 ct)                                
C* 5.*BXICSR    : BX IR clear sky radiance (0-254 ct)                   
C* If image header says additional chans are present (BXADDL>0) then:   
C* 6. ARAD(1)   : First additional channel radiance (0-254 ct)          
C* 7. ARAD(2)   : Second additional channel radiance (0-254 ct)         
C* 8. ARAD(3)   : Third additional channel radiance (0-254 ct)          
C*                                                                      
C* DATA SECTION 2 - sun angles, VIS cloud detection                     
C* if NODAY = 0 (day sections present) then:                            
C* 9. GLINT,VCSLOG,BXVTHR (G,VVVV,BBB)                                  
C*       GLINT  : Glint flag (0-1), 1 = glint condition exists          
C*      *VCSLOG : VIS clear sky composite logic code (0-14)             
C*      *BXVTHR : BX VIS threshold result                               
C*10. MU0       : Cosine of sun angle * 100 (0-100)                     
C*11. PHI       : Relative azimuth angle (0-180 degrees)                
C*12. VRAD      : VIS radiance (0-254 ct)                               
C*13.*BXVCSR    : BX VIS clear sky radiance (0-254 ct)                  
C*                                                                      
C* DATA SECTION 3 - adjusted cloud detections, IR retrievals,           
C*                  additional channel cloud detections                 
C*14. DAYNIT,ITHR,VTHR,SHORE (D,III,VVV,S)                              
C*       DAYNIT : Day/night flag (0-1), 1 = night pixel (no VIS retrieva
C*       ITHR   : IR threshold result (0-5)                             
C*       VTHR   : VIS threshold result (0-5)                            
C*       SHORE  : Shore flag (0-1), 1 = shore pixel                     
C*15. IRET,ICSRET (IIII,IIII)                                           
C*      *IRET   : IR retrieval code (0-12)                              
C*      *ICSRET : IR clear sky composite retrieval code (0-12)          
C*16. ICSRAD    : IR clear sky composite radiance (0-254 ct)            
C*17. ITMP      : IR retrieved cloud top or surface temperature (0-254 c
C*18. IPRS      : IR retrieved cloud top pressure (0-254 ct)            
C*19. ICSTMP    : IR retrieved clear sky composite temperature (0-254 ct
C*20. ICSPRS    : IR retrieved clear sky composite pressure (0-254 ct)  
C* if image header says is polar (SATTYP = 1,2,or -3) then:             
C*21. NREF      : NI reflectivity (NOAA channel 3) (0-254 ct)           
C*22. NTHR      : NI threshold result (NOAA channel 3) (0-254 ct)       
C*23. NCSREF    : NI clear sky composite reflectance (NOAA channel 3) (0
C*                                                                      
C* DATA SECTION 4 - VIS retrievals                                      
C* if NODAY = 0 (day sections present) then:                            
C* if DAYNIT = 1 (night, no VIS retrievals) these will all be 255       
C*24. VRET,VCSRET (VVVV,VVVV)                                           
C*      *VRET   : VIS retrieval code (0-14)                             
C*      *VCSRET : VIS clear sky composite retrieval code (0-14)         
C*25. VCSRAD    : VIS clear sky composite radiance (0-254 ct)           
C*26. VALBTA    : VIS retrieved cloud tau or surface albedo (0-254 ct)  
C*27. VCSALB    : VIS retrieved clear sky composite albedo (0-254 ct)   
C*28. VTMP      : VIS adjusted cloud top temperature (0-254 ct)         
C*29. VPRS      : VIS adjusted cloud top pressure (0-254 ct)            
C*30. VTAUIC    : VIS retrieved ice cloud tau (0-254 ct)                
C*31. VTMPIC    : VIS retrieved ice cloud top temperature (0-254 ct)    
C*32. VPRSIC    : VIS retrieved ice cloud top pressure (0-254 ct)       
C**********************************************************************C
C**********************************************************************C
C**********************************************************************C
      INTEGER FUNCTION DXREAD(LUNIN)                                    
C* DX BIT UNPACKING TABLES                                              
      COMMON /DXTAB/  NITTAB,GLNTAB,SHRTAB,LNDTAB,TOPTAB,CLSTAB,SNITAB  
     $     ,IFLTAB,THRTAB,ANGTAB,VFLTAB,ITHRTB,VTHRTB,DRETAB,CRETAB     
     $     ,NSHTAB                                                      
      INTEGER         NITTAB(0:255),GLNTAB(0:255),SHRTAB(0:255)         
     $     ,LNDTAB(0:255),TOPTAB(0:255),CLSTAB(0:255),SNITAB(0:255)     
     $     ,IFLTAB(0:255),THRTAB(0:255),ANGTAB(0:255),VFLTAB(0:255)     
     $     ,ITHRTB(0:255),VTHRTB(0:255),DRETAB(0:255),CRETAB(0:255)     
     $     ,NSHTAB(0:255)                                               
C* IMAGE HEADER                                                         
      COMMON /HEADER/ YEAR,MONTH,DAY,GMT,SATID,SATTYP,NCHANS,NITIMG     
     $               ,B3VERS                                            
      CHARACTER*8     B3VERS                                            
      INTEGER         YEAR,MONTH,DAY,GMT,SATID,SATTYP,NCHANS,NITIMG     
C* DX FILE INFO                                                         
      COMMON /DXFILE/ IREC,IPIX,IWEST,IEAST,INORTH,ISOUTH,NPIX,IOUT     
     $               ,INPBYT,BXADDL                                     
      INTEGER         IREC,IWEST,IEAST,INORTH,ISOUTH,NPIX,IOUT          
     $               ,INPBYT,BXADDL                                     
C* PIXEL LOCATION INFORMATION                                           
      COMMON /LOCATE/ LON,LAT,IPOS,JPOS                                 
      INTEGER         LON,LAT,IPOS,JPOS                                 
C* PIXEL DATA                                                           
      COMMON /DXDATA/ LNDWTR,HITOPO,SNOICE,MUE,IRAD                     
     $     ,ARAD,GLINT,MU0,PHI,VRAD,DAYNIT,ITHR,VTHR,SHORE              
     $     ,ICSRAD,ITMP,IPRS,ICSTMP,ICSPRS,NREF,NTHR                    
     $     ,NCSREF,VCSRAD,VALBTA,VCSALB,VTMP,VPRS,VTAUIC                
     $     ,VTMPIC,VPRSIC,CLOUD                                         
      INTEGER         LNDWTR,HITOPO,SNOICE,MUE,IRAD                     
     $     ,ARAD(3),GLINT,MU0,PHI,VRAD,DAYNIT,ITHR,VTHR,SHORE           
     $     ,ICSRAD,ITMP,IPRS,ICSTMP,ICSPRS,NREF,NTHR                    
     $     ,NCSREF,VCSRAD,VALBTA,VCSALB,VTMP,VPRS,VTAUIC                
     $     ,VTMPIC,VPRSIC,CLOUD                                         
C* DX PIXEL DATA - INTERNAL USE ONLY                                    
      COMMON /DXHIDE/ NODAY,BXSHOR,TIMSPA,ICSLOG,BXITHR,BXICSR          
     $     ,VCSLOG,BXVTHR,BXVCSR,IRET,ICSRET,VRET,VCSRET,NDXBYT,DXBYT   
      INTEGER         NODAY,BXSHOR,TIMSPA,ICSLOG,BXITHR,BXICSR          
     $     ,VCSLOG,BXVTHR,BXVCSR,IRET,ICSRET,VRET,VCSRET,NDXBYT         
      PARAMETER     ( MAXDX = 32 )                                      
      CHARACTER*1     DXBYT(MAXDX)                                      
C?????????????                                                          
C* DIAGNOSTIC COUNTERS                                                  
      INTEGER NDCNT(0:1)                                                
      INTEGER ND2CNT(0:1)                                               
C?????????????                                                          
C* INPUT BUFFER FOR ONE DATA RECORD                                     
      PARAMETER     ( MAXBYT = 30720 - 24 )                             
      CHARACTER*1     BUFFER(MAXBYT)                                    
      PARAMETER     ( MAXPIX = 1535 )                                   
      INTEGER*2       LATBUF(MAXPIX),LONBUF(MAXPIX)                     
      INTEGER*2       XBUF(MAXPIX),YBUF(MAXPIX)                         
C* CONVERSION TO INTEGER                                                
      INTEGER*4       ICHAR                                             
      CHARACTER*1     CHARS(4)                                          
      EQUIVALENCE   ( CHARS(1), ICHAR )                                 
C*--------------------------------------------------------------------*C
C* READ A NEW DATA RECORD IF BUFFER IS EMPTY                          *C
C*--------------------------------------------------------------------*C
      ICHAR = 0                                                         
      IF ( IPIX .EQ. NPIX ) THEN                                        
         IREC = IREC + 1                                                
C* CMS/MVS BEGIN                                                        
c        READ(LUNIN,IOSTAT=IRC)                                         
c    $         IWEST,IEAST,INORTH,ISOUTH,NPIX,IOUT                      
c    $        ,(LONBUF(I),LATBUF(I),XBUF(I),YBUF(I),I=1,NPIX)           
c    $        ,(BUFFER(I),I=1,IOUT)                                     
C* CMS/MVS END                                                          
C* UNIX/AIX BEGIN                                                       
         READ(LUNIN,REC=IREC,IOSTAT=IRC)                                
     $         IWEST,IEAST,INORTH,ISOUTH,NPIX,IOUT                      
     $        ,(LONBUF(I),LATBUF(I),XBUF(I),YBUF(I),I=1,NPIX)           
     $        ,(BUFFER(I),I=1,IOUT)                                     
C* UNIX/AIX END                                                         
         IF ( IRC .NE. 0 ) GOTO 900                                     
         PRINT *,IWEST,IEAST,INORTH,ISOUTH,NPIX,IOUT                    
         INPBYT = 0                                                     
         IPIX = 0                                                       
      END IF                                                            
C*--------------------------------------------------------------------*C
C* UNPACK ONE PIXEL                                                   *C
C*--------------------------------------------------------------------*C
  200 CONTINUE                                                          
      IPIX = IPIX + 1                                                   
      CHARS(4) = BUFFER(INPBYT+1)                                       
      IOFF = INPBYT                                                     
      IPOS = XBUF(IPIX)                                                 
      JPOS = YBUF(IPIX)                                                 
      LAT = LATBUF(IPIX)                                                
      LON = LONBUF(IPIX)                                                
C*--------------------------------------------------------------------*C
C* SECTION 1: PIXEL ID, SATELLITE ANGLES, IR CLOUD DETECTION          *C
C*--------------------------------------------------------------------*C
      NODAY = NITTAB(ICHAR)                                             
C????????????                                                           
      NDCNT(NODAY) = NDCNT(NODAY) + 1                                   
C????????????                                                           
      BXSHOR  = SHRTAB(ICHAR)                                           
      LNDWTR = LNDTAB(ICHAR)                                            
      HITOPO = TOPTAB(ICHAR)                                            
      SNOICE = SNITAB(ICHAR)                                            
      TIMSPA = CLSTAB(ICHAR)                                            
      CHARS(4) = BUFFER(INPBYT+2)                                       
      ICSLOG = IFLTAB(ICHAR)                                            
      BXITHR = THRTAB(ICHAR)                                            
      CHARS(4) = BUFFER(INPBYT+3)                                       
      MUE    = ANGTAB(ICHAR)                                            
      CHARS(4) = BUFFER(INPBYT+4)                                       
      IRAD   = ICHAR                                                    
      CHARS(4) = BUFFER(INPBYT+5)                                       
      BXICSR = ICHAR                                                    
      INPBYT = INPBYT + 5                                               
      NDXBYT = 5                                                        
C* ADDITIONAL CHANNELS IF PRESENT                                       
      DO 300 ICHAN = 1,BXADDL                                           
         CHARS(4) = BUFFER(INPBYT+ICHAN)                                
         ARAD(ICHAN) = ICHAR                                            
  300 CONTINUE                                                          
      DO 305 ICHAN = BXADDL+1,3                                         
         ARAD(ICHAN) = 255                                              
  305 CONTINUE                                                          
      INPBYT = INPBYT + BXADDL                                          
      NDXBYT = NDXBYT + BXADDL                                          
C*--------------------------------------------------------------------*C
C* SECTION 2: SUN ANGLES, VIS CLOUD DETECTION                         *C
C*--------------------------------------------------------------------*C
 1300 CONTINUE                                                          
      IF ( NODAY .EQ. 0 ) THEN                                          
         CHARS(4) = BUFFER(INPBYT+1)                                    
         GLINT  = GLNTAB(ICHAR)                                         
         VCSLOG = VFLTAB(ICHAR)                                         
         BXVTHR = THRTAB(ICHAR)                                         
         CHARS(4) = BUFFER(INPBYT+2)                                    
         MU0    = ICHAR                                                 
         CHARS(4) = BUFFER(INPBYT+3)                                    
         PHI    = ICHAR                                                 
         CHARS(4) = BUFFER(INPBYT+4)                                    
         VRAD   = ICHAR                                                 
         CHARS(4) = BUFFER(INPBYT+5)                                    
         BXVCSR = ICHAR                                                 
         INPBYT = INPBYT + 5                                            
         NDXBYT = NDXBYT + 5                                            
      ELSE                                                              
         GLINT  = 255                                                   
         VCSLOG = 255                                                   
         BXVTHR = 255                                                   
         MU0    = 255                                                   
         PHI    = 255                                                   
         VRAD   = 255                                                   
         BXVCSR = 255                                                   
      END IF                                                            
C*--------------------------------------------------------------------*C
C* SECTION 3: ADJUSTED CLOUD DETECTIONS, ADDITIONAL CHANNEL CLOUD     *C
C*            DETECTIONS, IR RETRIEVALS                               *C
C*--------------------------------------------------------------------*C
      CHARS(4) = BUFFER(INPBYT+1)                                       
      DAYNIT = NITTAB(ICHAR)                                            
C??????????????                                                         
      ND2CNT(DAYNIT) = ND2CNT(DAYNIT) + 1                               
C??????????????                                                         
      ITHR   = ITHRTB(ICHAR)                                            
      VTHR   = VTHRTB(ICHAR)                                            
      SHORE  = NSHTAB(ICHAR)                                            
      CHARS(4) = BUFFER(INPBYT+2)                                       
      IRET   = DRETAB(ICHAR)                                            
      ICSRET = CRETAB(ICHAR)                                            
      CHARS(4) = BUFFER(INPBYT+3)                                       
      ICSRAD = ICHAR                                                    
      CHARS(4) = BUFFER(INPBYT+4)                                       
      ITMP   = ICHAR                                                    
      CHARS(4) = BUFFER(INPBYT+5)                                       
      IPRS   = ICHAR                                                    
      CHARS(4) = BUFFER(INPBYT+6)                                       
      ICSTMP = ICHAR                                                    
      CHARS(4) = BUFFER(INPBYT+7)                                       
      ICSPRS = ICHAR                                                    
      INPBYT = INPBYT + 7                                               
      NDXBYT = NDXBYT + 7                                               
C* POLAR SECTION                                                        
      IF ( SATTYP.EQ.1 .OR. SATTYP.EQ.2 .OR. SATTYP.EQ.-3 ) THEN        
         CHARS(4) = BUFFER(INPBYT+1)                                    
         NREF = ICHAR                                                   
         CHARS(4) = BUFFER(INPBYT+2)                                    
         NTHR = CRETAB(ICHAR)                                           
         CHARS(4) = BUFFER(INPBYT+3)                                    
         NCSREF = ICHAR                                                 
         INPBYT = INPBYT + 3                                            
         NDXBYT = NDXBYT + 3                                            
      ELSE                                                              
         NREF = 255                                                     
         NTHR = 0                                                       
         NCSREF = 255                                                   
      END IF                                                            
C*--------------------------------------------------------------------*C
C* SECTION 4: VIS RETRIEVALS                                          *C
C*--------------------------------------------------------------------*C
      IF ( DAYNIT .EQ. 0 ) THEN                                         
         CHARS(4) = BUFFER(INPBYT+1)                                    
         VRET   = DRETAB(ICHAR)                                         
         VCSRET = CRETAB(ICHAR)                                         
         CHARS(4) = BUFFER(INPBYT+2)                                    
         VCSRAD = ICHAR                                                 
         CHARS(4) = BUFFER(INPBYT+3)                                    
         VALBTA = ICHAR                                                 
         CHARS(4) = BUFFER(INPBYT+4)                                    
         VCSALB = ICHAR                                                 
         CHARS(4) = BUFFER(INPBYT+5)                                    
         VTMP   = ICHAR                                                 
         CHARS(4) = BUFFER(INPBYT+6)                                    
         VPRS   = ICHAR                                                 
         CHARS(4) = BUFFER(INPBYT+7)                                    
         VTAUIC = ICHAR                                                 
         CHARS(4) = BUFFER(INPBYT+8)                                    
         VTMPIC = ICHAR                                                 
         CHARS(4) = BUFFER(INPBYT+9)                                    
         VPRSIC = ICHAR                                                 
         INPBYT = INPBYT + 9                                            
         NDXBYT = NDXBYT + 9                                            
      ELSE                                                              
         VRET   = 255                                                   
         VCSRET = 255                                                   
         VCSRAD = 255                                                   
         VALBTA = 255                                                   
         VCSALB = 255                                                   
         VTMP   = 255                                                   
         VPRS   = 255                                                   
         VTAUIC = 255                                                   
         VTMPIC = 255                                                   
         VPRSIC = 255                                                   
         IF ( NODAY .EQ. 0 ) INPBYT = INPBYT + 9                        
      END IF                                                            
C*--------------------------------------------------------------------*C
C* SAVE DATA IN PACKED FORM                                           *C
C*--------------------------------------------------------------------*C
      DO 810 I=1,NDXBYT                                                 
         DXBYT(I) = BUFFER(I+IOFF)                                      
  810 CONTINUE                                                          
C*--------------------------------------------------------------------*C
C* SET THE CLOUD FLAG                                                 *C
C*--------------------------------------------------------------------*C
      IF ( ITHR .GT. 3 .OR. VTHR .GT. 3 .OR. NTHR .GT. 8 ) THEN         
         CLOUD = 1                                                      
      ELSE                                                              
         CLOUD = 0                                                      
      END IF                                                            
C*--------------------------------------------------------------------*C
C* RETURN SUCCESS                                                     *C
C*--------------------------------------------------------------------*C
      DXREAD = 0                                                        
      RETURN                                                            
C*--------------------------------------------------------------------*C
C* RETURN END-OF-IMAGE                                                *C
C*--------------------------------------------------------------------*C
  900 CONTINUE                                                          
      PRINT *,'END OF IMAGE ON LINE',JPOS                               
C??????????????                                                         
      PRINT *,NDCNT                                                     
      PRINT *,ND2CNT                                                    
C??????????????                                                         
      DXREAD = -1                                                       
      RETURN                                                            
      END                                                               
C**********************************************************************C
C**********************************************************************C
C**********************************************************************C
C*                                                                    *C
C*                     B L O C K   D A T A                            *C
C*                                                                    *C
C*--------------------------------------------------------------------*C
C* THIS SUB-PROGRAM INITIALIZES THE CONVERSION TABLES TO CONVERT      *C
C* COUNTS (0-255) INTO PHYSICAL QUANTITIES.  TO USE THESE TABLES      *C
C* WITH ANY PROGRAM, JUST COPY THE COMMON BLOCK DECLARATION (3 LINES) *C
C* INTO YOUR PROGRAM, AND APPEND THIS SUB-PROGRAM.  NO CALL IS        *C
C* NECESSARY BECAUSE THE INITIALIZATION IS DONE AT COMPILE TIME.      *C
C**********************************************************************C
C**********************************************************************C
C**********************************************************************C
      BLOCK DATA                                                        
C*--------------------------------------------------------------------*C
C* COMMON BLOCK DECLARATION:  (3 LINES)                               *C
C*--------------------------------------------------------------------*C
      PARAMETER     ( MAXCNT = 255 )                                    
      COMMON /CNTTAB/ TMPTAB(0:MAXCNT),PRETAB(0:MAXCNT)                 
     $               ,RFLTAB(0:MAXCNT),TAUTAB(0:MAXCNT)                 
C*--------------------------------------------------------------------*C
C* DATA                                                               *C
C*--------------------------------------------------------------------*C
      DATA (TMPTAB(I),I=0,127) /                                        
     &  -100.000,165.000,169.000,172.000,175.000,177.800,180.500,       
     &   183.000,185.500,187.800,190.000,192.000,194.000,195.700,       
     &   197.500,199.200,201.000,202.700,204.500,206.200,208.000,       
     &   209.700,211.500,212.800,214.100,215.400,216.700,217.900,       
     &   219.200,220.500,221.800,223.100,224.400,225.400,226.500,       
     &   227.500,228.600,229.600,230.600,231.700,232.700,233.800,       
     &   234.800,235.700,236.600,237.500,238.400,239.200,240.100,       
     &   241.000,241.900,242.800,243.700,244.500,245.300,246.100,       
     &   246.900,247.700,248.500,249.300,250.100,250.900,251.700,       
     &   252.400,253.100,253.900,254.600,255.300,256.000,256.700,       
     &   257.500,258.200,258.900,259.500,260.200,260.800,261.500,       
     &   262.100,262.800,263.400,264.100,264.700,265.400,266.000,       
     &   266.600,267.200,267.800,268.400,269.100,269.700,270.300,       
     &   270.900,271.500,272.100,272.700,273.200,273.800,274.400,       
     &   275.000,275.600,276.100,276.700,277.300,277.800,278.400,       
     &   278.900,279.500,280.000,280.500,281.100,281.600,282.200,       
     &   282.700,283.200,283.700,284.200,284.700,285.200,285.800,       
     &   286.300,286.800,287.300,287.800,288.300,288.800,289.300,       
     &   289.800,290.200/                                               
      DATA (TMPTAB(I),I=128,255) /                                      
     &                   290.700,291.200,291.700,292.200,292.700,       
     &   293.200,293.600,294.100,294.600,295.000,295.500,296.000,       
     &   296.500,296.900,297.400,297.800,298.300,298.700,299.200,       
     &   299.600,300.100,300.500,301.000,301.400,301.900,302.300,       
     &   302.800,303.200,303.600,304.000,304.500,304.900,305.300,       
     &   305.800,306.200,306.600,307.000,307.500,307.900,308.300,       
     &   308.700,309.100,309.600,310.000,310.400,310.800,311.200,       
     &   311.600,312.000,312.400,312.900,313.300,313.700,314.100,       
     &   314.500,314.900,315.300,315.700,316.100,316.400,316.800,       
     &   317.200,317.600,318.000,318.400,318.800,319.200,319.500,       
     &   319.900,320.300,320.700,321.100,321.400,321.800,322.200,       
     &   322.600,323.000,323.300,323.700,324.100,324.500,324.900,       
     &   325.200,325.600,326.000,326.400,326.700,327.100,327.400,       
     &   327.800,328.200,328.500,328.900,329.200,329.600,329.900,       
     &   330.300,330.600,331.000,331.300,331.700,332.000,332.400,       
     &   332.700,333.100,333.400,333.800,334.100,334.500,334.800,       
     &   335.200,335.500,335.900,336.200,336.600,336.900,337.300,       
     &   337.600,338.000,338.300,338.600,339.000,339.300,339.700,       
     &   340.000,345.000,-200.000,-1000.000/                            
      DATA (PRETAB(I),I=0,127) /                                        
     &  -100.00,  1.00, 5.00, 10.00,15.00,20.00,25.00,30.00,35.00,40.00,
     &    45.00, 50.00, 55.00,60.00,65.00,70.00,75.00,80.00,85.00,90.00,
     &    95.00,100.00,105.00,110.00,115.00,120.00,125.00,130.00,135.00,
     &   140.00,145.00,150.00,155.00,160.00,165.00,170.00,175.00,180.00,
     &   185.00,190.00,195.00,200.00,205.00,210.00,215.00,220.00,225.00,
     &   230.00,235.00,240.00,245.00,250.00,255.00,260.00,265.00,270.00,
     &   275.00,280.00,285.00,290.00,295.00,300.00,305.00,310.00,315.00,
     &   320.00,325.00,330.00,335.00,340.00,345.00,350.00,355.00,360.00,
     &   365.00,370.00,375.00,380.00,385.00,390.00,395.00,400.00,405.00,
     &   410.00,415.00,420.00,425.00,430.00,435.00,440.00,445.00,450.00,
     &   455.00,460.00,465.00,470.00,475.00,480.00,485.00,490.00,495.00,
     &   500.00,505.00,510.00,515.00,520.00,525.00,530.00,535.00,540.00,
     &   545.00,550.00,555.00,560.00,565.00,570.00,575.00,580.00,585.00,
     &   590.00,595.00,600.00,605.00,610.00,615.00,620.00,625.00,630.00/
      DATA (PRETAB(I),I=128,255) /                                      
     &   635.00,640.00,645.00,650.00,655.00,660.00,665.00,670.00,675.00,
     &   680.00,685.00,690.00,695.00,700.00,705.00,710.00,715.00,720.00,
     &   725.00,730.00,735.00,740.00,745.00,750.00,755.00,760.00,765.00,
     &   770.00,775.00,780.00,785.00,790.00,795.00,800.00,805.00,810.00,
     &   815.00,820.00,825.00,830.00,835.00,840.00,845.00,850.00,855.00,
     &   860.00,865.00,870.00,875.00,880.00,885.00,890.00,895.00,900.00,
     &   905.00,910.00,915.00,920.00,925.00,930.00,935.00,940.00,945.00,
     &   950.00,955.00,960.00,965.00,970.00,975.00,980.00,985.00,990.00,
     &   995.00,1000.00,1005.00,1010.00,1015.00,1020.00,1025.00,1030.00,
     &   1035.00,1040.00,1045.00,1050.00,1055.00,1060.00,1065.00,       
     &   1070.00,1075.00,1080.00,1085.00,1090.00,1095.00,1100.00,       
     &   1105.00,1110.00,1115.00,1120.00,1125.00,1130.00,1135.00,       
     &   1140.00,1145.00,1150.00,1155.00,1160.00,1165.00,1170.00,       
     &   1175.00,1180.00,1185.00,1190.00,1195.00,1200.00,-200.00,       
     &   -200.00,-200.00,-200.00,-200.00,-200.00,-200.00,-200.00,       
     &   -200.00,-200.00,-200.00,-200.00,-200.00,-1000.00/              
      DATA (RFLTAB(I),I=0,127) /                                        
     &  -100.000,0.000,0.008,0.012,0.016,0.020,0.024,0.028,0.032,0.036, 
     &     0.040,0.044,0.048,0.052,0.056,0.060,0.064,0.068,0.072,0.076, 
     &     0.080,0.084,0.088,0.092,0.096,0.100,0.104,0.108,0.112,0.116, 
     &     0.120,0.124,0.128,0.132,0.136,0.140,0.144,0.148,0.152,0.156, 
     &     0.160,0.164,0.168,0.172,0.176,0.180,0.184,0.188,0.192,0.196, 
     &     0.200,0.204,0.208,0.212,0.216,0.220,0.224,0.228,0.232,0.236, 
     &     0.240,0.244,0.248,0.252,0.256,0.260,0.264,0.268,0.272,0.276, 
     &     0.280,0.284,0.288,0.292,0.296,0.300,0.304,0.308,0.312,0.316, 
     &     0.320,0.324,0.328,0.332,0.336,0.340,0.344,0.348,0.352,0.356, 
     &     0.360,0.364,0.368,0.372,0.376,0.380,0.384,0.388,0.392,0.396, 
     &     0.400,0.404,0.408,0.412,0.416,0.420,0.424,0.428,0.432,0.436, 
     &     0.440,0.444,0.448,0.452,0.456,0.460,0.464,0.468,0.472,0.476, 
     &     0.480,0.484,0.488,0.492,0.496,0.500,0.504,0.508/             
      DATA (RFLTAB(I),I=128,255) /                                      
     &                                                     0.512,0.516, 
     &     0.520,0.524,0.528,0.532,0.536,0.540,0.544,0.548,0.552,0.556, 
     &     0.560,0.564,0.568,0.572,0.576,0.580,0.584,0.588,0.592,0.596, 
     &     0.600,0.604,0.608,0.612,0.616,0.620,0.624,0.628,0.632,0.636, 
     &     0.640,0.644,0.648,0.652,0.656,0.660,0.664,0.668,0.672,0.676, 
     &     0.680,0.684,0.688,0.692,0.696,0.700,0.704,0.708,0.712,0.716, 
     &     0.720,0.724,0.728,0.732,0.736,0.740,0.744,0.748,0.752,0.756, 
     &     0.760,0.764,0.768,0.772,0.776,0.780,0.784,0.788,0.792,0.796, 
     &     0.800,0.804,0.808,0.812,0.816,0.820,0.824,0.828,0.832,0.836, 
     &     0.840,0.844,0.848,0.852,0.856,0.860,0.864,0.868,0.872,0.876, 
     &     0.880,0.884,0.888,0.892,0.896,0.900,0.904,0.908,0.912,0.916, 
     &     0.920,0.924,0.928,0.932,0.936,0.940,0.944,0.948,0.952,0.956, 
     &     0.960,0.964,0.968,0.972,0.976,0.980,0.984,0.988,0.992,1.000, 
     &     1.016,1.040,1.072,1.108,-200.000,-1000.000/                  
      DATA (TAUTAB(I),I=0,127) /                                        
     &  -100.000,0.020,0.040,0.060,0.090,0.110,0.140,0.160,0.190,0.220, 
     &     0.240,0.270,0.300,0.330,0.370,0.400,0.430,0.460,0.500,0.530, 
     &     0.570,0.600,0.640,0.680,0.720,0.750,0.790,0.830,0.870,0.920, 
     &     0.960,1.000,1.040,1.090,1.130,1.180,1.220,1.270,1.320,1.370, 
     &     1.420,1.470,1.520,1.570,1.620,1.670,1.730,1.780,1.830,1.890, 
     &     1.950,2.000,2.060,2.120,2.180,2.240,2.300,2.360,2.430,2.490, 
     &     2.550,2.620,2.690,2.750,2.820,2.890,2.960,3.030,3.100,3.180, 
     &     3.250,3.320,3.400,3.480,3.550,3.630,3.710,3.790,3.880,3.960, 
     &     4.040,4.130,4.220,4.300,4.390,4.480,4.570,4.670,4.760,4.850, 
     &     4.950,5.050,5.150,5.250,5.350,5.450,5.560,5.660,5.770,5.880, 
     &     5.990,6.110,6.220,6.340,6.450,6.570,6.690,6.820,6.940,7.070, 
     &     7.190,7.330,7.460,7.590,7.730,7.870,8.010,8.150,8.300,8.440, 
     &     8.590,8.740,8.900,9.060,9.220,9.380,9.540,9.710/             
      DATA (TAUTAB(I),I=128,255) /                                      
     &                                                     9.880,10.050,
     &   10.230,10.410,10.590,10.780,10.970,11.160,11.350,11.550,11.760,
     &   11.960,12.170,12.390,12.600,12.830,13.050,13.280,13.520,13.760,
     &   14.000,14.250,14.510,14.770,15.030,15.300,15.580,15.860,16.150,
     &   16.440,16.740,17.050,17.360,17.690,18.020,18.350,18.700,19.050,
     &   19.410,19.780,20.160,20.540,20.940,21.350,21.770,22.200,22.630,
     &   23.080,23.550,24.030,24.520,25.020,25.540,26.070,26.620,27.190,
     &   27.770,28.370,28.990,29.630,30.290,30.970,31.670,32.400,33.160,
     &   33.940,34.740,35.580,36.450,37.350,38.290,39.260,40.260,41.320,
     &   42.420,43.570,44.760,46.000,47.310,48.680,50.110,51.600,53.170,
     &   54.840,56.590,58.430,60.360,62.400,64.590,66.900,69.360,71.960,
     &   74.720,77.730,80.940,84.380,88.060,92.020,96.400,101.010,      
     &   105.510,109.870,114.330,119.590,125.920,133.660,143.120,       
     &   154.650, 169.560, 187.490, 207.200, 228.130, 250.440, 282.780, 
     &   323.920, 378.650,-200.000,-200.000,-200.000,-200.000,-200.000, 
     &  -200.000,-200.000,-200.000,-200.000,-200.000,-200.000,          
     &  -1000.000/                                                      
      END                                                               
