create mappings
  Cylinder
    orientation
      1 2 0
    bounds on the radial variable
      .25 .5
    bounds on the axial variable
      -1.5 1.
    mappingName
      mainPipe
    lines
      21 21 7
    boundary conditions
      -1 -1 1 1 0 2
    share
      0 0 1 2 0 0
    exit
  Box
    specify corners
      -1.5 -.3 -.3 1. .3 .3
    lines
      21 9 9
    boundary conditions
      1 1 0 0 0 0
    mappingName
      mainCore
    share
      1 2 0 0 0 0
    exit
  Cylinder
    orientation
      2 0 1
    bounds on the radial variable
      .2 .4
    bounds on the axial variable
      .25 1.25
    lines
      21  11 7
    boundary conditions
      -1 -1 0 1 0 2
    share
      0   0 0 3 0 0
    mappingName
      branchPipe
    exit
  Box
    specify corners
      -.25 .25 -.25 .25 1.25 .25
    lines
      9 15 9
    boundary conditions
      0 0 0 1 0 0
    share
      0 0 0 3 0 0
    mappingName
      branchCore
    exit
  exit
check overlap
  branchPipe
  branchCore
  mainPipe
  mainCore
  done
  change parameters
    prevent hole cutting
      all
      all
    done
  exit
