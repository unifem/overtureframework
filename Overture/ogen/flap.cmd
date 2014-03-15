create mappings
  read plot3d file
    /home/henshaw/larc/grids/grid_3element.p3d_for
*
  change a mapping
  grid_3element.p3d_for-grid1
    boundary conditions
      0 0 1 0
   mappingName
     flap
    exit
*
  rectangle
    set corners
      -.2 1. -.5 .5 
    lines
      61 51
    mappingName
      backGround
    exit
*
*   view mappings
*     grid_3element.p3d_for-grid1
*    square
*   exit
  exit this menu
  generate an overlapping grid
    backGround
    flap
    done choosing mappings
  change parameters
    mixed boundary
      flap
      bottom (side=0,axis=1)
      flap
       r matching tolerance
         .01
       done
    done
    ghost points
      all
      2 2 2 2 2 2
  exit
  * display intermediate
  * pause
  compute overlap
  * pause
  exit
*
save an overlapping grid
flap.hdf
flap
exit
