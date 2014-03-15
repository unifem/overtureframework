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
     0 0 0 0 0 0
   share
     0 0 0 0 0 0
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
open a data-base
   ../mapping/catPortCylinderEdge.hdf
   open an old file read-only
   get all mappings from the data-base
*
open a data-base
  ../mapping/catPortCylinderEdgePort.hdf
  open an old file read-only
  get all mappings from the data-base
*
  Cylinder
    mappingName
     cylinder
    orientation
      2 0 1
    bounds on the radial variable
      42.5 62.5   40 62.5   40 62    40 63  40 65   40 60
    bounds on the axial variable
      -40 0
    centre for cylinder
      78 0 105
    lines
      * worked 201 9 13
      191 9 11    201 9 13
   boundary conditions
     -1 -1 1 1 0 1
   share
     0 0 4 1 0 5
    exit
*
  Box
    * aspect ratios 
    specify corners
      33 -40 60 123 0 150  
    lines
      * worked 61 9 61 
      51 9 51 
   mappingName
     cylinderCore
   boundary conditions
     0 0 4 1 0 0
   share
     0 0 4 1 0 0
   exit
*
  Box
    * aspect ratios 36 10 36
    specify corners
     23 -5 77   59 5.25  113   17 -5 77   59 5.25  113      23 -5 77   59 5.25  113         22 -5 77   58 5.25  113
    lines
      19 7 19  23 7 19 
   mappingName
     leftPortCore
   boundary conditions
     0 0 0 2 0 0
   share
     0 0 0 2 0 0
   exit
*
  Box
*    18.75           
    specify corners
       51 -5 124  88 18.75  161       51 -5 124   88 5.25  161  
    lines
      19 19 19   19 7 19  19 7 23
   mappingName
     rightPortCore
   boundary conditions
     0 0 0 0 0 0
   share
     0 0 0 0 0 0
   exit
*
open a data-base
   ../mapping/rightPortGrid.hdf
   open an old file read-only
   get all mappings from the data-base
*
* here is the valve
*
  open a data-base
   ../mapping/valveVolume.hdf
   open an old file read-only
   get all mappings from the data-base
*
  Box
    * centre 69.5,-12.75,143,
    specify corners
      59.5 -22.5 133  79.5 -12.75 153      -12.5
    lines
      * worked 61 9 61 
      11 6  11 
   mappingName
     valveBottom
   boundary conditions
     0 0 0 6 0 0
   share
     0 0 0 6 0 0
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
    valveBottom
    valveVolume
    rightPortVolume
*
    cylinderCore
    cylinder
    rightPortCore
    rightPortEdge
    rightPortBaseVolume
    rightPortBaseOverhangVolume
    leftPortCore
    leftPortEdge
    leftPortBaseVolume
    leftPortBaseOverhangVolume
    joinStrip
    done
    change parameters
      boundary conditions
        frontPortUpperVolume
          -1 -1 0 0 1 0
        rightPortBaseVolume
           0  0 0 0 1 0
        rightPortBaseOverhangVolume
           0  0 0 0 1 0
        done
      prevent hole cutting
        cylinder
          all
        all
          cylinder
        cylinderCore
          all
        done
      allow hole cutting
        cylinder
          cylinderCore
      done
      prevent hole cutting
*
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

