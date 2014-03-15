*
* two co-centric annulus's to test hole cutting
*
create mappings
*
rectangle
  set corners
    -1. 1. -1. 1.
  lines
    5 5 7 7 8 8 
  boundary conditions
    1 1 1 1
  mappingName
  square
exit
*
Annulus
  inner radius
    .25
  outer radius
    .35
  lines
    25 3
  boundary conditions
    -1 -1 1 0
  mappingName
   annulus
exit
*
Annulus
  inner radius
    .35
  outer radius
    .7
  lines
    25 7
  boundary conditions
    -1 -1 0 0
  mappingName
   refinement
exit
*
exit
generate an overlapping grid
    square
    annulus
    refinement
  done
  display intermediate results
  compute overlap
junk

pause
  exit
*
save an overlapping grid
cic.hdf
cic
exit

