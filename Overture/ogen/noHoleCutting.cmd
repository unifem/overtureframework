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
    exit
*
  rectangle
    specify corners
      0.01 .25 .51 .75
    lines
      6 6
    boundary conditions
      0 0 0 0
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

