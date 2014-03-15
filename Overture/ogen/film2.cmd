*
* create a grid for the soap film experiment
*
create mappings
  rectangle
    specify corners
    -1 -1 4 1
    lines
    * 5N 2N
    501 201     251 101 
  exit
  Annulus
    centre for annulus
    .0 .5
    inner radius
    .25
    outer radius
    .4
    boundary conditions
    -1 -1 1 0
    lines
     181 21   91  11
    mappingName
      annulus1
  exit
  Annulus
    centre for annulus
     0. -.5
    inner radius
    .25
    outer radius
    .4
    boundary conditions
    -1 -1 1 0
    lines
     181 21   91 11
    mappingName
      annulus2
  exit
* Stretch coordinates
  stretch coordinates
    transform which mapping?
    annulus1
    stretch
      specify stretching along axis=1
        layers
        1
       1. 4. 0.
      exit
    exit
    mappingName
    stretched-annulus1
  exit
* Stretch coordinates
  stretch coordinates
    transform which mapping?
    annulus2
    stretch
      specify stretching along axis=1
        layers
        1
       1. 4. 0.
      exit
    exit
    mappingName
    stretched-annulus2
  exit
*
exit this menu
make an overlapping grid
  square
  stretched-annulus1
  stretched-annulus2
  Done choosing Mappings
* Specify new CompositeGrid parameters
*   interpolationIsImplicit
*   Implicit
*   Repeat
* Done specifying CompositeGrid parameters
  Specify new MappedGrid Parameters
    numberOfGhostPoints
    2 2 2 2
    Repeat
  Done specifying MappedGrid parameters
  exit this menu
Done specifying the CompositeGrid
save an overlapping grid
film2.hdf
film
exit
