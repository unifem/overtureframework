create mappings
  read plot3d file
    /home/henshaw/larc/grids/grid_3element.p3d_for
*
  change a mapping
  grid_3element.p3d_for-grid0
    mappingName
      airfoil
    exit
*
  change a mapping
  grid_3element.p3d_for-grid1
    mappingName
     flap
    boundary conditions
      0 0 1 0
    exit
*
  change a mapping
  grid_3element.p3d_for-grid2
    mappingName
      backflap
    boundary conditions
      0 0 1 0
    exit
*   view mappings
*     grid_3element.p3d_for-grid1
*    square
*   exit
  exit this menu
  generate an overlapping grid
    airfoil
    flap
    backflap
    done choosing mappings
*    display intermediate results
    * set debug
    *   7
    change parameters
    mixed boundary
      flap
      bottom (side=0,axis=1)
      flap
       r matching tolerance
         .01
       done
      backflap
      bottom (side=0,axis=1)
      backflap
       r matching tolerance
         .01
       done
      airfoil
      bottom (side=0,axis=1)
      airfoil
       r matching tolerance
         .01
       done
    done
      improve quality of interpolation
      set quality bound
        1.15 1.2 1.25
    ghost points
      all
      2 2 2 2 2 2
    exit
    compute overlap
    * pause 
  exit
*
save an overlapping grid
threeElement.hdf
threeElement
exit
