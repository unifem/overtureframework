* Generate a hyperbolic grid starting from a spline
debug
  3
spline
enter spline points
2
0 0
1 0
mappingName
spline
exit
hyperbolic
  mappingName
    bump
    backward
    spacing: geometric
    points on initial curve 11
    generate

#
    forward and backward
    generate
    distance to march .3 .3 (forward,backward)  
    lines to march 4,4 (forward,backward)
    generate
pause
  exit
exit


