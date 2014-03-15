*
* Make a grid around a NACA0012 airfoil
*
  Airfoil
    airfoil type
      naca
  exit
  Circle or ellipse
    specify centre
     .5 .0
    specify axes of the ellipse
      1.5 1.
  exit
  tfi
    choose curves for sides
      airfoil
      circle
    boundary conditions
      -1 -1 1 0
  exit
  *
  elliptic
    * do NOT project since the mapping has a singularity
    * at the trailing edge:
    project onto original mapping (toggle)
    * pause
    set Poisson j-line sources
    * number of sources, power,  diffusivity (exponent) and location
      1
      5.
      2.
      0. 
    set maximum number of iterations
      10
    * generate the elliptic grid
    elliptic smoothing
    mappingName
      airfoil-grid
  exit
open a data-base
  af.hdf
  open a new file
put to the data-base
  airfoil-grid
get from the data-base
  airfoil-grid
view mappings


