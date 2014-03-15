create mappings
  read plot3d file
    /home/henshaw/larc/grids/grid_3element.p3d_for
*
  change a mapping
  grid_3element.p3d_for-grid2
    c-grid
      determine c-grid automatically    
    done
    boundary conditions
      0 0 1 0
*   pause
    exit
*
  rectangle
    set corners
      .75  2.5  -.5 .5 
    lines
      75 52 201 151  71 45  75 52   
    exit
*
*   view mappings
*     grid_3element.p3d_for-grid1
*    square
*   exit
  exit this menu
  generate an overlapping grid
    square
    grid_3element.p3d_for-grid2
    done choosing mappings
    * display intermediate results
    * set debug
    *  7
    compute overlap
