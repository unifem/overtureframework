*
* 3D 'mast and sail' from a sweep mapping
*
create mappings
*
Box
  specify corners
    -2. 0. -1.5  4. 4. 1.5 
  lines
    61 31 31
  boundary conditions
    1 1 1 1 1 1
  share
    0 0 2 3 0 0 
  mappingName
  backGround
 exit
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
      lines 
        51
    exit
*
* create the mast grid
*
  hyperbolic
    distance to march
      .5 .4
    grow grid in opposite direction
    lines to march 
      9
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
* stretch the mast grid
*
  stretch coordinates
    transform which mapping?
      mast-grid
    mappingName
      stretched-mast-grid
    stretch
      specify stretching along axis=0
* choose a layer stretching a*tanh(b*(r-c))     
        layers
          2
*         give a,b,c in above formula
          .25 20. 0.
          .25 20. 1.
        exit
      exit
    * pause
  exit
*
* create the sail grid
*
  hyperbolic
    start from which curve/surface?
    sail
    distance to march
      .6 .5
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
  *
* make the sweep curve
  spline (3D)
    enter spline points
      3
      0 0 0
      0 2 0
      0 4 0
    mappingName
      sweepCurve
    exit
*
* scaling curve
*
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
    choose reference mapping
      sail-grid
    choose sweep curve
      sweepCurve
    choose a scaling curve
      scaleCurve
    use center of sweep curve
    boundary conditions
      0 0 1 0 1 1
    share
      0 0 1 0 2 3
    mappingName
      sail-volume
    exit
*
*
*
  sweep
    choose reference mapping
      stretched-mast-grid
    choose sweep curve
      sweepCurve
    choose a scaling curve
      scaleCurve
    use center of sweep curve
    boundary conditions
      1 1 1 0 1 1
    share
      1 1 0 0 2 3
    mappingName
      mast-volume
    exit
*
   DataPointMapping
     build from a mapping
       sail-volume
     mappingName
       sail-volume-dp
  exit
*
   DataPointMapping
     build from a mapping
       mast-volume
     mappingName
       mast-volume-dp
  exit
exit
generate an overlapping grid
    backGround
    sail-volume-dp
    mast-volume-dp
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
*  pause
  compute overlap
  pause
  exit
*
save an overlapping grid
mastSail3d.hdf
mastSail3d
exit
