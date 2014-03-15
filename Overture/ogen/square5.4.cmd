* make a simple square, fourth order
create mappings
  rectangle
    mappingName
      square
    lines
      6 6
    boundary conditions
      1 1 1 1
  exit
exit
*
generate an overlapping grid
  square
  done
  change parameters
    order of accuracy
      fourth order
    ghost points
      all
      2 2 2 3
  exit
  compute overlap
exit
*
save an overlapping grid
  square5.4.hdf
  square5
exit
