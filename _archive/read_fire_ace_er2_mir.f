      Program read_fire_ace_mir
c**********************************************************************
c
c function:  Read FIRE_ACE_ER2_MIR data files for a given flight and
c            print selected parameters for every 1000th record.
c            Each flight is contained in one file.  All values are 
c            in four-byte IEEE floating point values.  Each logical record
c            contains one calibrated MIR scan.
c 
c     *** NOTE: use the option -bytereclen when compiling on an SGI ***
c     *** NOTE: use the option -assume byterecl when compiling on an DecAlpha
c               AND Comment out the subroutine call to FLIP         ***
c
c
c     *** Please see the README file for further details ***
c
c              Logical Record Format
c              ---------------------
c    Word         Parameter
c    ----         ---------
c       1         Record Number
c       2         Month           - Real time clock (RTC)
c       3         Day             - Real time clock (RTC)
c       4         Hour            - IRIG
c       5         Minute          - IRIG
c       6         Second          - IRIG
c       7         Julian Day      - Navigation
c       8         Hour            - Navigation
c       9         Minute          - Navigation
c      10         Second          - Navigation
c      11         Latitude        - Degrees
c      12         Longitude       - Degrees (-West, +East)
c      13         Air Temperature - Degrees celsius
c      14         Altitude        - Feet
c      15         Pitch           - Degrees (+ for nose down)
c      16         Roll            - Degrees (+ for roll right)
c      17         Heading         - Degrees
c
c  18- 26         HouseKeeping Temperatures    
c      27         Hot  average temperature for this scan
c      28         Cold average temperature for this scan
c      29         Hot  temperature, 8-scan moving average
c      30         Cold temperature, 8-scan moving average
c
c  31- 39         Hot  average counts for this scan
c  40- 48         Cold average counts for this scan
c  49- 57         Hot  counts, 8-scan moving average
c  58- 66         Cold counts, 8-scan moving average
c
c  67-123         57 brightness temperatures    - degrees Kelvin   89 GHz
c 124-180          "     "           "          -    "      "     150
c 181-237          "     "           "          -    "      "     183.3 +/-1
c 238-294          "     "           "          -    "      "     183.3 +/-3
c 295-351          "     "           "          -    "      "     183.3 +/-7
c 352-408          "     "           "          -    "      "     220
c 409-465          "     "           "          -    "      "     340
c 466-579                       *** unused ***
c
c Logical record length (4*579)..........                
c
c==================================================================================
c ---------------------------------------------------------------------
c This software has been tested on the following systems:
c 
c	Computer		Operating System
c	-------------------  	----------------
c	Sun Sparc		Solaris 2.6
c	SGI Origin 2000		IRIX 6.4
c	HP 9000/735		HP-UX 10.10
c       Dec Alpha               Digital UNIX 4.0A
c
c ---------------------------------------------------------------------
c
c  Contact: Langley DAAC User and Data Services Office 
c           NASA Langley Research Center 
c           Mail Stop 157D 
c           Hampton, VA 23681-2199 
c           Phone (757)864-8656   Fax (757)864-8807 
c           e-mail:  larc@eos.nasa.gov 
c           http://eosweb.larc.nasa.gov 
c
c**********************************************************************
      implicit none
c*====================================================================*c
      integer max_records
      parameter  (max_records=10000)      
c*====================================================================*c
      structure /OutStruct/
      real*4   		RecNumber
      real*4   		RTCmonth, RTCDayOfMonth
      real*4   		IrigHour, IrigMin, IrigSec
      real*4   		NavJulian, NavHour,  NavMin,  NavSec
      real*4		latitude, longitude
      real*4		AirTemp, altitude, pitch, roll, heading
      real*4            HouskptTmp(9)
      real*4            HotTemp,CldTemp,HotTemp8a,CldTemp8a
      real*4            HotCnt(9) ,CldCnt(9) ,HotCnt8a(9),CldCnt8a(9)
      real*4   		BrightTemps(9,57)
      end structure
c*====================================================================*c
      record/OutStruct/ i
      record/OutStruct/ j
c*====================================================================*c
      character*80 filename
c*====================================================================*c      
      integer   m, k, first_record, last_record
c*====================================================================*c
      logical exist
c*====================================================================*c      
 
c=========================  Begin Processing  =======================c

c === Obtain input filename.
      print*,'Enter Input Filename:'
      read(5,100) filename

c === Check for input file existance.
      inquire (file=filename, exist=exist)
      if (.not. exist) then
         print*
         print*,'   Invalid Input Filename: ',filename
         print*
         stop ' '
      endif

c === Open the data file.
      open(10, file=filename,
     & access='DIRECT', status='OLD', err=1999, recl=579*4)

      print*
      print*,'Opened Input File ',filename
      
      last_record=0
      
c === Read all the data from the file.    
      do m= 1, max_records

         read(10, rec=m, err=400) j 

         i=j       

c === comment out this call to the subroutine if you're working on a DecAlpha
         call FLIP(j,i,579*4)

         if(m.eq.1)first_record=i.RecNumber
         if(i.RecNumber.gt.last_record)last_record=i.RecNumber
         
c === Print selected parameters for every 1000th record
         if(mod(m,1000).eq.0)
     &   print200,int(i.RecNumber),int(i.RTCmonth),int(i.RTCDayOfMonth),
     &            int(i.IrigHour), int(i.IrigMin),int(i.IrigSec),
     &            i.latitude,i.longitude,i.altitude,
     &            (i.BrightTemps(k,29),k= 1,7)     

      end do

      stop 
      
c === Error Handling
 400  print*
      print*,'Last Data Record ',last_record
      print*,'Total Data Records ',last_record-first_record
      stop 
 1999 print*,'Error Opening Input File ',filename
      stop 5

c === Format Statements
100   format(a80)
200   format(i4.4,3(1x,i2.2),2(':',i2.2),1x,f6.2,
     & 1x,f7.2,1x,f7.1,7(1x,f6.2))
     
      end
c=======================  End of main source code  =====================c
c subroutine to flip bits
c big endian/little endian swap
c
c **** NOTE: This subroutine is not used on a DecAlpha ****

      subroutine flip(ib,jb,n)
      character*1 ib(n),jb(n)
       do k=1,n-3,4
        jb(k  )=ib(k+3)
        jb(k+1)=ib(k+2)
        jb(k+2)=ib(k+1)
        jb(k+3)=ib(k  )
       end do
      return
      end
c=======================  End of subroutine flip  =====================c
