* here is the mast
  spline
    mappingName
      mast-surface
    enter spline points
      9
      0    0.
      -.05 -.2
      -.25 -.25
      -.45 -.2
      -.5 0
      -.45 .2
      -.25 .25
      -.05 .2
      0. .0
    periodicity
      2
    lines
      61
    exit
*
* create the mast grid
*
  hyperbolic
    mappingName
      mast
    distance to march
      1.
    grow grid in opposite direction
    lines to march 
      21
    generate
    break
    exit
