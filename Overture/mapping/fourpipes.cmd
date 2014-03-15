  Annulus
    inner radius
    0.0
    make 3d (toggle)
    0.0
    mappingName
    surface1
    exit
  Circle or ellipse (3D)
    specify radius of the circle
    4.0
    specify start/end angles
    0. 0.5
    specify centre
    0. 0. 0.
    exit
  rotate/scale/shift
    rotate
    90. 0
    0. 0. 0.
    mappingName
    curve1
    exit
  Annulus
    centre for annulus
    0. 0.
    inner radius
    0.
    outer radius
    0.5
    make 3d (toggle)
    4.5
    mappingName
    surface2
    exit
  rotate/scale/shift
    rotate
    20. 1
    0. 0. 0.
    mappingName
    surface3
    exit
  rotate/scale/shift
    transform which mapping?
    surface2
    rotate
    -20. 1
    0. 0. 0.
    mappingName
    surface4
    exit
  Annulus
    inner and outer radii
      0. 0.5
    make 3d (toggle)
      8.
    mappingName
      surface20
    exit
  CrossSection
    general
      2
    surface2
    surface20
    lines
    15 6 17
    mappingName
      Pipe1
    exit
  sweep
    choose reference
    surface1
    choose sweep curve
    curve1
    lines
    33 11 63
    mappingName
      Pipe0
pause
    exit
  Circle or ellipse (3D)
    specify centre
      9.5391 0. 4.228617
    specify radius of the circle
      8.
    specify start/end angles
      0. 0.07
    exit
  rotate/scale/shift
    rotate
      154.8 2
      9.5391 0. 4.228617
    rotate
      90. 0
      9.5391 0. 4.22862
    mappingName
      curve4
    exit
  Circle or ellipse (3D)
    specify centre
      -9.5391 0. 4.22862
    specify radius of the circle
      8.
    specify start/end angles
      0. 0.07
    exit
  rotate/scale/shift
    rotate
      90. 0
      -9.5391 0. 4.228617
    mappingName
      curve3
    exit
  sweep
    choose reference
    surface3
    choose sweep curve
    curve3
    lines
    15 6 17
    mappingName
      Pipe2
pause
    exit
  sweep
    choose reference
    surface4
    choose sweep curve
    curve4
    lines
    15 6 17
    mappingName
      Pipe3
pause
    exit
