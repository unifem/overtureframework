#
# cgins: Flow past a cylinder with a flexible beam attached at one end
#       
#  cgins [-noplot] verticalBeamInAChannel -g=<name> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -ts=<implicit> -debug=<num> ..,
#         -nu=<num>   -show=<name> -implicitFactor=<num> ...,
#         -solver=[best|yale|mg] -psolver=[best|yale|mg] -pc=[ilu|lu]
# Options:
#  -project : 1=project initial conditions
#  -solver : implicit solver
#  -psolver : pressure solver
# 
# Examples:
#    cgins verticalBeamInAChannel -g=verticalBeamInAChannelGride2.order2.hdf -tf=1. -tp=.01
# 
$grid="beamInAChannelGride2.order2.hdf"; $ts="adams PC"; $noplot=""; $backGround="square"; $uIn=1.0*1.5; $v0=0.; $T0=0.; $p0=0.; $cfl=.9; $useNewImp=1;
$tFinal=20.; $tPlot=.05; $dtMax=.1; $degreeSpace=2; $degreeTime=2; $show=" "; $debug=1; $go="halt";
$nu=1e-3; $Prandtl=.72; $thermalExpansivity=3.4e-3; $Tin=-10.;  $implicitFactor=.5; $implicitVariation="viscous"; 
$tz=0; # turn on tz here
$ad2=1; $ad21=1.; $ad22=1.; $ad4=0; $ad41=2.; $ad42=2.;
# $outflowOption="neumann"; causes wiggles at outflow
$outflowOption="extrapolate"; 
$gravity = "0 0.0 0."; $cdv=1.; $cDt=.25; $project=1; $restart=""; 
# $solver="choose best iterative solver";
$solver="yale";  $rtoli=1.e-5; $atoli=1.e-6; $idebug=0; 
$psolver="yale"; $rtolp=1.e-5; $atolp=1.e-6; $pdebug=0; $dtolp=1.e20; 
$pc="ilu"; $refactorFrequency=500; 
# 
$addedMass=0; $ampProjectVelocity=0;  $delta=100.; $E=10.; $bdebug=0; 
$useApproximateAMPcondition=0;
$projectNormalComponent=1; # 1 = project only the normal component of the velocity
$projectVelocityOnBeamEnds=1; 
$smoothInterfaceVelocity=1; $numberOfInterfaceVelocitySmooths=2; 
$fluidOnTwoSides=1;  # beam has fluid on two sides
$orderOfProjection=4; # order of accuracy for beam element integrals
#
$option="verticalFlow"; 
#
$cfls=1.; # CFL number for the structure (beam model is implicit)
$numElem = 11; # number of elements in the beam
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"refactorFrequency=i"=>\$refactorFrequency, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl,"cfsl=f"=>\$cfls,"noplot=s"=>\$noplot,\
 "go=s"=>\$go,"dtMax=f"=>\$dtMax,"cDt=f"=>\$cDt,"iv=s"=>\$implicitVariation,"Tin=f"=>\$Tin,"ad2=i"=>\$ad2,\
 "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"outflowOption=s"=>\$outflowOption,"ad4=i"=>\$ad4,\
 "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"project=i"=>\$project,"cfl=f"=>\$cfl,\
 "restart=s"=>\$restart,"useNewImp=i"=>\$useNewImp,"p0=f"=>\$p0,"addedMass=i"=>\$addedMass,"delta=f"=>\$delta,\
 "bdebug=i"=>\$bdebug,"E=f"=>\$E,"ampProjectVelocity=i"=>\$ampProjectVelocity,\
  "projectNormalComponent=i"=>\$projectNormalComponent,"smoothInterfaceVelocity=i"=>\$smoothInterfaceVelocity,\
  "nis=i"=>\$numberOfInterfaceVelocitySmooths,"fluidOnTwoSides=i"=>\$fluidOnTwoSides,\
  "orderOfProjection=i"=>\$orderOfProjection,"numElem=i"=>\$numElem,"option=s"=>\$option,\
  "projectVelocityOnBeamEnds=i"=>\$projectVelocityOnBeamEnds,\
  "useApproximateAMPcondition=i"=>\$useApproximateAMPcondition );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
if( $pc eq "ilu" ){ $pc = "incomplete LU preconditioner"; }elsif( $pc eq "lu" ){ $pc = "lu preconditioner"; }else{ $pc="#"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
if( $ts eq "fe" ){ $ts="forward Euler";}
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $ts eq "pc4" ){ $ts="adams PC order 4"; $useNewImp=0; }
if( $useNewImp eq 1 ){ $useNewImp ="useNewImplicitMethod"; }else{ $useNewImp="#"; }
if( $implicitVariation eq "viscous" ){ $implicitVariation = "$useNewImp\n implicitViscous"; }\
elsif( $implicitVariation eq "adv" ){ $implicitVariation = "$useNewImp\n implicitAdvectionAndViscous"; }\
elsif( $implicitVariation eq "full" ){ $implicitVariation = "$useNewImp\n implicitFullLinearized"; }\
else{ $implicitVariation = "implicitFullLinearized"; }
if( $project eq "1" && $restart eq "" ){ $project = "project initial conditions"; }else{ $project = "do not project initial conditions"; }
#
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
$kThermal=$nu/$Prandtl;
$Pi=4.*atan2(1.,1.);
#
# specify the grid: 
$grid
# 
  incompressible Navier Stokes
  ##  Boussinesq model
 #   define real parameter kappa $kThermal
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  continue
# 
  OBTZ:polynomial
  OBTZ:twilight zone flow $tz
  degree in space $degreeSpace
  degree in time $degreeTime
# 
# choose time stepping method:
  $ts
# -- for added mass algorithm:
  use added mass algorithm $addedMass
  use approximate AMP condition $useApproximateAMPcondition
  project added mass velocity $ampProjectVelocity
  project normal component $projectNormalComponent
  project velocity on beam ends $projectVelocityOnBeamEnds
  smooth interface velocity $smoothInterfaceVelocity
  number of interface velocity smooths $numberOfInterfaceVelocitySmooths
  # for now we let the solver know that the added mass algorithm needed predicted values for the pressure:
  predicted pressure needed $addedMass
  adjust dt for moving bodies 1
#   
  turn on moving grids
  specify grids to move
      deforming body
        user defined deforming body
          elastic beam
          $I=1.;  $rhoBeam=$delta; $length=1.; $thick=.2; $pNorm=1.; 
          $angle=90.; # $Pi*.5; 
          elastic beam parameters...
            number of elements: $numElem
            cfl: $cfls
            area moment of inertia: $I
            elastic modulus: $E
            density: $rhoBeam
            thickness: $thick
            length: $length
            pressure norm: $pNorm
            initial declination: $angle (degrees)
            position: 0, 0, 0 (x0,y0,z0)
            bc left:clamped
            bc right:free
            debug: $bdebug
            #
            $cmd = "initial conditions...\n  Initial conditions:zero\n exit";
            $cmd
            order of Galerkin projection: $orderOfProjection
            fluid on two sides $fluidOnTwoSides
            #
            use implicit predictor 1
            #
          exit
          # ----
          boundary parameterization
             1
          BC left: Dirichlet
          BC right: Dirichlet
          BC bottom: Dirichlet
          BC top: Dirichlet
        #
        done
        choose grids by share flag
          100
     done
  done
#
  # number of PC corrections 100
  $implicitVariation
  implicit factor $implicitFactor 
  refactor frequency $refactorFrequency
# 
  choose grids for implicit
    all=implicit
   done
# 
#
  final time $tFinal
  times to plot $tPlot
  dtMax $dtMax
  pde parameters
    nu  $nu
    density 1.
    kThermal $kThermal
    gravity
      $gravity
    OBPDE:divergence damping  $cdv 
    OBPDE:cDt div damping $cDt
   OBPDE:second-order artificial diffusion $ad2
   OBPDE:ad21,ad22 $ad21, $ad22
   OBPDE:fourth-order artificial diffusion $ad4
   OBPDE:ad41,ad42 $ad41, $ad42
   # MG solver currently wants a Neumann BC at outflow
   ## if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="OBPDE:expect inflow at outflow\n use extrapolate BC at outflow"; }
   if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="use extrapolate BC at outflow"; }
   if( $outflowOption eq "neumann" ){ $cmd = "use Neumann BC at outflow"; }else{ $cmd="OBPDE:check for inflow at outflow\n use extrapolate BC at outflow"; }
     $cmd
   done
# 
  debug $debug
#**
  show file options
    compressed
     OBPSF:maximum number of parallel sub-files 8
      open
      $show
    frequency to flush
      4
    exit
#**
#
  pressure solver options
   $ogesSolver=$psolver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesPC=$pc; $ogesDebug=$pdebug; $ogmgDebug=$pdebug;
   $ogesDtol=$dtolp; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtoli; $ogesAtol=$atoli; $ogesPC=$pc; $ogesDebug=$idebug; 
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
 cfl $cfl
# 
  $cpn=1.;
  boundary conditions
    all=noSlipWall 
    $halfH=0.2;
    if( $option eq "verticalFlow" ){ $cmd = "bcNumber1=inflowWithVelocityGiven, parabolic(d=$halfH, p=1.,v=$uIn,T=$Tin)\n  bcNumber2=outflow, pressure(.1*p+$cpn*p.n=0.)\n bcNumber3=slipWall \n bcNumber4=slipWall"; }
#
    if( $option eq "upwardFlow" ){ $cmd="bcNumber3=inflowWithVelocityGiven, parabolic(d=$halfH, p=1.,u=-$uIn,T=$Tin)\n bcNumber4=outflow, pressure(.1*p+$cpn*p.n=0.) \n bcNumber2=slipWall"; }
    $cmd
    #
    # bcNumber1=inflowWithVelocityGiven, parabolic(d=$halfH, p=1.,u=$uIn,T=$Tin)
    #bcNumber3=inflowWithVelocityGiven, parabolic(d=$halfH, p=1.,v=$uIn,T=$Tin)
    #bcNumber1=inflowWithVelocityGiven, uniform(p=1.,u=$uIn,T=$Tin)
    #bcNumber4=outflow, pressure(.1*p+$cpn*p.n=0.)
    #    bcNumber3=slipWall
    #    bcNumber4=slipWall
  done
# 
  if( $restart eq "" ){ $cmds = "uniform flow\n" . "p=$p0, v=$uIn, v=0, T=$T0\n"; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
#
  initial conditions
    $cmds
  exit
# 
  $project
  continue
  #
  plot structures 1
  plot:u
# 
  $go
