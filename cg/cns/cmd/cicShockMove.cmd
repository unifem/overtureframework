#==============================================================
# cgcns example: shock hitting a moving cylinder (with AMR)
#
# Examples:
#     cgcns cicShockMove -g=cice2.order2
#     cgcns cicShockMove -g=cice2.order2 -amr=1
#    
# Parallel:
#   mpirun -np 1 $cgcnsp cicShockMove -g=cic2e -fullGridGenFreq=1
#   mpirun -np 1 $cgcnsp cicShockMove -g=cice1.order2 -fullGridGenFreq=1
#   mpirun -np 1 $cgcnsp cicShockMove -g=cice2.order2 -fullGridGenFreq=1
#   totalview srun -a -N1 -n1 -ppdebug $cgcnsp cicShockMove -g=cice2.order2 -fullGridGenFreq=1
#   srun -N1 -n2 -ppdebug $cgcnsp cicShockMove -g=cice2.order2 -fullGridGenFreq=1
#   srun -N1 -n4 -ppdebug $cgcnsp cicShockMove -g=cice4.order2 -fullGridGenFreq=1 
#=============================================================
#
# --- set default values for parameters ---
# 
$cnsVariation="godunov"; $ts="pc"; $show=" ";
$grid="cic.hdf"; $backGround="backGround"; $bcn="slipWall"; $uInflow=.1; 
$mg="square"; $mt="shift"; $vg0=0.; $vg1=0.; $vg2=0.; $fullGridGenFreq=10;
$tFinal=1.; $tPlot=.05; $cfl=.9; $mu=.0; $Prandtl=.72; $thermalExpansivity=.1; 
$noplot=""; $debug = 0;  
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$ad2=0; $ad22=2.; 
$amr=0;
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "cnsVariation=s"=>\$cnsVariation,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "go=s"=>\$go,"fullGridGenFreq=i"=>\$fullGridGenFreq,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"bcn=s"=>\$bcn,"vg0=f"=>\$vg0,"vg1=f"=>\$vg1,"vg2=f"=>\$vg2,\
 "dtMax=f"=>\$dtMax,"bc=s"=>\$bc,"mg=s"=>\$mg,"mt=s"=>\$mt,"ts=s"=>\$ts,"amr=i"=>\$amr );
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
          .25 
        moments of inertia
          1.
        initial centre of mass
           0. 0.
        done
        Annulus
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
    square(0,0)=superSonicInflow uniform(r=2.6069,T=.943011,u=0.694444,v=0.,s=0.0)
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
***************
*  cfl
*   .5 
*    .95
*   OBPDE:exact Riemann solver
* OBPDE:Roe Riemann solver
* OBPDE:HLL Riemann solver
*   OBPDE:Godunov order of accuracy 2
******************
*  debug
*    1
*
  initial conditions
    step function
    x=-1.
    T=.943011, u=.694444, v=0., r=2.6069
    T=.714286, u=0., v=0., r=1.4
  continue
continue
*

movie mode
finish
