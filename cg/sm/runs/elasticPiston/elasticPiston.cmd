#
#  cgsm -- Model an elastic piston for FSI problem with INS
#
# Usage:
#   
#  cgsm [-noplot] elasticPiston -g=<name> -pv=[nc|c|g|h] -tf=<tFinal> -tp=<tPlot> ...
#                    -bcn=[d/sf] -diss=<> -order=<2/4> -debug=<num> -bg=<backGround> -cons=[0/1] -go=[run/halt/og]
# 
#  -diss : coeff of artificial diffusion 
#  -bcn : d=displacementBC, sf=stress-free
#  -go : run, halt, og=open graphics
#  -cons : 1= conservative difference 
# 
#   FOS-G:
#    cgsm elasticPiston -g=square20.order2.hdf -pv=g -tp=.1 -tf=10. 
# 
# 
# --- set default values for parameters ---
# 
$tFinal=10.; $cfl=.9; $pv="g"; 
$noplot=""; $backGround="rectangle"; $grid="rectangle80.ar10"; $mu=1.; $lambda=1.; $rho=1.; 
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $bcn="d"; $cons=1; $flushFrequency=10;
$order = 2; $go="run"; 
$amp=.1; 
$rampOrder=2;  # number of zero derivatives at start and end of the ramp
$ra=.1; $rb=.6; # ramp interval -- actual interval shifted by Hbar/cp 
#
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"pv=s"=>\$pv,"diss=f"=>\$diss,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "rho=f"=>\$rho,"mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax,"amp=f"=>\$amp,\
  "rampOrder=i"=>\$rampOrder,"ra=f"=>\$ra,"rb=f"=>\$rb );
# -------------------------------------------------------------------------------------------------
if( $pv eq "nc" ){ $pv = "non-conservative"; $cons=0; }
if( $pv eq "c" ){ $pv = "conservative"; $cons=1; }
if( $pv eq "g" ){ $pv = "godunov"; }
if( $pv eq "h" ){ $pv = "hemp"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $bcn eq "d" ){ $bcn = "bc: all=displacementBC"; }
if( $bcn eq "sf" ){ $bcn = "bc: all=displacementBC\n bc: $backGround(0,0)=tractionBC\n bc: $backGround(1,0)=tractionBC"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
# 
# $tFinal=10.; $tPlot=.05; $backGround="rectangle"; 
# $diss=0.; $cfl=.9;
# 
# $grid = "rectangle80.ar10"; $diss=10.; $tPlot=.2; $cfl=.5; 
#
$grid
# -new: set-up stage: 
linear elasticity
$pv 
continue
# 
modifiedEquationTimeStepping
# 
final time $tFinal
times to plot $tPlot
cfl $cfl
use conservative difference $cons
#
SMPDE:lambda $lambda
SMPDE:mu $mu 
SMPDE:rho $rho
#
boundary conditions
  all=dirichletBoundaryCondition
##  rectangle(0,1)=displacementBC
#
#  all=tractionBC
#  bcNumber1=displacementBC
#  bcNumber2=displacementBC
 # $backGround(0,0)=displacementBC
 # $backGround(1,0)=displacementBC
done  
$Pi=4.*atan2(1.,1.);
$k=2.; $t0=0; $H=1.; $Hbar=.5; $rho=1.; 
$rhoBar=$rho; $lambdaBar=$lambda; $muBar=$mu;
# 
OBTZ:user defined known solution
 choose a common known solution
  bulk solid piston
   $amp,$k,$t0,$H,$Hbar,$rho,$rhoBar,$lambdaBar,$muBar
   $rampOrder $ra $rb
  done
 done
#
initial conditions options...
  knownSolutionInitialCondition
#*********************************
show file options...
  OBPSF:compressed
 # specify the max number of parallel hdf sub-files: 
  OBPSF:maximum number of parallel sub-files 8
  OBPSF:open
    $show
 # OBPSF:frequency to save 
  OBPSF:frequency to flush $flushFrequency
exit
#**********************************
close initial conditions options
# 
displacement scale factor 0.5
dissipation $diss
if( $pv eq "godunov" ){ $plotStress=0; }else{ $plotStress=1; }
plot stress $plotStress
check errors 1
plot errors 1
continue
# 
contour
  plot:v
  displacement scale factor 1
  adjust grid for displacement 0
exit
contour
exit
$go