*
* make a stretched square
*
create mappings
  rectangle
  exit
  stretch coordinates
    mappingName
      stretched-square
    stretch
    specify stretching along axis=0
      layers
      1
      1. 10. .0
     exit
    exit
  exit
exit this menu
generate an overlapping grid
  stretched-square
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
  stretchedSquare.hdf
  stretchedSquare
exit
