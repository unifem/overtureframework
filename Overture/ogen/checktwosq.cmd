* one square on top of another
create mappings
  rectangle
    mappingName
    backGround
    lines
      9 7
    share
      0 1 2 3
  exit
  rectangle
    mappingName
    top
    specify corners
      .25 0. .75 1.
    lines
      6 7
    boundary conditions
      0 1 1 1 
    share
      0 1 2 3
  exit
  * pause
exit this menu
check overlap
  backGround
  top
junk

