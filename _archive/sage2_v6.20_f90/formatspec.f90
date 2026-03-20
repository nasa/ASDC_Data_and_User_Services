subroutine formatspec(unit,spec)
!subroutine to print out one speciesinfo record with standard
!formatting.  Provide the file unit number for output, and
!a variable of type speciesinfo that is to be examined.
   use specinfo
   integer(kind=2),intent(in) :: unit
   type(speciesinfo),intent(in) :: spec
   character(len=20) :: realformat
   character(len=20) :: intformat

   realformat = '(5g15.7)'
   intformat = '(11i7)'
   
   write(unit,'("Tan_Alt")')
   write(unit,realformat) spec%tan_alt
   write(unit,'("Tan_Lat")')
   write(unit,realformat) spec%tan_lat
   write(unit,'("Tan_Lon")')
   write(unit,realformat) spec%tan_lon
   write(unit,'("NMC_Pres")')
   write(unit,realformat) spec%nmc_pres
   write(unit,'("NMC_Temp")')
   write(unit,realformat) spec%nmc_temp
   write(unit,'("NMC_Dens")')
   write(unit,realformat) spec%nmc_dens
   write(unit,'("NMC_Dens_Err")')
   write(unit,intformat) spec%nmc_dens_err
   write(unit,'("Trop_Height")')
   write(unit,realformat) spec%trop_height
   write(unit,'("Wavelength")')
   write(unit,realformat) spec%wavelength
   write(unit,'("O3")')
   write(unit,realformat) spec%o3
   write(unit,'("NO2")')
   write(unit,realformat) spec%no2
   write(unit,'("H2O")')
   write(unit,realformat) spec%h2o
   write(unit,'("Ext386")')
   write(unit,realformat) spec%ext386
   write(unit,'("Ext452")')
   write(unit,realformat) spec%ext452
   write(unit,'("Ext525")')
   write(unit,realformat) spec%ext525
   write(unit,'("Ext1020")')
   write(unit,realformat) spec%ext1020
   write(unit,'("Density")')
   write(unit,realformat) spec%density
   write(unit,'("SurfDen")')
   write(unit,realformat) spec%surfden
   write(unit,'("Radius")')
   write(unit,realformat) spec%radius
   write(unit,'("Dens_Mid_Atm")')
   write(unit,realformat) spec%dens_mid_atm
   write(unit,'("O3_Err")')
   write(unit,intformat) spec%o3_err
   write(unit,'("NO2_Err")')
   write(unit,intformat) spec%no2_err
   write(unit,'("H2O_Err")')
   write(unit,intformat) spec%h2o_err
   write(unit,'("Ext386_Err")')
   write(unit,intformat) spec%ext386_err
   write(unit,'("Ext452_Err")')
   write(unit,intformat) spec%ext452_err
   write(unit,'("Ext525_Err")')
   write(unit,intformat) spec%ext525_err
   write(unit,'("Ext1020_Err")')
   write(unit,intformat) spec%ext1020_err
   write(unit,'("Density_Err")')
   write(unit,intformat) spec%density_err
   write(unit,'("SurfDen_Err")')
   write(unit,intformat) spec%surfden_err
   write(unit,'("Radius_Err")')
   write(unit,intformat) spec%radius_err
   write(unit,'("Dens_Mid_Atm_Err")')
   write(unit,intformat) spec%dens_mid_atm_err
   write(unit,'("InfVec")')
   write(unit,intformat) spec%infvec
end subroutine formatspec
