  Annulus
    inner radius
    0.
    centre for annulus
    1 0
    make 3d (toggle)
    -1
    exit
  rotate/scale/shift
    rotate
    -90. 0
    1. 0. -1.
    mappingName
    surface
    exit
  spline (3D)
    enter spline points
    8
    1. 0. -1.
    1. 4. -1.
    1. 8. -1.
    *     0. 10.44949 0.
    *     -1. 10.828427 1.
    *     -2 10.44949 2.
    *     -3. 8. 3
    -1 10.44949 0.
    -2.25 10.828427 1.
    -3.25 10.44949 2.
    -4.25 8 3
    -3.25  6.0635083 3.5
    mappingName
    curve1
    exit
  sweep
    specify scaling factors
    7
    1.
    1.
    1.
    1.125
    1.25
    1.25
    1.25
    choose reference
      surface
    choose sweep curve
      curve1
    lines 
    21 11 91
    mappingName
      Part1
pause
    exit
  spline (3D)
    enter spline points
      4
      -1.75 10.5 0.25
      -1.25 11.  0.0
      -.5 12.75 -0.125
      -.5 13.75 -0.125
    mappingName
      curve2
    exit
  Annulus
    centre for annulus
      -1. 10.44949
    inner radius
      0.
    outer radius
      0.5
    make 3d (toggle)
      0.
    exit
  rotate/scale/shift
    rotate
      -90 0
      -1.25 9.75 0.
    mappingName
      surface2
    exit
  sweep
    specify scaling factors
      4
      1.5
      1.35
      1.
      1.
    choose reference
      surface2
    choose sweep
      curve2
    lines 
    21 11 15
    mappingName
      Part2
    pause
    exit
  spline (3D)
    enter spline points
      4
      -2.75 11.25 1.05
    -2.5 11.5 0.9
     -2. 12.5 0.6
    -2.  13.75 0.6
    mappingName
      curve3
    exit
  Annulus
    centre for annulus
      -2.25 9.95
    inner radius
      0.
    outer radius
      0.4
    make 3d (toggle)
      0.75
    exit
  rotate/scale/shift
    rotate
      -90 0
      -2.25 9.95 0.75
    mappingName
      surface3
    exit
  sweep
    specify scaling factors
      4
      1.5
      1.35
      1.
      1.
    choose reference
      surface3
    choose sweep
      curve3
    lines 
    21 11 15
    mappingName
      Part3
pause
    exit
  spline (3D)
    enter spline points
      4
*      -3.25 9.75 1.75
*    -3.25 10.44949 2.
**     0.5 12.5 -0.75
*    -4.25  14.25 3
    -3.25 10.44949 2.
    -3.5  11.69949 2.
    -4.5  12.5 2.25
    -5.5  13. 2.5
    mappingName
      curve4
    exit
  Annulus
    centre for annulus
      -3.25 9.75
    inner radius
      0.
    outer radius
      0.7
    make 3d (toggle)
      1.75
    exit
  rotate/scale/shift
    rotate
      -90 0
      -3.25 9.75 1.75
    mappingName
      surface4
    exit
  sweep
    specify scaling factors
      3
      1.125
      1.0625
      1
    choose reference
    surface4
    choose sweep curve
    curve4
    lines 
    21 11 15
    mappingName
      Part4
pause
    exit
