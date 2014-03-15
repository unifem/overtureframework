create mappings
  *
  * First make a back-ground grid  
  *
  rectangle
    mappingName
      backGround
    set corners
      -1.5 2.5  -1.5 1.5 
    lines
      61 45 
  exit
  * make the NACA airfoil (curve)
  Airfoil
    airfoil type
      naca
    camber 
      .1
    lines
     91
    mappingName
      airfoil-curve
    exit
*
  hyperbolic
    boundary conditions for marching
      left   (side=0,axis=0)
      trailing edge
      right  (side=1,axis=0)
      trailing edge
      exit
    distance to march
      .3
    lines to march
      21
    distance to march
      .3
    uniform dissipation coefficient
      .01
    geometric stretching, specified ratio
      1.4
    generate
pause
    mappingName
     airfoil
    boundary conditions
      -1 -1 1 0
    exit
 exit
*
* make an overlapping grid
*
generate an overlapping grid
    backGround
    airfoil
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
  exit
  * pause
  compute overlap
exit
*
save an overlapping grid
naca.hype.hdf
naca
exit
