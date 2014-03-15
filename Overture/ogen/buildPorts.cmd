*
* build the grid around the upper part of the cylinder
* where thw two ports enter the cylinder.
*
create mappings
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
    specify corners
       51 -5 124  88 5.25  161   51 -5 124  88 5.25  167   51 -5 124  88 5.25  161   52 -5 124  89 5.25  161
    lines
      19 7 19   19 7 23
   mappingName
     rightPortCore
   boundary conditions
     0 0 0 3 0 0
   share
     0 0 0 3 0 0
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
      done
      prevent hole cutting
        rightPortEdge
          cylinder
        leftPortEdge
          cylinder
      done
    exit
    * display intermediate results
    compute overlap
    pause
    exit
*
save an overlapping grid
buildPorts.hdf
buildPorts
exit


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


