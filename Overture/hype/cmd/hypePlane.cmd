  plane or rhombus
    specify plane or rhombus by three points
    0 0 0   0 0 1   1 0 0
  exit
  hyperbolic 
    points on initial curve 3 3
    lines to march 3
    points on initial curve 11 11
    lines to march 7
    distance to march .2
    BC: bottom fix x, float y and z
    BC: top fix x, float y and z
    BC: left fix z, float x and y
    BC: right fix z, float x and y
# OK: 
    generate
# pause
#   -- test periodic --
    boundary conditions
#     -1 -1 -1 -1 2 0
#    -1 -1 0 0 2 0
    0 0 -1 -1 2 0
# BAD
    debug
    3
    generate


    smaller:0

    specify plane or rhombus by three points
    0 0 0 0 0 1 2 1 0
    exit


  plane or rhombus
    specify plane or rhombus by three points
    0 0 0 0 0 1000 2000 1000 0
    exit
  hyperbolic
    generate
    marching options...
    BC: bottom fix y, float x and z
    distance to march 500
    lines to march 11
    generate


  plane or rhombus
    specify plane or rhombus by three points
    0 0 0 0 0 100 200 100 0
    exit
  hyperbolic
    generate
    marching options...
    BC: bottom fix y, float x and z
    distance to march 50
    lines to march 11
    generate



  plane or rhombus
    specify plane or rhombus by three points
    0 0 0 0 0 1 2 1 0
    exit
  hyperbolic
    generate
    marching options...
    BC: bottom fix y, float x and z
    distance to march .5
    lines to march 11
    generate




