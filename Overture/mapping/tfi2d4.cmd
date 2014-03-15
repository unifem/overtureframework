  spline
    enter spline points
      3
      -1.0 -1.0
      0.0 -.5
      1.0 -1.0
    lines
      21
    mappingName
      bottomSpline
    exit
  spline
    enter spline points
      3
      -1.5 .5
      0.0 1.0
      1.5 1.0
    lines
      21
    mappingName
      topSpline
    exit
  spline
    enter spline points
      3
      -1.0 -1.0
      -1.0 -.25
      -1.5 .5
    lines
      21
    mappingName
      leftSpline
    exit
  spline
    enter spline points
      3
      1.0 -1.0
      1.25 -.25
      1.5 1.0
    lines
      21
    mappingName
      rightSpline
    exit
  tfi
    mappingName
      tfi2d4
    choose bottom
      bottomSpline
    choose top
      topSpline
    choose left
      leftSpline
    choose right
      rightSpline
    pause
    exit
  check
    tfi2d4
