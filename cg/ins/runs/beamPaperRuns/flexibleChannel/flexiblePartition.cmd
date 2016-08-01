#
# cgins: Flow in a rectangle with a flexible partition
#       
#  cgins [-noplot] flexiblePartition -g=<name> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -ts=<implicit> -debug=<num> ..,
#         -nu=<num>   -show=<name> -implicitFactor=<num> ...,
#         -solver=[best|yale|mg] -psolver=[best|yale|mg] -pc=[ilu|lu]
# Options:
#  -project : 1=project initial conditions
#  -solver : implicit solver
#  -psolver : pressure solver
#  -option : "beamPiston" = beam-piston solution
#            "beamUnderPressure" = steady deflected beam
#  -orientation = horizontal or vertical
# 
# Examples:
#    cgins flexiblePartition -g=flexiblePartitionGride2.order2.hdf -nu=.01 -tf=1. -tp=.01
# 
$grid="flexiblePartitionGride2.order2.hdf"; $ts="adams PC"; $noplot=""; $backGround="square"; $uIn=1.0*1.5; $v0=0.; $T0=0.; $p0=1.; $cfl=.9; $useNewImp=1;
$tFinal=20.; $tPlot=.05; $dtMax=.1; $degreeSpace=2; $degreeTime=2; $show=" "; $debug=1; $go="halt";
$nu=.01; $Prandtl=.72; $thermalExpansivity=3.4e-3; $Tin=-10.;  $implicitFactor=.5; $implicitVariation="viscous"; 
$tz=0; # turn on tz here
$ad2=1; $ad21=.5; $ad22=.5; $ad4=0; $ad41=2.; $ad42=2.;
# $outflowOption="neumann"; causes wiggles at outflow
$outflowOption="extrapolate"; 
$gravity = "0 0.0 0."; $cdv=1.; $cDt=.25; $project=0; $restart=""; 
# $solver="choose best iterative solver";
$solver="yale";  $rtoli=1.e-5; $atoli=1.e-6; $idebug=0; 
$psolver="yale"; $rtolp=1.e-5; $atolp=1.e-6; $pdebug=0; $dtolp=1.e20; 
$pc="ilu"; $refactorFrequency=500; 
$sideBC="noSlipWall"; 
#
$option=""; 
$orientation="horizontal";
# 
$addedMass=0; 
$useApproximateAMPcondition=0;
$ampProjectVelocity=0;
$projectNormalComponent=0; # 1 = project only the normal component of the velocity
$projectVelocityOnBeamEnds=1; 
$smoothInterfaceVelocity=1; $numberOfInterfaceVelocitySmooths=2; 
$projectBeamVelocity=1; 
#
$smoothBeam=0; $numberOfBeamSmooths=2; 
#
$useTP=0; # set to 1 to iterate with TP scheme
$addedMassRelaxation=1.; # 1=no-relaxation
$addedMassTol=1.e-3;
$numberOfCorrections=100; 
#
# -- beam parameters
#
$bdebug=0;
$BM="FEM";
$ps="newmark2Implicit"; #solid predictor
$cs="newmarkCorrector"; #solid corrector
$useSameStencilSize=0;
$cfls=1.; # CFL number for the structure (beam model is implicit)
$tension=10.; $I=1.; $E=0.; $K0=0.; $Kt=0.; $Kxxt=0.; 
$rhoBeam=100.; $thickness=.1;  $length=2.; 
$numElem = 21; # number of elements in the beam
$fluidOnTwoSides=1;  # beam has fluid on two sides
$orderOfProjection=4; # order of accuracy for beam element integrals
$beamProbeFileName="beamProbe.text"; 
$probePosition=1.; # probe location in [0,1]
# 
$pMax=1; $tMax=.5;  
#
$beamPlotScaleFactor=1.; # scale beam displacement for plotting 
#
# recompute grid velocity on corrections
$recomputeGVOnCorrection=0;
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"refactorFrequency=i"=>\$refactorFrequency, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl,"cfls=f"=>\$cfls,"noplot=s"=>\$noplot,\
 "go=s"=>\$go,"dtMax=f"=>\$dtMax,"cDt=f"=>\$cDt,"iv=s"=>\$implicitVariation,"Tin=f"=>\$Tin,"ad2=i"=>\$ad2,\
 "solver=s"=>\$solver,"psolver=s"=>\$psolver,"pc=s"=>\$pc,"outflowOption=s"=>\$outflowOption,"ad4=i"=>\$ad4,\
 "debug=i"=>\$debug,"pdebug=i"=>\$pdebug,"idebug=i"=>\$idebug,"project=i"=>\$project,"cfl=f"=>\$cfl,\
 "restart=s"=>\$restart,"useNewImp=i"=>\$useNewImp,"p0=f"=>\$p0,"addedMass=i"=>\$addedMass,"delta=f"=>\$delta,\
 "bdebug=i"=>\$bdebug,"E=f"=>\$E,"ampProjectVelocity=i"=>\$ampProjectVelocity,"tension=f"=>\$tension,\
  "tMax=f"=>\$tMax,"pMax=f"=>\$pMax,"thickness=f"=>\$thickness,"length=f"=>\$length,"E=f"=>\$E,"Kt=f"=>\$Kt,\
  "rhoBeam=f"=>\$rhoBeam,"beamPlotScaleFactor=f"=>\$beamPlotScaleFactor,"numElem=i"=>\$numElem,"K0=f"=>\$K0,\
  "Kxxt=f"=>\$Kxxt,"fluidOnTwoSides=i"=>\$fluidOnTwoSides,"orderOfProjection=i"=>\$orderOfProjection,\
  "sideBC=s"=>\$sideBC,"smoothInterfaceVelocity=i"=>\$smoothInterfaceVelocity,"option=s"=>\$option,\
  "nis=i"=>\$numberOfInterfaceVelocitySmooths,"beamProbeFileName=s"=>\$beamProbeFileName,"ps=s"=>\$ps,"cs=s"=>\$cs,\
  "projectNormalComponent=i"=>\$projectNormalComponent,"projectVelocityOnBeamEnds=i"=>\$projectVelocityOnBeamEnds,\
  "useApproximateAMPcondition=i"=>\$useApproximateAMPcondition,"orientation=s"=>\$orientation,\
  "projectBeamVelocity=i"=>\$projectBeamVelocity,"numberOfCorrections=i"=>\$numberOfCorrections,\
  "useTP=i"=>\$useTP,"addedMassRelaxation=f"=>\$addedMassRelaxation,"addedMassTol=f"=>\$addedMassTol,"BM=s"=>\$BM,\
  "smoothBeam=i"=>\$smoothBeam,"numberOfBeamSmooths=i"=>\$numberOfBeamSmooths,"probePosition=f"=>\$probePosition,\
  "useSameStencilSize=i"=>\$useSameStencilSize,"recomputeGVOnCorrection=i"=>\$recomputeGVOnCorrection);
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
$pi=4.*atan2(1.,1.);
#
#
if($BM eq "FEM") {$beamModel = "FEMBeamModel";}
if($BM eq "FD")  {$beamModel = "FDBeamModel";}
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
#  *** Use new way to compute past time values:
  use new time-stepping startup 1
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
  #
  adjust dt for moving bodies 1
#
  # adjust the beam height so that fluid surface matches the bottom of the beam surface
  $height=.5+$thickness*.5; 
  turn on moving grids
  specify grids to move
      #Longfei 20160721: recompute grid velocity on corrections
      recompute grid velocity on correction $recomputeGVOnCorrection
      deforming body
        user defined deforming body
          elastic beam
	  $beamModel	
          $pNorm=1.;
          elastic beam parameters...
	    predictor: $ps  
	    corrector: $cs
            number of elements: $numElem
            cfl: $cfls
	    use same stencil size for FD $useSameStencilSize	
            area moment of inertia: $I
            elastic modulus: $E
            tension: $tension
            K0: $K0
            Kt: $Kt
            Kxxt: $Kxxt
            density: $rhoBeam
            thickness: $thickness
            length: $length
            pressure norm: $pNorm
            $angle=0.; $x0=-1.; $y0=0.; $signForNormal=1.;
            if( $orientation eq "vertical" ){ $angle=90.; $x0=0.; $y0=-1.; $signForNormal=-1.; }
            initial declination: $angle (degrees)
            sign for normal: $signForNormal
            # shift beam up for a one-sided fluid since grid stops at y=0
            ## no if( $fluidOnTwoSides == 0 ){ $y0=.5*$thickness; } 
            position: $x0, $y0, 0 (x0,y0,z0)
            # Use pinned BC for EI=0 
            if( $E == 0. ){ $beamBC="pinned"; }else{ $beamBC="clamped"; }
            # *OLD* if( $sideBC eq "slipWall" ){ $beamBC="free"; }
            if( $sideBC eq "slipWall" ){ $beamBC="slide"; }
            bc left:$beamBC
            bc right:$beamBC
            #
            use small deformation approximation 1
            #
            plotting scale factor: $beamPlotScaleFactor
            debug: $bdebug
            #
            order of Galerkin projection: $orderOfProjection
            fluid on two sides $fluidOnTwoSides
            #
            # use implicit predictor 1
            # if( $useTP eq 1 ){ $cmd="use implicit predictor 0\n use second order Newmark predictor 0";}else{ $cmd="#"; }
            # $cmd
            # -- for TP scheme 
            relax correction steps $useTP
            added mass relaxation: $addedMassRelaxation
            added mass tol: $addedMassTol
            #
            smooth solution $smoothBeam
            number of smooths: $numberOfBeamSmooths
            #
            # probe location in [0,1]
            probe position: $probePosition
            probe file name: $beamProbeFileName
            probe file save frequency: 10
            save probe file 1
            #
            # "Enter ya,yb,pa,pb,rhos,hs,fluidHeight,K0,Kt")
           $hs=$thickness; 
           $ya=-.5-.5*$hs; $yb=.5+.5*$hs; $pa=1.; $pb=0.; $fluidHeight=1.; 
           if( $fluidOnTwoSides == 0 ){ $ya=-.5-.5*$hs; $yb=.5*$hs; $fluidHeight=.5; }
           if( $option eq "beamPiston" ){ $cmd = "exact solution...\n Exact solution:beam piston\n $ya $yb $pa $pb $rhoBeam $hs $fluidHeight $K0 $Kt\n exit";}else{ $cmd="#"; }
            $cmd
            if( $option eq "beamPiston" ){ $cmd = "initial conditions...\n Initial conditions:exact solution\n exit";}else{ $cmd="#"; }
            $cmd
            # For the beamUnderPressure we use zero initial conditions (we do not know the time-dependent solution)
            $dp=$pa-$pb; 
            if( $option eq "beamUnderPressure" ){ $cmd = "exact solution...\nExact solution:beam under pressure\n $dp\n exit";}else{ $cmd="#"; }
            $cmd
            if( $option eq "beamUnderPressure" ){ $cmd = "initial conditions...\n  Initial conditions:zero\n exit";}else{ $cmd="#"; }
            $cmd
            #
          exit
          # ----
          # --- generate deforming grids at past times using the exact solution:
          if( $option eq "beamPiston" ){ $cmd ="generate past history 1"; }else{ $cmd="#"; }
          $cmd
          number of past time levels: 3
          past time dt: 0.005
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
  # number of PC corrections 200
  if( $useTP eq 1 ){ $cmd="number of PC corrections $numberOfCorrections"; }else{ $cmd="#"; }
  $cmd
#   
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
      20
    exit
#**
#
# --- choose the known solution: 
#
if( $option eq "beamPiston" ){ $cmd="OBTZ:user defined known solution\n beam piston\n $ya $yb $pa $pb $rhoBeam $hs $fluidHeight $K0 $Kt\n done"; }else{ $cmd="#"; }
$cmd
# 
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
    # all=$sideBC
    bcNumber1=$sideBC
    bcNumber2=$sideBC
    bcNumber100=noSlipWall
#     bcNumber3=inflowWithPressureAndTangentialVelocityGiven, userDefinedBoundaryData
#       pressure pulse
#         $pMax $tMax
#       done
    # Set pressure at bottom to p=p0, and on top to p=0
    if( $orientation eq "horizontal" ){ $cmd="bcNumber3=outflow, pressure(1.*p+0.*p.n=$p0)\n bcNumber4=outflow, pressure(1.*p+0.*p.n=0.)"; }
    # set pressure on left to p=p0 and on right to p=0.
    if( $orientation eq "vertical" ){ $cmd="bcNumber1=outflow, pressure(1.*p+0.*p.n=$p0\n bcNumber2=outflow, pressure(1.*p+0.*p.n=0.)"; }
    $cmd
    #
    # **** ramp the pressure on the bottom ****
    $cmd="bcNumber3=outflow, pressure(1.*p+0.*p.n=$p0), userDefinedBoundaryData\n" . \
    " time function option\n" . \
    "   ramp function\n" .\
    "   ramp end values: 0,1 (start,end)\n" .\
    "   ramp times: 0,1 (start,end)\n" .\
    "   ramp order: 3\n" .\
    " exit \n" .\
    "done";
    if( $option ne "beamUnderPressure" ){ $cmd = "#"; }
    $cmd
    #
    # the oscillating inflow has the form: a0 + a1*cos((t-t0)*(2*pi*omega))*( uniform/parabolic)
    ## bcNumber3=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.,u=0,v=0.), oscillate(a0=.5,a1=-.5,t0=.0,omega=.5)
    # bcNumber3=outflow, pressure(1.*p+0.*p.n=1.)
    # bcNumber4=outflow, pressure(1.*p+0.*p.n=0.)
    # bcNumber1=slipWall
    # bcNumber2=slipWall
  done
#
#  if( $restart eq "" ){ $cmds = "uniform flow\n" . "p=$p0, u=0., v=0, T=$T0\n"; }\
# Longfei 20160621: make initial p=0 instead of p0
  if( $restart eq "" ){ $cmds = "uniform flow\n" . "p=0, u=0., v=0, T=$T0\n"; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
  if( $option eq "beamPiston" ) { $cmds="OBIC:known solution"; }
#
  initial conditions
    $cmds
  exit
# 
  $project
  continue
  #
  plot structures 1
  plot:p
# 
  $go
