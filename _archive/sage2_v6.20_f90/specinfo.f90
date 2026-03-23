module specinfo
! Define the profile structure
! Field				   Type			Description
   integer,parameter::nper = 2
   type speciesinfo
!Event Specific
         ! Subtangent Altitudes (0, 10, 20, ..., 70) (km)
        real(kind=4),dimension(8) :: Tan_Alt
        ! Subtangent Latitudes  @ Tan_Alt (deg)
        real(kind=4),dimension(8) :: Tan_Lat
        ! Subtangent Longitudes @ Tan_Alt (deg)
        real(kind=4),dimension(8) :: Tan_Lon


!Interpolated Met Vectors
        ! Gridded Pressure profile (mb)
	real(kind=4),dimension(nper*70) :: NMC_Pres
        ! Gridded Temperature profile (K)
	real(kind=4),dimension(nper*70) :: NMC_Temp
        ! Gridded Density profile (cm^(-3))
	real(kind=4),dimension(nper*70) :: NMC_Dens
        ! Error in NMC_Dens (%*100)
	integer(kind=2),dimension(nper*70) :: NMC_Dens_Err
        ! NMC Tropopause height (km)
        real(kind=4) :: Trop_Height
        
!Channel Info
        ! Wavelength of each channel (nm)
	real(kind=4),dimension(7) :: Wavelength

!Species Vectors
        ! O3  Density profile 0-70Km (cm^(-3))
	real(kind=4),dimension(70*nper) :: O3
        ! NO2 Density profile 0-50Km (cm^(-3))
	real(kind=4),dimension(50*nper) :: NO2
        ! H2O Mixing ratio profile 0-50Km (parts per part)
	real(kind=4),dimension(50*nper) :: H2O
        
        ! 385 nm Extinction   0-40Km (1/km)
	real(kind=4),dimension(40*nper) :: Ext386
        ! 453 nm Extinction   0-40Km (1/km)
	real(kind=4),dimension(40*nper) :: Ext452
        ! 525 nm Extinction   0-40Km (1/km)
	real(kind=4),dimension(40*nper) :: Ext525
        ! 1020nm Extinction   0-40Km (1/km)
	real(kind=4),dimension(40*nper) :: Ext1020
        
        ! Calculated Density  0-70Km (cm^(-3))
	real(kind=4),dimension(70*nper) :: Density
        ! Aerosol Surface Area Density (micrometer^2/cm^3)
	real(kind=4),dimension(40*nper) :: SurfDen
        ! Aerosol Effective Radius (micrometer)
        real(kind=4),dimension(40*nper) :: Radius
        ! Middle atmosphere retrieved density 40-75KM (cm^(-3))
        real(kind=4),dimension(35*nper) :: Dens_Mid_Atm
        
        ! Error in  O3 number density profile (%*100)
	integer(kind=2),dimension(70*nper) :: O3_Err
        ! Error in NO2 number density profile (%*100)
	integer(kind=2),dimension(50*nper) :: NO2_Err
        ! Error in H2O number density (%*100)
	integer(kind=2),dimension(50*nper) :: H2O_Err
        
        ! Error in  385nm Extinction (%*100)
	integer(kind=2),dimension(40*nper) :: Ext386_Err
        ! Error in  453nm Extinction (%*100)
	integer(kind=2),dimension(40*nper) :: Ext452_Err
        ! Error in  525nm Extinction (%*100)
	integer(kind=2),dimension(40*nper) :: Ext525_Err
        ! Error in 1020nm Extinction (%*100)
	integer(kind=2),dimension(40*nper) :: Ext1020_Err
        
        ! Error in Density (%*100)
	integer(kind=2),dimension(70*nper) :: Density_Err
        ! Error in Aerosol Surface Area Density (%*100)
	integer(kind=2),dimension(40*nper) :: SURFDen_Err
        ! Error in Aerosol Effective Radius (%*100)
        integer(kind=2),dimension(40*nper) :: Radius_Err
        
        ! Error in middle atmosphere retreived density (%*100)
        integer(kind=2),dimension(35*nper) :: Dens_Mid_Atm_Err
        
        ! Informational Bit-Flag vs Altitude (0-70Km)
	integer(kind=2),dimension(70*nper) :: InfVec
  end type speciesinfo
  contains
     subroutine initspec(s2spec)
        integer(kind=2),parameter :: intfill = -999
        real(kind=4),parameter :: realfill = -999.
        type(speciesinfo) :: s2spec
        s2spec = speciesinfo( &
          realfill,realfill,realfill,&
          realfill,realfill,realfill,intfill,realfill,&
          realfill,&
          realfill,realfill,realfill,&
          realfill,realfill,realfill,realfill,&
          realfill,realfill,realfill,&
          realfill,&
          intfill,intfill,intfill,&
          intfill,intfill,intfill,intfill,&
          intfill,intfill,intfill,&
          intfill,&
          intfill)
     end subroutine initspec
     subroutine fillspec(iffspec,numprof)
        type(speciesinfo) :: s2spec
        integer(kind=4) :: iffspec
        call initspec(s2spec)
        write(iffspec,rec=numprof) s2spec
     end subroutine fillspec
end module specinfo
