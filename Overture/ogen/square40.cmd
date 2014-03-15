* make a simple square
create mappings
  rectangle
    mappingName
      square
    lines
      41 41
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
*     1 1 1 1 1 1
  exit
  compute overlap
exit
*
save an overlapping grid
  square40.hdf
  square40
exit

