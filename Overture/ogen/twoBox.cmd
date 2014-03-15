*
* two boxes side by side
*
create mappings
Box
  specify corners
    0. 0. 0. .7 1. 1. 
  lines
  7 11 11
  boundary conditions
  1 0 1 1 1 
  mappingName
  box1
exit
Box
  specify corners
    .3 0. 0. 1. 1. 1.
  lines
    7 11 11
  boundary conditions
    0 2 2 2 2 2
  mappingName
  box2
exit
* pause
exit
generate an overlapping grid
  box1
  box2
  done
  change parameters
    interpolation type
     explicit for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
save an overlapping grid
twoBox.hdf
box
exit

