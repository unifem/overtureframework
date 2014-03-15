*
* Make a grid around a NACA0012 airfoil
*
* create mappings
  *
  * First make a back-ground grid  
  *
  rectangle
    mappingName
      backGround
    specify corners
      -1.5 -1.5 2.5 1.5 
    lines
      41 31
  exit
  * make the NACA0012 airfoil (curve)
  Airfoil
    airfoil type
      naca
  exit
  * make an ellipse as an outer boundary
  Circle or ellipse
    specify centre
     .5 .0
    specify axes of the ellipse
      1.5 1.
  exit
  * blend the airfoil to the ellipse to make a grid
  tfi
    choose bottom curve
      airfoil
    choose top curve
      circle
    boundary conditions
      -1 -1 1 0
    lines
      65 17
    mappingName
      airfoil-tfi
  exit
  *
  elliptic
    *project onto original mapping (toggle)
    transform which mapping?
      airfoil-tfi
  elliptic smoothing
    number of multigrid levels
      1
    maximum number of iterations
      1
      boundary conditions
        bottom (side=0,axis=1)
        noSlip orthogonal and specified spacing
          .05
        exit
    


    maximum number of iterations
      5
    red black
    number of multigrid levels
      3
    smoother relaxation coefficient
      .1
    generate grid
    maximum number of iterations
      20
    smoother relaxation coefficient
      .8
    generate grid


* Note: use spacing .025 is ok