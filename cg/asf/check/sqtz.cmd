* 
*  test the asf solver
*
$tFinal=.5; $tPlot=.1; $debug=0;
*
$grid="square10.hdf"; 
* $grid="square5.hdf"; $tFinal=.01; $tPlot=.01; $debug=63;
* 
* $grid="cic.hdf";
*
$grid
* 
  all speed Navier Stokes
  continue
* 
  linearized all speed implicit
*
  OBPDE:densityFromGasLawAlgorithm
  * OBPDE:defaultAlgorithm
* 
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  OBTZ:degree in space 1
  OBTZ:degree in time 1
  final time $tFinal
  cfl 0.5
  times to plot $tPlot
*
  pde parameters
   OBPDE:mu 0.1
   * M = 1 /sqrt( gamma*Rg ) 
   OBPDE:Rg (gas constant) 2
*   OBPDE:pressureLevel 100.
*   dissipation on rho:
*   OBPDE:nuRho .1
*   OBPDE:anu 1.
  done
*
   debug $debug
*
  boundary conditions...
    bc command all=noSlipWall  
    done
  continue
  movie mode
  finish


