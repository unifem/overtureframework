*
* cgins: Boussinesq flow in a box with a temperature gradient
*       
*  cgins [-noplot] tz -g=<name> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -ts=<im|pc> -debug=<num> ..,
*                     -nu=<num> -kThermal=<num> -bg=<backGround> -show=<name> -implicitFactor=<num>
* 
* Examples:
*  cgins heatedBox -g=square20.order2 -nu=.1 -kThermal=.15 -tf=5. -tp=.2
*  cgins heatedBox -g=square40.order2 -nu=.1 -kThermal=.15 -tf=5. -tp=.5 -ts=imp -implicitFactor=.55
* 
* --- set default values for parameters ---
* 
$grid="square10.hdf"; $ts="adams PC"; $noplot="";
$tFinal=20.; $tPlot=.1; $dtMax=.1; $degreeSpace=2; $degreeTime=2; $show=" "; $debug=1; 
$nu=.1; $kThermal=.1; $thermalExpansivity=.1; $Twall=1.;  $kappa=.1; $implicitFactor=.5; 
$tz=0; # turn on tz here
$gravity = "0 -10. 0.";
* $solver="choose best iterative solver";
$solver="yale";
* 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"kThermal=f"=>\$kThermal,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "noplot=s"=>\$noplot );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $ts eq "fe" ){ $ts="forward Euler";}
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "pc4" ){ $ts="adams PC order 4"; }
*
* $grid="square10.hdf"; $tPlot=.01; $debug=7; $solver="yale";
* $grid="square20.hdf"; $tPlot=.1;
* $grid="square40.hdf"; $nu=.05; $kThermal=.1; $tPlot=.1;
* $grid="square40.hdf"; $nu=.1; $kThermal=.1; $tPlot=2.; $show="heatedBox.show"; 
* $grid="square40.hdf"; $nu=.01; $kThermal=.01; $tPlot=2.;
*
$grid
* 
  incompressible Navier Stokes
  Boussinesq model
  define real parameter kappa $kThermal
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  continue
* 
  OBTZ:polynomial
  OBTZ:twilight zone flow $tz
  degree in space $degreeSpace
  degree in time $degreeTime
* 
* choose time stepping method:
  $ts
*   
  implicit factor $implicitFactor 
* 
  choose grids for implicit
    all=implicit
   done
* 
*
  final time $tFinal
  times to plot $tPlot
  dtMax $dtMax
  pde parameters
    nu  $nu
    kThermal $kThermal
    gravity
      $gravity
   done
* 
  debug $debug
***
  show file options
    compressed
      open
      $show
    frequency to flush
      4
    exit
***
  pressure solver options
     $solver
     * yale
     * these tolerances are chosen for PETSc
     relative tolerance
       1.e-4
     absolute tolerance
       1.e-6
    exit
* 
  boundary conditions
    all=noSlipWall
    bcNumber3=noSlipWall, uniform(T=$Twall)
#     square(0,0)=noSlipWall, uniform(T=$Twall)
#     square(1,0)=noSlipWall, uniform(T=0.)
#     * adiabatic walls:
# *     square(0,1)=noSlipWall 
#     square(0,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
#     square(1,1)=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
# *     $backGround(1,0)=outflow
# * ---- test: 
# *-     all=noSlipWall , mixedDerivative(0.*t+1.*t.n=0.)
# *-     bcNumber4=inflowWithVelocityGiven, uniform(p=1.,v=-.5,T=-1.)
# *-     bcNumber3=outflow
  done
#  allow user defined output 1
* 
  initial conditions
  if( $tz eq "0" ){ $commands="uniform flow\n" . "p=1., u=0.\n" . "continue"; }else{ $commands="continue";}
    $commands
* 
  continue
  plot:T
