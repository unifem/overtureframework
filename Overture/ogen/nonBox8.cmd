*
create mappings
  Box
    lines
      9 9 9
    mappingName
      box-analytic
  exit
  rotate/scale/shift
    mappingName
    box
  exit
exit
*
generate an overlapping grid
  box
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
* save an overlapping grid
save a grid (compressed)
nonBox8.hdf
box8
exit
