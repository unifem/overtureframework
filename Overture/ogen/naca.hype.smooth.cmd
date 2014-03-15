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
* pause
    exit
*
  hyperbolic
*
    BC: left trailing edge
    BC: right trailing edge
*
    lines to march 21
    distance to march .45
*    uniform dissipation coefficient .01
    geometric stretch factor 1.3
* 
    generate
*    -- smooth the hyperbolic grid ---
    smoothing...
    GSM:BC: left smoothed
    GSM:BC: right smoothed
    GSM:BC: top smoothed
    GSM:use initial grid as control grid 1
    GSM:number of iterations 20
    GSM:smooth grid
* 
* pause
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
  * pause
exit
*
save an overlapping grid
naca.hype.smooth.hdf
naca
exit
