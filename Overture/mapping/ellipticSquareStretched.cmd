*  grid for a corner geometry
  rectangle
    lines
     21 21
    periodicity
     1 0
    exit
*
  elliptic
    elliptic smoothing
      maximum number of iterations
        10
      number of multigrid levels
        2
      elliptic boundary conditions
        bottom (side=0,axis=1)
        noSlip orthogonal and specified spacing
          .01
        exit
*       debug
*        7
      line
      generate grid
