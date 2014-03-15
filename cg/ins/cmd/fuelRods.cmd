*
* cgins example: flow past some fuel rods with/with-out wire wraps
* 
* Usage:
*   
*  cgins [-noplot] fuelRods.cmd -g=<name> -nu=<value> -its=<tFinal> -pits=<tPlot> -solver=<yale/best> -model=<ins/boussinesq> -debug=<num> -ts=<fe/ss>
* where
*   -its    : max iterations
*   -pits   : plot iterations; number of iterations between plot output.
*   -ts     : fe=forward Euler, ss=steady-state
* 
* Examples:
* 
*      cgins fuelRods.cmd -g="fuelAssembly3d1pinse1.order2" -ts=im -nu=.01 -tp=.1
*      srun -N1 -n1 -ppdebug $cginsp fuelRods.cmd -g="fuelAssembly3d1pinse1.order2" -ts=im -nu=.01 -tp=.1
* 
*      srun -N1 -n1 -ppdebug memcheck_all $cginsp fuelRods.cmd -g="solidFuelAssembly1pinse1.order2" -ts=im -nu=.01 -tp=.1
* 
*      cgins fuelRods.cmd -g="wireWrap1pinsi1.order2.hdf" -nu=.01 -tp=.1
* 
*      cgins fuelRods.cmd -g="fuelAssembly3d1pinse1.order2.hdf" -model=ins -nu=.05 -its=1000 -pits=2 
* 
*      cgins fuelRods.cmd -g="fuelAssembly3d1pinse1.order2.hdf" -model=ins -nu=.05 -its=1000 -pits=2
*      srun -N1 -n2 -ppdebug $cginsp fuelRods.cmd -g="fuelAssembly3d1pinse1.order2.hdf" -model=ins -nu=.05 -its=1000 -pits=2 -ts=ss
*
* 
* --- set default values for parameters ---
* 
$tFinal=20.; $tPlot=.1; $degreeSpace=2; $degreeTime=2; $show=" "; 
$nu=.1; $kThermal=.1; $thermalExpansivity=.1; $Twall=1.;  $kappa=.1; $Prandtl=.72; $debug=3; 
$rtolp=1.e-4; $atolp=1.e-4; $pdebug=0; $rtoli=1.e-5; $atoli=1.e-7; $idebug=0;
*********
$rtolp=1.e-7; $atolp=1.e-7; $pdebug=0; $rtoli=1.e-7; $atoli=1.e-7; $idebug=0;
********
$tz=0; # turn on tz here
$gravity = "0  0. 10.";
$solver="choose best iterative solver";
* $solver="yale";
$ts="pc"; $its=1000; $pits=10; $dtMax=.05; 
$model ="boussinesq"; 
* 
* $grid="wireWrap1pinsi1.order2.hdf"; $tPlot=.1; $nu=.01; 
* $grid="wireWrap1pinsi2.order2.hdf"; $tPlot=.1; $nu=.05;
* $grid="wireWrap3pinsi1.order2.hdf"; $tFinal=1.; $tPlot=.1; $nu=.01; $rtol=1.e-3; $atol=1.e-4; $show="wireWrap3.show"; 
* $grid="wireWrap3pinsl10i1.hdf";  $tFinal=1.; $tPlot=.2; $nu=.01; $rtol=1.e-3; $atol=1.e-4; $show="wireWrap3l10.show";
*
* -- no wire wrap:
* $grid="fuelAssembly3d1pinse1.order2.hdf"; $tPlot=.01; $nu=.02;
* $grid="fuelAssembly3d1pinse1.order2.hdf"; $ts=$tsSS; $its=1000; $pits=2; $model="ins";
* $grid="fuelAssembly3d3pinse2.order2.hdf"; $ts=$tsSS; $its=1000; $pits=2; $model="ins";
* 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"its=i"=>\$its,"pits=i"=>\$pits,"model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "show=s"=>\$show,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "noplot=s"=>\$noplot );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $ts eq "ss" ){ $ts = "steady state RK-line"; }
if( $ts eq "fe" ){ $ts="forward Euler";  $tsd="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; $tsd="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       $tsd="implicit";  }
if( $ts eq "pc" ){ $ts="adams PC";       $tsd="adams PC";  }
if( $ts eq "mid"){ $ts="midpoint";       $tsd="forward Euler"; }  
*
$kThermal=$nu/$Prandtl;
*
$grid
* 
  $model
* 
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
*   forward Euler
  $ts 
    dtMax $dtMax
* 
*
  final time $tFinal
  times to plot $tPlot
*
  max iterations $its
  plot iterations $pits
*
  pde parameters
    nu  $nu
    kThermal $kThermal
    gravity
      $gravity
   done
* 
   debug $debug
* 
  show file options
    compressed
      open
      $show
    frequency to flush
     4
    exit
***
* choose implicit time stepping:
*  implicit
* but integrate the square explicitly:
  choose grids for implicit
    all=implicit
*     $backGround=explicit
    done
***
  pressure solver options
     $solver
     * yale
     * these tolerances are chosen for PETSc
     relative tolerance
       $rtolp
     absolute tolerance
       $atolp
    exit
  implicit time step solver options
     choose best iterative solver
     * PETSc
     * these tolerances are chosen for PETSc
     relative tolerance
       $rtoli
     absolute tolerance
       $atoli
    exit
* 
$inflowBC=3; $outflowBC=4; 
$ductBC=2; $rodBC=5; $wireBC=6; 
  boundary conditions
*     $backGround=slipWall
    all=noSlipWall, uniform(T=0.)
    bcNumber3=inflowWithVelocityGiven, parabolic(d=.2,p=1.,w=1.)
*    bcNumber3=inflowWithVelocityGiven, uniform(d=.2,p=1.,w=1.)
    bcNumber4=outflow
    bcNumber5=noSlipWall, uniform(T=$Twall)
  done
* 
  initial conditions
  if( $tz eq "0" ){ $commands="uniform flow\n" . "p=1., w=1. T=0.\n" . "continue"; }else{ $commands="continue";}
    $commands
* 
  project initial conditions
  continue
* 
  plot:w
x-r 80

  movie mode
  finish

