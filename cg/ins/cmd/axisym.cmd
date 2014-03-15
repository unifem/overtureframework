*
* cgins example: test axisymmetric flow (including flow past a half annulus, ie. flow past a sphere)
*
* Usage:
*   
*  cgins [-noplot] axisym -g=<name> -tz=<poly/trig/none> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
*        -bc=<a|d|r>  -solver=<yale/best> -order=<2/4> -model=<ins/boussinesq> -ts=[imp|pc] -debug=<num> ..,
*        -ad2=<0|1> -bg=<backGround> -project=<0/1> -iv=[viscous/adv/full] -imp=<val> -rf=<val> -go=[run/halt/og]
* 
*  -iv : implicit variation : viscous=viscous terms implicit, adv=viscous + advection, full=full linearized version
*  -imp : .5=CN, 1.=BE, 0.=FE
*  -rf : refactor frequency
*  -go : run, halt, og=open graphics
*  -bc : d=dirichlet, a=dirichlet+axisymmetric, r=real BC's  
*  -ad2 : turn on or off the 2nd order artificial dissipation 
* 
* Examples: (offsetSquare grids are made with rectangle.cmd)
* 
*  cgins axisym -g=offsetSquare5.hdf -bg=rectangle -nu=.1 -tf=.2 -tp=.01 -model=ins -tz=poly -project=0 
*  cgins axisym -g=offsetNonSquare5.hdf -bg=rectangle -nu=.1 -tf=.2 -tp=.01 -model=ins -tz=poly -project=0 -degreex=1 -degreet=0
*  cgins axisym -g=offsetSquare5.hdf -bg=rectangle -nu=.1 -tf=.2 -tp=.01 -model=boussinesq -tz=poly -project=0 
*  cgins axisym -g=offsetSquare20.hdf -nu=.1 -tf=2. -tp=.1 -model=ins -tz=poly -project=0 
*  cgins axisym -g=squareOnAxis20 -nu=.1 -tf=2. -tp=.1 -model=ins -tz=poly -project=0 
* 
*  cgins axisym -g=squareOnAxis5 -bg=rectangle -nu=.1 -tf=.2 -tp=.01 -model=ins -tz=poly -project=0 
*        shortHalfCylinder grids made in the ogen script axisym.cmd
*  cgins axisym -g=shortHalfCylinder.hdf -nu=.1 -tf=2. -tp=.1 -model=boussinesq -tz=trig -project=0
*  cgins axisym -g=shortHalfCylinder1.hdf -nu=.1 -tf=2. -tp=.1 -model=boussinesq -tz=trig -project=0
*  cgins axisym -g=shortHalfCylinder2.hdf -nu=.1 -tf=2. -tp=.1 -model=boussinesq -tz=trig -project=0
* 
* implicit and boussinesq: 
* cgins axisym -g=offsetSquare5 -bg=rectangle -nu=.1 -tf=.2 -tp=.01 -model=boussinesq -tz=poly -project=0 -ts=im -dtMax=.01 -iv=viscous -degreex=2 -degreet=1  [exact -- bug fixed]
* cgins axisym -g=squareOnAxis5 -bg=rectangle -nu=.1 -tf=.2 -tp=.01 -model=boussinesq -tz=poly -project=0 -ts=im -dtMax=.01 -iv=viscous -degreex=2 -degreet=1 [exact]
* cgins axisym -g=squareOnAxis5 -bg=rectangle -nu=.1 -tf=.2 -tp=.01 -model=boussinesq -tz=poly -project=0 -ts=im -dtMax=.01 -iv=full -degreex=2 -degreet=1 [exact]
*
*  cgins noplot axisym -g=offsetSquare5.hdf -bg=rectangle -nu=.1 -tf=.2 -tp=.1 -dtMax=.1 -model=ins -tz=poly -project=0 -ts=im -bc=d -degreex=2 -degreet=1 -go=go  [exact]
*  ccgins noplot axisym -g=squareOnAxis5.hdf -bg=rectangle -nu=.1 -tf=.2 -tp=.1 -dtMax=.1 -model=ins -tz=poly -project=0 -ts=im -bc=a -degreex=2 -degreet=1 -go=go -debug=15 [exact]
*  cgins axisym -g=shortHalfCylinder.hdf -nu=.1 -tf=2. -tp=.1 -dtMax=.1 -model=ins -tz=trig -project=0 -ts=im
* implicit boussinesq: 
*  cgins noplot axisym -g=offsetSquare5.hdf -bg=rectangle -nu=.1 -tf=.2 -tp=.1 -dtMax=.1 -model=boussinesq -tz=poly -project=0 -ts=im -bc=d -degreex=2 -degreet=1 -go=go -debug=3
*  cgins noplot axisym -g=squareOnAxis5 -bg=rectangle -nu=.1 -tf=.2 -tp=.1 -dtMax=.1 -model=boussinesq -tz=poly -project=0 -ts=im -bc=a -degreex=2 -degreet=1 -go=go -debug=3
*  cgins axisym -g=shortHalfCylinder.hdf -nu=.1 -tf=2. -tp=.1 -dtMax=.1 -model=boussinesq -tz=trig -project=0 -ts=im
* 
* real runs: 
*  cgins axisym -g=shortHalfCylinder.hdf -nu=.1 -tf=20. -tp=.5 -model=ins -bc=r   
*  cgins axisym -g=shortHalfCylinder2.hdf -nu=1.e-4 -tf=20. -tp=.5 -model=ins -bc=r -ad2=1 
*  cgins axisym -g=shortHalfCylinder2.hdf -nu=.1 -tf=2. -tp=.1 -model=boussinesq -bc=r   
*  cgins axisym -g=shortHalfCylinder2.hdf -nu=.1 -tf=10. -tp=.5 -dtMax=.1 -model=boussinesq -bc=r -ts=im -rf=10 -imp=1. -ad2=1
*  cgins axisym -g=shortHalfCylinder4.hdf -nu=.05 -tf=10. -tp=.1 -dtMax=.1 -model=boussinesq -bc=r -ts=im -rf=10 -imp=1.
*  cgins axisym -g=halfCylinder.hdf -tf=2. -tp=.1 -model=ins  -bc=r   
*  cgins axisym -g=halfCylinder.hdf -tf=20. -tp=.5 -dtMax=.1 -model=ins  -bc=r -ts=im -rf=10 -imp=1.
*  cgins axisym -g=halfCylinder4.hdf -tf=20. -tp=.5 -dtMax=.1 -model=ins -nu=.01  -bc=r -ts=im -rf=50 -imp=1.
*  cgins axisym -g=halfCylinder4.hdf -tf=20. -tp=.5 -dtMax=.1 -model=ins -nu=1.e-5 -ad2=1 -ad22=5. -bc=r -ts=im -rf=20 -imp=1.
*  cgins axisym -g=halfCylinder.hdf -tf=2. -tp=.1 -model=boussinesq -bc=r  
*  cgins axisym -g=halfCylinder.hdf -tf=2. -tp=.005 -tz=poly -model=ins -bc=r 
* 
* NOTE: For TZ flow -- the (u,v) is NOT div free for axisymmetric! --> turn off div-damping 
* --- set default values for parameters ---
* 
$grid="halfCylinder.hdf"; $backGround="square"; 
$tFinal=1.; $tPlot=.1; $cfl=.9; $nu=.05; $Prandtl=.72; $thermalExpansivity=.1; 
$gravity = "1. 0. 0.";   # NOTE: gravity must be in the x-direction for axisymmetric
* $gravity = "0. 0. 0."; 
$model="ins"; $ts="pc"; $noplot=""; $implicitVariation="full"; $refactorFrequency=100; 
$debug = 0;   $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$solver="yale"; $ogesDebug=0; $project=1; $cdv=1.; $ad2=0; $ad22=2.; 
$bc="a"; 
* 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
  "bc=s"=>\$bc );
* -------------------------------------------------------------------------------------------------
$kThermal=$nu/$Prandtl; 
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
* 
* 
if( $ts eq "fe" ){ $ts="forward Euler";}
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "mid"){ $ts="midpoint"; }  
* 
if( $implicitVariation eq "viscous" ){ $implicitVariation = "useNewImplicitMethod\n implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "useNewImplicitMethod\n implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "useNewImplicitMethod\n implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $project eq "1" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
* specify the overlapping grid to use:
$grid
* Specify the equations we solve:
  $model
  define real parameter kappa $kThermal
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  done
* 
* -- twilightzone options:
  $tz
  degree in space $degreex
  degree in time $degreet
  frequencies (x,y,z,t)   $fx $fy $fz $ft
* 
* choose the time stepping:
  $ts
* 
  * useNewImplicitMethod
  $implicitVariation
  use full implicit system $useFullSystem
  refactor frequency $refactorFrequency
  choose grids for implicit
    all=implicit
*     square=explicit
    done
  final time $tFinal
  times to plot $tPlot
  cfl $cfl
  dtMax $dtMax
* 
* 
  plot and always wait
  * no plotting
  pde parameters
    nu $nu
    kThermal $kThermal
    gravity
      $gravity
* 
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22  $ad22, $ad22
    OBPDE:divergence damping  $cdv 
  done
* 
 turn on axisymmetric flow
* 
  boundary conditions
    all=slipWall
    square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)
    square(1,0)=outflow
    bcNumber13=axisymmetric
    annulus(0,1)=noSlipWall, uniform(T=1.)
* 
    if( $bc eq "d" ){ $cmd = "all=dirichletBoundaryCondition\n done"; }\
    elsif( $bc eq "a" ){ $cmd = "all=dirichletBoundaryCondition\n bcNumber13=axisymmetric\n done"; }\
    else{ $cmd = "done"; }
    $cmd
* 
  initial conditions
   if( $tz eq "turn off twilight zone" ){ $ic = "uniform flow\n p=1., u=1., T=0. \n done";}else{ $ic = "done"; }
   $ic 
  done
  debug $debug
  $project
  exit
  $go
