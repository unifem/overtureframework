* make a simple perioidic square
create mappings
  rectangle
    mappingName
      square
    lines
      6 6
    boundary conditions
      -1 -1 -1 -1 
  exit
exit
*
generate an overlapping grid
  square
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
*
save an overlapping grid
  square5p.hdf
  square5p
exit



*
* make a simple square (periodic BC's)
*
create mappings
rectangle
lines
6 6
boundary conditions
-1 -1 -1 -1 
period
1 1
exit
exit
*
make an overlapping grid
  square
  Done
  Specify new MappedGrid Parameters
    numberOfGhostPoints
      2 2 2 2 2 2
  Done
Done
save an overlapping grid
square5p.hdf
square5p
exit
