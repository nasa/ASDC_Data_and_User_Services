c
	PROGRAM emis_sample_reader
c
	parameter (nch=8461)
	parameter (nx=10, len=16)
	dimension freq(nch), buf(len)
	dimension cc(nx),vcv(nch,nx),em(nch)
	dimension emis(nch)
	real*8 work(nch)
	real alon,alat
	integer n1, n2, nnn
	character*80 inevf, rtvfile

	data inevf/'IASI_B_EV_FUNC_GLOBAL_V4.bin'/

	write(6,*) 'enter file
     &   (e.g., MFEMI_MONTH08_A_V5P.bin)'
	read(5,'(a)') rtvfile

40      write(6,*) 'enter Lat. & long. ( e.g., 18.2, 12.7 ) '
	read(5,*) alat, alon
	n1 = floor( (180+alon)/.25 ) + 1
	n2 = floor( ( 90+alat)/.25 ) + 1
	nnn=(n1-1)*720 + n2

	open(76,file='emis_sample.dat',status='unknown')
 
C *** Open eFEOFA Eigenvector file
	lenxy=nch*8
	open(20,file=inevf,recl=lenxy,access='direct',status='old')
	read(20,rec=1) work
	do j=1,nch
	   em(j) = work(j)
	enddo
	do i=1,nx
	   read(20,rec=i+1) work
	   sum=0.0
	   do j=1,nch
	      vcv(j,i)=work(j)
	      sum=sum+vcv(j,i)**2
	   enddo
	enddo
	close(20)

C *** IASI frequence (channels)
	do i=1,nch
	   freq(i)=645.00+0.25*(i-1)
	enddo

C *** Open retrieved *FEMI* file
	lenxy=len*4
	open(30,file=rtvfile,recl=lenxy,access='direct',status='old')

C *** Read rec. # nnn 
	read(30,rec=nnn,err=3001) buf

C *** Computer emis if it is over the land
c	if(buf(1).lt.0.2 .and. buf(6).eq.-999) then
c	   write(6,*) 'it is over water. Re-enter Lat. & Long.'
c	   goto 40
c	endif
	   
	if(buf(5).lt.50) then
	   do i=1,nch
	      emis(i)=-999.	! under cloud or bad data
	   enddo
	else
	   do i=1,nx
	      cc(i)=buf(5+i)
	   enddo
	   do i=1,nch
	      emis(i)=em(i)
	      do j=1,nx
		 emis(i)=emis(i)+cc(j)*vcv(i,j)
	      enddo
	      emis(i)=0.995-exp(-0.693147181-exp(emis(i)))
	   enddo
	endif
 
	write(6,*)
	write(6,71) buf(1)
	write(6,72) buf(2)-0.125, buf(2)+0.125
	write(6,73) buf(3)-0.125, buf(3)+0.125
	write(6,74) buf(4)
	write(6,75) buf(5)
	write(6,76) buf(16)
	write(6,*)
	write(6,*) 'Emissivity spectral data in ascci file " emis_sample.dat "'
	write(6,*)
	do i=1,nch
	   write(76,*) i, freq(i), emis(i)
	enddo

71	format('land frac (1: land; 0: water)   : ',f11.2)
72	format('latitude          		: ',f8.2,' to ',f8.2,' deg')
73	format('longitude         		: ',f8.2,' to ',f8.2,' deg')
74	format('surf pressure      		: ',f11.2,' mb')
75	format('surf temperature  		: ',f11.2,' deg-K')
76	format('# of ensemble of FOVs		: ',f11.2)

3001	stop
	end
