  * Here is the sail
  spline
    enter spline points
      10
      0   -.05
      .5  -.05
      1.  -.05
      1.5 -.05
      2.  -.05
      2.   .05
      1.5  .05
      1.   .05
      .5   .05
      0.   .05
    shape preserving (toggle)
    lines
      41
    * pause
    exit
  reparameterize
    equidistribution
    curvature weight
      20
    curvature weight
      10
* pause
    re-evaluate equidistribution
    re-evaluate equidistribution
    re-evaluate equidistribution
    mappingName
      sail 
    * pause
    exit
* here is the mast
  spline
    mappingName
      mast
    enter spline points
      9
      0    -.05
      -.05 -.2
      -.25 -.25
      -.45 -.2
      -.5   .0
      -.45  .2
      -.25  .25
      -.05  .2
      0.    .05
     * pause
    exit
*
* create the mast grid
*
  hyperbolic
    distance to march
      1.
    grow grid in opposite direction
    lines to march 
      11
*
    BC: left match to a mapping
      sail
    BC: right match to a mapping
      sail
    geometric stretching, specified ratio
      1.15
debug
 3
    generate
    pause
    share
      1 1 0 0
    mappingName
      mast-grid
    exit
*
* create the sail grid
*
  hyperbolic
    start from which curve/surface?
    sail
    distance to march
      .5
    lines to march
      11
    geometric stretching, specified ratio
      1.15
    generate

pause
    mappingName
      sail-grid
    share
      0 0 1 0
    exit
*
 view mappings
  mast-grid
  sail-grid