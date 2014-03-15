create mappings
  rectangle
    mappingName
      backGround
    boundary conditions
      2 2 2 2
    specify corners
      -7 -7 100 7 
    lines
      50 25
  exit
  spline
    enter spline points
      17
     31.3 0
     30    .15
     25    .35
     20    .55
     15    .7
      2    .7
      1    .65
      .01  .1
      0 0
      .01 -.1
      1   -.65
      2   -.7
     15   -.7
     20   -.55
     25   -.35
     30   -.15
     31.3  0
    mappingName
      fishSurface
    boundary conditions
      -1 -1 1 0
    exit
  pause
  * make an ellipse as an outer boundary
  Circle or ellipse
    specify centre
     15. .0
    specify axes of the ellipse
      18. 4. 
  exit
 tfi
    choose top curve    (r_2=1)
      circle
    choose bottom curve (r_2=0)
      fishSurface
    boundary conditions
      -1 -1 1 0
    lines
      101 9 
    mappingName
      fish-tfi
    pause
  exit
  elliptic
    transform which mapping?
      fish-tfi
    set maximum number of iterations
      100
    * do NOT project since the mapping has a singularity
    * at the trailing edge:
    project onto original mapping (toggle)
    * generate the elliptic grid
    elliptic smoothing
    mappingName
      fish-grid
    pause
  exit
exit this menu
  generate an overlapping grid
    backGround
    fish-grid
    done choosing mappings
