create mappings
  rectangle
    specify corners
      -.25 -.25 .75 .75
    lines
      21 41
    exit
  stretch coordinates
    stretch
      specify stretching along axis=1 (x2)
        layers
          2
          1. 20. 0.
          1. 20. 1.
        exit
      exit
    exit
  exit this menu
  generate an overlapping grid
    stretched-square
    done choosing mappings
  change parameters
    ghost points
      all
      2 2 2 2 2 2
    exit
  compute overlap
  exit
*
save an overlapping grid
stretch3.hdf
stretch3
exit
