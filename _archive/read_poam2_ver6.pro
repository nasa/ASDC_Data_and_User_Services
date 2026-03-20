;  IDL PROCEDURE: READ_POAM2_VER6
;  -------------------------------------------------------
;  
;  Function:  Open and read POAM2_VER6 HDF data files.
;             Write minimum and maximum values of the parameters
;             to the output file.
;             Keywords are available if the User wants to subset the 
;             by date or orbit number (rev).
;             A call to dialog_pickfile is used to create a dialog 
;             box for filename choice. This routine assumes those files 
;             are in the current directory.  The 'PATH' parameter may 
;             need to be modified to correspond to the location
;             of the input files. 
;
;     *** Please see the README file for further details ***
;
;  -------------------------------------------------------
;     
;  1-DIM ARRAYS ("n" dimension is orbit number, a unique id)
;  -------------------------------------------------------
;  rev(n)       = integer: Orbit Number (unique id)
;  date(n)      = long:    Date (YYYYMMDD)
;  sec(n)       = float:   Seconds of day (UT)
;  lat(n)       = float:   Latitude (-90 - +90 deg)
;  lon(n)       = float:   Longitude (0 - 360 deg)
;  z_ozone(36)  = float:   Ozone altitude grid 15-50 (km)
;  z_no2(21)    = float:   NO2 altitude grid 20-40 (km) 
;  z_aerosol(21)= float:   Aerosol altitude grid 10-30 (km) 
;  z_anc(61)    = float:   Anc. data altitude grid 0-60 (km)
;  rev_index(n) = float:
;  
;  2-DIM ARRAYS ("n by nz", nz is altitude dimension)
;  -------------------------------------------------------
;  ozone(n,36)      = float: Ozone Concentration (cm-3)
;  err_ozone(n,36)  = float: Ozone Error Bars (cm-3)
;  no2(n,21)        = float: NO2 Concentration (cm-3)
;  err_no2(n,21)    = float: NO2 Error Bars (cm-3)
;  aerosol(n,21)    = float: Aerosol Extinction at 1.06um (km-1)
;  err_aerosol(n,21)= float: Aerosol Error Bar (km-1)
;  temperature(n,61)= float: UKMO Temperature (K)
;  pressure(n,61)   = float: UKMO Pressure (mb)
;  pv(n,61)         = float: UKMO Potential Vorticity
;
;  OPTIONAL KEYWORDS WHICH RETURN SUB-ARRAYS
;  ------------------------------------------
;  startrev  = integer: starting orbit for all arrays  (overrides startdate)
;  endrev    = integer: last orbit for all arrays      (overrides enddate)
;  startdate = long (YYYYMMDD): starting date for arrays
;  endate    = long (YYYYMMDD): last date for arrays
;
;  EXAMPLE USAGE
;  -------------
;  Obtain data for Jan. 1, 1995 through Jan. 11, 1995:
;
;    read_poam2_ver6,startdate=19950101L,enddate=19950111L
;
;
;  This software has been tested on the following systems:
; 
;       Computer                Operating System	IDL Version
;       ---------------         ----------------        -----------
;       Sun Sparc               Solaris 2.6              5.1
;       SGI Origin 2000         IRIX 6.4                 5.2
;       HP 9000/735             HP-UX 10.10              5.1
;
; --------------------------------------------------------------------
;
;  Contact: Langley DAAC User and Data Services Office 
;           NASA Langley Research Center 
;           Mail Stop 157D 
;           Hampton, VA 23681-2199 
;           Phone (757)864-8656   Fax (757)864-8807 
;           e-mail:  support-asdc@earthdata.nasa.gov 
;           http://eosweb.larc.nasa.gov 
;
;=================================================================================
PRO read_poam2_ver6,startrev=startrev,endrev=endrev,startdate=startdate,enddate=enddate

;
; dialog box containing filenames
; The path parameter may be added to suit your system
; For example, if the files are located in the directory data, use:
; filename = dialog_pickfile(/read,path='data/',filter='*.hdf')

filename = dialog_pickfile(/read,filter='*.hdf')

;
; open output file
;
openw,out_lun,filename+'_output',/get_lun

;
;  OPEN AND INITIALIZE HDF FILE INFO
;
   if (HDF_ISHDF(filename) ne 1) then begin
      print,"Error, not a HDF file: ",filename
      stop
   endif
   handle = HDF_OPEN(filename,/read)
   hid    = HDF_SD_START(filename,/read)

;
;  FIND DIMENSION FROM REV ARRAY
;
   index = HDF_SD_NAMETOINDEX(hid,'rev')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETINFO,sid,dims=dims
   nevents = dims(0)
   max_events=nevents
   
;
;  READ REV ARRAY AND DATE ARRAY
;
   HDF_SD_GETDATA,sid,rev
   index = HDF_SD_NAMETOINDEX(hid,'date')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,date
   max_rev=max(rev,min=min_rev)
   max_date=max(date,min=min_date)
;
; get array dimensions based on rev array
;
   nz    = 61               ;altitude dimension size for ancillary
   na    = 21               ;altitude dimension size for aerosol data
   nO3   = 36               ;altitude dimension size for ozone data
   nNO2  = 21               ;altitude dimension size for NO2 data
   n     = nevents          ;rev dimension size for all data
   c1 = [nevents]           ;count array for reading 1-dim data
   c2 = [nevents,nz]        ;count array for reading 2-dim data
   c3 = [nevents,nO3]       ;count array for reading 2-dim data
   c4 = [nevents,nNO2]      ;count array for reading 2-dim data
   c5 = [nevents,na]
   c6 = [nO3]
   c7 = [nz]
   c8 = [na]
   c9 = [nNO2]
   s1 = [0]                 ;starting offset for reading 1-dim data
   s2 = [0,0]               ;starting offset for reading 2-dim data

;
;  IF START/END REV/DATE KEYWORDS ARE SET THEN OBTAIN
;  OFFSET VALUES FOR SUB-REGION OF REVS
;
if (keyword_set(startrev) or keyword_set(startdate)) then begin
    if (keyword_set(startrev)) then begin
       i = where(rev ge startrev,cnt)
    endif else begin
       i = where(date ge startdate,cnt)
    endelse
    if (cnt le 0) then begin
        print,'Error: starting rev not found'
        stop
    endif
    i  = i(0)
    print,'First Rev = ',rev(i)
    min_rev=rev(i)
    min_date=date(i)
    n  = n - i
    s1 = [i]
    s2 = [i,0]
    c1 = [n]
    c2 = [n,nz]
    c3 = [n,nO3]
    c4 = [n,nNO2]
    c5 = [n,na]
endif

if (keyword_set(endrev) or keyword_set(enddate)) then begin
    if (keyword_set(endrev)) then begin
       i = where(rev le endrev,cnt)
    endif else begin
       i = where(date le enddate,cnt)
    endelse
    if (cnt le 0) then begin
        print,'Error: ending rev not found'
        stop
    endif
    print,'Last Rev = ',rev(i(cnt-1))
    max_rev=rev(i(cnt-1))
    max_date=date(i(cnt-1))
    n  = n - (nevents-1-i(cnt-1))
    c1 = [n]
    c2 = [n,nz]
    c3 = [n,nO3]
    c4 = [n,nNO2]
    c5 = [n,na]
endif
max_events=n
;
;   RESAMPLE THE REV ARRAY
;
rev = rev(s1(0):s1(0)+c1(0)-1)
date = date(s1(0):s1(0)+c1(0)-1)
;
;   READ ALL THE 1-DIM AND 2-DIM ARRAYS
;   GET MIN/MAX VALUES
;

; latitude(nevents)
   index = HDF_SD_NAMETOINDEX(hid,'lat')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,lat,start=s1,count=c1
   max_lat=max(lat,min=min_lat)
   
; longitude(nevents)
   index = HDF_SD_NAMETOINDEX(hid,'lon')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,lon,start=s1,count=c1
   max_lon=max(lon,min=min_lon)

; seconds(nevents)
   index = HDF_SD_NAMETOINDEX(hid,'sec')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,sec,start=s1,count=c1

; rev_index(nevents)
   index = HDF_SD_NAMETOINDEX(hid,'rev_index')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,rev_index,start=s1,count=c1
   max_rev_index=max(rev_index,min=min_rev_index)

; ozone(nevents,nO3)
   index = HDF_SD_NAMETOINDEX(hid,'ozone')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,ozone,start=s2,count=c3
   hdf_sd_getinfo,sid,fill=ozone_fill
; get min/max without getting fill value
   goodIndices = where( ozone ne ozone_fill, count)
   if count gt 0 then begin
    temp = ozone[goodIndices]
    max_o3=max(temp,min=min_o3)
   endif

; ozone_err(nevents,nO3)
   index = HDF_SD_NAMETOINDEX(hid,'ozone_err')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,ozone_err,start=s2,count=c3
   hdf_sd_getinfo,sid,fill=o3err_fill
; get min/max without getting fill value
   goodIndices = where( ozone_err ne o3err_fill, count)
   if count gt 0 then begin
    temp = ozone_err[goodIndices]
    max_o3err=max(temp,min=min_o3err)
   endif
 
; z_ozone(nO3)
   index = HDF_SD_NAMETOINDEX(hid,'z_ozone')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,z_ozone
   max_o3z=max(z_ozone,min=min_o3z)

; z_anc(nz)
   index = HDF_SD_NAMETOINDEX(hid,'z_anc')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,z_anc
   max_ancz=max(z_anc,min=min_ancz)

; pressure(nevents,nz)
   index = HDF_SD_NAMETOINDEX(hid,'P')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,pressure,start=s2,count=c2
   max_P=max(pressure,min=min_P)

; temperature(nevents,nz)
   index=HDF_SD_NAMETOINDEX(hid,'T')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,temperature,start=s2,count=c2
   max_T=max(temperature,min=min_T) 

; potential_vorticity(nevents,nz)
   index = HDF_SD_NAMETOINDEX(hid,'PV')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,pv,start=s2,count=c2
   hdf_sd_getinfo,sid,fill=PV_fill
; get min/max without getting fill value
   goodIndices = where( pv ne PV_fill, count)
   if count gt 0 then begin
    temp = pv[goodIndices]
    max_pv=max(temp,min=min_pv)
   endif
  
; aerosol(nevents,na)
   index = HDF_SD_NAMETOINDEX(hid,'aerosol')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,aerosol,start=s2,count=c5
   aerosol=reform(aerosol)
   hdf_sd_getinfo,sid,fill=aer_fill
; get min/max without getting fill value
   goodIndices = where( aerosol ne aer_fill, count)
   if count gt 0 then begin
    temp = aerosol[goodIndices]
    max_aer=max(temp,min=min_aer)
   endif

; aer_err(nevents,na)
   index = HDF_SD_NAMETOINDEX(hid,'aerosol_err')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,aer_err,start=s2,count=c5
   aer_err=reform(aer_err)
   hdf_sd_getinfo,sid,fill=sds_fill
; get min/max without getting fill value
   goodIndices = where( aer_err ne sds_fill, count)
   if count gt 0 then begin
    temp = aer_err[goodIndices]
    max_aererr=max(temp,min=min_aererr)
   endif

; z_aerosol(na) 
   index = HDF_SD_NAMETOINDEX(hid,'z_aerosol')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,z_aerosol
   max_aerz=max(z_aerosol,min=min_aerz)

; no2(nevents,nNO2) 
   index = HDF_SD_NAMETOINDEX(hid,'no2')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,no2,start=s2,count=c4
   hdf_sd_getinfo,sid,fill=no2_fill
   goodIndices = where( no2 ne no2_fill, count)
   if count gt 0 then begin
    temp = no2[goodIndices]
    max_no2=max(temp,min=min_no2)
   endif

; no2_err(nevents,nNO2)
   index = HDF_SD_NAMETOINDEX(hid,'no2_err')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,no2_err,start=s2,count=c4
   hdf_sd_getinfo,sid,fill=sds_fill
; get min/max without getting fill value
   goodIndices = where( no2_err ne no2_fill, count)
   if count gt 0 then begin
    temp = no2_err[goodIndices]
    max_no2err=max(temp,min=min_no2err)
   endif

; z_no2(nNO2) 
   index = HDF_SD_NAMETOINDEX(hid,'z_no2')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,z_no2   
   max_no2z=max(z_no2,min=min_no2z)   


   ;---------------------- CLOSE FILE 
   HDF_SD_END,hid
   HDF_CLOSE,handle 

; OUTPUT MIN/MAX VALUES TO A FILE
printf,out_lun,'******  ',filename,'  ******'
printf,out_lun,'rev: ',min_rev,max_rev
printf,out_lun,'date: ',min_date,max_date
printf,out_lun,'lat: ',min_lat,max_lat
printf,out_lun,'lon: ',min_lon,max_lon
printf,out_lun,'o3: ',min_o3,max_o3
printf,out_lun,'o3err: ',min_o3err,max_o3err
printf,out_lun,'o3z: ',min_o3z,max_o3z
printf,out_lun,'ancz: ',min_ancz,max_ancz
printf,out_lun,'P: ',min_P,max_P
printf,out_lun,'T: ',min_T,max_T
printf,out_lun,'PV: ',min_PV,max_PV
printf,out_lun,'aer: ',min_aer,max_aer
printf,out_lun,'aererr: ',min_aererr,max_aererr
printf,out_lun,'aerz: ',min_aerz,max_aerz
printf,out_lun,'no2: ',min_no2,max_no2 
printf,out_lun,'no2err: ',min_no2err,max_no2err  
printf,out_lun,'no2z: ',min_no2z,max_no2z 
printf,out_lun,'total number of events: ',max_events




print,'Completed reading ',filename
print,'Output is in ',filename+'_output' 
close,out_lun
free_lun,out_lun
END
