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
      2.0
    mappingName
      circle3
    exit
  CrossSection
    general
      4
    circ0
    circle1
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
 exit
*
reparameterize
  mappingName
   leftPipe
  restrict parameter space
      specify corners
        0 0 0 1. 1. .5
    exit
  boundary conditions
    -1 -1 1 2 3 0
  share
     0 0  0 1  0 0    
 exit
reparameterize
  mappingName
   rightPipe
  restrict parameter space
   specify corners
        0 0 .5 1. 1. 1.
    exit
  boundary conditions
    -1 -1 1 2 0 4
  share
     0 0  0 1  0 0    
exit
  exit this menu
  generate an overlapping grid
    leftPipe
    rightPipe
    done choosing mappings
