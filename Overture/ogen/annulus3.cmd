*
*  Simple annulus 
*
create mappings
*
annulus
  lines
    161 21  81 11 9 41 6 
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
  exit
*
save an overlapping grid
annulus3.hdf
annulus3
exit











*
*  Simple annulus 
*
create mappings
*
annulus
  lines
    81 17  41 9 
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
  exit
*
save an overlapping grid
annulus3.hdf
annulus3
exit

