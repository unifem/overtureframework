create mappings
*
open a data-base
   ../mapping/frontValveExit.hdf
   open an old file read-only
   get all mappings from the data-base
*
open a data-base
   ../mapping/frontPortUpper.hdf
   open an old file read-only
   get all mappings from the data-base
*
  Box
   mappingName
    portCore
    * aspect ratios: 32 51.25 35
    specify corners
      54 18.75 127  86 70 162        54 18.75 127  86 65 162         54 18.75 127  86 60 158
    rotate
      -29.539 1
      70. 20 142.5
    lines
      31 53 31
   boundary conditions
     0 0 4 0 0 0
   share
     0 0 4 0 0 0
   exit
*
  Box
   mappingName
    exitCore
    * aspect ratios  dx=58-41=17  dz=120-150=-30 
    specify corners
       41.  32  150   55.  66. 120.   41.  32  150   51.  66. 120.
    rotate
      -29.539 1
      41.  35  150
    lines
      11 31 25
   boundary conditions
     2 0 0 0 0 0
   share
     2 0 0 0 0 0
   exit
*
  Cylinder
    orientation
      2, 0, 1
    bounds on the axial variable
      18.75 60.      18.75 62.
    bounds on the radial variable
      4.7375  7.735
    center for cylinder
      69.70 0. 142.55   69.75 0. 142.5  70. 0. 142.5
    lines
      27 21 5
    boundary conditions
      -1 -1 4 0 6 0
    share
       0  0 4 0 6 0
    mappingName
      valveStem
    exit
*
exit
*
  generate an overlapping grid
    portCore
    exitCore
    frontValveInletTopVolume
    valveStemExitVolume
    frontPortVolume
    frontPortUpperVolume
    frontPortEndVolume
    topFrontPortVolume
    valveStem
    done
    change parameters
      prevent hole cutting
        frontValveInletTopVolume
          frontPortVolume 
        frontValveInletTopVolume
          frontPortEndVolume
        frontValveInletTopVolume
          topFrontPortVolume
        frontValveInletTopVolume
          valveStemExitVolume
*
        frontPortVolume
          frontValveInletTopVolume
        frontPortVolume
          frontPortEndVolume
        frontPortVolume
          frontPortUpperVolume
        frontPortUpperVolume
          frontPortEndVolume
*
        frontPortEndVolume
         frontPortUpperVolume
        frontPortEndVolume
         frontPortVolume
        frontPortEndVolume
         frontValveInletTopVolume
*
        topFrontPortVolume
          frontValveInletTopVolume
        topFrontPortVolume
          frontPortEndVolume
        topFrontPortVolume
          frontPortVolume
        topFrontPortVolume
          valveStemExitVolume
*
        valveStem
          valveStemExitVolume
      done
    exit
    compute






    change parameters
      shared boundary normal tolerance
        .75
    exit
    compute
    compute

    compute

    compute


     valveStemExitVolume
    valveStem
    done
    allow hanging interpolation
    display intermediate results
    compute
    





    exitCore
    portCore
    frontPortVolume
    frontPortUpperVolume
    frontPortEndVolume
    done
    display intermediate results
pause
    compute
    compute







     frontValveInletTopVolume
     valveStemExitVolume
*     valveStem
    done
    allow hanging interpolation
    display intermediate results
    compute



    portCore
    frontPortEndVolume
     frontPortVolume
    frontPortUpperVolume
    done
    display intermediate results






     exitCore
     frontPortEndVolume
     frontPortVolume
     topFrontPortVolume
     frontPortUpperVolume
    done
    allow hanging interpolation
    display intermediate results
    compute






     exitCore
     frontPortEndVolume
     frontPortVolume
     topFrontPortVolume
     frontPortUpperVolume
    done
    allow hanging interpolation
    display intermediate results
    compute



    portCore
    exitCore
    frontValveInletTopVolume
    valveStemExitVolume
    frontPortVolume
    frontPortUpperVolume
    frontPortEndVolume
    topFrontPortVolume
    valveStem
    display intermediate results
    change parameters
      shared boundary normal tolerance
        .75
    exit











    frontValveInletTopVolume
    topFrontPortVolume
    frontPortVolume
    done
    allow hanging interpolation
    display intermediate results

    compute




    frontValveInletTopVolume
    valveStemExitVolume
    frontPortVolume
    topFrontPortVolume



    portCore
    exitCore
    frontValveInletTopVolume
    valveStemExitVolume
    frontPortVolume
    frontPortUpperVolume
    frontPortEndVolume
    topFrontPortVolume
    valveStem
    display intermediate results




    frontValveInletTopVolume
    valveStemExitVolume
    done choosing mappings
    * allow hanging interpolation

