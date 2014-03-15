  Annulus
    inner radius
      0.
    make 3d (toggle)
      -0.6
    mappingName
      circle0
    exit
  Annulus
    inner radius
      0.
    make 3d (toggle)
      0.
    mappingName
      circle1
    exit
  Annulus
    inner radius
      0.
    outer radius
      0.9
    centre for annulus
      0. -.1
    make 3d (toggle)
      1.4
    mappingName
      circle2
    exit
  Annulus
    inner radius
      0.
    outer radius
      0.9
    centre for annulus
      0. -.1
    make 3d (toggle)
      1.6
    mappingName
      circle3
    exit
  CrossSection
    general
      4
    circle0
    circle1
    circle2
    circle3
  mappingName 
    Middle
 pause
    exit
  rotate/scale/shift
  transform which mapping
    Middle
  rotate
  90. 1
  0. 0. 0.
  mappingName
    part1
  lines
  41 15 21
  exit
  stretch coordinates
  transform which mapping
  part1
  stretch
  specify stretching along axis=2 (x3)
  layers
  2
  1. 5. 0.0
  1. 5. 1.0
  exit
  exit
boundary conditions
-1 -1 1 1 0 1
    share
    0 0 0 1 0 0
  mappingName 
  Part1
  exit
