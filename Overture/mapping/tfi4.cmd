*
*  3D TFI Mapping between two Annulus mappings
*
* make the annulus for the top
Annulus
  make 3d (toggle)
    2.
  mappingName
    top-annulus
  exit
* make the annulus for the bottom
Annulus
  outer radius
    1.5
  inner radius
    1.
  make 3d (toggle)
    0.
  mappingName
    bottom-annulus
exit
tfi
  choose back curve  
    bottom-annulus
  choose front curve 
    top-annulus
  boundary conditions
    -1 -1 1 2 3 4


