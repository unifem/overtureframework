*
* make a box
*
create mappings
  Box
    specify corners
     0. 0. 0. 2. 1. 1. 
    lines
      41 21 3
    mappingName
      channel
  exit
exit
*
generate an overlapping grid
  channel
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
save an overlapping grid
channel3d.hdf
channel
exit




