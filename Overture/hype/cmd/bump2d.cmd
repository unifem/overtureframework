* Generate a hyperbolic grid starting from a spline
debug
  3
spline
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
    # TEST: 
    # points on initial curve 21
    generate
pause
#
    forward and backward
    generate
    distance to march .3 .3 (forward,backward)  
    lines to march 4,4 (forward,backward)
    generate
pause
  exit
exit


