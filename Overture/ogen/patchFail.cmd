* this works, I though it might fail.
create mappings
  rectangle
    mappingName
      backGround
    exit
  rectangle
    mappingName
      patch1
    specify corners
      0 .25 .5 .75
    boundary conditions
      1 0 0 0
    exit
  rectangle
    mappingName
      patch2
    lines
      9 5
    specify corners
      .11 .3 .91 .7
    boundary conditions
      0 0 0 0
    exit
  exit this menu
*
  generate an overlapping grid
    backGround
    patch1
    patch2


