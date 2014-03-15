create mappings
  rectangle
    set corners
      -1. 20 -5 5
    lines
      101 51
    boundary conditions
      0 2 0 3
    share
      0 2 0 4
    mappingName
    background
    exit
* 
  smoothedPolygon
    vertices
      10
      -5. 0.
      -4. 0.
      -3. 0.
      -2. 0.
      -1. 0.
      0 0
      5 -5
      6 -5
      8 -5
      20 -5
    n-dist
    fixed normal distance
      1.
    t-stretch
     0 50
     0 50
     0 50
     0 50
     0 50
     0 50
     0 50
     0 50
     0 50
     0 50
    n-stretch
     0 4 0
    lines
      137 6
    boundary conditions
      0 2 3 0
    share
      0 2 3 0
    mappingName
      longRamp
    exit
* 
*  Make a curve for the ramp boundary
  smoothedPolygon
    vertices
      10
      -5. 0.
      -4. 0.
      -3. 0.
      -2. 0.
      -1. 0.
      0 0
      5 -5
      6 -5
      8 -5
      20 -5
    t-stretch
     0 50
     0 50
     0 50
     0 50
     0 50
     0 50
     0 50
     0 50
     0 50
     0 50
    curve or area (toggle)
    lines
      137 
    mappingName
      rampCurve
    exit
*   make the bottom curve for the inlet tfi
  reparameterize
    restrict parameter space
      set corners
      0 .148 
      exit
    mappingName
      rampCurveInlet
    exit
*  make the top curve for the inlet tfi
  line (2D)
    number of dimensions
      2
    set end points
      -5. -1.  5. 5.
    exit
*
* here is the inlet grid
  tfi
    choose bottom curve (r_2=0)
      rampCurveInlet
    choose top curve    (r_2=1)
      line
    lines
      22 26
    boundary conditions
      1 0 2 3
    share 
      0 0 3 4
     mappingName
      inlet
    exit
*
* here is the shortened ramp without the inlet section
  reparameterize
    transform which mapping?
      longRamp
    set corners
      .148  1. 0. 1.
    mappingName
      ramp
* pause
    exit
  exit this menu
*
generate an overlapping grid
  background
  ramp
  inlet
  done
* pause
  change parameters
    ghost points
      all
      2 2 2 2
    exit
  compute overlap
  pause
  exit
save a grid
detRamp.hdf
detRamp
exit
