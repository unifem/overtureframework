*
* make a c-grid
*
create mappings
*
  spline
    enter spline points
      15
      1.    1.e-4
      .9    1.e-4
      .8    1.e-4
      .7    1.e-4
      .6    1.e-4
      .5    1.e-4
      .25   -.1
*      .005  -.03
      0.    0.
*      .005 +.03
      .25  +.1
      .5    -1.e-4
      .6    -1.e-4
      .7    -1.e-4
      .8    -1.e-4
      .9    -1.e-4
      1.    -1.e-4
    shape preserving (toggle)
    lines 
     51
    curvature weight
     2.
    mappingName
      c-surface
    exit
*
  hyperbolic
    distance to march
      .25
    uniform dissipation coefficient
      .02
    grow grid in opposite direction
    geometric stretching, specified ratio
      1.1
    generate
    mappingName
      c-grid
    * use robust inverse
    boundary conditions
      0 0 1 0
    share
      0 0 1 0
    exit
*
  rectangle
*
    mappingName
      backGround
    set corners
      -.5 1.5 -.5 .5
    lines
      41 21
    exit
*
exit
generate an overlapping grid
    backGround
     c-grid
  done
  * display intermediate results
  change parameters
    mixed boundary
      c-grid
      bottom (side=0,axis=1)
      c-grid
        specify mixed boundary points
        0 15 -1 -1
        done
      c-grid
      bottom (side=0,axis=1)
      c-grid
        specify mixed boundary points
        35 50 0 0
        done
      done
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
  exit
*
save an overlapping grid
cgrid.manual.hdf
cgrid
exit


