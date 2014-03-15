create mappings
*
rectangle
  set corners
    0. 1. 0. 1.
  lines
    11 11
  boundary conditions
    1 1 1 1
  share
    0 1 0 0
  mappingName
    leftSquare
  exit
*
rectangle
  set corners
    1. 2. .5 1.5
*   1. 2. 0. 1. 
  lines
    11 11
  boundary conditions
    1 1 1 1
  share
    1 0 0 0
  mappingName
    rightSquare
  exit
exit
  generate an overlapping grid
    leftSquare
    rightSquare
    done choosing mappings
    change parameters
      prevent hole cutting
       all
       all
      done
      mixed boundary
        leftSquare
          right
        rightSquare
        done
*
        rightSquare
          left
        leftSquare
        done
      done
    ghost points
      all
      2 2 2 2 2 2
  exit
  * display intermediate
  compute overlap

  *  pause
  exit
*
save an overlapping grid
twoBlock.hdf
twoBlock
exit
