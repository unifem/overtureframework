create mappings
  Annulus
    inner radius
      0.
    * outer radius
    *   1.4
    make 3d (toggle)
      -1.5
    mappingName
      circleA
    exit
  Annulus
    inner radius
      0.
    * outer radius
    *   1.4
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
      2.0
    mappingName
      circle3
    exit
  Annulus
    inner radius
      0.
    outer radius
      0.9
    centre for annulus
      0. -.1
    make 3d (toggle)
      3.0
    mappingName
      circleB
    exit
  CrossSection
    general
      6
    circleA
    circle0
    circle1
    circle2
    circle3
    circleB
  mappingName 
    pipe
  cubic
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
  pause
  compute overlap
  * pause
exit
save an overlapping grid
itd2.hdf
pipe
exit

