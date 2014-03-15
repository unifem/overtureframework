  Cylinder
    exit
  reduce domain dimension
    reduce the domain dimension of which mapping?
    Cylinder
    choose the in-active axes
      active
      0.
      active
    mappingName
      s=0
    exit
  reduce domain dimension
    reduce the domain dimension of which mapping?
    Cylinder
    choose the in-active axes
      active
      1.
      active
    mappingName
      s=1
    exit
  reduce domain dimension
    reduce the domain dimension of which mapping?
    Cylinder
    choose the in-active axes
      active
      active
      0.
    mappingName
      r=0
    exit
  reduce domain dimension
    reduce the domain dimension of which mapping?
    Cylinder
    choose the in-active axes
      active
      active
      1.
    mappingName
      r=1
    exit
*
  tfi
    mappingName
      tfi4
    choose back
      r=0
    choose front
      r=1
    pause
    choose bottom
      s=0
    choose top
      s=1
*   boundary conditions
*     1 2 3 4 -1 -1
*    axes orientation
*      2 1 0
  mappingName
    tfi
  pause
 exit
 check
   tfi
