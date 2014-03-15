* make a simple square
create mappings
  rectangle
    mappingName
      square
    lines
      129 129  65 65 
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
        3 2 2 2 2 2
    order of accuracy
     fourth order
  exit
  compute overlap
exit
*
save an overlapping grid
  square128.order4.hdf
  square128.order4
exit
