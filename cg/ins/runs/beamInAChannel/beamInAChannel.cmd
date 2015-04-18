#
# cgins: Flow past a cylinder with a flexible beam attached at one end
#       
#  cgins [-noplot] beamInAChannel -g=<name> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -ts=<implicit> -debug=<num> ..,
#         -nu=<num>   -show=<name> -implicitFactor=<num> ...,
#         -solver=[best|yale|mg] -psolver=[best|yale|mg] -pc=[ilu|lu]
# Options:
#  -project : 1=project initial conditions
#  -solver : implicit solver
#  -psolver : pressure solver
# 
# Examples:
#    cgins beamInAChannel -g=beamInAChannelGride2.order2.hdf -tf=1. -tp=.01
# 
$grid="beamInAChannelGride2.order2.hdf"; $ts="adams PC"; $noplot=""; $backGround="square"; $uIn=1.0; $v0=0.; $T0=0.; $p0=0.; $cfl=.9; $useNewImp=1;
$tFinal=20.; $tPlot=.05; $dtMax=.1; $degreeSpace=2; $degreeTime=2; $show=" "; $debug=1; $go="halt";
$nu=1e-3; $Prandtl=.72; $thermalExpansivity=3.4e-3; $Tin=-10.;  $implicitFactor=.5; $implicitVariation="viscous"; 
$tz=0; # turn on tz here
$ad2=0; $ad21=.5; $ad22=.5; $ad4=0; $ad41=2.; $ad42=2.;
# $outflowOption="neumann"; causes wiggles at outflow
$outflowOption="extrapolate"; 
$gravity = "0 0.0 0."; $cdv=1.; $cDt=.25; $project=1; $restart=""; 
# $solver="choose best iterative solver";
$solver="yale";  $rtoli=1.e-5; $atoli=1.e-6; $idebug=0; 
$psolver="yale"; $rtolp=1.e-5; $atolp=1.e-6; $pdebug=0; $dtolp=1.e20; 
$pc="ilu"; $refactorFrequency=500; 
#
$rampInflow=0; # set to 1 to ramp inflow
# 
$addedMass=0; $ampProjectVelocity=0; $E=10.; $tension=0.; $bdebug=0; $K0=0.; $Kt=0.; $Kxxt=0.;
$rhoBeam=100.; 
$delta=-1.; # old way 
$useApproximateAMPcondition=0;
$projectNormalComponent=1; # 1 = project only the normal component of the velocity
$projectBeamVelocity=1; 
$projectVelocityOnBeamEnds=0; # do NOT project on ends if we only project normal component -- tangential velocity on ends may be funny
$smoothInterfaceVelocity=1; $numberOfInterfaceVelocitySmooths=2; 
$fluidOnTwoSides=1;  # beam has fluid on two sides
$orderOfProjection=4; # order of accuracy for beam element integrals
#
$smoothBeam=0; $numnberOfBeamSmooths=2; 
#
$useTP=0; # set to 1 to iterate with TP scheme
$addedMassRelaxation=1.; # 1=no-relaxation
$addedMassTol=1.e-3;
$numberOfCorrections=100; 
#
$newmarkBeta=.25; $newMarkGamma=.5; # second-order, BE: set beta=.5 gamma=1
$probeFileName="beamTipFile.txt"; 
#
$cfls=1.; # CFL number for the structure (beam model is implicit)
$numElem = 11; # number of elements in the beam
# 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"refactorFrequency=i"=>\$refactorFrequency, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl,"cfls=f"=>\$cfls,"noplot=s"=>\$noplot,\
 "go=s"=>\$go,"dtMax=f"=>\$dtMax,"cDt=f"=>\$cDt,"iv=s"=>\$implicitVariation,"Tin=f"=>\$Tin,"ad2=i"=>\$ad2,\
 "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"outflowOption=s"=>\$outflowOption,"ad4=i"=>\$ad4,\
 "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"project=i"=>\$project,\
 "restart=s"=>\$restart,"useNewImp=i"=>\$useNewImp,"p0=f"=>\$p0,"addedMass=i"=>\$addedMass,"delta=f"=>\$delta,\
 "bdebug=i"=>\$bdebug,"E=f"=>\$E,"ampProjectVelocity=i"=>\$ampProjectVelocity,\
  "projectNormalComponent=i"=>\$projectNormalComponent,"smoothInterfaceVelocity=i"=>\$smoothInterfaceVelocity,\
  "nis=i"=>\$numberOfInterfaceVelocitySmooths,"fluidOnTwoSides=i"=>\$fluidOnTwoSides,\
  "orderOfProjection=i"=>\$orderOfProjection,"numElem=i"=>\$numElem,"tension=f"=>\$tension,"K0=f"=>\$K0,\
  "Kt=f"=>\$Kt,"Kxxt=f"=>\$Kxxt,"rhoBeam=f"=>\$rhoBeam,"projectVelocityOnBeamEnds=i"=>\$projectVelocityOnBeamEnds,\
  "projectBeamVelocity=i"=>\$projectBeamVelocity,"numberOfCorrections=i"=>\$numberOfCorrections,\
  "useApproximateAMPcondition=i"=>\$useApproximateAMPcondition,"rampInflow=i"=>\$rampInflow,\
  "useTP=i"=>\$useTP,"addedMassRelaxation=f"=>\$addedMassRelaxation,"addedMassTol=f"=>\$addedMassTol,\
  "smoothBeam=i"=>\$smoothBeam,"numberOfBeamSmooths=i"=>\$numberOfBeamSmooths,"newmarkBeta=f"=>\$newmarkBeta,\
  "newmarkGamma=f"=>\$newmarkGamma,"probeFileName=s"=>\$probeFileName );
# -------------------------------------------------------------------------------------------------
if( $delta > 0. ){ $rhoBeam=$delta; }
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
  number of PC corrections $numberOfCorrections
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
  use moving grid sub-iterations $useTP
  adjust dt for moving bodies 1
  # For steady state do not recompute dt too often or else it changes to reach tprint
  recompute dt every 1000 steps
#   
  turn on moving grids
  specify grids to move
      deforming body
        user defined deforming body
          elastic beam
          $I=1.;  $length=1.; $thick=.2; $pNorm=1.; 
          $angle=90.; # $Pi*.5; 
          elastic beam parameters...
            number of elements: $numElem
            cfl: $cfls
            # beta=.25, gamma=.5 => second-order;  BE: set beta=.5 gamma=1
            Newmark beta: $newmarkBeta
            Newmark gamma: $newmarkGamma
            area moment of inertia: $I
            elastic modulus: $E
            tension: $tension
            K0: $K0
            Kt: $Kt
            Kxxt: $Kxxt
            density: $rhoBeam
            thickness: $thick
            length: $length
            pressure norm: $pNorm
            initial declination: $angle (degrees)
            position: 0, 0, 0 (x0,y0,z0)
            bc left:clamped
            #  bc left:free
            bc right:free
            debug: $bdebug
            #
            $cmd = "initial conditions...\n  Initial conditions:zero\n exit";
            $cmd
            order of Galerkin projection: $orderOfProjection
            fluid on two sides $fluidOnTwoSides
            #
            use implicit predictor 1
            # -- for TP scheme 
            relax correction steps $useTP
            added mass relaxation: $addedMassRelaxation
            added mass tol: $addedMassTol
            #
            smooth solution $smoothBeam
            number of smooths: $numberOfBeamSmooths
            #
            probe file name: $probeFileName
            save probe file 1
            probe file save frequency: 10
            #
          exit
          # ---- 1=index parameterization
          boundary parameterization
             1
          #
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
  plot residuals 1
# 
  boundary conditions
    all=noSlipWall 
    $halfH=0.2;
#    bcNumber1=inflowWithVelocityGiven, parabolic(d=$halfH, p=1.,u=$uIn,T=$Tin)
     bcNumber1=inflowWithVelocityGiven, parabolic(d=$halfH, p=1.,u=$uIn,T=$Tin), userDefinedBoundaryData
     time function option
       ramp function
       ramp end values: 0,1 (start,end)
       ramp times: 0,1 (start,end)
       # ramp-order = number of time derivatives that are zero at ends
       ramp order: 3
     exit
    done
# 
#    if( $rampInflow eq 1 ){ $rampOption = ", ramp(ta=0.,tb=1.,ua=0.,ub=$uIn)"; }else{ $rampOption = ""; }
#    bcNumber1=inflowWithVelocityGiven, parabolic(d=$halfH, p=1.,u=$uIn,T=$Tin) $rampOption
    #bcNumber1=inflowWithVelocityGiven, uniform(p=1.,u=$uIn,T=$Tin)
    $cpn=1.;
    # *wdh* 2015/03/25 bcNumber2=outflow, pressure(.1*p+$cpn*p.n=0.)
    bcNumber2=outflow, pressure(10.*p+$cpn*p.n=0.)
    bcNumber4=slipWall
  done
# 
  if( $rampInflow eq 1 ){ $u0=0.; }else{ $u0=$uIn; }
  if( $restart eq "" ){ $cmds = "uniform flow\n" . "p=$p0, u=$u0, v=0, T=$T0\n"; }\
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
