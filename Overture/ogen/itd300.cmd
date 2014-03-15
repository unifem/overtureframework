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
    -0.6
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
    pause
    exit
   rotate/scale/shift
    transform which mapping
    part0
     rotate
     90. 1
     0. 0. 0.
    *lines
    * 41 15 21
    boundary conditions
    -1 -1 1 1 1 0
    *exit
    *stretch coordinates
     *stretch
     *specify stretching along axis=2 (x3)
     *layers
       *1
      *1. 5. 1
     *exit
     *exit
    mappingName
      Part0
    exit
