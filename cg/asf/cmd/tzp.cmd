* 
*  test the asf solver 
*
$tFinal=.5; $tPlot=.1; $debug=0; $degreeX=1; $degreeT=2; $cfl=.9; $nuRho=.1; $anu=0.; $tol=1.e-12; 
$lasi ="linearized all speed implicit"; 
$allSpeed="all speed implicit";
$method=$lasi;
*
* $grid="square20.hdf"; 
* $grid="square5.hdf";  $degreeX=1; $degreeT=1; $tFinal=.5; $tPlot=.01; $debug=31; 
* $grid="nonSquare5.hdf";  $degreeX=1; $degreeT=1; $tFinal=.5; $tPlot=.01; $debug=1; 
* $grid="square10.hdf";  $degreeX=1; $degreeT=1; $tFinal=.5; $tPlot=.01; $debug=1; 
* $grid="square40.hdf";  $degreeX=2; $degreeT=2; $tPlot=.01; $anu=5.;  $cfl=.25; 
* $grid="square5.hdf"; $tFinal=.01; $tPlot=.01; $debug=15;
* 
* $grid="sinfoil0.hdf";  $degreeX=1; $degreeT=1; $tFinal=.5; $tPlot=.01; $debug=1; 
* $grid="sinfoil2.hdf";  $degreeX=2; $degreeT=3; $tFinal=.5; $tPlot=.01; $debug=1; 
* $grid="cice2.order2.hdf"; $degreeX=1; $degreeT=1; $tFinal=.5; $tPlot=.01; $debug=1; 
* 
* $grid="cic.hdf"; 
$grid="cice2.hdf";  $degreeX=2; $degreeT=2; $tPlot=.01; $anu=0.;
* $grid="cic4.hdf";  $degreeX=2; $degreeT=2; $tPlot=.01;
* $grid="cic6.hdf"; $anu=5. 
*
* $grid="box5.hdf";  $degreeX=1; $degreeT=1; $tFinal=.5; $tPlot=.01; $debug=1; 
*
$grid
* 
  all speed Navier Stokes
  continue
  * choose an explicit time stepping method:
  * adams order 2
* 
* 
  OBPDE:densityFromGasLawAlgorithm
  * OBPDE:defaultAlgorithm
*
  $method
* 
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  OBTZ:degree in space $degreeX
  OBTZ:degree in time $degreeT
  final time $tFinal
  cfl $cfl 
  times to plot $tPlot
*
  pde parameters
   OBPDE:mu 0.1
   OBPDE:kThermal 0.14
   * M = 1 /sqrt( gamma*Rg ) 
   OBPDE:Rg (gas constant) 1
   OBPDE:pressureLevel 10.
*   dissipation on rho:
   OBPDE:nuRho $nuRho
   OBPDE:anu $anu
  done
*
   debug $debug
*
  boundary conditions
**    square(0,0)=dirichletBoundaryCondition
**    bc command all=noSlipWall  
   all=noSlipWall  
**    bc command all=dirichletBoundaryCondition
  done
*
*
   implicit time step solver options
     * choose best iterative solver
     * these tolerances are chosen for PETSc
     choose best iterative solver
     * PETScNew     
     relative tolerance
       $tol
     absolute tolerance
       1.e-15
     debug 
      1 7 0 1 7 3
    exit
  continue





* 
*  test the asf solver
*
$tFinal=1.; $tPlot=.1; $debug=0;
*
* $grid="square10.hdf"; 
$grid="square5.hdf"; $tFinal=.01; $tPlot=.01; $debug=63;
* $grid="box10.hdf"; 
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
* 
  continue


