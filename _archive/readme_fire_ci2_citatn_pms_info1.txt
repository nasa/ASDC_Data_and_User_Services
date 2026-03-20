

      The PMS One-D record has 8 logical records per physical
      record.  The physical record is 2048 bytes total.
      Hwrd is a 16 bit integer.
      Here is the format for each logical record of PMS One-D data.

       Byte    Hwrd   Value
       00-03   1-2    Recorded Time   (10000*SecondsFromMidnight)
       04-05    3     TAS             (100*TAS) in meters per second
       06-07    4     Spare
       08-11   5-6    Date (mmddyy)
       12-13    7     2-DA Shadow Or Count
       14-15    8     2-DA Housekeeping Channel
       16-17    9     2-DA TAS Count
       18-19   10     2-DA Housekeeping Channel Number
       20-21   11     FSSP Size Range (See notes)
       22-23   12     FSSP Channel One Count
         :      :       :     :          :
         :      :       :     :          :
       50-51   26     FSSP Channel Fifteen Count
       52-53   27     Spare FSSP counter
       54-55   28     FSSP total (Ch 1-15)
       56-57   29     FSSP total strobes
       58-59   30     FSSP activity
       60-63  31-32   Spare
       64-65   33     Particle Spacing Monitor (PSM) Range/Control
       66-67   34     PSM Channel One
         :      :      :     :          :
         :      :      :     :          :
       94-95   48     PSM Channel Fifteen
       96-97   49     1D-C Range/Control
       98-99   50     1D-C Channel One Count
         :             :     :          :
         :             :     :          :
      158-159  80     1D-C Channel Thirty-one Count
      160-161  81     1D-C Total (Ch 1-31)
      162-163  82     1D-C Spare Counter #1
      164-165  83     1D-C Spare Counter #2
      166-167  84     1D-C Spare Counter #3
      168-175 85-88   Spare
      176-177  89     1D-P Range/Control
      178-179  90     1D-P Channel One Count
         :             :     :          :
         :             :     :          :
      206-207  104    1D-P Channel Fifteen Count
      208-255 105-128 Spare
1


      Notes:
          For the FSSP, PSM, 1-DC and 1-DP size ranges, each is stored
          in a halfword with other information on a bit function.  With
          bit 0 being the high order bit, the information is --
              bits 0-3    always 0
              bit  4      transit delay - should be 0
              bit  5      velocity averaging - should be 0
              bits 6-7    size range
          Transit delay and velocity averaging are no longer
          available.  These lines are being used for FSSP
          total strobes and FSSP activity.
          The 1-DC probe and the1-DP probe are not adjustable and have
          no size range.  The size range values reflect the setting
          of the switches on the front pannel of the SEA Model 100.
          The 1-DP size range halfword and the PSM size range
          halfword is being used by the display and may have any value.
          The FSSP probe has four size ranges (0-3) --
              0  -  3.0 to 47.5  microns
              1  -  2.0 to 32.0  microns
              2  -  1.0 to 16.0  microns
              3  -  0.5 to  8.0  microns
          The FSSP size range is set using the switches on the front
          pannel of the SEA Model 100.
          The housekeeping channel number for the 2-D probes identifies
          a particular housekeeping channel.  Only one of the eight
          housekeeping channels is recorded with each logical record
          for each probe (the same channel for both probes, however.)
              0  -  +15 V power supply
              1  -  mirror temperature
              2  -  spare
              3  -  spare
              4  -  end element #1
              5  -  end element #32
              6  -  -15 V power supply
              7  -  +5 V power supply
          The -15 volt power supply value is recorded as positive.
          Each of the values has been changed from a voltage to a
          frequency.  One volt is changed to one kilohertz.  The
          -15 volt and +15 volt power supplies are scaled with a
          gain of .1 requiring those values to be multiplied by
          10 to get the correct frequency.  To get the frequency,
          it is necessary to divide the value by the time interval
          since the last logical record.  The mirror temperature is
          fitted to a double exponential curve, and the inverse formula
          is:
              Temp = -124.2 * ln(ln(freq)) - 1.765)
          This value is in degrees Centigrade.
1


          Sample aArea for the FSSP in square centimeters is 0.00362.

          FSSP channel boundaries in microns:

          Channel-0 and channel-1 numbers are from Darrel Baumgardner's
          calibration software (FSPCL.FTN).  Numbers for channel-2 and 3
          are from the FSSP manual.

          Channel 0   2.8,  4.8,  7.6, 12.3, 13.5,
                     15.4, 19.2, 23.8, 26.8, 31.4,
                     35.7, 39.5, 43.2, 46.6, 50.2, 53.7

          Channel 1   2.7,  3.1,  5.0,  8.3, 11.3,
                     13.3, 13.6, 15.4, 17.5, 20.7,
                     23.6, 25.8, 27.7, 30.1, 34.2, 36.8

          Channel 2   1.0,  2.0,  3.0,  4.0,  5.0,
                      6.0,  7.0,  8.0,  9.0, 10.0,
                     11.0, 13.0, 13.0, 14.0, 15.0, 16.0

          Channel 3   0.5,  1.0,  1.5,  2.0,  2.5,
                      3.0,  3.5,  4.0,  4.5,  5.0,
                      5.5,  6.0,  6.5,  7.0,  7.5,  8.0

          PMS One-DC Channel sample areas in square centimeters.
                   0.00227, 0.01733, 0.05056, 0.09413, 0.13872,
                   0.18911, 0.24402, 0.28060, 0.26840, 0.25620,
                   0.24400, 0.23180, 0.21960, 0.20740, 0.19520,
                   0.18300, 0.17080, 0.15860, 0.14640, 0.13420,
                   0.12200, 0.10980, 0.09760, 0.08540, 0.07320,
                   0.06100, 0.04880, 0.03660, 0.02440, 0.01220

          PMS One-DC Channel boundaries in microns.
                    14.2,  34.0,  53.8,  73.6,  93.4,
                   113.2, 132.8, 152.4, 172.2, 193.6,
                   211.8, 231.2, 251.0, 270.6, 290.4,
                   310.0, 330.0, 350.0, 370.0, 390.0,
                   410.0, 430.0, 450.0, 470.0, 490.0,
                   510.0, 530.0, 550.0, 570.0, 590.0, 610.0

          PMS One-DC Channel mid-points in microns.
                   24.1,  43.9,  63.7,  83.5, 103.3,
                  123.0, 142.6, 162.3, 182.9, 202.7,
                  221.5, 241.1, 260.8, 280.5, 300.2,
                  320.0, 340.0, 360.0, 380.0, 400.0,
                  420.0, 440.0, 460.0, 480.0, 500.0,
                  520.0, 540.0, 560.0, 580.0, 600.0/

          PMS One-DP Channel sample areas in square centimeters.
                   14.85, 16.57, 15.78, 14.99, 14.20,
                   13.41, 12.62, 11.83, 11.05, 10.26,
                    9.47,  8.68,  7.89,  7.10,  6.31

          PMS One-DP Channel boundaries in microns.
                    213.0,  510.0,  813.0, 1104.0, 1404.0,
                   1698.0, 1992.0, 2289.0, 2586.0, 2877.0,
                   3180.0, 3465.0, 3750.0, 4050.0, 4350.0, 4650.0

          PMS One-DP Channel mid-points in microns.
                   361.5,  661.5,  958.5, 1254.0, 1551.0,
                  1845.0, 2140.5, 2437.5, 2731.5, 3028.5,
                  3322.5, 3607.5, 3900.0, 4200.0, 4500.0
1




          The PMS 2D record is 4128 bytes long.  Its format is as
          follows:


          PMS 2-D RECORD

          4128 BYTES, ARRAY(1032) is a 32 bit integer array.

          NAME(SIZE)  TYPE  DESCRIPTION                       LOC IN ARRAY
          ----------  ----  -----------                       --- -- ------
          FRONT(1)    INT   CONSTANT  Y'10100001'                 0001
          IMAGE(1024) INT   2-D IMAGE SLICES                      0002-1025
          STTIME(1)   INT   SECONDS FROM MIDNIGHT * 10000         1026
                            AT START OF 2-D IMAGE
          NDTIME(1)   INT   SECONDS FROM MIDNIGHT * 10000         1027
                            AT END OF 2-D IMAGE
          STTAS(1)    INT2  TAS * 100 AT START OF 2-D IMAGE.      1028
          NDTAS(1)    INT2  TAS * 100 AT END OF 2-D IMAGE.        1028
          SPARE(1)    INT   SPARE LOCATION                        1029
          DATE(1)     INT   DATE (MMDDYY).                        1030
          SPRPMS(2)   INT   SPARE LOCATIONS FOR EXPANSION         1031-1032


            NOTES :

          Locations in the array are fullword subscripts.

          The TAS values are stored in the two half words of
          fullword ARRAY(1028).

          EACH ELEMENT OF IMAGE REPRESENTS ONE 2-D IMAGE SLICE.
          EACH BIT OF EACH SLICE INDICATES WHETHER THE CORRES-
          PONDING 2-D PHOTO SENSOR WAS LIT OR UNLIT.  A LIT
          SENSOR INDICATES NO IMAGE AND SETS THE BIT.  AN UNLIT
          SENSOR INDICATES AN IMAGE IS PRESENT AND RESETS THE BIT.

          AS WITH THE PMS SLOW RECORD, THE TIME IS ACCURATE TO
          ONLY THREE PLACES (AFTER DIVIDING BY 10000).

          STTIME IS THE TIME WHEN THE 2-D PROBE WAS SIGNALED TO
          BEGIN COLLECTING 2-D IMAGES.  NDTIME IS THE TIME WHEN
          THE 2-D PROBE'S BUFFER WAS FULL AND IT SIGNALED THAT
          CONDITION.

          The diode spacing for this probe is 33 microns.

      If you have any questions concerning the format,
      please contact:

          Martin Brown
          University of North Dakota
          Center for AeroSpace Sciences
          P.O. Box 9022, University Station
          Grand Forks, ND  58202-9022
          vox: 701-777-2954
          fax: 701-777-2940

