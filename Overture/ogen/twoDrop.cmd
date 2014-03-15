*
* two drops in a channel
*
create mappings
*
rectangle
  set corners
    -1. 1. -6. 1. 
  lines
    21 71 
  boundary conditions
    1 1 1 1
  mappingName
   channel
exit
*
Annulus
  lines
    33 6
  inner and outer radii
    .3 .6
  centre for annulus
    -.25 -.75
  boundary conditions
    -1 -1 1 0
  mappingName
   drop
exit
*
*
Annulus
  lines
    33 6
  inner and outer radii
    .3 .6
  centre for annulus
    .25 .25     -> trouble: .6 .25 
  boundary conditions
    -1 -1 1 0
  mappingName
   drop2
exit
*
exit
generate an overlapping grid
    channel
    drop
    drop2
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
  compute overlap
*  pause
  exit
*
save an overlapping grid
twoDrop.hdf
twoDrop
exit

