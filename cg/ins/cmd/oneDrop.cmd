* ============================================================================
* cgins: one dropping cylinder in a channel (test low density body)
* Usage:
*    cgins [-noplot] oneDrop -g=<name> -tf=<> -tp=<> -bodyDensity=<> -inertial=<>  -relaxRigidBody=<> -alpha=<>
*
* -inertial = moment of inertial
* -relaxRigidBody : 1= relaxation is used for light bodies to stabilize the time stepping
* -alpha : relaxation parameter, proportional to mass/rho_fluid approx. 
*
* -- dropping cyl: I = (M/2) r^2 
*   cgins oneDrop -g=oneDrop1.hdf -tf=2. -tp=.1
*
* -- dropping stick:  note: volume=.0485, I = (M/12)( w^2 + d^2) = (M/12)*( .5^2 + .1^2 ) = (M/12)*( .26 ) = rho*.05*.26/12. rho*.00108333
*   cgins oneDrop -g=dropSticki1.order2 -tf=2. -tp=.01 -bodyDensity=2. -nc=10 -relaxRigidBody=1 -inertia=.00216667 -alpha=.3 [Note: 
* 
* -- Light body examples: (need to turn on force relaxation)
*    - V=1.963e-01, density=1.000e-01; M=1.963e-02
*   cgins oneDrop -g=oneDrop1.hdf -tf=2. -tp=.01 -bodyDensity=.1 -relaxRigidBody=1 -alpha=.1 -nc=10 [*ok*
*   cgins oneDrop -g=oneDrop1.hdf -tf=2. -tp=.01 -bodyDensity=.1 -relaxRigidBody=1 -alpha=.1 -nc=10 -ts=pc
*   cgins oneDrop -g=oneDrop1.hdf -tf=2. -tp=.01 -bodyDensity=.1 [*bad* : default time-stepping fails 
*
*   srun -N1 -n2 -ppdebug $cginsp oneDrop -freqFullUpdate=1 -g=oneDrope.hdf
* 
* ============================================================================
$grid="twoDrop.hdf";  $tFinal=1.; $tPlot=.05; $bodyDensity=3.;  $fluidDensity=1.; $go="halt";
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
 $ts="implicit"; $numberOfCorrections=1; 
$solver="choose best iterative solver"; 
$psolver="choose best iterative solver";  
$solver="yale"; $psolver="yale"; 
$relaxRigidBody=0; # set to 1 for "light bodies" which are otherwise unstable 
$alpha=.5; $inertia=1.; 
$rtolc=.02; $atolc=1.e-5;  # light bodies 
$mbpbc=0; $mbpbcc=1.; # fix for light bodies: mbpbc=1 
$detectCollisions=1; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"bodyDensity=f"=>\$bodyDensity,"inertia=f"=>\$inertia,\
  "nc=i"=>\$numberOfCorrections,"mbpbc=i"=>\$mbpbc,"mbpbcc=f"=>\$mbpbcc,"relaxRigidBody=i"=>\$relaxRigidBody,\
  "alpha=f"=>\$alpha,"rtolc=f"=>\$rtolc,"atolc=f"=>\$atolc,"detectCollisions=i"=>\$detectCollisions );
# -------------------------------------------------------------------------------------------------
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
* 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
* 
  $grid
* 
  incompressible Navier Stokes
  exit
*
  show file options
   open
      oneDrop.show
    frequency to flush
      4 1
  exit  
  turn off twilight zone
  degree in space
    1
  project initial conditions
*
  dtMax .1
*-----------------------------
  turn on moving grids
  detect collisions $detectCollisions
  specify grids to move
    rigid body
      density
        $bodyDensity
      moments of inertia
        $inertia
      *initial centre of mass
      *   -.25 -.75
     # relaxation is used for light bodies to stabilize the time stepping
      relax correction steps $relaxRigidBody
      # $alpha=$bodyDensity*.5; # guess for relaxation parameter; alpha should be <=1 
      debug: $debug
      force relaxation parameter: $alpha
      force relative tol: $rtolc
      force absolute tol: $atolc
      $beta=$alpha; 
      torque relaxation parameter: $beta
      torque relative tol: $rtolc
      torque absolute tol: $atolc
    done
    drop
   done
  done
# 
  frequency for full grid gen update $freqFullUpdate
#
  * use implicit time stepping
  $ts
  number of PC corrections $numberOfCorrections
  choose grids for implicit
     all=implicit
     * all=explicit
     * channel=implicit
     * drop=implicit
     channel=explicit
    done
#
  pressure solver options
     $solver
     # these tolerances are chosen for PETSc
     relative tolerance
       $rtolp
     absolute tolerance
       $atolp
    exit
# 
  implicit time step solver options
     $psolver
     # these tolerances are chosen for PETSc
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
# 
  pde parameters
    nu
      .025
   fluid density
     $fluidDensity
*  turn on gravity
  gravity
     0. -1. 0.
    done
  boundary conditions
    all=noSlipWall
    channel(1,1)=inflowWithVelocityGiven,  parabolic(d=.2,p=1,v=0.)
    channel(0,1)=outflow
   channel(0,1)=inflowWithVelocityGiven,  parabolic(d=.2,p=1,v=0.)
   channel(1,1)=outflow
    done
#
  boundary conditions...
   # for light moving bodies: (1=turn on for moving bodies, 2=turn on for all walls (for testing)
   moving body pressure BC $mbpbc
   moving body pressure BC coefficient $mbpbcc
  done
# 
  initial conditions
    uniform flow
     p=1.
  exit
  final time $tFinal
  times to plot $tPlot
  plot and always wait
 * no plotting
  debug
    1  63
  continue
*
  plot:p
* 
  $go



*
    grid
      raise the grid by this amount (2D) 1
#      raise the grid by this amount (2D) 2
    exit this menu



*  output resolution
*   512
* movie and save
