* ============================================================================
* cgins: two dropping cylinders in a channel (2d)
*
*   cgins twoDrop -g=twoDrop.hdf -tf=5. -tp=.1
*   cgins twoDrop -g=twoDropi4.order2.hdf -nu=.01 -tf=5. -tp=.1
*
* Parallel:
*   srun -N1 -n1 -ppdebug $cginsp twoDrop -freqFullUpdate=1 -g=twoDrope.hdf
*   srun -N1 -n2 -ppdebug $cginsp twoDrop -freqFullUpdate=1 -g=twoDrope.hdf
* 
* ============================================================================
$grid="twoDrop.hdf"; $tFinal=8.; $tPlot=.1; $go="halt"; 
$bodyDensity=3.;  $fluidDensity=1.; $nu=.025; $show="twoDrop.show"; 
$freqFullUpdate=10; # frequency for using full ogen update in moving grids 
$rtolp=1.e-3; $atolp=1.e-4;  # tolerances for the pressure solve
$rtol=1.e-4; $atol=1.e-5;    # tolerances for the implicit solver
$solver="choose best iterative solver"; 
$psolver="choose best iterative solver";  
$solver="yale"; $psolver="yale"; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "model=s"=>\$model,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround,"fullSystem=i"=>\$fullSystem, "go=s"=>\$go,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"project=i"=>\$project,"rf=i"=> \$refactorFrequency,"impGrids=s"=>\$impGrids,\
 "iv=s"=>\$implicitVariation,"dtMax=f"=>\$dtMax,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"imp=f"=>\$implicitFactor,\
 "rtol=f"=>\$rtol,"atol=f"=>\$atol,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"restart=s"=>\$restart,\
  "impFactor=f"=>\$impFactor,"freqFullUpdate=i"=>\$freqFullUpdate );
# -------------------------------------------------------------------------------------------------
# 
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
      $show
    frequency to flush
      4 
  exit  
  turn off twilight zone
  degree in space
    1
  project initial conditions
*
  dtMax .1
*-----------------------------
  turn on moving grids
  detect collisions 1
  specify grids to move
      rigid body
        density
          $bodyDensity
        moments of inertia
          1.
        initial centre of mass
           -.25 -.75
        done
        drop
       done
      rigid body
        density
          $bodyDensity
        moments of inertia
          1.
        initial centre of mass
          .25 .25
        done
        drop2
      done
  done
# 
  frequency for full grid gen update $freqFullUpdate
#
  * use implicit time stepping
  implicit
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
    nu $nu
   fluid density
     1.
*  turn on gravity
  gravity
     0. -1. 0.
    done
  boundary conditions
    all=noSlipWall
    channel(1,1)=inflowWithVelocityGiven,  parabolic(d=.2,p=1,v=0.)
    channel(0,1)=outflow
    done
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
  plot:v
*
    grid
      raise the grid by this amount (2D) 1
#      raise the grid by this amount (2D) 2
    exit this menu
$go
