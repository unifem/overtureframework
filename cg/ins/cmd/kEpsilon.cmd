*
* cgins: Test the K-Epsilon Turbulence model with twilight-zone
*
* Usage: 
*    cgins [noplot] kEpsilon -g=<name> -degreex=<> -degreet=<> -oges=[d/i]...
*                      -its=<max its> -pits=<tPlot> -ic=<uniform/tz> -ad2=[0/1] -bg=<name> -ts=[pc/line/imp] ...
*                      -tp=<val> -tf=<val> -kThermal=<val> -iv=[viscous/adv/full] -imp=<val> -rf=<val> -dtMax=<val>...
*                      -debug=<> -show=<name> -bg=<grid-name> -cfl=<num> -solver=<yale/best> -go=[run/halt/og] 
*
* -ts : times-stepping method, pc: adams predictor-corrector, line: line-solver, imp: implicit
* -rf : refactor frequency
* -oges : d=direct solver, i=iterative solver
* 
* Examples:
*
* Full implicit method:
* cgins kEpsilon -g=square5.hdf -degreex=1 -ic=tz -ts=imp -dtMax=.02 -tp=.02 -tf=.04 -iv=full -debug=1 -go=halt 
* cgins kEpsilon -g=nonSquare5.hdf -degreex=1 -ic=tz -ts=imp -dtMax=.02 -tp=.02 -tf=.04 -iv=full -debug=1 -go=halt 
* cgins kEpsilon -g=rotatedSquare5.hdf -degreex=1 -ic=tz -ts=imp -dtMax=.02 -tp=.02 -tf=.04 -iv=full -debug=1 -go=halt 
* cgins kEpsilon -g=sbs.hdf -degreex=1 -ic=tz -ts=imp -dtMax=.02 -tp=.02 -tf=.04 -iv=full -debug=1 -go=halt 
* cgins kEpsilon -g=sis0.hdf -degreex=1 -ic=tz -ts=imp -dtMax=.02 -tp=.02 -tf=.04 -iv=full -debug=1 -go=halt 
* cgins kEpsilon -g=rsis2.hdf -degreex=1 -ic=tz -ts=imp -dtMax=.02 -tp=.02 -tf=.04 -iv=full -debug=1 -go=halt 
* 
* cgins noplot kEpsilon -g=square5.hdf -degreex=2 -ic=tz -ts=imp -tp=.02 -tf=.04 -iv=full -debug=1 -go=go -dtMax=.02
* 
* -- trig: convergence to steady state: 
* cgins kEpsilon -g=square20.hdf -ic=tz -tz=trig -ts=imp -dtMax=.1 -tp=.5 -tf=10. -iv=full -debug=1 -go=halt 
* cgins kEpsilon -g=rsis2.hdf -ic=tz -tz=trig -ts=imp -dtMax=.1 -tp=.5 -tf=10. -iv=full -debug=1 -go=halt 
* cgins kEpsilon -g=cice1.order2 -ic=tz -tz=trig -ts=imp -dtMax=.1 -tp=.5 -tf=10. -iv=full -debug=1 -go=halt 
* 
* 3D : 
* cgins kEpsilon -g=box5.hdf -degreex=1 -ic=tz -ts=imp -dtMax=.02 -tp=.02 -tf=.04 -iv=full -debug=1 -go=halt 
* cgins kEpsilon -g=rotatedBox1.order2 -degreex=1 -ic=tz -ts=imp -dtMax=.02 -tp=.02 -tf=.04 -iv=full -debug=1 -go=halt 
* cgins kEpsilon -g=rbibe1.order2 -degreex=1 -ic=tz -ts=imp -dtMax=.05 -tp=.05 -tf=.10 -iv=full -debug=1 -oges=i -go=halt 
*
* 
* --- set default values for parameters ---
$debug=1; $tFinal=1.; $tPlot=.1; $nu=1.;
$cfl=.9; $ts="line";  $implicitFactor=1.; $refactorFrequency=100; $numberOfCorrections=1; 
$implicitVariation="full"; #  $implicitVariation="full"; 
$cDt=-1.; # this means use the default given below. old:  $cDt=.25;
$its=100; $pits=1; $dtMax=.1; $gravity = "0. 0. 0."; $fx=.5; $ft=0.;
$tz="poly"; $degreex=2; $degreet=0; 
$ic ="uniform"; $ad2=0; $ad21=1.; $ad22=1.; $go="halt"; $bg=square; 
$oges="direct"; $psolver="yale"; $solver="yale"; $rtol=1.e-8; $atol=.1e-9; 
* $psolver="choose best iterative solver"; $solver="choose best iterative solver";
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions("g=s"=>\$grid,"its=i"=> \$its,"pits=i"=> \$pits,"cfl=f"=>\$cfl,"debug=i"=> \$debug,"dtMax=f"=>\$dtMax, \
           "degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "show=s"=>\$show, "bg=s"=>\$bg, "noplot=s"=>\$noplot, \
           "solver=s"=>\$solver, "model=s"=>\$model, "gravity=s"=>\$gravity, "ic=s"=>\$ic, "ts=s"=>\$ts,\
           "tp=f"=>\$tPlot,"tf=f"=>\$tFinal,"nu=f"=>\$nu,"tz=s"=>\$tz,"imp=f"=>\$implicitFactor,\
           "iv=s"=>\$implicitVariation,"oges=s"=>\$oges,\
           "rf=i"=> \$refactorFrequency,"ad2=i"=> \$ad2,"bg=s"=>\$bg,"gravity=s"=>\$gravity,"fx=f"=>\$fx,\
           "cDt=f"=>\$cDt,"nc=i"=> \$numberOfCorrections,"go=s"=>\$go );
* -------------------------------------------------------------------------------------------------
if( $ic eq "tz" ){ $ic="initial conditions\n exit";}else\
                 { $ic="initial conditions\n uniform flow\n  p=0., u=0., v=0., T=0., k=1., eps=1. \n exit";}
* 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $ts eq "line" ){ $ts="steady state RK-line"; }
if( $ts eq "imp" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $tz eq "poly" ){ $tz="turn on polynomial"; }
if( $tz eq "trig" ){ $tz="turn on trigonometric"; }
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $cDt<0. ){ $cDt= .2/$dtMax; }
if( $oges eq "i" ){ $psolver="choose best iterative solver"; $solver="choose best iterative solver"; }
*
* -- here is the grid we use: 
$grid
*
  incompressible Navier Stokes
  k-epsilon
*   Baldwin-Lomax
*  define real parameter nuViscoPlastic $nuVP
  define integer parameter maximumNumberOfLinesToSolveAtOneTime 100 3 100 3
  exit
*
* define the time-stepping method:
  $ts
  first order predictor
  number of PC corrections $numberOfCorrections
* 
  dtMax $dtMax
* 
*
  max iterations $its
  plot iterations $pits
  plot residuals 1
* 
  final time $tFinal
  times to plot $tPlot
* 
  useNewImplicitMethod
  refactor frequency $refactorFrequency
* 
  implicit factor $implicitFactor 
  use full implicit system 1
  $implicitVariation
* 
*   turn on polynomial
  $tz
* 
  degree in space $degreex
  degree in time $degreet
  OBTZ:frequencies (x,y,z,t) $fx, $fx, $fx, $ft
* 
  * plot and always wait
  no plotting
  pde parameters
    nu  $nu
    kThermal $kThermal
    gravity
      $gravity
    cDt div damping $cDt
    * now default: use Neumann BC at outflow
   done
  cfl $cfl
**
  OBPDE:second-order artificial diffusion $ad2
  OBPDE:ad21,ad22  $ad21 $ad22
*  OBPDE:ad21,ad22  0. 0. 
********
  * OBPDE:fourth-order artificial diffusion 
  OBPDE:use implicit fourth-order artificial diffusion 0
**  OBPDE:ad41,ad42 1,1
********
  pressure solver options
     $psolver
     * yale
     * these tolerances are chosen for PETSc
     relative tolerance
       $rtol 
     absolute tolerance
       $atol
    exit
  implicit time step solver options
     * choose best iterative solver
     $solver
     * PETSc
     * these tolerances are chosen for PETSc
     relative tolerance
       $rtol
     absolute tolerance
       $atol
     * debug 
     *   3
    exit
* 
  boundary conditions
    all=dirichletBoundaryCondition
*  square(0,0)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
*  square(1,0)=noSlipWall
*     box(0,0)=noSlipWall
*     box(1,1)=noSlipWall
*     box(1,2)=noSlipWall
*  all=noSlipWall
* ---
* all=noSlipWall
* $bg(0,1)=noSlipWall
* $bg(1,1)=noSlipWall
*  $bg(1,1)=slipWall
* $bg(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
* $bg(1,0)=outflow , pressure(1.*p+1.*p.n=0.)
* all=dirichletBoundaryCondition
* $bg(0,1)=noSlipWall
* $bg(1,1)=noSlipWall
* all=noSlipWall
***
* ----
   done
  debug  $debug
*
  $ic 
  check error on ghost 
    0 1
 continue
$go


 movie mode 
 finish
