# -- test derivative periodic grids --
#
# Generate a hyperbolic grid starting from a spline
#
spline
 periodicity
   1
 enter spline points
  7
  0 0
  1 0
  2 1
  3 2
  4 1
  5 0
  6 0
 mappingName
  spline
exit
hyperbolic
  mappingName
    bump
    backward
    spacing: geometric
#
    BC: left fix x, float y and z
    BC: right fix x, float y and z
    periodicity: axis 0 derivative periodic
# 
    generate
# pause
#
    forward and backward
    generate
    distance to march .3 .3 (forward,backward)  
    lines to march 4,4 (forward,backward)
    generate
# pause
exit
exit

