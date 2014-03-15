*
*  one square cutting into another
*
create mappings
  rectangle
    mappingName
    main-square
  exit
  rectangle
    specify corners
*     .75 .25 1.25 .75
*      .90 .25 1.25 .75
      .85 .25 1.25 .75
    mappingName
     cutting-square
    lines
      6 11
    boundary condition
      0 1 1 1
  exit
exit
generate an overlapping grid
  cutting-square
  main-square
  done
  change parameters
    prevent hole cutting
      all
      all
      done
    *cell centering
    *  cell centered for all grids
  exit
  * pause
  compute overlap
  exit
*
save an overlapping grid
cutSquares.hdf
cutSquares
exit

    