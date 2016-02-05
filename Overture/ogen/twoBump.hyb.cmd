*
* Two half cylinders in a channel
*
create mappings
*
rectangle
  specify corners
    -1.5 -0. 4. 3.
  lines
    83 45     
    * 165 90     
    * 330 180
  boundary conditions
    1 1 1 1
  mappingName
   channel
exit
*
Annulus
  mappingName
    bottomAnnulus
  lines
    45 9
    * 90 18 
    * 180 36
  inner and outer radii
    .5 1.
  start and end angles
    0. .5
  centre for annulus
    .5 0.
  boundary conditions
    1 1 1 0
exit
Annulus
  mappingName
    topAnnulus
  lines
    45 9
    * 90 18 
    * 180 36
  inner and outer radii
    .5 1.
  start and end angles
    .5 1. 
  centre for annulus
    1. 3.
  boundary conditions
    1 1 1 0
exit
*
exit
generate a hybrid mesh
    channel
    bottomAnnulus
    topAnnulus
  done
  compute overlap
  exit
  set plotting frequency (<1 for never)
  -1
  continue generation
  exit 
  save grid in ingrid format
  twoBump.hyb.msh
exit
