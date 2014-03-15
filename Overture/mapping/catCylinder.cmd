  *
  *  make a hyperbolic surface grid for a valve
  *
  open a data-base
  catCylinder.hdf
  open an old file read-only
  get from the data-base
  cylinder
  hyperbolic
    set view 0 0 0 1 0.99277 -0.0465624 -0.110632 -0.0438887 0.717054 -0.695635 0.111719 0.695461 0.709826
    choose the initial curve
    create a curve from the surface
    turn off all sub-surfaces
    specify active sub-surfaces
      4
      5
     -1

    choose an edge
    mogl-select 5 
          192 863673984 863678016  204 838972928 839010304  209 838967296 839363072  
          377 838972928 839010304  384 838963840 839002112  
    mogl-pick
    curve 35 (on)
    mogl-select 5 
          192 854016512 854690688  209 845373376 846244288  218 845373120 847684480  
          379 845373120 846244160  391 845373120 846233344  
    mogl-pick
    curve 38 (on)

    done
    exit
    implicit coefficient
      0.
    curvature speed coefficient
      0.
    equidistribution weight
      0.
    grow grid in opposite direction
    lines to march
      31
    * top is between 260 and 265
    distance to march
      260
    generate
  exit
*
*   volume grid
*
  hyperbolic
    implicit coefficient
      0
    equidistribution weight
      0
    curvature speed coefficient
      0
    grow grid in opposite direction
    lines to march
      11
    distance to march
      30
    boundary conditions
    set side=0, axis=1 (bottom)
    fix y, float x and z
    exit
    generate
