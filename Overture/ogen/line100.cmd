*
* make a line
*
create mappings
  line
    specify end points
    .0  1.
    lines
      101
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
  line100.hdf
  line100
exit






*
* make a line
*
create mappings
  line
    specify end points
    .0  1.
    lines
      101
    mappingName
     line1d
  exit
exit
make an overlapping grid
    line1d
  Done
  Specify new MappedGrid Parameters
    numberOfGhostPoints
      2 2 2 2 2 2
  Done
Done
save an overlapping grid
line100.hdf
line
exit
