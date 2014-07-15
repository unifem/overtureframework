#==============================================================
# Cgcns: shock hitting a flexible beam
#
# Examples:
#     cgcns beamInAChannel -g=beamInAChanellGride2.order2
#=============================================================
#
# --- set default values for parameters ---
# 
$cnsVariation="godunov"; $ts="pc"; $show=" ";
$grid="beamInAChanellGride2.order2.hdf"; $bcn="slipWall"; $uInflow=.1; 
$fullGridGenFreq=10;
$tFinal=1.; $tPlot=.05; $cfl=.9; $mu=.0; $Prandtl=.72; $thermalExpansivity=.1; 
$noplot=""; $debug = 0;  
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$ad2=0; $ad22=2.; 
$addedMass=0; $ampProjectVelocity=0;  $delta=100.; $E=10.; $bdebug=0; 
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "cnsVariation=s"=>\$cnsVariation,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "nu=f"=>\$nu,"cfl=f"=>\$cfl, "go=s"=>\$go,"fullGridGenFreq=i"=>\$fullGridGenFreq,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"bcn=s"=>\$bcn,\
 "dtMax=f"=>\$dtMax,"bc=s"=>\$bc,"mg=s"=>\$mg,"mt=s"=>\$mt,"ts=s"=>\$ts,"amr=i"=>\$amr,"addedMass=i"=>\$addedMass,"delta=f"=>\$delta,\
 "bdebug=i"=>\$bdebug,"E=f"=>\$E,"ampProjectVelocity=i"=>\$ampProjectVelocity );
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
# -- for added mass algorithm:
  use added mass algorithm $addedMass
  project added mass velocity $ampProjectVelocity
*   
  turn on moving grids
  specify grids to move
      deforming body
        user defined deforming body
          elastic beam
          $I=1.;  $rhoBeam=$delta; $length=1.; $thick=.2; $pNorm=1.; 
          $angle=90.; # $Pi*.5; 
          elastic beam parameters...
            number of elements: 11
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
* ---
 frequency for full grid gen update $fullGridGenFreq
# 
***************************
  reduce interpolation width
    2
*****
  boundary conditions
    all=slipWall
#    bcNumber1=superSonicInflow uniform(r=2.6069,T=.943011,u=0.694444,v=0.,s=0.0)
    bcNumber1=superSonicOutflow
   done
*
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
*
  initial conditions
    step function
    x=-.5
    T=.943011, u=.694444, v=0., r=2.6069
    T=.714286, u=0., v=0., r=1.4
  continue
continue
*
$go
