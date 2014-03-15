*
* make a simple square (periodic BC's)
*
create mappings
rectangle
lines
21 21
boundary conditions
  1 1  -1 -1 
*  -1 -1 -1 -1 
period
 0 1
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
square20p.hdf
square20p
exit
