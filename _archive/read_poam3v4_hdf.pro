;  IDL PROCEDURE: read_p3ver2_hdf,filename
;
;  All arrays are returned as optional keyword parameters.
;     
;  1-DIM ARRAYS ("n" dimension is orbit number, a uniq id)
;  -------------------------------------------------------
;  rev(n)       = long:    Orbit Number (unique id)
;  date(n)      = long:    Date (YYYYMMDD)
;  sec(n)       = float:   Seconds of day (UT)
;  lat(n)       = float:   Latitude (deg)
;  lon(n)       = float:   Longitude (0-360 deg)
;  max_ss(n)    = float:   Maximum sunspot index for profile
;  wavelength(6)= float:   Wavelength (micron)
;  z_ozone(56)  = float:   Ozone altitude grid 5-60 (km)
;  z_no2(21)    = float:   NO2 altitude grid 20-45 (km) 
;  z_h2o(41)    = float:   H2O Vapor altitude grid 5-50  (km) 
;  z_aerosol(21)= float:   Aerosol altitude grid 5-25 (km) 
;  z_anc(61)    = float:   Anc. data altitude grid 0-60 (km)
;  
;  2-DIM ARRAYS ("n by nz", nz is alititude dimension)
;  -------------------------------------------------------
;  ozone(n,56)      = float:   Ozone Concentration (molecule/cc)
;  err_ozone(n,56)  = float:   Ozone Concentration Error Bars (molecule/cc)
;  flag_ozone(n,56) = integer: Ozone Quality Flag
;  no2(n,21)        = float:   NO2 Concentration (molecule/cc)
;  err_no2(n,21)    = float:   NO2 Concentration Error Bars (molecule/cc)
;  flag_no2(n,21)   = integer: NO2 Quality Flag
;  h2o(n,41)        = float:   H2O Vapor Concentration (molecule/cc)
;  h2o_err(n,41)    = float:   H2O Vapor Concentration Error Bars (molecule/cc)
;  h2o_flag(n,41)   = integer: H2O Vapor Quality Flag
;  temperature(n,61)= float:   UKMO Temperature (K)
;  pressure(n,61)   = float:   UKMO Pressure (mb)
;  pv(n,61)         = float:   UKMO Potential Vorticity (K/s/Pa, PV units)
;
;  3-DIM ARRAYS ("n by nchan by nz")
;  -------------------------------------------------------
;  aerosol(n,nchan,21)    = float: Aerosol Extinction at six wavelengths (km-1)
;  err_aerosol(n,nchan,21)= float: Aerosol Extinction Error Bar (km-1)
;
;  OPTIONAL KEYWORDS WHICH RETURN SUB-ARRAYS
;  ------------------------------------------
;  startrev  = integer: starting rev for all arrays  (overrides startdate)
;  endrev    = integer: last rev for all arrays      (overrides enddate)
;  startdate = long (YYYYMMMDD): starting date for arrays
;  enddate   = long (YYYYMMMDD): last date for arrays
;
;  EXAMPLE CALLS
;  -------------
;  - Get the ozone, aerosol, time/location for all of July 1999
;    in the northern hemisphere.
;
;IDL> filename = 'poam3_ver4_nh_199907.hdf'
;IDL> rev=1 & date=1 & sec=1 & lat=1 & lon=1 & o3=1 & aer=1
;IDL> read_p3ver4_hdf,filename,rev=rev,date=date,sec=sec,lat=lat,lon=lon, $
;                  ozone=o3, aerosol=aer
;    
;
;
;=================================================================================
PRO read_p3ver4_hdf, filename, outfilename   $ 
   ,rev         = rev                 $ 
   ,date        = date                $ 
   ,lat         = lat                 $ 
   ,lon         = lon                 $ 
   ,sec         = sec                 $ 
   ,pressure    = pressure            $ 
   ,temperature = temperature         $ 
   ,pv          = pv                  $ 
   ,z_anc       = z_anc               $ 
   ,ozone       = ozone               $ 
   ,err_ozone   = ozone_err           $ 
   ,flag_ozone  = ozone_flag          $
   ,z_ozone     = z_ozone             $ 
   ,aerosol     = aerosol             $ 
   ,err_aerosol = aerosol_err         $ 
   ,z_aerosol   = z_aerosol           $ 
   ,no2         = no2                 $ 
   ,err_no2     = no2_err             $
   ,flag_no2    = no2_flag            $ 
   ,z_no2       = z_no2               $ 
   ,h2o         = h2o                 $ 
   ,err_h2o     = h2o_err             $
   ,flag_h2o    = h2o_flag            $ 
   ,z_h2o       = z_h2o               $ 
   ,startrev    = startrev            $ 
   ,endrev      = endrev              $ 
   ,startdate   = startdate           $ 
   ,enddate     = enddate             

;  OPEN OUTPUT FILE
   if not keyword_set(outfilename) then uid=-1 else $
   openw,uid,outfilename,/get_lun

;
;  OPEN AND INITIALIZE HDF FILE INFO
;
   if (HDF_ISHDF(filename) ne 1) then begin
     printf,uid,"Error, not a HDF file: ",filename
     mes_box = DIALOG_Message( /ERROR, 'Input file not a HDF file.' )
     exit, status=56
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


;
;  READ REV ARRAY AND DATE ARRAY
;
   HDF_SD_GETDATA,sid,rev
   index = HDF_SD_NAMETOINDEX(hid,'date')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,date
;
;  GET ALTITUDE ARRAYS TO DETERMINE DIMENSIONS
;
   index = HDF_SD_NAMETOINDEX(hid,'wavelength')
   if (index ne -1) then begin 
      sid = HDF_SD_SELECT(hid,index)
      HDF_SD_GETDATA,sid,wavelength
      aer1D = 0
   endif else begin
      wavelength = [1.018]
      aer1D = 1
   endelse
   index = HDF_SD_NAMETOINDEX(hid,'z_aerosol')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,z_aerosol

   index = HDF_SD_NAMETOINDEX(hid,'z_anc')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,z_anc

   index = HDF_SD_NAMETOINDEX(hid,'z_ozone')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,z_ozone

   index = HDF_SD_NAMETOINDEX(hid,'z_no2')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,z_no2     

   index = HDF_SD_NAMETOINDEX(hid,'z_h2o')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,z_h2o     


   nchan = n_elements(wavelength) ;aerosol wavelength dimension


   nz    = n_elements(z_anc)     ;alt. dimension for ancillary
   na    = n_elements(z_aerosol) ;alt. dimension for aerosol data
   nO3   = n_elements(z_ozone)   ;alt. dimension for ozone data
   nNO2  = n_elements(z_no2)     ;alt. dimension for NO2 data
   nH2O  = n_elements(z_h2o)     ;alt. dimension for NO2 data

   n     = nevents          ;rev dimension size for all data
   c1 = [nevents]           ;count array for reading 1-dim arrays
   c2 = [nevents,nz]        ;count array for ANC arrays
   c3 = [nevents,nO3]       ;count array for ozone arrays       
   c4 = [nevents,nNO2]      ;count array for no2 arrays         
   c5 = [nevents,nchan,na]  ;count array for aerosol arrays  
   c6 = [nevents,nH2O]      ;count array for h2o arrays        
   s1 = [0]                 ;starting offset for reading 1-dim data
   s2 = [0,0]               ;starting offset for reading 2-dim data
   s3 = [0,0,0]             ;starting offset for reading 3-dim data

   printf,uid, '  POAM3 HDF file: ',STRTRIM(filename, 2)
   printf,uid, ''
 

   if ( long(startdate) gt long(enddate)) then begin
     err_msg = STRARR(2)
     err_msg[0] = 'Input startdate (' +                         $
                  STRTRIM(startdate,2) +                          $
                  ') is greater then the enddate (' +           $
                  STRTRIM(enddate,2) + ').'
     err_msg[1] = 'Please modifiy dates appropriately.'
     mes_box = DIALOG_Message ( /ERROR, err_msg)
     exit, status=56
   endif
;
;  SET OFFSET VALUES FOR SUB-REGION OF REVS
;
if (keyword_set(startrev) or keyword_set(startdate)) then begin
    if (keyword_set(startrev)) then begin
       i = where(rev ge startrev,cnt)
       printf,uid, '  Startrev = ',startrev
    endif else begin
       i = where(date ge startdate,cnt)
       printf,uid, ' Startdate = ',startdate
    endelse
    if (cnt le 0) then begin
      printf,uid,'Error: starting rev not found'
      mes_box = DIALOG_Message( /ERROR,  'Rev number calulation failed, Input startdate ('  $
         + STRTRIM(startdate,2) + ') is greater then the enddate (' + STRTRIM(enddate,2) + ').' ) 
      exit, status=56
    endif
    i  = i(0)
    printf,uid,' First Rev = ',rev(i)
    n  = n - i
    s1 = [i]
    s2 = [i,0]
    s3 = [i,0,0]
    c1 = [n]
    c2 = [n,nz]
    c3 = [n,nO3]
    c4 = [n,nNO2]
    c5 = [n,nchan,na]
    c6 = [n,nH2O]
endif
if (keyword_set(endrev) or keyword_set(enddate)) then begin
    if (keyword_set(endrev)) then begin
       i = where(rev le endrev,cnt)
       printf,uid, '    Endrev = ',endrev
    endif else begin
       i = where(date le enddate,cnt)
       printf,uid, '   Enddate = ',enddate
    endelse
    if (cnt le 0) then begin
      printf,uid,'Error: ending rev not found'
      mes_box = DIALOG_Message( /ERROR,  'Rev number calulation failed, Input startdate ('  $
         + STRTRIM(startdate,2) + ') is greater then the enddate (' + STRTRIM(enddate,2) + ').' )
      exit, status=56
    endif
    printf,uid,'  Last Rev = ',rev(i(cnt-1))
    n  = n - (nevents-1-i(cnt-1))
    c1 = [n]
    c2 = [n,nz]
    c3 = [n,nO3]
    c4 = [n,nNO2]
    c5 = [n,nchan,na]
    c6 = [n,nH2O]
endif

if (aer1D eq 1) then begin
   c5 = [c5(0),c5(2)]     
   s3 = [s3(0),s3(2)]
endif


;
;   RESAMPLE THE REV ARRAY
;
rev = rev(s1(0):s1(0)+c1(0)-1)
date = date(s1(0):s1(0)+c1(0)-1)
;
;   READ ALL THE REQUESTED 1-DIM AND 2-DIM ARRAYS
;
printf,uid,''
printf,uid,'- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -'
printf,uid,''

if (keyword_set(date) NE 0) then begin
   printf,uid,format='(" Date (YYYYMMDD) [1:",I3,"]")',c1
   printf,uid,format='("   ",5I13)',date[0:c1-1]
endif
if (keyword_set(sec)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'sec')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,sec,start=s1,count=c1
   printf,uid,format='(" Seconds of day (UT) [1:",I3,"]")',c1
   printf,uid,format='("   ",5F13.1)',sec[0:c1-1]
endif
if (keyword_set(rev) NE 0) then begin
   printf,uid,format='(" Orbit Number (unique id) [1:",I3,"]")',c1
   printf,uid,format='("   ",5I13)',rev[0:c1-1]
endif
if (keyword_set(lat) NE 0) then begin
   index = HDF_SD_NAMETOINDEX(hid,'lat')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,lat,start=s1,count=c1
   printf,uid,format='(" Latitude (deg) [1:",I3,"]")',c1
   printf,uid,format='("   ",5F13.2)',lat[0:c1-1]
endif
if (keyword_set(lon)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'lon')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,lon,start=s1,count=c1
   printf,uid,format='(" Longitude (deg) [1:",I3,"]")',c1
   printf,uid,format='("   ",5F13.2)',lon[0:c1-1]
endif
if (keyword_set(lon)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'max_ss')
   if ( index gt 0) then begin
       sid = HDF_SD_SELECT(hid,index)
       HDF_SD_GETDATA,sid,max_ss,start=s1,count=c1
       printf,uid,format='(" Maximum sunspot index for profile [1:",I3,"]")',c1
       printf,uid,format='("   ",5F13.2)',max_ss[0:c1-1]
   endif else begin
       printf,uid,format='(" Maximum sunspot index NOT included in this file")'
   endelse
endif

if (keyword_set(pressure)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'P')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,pressure,start=s2,count=c2
   for i = 0, c2[1]-1 do begin
      alt_str = STRING ( z_anc[i])
      printf,uid,format='(" UKMO Pressure (mb) at ",A," km [1:",I3,",",I3,"]")',    $
        STRTRIM(alt_str,2),c2[0],i+1
      printf,uid,format='("   ",5G13.6)',pressure[0:c2[0]-1,i]
   endfor
endif
if (keyword_set(temperature)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'T')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,temperature,start=s2,count=c2
   for i = 0, c2[1]-1 do begin
      alt_str = STRING ( z_anc[i])
      printf,uid,format='(" UKMO Temperature (K) at ",A," km [1:",I3,",",I3,"]")',  $    
        STRTRIM(alt_str,2),c2[0],i+1
      printf,uid,format='("   ",5G13.6)',temperature[0:c2[0]-1,i]
   endfor
endif
if (keyword_set(pv)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'PV')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,pv,start=s2,count=c2
   for i = 0, c2[1]-1 do begin
      alt_str = STRING ( z_anc[i])
      printf,uid,format='(" UKMO Potential Vorticity (K/s/Pa, PV units) at ",A," km [1:",I3,",",I3,"]")',  $   
        STRTRIM(alt_str,2),c2[0],i+1
      printf,uid,format='("   ",5G13.6)',pv[0:c2[0]-1,i]
   endfor
endif
;if (keyword_set(z_anc)) then begin
if (keyword_set(pressure) or keyword_set(temperature) or keyword_set(pv)) then begin
   printf,uid,format='(" Anc. data altitude grid 0-60 (km) [1:",I3,"]")',nz
   printf,uid,format='("   ",5G13.6)',z_anc[0:nz-1]
endif

if (keyword_set(ozone)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'ozone')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,ozone,start=s2,count=c3
   for i = 0, c3[1]-1 do begin
      alt_str = STRING ( z_ozone[i])
      printf,uid,format='(" Ozone Concentration (molecule/cc) at ",A," km [1:",I3,",",I3,"]")',  $  
        STRTRIM(alt_str,2),c3[0],i+1
      printf,uid,format='("   ",5G13.6)',ozone[0:c3[0]-1,i]
   endfor
;endif
;if (keyword_set(ozone_err)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'ozone_err')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,ozone_err,start=s2,count=c3
   for i = 0, c3[1]-1 do begin
      alt_str = STRING ( z_ozone[i])
      printf,uid,format='(" Ozone Concentration Error Bars (molecule/cc) at ",A," km [1:",I3,",",I3,"]")',  $ 
        STRTRIM(alt_str,2),c3[0],i+1
      printf,uid,format='("   ",5G13.6)',ozone_err[0:c3[0]-1,i]
   endfor
;endif
;if (keyword_set(ozone_flag)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'ozone_flag')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,ozone_flag,start=s2,count=c3
   for i = 0, c3[1]-1 do begin
      alt_str = STRING ( z_ozone[i])
      printf,uid,format='(" Ozone Quality Flag at ",A," km [1:",I3,",",I3,"]")',    $
        STRTRIM(alt_str,2),c3[0],i+1
      printf,uid,format='("   ",5G13.6)',ozone_flag[0:c3[0]-1,i]
   endfor
;endif
;if (keyword_set(z_ozone)) then begin
   printf,uid,format='(" Ozone altitude grid 5-60 (km) [1:",I3,"]")',nO3
   printf,uid,format='("   ",5G13.6)',z_ozone[0:nO3-1]
endif

if (keyword_set(h2o)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'h2o')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,h2o,start=s2,count=c6
   for i = 0, c6[1]-1 do begin
      alt_str = STRING ( z_h2o[i])
      printf,uid,format='(" H2O Vapor Concentration (molecule/cc) at ",A," km [1:",I3,",",I3,"]")',  $ 
        STRTRIM(alt_str,2),c6[0],i+1
      printf,uid,format='("   ",5G13.6)',h2o[0:c6[0]-1,i]
   endfor
;endif
;if (keyword_set(h2o_err)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'h2o_err')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,h2o_err,start=s2,count=c6
   for i = 0, c6[1]-1 do begin
      alt_str = STRING ( z_h2o[i])
      printf,uid,format='(" H2O Vapor Concentration Error Bars (molecule/cc) at ",A," km [1:",I3,",",I3,"]")', $
        STRTRIM(alt_str,2),c6[0],i+1
      printf,uid,format='("   ",5G13.6)',h2o_err[0:c6[0]-1,i] 
   endfor
;endif
;if (keyword_set(h2o_flag)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'h2o_flag')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,h2o_flag,start=s2,count=c6
   for i = 0, c6[1]-1 do begin
      alt_str = STRING ( z_h2o[i])
      printf,uid,format='(" H2O Vapor Quality Flag at ",A," km [1:",I3,",",I3,"]")',  $
        STRTRIM(alt_str,2),c6[0],i+1
      printf,uid,format='("   ",5G13.6)',h2o_flag[0:c6[0]-1,i]
   endfor
;endif
;if (keyword_set(z_h2o)) then begin
   printf,uid,format='(" H2O Vapor altitude grid 5-50  (km)[1:",I3,"]")',nh2o
   printf,uid,format='("   ",5G13.6)',z_h2o[0:nh2o-1]
endif

if (keyword_set(no2)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'no2')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,no2,start=s2,count=c4
   for i = 0, c4[1]-1 do begin
      alt_str = STRING ( z_no2[i])
      printf,uid,format='(" NO2 Concentration (molecule/cc) at ",A," km [1:",I3,",",I3,"]")', $
        STRTRIM(alt_str,2),c4[0],i+1 
      printf,uid,format='("   ",5G13.6)',no2[0:c4[0]-1,i]
   endfor
;endif
;if (keyword_set(no2_err)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'no2_err')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,no2_err,start=s2,count=c4
   for i = 0, c4[1]-1 do begin
      alt_str = STRING ( z_no2[i])
      printf,uid,format='(" NO2 Concentration Error Bars (molecule/cc) at ",A," km [1:",I3,",",I3,"]")',  $
        STRTRIM(alt_str,2),c4[0],i+1
      printf,uid,format='("   ",5G13.6)',no2_err[0:c4[0]-1,i] 
   endfor
;endif
;if (keyword_set(no2_flag)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'no2_flag')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,no2_flag,start=s2,count=c4
   for i = 0, c4[1]-1 do begin
      alt_str = STRING ( z_no2[i])
      printf,uid,format='(" NO2 Quality Flag at ",A," km [1:",I3,",",I3,"]")',   $
        STRTRIM(alt_str,2),c4[0],i+1
      printf,uid,format='("   ",5G13.6)',no2_flag[0:c4[0]-1,i]
   endfor
;endif
;if (keyword_set(z_no2)) then begin
   printf,uid,format='(" NO2 altitude grid 20-45 (km) [1:",I3,"]")',nNO2
   printf,uid,format='("   ",5G13.6)',z_no2[0:nNO2-1]
endif

if (keyword_set(aerosol)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'wavelength')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,wavelength,start=0,count=nchan
   printf,uid,format='(" Aerosol wavelengths (micron) [1:",I3,"]")',nchan
   printf,uid,format='("   ",3G13.6)',wavelength[0:nchan-1]

   index = HDF_SD_NAMETOINDEX(hid,'aerosol')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,aerosol,start=s3,count=c5
   aerosol=reform(aerosol)
   for i = 0, c5[1]-1 do begin
     wave_str = STRING ( wavelength[i])
     for j = 0, c5[2]-1 do begin
      alt_str = STRING ( z_aerosol[j])
      printf,uid,     $
        format='(" Aerosol Extinction (km-1) at ",A," microns at ",A," km [1:",I3,",",I3,",",I3,"]")',$
           STRTRIM(wave_str,2),STRTRIM(alt_str,2),c5[0],i+1,j+1 
      printf,uid,format='("   ",5G13.6)',aerosol[0:c5[0]-1,i,j]
     endfor
   endfor
;endif
;if (keyword_set(aerosol_err)) then begin
   index = HDF_SD_NAMETOINDEX(hid,'aerosol_err')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,aerosol_err,start=s3,count=c5
   aer_err=reform(aerosol_err)
   for i = 0, c5[1]-1 do begin
     wave_str = STRING ( wavelength[i])
     for j = 0, c5[2]-1 do begin
      alt_str = STRING ( z_aerosol[j])
      printf,uid,    $
        format='(" Aerosol Extinction Error Bar (km-1) at ",A," microns at ",A," km [1:",I3,",",I3,",",I3,"]")',$
           STRTRIM(wave_str,2),STRTRIM(alt_str,2),c5[0],i+1,j+1  
      printf,uid,format='("   ",5G13.6)',aer_err[0:c5[0]-1,i,j] 
     endfor
   endfor
;endif
;if (keyword_set(z_aerosol)) then begin
   printf,uid,format='(" Aerosol altitude grid 5-25 (km) [1:",I3,"]")',na
   printf,uid,format='("   ",5G13.6)',z_aerosol[0:na-1]
endif

   
   ;---------------------- CLOSE FILE 
   HDF_SD_END,hid
   HDF_CLOSE,handle 

printf,uid,' '
printf,uid,'End of File'

mes_box = DIALOG_Message( /INFORMATION, 'Output generated in file: ' + STRTRIM(outfilename,2))
if uid ne -1 then free_lun,uid

return

END

;
; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;
 
PRO FileStart_Event, event
 
Widget_Control, get_uvalue=Data, event.top
 
   if ( STRLEN(Data.startdate) ne 8) then begin
;  CHECK DATES ARE VALID ( STARTDATE /= 8 characters)
     err_msg = strarr(2)
     err_msg[0] = 'Input startdate (' +                         $
                  STRTRIM(Data.startdate,2) +                     $
                  ') is not equal to 8 numbers.'
     err_msg[1] = 'Please modifiy start date ( yyyymmdd) appropriately.'
     mes_box = DIALOG_Message ( /ERROR, err_msg)
     return
   endif
   if ( STRLEN(Data.enddate) ne 8) then begin
;  CHECK DATES ARE VALID ( STARTDATE /= 8 characters)
     err_msg = strarr(2)
     err_msg[0] = 'Input enddate (' +                         $
                  STRTRIM(Data.enddate,2) +                     $
                  ') is not equal to 8 numbers.' 
     err_msg[1] = 'Please modifiy end date ( yyyymmdd) appropriately.'
     mes_box = DIALOG_Message ( /ERROR, err_msg)
     return
   endif

   for i = 0, 7 do begin
;  CHECK DATES ARE VALID ( STARTDATE = 8 digits)
      valid = 0
      digit = LONG( STRMID(Data.startdate,i,1))
      ON_IOERROR, bad_num1

      valid = 1
      bad_num1: if ( not valid) then begin
          err_msg = strarr(2)
          err_msg[0] = 'TEST1: Input startdate (' +                           $
                       STRTRIM(Data.startdate,2) +                          $
                       ') contains a character not equal to 0-9.'
          err_msg[1] = 'Please modify start date ( yyyymmdd) appropriately.'
          mes_box = DIALOG_Message ( /ERROR, err_msg)
          return
       endif
   endfor

   for i = 0, 7 do begin
;  CHECK DATES ARE VALID ( ENDDATE = 8 digits)
      valid = 0
      digit = LONG( STRMID(Data.enddate,i,1))
      ON_IOERROR, bad_num2
 
      valid = 1
      bad_num2: if ( not valid) then begin
          err_msg = strarr(2)
          err_msg[0] = 'TEST2: Input enddate (' +                           $
                       STRTRIM(Data.enddate,2) +                          $ 
                       ') contains a character not equal to 0-9.' 
          err_msg[1] = 'Please modify end date ( yyyymmdd) appropriately.'
          mes_box = DIALOG_Message ( /ERROR, err_msg)
          return
       endif
   endfor

   if ( long(Data.startdate) gt long(Data.enddate)) then begin
;  CHECK DATES ARE VALID ( STARTDATE < ENDDATE)
     err_msg = strarr(2)
     err_msg[0] = 'Input startdate (' +                              $
                  STRTRIM(Data.startdate,2) +                          $
                  ') is greater then the enddate (' +                $
                  STRTRIM(Data.enddate,2) + ').'
     err_msg[1] = 'Please modifiy start and/or end dates appropriately.'
     mes_box = DIALOG_Message ( /ERROR, err_msg)
     return
   endif

if ( Data.rev eq 1) then rev = 1
if ( Data.lat eq 1) then lat = 1
if ( Data.lon eq 1) then lon = 1
if ( Data.sec eq 1) then sec = 1
if ( Data.date eq 1) then date = 1
if ( Data.o3 eq 1) then o3 = 1
if ( Data.err_o3 eq 1) then err_o3 = 1
if ( Data.z_o3 eq 1) then z_o3 = 1
if ( Data.aerosol eq 1) then aerosol = 1
if ( Data.err_aerosol eq 1) then err_aerosol = 1
if ( Data.z_aerosol eq 1) then z_aerosol = 1
if ( Data.no2 eq 1) then no2 = 1
if ( Data.err_no2 eq 1) then err_no2 = 1
if ( Data.z_no2 eq 1) then z_no2 = 1
if ( Data.h2o eq 1) then h2o = 1
if ( Data.err_h2o eq 1) then err_h2o = 1
if ( Data.z_h2o eq 1) then z_h2o = 1
if ( Data.z_anc eq 1) then z_anc = 1
if ( Data.temp eq 1) then temp = 1
if ( Data.press eq 1) then press = 1
if ( Data.pv eq 1) then pv = 1
if ( Data.flag_ozone eq 1) then flag_ozone = 1
if ( Data.flag_h2o eq 1) then flag_h2o = 1
if ( Data.flag_no2 eq 1) then flag_no2 = 1
 
read_p3ver4_hdf,Data.filename, Data.outfilename,                     $
   rev=rev,lat=lat,lon=lon,sec=sec,date=date,                        $
   startdate=Data.startdate, enddate=Data.enddate,                   $
      ozone=Data.o3,err_ozone=Data.err_o3,z_ozone=Data.z_o3,         $
      aerosol=Data.aerosol, err_aerosol=Data.err_aerosol,            $
      z_aerosol=Data.z_aerosol,                                      $
      no2=Data.no2,err_no2=Data.err_no2,                             $
      z_no2=Data.z_no2,                                              $
      h2o=Data.h2o,err_h2o=Data.err_h2o,z_h2o=Data.z_h2o,            $
      z_anc=Data.z_anc,temp=Data.temp,press=Data.press,pv=Data.pv,   $
      flag_ozone=Data.flag_ozone,flag_h2o=Data.flag_h2o,             $
      flag_no2=Data.flag_no2

       PRINT,'Generating output file.'
       WIDGET_CONTROL, event.top, /DESTROY
 
End
;
; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;
 
PRO FileWid_Event, event
 

Widget_Control, get_uvalue=Data, event.top
Widget_Control, get_uvalue=cevent, event.id

;print,' ---------------------event---------------'
; print,'cevent = ',cevent
; print,'Data: ',Data
 
 IF ( cevent eq 'bdate') then begin
;    print,'event.value = ',event.value
    Data.startdate = event.value
;    print,'begin date: ',Data.startdate
 ENDIF ELSE IF ( cevent eq 'edate') then begin
;    print,'event.value = ',event.value 
    Data.enddate = event.value
;    print,'  end date: ',Data.enddate
 ENDIF ELSE IF ( cevent eq 'list') then begin 
    ON_OFF = event.select
 
    Val = event.value
    CASE Val OF
        0: BEGIN
            if ( ON_OFF eq 1) then begin
;               PRINT,' Setting values for Pressure....'
               Data.z_anc = 1
               Data.press = 1
            endif else begin
;               PRINT,' UnSetting values for Pressure....'
               Data.z_anc = 0
               Data.press = 0
            endelse
           END
        1: BEGIN
            if ( ON_OFF eq 1) then begin
;               PRINT,' Setting values for Temperature....'
               Data.z_anc = 1
               Data.temp = 1
            endif else begin
;               PRINT,' UnSetting values for Temperature....'
               Data.z_anc = 0
               Data.temp = 0
            endelse
           END
        2: BEGIN
            if ( ON_OFF eq 1) then begin
;               PRINT,' Setting values for Potential Vorticity....'
               Data.z_anc = 1
               Data.pv = 1
            endif else begin
;               PRINT,' UnSetting values for Potential Vorticity....'
               Data.z_anc = 0
               Data.pv = 0
            endelse
           END
        3: BEGIN
            if ( ON_OFF eq 1) then begin
;               PRINT,' Setting values for Ozone....'
               Data.o3 = 1
               Data.err_o3 = 1
               Data.z_o3 = 1
               Data.flag_ozone = 1
            endif else begin
;               PRINT,' UnSetting values for Ozone....'
               Data.o3 = 0
               Data.err_o3 = 0
               Data.z_o3 = 0
               Data.flag_ozone = 0
            endelse
           END
        4: BEGIN
            if ( ON_OFF eq 1) then begin
;               PRINT,' Setting values for Water....'
               Data.h2o = 1
               Data.err_h2o = 1
               Data.z_h2o = 1
               Data.flag_h2o = 1
            endif else begin
;               PRINT,' UnSetting values for Water....'
               Data.h2o = 0
               Data.err_h2o = 0
               Data.z_h2o = 0
               Data.flag_h2o = 0
            endelse
           END
        5: BEGIN
            if ( ON_OFF eq 1) then begin
;               PRINT,' Setting values for NO3....'
               Data.no2 = 1
               Data.err_no2 = 1
               Data.z_no2 = 1
               Data.flag_no2 = 1
            endif else begin
;               PRINT,' UnSetting values for NO3....'
               Data.no2 = 0
               Data.err_no2 = 0
               Data.z_no2 = 0
               Data.flag_no2 = 0
            endelse
           END
        6: BEGIN
            if ( ON_OFF eq 1) then begin
;               PRINT,' Setting values for Aerosol....'
               Data.aerosol = 1
               Data.err_aerosol = 1
               Data.z_aerosol = 1
            endif else begin
;               PRINT,' UnSetting values for Aerosol....'
               Data.aerosol = 0
               Data.err_aerosol = 0
               Data.z_aerosol = 0
            endelse
           END
        ELSE: BEGIN
;            PRINT,' Not one of the numbers'
;            PRINT,' Setting values for DEFAULT....'

            Data.o3 = 1
            Data.err_o3 = 1
            Data.flag_ozone = 1
            Data.z_o3 = 1
            Data.flag_ozone = 1

            Data.aerosol = 1
            Data.err_aerosol = 1
            Data.z_aerosol = 1

            Data.h2o = 1
            Data.err_h2o = 1
            Data.z_h2o = 1
            Data.flag_h2o = 1

            Data.no2 = 1
            Data.err_no2 = 1
            Data.z_no2 = 1
            Data.flag_no2 = 1
        END
    ENDCASE
 ENDIF
 
;print,'Data: ',Data
;print,' ---------------------end event---------------'
 
 
   Widget_Control, event.top, Set_UValue=Data
 
END
;
; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
;
PRO read_poam3v4_hdf 

;    XDisplayFile, './Instruct.README', Title = 'Information'

file_dir = ' '
gfile_dir = ' '

    Data = CREATE_STRUCT (                                          $
                'filename', ' ', 'outfilename', ' ',                $
                'startdate', ' ', 'enddate', ' ',                   $
                'rev', 1, 'lat', 1, 'lon', 1,                       $
                'sec', 1, 'date', 1,                                $
                'z_anc', 0, 'temp', 0, 'press', 0, 'pv', 0,         $
                'o3', 0, 'err_o3', 0, 'z_o3', 0, 'flag_ozone', 0,   $
                'h2o', 0, 'err_h2o', 0, 'z_h2o', 0, 'flag_h2o', 0,  $
                'no2', 0, 'err_no2', 0, 'z_no2', 0, 'flag_no2', 0,  $
                'aerosol', 0, 'err_aerosol', 0, 'z_aerosol', 0)
 
    paramlist = ['UKMO Pressure','UKMO Temperature', 'UKMO Potential Vorticity', $
                 'Ozone (O3) Concentration', 'Water Vapor (H2O) Concentration',        $
                 'Nitrogen Dioxide (NO2) Concentration', 'Aerosol Extinction']

Data.filename = STRTRIM (                                                 $
                  DIALOG_PickFile (/READ, Title='Select POAM3 HDF File', Filter='poam3_ver4_*.hdf')  $
                    , 2)
    if ( Data.filename eq '') then begin
      mes_box = DIALOG_Message( /ERROR, 'Input file not selected.' )
      exit, status=56
    endif
 
Data.outfilename = STRMID (Data.filename,0,STRPOS ( Data.filename,'.hdf',/REVERSE_SEARCH)) + '.out'

;  OPEN AND INITIALIZE HDF FILE INFO
;
   if (HDF_ISHDF(Data.filename) ne 1) then begin
     printf,uid,"Error, not a HDF file: ",Data.filename
     mes_box = DIALOG_Message( /ERROR, 'Input file not a HDF file.' )
     exit, status=56
   endif
   handle = HDF_OPEN(Data.filename,/read)
   hid    = HDF_SD_START(Data.filename,/read)
   index = HDF_SD_NAMETOINDEX(hid,'date')
   sid = HDF_SD_SELECT(hid,index)
   HDF_SD_GETDATA,sid,date
   HDF_SD_END,hid
   HDF_CLOSE,handle

   Data.startdate = STRTRIM(date[0],2)
   Data.enddate = STRTRIM(date[n_elements(date)-1],2)

    tlb = Widget_Base(Column=1, Title='Select Parameters')
 
    Val = 0
 
    Field_txt1   = Widget_Label(tlb, VALUE = 'Optional subsetting by Date:') 
    Field1       = CW_Field(tlb, TITLE = 'Begin Date', VALUE=STRTRIM(Data.startdate),       $
                            /FRAME, /STRING, /All_Events, UVALUE='bdate')
    Field2       = CW_Field(tlb, TITLE = '  End Date', VALUE=STRTRIM(Data.enddate),         $
                            /FRAME, /STRING, /All_Events, UVALUE='edate')
    Field_txt3   = Widget_Label(tlb, VALUE = '---------------------------------------')      
    Field_txt3   = Widget_Label(tlb, VALUE = 'Select the Parameter ( Default is none):')      

    SelectList   = CW_BGROUP(tlb, paramlist, button_uvalue = bdata, /COLUMN, UVALUE='list', $
                             /NONEXCLUSIVE)
    StartBtn     = Widget_Button  (tlb, Value='GO',                                         $
                                   Event_Pro='FileStart_Event',UValue='Quit',               $
                                   SCR_XS = 85, /ALIGN_CENTER)
 
    Widget_Control, tlb, Set_UValue=Data
    Widget_Control, tlb, /Realize

    XMANAGER, 'FileWid', tlb, Event='FileWid_Event'

 
END
