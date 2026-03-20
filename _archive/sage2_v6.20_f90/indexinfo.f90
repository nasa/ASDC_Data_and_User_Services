module indexinfo
! Define the header structure
! Field				   Type				Description
       type index
	integer(kind=4) :: Num_Prof		! Number of profiles(records) in these files

!Revision Info
	integer(kind=4) :: Met_Rev_Date 	! LaRC Met Model Rev Date (yyyymmdd)
	character(len=8) :: Driver_Rev 	 ! LaRC Driver version (eg. 6.0a)
	character(len=8) :: Transmission_Rev   ! LaRC Transmission version
	character(len=8) :: Inversion_Rev      ! LaRC Inversion version
	character(len=8) :: Spectroscopy_Rev   ! LaRC Inversion version
	character(len=32) :: Eph_File_Name     ! Ephem file name
	character(len=32) :: Met_File_Name     ! Met file name
	character(len=32) :: Ref_File_Name     ! Refraction file name
	character(len=32) :: Trans_File_Name   ! Transmission file name
	character(len=32) :: Spec_File_Name    ! Species profile file name
	real(kind=4) ::      FillVal           ! Fill value

!Altitude grid and range info
	real(kind=4) :: Grid_Size                  ! Altitude Grid spacing (0.5km)
	real(kind=4),dimension(200) :: Alt_Grid    ! Geometric Altitudes (0.5, 1.0,..., 100.0km)
        real(kind=4),dimension(70) :: Alt_Mid_Atm   ! Altitudes for mid atmosphere
	real(kind=4),dimension(2) :: Range_Trans   ! Transmission Min & Max altitudes       [0.5,100.]
	real(kind=4),dimension(2) :: Range_O3      ! Ozone Density Min & Max altitudes      [0.5,70.0]
	real(kind=4),dimension(2) :: Range_NO2     ! NO2 Density Min & Max altitudes        [0.5,50.0]
	real(kind=4),dimension(2) :: Range_H2O     ! H2O Density Min & Max altitudes        [0.5,50.0]
	real(kind=4),dimension(2) :: Range_Ext     ! Aerosol Extinction Min & Max altitudes [0.5,40.0]
	real(kind=4),dimension(2) :: Range_Density ! Density Min & Max altitudes            [0.5,70.0]
	real(kind=4),dimension(2) :: Range_Surface ! Surface Area Min & Max altitudes       [0.5,40.0]

!Event Specific Info useful for data subsetting
	integer(kind=4),dimension(930) :: YYYYMMDD     ! Event Date (yyyymmdd) at 20km
        integer(kind=4),dimension(930) :: event_num    ! The event number
	integer(kind=4),dimension(930) :: HHMMSS       ! Event Time (hhmmss) at 20km
	real(kind=4),dimension(930) :: Day_Frac        ! Time of Year (ddd.fraction)
	real(kind=4),dimension(930) :: Lat             ! Subtangent Latitude at 20km (-90 to 90)
	real(kind=4),dimension(930) :: Lon             ! Subtangent Longitude at 20km (-180 to 180)
	real(kind=4),dimension(930) :: Beta            ! Spacecraft Beta angle (degrees)
	real(kind=4),dimension(930) :: Duration        ! Duration of event (seconds)
	integer(kind=2),dimension(930) :: Type_Sat     ! Event Type, Instrument (0=sr, 1=ss)
	integer(kind=2),dimension(930) :: Type_Tan     ! Event Type, Local (0=sr,1=ss)

!Process Tracking
 !Flag info
        integer(kind=4),dimension(930) :: Dropped      ! Non-zero if event is dropped
        integer(kind=4),dimension(930) :: InfVec      ! Bit flags describing event processing
 !Ephemeris
	integer(kind=4),dimension(930) :: Eph_Cre_Date ! Record creation date (yyyymmdd)
	integer(kind=4),dimension(930) :: Eph_Cre_Time ! Record creation time (hhmmss)
 !Met
	integer(kind=4),dimension(930) :: Met_Cre_Date ! Record creation date (yyyymmdd)
	integer(kind=4),dimension(930) :: Met_Cre_Time ! Record creation time (hhmmss)
 !Refraction
	integer(kind=4),dimension(930) :: Ref_Cre_Date ! Record creation date (yyyymmdd)
	integer(kind=4),dimension(930) :: Ref_Cre_Time ! Record creation time (hhmmss)
 !Transmission
	integer(kind=4),dimension(930) :: TRANS_Cre_Date ! Record creation date (yyyymmdd)
	integer(kind=4),dimension(930) :: TRANS_Cre_Time ! Record creation time (hhmmss)
 !Inversion
	integer(kind=4),dimension(930) :: SPECIES_Cre_Date ! Record creation date (yyyymmdd)
	integer(kind=4),dimension(930) :: SPECIES_Cre_Time ! Record creation time (hhmmss)
 end type index
 contains
    subroutine initindex(s2ind)
       integer(kind=4),parameter :: intfill = -999    !integer fill value
       integer(kind=4),parameter :: flagfill = 0
       real(kind=4),parameter :: realfill = -999.0
       type(index) :: s2ind
       s2ind = index( &
         intfill,&

         intfill,"","","","","",&
         "","","","",realfill,&
         
         realfill,realfill,realfill,realfill,realfill, &
         realfill,realfill,realfill,realfill,realfill,&
         
         intfill,intfill,intfill,realfill,realfill,&
         realfill,realfill,realfill,intfill,intfill,&

         intfill,flagfill,&
         intfill,intfill,&
         intfill,intfill,&
         intfill,intfill,&
         intfill,intfill,&
         intfill,intfill)
    end subroutine initindex
end module indexinfo



