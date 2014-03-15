* make a simple square
create mappings
  rectangle
    mappingName
      square
    lines
      7 7 
    boundary conditions
      1 1 1 1
  exit
exit
*
generate an overlapping grid
  specify 
    2
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
  sqmg.hdf
  sqmg
exit
