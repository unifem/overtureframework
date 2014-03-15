***********************************************************************
* 
*  cgcns command file: 
*        Viscous compressible flow over a backward facing step
* 
***********************************************************************
*
$tFinal=20.; $tPlot=.1; $show=" "; $mu=0.; $kThermal=0.; $Prandtl=.72; 
* 
$grid="backStepSmooth.hdf"; 
$tPlot=.1; $mu=.01; $kThermal=$mu/$Prandtl;
* 
*
$grid
*
  compressible Navier Stokes (Jameson)
*   one step
  exit
  turn off twilight
*
*  do not use iterative implicit interpolation
*
  final time $tFinal
  times to plot $tPlot
*plot and always wait
*
  show file options
    compressed
    open
      $show
    frequency to flush
      1
    exit
  reduce interpolation width
    2
*****
  boundary conditions
    all=noSlipWall uniform(T=.943011)
    inlet(0,0)=superSonicInflow uniform(r=2.6069,T=.943011,u=0.694444,v=0.,s=0.0)
    corner(0,0)=superSonicInflow uniform(r=2.6069,T=.943011,u=0.694444,v=0.,s=0.0)
    mainChannel(1,0)=superSonicOutflow
    inlet(1,1)=slipWall
    mainChannel(1,1)=slipWall
    done
*
  pde parameters
    mu
     $mu
    kThermal
     $kThermal
  done
*  debug 
*    1
*
  initial conditions
   uniform flow
    r=2.6069,T=.943011,u=0.694444
  continue
continue
*

movie mode
finish
