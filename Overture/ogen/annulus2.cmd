*
*  Simple annulus 
*
create mappings
*
annulus
  lines
    81 11 9 41 6 
  boundary conditions
    -1 -1 1 2
exit
*
exit
generate an overlapping grid
    Annulus
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
* pause
  exit
*
save an overlapping grid
annulus2.hdf
annulus2
exit

