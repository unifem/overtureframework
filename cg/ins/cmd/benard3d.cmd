*
* cgins - 3D Rayleigh Benard problem
*
$tFinal=1.; $tPlot=.1; $nu=.1; $kThermal=.1; $thermalExpansivity=1.; $Tbottom=10.; 
* 
$grid = "benard3d2.hdf"; $tFinal=1000.; $tPlot=.1; $nu=.02; $kThermal=.02;
* 
$grid
*
  incompressible Navier Stokes
  Boussinesq model
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  exit
* 
  turn off twilight zone 
  final time $tFinal
  times to plot $tPlot
  * plot and always wait
  no plotting
*
*  cfl .75
* 
  pde parameters
    nu  $nu
    kThermal $kThermal
    gravity
      0. -1. 0. 
   done
  * choose a method to solve the pressure equation
  pressure solver options
     choose best iterative solver
     * PETSc
     * these tolerances are chosen for PETSc
     relative tolerance
       1.e-4
     absolute tolerance
       1.e-6
    exit
* 
  boundary conditions
    all=slipWall
    backGround(0,1)=noSlipWall , uniform(T=$Tbottom)
    backGround(1,1)=noSlipWall , uniform(T=0.)
   done
* ----------
  initial conditions
  uniform flow
    p=1., u=0., v=0., T=0. 
  exit
*   project initial conditions
* ----------
* 
  debug
    1 31
*   check error on ghost
*     1
 continue


 movie mode 
 finish
