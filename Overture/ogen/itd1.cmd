create mappings
 Annulus
  inner radius
   0.
  make 3d (toggle)
   -4.0
  mappingName
    circ0
  exit
  Annulus
   inner radius
    0.
   make 3d (toggle)
    0.0
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
    21 7 13
    boundary conditions
    -1 -1 1 1 1 0
    share
    0 0 0 1 0 0
    *exit
    *stretch coordinates
     *stretch
     *specify stretching along axis=2 (x3)
     *layers
       *1
      *1. 5. 1
     **exit
     **exit
    mappingName
      Part0
    exit
  Annulus
    inner radius
      0.
    make 3d (toggle)
      -0.5
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
      1.9
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
   change arclength spline parameters
    specify tension
     20
    exit
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
  **lines
  **41 15 21
  **exit
  **stretch coordinates
  **transform which mapping
  **part1
  **stretch
  **specify stretching along axis=2 (x3)
  **layers
  **1
  **1. 2. 0.5
  **exit
  **exit
boundary conditions
-1 -1 1 1 0 0
    share
    0 0 0 1 0 0
  mappingName 
  Part1
  exit
 Annulus
  inner radius
   0.
  outer radius
    0.9
  centre for annulus
    0. -.1
  make 3d (toggle)
   1.5
  mappingName
    circ00
  exit
  Annulus
   inner radius
    0.
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
    *lines
     *41 15 21
    boundary conditions
    -1 -1 1 1 0 1
    share
    0 0 0 1 0 0
    *exit
    *stretch coordinates
    * stretch
    * specify stretching along axis=2 (x3)
    * layers
    *   1
    *  1. 5. 0
    * exit
    * exit
    mappingName
      Part2
    exit
  Annulus
    inner radius
      0.
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
    mappingName
      vsection1
    exit
  rotate/scale/shift
    shift
      0. 0.5 0.
    mappingName
      vsection0
    exit
  rotate/scale/shift 
   transform which mapping?
     vsection1
    rotate
     -2.5. 2
     -4. 1.2 0.
    mappingName
      vsection2
    exit
  rotate/scale/shift 
   transform which mapping?
     vsection1
    rotate
     -5. 2
     -4. 1.2 0.
    mappingName
      vsection3
    exit
  rotate/scale/shift 
   transform which mapping?
     vsection1
    rotate
     -7.5. 2
     -4. 1.2 0.
    mappingName
      vsection4
    exit
  rotate/scale/shift 
   transform which mapping?
     vsection1
    rotate
     -10. 2
     -4. 1.2 0.
    mappingName
      vsection5
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
    mappingName
     Part3
    boundary conditions
    -1 -1 1 1 0 0
    lines
    21 7 20
    share
    0 0 0 2 0 0
    exit
  Annulus
    inner radius
      0.
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
      0.
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
  *lines
  *41 15 21
  mappingName
    Part4
    boundary conditions
    -1 -1 1 1 0 1
    share
    0 0 0 2 0 0
  exit
exit
generate an overlapping grid
   Part0
   Part1
   Part2
   Part3
   Part4
  done
  change parameters
    prevent hole cutting
      all
        all
    done
  exit
  compute overlap
exit
*
save an overlapping grid
itd1
pipes
exit
