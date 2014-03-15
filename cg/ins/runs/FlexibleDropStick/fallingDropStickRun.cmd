# ============================================================================
# cgins: one dropping flexible stick in a channel
# Usage:
#    cgins [-noplot] fallingDropStick -g=<name> -tf=<> -tp=<> -bodyDensity=<> 
#           -vIn=<> 
#
#   cgins fallingDropStick -g=dropSticki1.order2.hdf -nu=1e-2 -bodyDensity=10.0 g=dropSticki2.order2 -tf=5. -tp=.1 -nc=10 -vIn=.3 -show=fallingStick.show
# 
# ============================================================================
$grid="fallingStick.hdf";  $tFinal=1.; $tPlot=.05; $bodyDensity=3.;  $fluidDensity=1.; $go="halt"; $show=" "; $vIn=.1; $nu=.025; 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
 $ts="implicit"; $numberOfCorrections=1; 
$solver="choose best iterative solver"; 
$psolver="choose best iterative solver";  
$solver="yale"; $psolver="yale"; 
$mbpbc=0; $mbpbcc=1.; # fix for light bodies: mbpbc=1 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,"vIn=f"=>\$vIn,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate,"bodyDensity=f"=>\$bodyDensity,"inertia=f"=>\$inertia,\
  "nc=i"=>\$numberOfCorrections,"mbpbc=i"=>\$mbpbc,"mbpbcc=f"=>\$mbpbcc);
# -------------------------------------------------------------------------------------------------
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
# 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
$bodyDensity=$bodyDensity*1;
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
  degree in space
    1
  project initial conditions
#
  dtMax .1
#-----------------------------
$angleradians=-30.0*3.141592653589/180.0;
$beamX0 = -0.25*cos($angleradians);
$beamY0 = -0.25*sin($angleradians);
  turn on moving grids
  specify grids to move
    deforming body
        user defined deforming body
          elastic beam
          elastic beam parameters
            6.667e-7 20000.0 $bodyDensity 0.5 0.02 1.0 $beamX0 $beamY0 0.0
            5 2 2 0
          beam free motion
            0.0 0.0 $angleradians
          boundary parameterization
             1
          added mass relaxation factor
             0.5
        done
        choose grids by share flag
         5 
     done
  done
# 
  frequency for full grid gen update $freqFullUpdate
#
 # use implicit time stepping
  $ts
  number of PC corrections $numberOfCorrections
  choose grids for implicit
     all=implicit
 # all=explicit
 # channel=implicit
 # drop=implicit
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
     $nu
   fluid density
     $fluidDensity
#  turn on gravity
  gravity
     0. -1.0 0.
    done
  boundary conditions
   all=slipWall
   drop=noSlipWall
   channel(0,1)=inflowWithVelocityGiven,  parabolic(d=.2,p=1,v=$vIn)
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
 # no plotting
  debug
    1  63
  continue
#
  plot:p
# 
  $go



#
    grid
      raise the grid by this amount (2D) 1
#      raise the grid by this amount (2D) 2
    exit this menu



#  output resolution
#   512
# movie and save
