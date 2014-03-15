*
* create a square with one end rounded
*
create mappings
  SmoothedPolygon
    mappingName
    end
    vertices
    4
    .5 1.
    0. 1.
    0. 0. 
    0. .5
    vertices
    4
    .5 1.
    0. 1.
    0. 0.
    .5 0.
    n-dist
    fixed normal distance
    .1
    sharpness
    10
    10 
    10
    10
    t-stretch
    0. .5
    .15 2
    .15 2
    0 5
    boundary conditions
    0 0 1 0
    share
    0 0 1 0
  exit
  rectangle
    boundary conditions
    0 1 1 1 
    share
    0 0 1 1
    lines
      41 41
  exit
  view mappings
    end
    square
  exit
exit this menu
check overlap
    square
    end
  done
  change parameters
    shared boundary tolerance
      all
      .25
  exit
