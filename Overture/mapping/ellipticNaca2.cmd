*
* Make a grid around a NACA0012 airfoil *** use Thomas' version
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
  elliptic2
    *project onto original mapping (toggle)
    transform which mapping?
      airfoil-tfi
    set GRID boundary conditions
    -1 -1 1 1
*    0.02
    *0.02
*   set Poisson j-line sources
*      **number of sources, power,  diffusivity (exponent) and location
*    1
*    5.
*    2.
*    0. 
  elliptic smoothing
    maximum number of V-cycles
    100
    *smoothing method
    Line Solver
    *Red Black
    *1.000
    *0
    *start smoothing
