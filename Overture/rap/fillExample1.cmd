*
* rap command file to build a shaped charge liner and fill it with spherical particles
*  To run this example type: 
*       rap fillExample1.cmd 
*
liner...
* choose points on the piecewise linear liner
  linear liner...
   ll point 1 0 0
   ll point 2 2.28284 2.28284
   ll point 3 2.28284 2
   ll point 4 .28284 0
  close linear liner
  pause
*  create the surface of revolution
  revolve around axis
*  load the liner volume with spheres
  radius for spheres .1 .05 .01
  probability for spheres .2 .3 .5
  total volume fraction 1.500e-01
  RNG seed 97845
  pause
  fill with spheres
