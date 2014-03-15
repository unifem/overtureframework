*
* Create a patch with transfinite interpolation
*
* create a line for the top boundary
line
  number of dimensions 
   2
  specify end points
    -1. 1. 1. 1.
exit
* create a spline for the bottom boundary
spline
  enter spline points
    5
    -1. 0.
    -.5 0.
    0. .25
    .5 .0
    1. 0.
*  periodic
*   1
exit
* create a tfi patch
tfi
  choose bottom
    spline
  choose top
    line
  exit
*
  elliptic
    elliptic smoothing
      maximum number of iterations
        10
      number of multigrid levels
        2
      line
      elliptic boundary conditions
        bottom (side=0,axis=1)
        noSlip orthogonal and specified spacing
          .05
        exit

      generate grid






    elliptic boundary conditions
      bottom
      slip orthogonal
    exit

    test orthogonal

    generate grid


*    set GRID boundary conditions
*      1 1 2 1
    elliptic smoothing
      maximum number of iterations
        20
      Line Solver
      start smoothing
