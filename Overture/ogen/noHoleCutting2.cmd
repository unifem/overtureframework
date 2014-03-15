*
* Test the case where hole hutting is turned off
* when it really shouldn't be.
*
create mappings
  rectangle
    specify corners
      0. 0. 1. 1.
    lines
      11 11
    mappingName
      outer-square
    share
      1 0 0 0
    exit
*
  rectangle
    specify corners
      0.0 .5 .5 1.
    lines
      8 8
    boundary conditions
      1 0 0 0
    share
      1 0 0 0
    mappingName
      inner-square
    exit
  exit
*
generate an overlapping grid
  outer-square
  inner-square
  done
  change parameters
    prevent hole cutting
      all
      all
    done
   exit
