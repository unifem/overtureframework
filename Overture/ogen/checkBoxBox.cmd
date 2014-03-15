* A non-conforming box sticking into another box
create mappings
  Box
    lines
      10 10 10
    mappingName
      mainBox
  exit
  Box
    specify corners
      -.4 .25 .25 .1 .75 .75
    boundary conditions
      1 0 1 1 1
    mappingName
      insert
  exit
  exit this menu
check overlap
  mainBox
  insert
  done
  change parameters
    prevent hole cutting
      all
      all
    done
  exit
