#
# Make an overlapping grid for two intersecting pipes
#     cpu=2s (ov15 sun-ultra optimized)
#
create mappings
# Here is the main pipe
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
      25 21 7
    boundary conditions
      -1 -1 1 1 0 2
    share
      0 0 1 2 0 0
    exit
# Here is the core of the main pipe
#  note: there is trouble if corner of the core just
#  sticks outside the main pipe -- hole cutter
#  misses. (happens with core half width= .3). OK if the
#  core is bigger or smaller
  Box
    specify corners
      -1.5 -.25 -.25 1. .25 .25
    lines
      21 9 9
    boundary conditions
      1 1 0 0 0 0
    mappingName
      mainCore
    share
      1 2 0 0 0 0
    exit
# Here is the branch pipe
  Cylinder
    orientation
      2 0 1
    bounds on the radial variable
      .2 .4
    bounds on the axial variable
      .25 1.25
    lines
      23 11 7    21  11 7
    boundary conditions
      -1 -1 0 1 0 2
    share
      0   0 0 3 0 0
    mappingName
      branchPipe
    exit
# Here is the core of the branch pipe
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
generate an overlapping grid
  branchCore
  branchPipe
  mainCore
  mainPipe
  done
  change parameters
    prevent hole cutting
      all
      all
    done
    allow hole cutting
      branchPipe
        branchCore
      mainPipe
        mainCore
    done
    ghost points
      all
      2 2 2 2 2 2
  exit
 # pause
  compute overlap
exit
save an overlapping grid
pipes.hdf
pipes
exit
