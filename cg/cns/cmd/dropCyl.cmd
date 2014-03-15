#==============================================================
# cgcns example: cylinder falling under gravity.
#
# Usage:  cgcns dropCyl -g=<> -rbScheme=[leapFrogTrapezoidal|implicitRungeKutta] -rbOrder=[1|2|3|4]
#
# Examples:
#     cgcns dropCyl -g=cice2.order2 -mass=5.  [Ok drop falls
#     cgcns dropCyl -g=cice2.order2 -mass=1.  [OK oscillates up and down
#     cgcns dropCyl -g=cice2.order2 -mass=.1  [OK drop rises
#     cgcns dropCyl -g=cice2.order2 -mass=.01 [OK 
#     cgcns dropCyl -g=cice2.order2 -mass=.001 [BAD
#    
#  DIRK: 
#     cgcns dropCyl -g=cice2.order2 -mass=1. -rbScheme=implicitRungeKutta -rbOrder=2
#
#     cgcns dropCyl -g=cice2.order2 -mass=.005 -tp=.01 -debug=3 -show="dropCyl.show"
#     cgcns -noplot dropCyl -g=cice2.order2 -mass=.002 -tp=.01 -tf=.05 -debug=3 -show="dropCyl2.show" -go=go
#
#=============================================================
#
# --- set default values for parameters ---
# 
$cnsVariation="godunov"; $ts="fe"; $newts=0; $show=" ";
$grid="cic.hdf"; $backGround="backGround"; $bcn="slipWall"; $uInflow=.1; $bcOption=4;
$mg="square"; $mt="shift"; $vg0=0.; $vg1=0.; $vg2=0.; $fullGridGenFreq=10;
$tFinal=10.; $tPlot=.1; $cfl=.9; $mu=.0; $Prandtl=.72; $thermalExpansivity=.1; 
$noplot=""; $debug = 0;  
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$ad2=0; $ad22=2.; 
$amr=0;
$mass=5.; # mass of the body
$I1=1.; $I2=1; $I3=1.; # moments of inertial -- only I3 needed in 2d
$gravity = "0 -1. 0.";
$gravity = "0 0. 0.";
$rbScheme="leapFrogTrapezoidal"; $rbOrder=2; $addedMass=0; $projectRigidBody=0; 
$bfx =0.;  $bfy=0.; # body force 
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "cnsVariation=s"=>\$cnsVariation,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "go=s"=>\$go,"fullGridGenFreq=i"=>\$fullGridGenFreq,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"bcn=s"=>\$bcn,"vg0=f"=>\$vg0,"vg1=f"=>\$vg1,"vg2=f"=>\$vg2,\
 "dtMax=f"=>\$dtMax,"bc=s"=>\$bc,"mg=s"=>\$mg,"mt=s"=>\$mt,"ts=s"=>\$ts,"amr=i"=>\$amr,"mass=f"=>\$mass,"newts=i"=>\$newts,\
 "rbScheme=s"=>\$rbScheme,"rbOrder=i"=>\$rbOrder,"addedMass=i"=>\$addedMass,"projectRigidBody=i"=>\$projectRigidBody,\
 "bcOption=i"=>\$bcOption,"bfx=f"=>\$bfx,"bfy=f"=>\$bfy,"gravity=s"=>\$gravity,"I3=f"=>\$I3 );
# -------------------------------------------------------------------------------------------------
$kThermal=$mu/$Prandtl;   # check this 
if( $amr eq 1 ){ $amr="turn on adaptive grids"; }else{ $amr="turn off adaptive grids"; }
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $tz eq "poly" ){ $tz="turn on twilight zone\n turn on polynomial"; $cdv=0.; }
if( $tz eq "trig" ){ $tz="turn on twilight zone\n turn on trigonometric"; $cdv=0.; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $cnsVariation eq "godunov" ){ $cnsVariation="compressible Navier Stokes (Godunov)"; $ts="fe"; }
if( $cnsVariation eq "jameson" ){ $cnsVariation="compressible Navier Stokes (Jameson)"; }  
if( $cnsVariation eq "nonconservative" ){ $cnsVariation="compressible Navier Stokes (non-conservative)"; } 
if( $ts eq "fe" ){ $ts="forward Euler"; }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit"; }
if( $ts eq "pc" ){ $ts="adams PC"; }
if( $newts eq "1" ){ $newts = "use new advanceSteps versions"; }else{ $newts = "#"; }
# 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# specify the overlapping grid to use:
$grid
  $cnsVariation
  done
# -- time stepping method:
  $ts
# time stepper:
$newts
# -- set projectRigidBody true if we use the new algorithm for light rigid bodies
project rigid body interface $projectRigidBody
# 
  turn off twilight
*
*  do not use iterative implicit interpolation
*
  final time $tFinal
  times to plot $tPlot
*
  show file options
    compressed
    open
      $show
    frequency to flush
      1
    exit
  * no plotting
****
* turn on user defined output
*****************************
* ---
 frequency for full grid gen update $fullGridGenFreq
# 
* There can be trouble if the grid moves too fast
  turn on moving grids
  specify grids to move
      rigid body
        mass
          $mass
        moments of inertia
          $I3
        initial centre of mass
            0. 0.
        body force
          $bfx $bfy 0.
        #
        # Choose time stepping scheme for the rigid body:
        # leapFrogTrapezoidal
        # implicitRungeKutta
        $rbScheme
        added mass $addedMass
        order of accuracy: $rbOrder
        debug: 1
        done
#
#        Annulus
        choose grids by share flag
         5
       done
  done
***************************
  reduce interpolation width
    2
*****
  $amr
  order of AMR interpolation
      2
  error threshold
      .0005
  regrid frequency
     8 4  8
  change error estimator parameters
    set scale factors
      1 10000 10000 10000 10000
    done
    weight for first difference
    0.
    weight for second difference
    .03
    exit
    truncation error coefficient
    1.
    show amr error function
  change adaptive grid parameters
    refinement ratio
      2 4 2 4
    default number of refinement levels
      2 3 2
    number of buffer zones
      2
    grid efficiency
      .7
  exit
*****
  boundary conditions
    all=slipWall
    # square(0,0)=superSonicInflow uniform(r=2.6069,T=.943011,u=0.694444,v=0.,s=0.0)
    done
*
  pde parameters
    mu
     0.0
    kThermal
     0.0
    gravity
      $gravity
  done
  cfl $cfl
***************
*   .5 
*    .95
*   OBPDE:exact Riemann solver
* OBPDE:Roe Riemann solver
*  OBPDE:HLL Riemann solver
** OBPDE:Godunov order of accuracy 1
******************
#********************************************************
#   choose the new slip wall BC
#      1=slipWallPressureEntropySymmetry
#      2=slipWallTaylor
#      3=slipWallCharacteristic
#*  OBPDE:slip wall boundary condition option 1
#   OBPDE:slip wall boundary condition option 2
  OBPDE:slip wall boundary condition option $bcOption
##  OBPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad
#********************************************************
#********************************************************
 boundary conditions...
   $orderOfExtrap=3;
    order of extrap for 2nd ghost line $orderOfExtrap
    order of extrap for interp neighbours 3
 done
#
#****************************************************
*  debug
*    1
*
*  --- to avoid large initial sound waves, initialize the density to be the steady
*      stratified profile. 
  initial conditions 
    OBIC:user defined...
      $T0=1.; 
      gravitationally stratified
        * rho(y) = rho0*exp( gravity[1]/(Rg*T0) ( y - y0 ))
        $rho0=1.; $y0=0.; 
         $rho0 $y0 
      r=1. u=0. v=0. T=$T0
     exit
   exit
  continue
$go

  initial conditions
    $gamma=1.4;
    $rho0=$gamma; $p0=1.; $T0=$p0/$rho0;
    OBIC:uniform state r=$rho0, u=0, v=0, w=0, T=$T0
    OBIC:assign uniform state
  continue
continue
$go


continue



*

movie mode
finish
