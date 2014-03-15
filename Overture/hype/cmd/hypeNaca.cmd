Airfoil
airfoil type
naca
camber
  .1
lines
  91
exit
hyperbolic
    boundary conditions for marching
      left   (side=0,axis=0)
      trailing edge
      right  (side=1,axis=0)
      trailing edge
      exit
* implicit coefficient
*   0.
 distance to march .45 .05  .025
 lines to march  21 3
* uniform dissipation .1
 geometric stretch factor 1.3
* debug
*   7
 generate
 mappingName
  airfoil
pause
  exit
exit

