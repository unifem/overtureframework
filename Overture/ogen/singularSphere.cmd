*
* Make a mapping that uses the sphere with
* the singular ends not removed.
*
create mappings
  Box
    specify corners
      -1 -1 -1 1 1 1
    lines
      15 15 15
    mappingName
      box
    exit
  Sphere
    mappingName
      sphere
    lines
      15 15 5
    * NOTE: we need to set the bc's at the
    * singular ends to be physical (=3)
    boundary conditions
      3 3 -1 -1 1 0     
    exit
  exit this menu
  generate an overlapping grid
    box
    sphere
    * display intermediate results
    compute overlap

