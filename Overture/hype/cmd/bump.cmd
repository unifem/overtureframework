* Generate a hyperbolic grid starting from a spline
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
    generate
#
    forward and backward
    generate
    distance to march .3 .3 (forward,backward)  
    lines to march 4,4 (forward,backward)
    generate

    SC:stretch r1 0 1 20. 0.5 (id,weight,exponent,position)


  distance to march .5
  lines to march 7 
  * grow grid in opposite direction
  generate

