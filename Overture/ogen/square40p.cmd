*
* make a simple square (periodic BC's)
*
create mappings
rectangle
lines
  41 41
boundary conditions
  -1 -1  -1  -1 
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
square40p.hdf
square40p
exit
