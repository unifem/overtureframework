*
* Make a grid with two airfoils
*
create mappings
  *
  * First make a back-ground grid  
  *
  rectangle
    mappingName
      backGround
    specify corners
      -1.0 -1.5 3.0 1.5 
    lines
      81 61 
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
      .9  .4
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
      71 11
    mappingName
      airfoil-tfi
    * pause
  exit
  *
  *
  elliptic
    *project onto original mapping (toggle)
    transform which mapping?
      airfoil-tfi
  elliptic smoothing
    * slow start to avoid porblems at trailing edge
    number of multigrid levels
      3
    maximum number of iterations
      15
    red black
    smoother relaxation coefficient
      .1
    generate grid
    * now reset parameters for better convergence
    maximum number of iterations
      30
    smoother relaxation coefficient
      .8
    generate grid
    exit
    mappingName
      airfoil-grid
    * pause
  exit
  rotate/scale/shift
    scale
      .75 .75
    shift
      .5 -.25
    mappingName
      lowerFoil
    exit
exit
*
* make an overlapping grid
*
generate an overlapping grid
    backGround
    airfoil-grid
    lowerFoil
  done
  change parameters
    improve quality of interpolation
    set quality bound
      1.5
    ghost points
      all
      2 2 2 2 2 2
  exit
  * pause
  compute overlap
exit
*
save an overlapping grid
biFoil.hdf
biFoil
exit

  
