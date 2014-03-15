*
* circle in a channel with a stretched grid
*
create mappings
  *
  rectangle
    specify corners
    -2. -2. 2. 2.
    lines
    61 61 
    boundary conditions
    1 1 1 1
    mappingName
    square
    exit
  *
  Annulus
    lines
    11 9
    boundary conditions
    -1 -1 1 0
    mappingName
      annulus
    exit
*
  stretch coordinates
    transform which mapping?
    annulus
    stretch
      * specify stretching along axis=0
      specify stretching along axis=1
        stretching type
        inverse hyperbolic tangent
        layers
          1
          .5 20. 0.
        exit
      exit
      mappingName
        annulus-stretched
    exit
  exit this menu
*
  generate an overlapping grid
    square
    annulus-stretched
    done choosing mappings
*
    display intermediate
    debug
     7
    compute overlap
    continue
