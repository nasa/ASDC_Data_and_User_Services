c----6----------------------------------------------------------------72>
      program meteosat
c
c>> define character variables
c
      character val*2,reserved3*2,spare1*2,saidhp*2,sn2lp*2
      character ans*3
      character ecdesi*4,ecdesv*4,ecdesw*4,satidl*4
      character vslot*5
      character setnam*6,setemp*6,setend*6
      character value*8,value4*8,value2*8,label*8,arcidt*8,infile*8
      character reserved*12,anqhp*12
      character fn2lp*8,ty2lp*8
      character value1*12,ecpoii1*12,ecpoii2*12,ecpoii3*12,ecpoii4*12
      character ecpoii5*12,ecpoiv1*12,ecpoiv2*12,ecpoiv3*12,ecpoiv4*12
      character ecpoiv5*12,ecpoiw1*12,ecpoiw2*12,ecpoiw3*12,ecpoiw4*12
      character ecpoiw5*12
      character photo1*16,photo2*16,photo3*16,photo4*16
      character spare*24
      character namdate*26
      character value28*28,anbhp*28
      character filename*40,fileout*40
      character reserved1*92
      character ecvali*256,ecvalv*256,ecvalw*256
      character cspare*5418
c
c>> define logical variables 
c
      logical*1 store,mltuhp,mltvhp,mltihp,mltwhp,vic2lp,irc2lp
      logical*1 wvc2lp,intdhp,notused,vis1,vis2,irdat,wvdat
c----6----------------------------------------------------------------72>
c
c>> define integer*2 variables
c
      integer*2 short,nslt,ndslt,nslt1,ndslt1
      integer*2 nslt2,ndslt2,nslt3,ndslt3,nslt4,ndslt4
      integer*2 nslt5,ndslt5,nslt6,ndslt6,nslt7,ndslt7
      integer*2 nslt8,ndslt8,nslt9,ndslt9,nslt10,ndslt10
      integer*2 nslt11,ndslt11,nslt12,ndslt12,nslt13,ndslt13
      integer*2 nslt14,ndslt14,nslt15,ndslt15,nslt16,ndslt16
      integer*2 nslt17,ndslt17,nslt18,ndslt18,nslt19,ndslt19
      integer*2 nslt20,ndslt20,nslt21,ndslt21,nslt22,ndslt22
      integer*2 nslt23,ndslt23
      integer*2 offset,jdayhp,slohp,nithp,slothp,gridhp
      integer*2 sspxhp,sspyhp,thrhp,nblhp,spcthp
      integer*2 spcthp1,spcthp2,lastl,visid,wvid,irid
      integer*2 vispor,lastl2,ilast,ivisid,iwvid,iirid
c
c>> define integer*4 variables
c
      integer*4 rlen ,nrecs,jslot,yr,day,slot,dty,dum1,dum2,dum3,dum4
      integer*4 blz,nbl,nhb,off,ofs,ofs1,ofs2,ofs3,ofs4,ofs5,ofs6,ofs7
      integer*4 ofs8,ofs9,ofs10,ofs11,ofs12,ofs13,ofs14,ofs15,ofs16
      integer*4 ofs17,ofs18,ofs19,ofs20,ofs21,ofs22,ofs23
      integer*4 ibgwnd,nblwnd,ibgpix,nblpix,mbywnd,mbypix,nslots
      integer*4 yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,blk2lp
      integer*4 nbl2lp,nhb2lp,milsec,fishre,ijslot
      integer*4 julslt,imgqua,ndgrpt,lstart,lend,lstep,jslohp

c
c>> define real*4 variables
c
      real*4 defmax,defmay,rmij,rsij,attf,attl,pixmid,linmid
c
c>> define real*8 variables
c
      real*8 gretg,grptg,ghtg,f1satg,orbf,orbl,forbs,forbn
c
c>> setup common with character variables
c
c----6----------------------------------------------------------------72>
      common/char/ val,reserved3,spare1,saidhp,ans,ecdesi,ecdesv,
     $ ecdesw,satidl,vslot,setnam,setemp,setend,value,value4,value2,
     $ label,arcidt,infile,reserved,fn2lp,value1,ecpoii1,ecpoii2,
     $ ecpoii3,ecpoii4,ecpoii5,ecpoiv1,ecpoiv2,ecpoiv3,ecpoiv4,ecpoiv5,
     $ ecpoiw1,ecpoiw2,ecpoiw3,ecpoiw4,ecpoiw5,photo1,photo2,photo3,
     $ photo4,spare,namdate,value28,anbhp,filename,fileout,reserved1,
     $ ecvali,ecvalv,ecvalw,cspare,ty2lp,sn2lp,anqhp
c
c>> setup logical variables in common
c
      common /logical/store(24),mltuhp(316),mltvhp(316),mltihp(316)
     $ ,mltwhp(316),vic2lp(4),irc2lp(2),wvc2lp(2),intdhp(1360)
     $ ,notused(148)
c
c>> setup integer*2 variables in common
c
      common /inttwo/ short,nslt,ndslt,nslt1,ndslt1,
     $  nslt2,ndslt2,nslt3,ndslt3,nslt4,ndslt4,nslt5,
     $  ndslt5,nslt6,ndslt6,nslt7,ndslt7,nslt8,ndslt8,nslt9,ndslt9,
     $  nslt10,ndslt10,nslt11,ndslt11,nslt12,ndslt12,nslt13,ndslt13,
     $  nslt14,ndslt14,nslt15,ndslt15,nslt16,ndslt16,nslt17,ndslt17,
     $  nslt18,ndslt18,nslt19,ndslt19,nslt20,ndslt20,nslt21,ndslt21,
     $  nslt22,ndslt22,nslt23,ndslt23,offset,jdayhp,slohp,nithp,
     $  slothp,gridhp,sspxhp,sspyhp,thrhp,nblhp(3,3),spcthp(3),spcthp1,
     $  spcthp2,lastl,visid,wvid,irid,vispor(2),lastl2
c---------------------------------------------------------------------->
c
c>> setup integer*4 variables in common
c
      common /intfour/ rlen,nrecs,jslot,yr,day,slot,dty,dum1,dum2,dum3,
     $  dum4,blz,nbl,nhb,off,ofs,ofs1,ofs2,ofs3,ofs4,ofs5,ofs6,ofs7,
     $  ofs8,ofs9,ofs10,ofs11,ofs12,ofs13,ofs14,ofs15,ofs16,ofs17,
     $  ofs18,ofs19,ofs20,ofs21,ofs22,ofs23,ibgwnd,nblwnd,ibgpix,nblpix,
     $  mbywnd,mbypix,nslots,yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,
     $  blk2lp,nbl2lp,nhb2lp,milsec,fishre(4),julslt,imgqua,
     $  ndgrpt,lstart,lend,lstep,jslohp

c
c>> setup real*4 variables in common
c
      common /realfour/ defmax(26,26),defmay(26,26),rmij(2,8,2),
     $  rsij(2,8,2),attf,attl,pixmid,linmid  
c
c>> setup real*8 variables in common
c
      common /realeight/gretg,grptg,ghtg,f1satg,orbf,orbl,forbs,
     $  forbn
c
      common/imgrec/ijslot(5),ilast(5),ivisid(5),iwvid(5),
     $              iirid(5),vis1(1504,5),vis2(1504,5),
     $              irdat(752,5),wvdat(752,5)
c
c*********************************************************************
c
c>> begin processing data
c
c*********************************************************************
      write(*,*)" "
      write(*,*)" "
      write(*,*)" "
      write(*,*)" "
      write(*,*)" "
      call banner
      write(*,*)" "
      write(*,*)" "
      call getfiles(filename,fileout)
      call menuopts(ans)
      call process
      write(*,55)fileout
55    format("Output is written to file ----> ",a)
      print*,"Normal termination of program."
      print*," "
      stop
      end
c************************************************************************
c----6----------------------------------------------------------------72>
      subroutine getfiles(filename,fileout)
c>>integer
      integer ios
c>> done initilzed to FALSE
      logical done ,ex
      character*40 filename
      character*40 fileout
      character *1 ans
c
      print*," Please enter the name of the INPUT file you wish"
     $      ," to read:"
      read(*,*)filename
      do while (done .ne. .TRUE.)
        print*," "
        print*," Please enter the name of the OUTPUT file you wish"
     $        ," to write to:"
        read(*,99)fileout
        inquire(file=fileout,iostat=ios,exist=ex)
        if (ios .ne. 0) then
          print*,"Error on file ",fileout
          stop
        endif
        if (ex .eq. .TRUE.) then
          print*," This file exists:---> ",fileout
c----6----------------------------------------------------------------72>
          print*," Do you wish to overwrite it? If so please enter 'y'"
     $          ," for yes."
          print*," Else enter [cr] to  re-enter file name."
          read(*,99)ans
          if (ans(1:1) .eq. "y") then
            done = .TRUE.
          endif
        else
           done = .TRUE.
        endif
      end do
9000  continue
      open(2,file=fileout,form="formatted")
      open(1,file=filename,err=100,form='unformatted',access='direct'
     $    ,recl=23402)
      open(3,file='scratch',form='formatted')
c     open(4,file='test',form='formatted')
      return
100   stop4
99    format(a)
      end
c************************************************************************
c----6----------------------------------------------------------------72>
      subroutine menuopts(ans)
c
      character*3 ans
10    print*,"Please make a selection."
      write(*,9)
c----6----------------------------------------------------------------72>
9     format("*********************************************************"
     $,"****************",
     $/" 1) Write HEADER record(s) only to the output file."
     $,/" 2) Write a Specific SLOT number data record to the output"
     $," file.",/"      [if you don't know which slots are on the file"
     $," do option 3 first]"
     $/" 3) Write a summary of Slot numbers found on the file to the"
     $," output file."
     $/"Enter 'h' for help about this menu."
     $/"Enter 'x' to exit or terminate process."
     $/"**************************************************************"
     $,"***********")
      read(*,20)ans
20    format(a)
      if(ans .eq. '1') then
      print*,ans
      go to 99
      elseif(ans .eq. '2') then
      print*,ans
      go to 99
      elseif(ans .eq. '3') then
      print*,ans
      go to 99
      elseif(ans .eq. '4') then
      print*,ans
      go to 99
      elseif(ans .eq. 'h') then
      print*," "
      print*," "
      print*," "
      print*," "
      print*," "
      print*," "
      print*," "
      print*," "
      print*,"****************** HELP COMMAND ************************"
      print*," This menu gives you three options from which to select."
      print*," Option 1 will only read the header records found at the"
      print*," beginning of the file selected. This allows the user"
      print*," to see the header file variable names and a description"
      print*," of each variable without dumping specific data records."
      print*," Option 2 prompts the user to select one of the 1 - 48"
      print*," slots for a given day found on a single file.  The files"
      print*," contain image data for all channels (Visible, Infra-red"
      print*," and Water Vapour) for a specified window or (sub-area)."
      print*," Option 3 allows the user to dump a particular data file"
      print*," and get a summary listing of the data file.  This is"
      print*," recommended before a file is processed to verify which"
      print*," slots are found on a file."
      print*," "
      go to 10
      elseif(ans .eq. 'x') then
      stop
      else
      print*,"This is not a valid selection --> ",ans
      go to 10
      endif
99    return
      end
c************************************************************************
c----6----------------------------------------------------------------72>
      subroutine banner
      write(*,9)
      return
9     format(80("*")/
     $"*",78x,"*"/
     $"*",25x,"Langley DAAC",41x,"*"/
     $"*",78x,"*"/
c----6----------------------------------------------------------------72>
     $"*",14x,"PROJECT_LEVEL/data_SET_LEVEL READ SOFTWARE",22x,"*"/
     $"*",4x,"VERSION NUMBER:  1.0",54x,"*"/
     $"*",4x,"Language: FORTRAN 77",54x,"*"/
     $"*",4x,"Platforms supported: SUN",50x,"*"/
     $"*",25x,"HP",51x,"*"/
     $"*",25x,"SGI",50x,"*"/
     $"*",25x,"Dec Alpha",44x,"*"/
     $"*",4x,"Contact: Langley DAAC user and Data Services Office",23x,
     $"*"/
     $"*",14x,"NASA Langley Research Center",36x,"*"/
     $"*",14x,"Mail Stop 157D",50x,"*"/
     $"*",14x,"Hampton, VA 23681-0001",42x,"*"/
c----6----------------------------------------------------------------72>
     $"*",14x,"PHONE:   (757)864-8656          FAX: (757) 864-8807",13x,
     $"*"/"*",78x,"*"/
     $80("*"))
      end
c************************************************************************
c----6----------------------------------------------------------------72>
      subroutine readfile(irec,imagline)
c
      character val*2,reserved3*2,spare1*2,saidhp*2,sn2lp*2
      character ans*3
      character ecdesi*4,ecdesv*4,ecdesw*4,satidl*4
      character vslot*5
      character setnam*6,setemp*6,setend*6,reserved2*6
      character value*8,value4*8,value2*8,label*8,arcidt*8,infile*8
      character reserved*12
      character fn2lp*8,ty2lp*8
      character value1*12,ecpoii1*12,ecpoii2*12,ecpoii3*12,ecpoii4*12
      character ecpoii5*12,ecpoiv1*12,ecpoiv2*12,ecpoiv3*12,ecpoiv4*12
      character ecpoiv5*12,ecpoiw1*12,ecpoiw2*12,ecpoiw3*12,ecpoiw4*12
      character ecpoiw5*12,anqhp*12
      character photo1*16,photo2*16,photo3*16,photo4*16
      character spare*24
      character namdate*26
      character value28*28,anbhp*28
      character filename*40,fileout*40
      character reserved1*92
      character ecvali*256,ecvalv*256,ecvalw*256
      character cspare*5418
      
      logical*1 store,mltuhp,mltvhp,mltihp,mltwhp,vic2lp,irc2lp
      logical*1 wvc2lp,intdhp,notused,vis1,vis2,irdat,wvdat
c
      integer*2 short,nslt,ndslt,nslt1,ndslt1
      integer*2 nslt2,ndslt2,nslt3,ndslt3,nslt4,ndslt4
      integer*2 nslt5,ndslt5,nslt6,ndslt6,nslt7,ndslt7
      integer*2 nslt8,ndslt8,nslt9,ndslt9,nslt10,ndslt10
      integer*2 nslt11,ndslt11,nslt12,ndslt12,nslt13,ndslt13
      integer*2 nslt14,ndslt14,nslt15,ndslt15,nslt16,ndslt16
      integer*2 nslt17,ndslt17,nslt18,ndslt18,nslt19,ndslt19
      integer*2 nslt20,ndslt20,nslt21,ndslt21,nslt22,ndslt22
      integer*2 nslt23,ndslt23
      integer*2 vispor,lastl2,ilast,ivisid,iwvid,iirid
      integer*2 offset,jdayhp,slohp,nithp,slothp,gridhp
      integer*2 sspxhp,sspyhp,thrhp,nblhp,spcthp
      integer*2 spcthp1,spcthp2,lastl,visid,wvid,irid
      integer*4 rlen ,nrecs,jslot,yr,day,slot,dty,dum1,dum2,dum3,dum4
      integer*4 blz,nbl,nhb,off,ofs,ofs1,ofs2,ofs3,ofs4,ofs5,ofs6,ofs7
      integer*4 ofs8,ofs9,ofs10,ofs11,ofs12,ofs13,ofs14,ofs15,ofs16
      integer*4 ofs17,ofs18,ofs19,ofs20,ofs21,ofs22,ofs23
      integer*4 ibgwnd,nblwnd,ibgpix,nblpix,mbywnd,mbypix,nslots
      integer*4 yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,blk2lp
      integer*4 nbl2lp,nhb2lp,milsec,fishre,ijslot
      integer*4 julslt,imgqua,ndgrpt,lstart,lend,lstep,jslohp
      integer*4 rec_per_blk,irec,imagline,lrec,krec
      real*4 defmax,defmay,rmij,rsij,attf,attl,pixmid,linmid
      real*8 gretg,grptg,ghtg,f1satg,orbf,orbl,forbs,forbn

c----6----------------------------------------------------------------72>
      common/char/ val,reserved3,spare1,saidhp,ans,ecdesi,ecdesv,
     $ ecdesw,satidl,vslot,setnam,setemp,setend,value,value4,value2,
     $ label,arcidt,infile,reserved,fn2lp,value1,ecpoii1,ecpoii2,
     $ ecpoii3,ecpoii4,ecpoii5,ecpoiv1,ecpoiv2,ecpoiv3,ecpoiv4,ecpoiv5,
     $ ecpoiw1,ecpoiw2,ecpoiw3,ecpoiw4,ecpoiw5,photo1,photo2,photo3,
     $ photo4,spare,namdate,value28,anbhp,filename,fileout,reserved1,
     $ ecvali,ecvalv,ecvalw,cspare,ty2lp,sn2lp,anqhp

      common /logical/store(24),mltuhp(316),mltvhp(316),mltihp(316)
     $ ,mltwhp(316),vic2lp(4),irc2lp(2),wvc2lp(2),intdhp(1360)
     $ ,notused(148)

      common /inttwo/ short,nslt,ndslt,nslt1,ndslt1,
     $  nslt2,ndslt2,nslt3,ndslt3,nslt4,ndslt4,nslt5,
     $  ndslt5,nslt6,ndslt6,nslt7,ndslt7,nslt8,ndslt8,nslt9,ndslt9,
     $  nslt10,ndslt10,nslt11,ndslt11,nslt12,ndslt12,nslt13,ndslt13,
     $  nslt14,ndslt14,nslt15,ndslt15,nslt16,ndslt16,nslt17,ndslt17,
     $  nslt18,ndslt18,nslt19,ndslt19,nslt20,ndslt20,nslt21,ndslt21,
     $  nslt22,ndslt22,nslt23,ndslt23,offset,jdayhp,slohp,nithp,
     $  slothp,gridhp,sspxhp,sspyhp,thrhp,nblhp(3,3),spcthp(3),spcthp1,
     $  spcthp2,lastl,visid,wvid,irid,vispor(2),lastl2

c----6----------------------------------------------------------------72>
      common /intfour/ rlen,nrecs,jslot,yr,day,slot,dty,dum1,dum2,dum3,
     $  dum4,blz,nbl,nhb,off,ofs,ofs1,ofs2,ofs3,ofs4,ofs5,ofs6,ofs7,
     $  ofs8,ofs9,ofs10,ofs11,ofs12,ofs13,ofs14,ofs15,ofs16,ofs17,
     $  ofs18,ofs19,ofs20,ofs21,ofs22,ofs23,ibgwnd,nblwnd,ibgpix,nblpix,
     $  mbywnd,mbypix,nslots,yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,
     $  blk2lp,nbl2lp,nhb2lp,milsec,fishre(4),julslt,imgqua,
     $  ndgrpt,lstart,lend,lstep,jslohp

      common /realfour/ defmax(26,26),defmay(26,26),rmij(2,8,2),
     $  rsij(2,8,2),attf,attl,pixmid,linmid
c
      common /realeight/gretg,grptg,ghtg,f1satg,orbf,orbl,forbs,
     $  forbn
c
      common/imgrec/ijslot(5),ilast(5),ivisid(5),iwvid(5),
     $              iirid(5),vis1(1504,5),vis2(1504,5),
     $              irdat(752,5),wvdat(752,5)
      data lrec/2/,krec/0/
      ii=0
      if (irec .eq. 1) then
        call header_rec
      elseif (irec .eq. 2) then
        call first_rectif(lrec,short,fn2lp,yy2lp,dd2lp,sl2lp,dt2lp
     $                   ,ipr2lp,spc2lp,reserved,blk2lp,nbl2lp
     $                   ,nhb2lp,ty2lp,sn2lp,reserved2,jslot
     $                   ,lastl,vic2lp,irc2lp,wvc2lp
     $                   ,milsec,vispor,fishre)
c
      elseif (irec .eq. 3) then
        call second_rectif(lrec,offset,julslt,imgqua,ndgrpt,lstart,lend
     $                    ,lstep,defmax,defmay,cspare)
c
      elseif (irec .eq. 4) then
        call third_rectif(lrec,offset,jslohp,jdayhp,slohp,nithp,slothp
     $                   ,gridhp,saidhp,sspxhp,sspyhp,mltuhp
     $                   ,mltvhp,mltihp,mltwhp,anbhp,anqhp
     $                   ,spcthp,spcthp1,spcthp2,spare2,intdhp)
      endif
c----6----------------------------------------------------------------72>
c second part
c----6----------------------------------------------------------------72>
c
c>> Image Record for Rectified
c
      if(irec .le. 4) go to 900
      if (imagline .ne. 0) then
	lrec=2
10      continue
        call first_rectif(lrec,short,fn2lp,yy2lp,dd2lp,sl2lp,dt2lp
     $                   ,ipr2lp,spc2lp,reserved,blk2lp,nbl2lp
     $                   ,nhb2lp,ty2lp,sn2lp,reserved2,jslot
     $                   ,lastl,vic2lp,irc2lp,wvc2lp
     $                   ,milsec,vispor,fishre)
        if(sl2lp .eq. imagline) then
          call second_rectif(lrec,offset,julslt,imgqua,ndgrpt,lstart
     $                      ,lend,lstep,defmax,defmay,cspare)
          call third_rectif(lrec,offset,jslohp,jdayhp,slohp,nithp
     $                     ,slothp,gridhp,saidhp,sspxhp,sspyhp,mltuhp
     $                     ,mltvhp,mltihp,mltwhp,anbhp,anqhp
     $                     ,spcthp,spcthp1,spcthp2,spare2,intdhp)
c
	call header_wrt(label,yr,day,slot,dty,blz,nbl,nhb,off,nslt,ndslt
     $                 ,ofs,ibgwnd,nblwnd,ibgpix,nblpix,namdate,setnam
     $                 ,arcidt,photo1,photo2,photo3,photo4,ecdesi
     $                 ,ecpoii1,ecpoii2,ecpoii3,ecpoii4,ecpoii5,ecdesv
     $                 ,ecpoiv1,ecpoiv2,ecpoiv3,ecpoiv4,ecpoiv5,ecdesw
     $                 ,ecpoiw1,ecpoiw2,ecpoiw3,ecpoiw4,ecpoiw5)
c
        call header_first(fn2lp,yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp
     $                   ,blk2lp,nbl2lp,nhb2lp,ty2lp,sn2lp,jslot,lastl
     $                   ,vic2lp,irc2lp,wvc2lp,milsec,vispor,fishre)
c
        call header_sec(julslt,imgqua,ndgrpt,lstart,lend,lstep
     $                 ,defmax,defmay)
c
        call header_third(jslohp,jdayhp,slohp,nithp,slothp,gridhp
     $                  ,saidhp,sspxhp,sspyhp,thrhp,anbhp,anqhp
     $                  ,spcthp,intdhp
     $                  ,nblhp,mltwhp,mltvhp,mltuhp,mltihp)
        call header_imgrec(jslot,lastl2,visid,wvid,irid)
cccccccccccccccccccccccccccccccccccccccc
          call get_records(rec_per_blk,offset,ijslot,ilast,ivisid
     $                    ,iwvid,iirid,notused,vis1,vis2,irdat,wvdat
     $                    ,sl2lp,lrec,imagline)
          go to 900
        endif
	lrec=lrec+1
	go to 10
      endif
900   return
      end
c***********************************************************************
c----6----------------------------------------------------------------72>
      subroutine get_records(rec_per_blk,offset,ijslot,ilast,ivisid
     $                      ,iwvid,iirid,notused,vis1,vis2,irdat,wvdat
     $                      ,sl2lp,lrec,imagline)
c>>integer
      integer*2 offset,ilast(*),ivisid(*),iirid(*),iwvid(*)
      integer*4 rec_per_blk,lrec,sl2lp,ii,imagline
      integer*4 ijslot(*)
c>>character
c>>logical
      logical*1 notused(*),vis1(1504,5),vis2(1504,5),irdat(752,5)
      logical*1 wvdat(752,5)
c
          rec_per_blk = 23400 / (160 + nblpix * 6)
          if (rec_per_blk .gt. 5) then
            print*,"Error there are more records on the file than"
            print*," the program can handle.!!"
            stop
           endif
c>>1
         lrec = lrec + 8
	 do 10 ii = 1 ,115
         read(1,rec=lrec,err=999)offset,ijslot(1),ilast(1),ivisid(1)
     $                          ,iwvid(1),iirid(1),(notused(i),i=1,148)
     $                          ,(vis1(i,1),i=1,1504)
     $                          ,(vis2(i,1),i=1,1504)
     $                          ,(irdat(i,1),i=1,752)
     $                          ,(wvdat(i,1),i=1,752)
c>>2
     $   ,ijslot(2),ilast(2),ivisid(2),iwvid(2),iirid(2)
     $   ,(notused(i),i=1,148),(vis1(i,2),i=1,1504)
     $   ,(vis2(i,2),i=1,1504)
     $   ,(irdat(i,2),i=1,752),(wvdat(i,2),i=1,752)
c>>3
     $   ,ijslot(3),ilast(3),ivisid(3),iwvid(3),iirid(3)
     $   ,(notused(i),i=1,148),(vis1(i,3),i=1,1504)
     $   ,(vis2(i,3),i=1,1504)
     $   ,(irdat(i,3),i=1,752),(wvdat(i,3),i=1,752)
c>>4
     $   ,ijslot(4),ilast(4),ivisid(4),iwvid(4),iirid(4)
     $   ,(notused(i),i=1,148),(vis1(i,4),i=1,1504)
     $   ,(vis2(i,4),i=1,1504),(irdat(i,4),i=1,752),(wvdat(i,4),i=1,752)
c>>5
     $   ,ijslot(5),ilast(5),ivisid(5),iwvid(5),iirid(5)
     $   ,(notused(i),i=1,148),(vis1(i,5),i=1,1504),(vis2(i,5),i=1,1504)
     $   ,(irdat(i,5),i=1,752),(wvdat(i,5),i=1,752)
c
      call writepix(imagline)
      lrec = lrec + 1
10    continue
      imagline = 999
      go to 900
999   print*,"Error on read"
      stop 
900   return
      end
c***********************************************************************
c----6----------------------------------------------------------------72>
      subroutine header_rec
      integer*2 short,nslt,ndslt,nslt1,ndslt1
      integer*2 nslt2,ndslt2,nslt3,ndslt3,nslt4,ndslt4
      integer*2 nslt5,ndslt5,nslt6,ndslt6,nslt7,ndslt7
      integer*2 nslt8,ndslt8,nslt9,ndslt9,nslt10,ndslt10
      integer*2 nslt11,ndslt11,nslt12,ndslt12,nslt13,ndslt13
      integer*2 nslt14,ndslt14,nslt15,ndslt15,nslt16,ndslt16
      integer*2 nslt17,ndslt17,nslt18,ndslt18,nslt19,ndslt19
      integer*2 nslt20,ndslt20,nslt21,ndslt21,nslt22,ndslt22
      integer*2 nslt23,ndslt23
      integer*2 vispor,lastl2
      integer*2 offset,jdayhp,slohp,nithp,slothp,gridhp
      integer*2 sspxhp,sspyhp,thrhp,nblhp,spcthp
      integer*2 spcthp1,spcthp2,lastl,visid,wvid,irid
      integer*4 rlen ,nrecs,jslot,yr,day,slot,dty,dum1,dum2,dum3,dum4
      integer*4 blz,nbl,nhb,off,ofs,ofs1,ofs2,ofs3,ofs4,ofs5,ofs6,ofs7
      integer*4 ofs8,ofs9,ofs10,ofs11,ofs12,ofs13,ofs14,ofs15,ofs16
      integer*4 ofs17,ofs18,ofs19,ofs20,ofs21,ofs22,ofs23
      integer*4 ibgwnd,nblwnd,ibgpix,nblpix,mbywnd,mbypix,nslots
      integer*4 yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,blk2lp
      integer*4 nhb2lp,milsec,fishre
      integer*4 julslt,imgqua,ndgrpt,lstart,lend,lstep,jslohp

      character cspare*5418,anbhp*28,reserved1*92
      character val*2,reserved3*2,spare1*2,saidhp*2,sn2lp*2
      character ecdesi*4,ecdesv*4,ecdesw*4,satidl*4,fn2lp*8
      character vslot*5,anqhp*12,value28*28,reserved*12
      character setnam*6,setemp*6,setend*6,value2*8,ty2lp*8
      character label*8,arcidt*8,infile*8,value*8,value4*8
c----6----------------------------------------------------------------72>
      character value1*12,ecpoii1*12,ecpoii2*12,ecpoii3*12,ecpoii4*12
      character ecpoii5*12,ecpoiv1*12,ecpoiv2*12,ecpoiv3*12,ecpoiv4*12
      character ecpoiv5*12,ecpoiw1*12,ecpoiw2*12,ecpoiw3*12,ecpoiw4*12
      character ecpoiw5*12,filename*40,fileout*40
      character photo1*16,photo2*16,photo3*16,photo4*16
      character spare*24,ans*3
      character namdate*26
      character ecvali*256,ecvalv*256,ecvalw*256
 
      logical*1 store,mltuhp,mltvhp,mltihp,mltwhp,vic2lp,irc2lp
      logical*1 wvc2lp,intdhp,notused

      common /logical/store(24),mltuhp(316),mltvhp(316),mltihp(316)
     $ ,mltwhp(316),vic2lp(4),irc2lp(2),wvc2lp(2),intdhp(1360)
     $ ,notused(148)
      common /inttwo/ short,nslt,ndslt,nslt1,ndslt1,
     $  nslt2,ndslt2,nslt3,ndslt3,nslt4,ndslt4,nslt5,
     $  ndslt5,nslt6,ndslt6,nslt7,ndslt7,nslt8,ndslt8,nslt9,ndslt9,
     $  nslt10,ndslt10,nslt11,ndslt11,nslt12,ndslt12,nslt13,ndslt13,
     $  nslt14,ndslt14,nslt15,ndslt15,nslt16,ndslt16,nslt17,ndslt17,
     $  nslt18,ndslt18,nslt19,ndslt19,nslt20,ndslt20,nslt21,ndslt21,
     $  nslt22,ndslt22,nslt23,ndslt23,offset,jdayhp,slohp,nithp,
     $  slothp,gridhp,sspxhp,sspyhp,thrhp,nblhp(3,3),spcthp(3),spcthp1,
     $  spcthp2,lastl,visid,wvid,irid,vispor(2),lastl2
 
c----6----------------------------------------------------------------72>
      common /intfour/ rlen,nrecs,jslot,yr,day,slot,dty,dum1,dum2,dum3,
     $  dum4,blz,nbl,nhb,off,ofs,ofs1,ofs2,ofs3,ofs4,ofs5,ofs6,ofs7,
     $  ofs8,ofs9,ofs10,ofs11,ofs12,ofs13,ofs14,ofs15,ofs16,ofs17,
     $  ofs18,ofs19,ofs20,ofs21,ofs22,ofs23,ibgwnd,nblwnd,ibgpix,nblpix,
     $  mbywnd,mbypix,nslots,yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,
     $  blk2lp,nbl2lp,nhb2lp,milsec,fishre(4),julslt,imgqua,
     $  ndgrpt,lstart,lend,lstep,jslohp
 
      common/char/ val,reserved3,spare1,saidhp,ans,ecdesi,ecdesv,
     $ ecdesw,satidl,vslot,setnam,setemp,setend,value,value4,value2,
     $ label,arcidt,infile,reserved,fn2lp,value1,ecpoii1,ecpoii2,
     $ ecpoii3,ecpoii4,ecpoii5,ecpoiv1,ecpoiv2,ecpoiv3,ecpoiv4,ecpoiv5,
     $ ecpoiw1,ecpoiw2,ecpoiw3,ecpoiw4,ecpoiw5,photo1,photo2,photo3,
     $ photo4,spare,namdate,value28,anbhp,filename,fileout,reserved1,
     $ ecvali,ecvalv,ecvalw,cspare,ty2lp,sn2lp,anqhp
c**********************************************************************
c>> File Header Record
      read(1,rec=1,err=900)short,label,yr,day,slot,dty,dum1,dum2,dum3
     $ ,dum4,blz,nbl,nhb,off,spare,nslt,ndslt,ofs,nslt1,ndslt1,ofs1
     $ ,nslt2,ndslt2,ofs2,nslt3,ndslt3,ofs3,nslt4,ndslt4,ofs4,nslt5
     $ ,ndslt5,ofs5,nslt6,ndslt6,ofs6,nslt7,ndslt7,ofs7,nslt8,ndslt8
     $ ,ofs8,nslt9,ndslt9,ofs9,nslt10,ndslt10,ofs10,nslt11,ndslt11,ofs11
     $ ,nslt12,ndslt12,ofs12,nslt13,ndslt13,ofs13,nslt14,ndslt14,ofs14
     $ ,nslt15,ndslt15,ofs15,nslt16,ndslt16,ofs16,nslt17,ndslt17,ofs17
     $ ,nslt18,ndslt18,ofs18,nslt19,ndslt19,ofs19,nslt20,ndslt20,ofs20
     $ ,nslt21,ndslt21,ofs21,nslt22,ndslt22,ofs22,nslt23,ndslt23,ofs23
     $ ,ibgwnd,nblwnd,ibgpix,nblpix,namdate,setnam,arcidt,photo1,photo2
     $ ,photo3,photo4,ecdesi,ecpoii1,ecpoii2,ecpoii3,ecpoii4,ecpoii5
     $ ,ecvali,ecdesv,ecpoiv1,ecpoiv2,ecpoiv3,ecpoiv4,ecpoiv5,ecvalv
     $ ,ecdesw,ecpoiw1,ecpoiw2,ecpoiw3,ecpoiw4,ecpoiw5,ecvalw,infile
     $ ,mbywnd,mbypix,nslots,(store(i),i=1,24),vslot,setemp,setend
c----6----------------------------------------------------------------72>
       length=2
       value1(1:12)=" "
       call ebcdtoa(sn2lp,value1,length)
900   return
      end
c**********************************************************************
c----6----------------------------------------------------------------72>
      subroutine first_rectif(lrec,short,fn2lp,yy2lp,dd2lp,sl2lp,dt2lp
     $                       ,ipr2lp,spc2lp,reserved,blk2lp,nbl2lp
     $                       ,nhb2lp,ty2lp,sn2lp,reserved2,jslot
     $                       ,lastl,vic2lp,irc2lp,wvc2lp
     $                       ,milsec,vispor,fishre)
c>>integer
      integer*2 short,lastl
      integer*2 vispor(2)
      integer*4 yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,blk2lp
      integer*4 nbl2lp,nhb2lp,milsec,fishre(4)
      integer*4 jslot,lrec
c>>character
      character fn2lp*8,reserved3*2,reserved*8,reserved1*12
      character ty2lp*8,sn2lp*2,reserved2*6
c>>logical
      logical*1 vic2lp(4),irc2lp(2)
      logical*1 wvc2lp(2)
c
c>> First Header for Rectified
c   RECT2LP
c----6----------------------------------------------------------------72>
      read(1,rec=lrec,err=900)short,fn2lp,yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp
     $ ,spc2lp,reserved,blk2lp,nbl2lp,nhb2lp,reserved1,ty2lp,sn2lp
     $ ,reserved2,jslot,lastl,(vic2lp(i),i=1,4),(irc2lp(i),i=1,2)
     $ ,(wvc2lp(i),i=1,2),reserved3,milsec,(vispor(i),i=1,2)
     $ ,(fishre(i),i=1,4)
900   return
      end
c**********************************************************************
c----6----------------------------------------------------------------72>
      subroutine second_rectif(lrec,offset,julslt,imgqua,ndgrpt
     $                        ,lstart,lend,lstep,defmax,defmay,cspare)
c>>integer
      integer*2 offset
      integer*4 julslt,imgqua,ndgrpt,lstart,lend,lstep,lrec
c>>real*4
      real*4 defmax(26,26),defmay(26,26)
c>>char
      character cspare*5418
c>>logical
c
c>> Second Header for Rectified
c>> IMAGE HEADER 2 - Deformation Data
c
       lrec = lrec + 1
       read(1,rec=lrec,err=900)offset,julslt,imgqua,ndgrpt,lstart,lend
     $ ,lstep,((defmax(j,i),j=1,26),i=1,26)
     $ ,((defmay(j,i),j=1,26),i=1,26),cspare
c    ,satidl,satidh,spare,rmij
900   return
      end
c************************************************************************
c----6----------------------------------------------------------------72>
      subroutine third_rectif(lrec,offset,jslohp,jdayhp,slohp,nithp
     $                       ,slothp,gridhp,saidhp,sspxhp,sspyhp,mltuhp
     $                       ,mltvhp,mltihp,mltwhp,anbhp,anqhp
     $                       ,spcthp,spcthp1,spcthp2,spare2,intdhp)
c>>integer
      integer*2 offset,jdayhp,slohp,nithp,slothp,gridhp
      integer*2 sspxhp,sspyhp,thrhp,nblhp(3,3),spcthp(3)
      integer*4 jslohp
      integer*4 lrec
c>>char
      character saidhp*2,anbhp*28,anqhp*12
c>>logical
      logical*1 mltuhp(316),mltvhp(316),mltihp(316),mltwhp(316)
      logical*1 intdhp(1360)
c
c>> Third Header for Rectified
c>> IMAGE HEADER 3 - Dissemination Data
c
       lrec = lrec + 1
c----6----------------------------------------------------------------72>
       read(1,rec=lrec,err=900)offset,jslohp,jdayhp,slohp,nithp,slothp,
     $               gridhp,saidhp,sspxhp,sspyhp,(mltuhp(i),i=1,316)
     $                     ,(mltvhp(i),i=1,316)
     $ ,(mltihp(i),i=1,316),(mltwhp(i),i=1,316)
     $ ,thrhp,((nblhp(j,i),j=1,3),i=1,3),anbhp,anqhp,(spcthp(i),i=1,3)
     $ ,reserved,(intdhp(i),i=1,1300)
c
c>> Header records 4 thru 10 are not used.
c
900   return
      end
c***********************************************************************
c convert ebcdic to ascii

      subroutine ebcdtoa( ebcstring, ascstring, length )
      character*1 ebcstring(1:length)
      character*1 ascstring(1:length) , astring
      dimension ntab(256)
      data ntab/
     a 000,001,002,003,156,009,134,127,151,141,142,011,012,013,014,015,
     b 016,017,018,019,157,133,008,135,024,025,146,143,028,029,030,031,
     c 128,129,130,131,132,010,023,027,136,137,138,139,140,005,006,007,
     d 144,145,022,147,148,149,150,004,152,153,154,155,020,021,158,026,
     e 032,160,161,162,163,164,165,166,167,168,213,046,060,040,043,124,
     f 038,169,170,171,172,173,174,175,176,177,033,036,042,041,059,094,
     g 045,047,178,179,180,181,182,183,184,185,229,044,037,095,062,063,
     h 186,187,188,189,190,191,192,193,194,096,058,035,064,039,061,034,
     i 195,097,098,099,100,101,102,103,104,105,196,197,198,199,200,201,
     j 202,106,107,108,109,110,111,112,113,114,203,204,205,206,207,208,
     k 209,126,115,116,117,118,119,120,121,122,210,211,212,091,214,215,
     l 216,217,218,219,220,221,222,223,224,225,226,227,228,093,230,231,
     m 123,065,066,067,068,069,070,071,072,073,232,233,234,235,236,237,
     n 125,074,075,076,077,078,079,080,081,082,238,239,240,241,242,243,
     o 092,159,083,084,085,086,087,088,089,090,244,245,246,247,248,249,
     p 048,049,050,051,052,053,054,055,056,057,250,251,252,253,254,255/

      do 10 i=1,length
      astring=char(ntab(ichar(ebcstring(i))+1))
c      write(*,400)  i  ,ebcstring(i) ,astring
      ascstring(i)=astring
   10 continue

  400 format( 2x , i2 , 4a1 , a4 )

      return
      end
c----6----------------------------------------------------------------72>
c***********************************************************************
      subroutine process
      character val*2,reserved3*2,spare1*2,saidhp*2
      character ans*3,sn2lp*2,response*5
      character ecdesi*4,ecdesv*4,ecdesw*4,satidl*4
      character vslot*5
      character setnam*6,setemp*6,setend*6
      character value*8,value4*8,value2*8,label*8,arcidt*8,infile*8
      character reserved*12
      character ty2lp*8
      character fn2lp*8
      character value1*12,ecpoii1*12,ecpoii2*12,ecpoii3*12,ecpoii4*12
      character ecpoii5*12,ecpoiv1*12,ecpoiv2*12,ecpoiv3*12,ecpoiv4*12
      character ecpoiv5*12,ecpoiw1*12,ecpoiw2*12,ecpoiw3*12,ecpoiw4*12
      character ecpoiw5*12,anqhp*12
      character photo1*16,photo2*16,photo3*16,photo4*16
      character spare*24
      character namdate*26
      character value28*28,anbhp*28
      character reserved1*92
      character ecvali*256,ecvalv*256,ecvalw*256
      character filename*40,fileout*40
      character cspare*5418
c
c>> define logical variables
c
      logical*1 store,mltuhp,mltvhp,mltihp,mltwhp,vic2lp,irc2lp
      logical*1 wvc2lp,intdhp,notused,vis1,vis2,irdat,wvdat
      logical*1 done
c---------------------------------------------------------------------->
c
c>> define integer*2 variables
c
      integer*2 short,nslt,ndslt,nslt1,ndslt1
      integer*2 nslt2,ndslt2,nslt3,ndslt3,nslt4,ndslt4
      integer*2 nslt5,ndslt5,nslt6,ndslt6,nslt7,ndslt7
      integer*2 nslt8,ndslt8,nslt9,ndslt9,nslt10,ndslt10
      integer*2 nslt11,ndslt11,nslt12,ndslt12,nslt13,ndslt13
      integer*2 nslt14,ndslt14,nslt15,ndslt15,nslt16,ndslt16
      integer*2 nslt17,ndslt17,nslt18,ndslt18,nslt19,ndslt19
      integer*2 nslt20,ndslt20,nslt21,ndslt21,nslt22,ndslt22
      integer*2 nslt23,ndslt23,ilast,ivisid,iwvid,iirid
      integer*2 offset,jdayhp,slohp,nithp,slothp,gridhp
      integer*2 sspxhp,sspyhp,thrhp,nblhp,spcthp
      integer*2 spcthp1,spcthp2,lastl,visid,wvid,irid
      integer*2 vispor,lastl2
c
c>> define integer*4 variables
c
      integer*4 rlen ,nrecs,jslot,yr,day,slot,dty,dum1,dum2,dum3,dum4
      integer*4 blz,nbl,nhb,off,ofs,ofs1,ofs2,ofs3,ofs4,ofs5,ofs6,ofs7
      integer*4 ofs8,ofs9,ofs10,ofs11,ofs12,ofs13,ofs14,ofs15,ofs16
      integer*4 ofs17,ofs18,ofs19,ofs20,ofs21,ofs22,ofs23
      integer*4 ibgwnd,nblwnd,ibgpix,nblpix,mbywnd,mbypix,nslots
      integer*4 yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,blk2lp
      integer*4 nbl2lp,nhb2lp,milsec,fishre
      integer*4 julslt,imgqua,ndgrpt,lstart,lend,lstep,jslohp
      integer*4 imagline,length

c
c>> define real*4 variables
c
      real*4 defmax,defmay,rmij,rsij,attf,attl,pixmid,linmid

c
c>> define real*8 variables
      real*8 gretg,grptg,ghtg,f1satg,orbf,orbl,forbs,forbn
c
c
c>> setup common with character variables
c
c----6----------------------------------------------------------------72>
      common/char/ val,reserved3,spare1,saidhp,ans,ecdesi,ecdesv,
     $ ecdesw,satidl,vslot,setnam,setemp,setend,value,value4,value2,
     $ label,arcidt,infile,reserved,fn2lp,value1,ecpoii1,ecpoii2,
     $ ecpoii3,ecpoii4,ecpoii5,ecpoiv1,ecpoiv2,ecpoiv3,ecpoiv4,ecpoiv5,
     $ ecpoiw1,ecpoiw2,ecpoiw3,ecpoiw4,ecpoiw5,photo1,photo2,photo3,
     $ photo4,spare,namdate,value28,anbhp,filename,fileout,reserved1,
     $ ecvali,ecvalv,ecvalw,cspare,ty2lp,sn2lp,anqhp
c
c>> setup logical variables in common
c
      common /logical/store(24),mltuhp(316),mltvhp(316),mltihp(316)
     $ ,mltwhp(316),vic2lp(4),irc2lp(2),wvc2lp(2),intdhp(1360)
     $ ,notused(148)

c
c>> setup integer*2 variables in common
c
      common /inttwo/ short,nslt,ndslt,nslt1,ndslt1,
     $  nslt2,ndslt2,nslt3,ndslt3,nslt4,ndslt4,nslt5,
     $  ndslt5,nslt6,ndslt6,nslt7,ndslt7,nslt8,ndslt8,nslt9,ndslt9,
     $  nslt10,ndslt10,nslt11,ndslt11,nslt12,ndslt12,nslt13,ndslt13,
     $  nslt14,ndslt14,nslt15,ndslt15,nslt16,ndslt16,nslt17,ndslt17,
     $  nslt18,ndslt18,nslt19,ndslt19,nslt20,ndslt20,nslt21,ndslt21,
     $  nslt22,ndslt22,nslt23,ndslt23,offset,jdayhp,slohp,nithp,
     $  slothp,gridhp,sspxhp,sspyhp,thrhp,nblhp(3,3),spcthp(3),spcthp1,
     $  spcthp2,lastl,visid,wvid,irid,vispor(2),lastl2
c---------------------------------------------------------------------->
c
c>> setup integer*4 variables in common
c
      common /intfour/ rlen,nrecs,jslot,yr,day,slot,dty,dum1,dum2,dum3,
     $  dum4,blz,nbl,nhb,off,ofs,ofs1,ofs2,ofs3,ofs4,ofs5,ofs6,ofs7,
     $  ofs8,ofs9,ofs10,ofs11,ofs12,ofs13,ofs14,ofs15,ofs16,ofs17,
     $  ofs18,ofs19,ofs20,ofs21,ofs22,ofs23,ibgwnd,nblwnd,ibgpix,nblpix,
     $  mbywnd,mbypix,nslots,yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,
     $  blk2lp,nbl2lp,nhb2lp,milsec,fishre(4),julslt,imgqua,
     $  ndgrpt,lstart,lend,lstep,jslohp

c
c>> setup real*4 variables in common
c
      common /realfour/ defmax(26,26),defmay(26,26),rmij(2,8,2),
     $  rsij(2,8,2),attf,attl,pixmid,linmid
c
c>> setup real*8 variables in common
c
      common /realeight/gretg,grptg,ghtg,f1satg,orbf,orbl,forbs,
     $  forbn
c
      common/imgrec/ijslot(5),ilast(5),ivisid(5),iwvid(5),
     $              iirid(5),vis1(1504,5),vis2(1504,5),
     $              irdat(752,5),wvdat(752,5)
      response = " "
c
c>> write a header to the scratch data file
cFile 3
      write(3,*)" "
      write(3,*)"                               METEOSAT" 
      write(3,*)"                (Real Time Window File Scratch File)"
c----6----------------------------------------------------------------72>
      write(3,*)" "
c
      if(ans .eq. "3") then
	call dump_rec
	write(*,55)fileout
55      format("Output is written to file ----> ",a)
	print*,"Normal termination of program."
        print*," "
	stop
      endif
      if (ans .eq. "2") then
40	print*,"Enter the number of the Slot you wish to see"
	print*,"written to the Output File."
	print*,"[1 thru  48]"
        read(*,20)response
	do 5 i = 1 ,5
	  if(response(i:i) .eq. " ") then
	    length = i
	    if (i .gt. 1) then
	      length = i - 1
	      go to 6
            endif
          endif
5       continue
6       continue
        if((response(1:length) .lt. "1") .or. 
     $    (response(1:length) .gt. "48")) then
	  print*,"This is not a valid entry --> ",response
	  print*,"Please try again"
	    go to 40
          endif
	  if(length .eq. 1) then
	    read(response,"(i1)")imagline
          else if(length .eq. 2) then
	    read(response,"(i2)")imagline
          endif
	if((imagline .le. 0) .or. (imagline .gt. 48)) then
	  print*,"This is not a valid entry ",imagline
	  print*,"Please try again"
	  go to 40
        endif	  
	print*,"Slot number ",imagline," will now be dumped."
	print*,"Thank you for waiting the program is"
	print*,"reading the data now!!"
20      format(a)
      endif
      irec=1
      do while(done .ne. .TRUE.)
        call readfile(irec,imagline)
	if(imagline .eq. 999) go to 900
        irec = irec + 1
        length=6
        value1(1:12)=" "
        call ebcdtoa(setnam,value1,length)
c
c>> write out the header file
c
      if ((ans .ne. "2") .and. (irec .eq. 4)) then
c----6----------------------------------------------------------------72>
	call header_wrt(label,yr,day,slot,dty,blz,nbl,nhb,off,nslt,ndslt
     $                 ,ofs,ibgwnd,nblwnd,ibgpix,nblpix,namdate,setnam
     $                 ,arcidt,photo1,photo2,photo3,photo4,ecdesi
     $                 ,ecpoii1,ecpoii2,ecpoii3,ecpoii4,ecpoii5,ecdesv
     $                 ,ecpoiv1,ecpoiv2,ecpoiv3,ecpoiv4,ecpoiv5,ecdesw
     $                 ,ecpoiw1,ecpoiw2,ecpoiw3,ecpoiw4,ecpoiw5)
c
        call header_first(fn2lp,yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp
     $                   ,blk2lp,nbl2lp,nhb2lp,ty2lp,sn2lp,jslot,lastl
     $                   ,vic2lp,irc2lp,wvc2lp,milsec,vispor,fishre)
c
        call header_sec(julslt,imgqua,ndgrpt,lstart,lend,lstep
     $                 ,defmax,defmay)
c
        saidhp = " "
	anqhp = " "
        call header_third(jslohp,jdayhp,slohp,nithp,slothp,gridhp
     $                  ,saidhp,sspxhp,sspyhp,thrhp,anbhp,anqhp
     $                  ,spcthp,intdhp
     $                  ,nblhp,mltwhp,mltvhp,mltuhp,mltihp)
	go to 900
      endif
      end do
900   return
      end
c----6----------------------------------------------------------------72>
c***********************************************************************
      subroutine writepix(imagline)
c
c>> define logical variables
c
      logical*1 store,mltuhp,mltvhp,mltihp,mltwhp,vic2lp,irc2lp
      logical*1 wvc2lp,intdhp,notused,vis1,vis2,irdat,wvdat
c---------------------------------------------------------------------->
c
c>> define integer*2 variables
c
      integer*2 short,nslt,ndslt,nslt1,ndslt1
      integer*2 nslt2,ndslt2,nslt3,ndslt3,nslt4,ndslt4
      integer*2 nslt5,ndslt5,nslt6,ndslt6,nslt7,ndslt7
      integer*2 nslt8,ndslt8,nslt9,ndslt9,nslt10,ndslt10
      integer*2 nslt11,ndslt11,nslt12,ndslt12,nslt13,ndslt13
      integer*2 nslt14,ndslt14,nslt15,ndslt15,nslt16,ndslt16
      integer*2 nslt17,ndslt17,nslt18,ndslt18,nslt19,ndslt19
      integer*2 nslt20,ndslt20,nslt21,ndslt21,nslt22,ndslt22
      integer*2 nslt23,ndslt23,ilast,ivisid,iwvid,iirid
      integer*2 offset,jdayhp,slohp,nithp,slothp,gridhp
      integer*2 sspxhp,sspyhp,thrhp,nblhp,spcthp
      integer*2 spcthp1,spcthp2,lastl,visid,wvid,irid
      integer*2 vispor,lastl2
c
c
c>> define integer*4 variables
c
      integer*4 imagline,i,j
      integer*4 rlen ,nrecs,ijslot,yr,day,slot,dty,dum1,dum2
      integer*4 dum3,dum4,ofs5,ofs6,ofs7,ofs15,ofs16
      integer*4 blz,nbl,nhb,off,ofs,ofs1,ofs2,ofs3,ofs4
      integer*4 ofs8,ofs9,ofs10,ofs11,ofs12,ofs13,ofs14
      integer*4 ofs17,ofs18,ofs19,ofs20,ofs21,ofs22,ofs23
      integer*4 ibgwnd,nblwnd,ibgpix,nblpix,mbywnd,mbypix,nslots
      integer*4 yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,blk2lp
      integer*4 nbl2lp,nhb2lp,milsec,fishre
      integer*4 julslt,imgqua,ndgrpt,lstart,lend,lstep,jslohp
c
c
c>> setup logical variables in common
c
      common /logical/store(24),mltuhp(316),mltvhp(316),mltihp(316)
     $ ,mltwhp(316),vic2lp(4),irc2lp(2),wvc2lp(2),intdhp(1360)
     $ ,notused(148)
c
c
c>> setup integer*2 variables in common
c
      common /inttwo/ short,nslt,ndslt,nslt1,ndslt1,
     $  nslt2,ndslt2,nslt3,ndslt3,nslt4,ndslt4,nslt5,
     $  ndslt5,nslt6,ndslt6,nslt7,ndslt7,nslt8,ndslt8,nslt9,ndslt9,
     $  nslt10,ndslt10,nslt11,ndslt11,nslt12,ndslt12,nslt13,ndslt13,
     $  nslt14,ndslt14,nslt15,ndslt15,nslt16,ndslt16,nslt17,ndslt17,
     $  nslt18,ndslt18,nslt19,ndslt19,nslt20,ndslt20,nslt21,ndslt21,
     $  nslt22,ndslt22,nslt23,ndslt23,offset,jdayhp,slohp,nithp,
     $  slothp,gridhp,sspxhp,sspyhp,thrhp,nblhp(3,3),spcthp(3),spcthp1,
     $  spcthp2,lastl,visid,wvid,irid,vispor(2),lastl2
c
c
c>> setup integer*4 variables in common
c
      common /intfour/ rlen,nrecs,jslot,yr,day,slot,dty,dum1,dum2,dum3,
     $  dum4,blz,nbl,nhb,off,ofs,ofs1,ofs2,ofs3,ofs4,ofs5,ofs6,ofs7,
     $  ofs8,ofs9,ofs10,ofs11,ofs12,ofs13,ofs14,ofs15,ofs16,ofs17,
     $  ofs18,ofs19,ofs20,ofs21,ofs22,ofs23,ibgwnd,nblwnd,ibgpix,nblpix,
     $  mbywnd,mbypix,nslots,yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,
     $  blk2lp,nbl2lp,nhb2lp,milsec,fishre(4),julslt,imgqua,
     $  ndgrpt,lstart,lend,lstep,jslohp
c
      common/imgrec/ijslot(5),ilast(5),ivisid(5),iwvid(5),
     $              iirid(5),vis1(1504,5),vis2(1504,5),
     $              irdat(752,5),wvdat(752,5)
c
      do 9998 j = 1 ,5
      write(2,78)ijslot(j),ilast(j)
c       write(2,78)ijslot(j),ilast(j),ivisid(j),iwvid(j),iirid(j)
78    format(/,"jslot =   ",i8,10x,"| Julian slot number.",/
     $ ,"lastl =   ",i10,8x,"| Image line number.",/)
c    $ ,"visid =   ",i10,8x,"| VIS channel indicator"/
c    $ ,28x,"| 0=off; 1=vis1; 2=vis2; 3=vis1+vis2",/
c    $ ,"wvid =   ",i10,9x,"| WV channel indicator. 0=off; 6=wv",/
c    $ ,"irid =   ",i10,9x,"| IR channel indicator. ",/
c    $ ,28x,"| 0=off; 4=ir1; 5=ir2")
	write(2,33)
33      format(/,"vis1 =",22x,"| Southern VIS",/80("-"))
	do 11 i = 1 ,1504
 	  write(2,333)vis1(i,j)
333       format(i4,$)
	  if(mod(i,20) .eq. 0) then
 	    write(2,*) " "
          endif
11      continue
	write(2,43)
43      format(/,"vis2 =",22x,"| Northern VIS",/80("-"))
	do 14 i = 1 ,1504
 	  write(2,334)vis2(i,j)
334       format(i4,$)
	  if(mod(i,20) .eq. 0) then
 	    write(2,*) " "
          endif
14      continue
	write(2,44)
44      format(/,"ir =",24x,"| IR",/80("-"))
	do 17 i = 1 ,752
 	  write(2,337)irdat(i,j)
337       format(i4,$)
	  if(mod(i,20) .eq. 0) then
 	    write(2,*) " "
          endif
17      continue
	write(2,54)
54      format(/,"wv =",24x,"| WV",/80("-"))
	do 179 i = 1 ,752
 	  write(2,338)wvdat(i,j)
338       format(i4,$)
	  if(mod(i,20) .eq. 0) then
 	    write(2,*) " "
          endif
179      continue
        write(2,*) " "
9998   continue
900    return
      end
c----6----------------------------------------------------------------72>
c***********************************************************************
      subroutine dump_rec
c>>integer
      integer*2 short,idone
      integer*4 yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,length,irec
      integer*4 rec_count
c>>character	
      character fn2lp*8,value*8
c>>logical
c
c>> First Header for Rectified
c   RECT2LP
      irec=2
      rec_count=0
      write(2,*)
      write(2,*)"                               METEOSAT "
      write(2,*)"                       (Real Time Window File)"
      write(2,*)"          Summary of 1st Header for RECTIFIED showing"
     $ ," Slot"," numbers."
      write(2,7)
      write(2,*)
      do while(idone .ne. 1)
c----$---------------------------------------------------------------72>
      read(1,rec=irec,err=900)short,fn2lp,yy2lp,dd2lp,sl2lp,dt2lp
     $                       ,ipr2lp
      rec_count = rec_count + 1
      length=8
      value(1:8)=" "
      call ebcdtoa(fn2lp,value,length)
      fn2lp = value
      write(2,77)rec_count,fn2lp,yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp
      write(2,8)
      irec= irec + 125
      end do
900   return
7     format(80("*"))
8     format(80("-"))
77    format("                                      rec_count = ",i4,/
     $"fn2lp  =  ",a,20x,"| File name.",/
     $"yy2lp  =   ",i4,23x,"| Year",/
     $"dd2lp  =   ",i4,23x,"| Day",/
     $"sl2lp  =   ",i4,23x,"| Slot number.",/
     $"dt2lp  =   ",i4,23x,"| Data type.",/
     $"ipr2lp =   ",i4,23x,"| Image process performed."/)
      end
c----$---------------------------------------------------------------72>
c*********************************************************************
      subroutine header_wrt(label,yr,day,slot,dty,blz,nbl,nhb,off,nslt
     $                     ,ndslt,ofs,ibgwnd,nblwnd,ibgpix,nblpix
     $                     ,namdate,setnam,arcidt,photo1,photo2,photo3
     $                     ,photo4,ecdesi,ecpoii1,ecpoii2,ecpoii3
     $                     ,ecpoii4,ecpoii5,ecdesv,ecpoiv1
     $                     ,ecpoiv2,ecpoiv3,ecpoiv4,ecpoiv5,ecdesw
     $                     ,ecpoiw1,ecpoiw2,ecpoiw3,ecpoiw4,ecpoiw5)
c>>integer
c 
      integer*2 nslt 
      integer*4 yr,day,slot,dty,ofs,ibgwnd,nblwnd
      integer*4 ibgpix,nblpix
c>>character
      character label*8,arcidt*8
      character value*8,value28*28,setnam*6
      character photo1*16,photo2*16,photo3*16,photo4*16
      character ecdesi*4,ecdesv*4,ecdesw*4
      character ecpoiv5*12,ecpoiw1*12,ecpoiw2*12,ecpoiw3*12,ecpoiw4*12
      character value1*12,ecpoii1*12,ecpoii2*12,ecpoii3*12,ecpoii4*12
      character namdate*26
      character ecpoii5*12,ecpoiv1*12,ecpoiv2*12,ecpoiv3*12,ecpoiv4*12
      character ecpoiw5*12
c
c>> define logical variables
c
c---------------------------------------------------------------------->
c
c>> define integer*2 variables
c
c
c>> define integer*4 variables
c
c
c>> write out the header file
c
      write(2,9)
9     format(80("*")/,"*",78x,"*"/,"*",36x,"Langley DAAC",30x,"*"/
     $ ,"*",22x,"PROJECT_LEVEL/DATA_SET_LEVEL READ SOFTWARE",14x,"*"
     $ ,/,"* VERSION NUMBER: 01",59x,"*"/,"* Language: FORTRAN 77"
     $ ,57x,"*"
     $ ,/,"* Platforms supported:  SUN",52x,"*"/,"*",23x,"HP",53x,"*"
     $ ,/,"*",23x,"SGI",52x,"*"/,"*"23x,"Dec Alpha",46x,"*"
     $ ,/,"* Contact:  Langley DAAC user and Data Services Office"
     $ ,25x,"*",/,"*",11x,"Mail Stop 157D",53x,"*",/,"*",11x
     $ ,"Hampton, VA 23681-0001",45x,"*",/,"*",5x
     $ ,"PHONE:   (757)864-8656            FAX: (757)864-8807",21x
     $ ,"*",/,80("*")/)
c
       length=8
       value1(1:12)=" "
       call ebcdtoa(label,value1,length)
       label = value1
       length=26
       value28(1:28)=" "
       call ebcdtoa(namdate,value28,length)
       namdate = value28
       length=6
       value1(1:12)=" "
       call ebcdtoa(setnam,value1,length)
       setnam = value1
       length=8
       value(1:8)=" "
       call ebcdtoa(arcidt,value,length)
       arcidt = value
       length = 16
       value28(1:16)=" "
       call ebcdtoa(photo1,value28,length)
       photo1 = value28
       value28(1:16)=" "
       call ebcdtoa(photo2,value28,length)
       photo2 = value28
       value28(1:16)=" "
       call ebcdtoa(photo3,value28,length)
       photo3 = value28
       value28(1:16)=" "
       call ebcdtoa(photo4,value28,length)
       photo4 = value28
       length=4
       value(1:8)=" "
       call ebcdtoa(ecdesi,value,length)
       ecdesi = value
       length=12
       value28(1:12)=" "
       call ebcdtoa(ecpoii1,value28,length)
       ecpoii1 = value28
       value28(1:12)=" "
       call ebcdtoa(ecpoii2,value28,length)
       ecpoii2 = value28
       value28(1:12)=" "
       call ebcdtoa(ecpoii3,value28,length)
       ecpoii3 = value28
       value28(1:12)=" "
       call ebcdtoa(ecpoii4,value28,length)
       ecpoii4 = value28
       value28(1:12)=" "
       call ebcdtoa(ecpoii5,value28,length)
       ecpoii5 = value28
       value(1:8)=" "
       call ebcdtoa(ecdesv,value,length)
       ecdesv = value
       value28(1:12)=" "
       call ebcdtoa(ecpoiv1,value28,length)
       ecpoiv1 = value28
       value28(1:12)=" "
       call ebcdtoa(ecpoiv2,value28,length)
       ecpoiv2 = value28
       value28(1:12)=" "
       call ebcdtoa(ecpoiv3,value28,length)
       ecpoiv3 = value28
       value28(1:12)=" "
       call ebcdtoa(ecpoiv4,value28,length)
       ecpoiv4 = value28
       value28(1:12)=" "
       call ebcdtoa(ecpoiv5,value28,length)
       ecpoiv5 = value28
       length = 8
       value(1:4) = " "
       call ebcdtoa(ecdesw,value,length)
       ecdesw = value
       value28(1:12) = " "
       call ebcdtoa(ecpoiw1,value28,length)
       ecpoiw1 = value28
       value28(1:12) = " "
       call ebcdtoa(ecpoiw2,value28,length)
       ecpoiw2 = value28
       value28(1:12) = " "
       call ebcdtoa(ecpoiw3,value28,length)
       ecpoiw3 = value28
       value28(1:12) = " "
       call ebcdtoa(ecpoiw4,value28,length)
       ecpoiw4 = value28
       value28(1:12) = " "
       call ebcdtoa(ecpoiw5,value28,length)
       ecpoiw5 = value28

       write(2,*)"                               METEOSAT "
       write(2,*)"                       (Real Time Window File)"
       write(2,*)
       write(2,*)" File Header Record "
       write(2,*)" __________________ "
       write(2,*)
c----$---------------------------------------------------------------72>
       write(2,999)label,yr,day,slot,dty,blz,nbl,nhb,off,nslt,ndslt,ofs
       write(2,111)ibgwnd,nblwnd,ibgpix,nblpix,namdate,setnam,arcidt
     $,photo1
       write(2,1111)photo2,photo3,photo4,ecdesi,ecpoii1,ecpoii2
     $,ecpoii3,ecpoii4
       write(2,222)ecpoii5,ecdesv,ecpoiv1,ecpoiv2,ecpoiv3,ecpoiv4
     $,ecpoiv5,ecdesw
     $,ecpoiw1,ecpoiw2,ecpoiw3,ecpoiw4,ecpoiw5
 
999    format("label = ",a15,5x,"| data label",/,"year  =  ",i10
     $ ,9x,"| year (1985,...)",/,"day   = ",i10,9x," | day (001 .. 365)"
     $ ,/,"slot  = ",i10,9x," | start slot, 1 or 25",/,"dty   = ",i10
     $ ,9x," | data type = 100",/,"blz   =   ",i10
     $ ,8x,"| block length = 18960 or 23400 ",/,"nbl   =  ",i10,8x
     $ ," | number blocks = 2305 or 3001",/,"nhb   = ",i10,9x
     $ ," | number header records = 1",/,"off   = ",i10,9x
     $ ," | length of image line headers = 160"/,"nslt  = ",i10,9x
     $ ," | slot number. repeated for each of 24 slots",/,"ndslt = "
     $ ,i10,9x," | 1 = slot present. 0 = slot absent"/,"ofs   = "
     $ ,i10,9x," | record number of the 1st header record of slot")

111    format("ibgwnd = ",i10,9x,"| first line extracted (from South"
     $ ,") e.g. 1950",/,"nblwnd =",i10,9x," | number of lines "
     $ ,"in the window e.g. 516",/,"ibgpix = ",i10,9x,"| first pixel"
     $ ," extracted (from East) e.g. 850",/,"nblpix =",i10
     $ ,9x," | number pixels per line e.g. 600",/,"nexper =       "a26/
     $ ,28x,"|name and date of experiment"/,"setnam =   ",a10
     $ ,7x,"|tape set name e.g. ZAFREA",/,"arcidt =     ",a10
     $ ,5x,"|archive id. of the original data",/,"photo1 = "
     $ ,a16,3x,"| when slot = 1")

1111   format("photo2 = ",a16,3x
     $ ,"| when slot = 1",/,"photo3 = ",a16,3x
     $ ,"| when slot = 1",/,"photo4 = ",a16,3x
     $ ,"| when slot = 1",/,"ecdesi = ",a4,15x,"| channel IR:",/
     $ ,"ecpoii1 =   ",a12,4x,"| coordinates of the enhancement"
     $ ," curve",/,"ecpoii2 =   " ,a12,4x,"| coordinates of the "
     $ ,"enhancement ","curve",/,"ecpoii3 = ",a12,6x,"| coordinates"
     $ ,"   of the enhancement curve",/,"ecpoii4 = ",a12,6x
     $ ,"| coordinates ","of the enhancement curve",/,"ecpoii5 = "
     $ ,a12,6x,"| coordinates of the enhancement curve",/,"ecvali ="20x
     $ ,"| enhancement curve to be used to produce negatives"/
     $ ,30x,"of the IR channel images",/,"ecdesv = ",a10,9x,
     $  "| enhancement curve"
     $ ," (VIS)",/,"ecpoiv1 =   ",a12,4x,"| as ecpoii, for VIS",/
     $ ,"ecpoiv2 =   ",a12,4x,"| as ecpoii, for VIS",/
     $ ,"ecpoiv3 =   ",a12,4x,"| as ecpoii, for VIS",/
     $ ,"ecpoiv4 =   ",a12,4x,"| as ecpoii, for VIS")

222    format("ecpoiv5 =   ",a12,4x,"| as ecpoii, for VIS",/
     $ ,"ecvalv  =   ",a12,4x,"| as ECVALI, for VIS",/
     $ ,"ecdesw  =   ",a10,6x
     $ ,"| enhancement curves (WV)",/,"ecpoiw1 =      ",a12,1x,"| as "
     $ ,"ECPOII for WV.",/,"ecpoiw2 =      ",a12,1x,"| as ECPOII for"
     $ ,"WV.",/ ,"ecpoiw3 =      ",a12,1x,"| as ECPOII for WV.",/
     $ ,"ecpoiw4 =      ",a12,1x,"| as ECPOII for WV.",/
     $ ,"ecpoiw5 =      ",a12,1x,"| as ECPOII for WV.",/)
      return
      end
      subroutine header_first(fn2lp,yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp
     $                       ,spc2lp,blk2lp,nbl2lp,nhb2lp,ty2lp,sn2lp
     $                       ,jslot,lastl,vic2lp,irc2lp,wvc2lp,milsec
     $                       ,vispor,fishre)
c---------------------------------------------------------------------->
c>>integer
      integer*2 vispor(2),lastl
      integer*4 yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,blk2lp
      integer*4 nbl2lp,nhb2lp,milsec,fishre(4),jslot
c>>character
      character value1*12
      character fn2lp*8,ty2lp*8,sn2lp*2
c
c>>logical
      logical*1 irc2lp(2),vic2lp(2),wvc2lp(2)
      length=10
      value1(1:12)=" "
      call ebcdtoa(fn2lp,value1,length)
      fn2lp = value1
      write(2,*)
      write(2,*)"First Header for RECTIFIED"
      write(2,*)"__________________________"
      write(2,*)
      length=8
      value1(1:12)=" "
      call ebcdtoa(ty2lp,value1,length)
      ty2lp=value1
      length=2
      value1(1:12)=" "
      call ebcdtoa(sn2lp,value1,length)
      sn2lp=value1
c---------------------------------------------------------------------->
      write(2,88)fn2lp,yy2lp,dd2lp,sl2lp,dt2lp,ipr2lp,spc2lp,blk2lp
      write(2,888)nbl2lp,nhb2lp,ty2lp,sn2lp,jslot,lastl,
     $ (vic2lp(i),i=1,4)
      write(2,777)(irc2lp(i),i=1,2),(wvc2lp(i),i=1,2)
     $  ,milsec,(vispor(i),i=1,2)
     $     ,(fishre(i),i=1,4)
c
88    format("fn2lp =   ",a,10x,"| File name.",/
     $ "yy2lp =   ",i10,8x,"| Year (e.g.1989)",/
     $ "dd2lp =   ",i10,8x,"| Day(1-366) Image",/
     $ "sl2lp =   ",i10,8x,"| Slot Number (1-48)",/
     $ "dt2lp =   ",i10,8x,"| Data Type (9)",/
     $ "ipr2lp =  ",i10,8x,"| Image process performed",/
     $ 28x,"| 0 = degraded mode of rectification",/
     $ 28x,"| 1 = normal rectification.",/
     $ "spc2lp =  ",i10,8x,"| Spectial content:",/
     $ 28x,"| vis1 1; vis2 2; vis3 4; vis4 8;",/
     $ 28x,"| ir1 16; ir2 32; wv1 64; wv2 128;",/
     $ 28x,"| associated values are added togeather.",/
     $ "reserved",/,"blk2lp =  ",i10,8x,"| File block size (15360)",
     $ ". not relevent.")

888   format("nbl2lp =   ",i10,7x,"| Number of blocks (2503)."
     $"not relevent.",/
     $ "nhb2lp =   ",i10,7x,"| Number of header blocks (93) not relevent.
     $",/,"reserved.",/
c---------------------------------------------------------------------->
     $ "ty2lp =   ",a,10x,"| Standard",/"sn2lp =   ",a,15x,
     $ " | Satellite Code (F2;P2;M4;M5)",/"reserved"/,
     $ "jslot =   ",i10,7x," | Julian slot number"/,"lastl =   ",
     $ i10,7x," | Last line of the Image",/
     $ "vic2lp =   ",i2,i2,i2,i2,7x,"  | channel indicators ")

777    format(28x,"| VIS1:VIS2:VIS3:VIS4: on/off (T/F)"/,"irc2lp =   ",
     $ i2,i2,12x," | Channel indicators ",/28x,"| IR1:IR2: on/off",
     $ "(T/F)",/"wvc2lp =   ",i2,i2,12x," | Channel indicators ",/
     $ 28x,"| WV1:WV2: on/off (T/F)",/,"reserved ",/
     $ "milsec =    ",i7,9x,"| seconds since midnight for southern"
     $ " horizon line ",/"vispor =   ",i2,i2,13x,"| (1) - VIS"
     $ ," channel (1-4) in southern set of VIS arrays.",/
     $ 28x,"| (2) - VIS channel (1-4) in northern set of VIS arrayd."/
     $ "fishre = ",i4,i4,i4,i4,3x,"| (1) : 0= removal off; i=",
     $ " removal on:",/28x,"| (2) : IR fishes found",/
     $ 28x,"| (3) : WV fishes found",/28x,"| (4) : Correlated Fishes",
     $ " (removed ones)",/"reserved",/)
      write(2,*)
      return
      end
      subroutine header_sec(julslt,imgqua,ndgrpt,lstart,lend,lstep
     $                     ,defmax,defmay)
c>>integer
      integer*4 julslt,imgqua,ndgrpt,lstart,lend,lstep
c>>real*4
      real*4 defmax(26,26),defmay(26,26)
c
      write(2,*)"Second Header for RECTIFIED"
      write(2,*)"___________________________"
      write(2,888)julslt,imgqua,ndgrpt,lstart,lend,lstep
c
c>> DEFMAX,DEFMAY will be written to a scratch file named "scratch".
c>> file3.
      write(3,5)
      do 2 i = 1 ,26
        do 1 j = 1 ,26
          write(3,8888)defmax(j,i)
          if(mod(j,11) .eq. 0) then
            write(3,*) " "
          endif
1       continue
      write(3,*) " "
2     continue
66    format(i2,$)
      write(3,7)
      do 4 i = 1 ,26
        do 3 j = 1 ,26
          write(3,8888)defmay(j,i)
          if(mod(j,11) .eq. 0) then
            write(3,*) " "
          endif
3       continue
      write(3,*) " "
4     continue
888   format("julslt =   ",i8,9x,"| Julian Slot number",/
     $ "imgqua =    ",i4,12x,"| Image Geometric Quality",/
     $ 28x,"| 0= Nominal;       1= Attitude unknown;",/
     $ 28x,"| 2= Orbit unknown; 3= Horizon incomplete",/
     $ "ndgrpt =   ",i4,13x,"| Number of grid points on which",/
     $ 28x,"| deformation matrix is based. (=26 in initial version)",/
     $ "lstart =   ",i4,13x,"| Line resp. pixel where deformation ",
     $ "grid starts",/,28x,"| (=50 in initial version)",/
     $ "lend =   ",i4,15x,"| Line resp. pixel where deformation grid",/
     $ 28x,"| ends. (=2450 in initial version)",/
     $ "lstep =   ",i4,14x,"| Line resp. pixel step of deformation ",/
     $ 28x,"| grid (=96 in initial version)")
5     format(/"defmax = ",/)
7     format(/"defmay = ",/)
8888  format(f7.2,$)
      write(2,*)
      return
      end
      subroutine header_third(jslohp,jdayhp,slohp,nithp,slothp,gridhp
     $                       ,saidhp,sspxhp,sspyhp,thrhp,anbhp,anqhp
     $                       ,spcthp,intdhp
     $                       ,nblhp,mltwhp,mltvhp,mltuhp,mltihp)
c>>character
      character value1*12,value28*28,anbhp*28,anqhp*12
      character saidhp*2
c>>integer
      integer*2 sspxhp,sspyhp,thrhp,spcthp(3)
      integer*2 jdayhp,slohp,nithp,slothp,gridhp,nblhp(3,3)
      integer*4 jslohp
c>>logical
      logical*1 intdhp(1360)
      logical*1 mltuhp(316),mltvhp(316),mltihp(316),mltwhp(316)
c
      write(2,*)
      write(2,*)"Third Header for RECTIFIED"
      write(2,*)"__________________________"
      length=2
      value1(1:12)=" "
      call ebcdtoa(saidhp,value1,length)
      saidhp=value1
      length=28
      value28(1:28)=" "
      call ebcdtoa(anbhp,value28,length)
      length=12
      value28(1:28)=" "
      call ebcdtoa(anqhp,value28,length)
      anqhp = value28
c----$---------------------------------------------------------------72>
      write(2,222)jslohp,jdayhp,slohp,nithp,slothp,gridhp,saidhp
      write(2,2222)sspxhp,sspyhp,thrhp,anbhp,anqhp,(spcthp(i),i=1,3)
c
222   format("jslohp =   ",i8,9x,"| Julian Slot Number.",/
     $       "jdayhp =   ",i8,9x,"| Julian day (01/01/50=0)",/
     $       "slohp =   ",i4,14x,"| Slot number",/
     $"nithp =   ",i4,14x,"| Nominal image time (HHMM)",/
     $28x,"| (1400,1230,..0730,etc)",/"slothp =   ",i4,13x,
     $"| Slot type",/28x,"| 0 = no data (being filled)",/
     $28x,"| 1 = nominal slot",/28x,"| 2 = service slot",/
     $28x,"| 3 = MOP format",/"gridhp =   ",
     $ i4,13x,"| Image quality flag.",/28x,"| 1 = grids can be added",/
     $28x,"| 2 = coastlines can be added.",/28x,"| 3 = both can be",
     $" added",/28x,"| 0 = should not add either",/
     $"saidhp =   ",a,15x,"| HR. Dissemination Sat.ID")

2222  format(
     $"sspxhp =   ",i4,13x,"| Sub-satellite point displacement in ",
     $"pixels",/28x,"| W-E direction. (East=positive) (=0)",/
     $"sspyhp =   ",i4,13x,"| Sub-satellite point displacement",
     $" in lines",/28x,"| S-N direction. (North=positive) (=0)",/
     $"thrhp =   "i4,14x,"| Maximum number of missing lines",
     $" replaced",/28x,"| by nearest neighbor",/"anbhp = ",
c----$---------------------------------------------------------------72>
     $a,16x,/,28x,"| Annotation for WEFAX formats",/28x,"| (METn DD",
     $" MMM YYYY HHMM ) in ASCII",/28x,"| note , the ",
     $"dissemination channel is added",/28x,"| by the BEP",
     $" computer.",/"anqhp =   ",a,14x,"| Annotation for image",/
     $"spcthp =   ",i4,i4,i4,5x,"| Spectral contents (1)=VIS;",
     $" (2)=IR; (3)=WV:",/28x,"| (for dissemination)",/
     $28x,"| 0 = no data: 1 = VIS1 : 2 = VIS2",/
     $28x,"| 3 = VIS1+2 : 4 = IR1  : 5 = IR2",/
     $28x,"| 6 = WV1    : 7 = WV2  : 8 = VIS3",/
     $28x,"| 9 = VIS4   : 10 = VIS3+1 : 11 = VIS4+2",/
     $28x,"| 12 = VIS3+4",/"reserved")
c
c>> MLTUHP,MLTVHP will be written to a scratch file named "scratch".
c>> file3.
      write(3,8)
8     format(/"mltuhp = ",/)
      do 123 i = 1 ,316
        write(3,134)mltuhp(i)
          if(mod(i,20) .eq. 0) then
            write(3,*) " "
          endif
134     format(i4,$)
123   continue
      write(3,*) " "
      write(3,91)
91    format(/"mltvhp = ",/)
      do 129 i = 1 ,316
        write(3,137)mltvhp(i)
          if(mod(i,20) .eq. 0) then
            write(3,*) " "
          endif
137     format(i4,$)
129   continue
c
c>> MLTIHP,MLTWHP will be written to a scratch file named "scratch".
c>> file3.
      write(3,*) " "
      write(3,90)
90    format(/"mltihp = ",/)
      do 921 i = 1 ,316
        write(3,237)mltihp(i)
          if(mod(i,20) .eq. 0) then
            write(3,*) " "
          endif
237     format(i4,$)
921   continue
      write(3,*) " "
      write(3,554)
554   format(/"mltwhp = ",/)
      do 941 i = 1 ,316
        write(3,267)mltwhp(i)
          if(mod(i,20) .eq. 0) then
            write(3,*) " "
          endif
267     format(i4,$)
941   continue
c
c>> nblhp will be written to a scratch file named "scratch".
c>> file3.
      write(3,*) " "
      write(3,559)
559   format(/"nblhp = ",/)
      do 4444 i = 1 ,3
        do 8128 j = 1 ,3
          write(3,277)nblhp(j,i)
          if(mod(j,9) .eq. 0) then
            write(3,*) " "
          endif
277     format(i4,2x,$)
8128  continue
      write(3,*) " "
4444  continue
c
c>> intdhp will be written to a scratch file named "scartch".
c>>file3.
      write(3,85)
85    format(/"intdhp = ",/)
      do 1111 i = 1 ,316
        write(3,22221)intdhp(i)
          if(mod(i,20) .eq. 0) then
            write(3,*) " "
          endif
22221 format(i4,$)
1111  continue
      return
      end
c**********************************************************************
c----$---------------------------------------------------------------72>
      subroutine header_imgrec(jslot,lastl2,visid,wvid,irid)
c>>integer
      integer*2 lastl2,visid,wvid,irid
      integer*4 jslot
      write(2,*)
      write(2,*)"Image Record for RECTIFIED"
      write(2,*)"__________________________"
      write(2,*)
c----$---------------------------------------------------------------72>
c>> Image Record for RECTIFIED
      write(2,77)jslot,lastl2,visid,wvid,irid
77    format("jslot =   ",i8,10x,"| Julian slot number.",/
     $ ,"lastl =   ",i10,8x,"| Image line number.",/
     $ ,"visid =   ",i10,8x,"| VIS channel indicator"/
     $ ,28x,"| 0=off; 1=vis1; 2=vis2; 3=vis1+vis2",/
     $ ,"wvid =   ",i10,9x,"| WV channel indicator. 0=off; 6=wv",/
     $ ,"irid =   ",i10,9x,"| IR channel indicator. ",/
     $ ,28x,"| 0=off; 4=ir1; 5=ir2")
      return
      end
