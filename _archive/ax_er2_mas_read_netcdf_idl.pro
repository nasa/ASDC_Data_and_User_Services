; This program is a modification of the hdfread.pro program 
; taken from the following web page www.dfanning.com/programs
; It was modified by Susan Haberer for CSC to read MAS (SCAR B)
; netcdf granules.

FUNCTION TEST_ISHDF,filename
CATCH, err
IF (err EQ 0) THEN RETURN, HDF_ISHDF(filename) $
        ELSE RETURN, 0
END ;------------------------------------------------------------------


PRO MAS_READ_NETCDF_IDL, filename   ; Open file and initialize the SDS interface.

IF N_ELEMENTS(filename) EQ 0 THEN filename = PICKFILE()
IF NOT TEST_ISHDF(filename) THEN BEGIN
   PRINT, 'Invalid HDF file ...'
   RETURN
   ENDIF ELSE $
   PRINT, 'Valid HDF file. Opening "' + filename + '"'

report_file = filename +'.rpt'
Openw, lun, report_file,/Get_Lun
print, 'Report file is called',report_file
printf, lun,'This is a report file for ',filename

newFileID = HDF_SD_START(filename, /READ)

   ; What is in the file. Print the number of
   ; dmv report_file scarb_mas_950825.00atasets, attributes, and palettes.

PRINTf,lun, 'Reading number of datasets and file attributes in file ...'
HDF_SD_FILEINFO, newFileID, datasets, attributes
numPalettes = HDF_DFP_NPALS(filename)
PRINTf,lun, '
PRINTf,lun, 'No. of Datasets:   ', datasets
PRINTf,lun, 'No. of File Attributes: ', attributes
PRINTf,lun, 'No. of Palettes:   ', numPalettes

   ; Print the name of each file attribute
glob = ' '
read, glob, prompt='Do you want the global attributes printed to the output file?'
global = strupcase(glob)
While Not ((global EQ 'Y') OR (global EQ 'N')) do begin
   read, glob,prompt='You must enter a Y for yes or a N for no!'
   global = strupcase(glob)
endwhile 
if (global eq 'Y') then begin
   PRINTf,lun, ''
   PRINTf,lun, 'Printing name of each file attribute...'
   FOR j=0, attributes-1 DO BEGIN
      HDF_SD_ATTRINFO, newFileID, j, NAME=thisAttr
      PRINTf,lun, 'File Attribute No. ', + STRTRIM(j, 2), ': ', thisAttr
      AttID = HDF_SD_ATTRFIND(newFileID,thisAttr)
      HDF_SD_ATTRINFO, newFileID, AttID, DATA=thisdata
      PRINTf, lun, thisAttr,' ', thisdata
   ENDFOR
endif
         ; Print the name of each SDS and associated data attributes.

PRINTf,lun, ''

; to get select dataset info
FOR j=0, datasets-1 DO BEGIN
   thisSDS = HDF_SD_SELECT(newFileID, j)
   HDF_SD_GETINFO, thisSDS, NAME=thisSDSName, NATTS=numAttributes
   PRINT, 'Dataset No. ', (STRTRIM(j,2)+1), ': ', thisSDSName
ENDFOR
DS = ' '
ALL = ' '
DS_SEL = BYTARR(44)
D = 0
ALLSTOP = 1
READ,ALL, PROMPT='Enter 0 for all datasets to be displayed or 1 to select datasets'
if (ALL EQ 1) then begin
  REPEAT BEGIN
    READ,DS,PROMPT='Enter dataset no. then <cr> or zero to end selection process'
    DSS = FIX(DS)
    DS_SEL[D] = DSS
    D = D+1
    if (DS eq 0) then allstop = 0
   endrep until (allstop eq 0)
endif

if (all EQ 0) then begin
FOR j=0, datasets-1 DO BEGIN
   thisSDS = HDF_SD_SELECT(newFileID, j)
   HDF_SD_GETINFO, thisSDS, NAME=thisSDSName, NATTS=numAttributes
   PRINTf,lun, 'Dataset No. ', STRTRIM(j,2), ': ', thisSDSName
   FOR k=0,numAttributes-1 DO BEGIN
      HDF_SD_ATTRINFO, thisSDS, k, DATA=thisData, NAME=thisAttrName
      PRINTf,lun, '   Data Attribute: ', thisAttrName, ' NAME: ',thisData
      
   ENDFOR
   PRINTf,lun, ''
   index = HDF_SD_NAMETOINDEX(newFileID, thisSDSName)
 
   ; Select the SDS.

   thisSdsID = HDF_SD_SELECT(newFileID, index)

   ; Print the names of the Data attributes.
   HDF_SD_GETINFO, thisSdsID, NATTS=numAttributes
   PRINTf,lun, 'Number of attributes: ', numAttributes
   
   ; Get the data.

   PRINTf,lun, ''
   PRINT, 'Reading data for ',thisSDSName
   HDF_SD_GETDATA, thisSdsID, newData 
   ;if (thisSDSName NE 'DataSetHeader') then printf,lun, ' MIN/MAX ', min(newData), max(newData)
   if (thisSDSName EQ 'DataSetHeader') then begin 
      s = strlen(newData)
      
   endif
   if (thisSDSName NE 'DataSetHeader') then printf, lun,newData else $
       begin
         i = 0
         while ( i LT s ) do begin
           b = strmid(newData,i,80)
           printf, lun,b
           i = i + 80
         endwhile
       endelse
   PRINTf,lun, ''
ENDFOR
endif else begin
d = 0
SEL_END = 1
REPEAT begin
   j = DS_SEL[D] 
   
   j = j - 1
   thisSDS = HDF_SD_SELECT(newFileID, j)
   HDF_SD_GETINFO, thisSDS, NAME=thisSDSName, NATTS=numAttributes
   PRINTf,lun, 'Dataset No. ', STRTRIM(j,2), ': ', thisSDSName
   FOR k=0,numAttributes-1 DO BEGIN
      HDF_SD_ATTRINFO, thisSDS, k, DATA=thisData, NAME=thisAttrName
      PRINTf,lun, '   Data Attribute: ', thisAttrName, ' NAME: ',thisData
      
   ENDFOR
   PRINTf,lun, ''
   index = HDF_SD_NAMETOINDEX(newFileID, thisSDSName)
 
   ; Select the SDS.

   thisSdsID = HDF_SD_SELECT(newFileID, index)

   ; Print the names of the Data attributes.
   HDF_SD_GETINFO, thisSdsID, NATTS=numAttributes, ndims=dims
   PRINTf,lun, 'Number of attributes: ', numAttributes
   
   ; Get the data.

   PRINTf,lun, ''
   PRINT, 'Reading data for ',thisSDSName
   HDF_SD_GETDATA, thisSdsID, newData
   if (thisSDSName EQ 'DataSetHeader') then begin
      s = strlen(newData)
      
   endif
   if (thisSDSName NE 'DataSetHeader') then printf, lun,newData else $
       begin
         i = 0
         while ( i LT s ) do begin
           b = strmid(newData,i,80)
           printf, lun,b
           i = i + 80
         endwhile
       endelse
   
   PRINTf,lun, ''
   D = D + 1
   if (DS_SEL[D] EQ 0) then sel_end = 0
ENDREP UNTIL (sel_end eq 0)
endelse

HDF_SD_END, newFileID
PRINT, 'Read operation complete.'
Free_Lun,lun
END
