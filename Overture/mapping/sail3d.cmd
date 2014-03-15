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
    exit
  reparameterize
    equidistribution
    curvature weight
    20
    curvature weight
    10
    re-evaluate equidistribution
    re-evaluate equidistribution
    re-evaluate equidistribution
    mappingName
    sail
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
    boundary conditions for marching
      left
      match to a mapping
      sail
      right
      match to a mapping
      sail
      exit
    geometric stretching, specified ratio
    1.15
    generate
    share
    1 1 0 0
    mappingName
    mast-grid
    * pause
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
    mappingName
    sail-grid
    share
    0 0 1 0
    exit
* sweep curve:
  spline (3D)
    enter spline points
      3
      0 0 0
      0 2 0
      0 4 0
    mappingName
      sweepCurve
    exit
* scaling curve
  spline (1D)
    enter spline points
      3
      1.1
      .7
      .5
    mappingName
      scaleCurve
    exit
*
  sweep
    choose sweep surface/curve
    sail-grid
    sweepCurve
*    specify scaling factors
*      2
*      1
*      .25
    choose a scaling curve
      scaleCurve
    use center of sweep curve
    boundary conditions
      1 1 1 0 1 1
    mappingName
      sail-volume
    exit
*
*
*
  sweep
    choose sweep surface/curve
    mast-grid
    sweepCurve
*     specify scaling factors
*       2
*      1
*       .25
    choose a scaling curve
      scaleCurve
    use center of sweep curve
    boundary conditions
      1 1 1 0 1 1
    mappingName
      mast-volume
    exit
*
   view mappings
    sail-volume
    mast-volume