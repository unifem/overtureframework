*
create mappings
  Box
    lines
      6 6 6
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
    order of accuracy
     fourth order
  exit
  compute overlap
exit
save an overlapping grid
nonBox5.order4.hdf
box5
exit
