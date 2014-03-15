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
      71 15
    mappingName
      airfoil-tfi
    * pause
  exit
  *
  elliptic
    transform which mapping?
      airfoil-tfi

    set Poisson j-line sources
      * number of sources, power,  diffusivity (exponent) and location
      1
      5.
      2.
      0. 
    set maximum number of iterations
      100
    * do NOT project since the mapping has a singularity
    * at the trailing edge:
    project onto original mapping (toggle)
    * generate the elliptic grid
    elliptic smoothing
    mappingName
      airfoil-grid
    * pause
  exit
