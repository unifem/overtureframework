create mappings
  Annulus
    start and end angles
      0 .25
    inner
      .5
    outer
      .7 
    boundary conditions
      0 4 1 0 
    mappingName
      leftAnnulus
    centre for annulus
      -.75 0
    lines
      31 5 
    exit
*
  Annulus
    start and end angles
      0 .25
    inner
      .8  
    outer
      1.  
    boundary conditions
      0 4 0 2
    mappingName
      rightAnnulus
    centre for annulus
      -.75 0
    lines
      31 5 
    exit
*
  Annulus
    inner
      .65
    outer
      .85
    start and end angles
      0 .25
    boundary conditions
      0 4 0 0 
    mappingName
      annulus2
    centre for annulus
      -.75 0
    lines
      25 9
    exit
*
  rectangle
    lines
      15 15
    specify corners
      -.5 -.9 .5 .1
    mappingName
      square
    lines
      31 31 
    exit
  exit this menu
  generate an overlapping grid
    leftAnnulus
    rightAnnulus
    annulus2
    square
    done choosing mappings
    change parameters
      prevent hole cutting
        all
        all
        done
      exit
    pause
    compute overlap
