*
* two circles in a box
*
create mappings
*
rectangle
  specify corners
    -2. -2. 2. 2.
  lines
    91 91 
  boundary conditions
    1 1 1 1
  mappingName
  backGround
exit
*
Annulus
  mappingName
    annulus1
  centre
    0. .75
  lines
    101 21
  boundary conditions
    -1 -1 1 0
exit
Annulus
  mappingName
    annulus2
  centre
    0. -.75
  lines
    101 21
  boundary conditions
    -1 -1 1 0
  exit
*
  stretch coordinates
    transform which mapping
      annulus1
    mappingName
      stretched-annulus1
    stretch
    specify stretching along axis=1
      layers
      1
      1. 5. .0
     exit
    exit
  exit
*
  stretch coordinates
    transform which mapping
      annulus2
    mappingName
      stretched-annulus2
    stretch
    specify stretching along axis=1
      layers
      1
      1. 5. .0
     exit
    exit
  exit
*
exit
generate an overlapping grid
    backGround
    stretched-annulus1
    stretched-annulus2
  done
  change parameters
    improve quality of interpolation
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
  exit
*
save an overlapping grid
twoCircles2.hdf
twoCircles
exit
