PROGRAM READ_CERES_EBAF

!+
!NAME:
!  READ_CERES_EBAF
!PURPOSE:
!  This program will read the EBAF full netcdf file.
!CATEGORY:
!  netcdf read
!COMPILE:
! If you have the nc-config command available as part of your netcdf installation, use:
!  gfortran `nc-config --fflags` read_ceres_ebaf.f90 `nc-config --flibs` -ldl -o read_ebaf
! OR if you would like to specify your netCDF include and library directories yourself, use:
!  gfortran -I/your_include_dir read_ceres_ebaf.f90 -L/your_lib_dir -lyour_lib1 -lyour_lib2 -ldl -o read_ebaf
! Example: gfortran -g -O2 -I/usr/local/include read_ceres_ebaf.f90 -L/usr/local/lib -lnetcdf -lnetcdff -ldl -o read_ebaf
!RUN:
!  read_ebaf
!INPUT:
!  CERES_EBAF_Edition4.1_200003-201809.nc
!OUTPUT:
!  ASCII file: info_about_ncfile.txt
!  ASCII file: sample_output_ebaf.txt
!KEYWORDS:
!  none
!NOTES:
!
!AUTHOR: CERES Team

USE netcdf
IMPLICIT NONE

CHARACTER(LEN=255) :: ncfile,outfile,dname,vname,aname,sname
CHARACTER(LEN=10), DIMENSION(6)  :: datatype
CHARACTER(LEN=21)  :: file_info
CHARACTER(LEN=1)   :: cndims
CHARACTER(LEN=2)   :: calen
CHARACTER(LEN=40)  :: dformat,aformat

CHARACTER(LEN=255),DIMENSION(:), ALLOCATABLE :: dim_names

INTEGER :: ncid,idim,status,ndims_nc,nvars_nc,ivar,xtype,dimlen
INTEGER :: ndims,natts,imonth
INTEGER :: ntime,dim1,var_id,jj
INTEGER :: atype,iatt,alen,i

INTEGER,DIMENSION(:), ALLOCATABLE :: var_dimids

REAL, DIMENSION(:), ALLOCATABLE :: avalue
!example global mean variables that are extracted from the netcdf file
REAL,DIMENSION(:), ALLOCATABLE ::  gsfc_net_sw_all_mon,&
    gsfc_net_lw_all_mon,gsfc_net_tot_all_mon

datatype(1) = 'Byte'
datatype(2) = 'Character'
datatype(3) = 'Short'
datatype(4) = 'Integer'
datatype(5) = 'Real'
datatype(6) = 'Double'

ncfile = 'CERES_EBAF_Edition4.1_200003-201901.nc4'

file_info = 'info_about_ncfile.txt'
OPEN(27,FILE=file_info,STATUS='UNKNOWN')

!open netCDF file for READ only
status = NF90_OPEN(ncfile,NF90_NOWRITE,ncid)

!find out information about the netCDF file 
!ndims_nc = number of dimensions defined for the entire netCDF file
!nvars_nc = number of variables defined for the entire netCDF file (this includes the dimension variables)
status = NF90_INQUIRE(ncid,ndims_nc,nvars_nc)

ALLOCATE(dim_names(ndims_nc))
ALLOCATE(var_dimids(ndims_nc))

!list dimensions in netcdf file
WRITE(27,'(A:,I3)')'Number of defined dimensions in the file:',ndims_nc
DO idim = 1,ndims_nc 
  status = NF90_INQUIRE_DIMENSION(ncid,idim,NAME=dname,LEN=dimlen)   !find dimension info
  dim_names(idim) = dname
  WRITE(27,'(A:,I3,3A:,I6)') ' Dimension ',idim,' ',TRIM(dname),&
       '      Length = ',dimlen
ENDDO
WRITE(27,'(A:)') '***********************************************************************************'
WRITE(27,*) ''

WRITE(27,'(A:)') 'Here is a list of the variables found in the file:'
!list variables in file
DO ivar = 1,nvars_nc
  status = NF90_INQUIRE_VARIABLE(ncid,ivar,vname,NDIMS=ndims,NATTS=natts,&
       XTYPE=xtype,DIMIDS=var_dimids)  !find variable info--data type and number of dimensions and attributes 

  WRITE(27,'(A:,I3,2A)') 'Variable ',ivar, ':  ',TRIM(vname)
  WRITE(27,'(A15,A9,A25,I2)') 'Datatype = ',datatype(xtype),& 
   'Number of dimensions = ',ndims
  IF (ndims .GT. 0) THEN
     WRITE(cndims,"(I1)") ndims
     dformat='(A21,'//cndims//'(A:,", "))'  !string will be (A19,n(A:,", ")) where n=cndims
     WRITE(27,dformat) 'Dimensions are : ',&
          (TRIM(dim_names(var_dimids(jj))),jj=1,ndims)
  END IF
! List attributes.
  WRITE(27,'(A27,I2)') 'Number of attributes = ',natts
ENDDO !ivar

CLOSE(27)

! ----------- The following is a data read/write example---------------
!Three global mean fluxes will be written to a file.

!extract data from netcdf file
!Knowing the dimension names for the variables you want, get the dimension lengths.
status=NF90_INQ_DIMID(ncid,'time',dim1)     !find dimension id
status = NF90_INQUIRE_DIMENSION(ncid,dim1,LEN=ndims)   !find dimension info
ntime = ndims

!Allocate the space needed for the three flux variables.
ALLOCATE(gsfc_net_sw_all_mon(ntime))
ALLOCATE(gsfc_net_lw_all_mon(ntime))
ALLOCATE(gsfc_net_tot_all_mon(ntime))

!Extract the three flux variables.
status=NF90_INQ_VARID(ncid,'gsfc_net_sw_all_mon',var_id)   !find variable id
status=NF90_GET_VAR(ncid,var_id,gsfc_net_sw_all_mon)       !extract data

status=NF90_INQ_VARID(ncid,'gsfc_net_lw_all_mon',var_id)   !find variable id
status=NF90_GET_VAR(ncid,var_id,gsfc_net_lw_all_mon)       !extract data

status=NF90_INQ_VARID(ncid,'gsfc_net_tot_all_mon',var_id)   !find variable id
status=NF90_GET_VAR(ncid,var_id,gsfc_net_tot_all_mon)       !extract data

!close netCDF file
status=NF90_CLOSE(ncid)

! Write the first two year's worth of data to an ascii file.
outfile = 'sample_output_ebaf.txt'
OPEN(28,FILE=trim(outfile),STATUS='UNKNOWN')
WRITE(28,*)'                     Global Mean'
WRITE(28,*)'Surface SW Net  Surface LW Net   Surface Total Net All-sky'

DO imonth=1,24
  WRITE(28,*) gsfc_net_sw_all_mon(imonth),gsfc_net_lw_all_mon(imonth), &
    gsfc_net_tot_all_mon(imonth)
ENDDO

CLOSE(28)

END

