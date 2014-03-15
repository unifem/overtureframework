* make a simple square
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
    * make the grid cell-centered
    cell centering
      cell centered for all grids
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
  * pause
exit
*
save an overlapping grid
  square5CC.hdf
  square5CC
exit

