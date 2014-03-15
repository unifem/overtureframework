* create mappings
 Annulus
  inner radius
   0.  .2
  make 3d (toggle)
   -4.0
  mappingName
    circ0
  exit
  Annulus
   inner radius
    0.  .2
   make 3d (toggle)
    0.
   mappingName
    circ1
   exit
   CrossSection
    general
      2
    circ0
    circ1
    boundary conditions
    -1 -1 2 3 1 0
    mappingName
     middle
    share
    0 0 5 6 0 0
   exit
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
  pause
 exit
