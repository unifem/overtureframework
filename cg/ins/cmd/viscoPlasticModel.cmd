*
* cgins: Test the visco-plastic model with twilight-zone
*
* Usage: 
*    cgins [noplot] viscoPlasticModel -g=<name> -degreex=<> -degreet=<> -eta=<val> -yield=<val> -expVP=<val>...
*                  -its=<max its> -pits=<tPlot> -ic=<uniform/tz> -ad2=[0/1] -bg=<name> -ts=[pc/line/imp] ...
*                  -tp=<val> -tf=<val> -kThermal=<val> -iv=[viscous/adv/full] -imp=<val> -rf=<val> -dtMax=<val>...
*                  -debug=<> -show=<name> -ad2=<0|1> -bg=<grid-name> -cfl=<num> -solver=<yale/best> -go=[run/halt/og] 
*
* -eta = etaVP in equation below
* -yield = yield-stress, yieldStressVP in equation below. Set to zero for a Newtonian fluid model. 
* -expVP = exponentVP in the equation below
* -ts : times-stepping method, pc: adams predictor-corrector, line: line-solver, imp: implicit
* -rf : refactor frequency
* 
* Visco-plastic viscosity is: 
*   nuT = etaVP + (yieldStressVP/esr)*(1.-exp(-exponentVP*esr))
*   esr = effective strain rate = || (2/3)*eDot_ij ||
* 
* Examples:
*
* Explicit time-stepping:
* cgins noplot viscoPlasticModel -g=square5.hdf -degreex=2 -degreet=1 -eta=1. -yield=0. -ic=tz -ts=pc -tp=.02 -tf=.04 -debug=1 -go=go [exact]
*  
* Full implicit method:
* cgins noplot viscoPlasticModel -g=square5.hdf -degreex=2 -eta=1. -yield=0. -ic=tz -ts=imp -tp=.02 -tf=.04 -iv=full -debug=1 -go=go -dtMax=.02  [exact]
* cgins noplot viscoPlasticModel -g=square5.hdf -degreex=2 -eta=1. -yield=0. -ic=tz -ts=imp -tp=.02 -tf=.04 -iv=full -debug=15 -go=go
* cgins noplot viscoPlasticModel -g=nonSquare5.hdf -degreex=2 -eta=1. -yield=0. -ic=tz -ts=imp -tp=.02 -tf=.04 -iv=full -debug=15 -go=go
* cgins noplot viscoPlasticModel -g=rotatedSquare10.hdf -degreex=2 -eta=1. -yield=0. -ic=tz -ts=imp -tp=.02 -tf=.04 -iv=full -debug=15 -go=go
* cgins noplot viscoPlasticModel -g=sis.hdf -degreex=2 -eta=1. -yield=0. -ic=tz -ts=imp -tp=.02 -tf=.04 -iv=full -debug=1 -go=go
* cgins noplot viscoPlasticModel -g=rsis2.hdf -degreex=2 -eta=1. -yield=0. -ic=tz -ts=imp -tp=.02 -tf=.04 -iv=full -debug=1 -go=go
* cgins viscoPlasticModel -g=cice1.order2.hdf -eta=1. -yield=0. -ts=imp -tp=1. -tf=100. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.1 -rf=20 -tz=trig -cDt=.5
* cgins viscoPlasticModel -g=cice2.order2.hdf -eta=1. -yield=0. -ts=imp -tp=1. -tf=100. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.1 -rf=20 -tz=trig
* cgins viscoPlasticModel -g=cice4.order2.hdf -eta=1. -yield=0. -ts=imp -tp=1. -tf=100. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.1 -rf=20 -tz=trig -cDt=.5 
* 3D: 
* cgins viscoPlasticModel -g=box5.hdf -bg=box -degreex=2 -eta=1. -yield=0. -ic=tz -ts=imp -tp=.1 -dtMax=.1 -tf=1. -iv=full -debug=1 -go=halt
* cgins viscoPlasticModel -g=nonBox5.hdf -bg=box -degreex=2 -eta=1. -yield=0. -ic=tz -ts=imp -tp=.1 -dtMax=.1 -tf=1. -iv=full -debug=1 -go=halt
* cgins viscoPlasticModel -g=rotatedBox1.order2 -bg=box -degreex=2 -eta=1. -yield=0. -ic=tz -ts=imp -tp=.1 -dtMax=.1 -tf=1. -iv=full -debug=1 -go=halt
* cgins viscoPlasticModel -g=bibe.hdf -bg="outer-box" -degreex=2 -eta=1. -yield=0. -ic=tz -ts=imp -tp=.1 -dtMax=.1 -tf=1. -iv=full -debug=1 -go=halt
* 
* -- nonlinear viscosity
* cgins viscoPlasticModel -g=square20.hdf -eta=1. -yield=1. -expVP=2. -ts=imp -tp=1. -tf=100. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.1 -rf=10 -tz=trig
* cgins viscoPlasticModel -g=square40.hdf -eta=1. -yield=1. -expVP=2. -ts=imp -tp=1. -tf=100. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.1 -rf=20 -tz=trig
* cgins viscoPlasticModel -g=rotatedSquare20.hdf -eta=1. -yield=1. -expVP=2. -ts=imp -tp=1. -tf=100. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.05 -rf=20 -tz=trig
* 
* cgins viscoPlasticModel -g=rsis2.hdf -eta=1. -yield=1. -expVP=2. -ts=imp -tp=1. -tf=100. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.05 -rf=10 -tz=trig
* 
* cgins viscoPlasticModel -g=quarterAnnulus.hdf -eta=1. -yield=1. -expVP=2. -ts=imp -tp=1. -tf=100. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.05 -rf=20 -tz=trig
* cgins viscoPlasticModel -g=cice1.order2.hdf -eta=1. -yield=1. -expVP=2. -ts=imp -tp=1. -tf=100. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.1 -rf=20 -tz=trig 
*  3D 
* cgins viscoPlasticModel -g=box10 -bg=box -eta=1. -yield=1. -expVP=2. -ts=imp -tp=1. -tf=100. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.1 -rf=20 -tz=trig
* cgins viscoPlasticModel -g=rotatedBox1.order2 -bg=box -eta=1. -yield=1. -expVP=2. -ts=imp -tp=1. -tf=5. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.1 -rf=20 -tz=trig
* cgins viscoPlasticModel -g=quarterCyl2.order2 -bg=quarterCylinder -eta=1. -yield=1. -expVP=2. -ts=imp -tp=.1 -tf=5. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.1 -rf=20 -tz=trig
* cgins viscoPlasticModel -g=orthoSphere1.order2 -bg="north-pole" -eta=1. -yield=1. -expVP=2. -ts=imp -tp=.1 -tf=5. -imp=1. -iv=full -debug=1 -go=halt -ic=tz -dtMax=.1 -rf=20 -tz=trig
* 
* Line solver (?) 
*  cgins noplot viscoPlasticModel -g=square5.hdf -degreex=2 -eta=1. -yield=0. -ic=tz -pits=1 -its=1 -debug=15 -go=go
*  cgins viscoPlasticModel -g=nonSquare5.hdf -degreex=2 -eta=1. -yield=0. -debug=1
*  cgins viscoPlasticModel -g=sis.hdf -degreex=2 -eta=1. -yield=0. -ic=tz 
*
*  cgins viscoPlasticModel -g=square5.hdf -degreex=2 -eta=1. -yield=0. -ic=tz -debug=15
*  cgins noplot viscoPlasticModel -g=square40.hdf -degreex=2 -eta=1. -yield=1. -expVP=2. -ic=tz -pits=1 -its=1 -go=run
*  cgins noplot viscoPlasticModel -g=square5.hdf -degreex=1 -eta=1. -yield=0. -tp=.01 -tf=.01 -ic=tz -ts=pc -go=go -debug=15
*  cgins noplot viscoPlasticModel -g=rhombus10.hdf -degreex=1 -eta=1. -yield=0. -tp=.01 -tf=.01 -ic=tz -ts=pc -go=go -debug=15
* 
* ok: 
*  cgins noplot viscoPlasticModel -g=rotatedSquare10.hdf -degreex=2 -eta=1. -yield=1. -expVP=2. -ic=tz -pits=1 -its=10
*  cgins viscoPlasticModel -g=sise4.order2.hdf -degreex=2 -eta=1. -yield=1. -expVP=2. -ic=tz -pits=1 -its=10
*  cgins noplot viscoPlasticModel -g=rhombus10.hdf -degreex=0 -eta=1. -yield=0. -expVP=2. -ic=tz -pits=1 -its=1 -debug=15 -go=go
*  cgins viscoPlasticModel -g=rhombus20.hdf -degreex=0 -eta=1. -yield=0. -expVP=2. -ic=tz -pits=1 -its=100
*  cgins viscoPlasticModel -g=quarterAnnulus.hdf -degreex=2 -eta=1. -yield=0. -expVP=2. -ic=tz -pits=1 -its=100
*  cgins viscoPlasticModel -g=quarterAnnulus2.hdf -degreex=2 -eta=1. -yield=0. -expVP=2. -ic=tz -pits=1 -its=100
* 
*  cgins noplot viscoPlasticModel -g=annulus2.hdf -degreex=2 -eta=1. -yield=1. -expVP=2. -ic=tz -pits=1 -its=100 -go=run
*  cgins noplot viscoPlasticModel -g=cice2.order2.hdf -degreex=2 -eta=1. -yield=1. -expVP=2. -ic=tz -pits=1 -its=1 -go=run
* 
* Convergence tests with trigonometric solution:
*  cgins noplot viscoPlasticModel -g=square20.hdf -eta=1. -yield=1. -expVP=2. -ic=tz -pits=100 -its=400 -tz=trig -go=go >! vp.sq20.out &
*  cgins noplot viscoPlasticModel -g=square40.hdf -eta=1. -yield=1. -expVP=2. -ic=tz -pits=100 -its=600 -tz=trig -go=go >! vp.sq40.out  &
*  cgins viscoPlasticModel -g=cice1.order2.hdf -eta=1. -yield=1. -expVP=2. -ic=tz -pits=100 -its=500  -tz=trig
*  cgins viscoPlasticModel -g=cice2.order2.hdf -eta=1. -yield=1. -expVP=2. -ic=tz -pits=100 -its=500 -tz=trig
*  cgins viscoPlasticModel -g=cice4.order2.hdf -eta=1. -yield=1. -expVP=2. -ic=tz -pits=100 -its=500 -tz=trig
* 
* --- set default values for parameters ---
$debug=1; $tFinal=1.; $tPlot=.1; $nuVP=.0; $etaVP=0.1; $yieldStressVP=1.; $exponentVP=1.; $kThermal=1.; $epsVP=1.e-3; 
$cfl=.9; $ts="line";  $implicitFactor=.5; $refactorFrequency=100; $numberOfCorrections=1; 
$implicitVariation="full"; #  $implicitVariation="full"; 
$cDt=-1.; # this means use the default given below. old:  $cDt=.25; $advectionCoefficient=1.; 
$its=100; $pits=1; $dtMax=.1; $gravity = "0. 0. 0."; $fx=.5; $ft=0.;
$tz="poly"; $degreex=2; $degreet=0; 
$ic ="uniform"; $ad2=0; $ad21=1.; $ad22=1.; $go="halt"; $bg=square; 
$psolver="yale"; $solver="yale"; $rtol=1.e-8; $atol=.1e-9; 
* $psolver="choose best iterative solver"; $solver="choose best iterative solver";
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions("g=s"=>\$grid,"its=i"=> \$its,"pits=i"=> \$pits,"cfl=f"=>\$cfl,"debug=i"=> \$debug,"dtMax=f"=>\$dtMax, \
           "degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "show=s"=>\$show, "bg=s"=>\$bg, "noplot=s"=>\$noplot, \
           "solver=s"=>\$solver, "model=s"=>\$model, "gravity=s"=>\$gravity, "ic=s"=>\$ic, "ts=s"=>\$ts,\
           "ad2=i"=> \$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22,"advectionCoefficient=f"=>\$advectionCoefficient,\
           "tp=f"=>\$tPlot,"tf=f"=>\$tFinal,"kThermal=f"=>\$kThermal,"tz=s"=>\$tz,"imp=f"=>\$implicitFactor,\
           "iv=s"=>\$implicitVariation,"eta=f"=>\$etaVP,"yield=f"=>\$yieldStressVP,"expVP=f"=>\$exponentVP,\
           "rf=i"=> \$refactorFrequency,"ad2=i"=> \$ad2,"bg=s"=>\$bg,"gravity=s"=>\$gravity,"fx=f"=>\$fx,\
           "cDt=f"=>\$cDt,"epsVP=f"=>\$epsVP,"nc=i"=> \$numberOfCorrections,"go=s"=>\$go );
* -------------------------------------------------------------------------------------------------
if( $ic eq "tz" ){ $ic="initial conditions\n exit";}else\
                 { $ic="initial conditions\n uniform flow\n  p=0., u=0., v=0., T=0.\n exit";}
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
*
* -- here is the grid we use: 
$grid
*
  incompressible Navier Stokes
  visco-plastic model
  define real parameter nuViscoPlastic $nuVP
  define real parameter etaViscoPlastic $etaVP
  define real parameter yieldStressViscoPlastic $yieldStressVP
  define real parameter exponentViscoPlastic $exponentVP 
  define real parameter epsViscoPlastic $epsVP
  define integer parameter maximumNumberOfLinesToSolveAtOneTime 100 3 100 3
  exit
*
* define the time-stepping method:
  $ts
  first order predictor
  number of PC corrections $numberOfCorrections
* 
  dtMax $dtMax
* -- assign the coefficient of the advection terms: (1=NS, 0=Stokes)
  advectionCoefficient $advectionCoefficient
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
    nu  1.
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
**
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
 all=noSlipWall
 $bg(0,1)=noSlipWall
 $bg(1,1)=noSlipWall
 $bg(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)
 $bg(1,0)=outflow , pressure(1.*p+1.*p.n=0.)
 all=dirichletBoundaryCondition
 * $bg(0,0)=noSlipWall
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
