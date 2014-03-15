*
* make a line
*
create mappings
  line
    specify end points
    .0  1.
    lines
      11
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
pause
  compute overlap
exit
*
save an overlapping grid
  line10.hdf
  line10
exit

