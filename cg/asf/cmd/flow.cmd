*
* cgasf command file: all-speed flow past a body
*
* Usage:
*   
*  cgasf [-noplot] -g=<name> -tf=<tFinal> -tp=<tPlot> -Mach=<num> -Re=<num> -bg=<backGround> ...
*                  -solver=<yale/best> -ts=<implicit> -debug=<num> -go=[run/halt/og]
* 
* Examples:
* 
*  cgasf noplot flow -g=sinfoil.hdf -bg=airfoil -Mach=.1 -Re=10. -tp=.1 -tf=20. -debug=3
*  cgasf noplot flow -g=cice2.order2.hdf -bg=square -Mach=.1 -Re=50. -tp=.1 -tf=10. -go=og
* 
* 
* --- set default values for parameters ---
* 
$go="run";
$show =" "; $u0=1.; $Mach=.1; $Reynolds=10.; $tFinal=5.; $tPlot=.1; $debug=0; $cfl=.9; 
$anu=0.; $nuRho=1.; $debug=1; $tol=1.e-10; 
$method ="linearized all speed implicit"; 
* $method="all speed implicit"; 
* 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"go=s"=>\$go,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot, "solver=s"=>\$solver, "show=s"=>\$show, \
            "debug=i"=>\$debug,"ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, \
            "Mach=f"=>\$Mach,"Re=f"=>\$Reynolds, "noplot=s"=>\$noplot );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" ){ $go = "movie mode\n finish"; }
* if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
*                      { $model = "incompressible Navier Stokes\n Boussinesq model"; }
*
* $grid="ellipse.hdf"; $method="adams order 2"; $cfl=.25; $anu=10.; $backGround="backGround";
* $grid="ellipse.hdf"; $backGround="backGround"; $Mach=.1; 
* 
* $grid="sinfoila.hdf"; $backGround="airfoil"; $Mach=.1; $tPlot=.002; $debug=63; 
* $grid="sinfoil0.hdf"; $backGround="airfoil"; $Mach=.1; $tPlot=.02; $debug=1; 
* $grid="sinfoil.hdf"; $backGround="airfoil"; $Mach=.1; $tPlot=.1;
* $grid="sinfoil.hdf"; $backGround="airfoil"; $Mach=.01; $tPlot=.1;
* $grid="sinfoil2.hdf"; $backGround="airfoil"; $Mach=.1; $tPlot=.1; 
*
* $grid="cic.hdf"; $backGround="square"; $Mach=.1; $tPlot=.1; $Reynolds=50.; 
* $grid="cice2.order2.hdf"; $backGround="square"; $Mach=.1; $tPlot=.1; $Reynolds=50.; 
* $grid="cice4.order2.hdf"; $backGround="square"; $Mach=.1; $tFinal=10.; $tPlot=.1; $Reynolds=100.; 
*
$grid
*
  all speed Navier Stokes
  exit
*
  OBPDE:densityFromGasLawAlgorithm
  * OBPDE:defaultAlgorithm
  $method
* 
  turn off twilight zone 
* 
  final time $tFinal
  cfl $cfl 
  times to plot $tPlot
* 
* Next specify the file to save the results in. 
* This file can be viewed with Overture/bin/plotStuff.
  show file options
    * compressed
    open
     $show
    frequency to flush
      5
    exit
*
  plot and always wait
  * no plotting
  debug $debug 
* 
  pde parameters
    Mach number
     $Mach
**   OBPDE:pressureLevel 10.
    Reynolds number
      $Reynolds
    OBPDE:nuRho $nuRho
    OBPDE:anu $anu
* 
*     OBPDE:linearize implicit method 0
* 
    done
* 
  boundary conditions
    all=slipWall  uniform(T=1.)
    $backGround(0,0)=subSonicInflow uniform(r=1.,u=$u0,v=0.,T=1.)
    $backGround(1,0)=subSonicOutflow pressure(1.*p+1.*p.n=1.)
**    $backGround(1,0)=convectiveOutflow  pressure(1.*p+1.*p.n=1.)
    $backGround(0,1)=slipWall
    $backGround(1,1)=slipWall
    Annulus(0,1)=noSlipWall uniform(T=1.)
*
    done
  initial conditions
    * Give the perturbation pressure here
    uniform flow
      r=1., u=$u0, T=1., p=0.
  exit
  debug $debug
  project initial conditions
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
      0
    exit
*
  continue
  $go
