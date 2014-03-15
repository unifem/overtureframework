create mappings
  rectangle
    mappingName
      1
    boundary conditions
      1,0,1,0
    exit
  rectangle
    mappingName
      2
    specify corners
      0,1,1,2
    boundary conditions
      1,1,0,0
    exit
  rectangle
    mappingName
      3
    specify corners
      0,2,1,3
    boundary conditions
      1,0,0,1
    exit
  rectangle
    mappingName
      4
    specify corners
      1,2,2,3
    boundary conditions
      0,0,1,1
    exit
  rectangle
    specify corners
      2,2,3,3
    mappingName
      5
    boundary conditions
      0,1,0,1
    exit
  rectangle
    mappingName
      6
    boundary conditions
      1,1,0,0
    specify corners
      2,1,3,2
    exit
  rectangle
    specify corners
      2,0,3,1
    mappingName
      7
    boundary conditions
      0,1,1,0
    exit
  rectangle
    specify corners
      1,0,2,1
    boundary conditions
      0,0,1,1
    mappingName
      8
    exit
  exit this menu
  erase
  generate an overlapping grid
    1
    2
    3
    4
    5
    6
    7
    8
   done
    change parameters
      prevent hole cutting
        all
        all
        done
      exit

    compute overlap
