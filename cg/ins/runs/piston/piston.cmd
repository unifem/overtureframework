# ============================================================================
# Cgins: rigid body "piston" adjacent to an incompressible fluid channel
# 
# Usage:
#    cgins [-noplot] piston -g=<name> -tf=<> -tp=<> -bodyDensity=<> -inertial=<>  -relaxRigidBody=<> -omega=<> ...
#           -vIn=<> -rtolc=<> -atolc=<> -option=[none,rigidBodyPiston,closed]
#
# -inertial = moment of inertial
# -relaxRigidBody : 1= relaxation is used for light bodies to stabilize the time stepping
# -omega : relaxation parameter, proportional to mass/rho_fluid approx. 
# -rtolc, atolc : relative and absolute tolerances for light-body correction iterations
# -option : rigidBodyPiston
# 
# ============================================================================
$grid="plug4.hdf";  $tFinal=1.; $tPlot=.05; $bodyDensity=3.;  $fluidDensity=1.; $go="halt"; $show=" "; $vIn=.1;
$nu=.05; $gravity=1.; $cfl=.9; $dtMax=.1; $restart=""; $debug=1; 
$option="none"; 
#
$amp=.25; $freq=1.; $depth=1.;  # for rigidBodyPiston solution
#
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
$ts="pc"; $numberOfCorrections=1; 
$solver="choose best iterative solver"; 
$psolver="choose best iterative solver";  
$solver="yale"; $psolver="yale"; 
$relaxRigidBody=0; # set to 1 for "light bodies" which are otherwise unstable 
$addedMass=0; $useTP=0;  $useProvidedAcceleration=1; 
$addedDamping=0;  $addedDampingCoeff=1.; $scaleAddedDampingWithDt=0; $addedDampingProjectVelocity=0; 
$omega=.5; $inertia=1.; $rtolc=1.e-3; $atolc=1.e-7; 
$mbpbc=0; $mbpbcc=1.; # fix for light bodies: mbpbc=1 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver,"psolver=s"=>\$psolver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,"vIn=f"=>\$vIn,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"bodyDensity=f"=>\$bodyDensity,"inertia=f"=>\$inertia,\
  "nc=i"=>\$numberOfCorrections,"numberOfCorrections=i"=>\$numberOfCorrections,"mbpbc=i"=>\$mbpbc,"mbpbcc=f"=>\$mbpbcc,\
  "relaxRigidBody=i"=>\$relaxRigidBody,"omega=f"=>\$omega,"addedMass=f"=>\$addedMass,"useTP=i"=>\$useTP,\
  "rtolc=f"=>\$rtolc,"atolc=f"=>\$atolc,"useProvidedAcceleration=i"=>\$useProvidedAcceleration,\
  "option=s"=>\$option,"gravity=f"=>\$gravity,"dtMax=f"=>\$dtMax,"addedDamping=f"=>\$addedDamping,\
  "addedDampingCoeff=f"=>\$addedDampingCoeff,"scaleAddedDampingWithDt=f"=>\$scaleAddedDampingWithDt,\
  "addedDampingProjectVelocity=f"=>\$addedDampingProjectVelocity   );
# -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
if( $psolver eq "best" ){ $psolver="choose best iterative solver"; }
if( $psolver eq "mg" ){ $psolver="multigrid"; }
#
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
# 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
# 
  $grid
# 
  incompressible Navier Stokes
  exit
#
  show file options
   open
     $show
    frequency to flush
      5
  exit  
  turn off twilight zone
  final time $tFinal
  times to plot $tPlot
# 
##  project initial conditions
#
  dtMax $dtMax
  # Generate past time grids: 
  use new time-stepping startup 1
  #
#-----------------------------
  turn on moving grids
   # detect collisions 1
  specify grids to move
    rigid body
      log file: rigidBody.log
      debug: $debug
      # turn on added mass correction here (if Added-mass matrices exist)
      # added mass $addedMass
      # 
      # -- indicate if we are using the direct projection AMP scheme ---
      direct projection added mass $addedMass
      #
      use provided acceleration $useProvidedAcceleration
      #
      # relaxation is used for light bodies to stabilize the time stepping
      relax correction steps $relaxRigidBody
      $bodyMass=1.*$bodyDensity; 
      mass
        $bodyMass
      density
        $bodyDensity
      moments of inertia
        $inertia
      # try this for light solids
      position is constrained to a line
        1 0 0
      #
      # body displacement is amp*sin(fgreq*2*pi*t) 
      $pi=4.*atan2(1.,1.);
      $vBody=$amp*$freq*2.*$pi; # initial velocity of the body 
      if( $option eq "rigidBodyPiston" ){ $cmd="initial velocity\n $vBody 0. 0. "; }else{ $cmd="#"; }
      $cmd
      initial centre of mass
        # added damping is proportional to  w*h*(w+h)
        # -10 .5
         -1 .5
      # $omega=$bodyDensity*.5; # guess for relaxation parameter; omega should be <=1 
      debug: $debug
      force relaxation parameter: $omega
      force relative tol: $rtolc
      force absolute tol: $atolc
      $beta=$omega; 
      torque relaxation parameter: $beta
      torque relative tol: $rtolc
      torque absolute tol: $atolc
    done
      choose grids by share flag
          100
      # plug
   done
  done
# 
  frequency for full grid gen update $freqFullUpdate
#
 # use implicit time stepping
  $ts
  number of PC corrections $numberOfCorrections
  # -- for added mass algorithm:
  use added mass algorithm $addedMass
  # for now we let the solver know that the added mass algorithm needed predicted values for the pressure:
  predicted pressure needed $addedMass
  # for added damping algorithm: 
  added damping coefficient: $addedDampingCoeff
  use added damping algorithm $addedDamping
  scale added damping with dt $scaleAddedDampingWithDt
  added damping project velocity $addedDampingProjectVelocity
  # -- CHECK ME: 
  use moving grid sub-iterations $useTP
  # 
  choose grids for implicit
    all=implicit
   done
#
# --- choose the known solution: 
#
#  "Enter amp, freq, and depth (height of fluid channel)
if( $option eq "rigidBodyPiston" ){ $cmd="OBTZ:user defined known solution\n rigid body piston\n $amp $freq $depth\n done"; }else{ $cmd="#"; }
$cmd
# 
  pressure solver options
     $psolver
     # these tolerances are chosen for PETSc
     relative tolerance
       $rtolp
     absolute tolerance
       $atolp
     debug 
       $debug
    exit
# 
  implicit time step solver options
     $solver
     # these tolerances are chosen for PETSc
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
# 
  pde parameters
    nu
     $nu
   fluid density
     $fluidDensity
#  turn on gravity
  gravity
     $gravity 0 0 
    done
#
 cfl $cfl
#
  boundary conditions
   all=slipWall
   plug(0,0)=noSlipWall
   if( $option eq "rigidBodyPiston" ) { $cmd ="userDefinedBoundaryData\n known solution\n done"; }else{ $cmd=""; }
   square(1,0)=outflow, pressure(1.*p+0.*p.n=0.) $cmd
   # For testing a singular p-equation, make a closed container
   if( $option eq "closed" ){ $cmd="square(1,0)=noSlipWall"; }else{ $cmd="#"; }
   $cmd
   #  if( $option eq "rigidBodyPiston" ) { $cmd="square(1,0)=dirichletBoundaryCondition"; }else{ $cmd="#"; }
   #  $cmd
  done
#
  boundary conditions...
   # for light moving bodies: (1=turn on for moving bodies, 2=turn on for all walls (for testing)
   moving body pressure BC $mbpbc
   moving body pressure BC coefficient $mbpbcc
  done
# 
  if( $restart eq "" ){ $iccmds = "uniform flow\n" . "p=0, u=0., v=0\n"; }\
  else{ $iccmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
  if( $option eq "rigidBodyPiston" && $restart eq "" ) { $iccmds="OBIC:known solution"; }
#
  initial conditions
    $iccmds
  exit
  plot and always wait
 # no plotting
  debug
    $debug
  continue
#
  plot:p
# 
  $go


