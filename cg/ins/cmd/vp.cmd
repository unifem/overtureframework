*
* cgins: Test the visco-plastic model 
*
* Usage: 
*    cgins [noplot] viscoPlasticModel -g=<name> -eta=<val> -yield=<val> -expVP=<val> -kThermal=<val>...
*                      -its=<> -pits=<> -ad2=[0/1] -bg=<name> -d=<num> -ts=[pc/line/imp] -bc=[old|new] ...
*                      -debug=<> -show=<name> -bg=<grid-name> -cfl=<num> -solver=<yale/best> -nc=<> ...
*                      -iluLevels=<> -move=[on|off] -advectionCoefficient=<> -pGrad=<> -go=[run/halt/og]
*
* -eta = etaVP in equation below
* -yield = yield-stress, yieldStressVP in equation below. Set to zero for a Newtonian fluid model. 
* -expVP = exponentVP in the equation below
* -d : with of parabolic inflow 
* -ts : times-stepping method, -ts=pc: adams predictor-corrector, -ts=line: line-solver
* -nc : number of correction steps for the Adams PC time stepping
* -ad : -ad=1 : turn on second order artificial dissipation
* -advectionCoefficient : the coefficient of the advection terms: (1=NS, 0=Stokes)
* -pGrad=<val> : impose a pressure gradient (for periodic channels)
* -uInflow=<val> : inflow velocity
* -vMove=<val> : speed of the moving grid
* 
* Visco-plastic viscosity is: 
*   nuT = etaVP + (yieldStressVP/esr)*(1.-exp(-exponentVP*esr))
*   esr = effective strain rate = || (2/3)*eDot_ij ||
* 
* Examples:
*   Note: Ramp up parameters expVP ...
*
* implicit: (use vpCylGrid.cmd to generate grids)
*  cgins vp -g=vpCylGridi2.order2.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=1. -tf=100. -bc=old
*  cgins vp -g=vpCylGridi4.order2.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=10 -tp=1. -tf=100. -bc=old
* -- periodic domain with an imposed pressure gradient : 
*  cgins vp -g=vpCylGridi1p.order2.hdf -eta=.5 -yield=1. -expVP=2. -d=1. -dtmax=.05 -ts=imp -rf=10 -tp=.2 -psolver=best -tf=2. -bc=old -pGrad=2. -show="vpCyl.show"
*  cgins vp -g=vpCylGridi2p.order2.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -dtmax=.05 -ts=imp -rf=10 -tp=.2 -psolver=best -tf=2. -bc=old -pGrad=2. -show="vpCyl.show"
* 
*  cgins vp -g=cice1.order2.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=1. -tf=100.
*  cgins vp -g=cice2L8.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=1. -tf=100.
*  cgins vp -g=cice4L8.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=1. -tf=100.
* 
* cgins noplot vp -g=vpCylGridi2.order2.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=1. -tf=5. -show=vpCyl2d.show -go=go > ! vpCyl2d.out &
* 
* -- moving wall
*  cgins vp -g=vpCylGridi2.order2.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=10 -tp=1. -tf=5. -bc=movingWall -show=vpCylMovingWall.show -go=go >! vpCylMovingWall.out 
* 
* Moving:
*   cgins noplot vp -g=vpCylGridi2.order2.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -dtmax=.05 -ts=imp -rf=1 -tp=.2 -psolver=best -tf=2. -move=on -bc=old -uInflow=0. -show="vpMove.show" -go=go >! vpMove.out 
* --periodic domain with an imposed pressure gradient : 
*   cgins vp -g=vpCylGridi1p.order2.hdf -eta=.5 -yield=1. -expVP=2. -d=1. -dtmax=.05 -ts=imp -rf=1 -tp=.2 -psolver=best -tf=5. -move=on -bc=old -pGrad=2. -show="vpMove.show" -go=go >! vpMove.out 
*   cgins vp -g=vpCylGridi2p.order2.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -dtmax=.01 -ts=imp -rf=1 -tp=.2 -psolver=best -tf=2. -move=on -bc=old -pGrad=2. -show="vpMove.show" -go=go >! vpMove.out 
* 
*  -- moving starting with an offset cylinder 
*   cgins noplot vp -g=vpCylGridi2Offset2.order2.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -dtmax=.025 -ts=imp -rf=1 -tp=.2 -psolver=best -tf=2. -move=on -bc=old -uInflow=0. -show="vpMove.show" -go=go >! vpMove.out 
*   cgins vp -g=vpCylGridi4Offset2.order2.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -dtmax=.025 -ts=imp -rf=1 -tp=.2 -psolver=best -tf=2. -move=on -bc=old -uInflow=0.
* 
* --- 3D --- (use vpCylGrid3d.cmd to generate grids)
*  cgins vp -g=sibi1.order2 -bg=box -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=1. -tf=100.
*  cgins noplot vp -g=sibi2.order2 -bg=box -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=.2 -tf=4. -psolver=best -go=go -dtMax=.05 -bc=old -show=vpSib2.show >! vpSib2.out & 
*  -- the next requires about 2.2G : 
*  cgins noplot vp -g=sibi3.order2 -bg=box -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=.2 -tf=2. -psolver=best -go=go -dtMax=.05 -bc=old -iluLevels=0 -show=vpSib3.show >! vpSib3.out & 
*  -- the next requires about 5.1G (540K pts)
*  cgins noplot vp -g=sibi4.order2 -bg=box -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=.2 -tf=2. -psolver=best -go=go -dtMax=.05 -bc=old -iluLevels=0 -show=vpSib4.show >! vpSib4.out & 
* 
*  cgins vp -g=vpCylGrid3di2.order2.hdf -bg=box -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=1. -tf=100.
* cgins noplot vp -g=vpCylGrid3di2.order2.hdf -bg=box -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=1. -tf=5. -psolver=best -show=vpCyl3d.show -go=go > ! vpCyl3d.out &
* 
*  -- sphere in a tube -- 
*   N.B. reduce solver tolerances to get better convergence (Not sure if only pressure tol's need to be reduced?)
* cgins vp -g=vpSphereInATubei1.order2 -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=.2 -tf=5. -psolver=best 
*  This next grid uses 4.0g 
* cgins noplot vp -g=vpSphereInATubei2.order2 -iluLevels=0 -dtMax=.05 -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=10 -tp=.2 -tf=2. -psolver=best -rtol=1.e-6 -atol=1.e-7 -show=vpSIT2.show -go=go > ! vpSIT2.out &
*
*  This next grid has 1.2M pts and uses ?G 
* cgins noplot vp -g=vpSphereInATubei3.order2 -iluLevels=0 -dtMax=.05 -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=10 -tp=.2 -tf=2. -psolver=best -rtol=1.e-6 -atol=1.e-7 -show=vpSIT3.show -go=go > ! vpSIT3.out &
*
* ---- parallel (requires grids to be made with explicit interpolation)------
* parallel: 
* mpirun -np 1 -gdb $cginsp vp -g=vpCylGride2.order2.hdf -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=1. -tf=100. -bc=old -solver=best -psolver=best
* mpirun -np 1 $cginsp vp -g=vpSphereInATubee1.order2 -eta=.5 -yield=1. -expVP=5. -d=1. -ts=imp -rf=20 -tp=.2 -tf=5. -psolver=best -solver=best
*
* line solve: -- not currently recommended 
*  cgins vp -g=square20.hdf -eta=.5 -yield=1. 
*  cgins vp -g=rect2x1y4.hdf -bg=rectangle -eta=.5 -yield=1. -expVP=2. 
*  cgins vp -g=cice2.order2.hdf -eta=.5 -yield=0. -expVP=2. -d=.5
*  cgins vp -g=cice2L8.hdf -eta=.5 -yield=1. -expVP=10. -d=1. -pits=100
* 
* 
* --- set default values for parameters ---
$debug=1; $tFinal=1.; $tPlot=.1; $nuVP=.0; $etaVP=0.1; $yieldStressVP=1.; $exponentVP=1.; $kThermal=.2; $pGrad=0; 
$cfl=.9; $T0=1.; $numberOfCorrections=1; $move="off"; $vMove=-1.; $uInflow=1.; 
$its=10000; $pits=10; $dtMax=.1; $show=" "; $ts="imp"; $bc="new"; 
$ic ="uniform"; $ad2=0; $ad21=1.; $ad22=1.; $go="halt"; $bg="square"; $d=.25; $advectionCoefficient=1.; 
$implicitVariation="full"; $implicitFactor=1.; $refactorFrequency=20; $cDt=1.;
* use a direct solver:
$psolver="yale"; $solver="yale"; $rtol=1.e-6; $atol=.1e-7; $iluLevels=1;
* use an iterative solver: (in 3d use an iterative solver for the pressure eqn too)
$solver="choose best iterative solver"; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions("g=s"=>\$grid,"its=i"=> \$its,"pits=i"=> \$pits,"cfl=f"=>\$cfl,"debug=i"=> \$debug, \
           "show=s"=>\$show, "bg=s"=>\$bg, "noplot=s"=>\$noplot,"d=f"=>\$d,"cDt=f"=>\$cDt,"rtol=f"=>\$rtol,\
           "atol=f"=>\$atol,"iv=s"=>\$implicitVariation,"ad2=i"=> \$ad2,"ad21=f"=>\$ad21,"ad22=f"=>\$ad22, \
           "tp=f"=>\$tPlot,"tf=f"=>\$tFinal,"kThermal=f"=>\$kThermal,"rf=i"=> \$refactorFrequency,\
           "imp=f"=>\$implicitFactor,"nc=i"=> \$numberOfCorrections,"show=s"=>\$show,"move=s"=>\$move,\
           "solver=s"=>\$solver,"psolver=s"=>\$psolver, "model=s"=>\$model, "gravity=s"=>\$gravity,\
           "ic=s"=>\$ic,"ts=s"=>\$ts,"dtMax=f"=>\$dtMax,"iluLevels=i"=> \$iluLevels,"uInflow=f"=>\$uInflow,\
           "advectionCoefficient=f"=>\$advectionCoefficient,"bc=s"=>\$bc,"pGrad=f"=>\$pGrad, \
           "eta=f"=>\$etaVP,"yield=f"=>\$yieldStressVP,"expVP=f"=>\$exponentVP,"vMove=f"=>\$vMove,"go=s"=>\$go );
* -------------------------------------------------------------------------------------------------
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $ts eq "line" ){ $ts="steady state RK-line"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "imp" ){ $ts="implicit"; }
if( $implicitVariation eq "viscous" ){ $implicitVariation = "implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
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
  define real parameter epsViscoPlastic 1.e-3
  * to save space we only solve this many tridiagonal systems at once: 
  define integer parameter maximumNumberOfLinesToSolveAtOneTime 100  1000
  exit
*
  $ts
  * use a first order predictor until we fix the second-order predictor to take account of 
  * of the linearized solution changing 
  first order predictor
  number of PC corrections $numberOfCorrections
* 
* steady state RK-line
  dtMax $dtMax
* 
* -- assign the coefficient of the advection terms: (1=NS, 0=Stokes)
  advectionCoefficient $advectionCoefficient
*
  max iterations $its
  plot iterations $pits
* 
  final time $tFinal
  times to plot $tPlot
* 
  plot residuals 1
* 
  show file options
     compressed
      open
       $show
    frequency to flush
      5
    exit
* -- specify which variables will appear in the show file:
    showfile options...
    OBPSF:show variable: sigmaxx 1 
    OBPSF:show variable: sigmaxy 1 
    OBPSF:show variable: sigmayy 1 
    close show file options
* 
 turn off twilight zone 
* 
  useNewImplicitMethod
  implicit factor $implicitFactor 
  use full implicit system 1
  $implicitVariation
  refactor frequency $refactorFrequency
* 
  * plot and always wait
  * no plotting
*
***********
if( $move eq "off" ){ $move="turn off moving grids"; }\
else{ $move="turn on moving grids\n specify grids to move\n translate\n 1. 0. 0.\n  $vMove\n annulus\n done\n done"; }
* 
  $move
* 
* Here is were we specify a pressure gradient for flow in a periodic channel:
* This is done by adding a const forcing to the "u" equation 
if( $pGrad != 0 ){ $cmds ="user defined forcing\n constant forcing\n 1 $pGrad\n  done\n exit";}else{ $cmds="*"; }
$cmds
*
  pde parameters
    nu  .1 
    kThermal $kThermal
    cDt div damping $cDt
    * use extrapolate BC at outflow
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
     number of incomplete LU levels
       $iluLevels
     * do these next two work?
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -sub_pc_factor_levels $iluLevels
     relative tolerance
       $rtol 
     absolute tolerance
       $atol
    exit
  implicit time step solver options
     * choose best iterative solver
     $solver
     * 
     number of incomplete LU levels
       $iluLevels
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -sub_pc_factor_levels $iluLevels
     * these tolerances are chosen for PETSc
     relative tolerance
       $rtol
     absolute tolerance
       $atol
     * debug 
     *   3
    exit
*
* 
  boundary conditions
if( $bc eq "old" ){ $bc="all=noSlipWall\n$bg(0,0)=inflowWithVelocityGiven , parabolic(d=$d,p=1.,u=$uInflow,T=$T0),\n$bg(1,0)=outflow , pressure(1.*p+1.*p.n=0.)"; }
if( $bc eq "new" ){ $bc="all=dirichletBoundaryCondition\n bcNumber1=noSlipWall\n bcNumber2=inflowWithVelocityGiven , parabolic(d=$d,p=1.,u=$uInflow,T=$T0),\n bcNumber3=outflow , pressure(1.*p+1.*p.n=0.)"; }
if( $bc eq "movingWall" ){ $bc="all=noSlipWall\n$bg(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=$uInflow,T=$T0),\n$bg(1,0)=outflow , pressure(1.*p+1.*p.n=0.)\n square(0,1)=noSlipWall, uniform(u=$uInflow)\n square(1,1)=noSlipWall, uniform(u=$uInflow)"; }
$bc 
* ----
*   all=noSlipWall
*   $bg(0,0)=inflowWithVelocityGiven , parabolic(d=$d,p=1.,u=1.,T=$T0),
*   $bg(1,0)=outflow , pressure(1.*p+1.*p.n=0.)
*  ---- new way : use BC value: 
*   all=dirichletBoundaryCondition
*   bcNumber1=noSlipWall
*   bcNumber2=inflowWithVelocityGiven , parabolic(d=$d,p=1.,u=1.,T=$T0),
*   bcNumber3=outflow , pressure(1.*p+1.*p.n=0.)
* ----
   done
  debug  $debug
*
  initial conditions
  uniform flow
    p=0., u=$uInflow, v=0., w=0., T=$T0
    exit
  project initial conditions
*
  continue
  $go







*
  continue
  continue
  continue
  continue
  continue
* 
  $numSteps=5; $deltaExp=1.; 
  $commands ="";
  for( $i=0; $i<$numSteps; $i++){\
    $exponentVP=$exponentVP+$deltaExp;\
    $commands = $commands . "OBPDE:exponentViscoPlastic $exponentVP\n" . \
       "continue\ncontinue\ncontinue\ncontinue\ncontinue\ncontinue\ncontinue\n";}
* 
  $commands
pause
  $commands
pause
  $commands
pause
  $commands
pause
  $commands




  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
  continue
  continue
  continue
  continue
* 
  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
* 
  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
  continue
  continue
  continue
  continue
* 
  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
  continue
  continue
  continue
  continue
* 
  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
  continue
  continue
  continue
  continue
* 
  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
  continue
  continue
  continue
  continue
* 
  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
  continue
  continue
  continue
  continue
* 
  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
  continue
  continue
  continue
  continue
* 
  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
  continue
  continue
  continue
  continue
* 
  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
  continue
  continue
  continue
  continue
* 
  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
  continue
  continue
  continue
  continue
* 
  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
  continue
  continue
  continue
  continue
* 
  $exponentVP=$exponentVP+2.; 
  OBPDE:exponentViscoPlastic $exponentVP
  continue
  continue
  continue
  continue







 continue


 movie mode 
 finish
