* make a simple square
create mappings
  rectangle
    mappingName
      square
    lines
      11 11
    boundary conditions
      1 1 1 1
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
* save an overlapping grid
save a grid (compressed)
  square10.hdf
  square10
exit

