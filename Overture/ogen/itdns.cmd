create mappings
 Annulus
  inner radius
   0.3
  make 3d (toggle)
   -4.0
  mappingName
    circ0
  exit
  Annulus
   inner radius
    0.3
   make 3d (toggle)
    -0.0
   mappingName
    circ1
   exit
   CrossSection
    general
      2
    circ0
    circ1
    mappingName
     part0
    exit
   rotate/scale/shift
    transform which mapping
    part0
     rotate
     90. 1
     0. 0. 0.
    lines
     21 14 23
    boundary conditions
    -1 -1 0 1 1 0
    share
    0 0 0 1 3 4
    exit
    stretch coordinates
     stretch
     specify stretching along axis=2 (x3)
     layers
       1
      1. 4. 1
     exit
     specify stretching along axis=1 (x2)
     layers
       1
      1. 4. 1
     exit
     exit
    mappingName
      Part0
    exit
  rectangle
    specify corners
    -0.35 -0.35 0.35 0.35
    make 3d (toggle)
     -4.0
    mappingName
      rect0
      exit
  rectangle
     specify corners 
     -0.35 -0.35 0.35 0.35
     make 3d (toggle)
     0.0
     mappingName
      rect1
      exit
  CrossSection
    general
      2
    rect0
    rect1
    exit
  rotate/scale/shift
    rotate
    90. 1
    0. 0. 0.
    lines
    10 10 23
    boundary conditions
    0 0 0 0 1 0
    share
    0 0 0 0 3 4
     exit
    stretch coordinates
     stretch
      specify stretching along axis=2 (x3)
      layers
        1 
      1. 4. 1
     exit
     exit
     mappingName
     Part00
     exit
  Annulus
    inner radius
      0.3
    make 3d (toggle)
      -0.2
    mappingName
      circle0
    exit
  Annulus
    inner radius
      0.3
    make 3d (toggle)
      0.
    mappingName
      circle1
    exit
  Annulus
    inner radius
      0.3
    outer radius
      0.975
    centre for annulus
      0. -.025
    make 3d (toggle)
      0.35
    mappingName
      circle10
    exit
  Annulus
    inner radius
      0.3
    outer radius
      0.95
    centre for annulus
      0. -.05
    make 3d (toggle)
      0.7
    mappingName
      circle11
    exit
  Annulus
    inner radius
      0.3
    outer radius
      0.925
    centre for annulus
      0. -.075
    make 3d (toggle)
      1.05
    mappingName
      circle12
    exit
  Annulus
    inner radius
      0.3
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
      0.3
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
      7
    circle0
    circle1
    circle10
    circle11
    circle12
    circle2
    circle3
  mappingName 
    Middle
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
  21 14 25
  exit
  stretch coordinates
  transform which mapping
  part1
  stretch
  specify stretching along axis=2 (x3)
  layers
  2
  1. 4. 0.0
  1. 4. 1.0
  exit
  specify stretching along axis=1 (x2)
  layers
  1
  1. 4. 1.0
  exit
  exit
boundary conditions
-1 -1 0 1 0 0
    share
    0 0 0 1 5 6
  mappingName 
  Part1
  exit
 rectangle
  specify corners
  -0.35 -0.35 0.35 0.35
  make 3d (toggle)
  -0.2
  mappingName
  rectangle0
  exit
 rectangle
  specify corners
  -0.35 -0.35 0.35 0.35
  make 3d (toggle)
  0.0
  mappingName
  rectangle1
  exit
 rectangle
  specify corners
  -0.35 -0.375 0.35 0.325
  make 3d (toggle)
   0.35
  mappingName
  rectangle10
  exit
 rectangle
  specify corners
  -0.35 -0.4 0.35 0.3
  make 3d (toggle)
   0.7
  mappingName
  rectangle11
  exit
 rectangle
  specify corners
  -0.35 -0.425 0.35 0.275
  make 3d (toggle)
   1.05
  mappingName
  rectangle12
  exit
 rectangle
  specify corners
  -0.35 -0.45 0.35 0.25
  make 3d (toggle)
   1.4
  mappingName
   rectangle2
   exit
 rectangle
  specify corners
  -0.35 -0.45 0.35 0.25
  make 3d (toggle)
   1.6
  mappingName
   rectangle3
   exit
 CrossSection
  general
    7
  rectangle0
  rectangle1
  rectangle10
  rectangle11
  rectangle12
  rectangle2
  rectangle3
   exit
 rotate/scale/shift
 rotate
 90. 1
 0. 0. 0.
 lines
 10 10 25
 exit
 stretch coordinates
   stretch
   specify stretching along axis=2 (x3)
   layers
     2
    1. 4. 0.0
    1. 4. 1.0
   exit
   exit
 boundary conditions
 0 0 0 0 0 0
 share
 0 0 0 0 5 6
 mappingName
  Part10
 exit
 Annulus
  inner radius
   0.3
  outer radius
    0.9
  centre for annulus
    0. -.1
  make 3d (toggle)
   1.4
  mappingName
    circ00
  exit
  Annulus
   inner radius
    0.3
   outer radius
    0.9
   centre for annulus
    0. -.1
   make 3d (toggle)
    4.4
   mappingName
    circ10
   exit
   CrossSection
    general
      2
    circ00
    circ10
    mappingName
     part2
    exit
   rotate/scale/shift
    transform which mapping
    part2
     rotate
     90. 1
     0. 0. 0.
    lines
     21 14 21
    boundary conditions
    -1 -1 0 1 0 1
    share
    0 0 0 1 7 8
    exit
    stretch coordinates
     stretch
     specify stretching along axis=2 (x3)
     layers
       1
      1. 4. 0
     exit
     specify stretching along axis=1 (x2)
     layers
       1
      1. 4. 1
     exit
     exit
    mappingName
      Part2
    exit
  rectangle
    specify corners
    -0.35 -0.45 0.35 0.25
    make 3d (toggle)
    1.4
    mappingName
    rect00
    exit
  rectangle
    specify corners
    -0.35 -0.45 0.35 0.25 
    make 3d (toggle)
    4.4
    mappingName
    rect01
    exit
  CrossSection
    general
     2
    rect00
    rect01
    exit
  rotate/scale/shift
    rotate
    90. 1
    0. 0. 0.
    lines
    10 10 21
    boundary conditions
    0 0 0 0 0 1
    share
    0 0 0 0 7 8
    exit
    stretch coordinates
     stretch
     specify stretching along axis=2 (x3)
     layers
     1
     1. 4. 0.
     exit
     exit
     mappingName
     Part20
     exit
  Annulus
    inner radius
      0.3
    outer radius
      0.7
    centre for annulus
      0.7 1.2
    make 3d (toggle)
      0.
    exit
  rotate/scale/shift
    rotate
      -90. 0
      0.7 1.2 0.
    rotate
      -90. 1.
       0.7 1.2 0.
    mappingName
      vsection4
    exit
  rotate/scale/shift
    shift
      0. 0.2 0.
    mappingName
      vsection5
    exit
  rotate/scale/shift 
   transform which mapping?
     vsection4
    rotate
     -2.5 2
     -4. 1.2 0.
    mappingName
      vsection3
    exit
  rotate/scale/shift 
   transform which mapping?
     vsection4
    rotate
     -5. 2
     -4. 1.2 0.
    mappingName
      vsection2
    exit
  rotate/scale/shift 
   transform which mapping?
     vsection4
    rotate
     -7.5 2
     -4. 1.2 0.
    mappingName
      vsection1
    exit
  rotate/scale/shift 
   transform which mapping?
     vsection4
    rotate
     -10. 2
     -4. 1.2 0.
    mappingName
      vsection0
    exit
  CrossSection
    general
    6
    vsection0
    vsection1
    vsection2
    vsection3
    vsection4
    vsection5
    cubic
    lines
    21 14 27
    exit
    stretch coordinates
     stretch
     specify stretching along axis=2 (x3)
     layers
       1
      1. 4. 0.5
     exit
     specify stretching along axis=1 (x2)
     layers
       1
      1. 4. 1
     exit
     exit
    mappingName
     Part3
    boundary conditions
    -1 -1 0 1 0 0
    share
    0 0 0 9 10 11
    exit
  rectangle
    specify corners
    0.35 0.85 1.05 1.55
    make 3d (toggle)
    0.
    exit
  rotate/scale/shift
    rotate
     -90. 0
     0.7 1.2 0.
    rotate
      -90. 1.
       0.7 1.2 0.
    mappingName
    vrect4
    exit
  rotate/scale/shift
    shift
    0. 0.2 0.
    mappingName
    vrect5
    exit
  rotate/scale/shift
    transform which mapping?
    vrect4
    rotate
    -2.5 2
    -4. 1.2 0.
    mappingName
    vrect3
    exit
  rotate/scale/shift
    transform which mapping
    vrect4
    rotate
    -5. 2
    -4. 1.2 0.
    mappingName
     vrect2
     exit
  rotate/scale/shift
    transform which mapping
    vrect4
    rotate
    -7.5 2
    -4. 1.2 0.
    mappingName
    vrect1
    exit
  rotate/scale/shift
    transform which mapping
    vrect4
    rotate
    -10. 2
    -4. 1.2 0.
    mappingName
    vrect0
    exit
  CrossSection
    general
    6 
    vrect0
    vrect1
    vrect2
    vrect3
    vrect4
    vrect5
    cubic
    lines
    10 10 27
    exit
    stretch coordinates
     stretch
     specify stretching along axis=2 (x3)
     layers
       1
      1. 4. 0.5
     exit
    exit
    mappingName
    Part30
    boundary conditions
    0 0 0 0 0 0
    share
    0 0 0 0 10 11
    exit
  Annulus
    inner radius
      0.3
    outer radius
      0.7
    centre for annulus
      0.7 1.2
    make 3d (toggle)
      0.
    mappingName
      ann0
    exit
  Annulus
    centre for annulus
      0.7 1.2
    inner radius
      0.3
    outer radius
      0.7
    make 3d (toggle)
      3.
    mappingName
      ann1
    exit
  CrossSection
    general
      2
    ann0
    ann1
    exit
  rotate/scale/shift
    rotate
      -90. 0
      0.7 1.2 0.
    rotate
      -90. 1.
       0.7 1.2 0.
  lines
  21 14 25
   exit
    stretch coordinates
     stretch
     specify stretching along axis=2 (x3)
     layers
       1
      1. 2. 0.
     exit
     specify stretching along axis=1 (x2)
     layers
       1
      1. 4. 1
     exit
     exit
  mappingName
    Part4
    boundary conditions
    -1 -1 0 1 0 1
    share
    0 0 0 9 12 13
  exit
 rectangle
  specify corners
  0.35 0.85 1.05 1.55
  make 3d (toggle)
  0.
  mappingName
  rectang0
  exit
 rectangle
  specify corners
  0.35 0.85 1.05 1.55 
  make 3d (toggle) 
  3.
  mappingName
  rectang1
  exit
 CrossSection
  general
    2
  rectang0
  rectang1
   exit
  rotate/scale/shift
    rotate
      -90. 0
      0.7 1.2 0.
    rotate
      -90. 1.
       0.7 1.2 0.
  lines
  10 10 25
   exit
    stretch coordinates
     stretch
     specify stretching along axis=2 (x3)
     layers
       1
      1. 2. 0.
     exit
     exit
  mappingName
    Part40
    boundary conditions
    0 0 0 0 0 1
    share
    0 0 0 0 12 13
  exit
  exit this menu
  generate an overlapping grid
  Part0
  Part00
  Part1
  Part10
  Part2
  Part20
  Part3
  Part30
  Part4
  Part40
  done choosing mappings
  change parameters
   prevent hole cutting
   all
   all
  done
  allow hole cutting
   Part0
    Part00
   Part1
    Part10
   Part2
    Part20
   Part3
    Part30
   Part4
    Part40
  done
*  maximize overlap
*  do not interpolate ghost
  exit
  compute overlap
 exit
 save an overlapping grid
 itd1.hdf
 Crossing pipes
 exit
