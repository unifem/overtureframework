*
* square in a square
*
create mappings
  rectangle
    specify corners
      -1. -1. 1. 1.
    lines
      11 11
      * 21 21
    mappingName
      outer-square
    exit
*
  rectangle
    specify corners
      -.50001 -.50001 .50001 .50001
    lines
      6 6
      * 11 11
    boundary conditions
      0 0 0 0
    mappingName
      inner-square
    exit
  exit
*
check overlap
  outer-square
  inner-square
  done
junk

