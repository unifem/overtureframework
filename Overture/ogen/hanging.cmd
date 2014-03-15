create mappings
  Annulus
    boundary conditions
      -1 -1 1 0
    mappingName
      annulus
    exit
  rectangle
    mappingName
      square
    lines
      11 11
    boundary conditions
      0 1 0 1
  exit
  exit this menu
  generate an overlapping grid
    square
    annulus
    allow hanging interpolation

    compute overlap

