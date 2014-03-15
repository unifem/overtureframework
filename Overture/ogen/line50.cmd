*
* make a line
*
create mappings
  line
    specify end points
    .0  1.
    lines
      51
    mappingName
     line1d
  exit
exit
*
generate an overlapping grid
  line1d
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
  line50.hdf
  line50
exit

