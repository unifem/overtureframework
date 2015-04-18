#
# cgins: Flow past a cylinder with a flexible appendage
#       
#  cgins [-noplot] cylBeam -g=<name> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -ts=<implicit> -debug=<num> ..,
#         -nu=<num>   -show=<name> -implicitFactor=<num> ...,
#         -solver=[best|yale|mg] -psolver=[best|yale|mg] -pc=[ilu|lu]
# Options:
#  -project : 1=project initial conditions
#  -solver : implicit solver
#  -psolver : pressure solver
# 
# Examples:
#    Grid:  ogen -noplot cylBeamGrid -interp=e -factor=2
#    Run:   cgins cylBeam -g=cylBeamGride2.order2 -tf=1. -tp=.01
# 
$grid="cylBeamGride2.order2.hdf"; $ts="adams PC"; $noplot=""; $backGround="square"; $uIn=1.0; $v0=0.; $T0=0.; $p0=0.; $cfl=.9; $useNewImp=1;
$tFinal=20.; $tPlot=.05; $dtMax=.1; $degreeSpace=2; $degreeTime=2; $show=" "; $debug=1; $go="halt";
$nu=1e-3; $Prandtl=.72; $thermalExpansivity=3.4e-3; $Tin=-10.;  $implicitFactor=.5; $implicitVariation="viscous"; 
$tz=0; # turn on tz here
$ad2=1; $ad21=.5; $ad22=.5; $ad4=0; $ad41=2.; $ad42=2.;
# $outflowOption="neumann"; causes wiggles at outflow
$outflowOption="extrapolate"; 
$gravity = "0 0.0 0."; $cdv=1.; $cDt=.25; $project=1; $restart=""; 
# $solver="choose best iterative solver";
$solver="yale";  $rtoli=1.e-5; $atoli=1.e-6; $idebug=0; 
$psolver="yale"; $rtolp=1.e-5; $atolp=1.e-6; $pdebug=0; $dtolp=1.e20; 
$pc="ilu"; $refactorFrequency=500; 
# 
$addedMass=1; $ampProjectVelocity=0;  $rhoBeam=100.; $E=100.; $bdebug=0;  $cfls=1.; 
$useApproximateAMPcondition=0;
$ampProjectVelocity=0; 
$projectNormalComponent=1; # 1 = project only the normal component of the velocity
$projectVelocityOnBeamEnds=1;   # project velocity on beam ends ? 
$projectBeamVelocity=1;
$smoothInterfaceVelocity=1; $numberOfInterfaceVelocitySmooths=2;
$cfls=1.; # CFL number for the structure (beam model is implicit)
$orderOfProjection=4; # order of accuracy for beam element integrals
$fluidOnTwoSides=1;  # beam has fluid on two sides
#
$dsBeam=.1; # grid spacing for the beam 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"refactorFrequency=i"=>\$refactorFrequency, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl,"cfls=f"=>\$cfls,"noplot=s"=>\$noplot,\
 "go=s"=>\$go,"dtMax=f"=>\$dtMax,"cDt=f"=>\$cDt,"iv=s"=>\$implicitVariation,"Tin=f"=>\$Tin,"ad2=i"=>\$ad2,\
 "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"outflowOption=s"=>\$outflowOption,"ad4=i"=>\$ad4,\
 "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"project=i"=>\$project,"cfl=f"=>\$cfl,\
 "restart=s"=>\$restart,"useNewImp=i"=>\$useNewImp,"p0=f"=>\$p0,"addedMass=i"=>\$addedMass,"rhoBeam=f"=>\$rhoBeam,\
 "bdebug=i"=>\$bdebug,"E=f"=>\$E,"ampProjectVelocity=i"=>\$ampProjectVelocity,"dsBeam=f"=>\$dsBeam,\
 "projectNormalComponent=i"=>\$projectNormalComponent,"useApproximateAMPcondition=i"=>\$useApproximateAMPcondition,\
 "smoothInterfaceVelocity=i"=>\$smoothInterfaceVelocity,"nis=i"=>\$numberOfInterfaceVelocitySmooths,\
 "projectVelocityOnBeamEnds=i"=>\$projectVelocityOnBeamEnds,"projectBeamVelocity=i"=>\$projectBeamVelocity );
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
  project beam velocity $projectBeamVelocity
  smooth interface velocity $smoothInterfaceVelocity
  number of interface velocity smooths $numberOfInterfaceVelocitySmooths
  # for now we let the solver know that the added mass algorithm needed predicted values for the pressure:
  predicted pressure needed $addedMass
#
  adjust dt for moving bodies 1
#   
  turn on moving grids
  specify grids to move
      deforming body
        user defined deforming body
          elastic beam
          $I=1.;   $pNorm=1.; 
          # Turek-Hron: $x0=.2 + .05; $y0=.2; $beamLength=.35; $thick=.02;
          # $cylRad=.5; $x0=.0 + $cylRad; $y0=.0; $thick=.2; $beamLength=3.;
          # beam needs to be shorter than the fluid grid:
          $cylRad=.5; $x0=.0 + $cylRad; $y0=.0; $thick=.2; $beamLength=2.95;
          $angle=0.;
          elastic beam parameters...
            $nBeam=int( $beamLength/$dsBeam + .5 ); 
            number of elements: $nBeam
            cfl: $cfls
            area moment of inertia: $I
            elastic modulus: $E
            density: $rhoBeam
            thickness: $thick
            length: $beamLength
            pressure norm: $pNorm
            initial declination: $angle (degrees)
            position: $x0, $y0, 0 (x0,y0,z0)
            bc left:clamped
            bc right:free
            debug: $bdebug
            #
            order of Galerkin projection: $orderOfProjection
            fluid on two sides $fluidOnTwoSides
            #            #
            fluid on two sides $fluidOnTwoSides
            #
            use implicit predictor 1
            #
          exit
          # ----
          boundary parameterization
             1
          # -- WHAT ARE THESE: ??
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
      10
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
  boundary conditions
    all=noSlipWall 
    # bcNumber1=inflowWithVelocityGiven, parabolic(d=$halfH, p=1.,u=$uIn,T=$Tin)
    bcNumber1=inflowWithVelocityGiven, uniform(p=1.,u=$uIn,T=$Tin)
    $cpn=1.;
    bcNumber2=outflow, pressure(.1*p+$cpn*p.n=0.)
    bcNumber3=slipWall
    bcNumber4=slipWall
  done
# 
  if( $restart eq "" ){ $cmds = "uniform flow\n" . "p=$p0, u=$uIn, v=0, T=$T0\n"; }\
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
