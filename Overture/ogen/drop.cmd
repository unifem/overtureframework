*
* drop in a channel
*
create mappings
*
rectangle
  set corners
    -1. 1. -1. 1.  -1. 1. -3. 1. 
  lines
    21 21 21 41 
  boundary conditions
    1 1 1 1
  mappingName
   channel
exit
*
Annulus
  lines
    33 5
  inner and outer radii
    .3 .7
  centre for annulus
    0. 0.  .25 .25
  boundary conditions
    -1 -1 1 0
  mappingName
   drop
exit
*
exit
generate an overlapping grid
    channel
    drop
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
*  display intermediate results
  compute overlap
* pause
  exit
*
save an overlapping grid
drop.hdf
drop
exit

