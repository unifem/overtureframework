*
* Create a cylindrical body of revolution 
* from a Smoothed Polygon
*
SmoothedPolygon
  vertices
  7
  -1. 0.
  -1. .25
  -.8 .5
  0. .5
  .8 .5
  1. .25
  1. 0.
  n-dist
  fixed normal distance
  .1
  n-dist
  fixed normal distance
  .4
  corners
  specify positions of corners
  -1. 0.
  1. 0
  -1.4 0.
  1.4 0
  t-stretch
  0 5
  .15 10
  .15 10
  0 10
  .15 10
  .15 10
  0 10
exit
body of revolution
  tangent of line to revolve about
  1. 0 0
  boundary conditions
    0 0 -1 -1 1 0 
  mappingName
    cylinder
  lines
    45 21 7 
* exit