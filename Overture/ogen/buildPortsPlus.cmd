*
* build the grid around the upper part of the cylinder
* where the two ports enter the cylinder.
*   -- plus the right port
*
create mappings
open a data-base
   ../mapping/rightPortGrid.hdf
   open an old file read-only
   get all mappings from the data-base
*
open a data-base
   ../mapping/catPortCylinderEdge.hdf
   open an old file read-only
   get all mappings from the data-base
*
open a data-base
  ../mapping/catRightPortBase.hdf
  open an old file read-only
  get all mappings from the data-base
*
  Cylinder
    mappingName
     cylinder-unstretched
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
      241 29 25  221 25 21   201 23 19   191 19 17    191 19 13        191 9 11    201 9 13
   boundary conditions
     -1 -1 1 1 0 1
   share
     0 0 4 1 0 5
    exit
  * Stretch coordinates
  stretch coordinates
    transform which mapping?
    cylinder-unstretched
    stretch
      specify stretching along axis=2
      layers
        1
        1. 6. 1.   1. 8. 1.
      exit
    exit
    mappingName
    cylinder
    * pause
    exit
*
  Box
    * aspect ratios 
    specify corners
      33 -40 60 123 0 150  
    lines
      * worked 61 9 61 
      51 19  51  51 9 51 
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
    specify corners
       51 -5 124  88 18.734 161       51 -5 124  88 5.25  161  
    lines
      19 15 19  19 7 19 
   mappingName
     rightPortCore
   boundary conditions
     0 0 0 3 0 0
   share
     0 0 0 3 0 0
   exit
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
      11 9  11 
   mappingName
     valveBottom
   boundary conditions
     0 0 0 6 0 0
   share
     0 0 0 6 0 0
   exit
*
*
  Cylinder
    orientation
      2, 0, 1
    bounds on the axial variable
     13.734 18.734
    bounds on the radial variable
      4.7375  9.735    4.7375  7.735
    center for cylinder
      69.70 0. 142.55   69.75 0. 142.5  70. 0. 142.5
    lines
      27 5 5
    boundary conditions
      -1 -1 0 3 6 0
    share
       0  0 0 3 6 0
    mappingName
      valveStemInlet
    exit
*
  exit
*
  generate an overlapping grid
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
    rightPortVolume
    valveStemInlet
      valveBottom
      valveVolume
    done
    change parameters
      prevent hole cutting
        all
        all
      done
      allow hole cutting
        cylinder
          cylinderCore
        rightPortEdge
          all
        rightPortBaseVolume
          rightPortCore
        rightPortBaseOverhangVolume
          rightPortCore
        leftPortEdge
          all
        leftPortBaseVolume
          leftPortCore
        leftPortBaseOverhangVolume
          leftPortCore
        rightPortVolume
          rightPortCore
        rightPortVolume
          rightPortEdge
*
          valveStemInlet
            all
          valveBottom
           all
          valveVolume
            all
      done
      prevent hole cutting
        rightPortEdge
          cylinder
        leftPortEdge
          cylinder
      done
      boundary conditions
        rightPortBaseVolume 
         0 0 0 0 1 0
        rightPortBaseOverhangVolume
         0 0 0 0 1 0
       done
* *****
       do not use backup rules
    exit
    * pause
    * display intermediate results
    compute overlap

    pause
    exit
*
save an overlapping grid
buildPortsPlus.hdf
buildPortsPlus
exit

    valveStemInlet
    rightPortCore
    valveBottom
    valveVolume
    done

    cylinderCore
    cylinder
    valveStemInlet
    valveBottom
    valveVolume
    rightPortEdge
    










    cylinder
    leftPortEdge
    leftPortBaseVolume
    leftPortBaseOverhangVolume
   done
    change parameters
      prevent hole cutting
        all
        all
      done
      allow hole cutting
        leftPortEdge
          all
      done
    exit
    display intermediate
    compute
    cont






    cylinder
    leftPortCore
    leftPortEdge
   done
    change parameters
      prevent hole cutting
        all
        all
      done
      allow hole cutting
        leftPortEdge
          all
      done
    exit







    cylinder
    rightPortEdge
    done

    set debug
      7


