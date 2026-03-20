files = findfile('*_1.S')
num = n_elements(files)
data = fltarr(3,191)
header = fltarr(6)
salts = findgen(51)*0.5+10.
sdata = [-1]
;plot_oi,[0],xrange=[0.0001,0.1],yrange=[10.,35.]
for k = 0, num-1 do begin
	close,1
	openr,1,files(k)
	while (not EOF(1)) do begin
		readf,1,header
		lattmp = header(2)+header(3)*abs(header(2))/header(2)/60.
		readf,1,data
		sdatatmp = spline(data(0,*),smooth((data(1,*)-1.)*exp(-1.*(data(0,*)-10.)/6.5),5),salts)
;		oplot,sdatatmp/100.,salts
		if(sdata(0) eq -1) then begin
			sdata = sdatatmp
			lat = [lattmp]
		endif else begin
			sdata = [sdata,sdatatmp]
			lat = [lat,lattmp]
		endelse
	endwhile
endfor
num = n_elements(lat)
sdata = reform(sdata,51,num)
sarray = fltarr(17,51)
for k = 0, 16 do begin
	minlat = float(k)*5.-50.
	maxlat = minlat+5.
	s = where(lat ge minlat and lat le maxlat)
	for j = 0, 50 do sarray(k,j) = median(sdata(j,s))
endfor
slats = findgen(17)*5.0-47.5
cl = 10.^(findgen(46)/10.-4.)
contour,sarray,slats,salts,level=cl,/cell_fill
contour,sarray,slats,salts,level=[0.0001,0.0005,0.001,0.005,0.01,0.05,0.1,0.5,1.],/follow,/noerase
end