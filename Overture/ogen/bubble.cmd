* 
* Create a "bubble in a square"
*   This is like a circle in a square but the inside of the circle also has a grid
*
create mappings
* first make an annulus for the droplet
  Annulus
    inner radius
      .5
    outer radius
      1.
    lines
      31 6
    mappingName
      droplet-annulus
    boundary conditions
      -1 -1 0 1
    exit
*  make a rectangle to fill in the inside of the droplet
  rectangle
    specify corners
      -.7 -.7 .7 .7
    lines
      11 11
    boundary conditions
      0 0 0 0
    mappingName
      droplet-square
    exit
* Here is an annulus to go around the outside of the droplet
  Annulus
    inner radius
      1.
    outer radius
      1.5
    lines
      31 6
    mappingName
      droplet-outer-annulus
    boundary conditions
      -1 -1 1 0
    exit
*  make a rectangle for the region exterior to the droplet
  rectangle
    specify corners
      -2.0 -2.0 2.0 2.0 
    lines
      31 31 
    boundary conditions
      1 1 1 1
    mappingName
      backGround
    exit
  exit
*
generate an overlapping grid
  backGround
  droplet-outer-annulus
  droplet-square
  droplet-annulus
  Done
  Specify new CompositeGrid parameters
    interpolationIsImplicit
    * Explicit
    Implicit
    Repeat
  Done
Done
save an overlapping grid
  bubble.hdf
  bubble
exit
