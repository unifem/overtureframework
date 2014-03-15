#**************************************************************
#  Forced piston problem
#
#  cgcns -noplot plug -g=plug8.hdf -tp=.5 -tf=1. -go=go
#    determineErrors: t=1.000e+00, maxNorm errors: [rho,u,v,T]=[3.74e-03,2.67e-03,0.00e+00,7.63e-04]
#    determineErrors: t=1.000e+00, l1-norm errors: [rho,u,v,T]=[1.52e-04,1.10e-04,0.00e+00,3.10e-05]
#  cgcns -noplot plug -g=plug16.hdf -tf=.5 -go=go
#    determineErrors: t=1.000e+00, maxNorm errors: [rho,u,v,T]=[2.40e-03,1.72e-03,0.00e+00,4.91e-04]
#    determineErrors: t=1.000e+00, l1-norm errors: [rho,u,v,T]=[6.20e-05,4.48e-05,0.00e+00,1.27e-05]
# 
#  cgcns plug -g=nonPlug8.hdf -tp=.01 -tf=1. -go=halt
# 
#     Use 'plotStuff createPlug.cmd' to generate plug4.m, plug4Solution.m, etc. 
#     for $ob/doc/pistonCompare.m
#*************************************************************
#
$tFinal=1.; $tPlot=.1; $cfl=.9; $debug=0; $bcOption=4; $newts=0; $projectRigidBody=0; $addedMass=0; 
$moveOn="turn on moving grids"; $moveOff="turn off moving grids"; $move=$moveOff;
$ratio=4; $levels=2; $amrTol=.01; 
$amrOff = "turn off adaptive grids";
$amrOn = "turn on adaptive grids";
$amr=$amrOff;
$mass=2.;   # mass of the piston
$height=1.;
$bf =0.; # exact solution needs to be given the body force too
# Body force coeff's
$bfx0=0.; $bfx1=0.; $bfx2=0.; $bfx3=0.;
# tolerances for the exact solution:
$rtole=1.e-6; $atole=1.e-9; 
# 
$en="l1";
#
$rbScheme="implicitRungeKutta"; $rbOrder=2; $addedMass=0; 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=> \$nrl,"r=i"=> \$ratio, "tf=f"=>\$tFinal,"debug=i"=> \$debug, \
            "tp=f"=>\$tPlot, "xStep=s"=>\$xStep, "bg=s"=>\$backGround,"show=s"=>\$show,"go=s"=>\$go,\
            "cnsVariation=s"=>\$cnsVariation,"bcOption=i"=> \$bcOption, "rho0=f"=>\$rho0,"p0=f"=>\$p0,\
            "gridToMove=s"=>\$gridToMove,"pp=f"=>\$pp,"en=s"=>\$en,"mass=f"=>\$mass,"bf=f"=>\$bf,\
            "newts=i"=>\$newts, "projectRigidBody=i"=>\$projectRigidBody,\
            "bfx0=f"=>\$bfx0,"bfx1=f"=>\$bfx1,"bfx2=f"=>\$bfx2,"bfx3=f"=>\$bfx3,"rtole=f"=>\$rtol,"atole=f"=>\$atole,\
            "rbScheme=s"=>\$rbScheme,"rbOrder=i"=>\$rbOrder,"addedMass=i"=>\$addedMass );
# -------------------------------------------------------------------------------------------------
if( $cnsVariation eq "godunov" ){ $pdeVariation="compressible Navier Stokes (Godunov)"; }
if( $cnsVariation eq "jameson" ){ $pdeVariation="compressible Navier Stokes (Jameson)"; }   #
if( $cnsVariation eq "nonconservative" ){ $pdeVariation="compressible Navier Stokes (non-conservative)";}  #
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
# 
if( $en eq "max" ){ $errorNorm="OBTZ:maximum norm"; }
if( $en eq "l1" ){ $errorNorm="OBTZ:l1 norm"; }
if( $en eq "l2" ){ $errorNorm="OBTZ:l2 norm"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
#
# 
# $gridName = "plug1.hdf"; $show="plug1.show";  $tPlot=.1; $debug=1; 
# $gridName = "plug2.hdf"; $show="plug2.show";  $tPlot=.01; $debug=31; 
# gridName = "plug2.hdf"; $show="plug2.show";  
#---
#* $norm=$l2norm;
# ----
# $gridName = "plug4.hdf"; $show="plug4.show";  $tPlot=.5;  $matlab="plug4Forced.m"; 
# $gridName = "plug8.hdf"; $show="plug8.show"; $tPlot=.5; $matlab="plug8Forced.m"; 
# $gridName = "plug16.hdf"; $show="plug16.show"; $tPlot=.5;
#
# $gridName = "noPlug4.hdf"; $show=" ";  $tPlot=.5;  $matlab="noPlug4Forced.m"; 
#
# -- for the movie ---
# $gridName = "plug16.hdf"; $show="forcedPistion16Movie.show"; $tPlot=.2;
#
# -- AMR ---
# $gridName = "plug4.hdf"; $show="plug4.show";  $tPlot=.1;  $amr=$amrOn; $amrTol=.001; 
#
$grid
#
#  Either Jameson or Godunov should work
#  compressible Navier Stokes (Jameson)
compressible Navier Stokes (Godunov)
#   one step
#  -- turn off slope limiters:
#    define integer parameter SlopeLimiter 0
  exit
# 
  turn off twilight
#
#  do not use iterative implicit interpolation
#
  final time (tf=)
   $tFinal
#
  times to plot (tp=)
    $tPlot
 no plotting
#plot and always wait
#
  show file options
    compressed
    open
      $show
    frequency to flush
      10
    exit
 # no plotting
#****
# time stepper:
$newts
# -- set projectRigidBody true if we use the new algorithm for light rigid bodies
project rigid body interface $projectRigidBody
#
# ----------- norm to use:
$errorNorm
#
# -- do this for now:
#*  frequency for full grid gen update
#*    1
 # There can be trouble if the grid moves too fast
  turn on moving grids
  specify grids to move
      debug
        0
      rigid body
        mass
          $mass
        # Note: centre of mass should be correct to added mass torque terms are zero
        moments of inertia
          1.e10
        initial centre of mass
           0. 0.5
        body force
          $bf 0. 0.  # OLD WAY 
        body force x: $bfx0, $bfx1, $bfx2, $bfx3 (coeffs of time poly)
        #
        position is constrained to a line
          1. 0 0 
        rotation is fixed
        #
        # Choose time stepping scheme for the rigid body:
        # leapFrogTrapezoidal
        # implicitRungeKutta
        $rbScheme
        added mass $addedMass
        order of accuracy: $rbOrder
        log file: rigidBody.log
        debug: 1
        done
 # new way: 101101 
        choose grids by share flag
         100
        done
 #   plug
 # specify faces, specify body force and/or torque
 # specify faces
 #   1
 #   2
 #   done
 #   done
  done
#**************************
#   reduce interpolation width
#     2
#****
# 
  boundary conditions
 #  all=slipWall
    all=symmetry
    plug=symmetry
    plug(0,0)=slipWall
#    square(0,0)=superSonicInflow uniform(r=2.6069,T=.943011,v=0.,u=0.694444,s=0.0)
#    square(0,0)=superSonicInflow uniform(r=1.4,T=.714286,v=0.,u=0.,s=0.0)
    done
#********************************************************
#   choose the new slip wall BC
#      1=slipWallPressureEntropySymmetry
#      2=slipWallTaylor
#      3=slipWallCharacteristic
#*  OBPDE:slip wall boundary condition option 1
#   OBPDE:slip wall boundary condition option 2
  OBPDE:slip wall boundary condition option $bcOption
  OBPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad
#********************************************************
#********************************************************
 boundary conditions...
   $orderOfExtrap=3;
    order of extrap for 2nd ghost line $orderOfExtrap
    order of extrap for interp neighbours 3
 done
#
#****************************************************
OBTZ:user defined known solution
# --- NEW WAY
 piston motion
   # specified motion piston
   pressure driven piston
   pressure and body forced piston
   mass: $mass
   area: $height
   rho0: 1.4
   p0: 1
   gamma: 1.4
   body force:  $bfx0, $bfx1, $bfx2, $bfx3 (bf0,bf1,bf2,bf3)
   $pistonFinalTime = $tFinal*2.; # piston final time should be larger 
   tFinal: $pistonFinalTime
   ag, pg: -0.333333, 3 (specified motion: g=ag*t^pg)
   $pistonTol = $mass*$rtole + $atole;  # we need to solve to a finer tol for low mass
   tolerance: $pistonTol
 exit
# ----OLD WAY ---
#-  forced piston motion
#-  # Mass and Height:
#-     $mass $height
# 
 done
# -----------------
#  OBTZ:l1 norm
#****************************************************
#
  pde parameters
    mu
     0.0
    kThermal
     0.0
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
#**************
  cfl
   $cfl
#*  OBPDE:exact Riemann solver
# OBPDE:Roe Riemann solver
# OBPDE:HLL Riemann solver
  OBPDE:Godunov order of accuracy 2
#*****************
 debug
   $debug
#=================================================================
#
  $amr
#   save error function to the show file
  show amr error function
  order of AMR interpolation
      2
  error threshold
     $amrTol
  regrid frequency
    $regrid=2*$ratio; 
    $regrid=1; 
    $regrid
  change error estimator parameters
    default number of smooths
      1
    set scale factors     
      2 1 1 1 1 
    done
    weight for first difference
      .1
    weight for second difference
      1.
    exit
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $levels
    number of buffer zones
      2
    grid efficiency
      .7 .5 
  exit
#=================================================================
#
  initial conditions
    $rho0=1.4; $p0=1.;
    $T0=$p0/$rho0;
    OBIC:uniform state r=$rho0, u=0, v=0, w=0, T=$T0
    OBIC:assign uniform state
  continue
 continue
#
$go


#
  initial conditions
 # This shock has speed 1.5
    OBIC:step function
    x=-4.75
    T=.943011, v=0., u=.694444, r=2.6069
#    T=.714286, v=0., u=0., r=1.4
    $r0=1.4; $T0=1./$r0; 
    T=$T0, v=0., u=0., r=$r0
  continue
continue
#
#
contour
  ghost lines 0
  x-r 60
  exit
# 

continue
continue

Point (-1.037e-01,4.759e-05,-8.425e-01) : r = 8.5314e-01 from grid 1 (transform[grid=1]), (i1,i2,i3)=(0,0,0)

          contour
            set view:0 0.299094 0.244713 0 5.41145 1 0 0 0 1 0 0 0 1
            wire frame (toggle)
            set view:0 0.317687 0.327577 0 70.4574 1 0 0 0 1 0 0 0 1
            query value -1.036848e-01 4.758781e-05 -8.424960e-01
            query value -1.036848e-01 4.758781e-05 -8.424960e-01
            reset:0
            line plots
              specify lines
              1 201
              -.1037 .5 1.5 .5
                r
                add u
                add T
                add p
                add rhoTrue
                add uTrue
                add TTrue
                add rhoErr
                add uErr
                add TErr
                add x0
                save results to a matlab file
                lightPistonMass10em6.m
                exit this menu


# Here we save the solution and errors to a matlab file
      contour
        line plots
          specify lines
          1 101
 # The piston position should be ??  > -.38
          -.365 .5 1.25 .5
            add u
            add T
            add p
            add rhoErr
            add uErr
            add TErr
            add x0
            save results to a matlab file
            $matlab


movie mode
finish


