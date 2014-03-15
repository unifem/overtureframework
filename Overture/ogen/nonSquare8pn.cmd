* make a square that is not considered to be rectangular in order
* to build a simple test for codes that apply different methods for
* rectangular and curvilinear grids
create mappings
  rectangle
    mappingName
      square
    set corners
      0. 1. 0. 1. -1. 1. -1. 1. 
    lines
      9 9 
    boundary conditions
      -1 -1 1 1
    mappingName
     rectangularSquare
  exit
  rotate/scale/shift
    mappingName
    square
    exit
exit
*
generate an overlapping grid
  square
  done
  change parameters
    ghost points
      all
*      3 2 2 2 2 2
      2 2 2 2 2 2 
  exit
  compute overlap
exit
*
save an overlapping grid
  nonSquare8pn.hdf
  square
exit
