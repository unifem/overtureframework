#==============================================================
# cgcns example: a moving cylinder in a channel (specified motion)
# Usage:
#   cgcns cicMove -g=<grid-name> -amr=[0|1]  -l=<> -ratio=<> -simulateMotion=[0|1|2]  -memoryCheck=[0|1] ...
#         -move=[shift|oscillate|rotate|off]  -freq=<> -xShift=<val>
#
# -simulateMotion : 
#   0 = solve and move grids
#   1 = move and regenerate grids only
#   2 = move grids only
# -freq: for -move=rotate, this is the number of rotations per sec.
#
# Examples:
#     cgcns cicMove -g=cice2.order2
#    
# Parallel: ** something funny with the parallel version of Ogen when iw=2 **fix me**
#   srun -N1 -n2 -ppdebug $cgcnsp cicMove -g=cic2e -fullGridGenFreq=1
#   totalview srun -a -N1 -n2 -ppdebug $cgcnsp cicMove -g=cic2e -fullGridGenFreq=1
#   mpirun -np 1 $cgcnsp cicMove -g=cic2e -fullGridGenFreq=1
#   mpirun -np 1 $cgcnsp cicMove -g=cice1.order2 -fullGridGenFreq=1
#   mpirun -np 1 $cgcnsp cicMove -g=cice2.order2 -fullGridGenFreq=1
#   srun -N1 -n2 -ppdebug $cgcnsp cicMove -g=cice2.order2 -fullGridGenFreq=1 -interpWidth=3
#   srun -N1 -n4 -ppdebug $cgcnsp cicMove -g=cice4.order2 -fullGridGenFreq=1 -interpWidth=3
# amr:
#   mpirun -np 1 $cgcnsp cicMove -g=cice2.order2 -fullGridGenFreq=1 -amr=1 -interpWidth=3
#   srun -N1 -n2 -ppdebug $cgcnsp cicMove -g=cice2.order2 -fullGridGenFreq=1 -amr=1 -interpWidth=3
#   srun -N1 -n2 -ppdebug -memcheck_all $cgcnsp noplot cicMove -g=cice2.order2 -fullGridGenFreq=1 -amr=1 -tf=.5 -go=go
#   totalview srun -a -N1 -n2 -ppdebug $cgcnsp cicMove -g=cice2.order2 -fullGridGenFreq=1 -amr=1 -tf=.5 
#=============================================================
#
# --- set default values for parameters ---
# 
$cnsVariation="godunov"; $ts="pc"; $show=" ";
$grid="cic.hdf"; $backGround="backGround"; $bcn="slipWall"; $uInflow=.0; $checkForWallHeating=0; 
$mg="square"; $mt="shift"; $vg0=0.; $vg1=0.; $vg2=0.; $fullGridGenFreq=10; $simulateMotion=0;
$tFinal=1.; $tPlot=.1; $cfl=.9; $mu=.0; $Prandtl=.72; $thermalExpansivity=.1; 
$noplot=""; $debug = 0; $move="shift"; $xShift=-1.;  $freq=1.; $ramp=0.; $bcOption=0;
$tz = "none"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.; $dtMax=.5; 
$order = 2; $fullSystem=0; $go="halt"; 
$ad=0.; $ad2=0; $ad22=2.; $interpWidth=2; $memoryCheck=0; $restart="";
$amr=0; $ratio=2;  $nrl=2; $nbz=2; $tol=.001; 
$pi = 4.*atan2(1.,1.);
# 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, \
 "cnsVariation=s"=>\$cnsVariation,"memoryCheck=i"=>\$memoryCheck,"move=s"=>\$move,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "nu=f"=>\$nu,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "go=s"=>\$go,"fullGridGenFreq=i"=>\$fullGridGenFreq,\
 "noplot=s"=>\$noplot,"dtMax=f"=>\$dtMax,"bcn=s"=>\$bcn,"vg0=f"=>\$vg0,"vg1=f"=>\$vg1,"vg2=f"=>\$vg2,\
 "dtMax=f"=>\$dtMax,"bc=s"=>\$bc,"mg=s"=>\$mg,"mt=s"=>\$mt,"ts=s"=>\$ts,"amr=i"=>\$amr,\
 "interpWidth=i"=>\$interpWidth,"simulateMotion=f"=>\$simulateMotion,"uInflow=f"=>\$uInflow,\
 "freq=f"=>\$freq,"bcOption=i"=>\$bcOption,"restart=s"=>\$restart,"xShift=f"=>\$xShift,\
  "checkForWallHeating=i"=>\$checkForWallHeating,"ad=f"=>\$ad,"l=i"=> \$nrl,"r=i"=> \$ratio );
# -------------------------------------------------------------------------------------------------
$kThermal=$mu/$Prandtl;   # check this 
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $amr eq "0" ){ $amr="turn off adaptive grids"; }else{ $amr="turn on adaptive grids"; }
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
if( $simulateMotion eq 0 ){ $simulateMotion="solve and move grids"; }elsif( $simulateMotion eq 1 ){ $simulateMotion="move and regenerate grids only"; }else{ $simulateMotion="move grids only"; }
# 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
#
# specify the overlapping grid to use:
$grid
  $cnsVariation
  $simulateMotion
  done
if( $memoryCheck ne 0 ){ $cmd="turn on memory checking"; }else{ $cmd="#"; }
$cmd
# -- time stepping method:
  $ts
# 
  turn off twilight
#
#  do not use iterative implicit interpolation
#
  final time $tFinal
  times to plot $tPlot
  dtMax $dtMax
# 
 no plotting
 plot and always wait
#
  show file options
    compressed
     open
       $show
    frequency to flush
      2
    exit
 # no plotting
#****************************
 # There can be trouble if the grid moves too fast
if( $move eq "off" ){ $cmd="turn off moving grids"; }else{ $cmd="turn on moving grids"; }
  $cmd
* ---
 frequency for full grid gen update $fullGridGenFreq
#
  specify grids to move
  if( $move eq "shift" || $move eq "off" ){ $cmds="translate\n 1. 0. 0.\n $xShift"; }\
  elsif( $move eq "oscillate" ){ $cmds ="oscillate\n 1. .5 0\n  1.\n .25\n 0."; }\
  else{ $cmds = "rotate\n 0. 0. 0.\n $freq $ramp"; }
  $cmds
#- # rotate and shift to test slip-wall acceleration BC
#-       matrix motion
#-           translate along a line
#-           point on line: 0 0 0
#- ##          tangent to line: 1. 0 0
#-           tangent to line: 1. 1. 0
#-           edit time function
#- ##            linear parameters: 0, 1 (a0,a1)
#-             $vel=sqrt(2.);
#-             linear parameters: 0, $vel (a0,a1)
#-           exit
#- #
#-         add composed motion
#-         rotate around a line
#-         point on line: 0 0 0
#-         tangent to line: 0 0 1.
#- #
#-         edit time function
#-           $a1 = $freq*2.*$pi; 
#-           linear parameters: 0, $a1 (a0,a1)
#- #          linear parameters: 0, 0. (a0,a1)
#-           exit
#-         exit
#- #
#-       exit
#
    Annulus
 # square
    done
  done
#**************************
if( $interpWidth eq 2 ){ $cmds = "reduce interpolation width\n 2"; }else{ $cmds ="#"; }
$cmds
#****
*****  Here we optionally turn on AMR *******
  $amr
  order of AMR interpolation
      2
  error threshold
     $tol
  regrid frequency
    $regrid=$nbz*$ratio;
    $regrid
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
      $ratio
    default number of refinement levels
      $nrl 
    number of buffer zones
      $nbz
    grid efficiency
      .7
  exit
*****
  boundary conditions
 # all=noSlipWall uniform(T=.3572)
   all=slipWall 
   if( $uInflow >0. ){ $cmds="square(0,0)=superSonicInflow uniform(r=1.,u=$uInflow,e=1.786,s=0)\n"\
                            . "square(1,0)=superSonicOutflow"; } else{ $cmds="#"; }
   $cmds
 # square(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119,s=0)
 # square(1,0)=superSonicOutflow
    square(0,1)=slipWall
    square(1,1)=slipWall
#
#-    all=superSonicInflow uniform(r=1.,u=1.,v=1.,e=1.786,s=0)
#-    Annulus=slipWall
    done
#
#
  OBPDE:slip wall boundary condition option $bcOption
  OBPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad
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
   artificial diffusion $ad $ad $ad $ad $ad $ad $ad $ad
   check for wall heating $checkForWallHeating
  done
#**************
  cfl
   $cfl
#*****************
 debug
   $debug
#
if( $restart eq "" ){ $cmds = "uniform flow\n r=1. u=$uInflow v=0. e=1.786 s=0."; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
continue
$go
