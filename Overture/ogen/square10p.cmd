*
* make a simple square (periodic BC's)
*
create mappings
rectangle
  lines
    11 11
  periodicity
    1 1
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
square10p.hdf
square10p
exit
