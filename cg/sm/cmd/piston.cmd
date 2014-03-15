*
*  cgsm: compute the solid portion of the elastic piston solution
*
* Usage: (not all options implemented yet)
*   
*  cgsm [-noplot] piston -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -dissOrder=<> -order=<2/4> -debug=<num> ...
*                       -bc=[d|sf|slip|dirichlet] -bg=<backGround> -pv=[nc|c|g|h] -godunovOrder=[1|2] -godunovType=[0|1|2] ...
#                       -go=[run/halt/og]
* 
*  -bc : boundary conditions: -bc=dirichlet, -bc=sf (traction)
*        -bc=ellipseDeform : specified motion of the boundary
#  -godunovType : 0=linear, 2=SVK
*  -ic : initial conditions, ic=gaussianPulse, ic=zero, ic=special
*  -diss : coeff of artificial diffusion 
*  -go : run, halt, og=open graphics
*  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
*  -en : error norm, "max", "l1" or "l2"
* 
* Examples: (the pistonSolidGrid is made from bigSquare.cmd)
* 
*   cgsm piston -g=pistonSolidGridf8 -pv=nc -tp=.1 -ic=special -bc=dirichlet 
*   cgsm piston -g=pistonSolidGridf16 -pv=nc -tp=.1 -ic=special -bc=dirichlet 
*   cgsm piston -g=pistonSolidGridf32 -pv=nc -tp=.1 -ic=special -bc=dirichlet 
*     u0'=G1, v0=G2:
*       -->t=1.0000e-01 dt=1.1e-03 maxNorm errors:[2.1524e-10,0.0000e+00,], maxNorm(u):[1.34e-02,0.00e+00,]
*     v0=G: 
*     -->t=1.0000e-01 dt=1.1e-03 maxNorm errors:[5.8452e-04,2.3753e-04,], maxNorm(u):[7.96e-02,2.38e-04,]
*     -->t=5.0000e-01 dt=1.1e-03 maxNorm errors:[1.1307e-03,2.8720e-04,], maxNorm(u):[1.09e-01,2.87e-04,]
*   cgsm piston -g=pistonSolidGridf64 -pv=nc -tp=.1 -ic=special -bc=dirichlet 
*     -->t=1.0000e-01 dt=5.7e-04 maxNorm errors:[3.8037e-04,1.2274e-04,], maxNorm(u):[7.96e-02,1.23e-04,]
*     -->t=5.0000e-01 dt=5.7e-04 maxNorm errors:[6.7516e-04,1.5157e-04,], maxNorm(u):[1.09e-01,1.52e-04,]
* -- godunov:
*   cgsm piston -g=pistonSolidGridf16 -pv=g -tp=.1 -tf=10. -ic=special -bc=dirichlet 
*   cgsm piston -g=pistonSolidGridf32 -pv=g -tp=.1 -tf=10. -ic=special -bc=dirichlet 
*     u0'=G1, v0=G2:
*     -->t=1.0000e-01 dt=8.1e-04 maxNorm errors:[4.0024e-10,0.0000e+00,6.1901e-10,0.0000e+00,0.0000e+00,2.0634e-10,2.1673e-08,0.0000e+00,], maxNorm(u):[3.48e-01,0.00e+00,1.56e-01,0.00e+00,0.00e+00,5.20e-02,1.34e-02,0.00e+00,]
*   cgsm piston -g=pistonSolidGridf64 -pv=g -tp=.1 -tf=10. -ic=special -bc=dirichlet 
* 
* -- smoother motion: 
*   cgsm piston -g=pistonSolidGridf16 -pv=g -tp=.1 -tf=10. -ic=special -pp=4 -bc=dirichlet 
* 
* -- NOT periodic in y:
*   cgsm piston -g=pistonSolidGridfa16 -pv=g -tp=.1 -tf=10. -ic=special -pp=4 -bc=dirichlet 
* 
* --- set default values for parameters ---
* 
$k0=1.; $a0=.5; $b0=-.5; $godunovType=0; 
$tFinal=10.; $tPlot=.05; $backGround="square"; $cfl=.9; $bc="sf"; $pv="nc"; $ts="me";  $show=" ";
$filter=0; $filterFreq=1; 
$ic="special";  $exponent=10.; $x0=.5; $y0=.5; $z0=.5; $specialOption="pistonMotion"; 
$noplot=""; $grid="rectangle80.ar10"; $mu=1.; $lambda=1.; $godunovOrder=2;
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $bc="sf"; $cons=1; $dsf=0.1;
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
$order = 2; $go="run"; 
$en="max";
# piston motion: F(t)=-(a/p)*t^p
$ap=1.; $pp=3.;
* 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "dissOrder=i"=>\$dissOrder,"tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bc=s"=>\$bc,"ic=s"=>\$ic,"go=s"=>\$go,"noplot=s"=>\$noplot,"ts=s"=>\$ts,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"x0=f"=>\$x0,"y0=f"=>\$y0,"dsf=f"=>\$dsf,\
  "pv=s"=>\$pv,"exponent=f"=>\$exponent,"godunovOrder=f"=>\$godunovOrder,"specialOption=s"=>\$specialOption,\
  "en=s"=>\$en,"filter=i"=>\$filter,"filterFreq=i"=>\$filterFreq,"k0=f"=>\$k0,"a0=f"=>\$a0,"ap=f"=>\$ap,"pp=f"=>\$pp,\
  "godunovType=i"=>\$godunovType );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $pv eq "nc" ){ $pv = "non-conservative"; $cons=0; }
if( $pv eq "c" ){ $pv = "conservative"; $cons=1; }
if( $pv eq "g" ){ $pv = "godunov"; }
if( $pv eq "h" ){ $pv = "hemp"; }
if( $en eq "max" ){ $errorNorm="maximum norm"; }
if( $en eq "l1" ){ $errorNorm="l1 norm"; }
if( $en eq "l2" ){ $errorNorm="l2 norm"; }
*
if( $ts eq "me" ){ $ts = "modifiedEquationTimeStepping"; }
if( $ts eq "fe" ){ $ts = "forwardEuler"; }
if( $ts eq "ie" ){ $ts = "improvedEuler"; }
if( $ts eq "ab" ){ $ts = "adamsBashforth2"; }
* 
if( $bc eq "d" ){ $bc = "all=displacementBC"; }
if( $bc eq "sf" ){ $bc = "all=tractionBC"; }
if( $bc eq "slip" ){ $bc = "all=slipWall"; }
if( $bc eq "dirichlet" ){ $bc = "all=dirichletBoundaryCondition"; }
if( $bc eq "ellipseDeform" ){ $bc = "all=displacementBC , userDefinedBoundaryData\n ellipse deform\n .25 1.\n done"; }
if( $ic eq "gaussianPulse" ){ $ic="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 \n"; }
if( $ic eq "zero" ){ $ic = "zeroInitialCondition"; }; 
if( $ic eq "special" ){ $ic = "specialInitialCondition"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
* 
* $tFinal=10.; $tPlot=.05; $backGround="rectangle"; 
* $diss=0.; $cfl=.9;
* 
* $grid = "rectangle80.ar10"; $diss=10.; $tPlot=.2; $cfl=.5; 
*
* 
* Note: artificial dissipation is scaled by c^2
*
$grid
* 
* -new: set-up stage: 
linear elasticity
$pv
 continue
* 
* -- set the time-stepping method:
$ts
* 
apply filter $filter
if( $filter eq 1 ){ $cmds = "filter order 4\n filter frequency $filterFreq\n filter iterations 1\n filter coefficient 1. \n  filter stages 1\n explicit filter\n  exit"; }else{ $cmds = "#"; }
$cmds
*
* ----- trig IC's ----
* twilightZoneInitialCondition
* trigonometric
* TZ omega: 2 2 2 2 (fx,fy,fz,ft)
$errorNorm
* -----------------------------
close forcing options
* 
final time $tFinal
times to plot $tPlot
* 
SMPDE:lambda $lambda
SMPDE:mu $mu 
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:PDE type for Godunov $godunovType
*
boundary conditions
  $bc 
  bcNumber3=symmetry
  bcNumber4=symmetry
  * square(1,1)=tractionBC
  * square(0,1)=dirichletBoundaryCondition
  * square(0,1)=tractionBC
done  
*
debug $debug
*
displacement scale factor $dsf
dissipation $diss
order of dissipation $dissOrder
cfl $cfl
use conservative difference $cons
* 
plot divergence 1
plot vorticity 1
initial conditions options...
specialInitialCondition
Special initial condition option: $specialOption
# piston motion: F(t)=-(a/p)*t^p
$ap $pp 
# Gas properties: 
$gamma=1.4; $rhog=.1; $pg=$rhog/$gamma;  
$rhog $pg $gamma
close initial conditions options
*
$checkErrors=0;
if( $ic eq "specialInitialCondition" ){ $checkErrors=1; }
check errors $checkErrors
plot errors $checkErrors
*
* For displacement solvers plot velocity and stress: 
if( $pv eq "non-conservative" || $pv eq "conservative" ){ $plotCommands = "plot velocity 1\n plot stress 1"; }else{ $plotCommands="*"; }
$plotCommands
**********************************
show file options...
  OBPSF:compressed
  OBPSF:open
    $show
  * OBPSF:frequency to save 
  OBPSF:frequency to flush 50
exit
***********************************
continue
* 
  contour
    adjust grid for displacement 1
    plot:v1
    displacement scale factor 1
    exit
  contour
  exit

erase
displacement
exit
