*
*  cgsm: userDefined "rotation" initial conditions
*
* Usage: (not all options implemented yet)
*
*  cgsm [-noplot] pulse -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -dissOrder=<> -order=<2/4> -debug=<num> ...
*                       -amp=<> -exponent=<>
*                       -bc=[d|sf|slip] -bg=<backGround> -model=[linear|nonlinear] -pv=[nc|c|g|h] -godunovOrder=[1|2] -go=[run/halt/og]
*
*  -bc : boundary conditions: -bc=d : displacement, -bc=sf :stress-free, 
*        -bc=ellipseDeform : specified motion of the boundary
*  -ic : initial conditions, ic=gaussianPulse, ic=zero, ic=special
*  -diss : coeff of artificial diffusion 
*  -go : run, halt, og=open graphics
*  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
*
* Examples:
*     cgsm rotation -g=square5 -pv=g -tp=.05 -tf=10. -xc=0.5 -yc=0.5
*     cgsm rotation -g=square40 -pv=g -tp=.05 -tf=10. -xc=0.5 -yc=0.5 -godunovOrder=1
*     cgsm userICs -g=square40 -pv=g -tp=.05 -tf=10. -x0=0.5 -y0=0.5 -godunovOrder=1 -bc=sf -cfl=.5   *note cfl* 
*     cgsm userICs -g=cice2.order2 -pv=g -tp=.05 -tf=10. -x0=1. -y0=0. -bc=sf -cfl=.75
*
*  example with specified motion of the boundary:
*     cgsm userICs -g=sice3.order2.hdf -pv=c -diss=1. -tp=.1 -tf=10. -bc=ellipseDeform -ic=zero
*
# -- SVK example:
#   cgsm noplot rotation -g=square40 -pv=g -godunovType=2 -tp=.1 -tf=1. -xc=.5 -yc=.5 -bc=sf -show=rotation.show
#   cgsm noplot rotation -g=square40 -pv=g -godunovType=2 -tp=.1 -tf=10. -xc=.5 -yc=.5 -rate=1. -bc=sf -show=Showfiles/rotatingSquare.show
#   cgsm noplot rotation -g=annulus80 -pv=g -godunovType=2 -tp=.1 -tf=10. -xc=0. -yc=0. -rate=1. -bc=sf -show=Showfiles/rotatingAnnulus.show
#   cgsm noplot rotation -g=sici10.order2 -pv=g -godunovType=2 -tp=.1 -tf=10. -xc=0. -yc=0. -rate=1. -bc=sf -show=Showfiles/rotatingDisk.show
#
# -- successful SVK examples (1/25/12)
#  annulus
# cgsm noplot rotation -g=annulus40 -pv=g -godunovType=2 -tp=.1 -tf=10. -xc=0. -yc=0. -rate=1. -bc=sf -show=rotation.show -godunovOrder=2 -cfl=.8 -ad=1.
# cgsm noplot rotation -g=annulus80 -pv=g -godunovType=2 -tp=.1 -tf=10. -xc=0. -yc=0. -rate=1. -bc=sf -show=rotation.show -godunovOrder=2 -cfl=.8 -ad=1.
# cgsm noplot rotation -g=annulus160 -pv=g -godunovType=2 -tp=.1 -tf=10. -xc=0. -yc=0. -rate=1. -bc=sf -show=rotation.show -godunovOrder=2 -cfl=.8 -ad=1.  [dies at t=9.9]
#
#  disk
# cgsm noplot rotation -g=sici8.order2 -pv=g -godunovType=2 -tp=.1 -tf=10. -xc=0. -yc=0. -rate=1. -bc=sf -show=rotation.show -godunovOrder=2 -cfl=.8 -ad=2.
#
#  ellipse
# cgsm noplot rotation -g=ellipseSVK2 -pv=g -godunovType=2 -tp=.1 -tf=10. -xc=0. -yc=0. -rate=1. -bc=sf -show=rotation.show -godunovOrder=2 -cfl=.8 -ad=2.  [dies if -ad=1.]
# cgsm noplot rotation -g=ellipseSVKr2 -pv=g -godunovType=2 -tp=.1 -tf=10. -xc=-0.3 -yc=0.5 -rate=1. -bc=sf -show=rotation.show -godunovOrder=2 -cfl=.8 -ad=2.  [football]
#
#  square
# cgsm noplot rotation -g=square40 -pv=g -godunovType=2 -tp=.1 -tf=20. -xc=0.5 -yc=0.5 -rate=1. -bc=sf -show=rotation.show -godunovOrder=2 -cfl=.8 -ad=1.
# cgsm noplot rotation -g=square80 -pv=g -godunovType=2 -tp=.1 -tf=20. -xc=0.5 -yc=0.5 -rate=1. -bc=sf -show=rotation.show -godunovOrder=2 -cfl=.8 -ad=2.
#
*
* --- set default values for parameters ---
*
$tFinal=10.; $tPlot=.05; $backGround="square"; $cfl=.5; $bc="sf"; $pv="nc";
$rate=.1; $dsf=1.; $xc=.5; $yc=.5; $specialOption="default"; 
$noplot=""; $grid="rectangle80.ar10"; $mu=1.; $lambda=1.; $godunovOrder=2;
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $bc="d"; $cons=1; $dsf=0.4; $plotStress=0; 
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
$ad=0.; # art. diss for Godunov
$order = 2; $go="halt"; $show=" "; $model="linear";
$stressRelaxation=0; $godunovType=0;
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "dissOrder=i"=>\$dissOrder,"tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bc=s"=>\$bc,"ic=s"=>\$ic,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"xc=f"=>\$xc,"yc=f"=>\$yc,\
  "pv=s"=>\$pv,"godunovOrder=f"=>\$godunovOrder,"specialOption=s"=>\$specialOption,\
  "dsf=f"=>\$dsf,"filter=i"=>\$filter,"filterFrequency=i"=>\$filterFrequency,"filterOrder=i"=>\$filterOrder,\
  "filterStages=i"=>\$filterStages,"ad=f"=>\$ad,"model=s"=>\$model,"godunovType=i"=>\$godunovType,\
  "stressRelaxation=f"=>\$stressRelaxation,"rate=f"=>\$rate,"dsf=f"=>\$dsf,"plotStress=i"=>\$plotStress );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $pv eq "nc" ){ $pv = "non-conservative"; $cons=0; }
if( $pv eq "c" ){ $pv = "conservative"; $cons=1; }
if( $pv eq "g" ){ $pv = "godunov"; }
if( $pv eq "h" ){ $pv = "hemp"; }
if( $model eq "linear" ){ $model="linear elasticity"; }else{ $model="non-linear mechanics"; $godunovType=2; }
* 
if( $bc eq "d" ){ $bc = "all=displacementBC"; }
if( $bc eq "sf" ){ $bc = "all=tractionBC"; }
if( $bc eq "m" ){ $bc = "all=displacementBC\n Annulus(0,1)=tractionBC"; }
if( $bc eq "slip" ){ $bc = "all=slipWall"; }
if( $bc eq "ellipseDeform" ){ $bc = "all=displacementBC , userDefinedBoundaryData\n ellipse deform\n .25 1.\n done"; }
if( $ic eq "gaussianPulse" ){ $ic="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 \n"; }
if( $ic eq "zero" ){ $ic = "zeroInitialCondition"; }; 
if( $ic eq "special" ){ $ic = "specialInitialCondition"; $dsf=.1; }
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
$grid
*
* -new: set-up stage:
$model
# linear elasticity
$pv
 continue
*
modifiedEquationTimeStepping
* ----- trig IC's ----
* twilightZoneInitialCondition
* trigonometric
* TZ omega: 2 2 2 2 (fx,fy,fz,ft)
* -----------------------------
close forcing options
*
*
apply filter $filter
if( $filter eq 1 ){ $cmds = "filter order $filterOrder\n filter frequency $filterFrequency\n filter iterations 1\n filter coefficient 1. \n  filter stages $filterStages\n explicit filter\n  exit"; }else{ $cmds = "#"; }
$cmds
*
final time $tFinal
times to plot $tPlot
*
SMPDE:lambda $lambda
SMPDE:mu $mu
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:PDE type for Godunov $godunovType
SMPDE:slope limiting for Godunov 0
SMPDE:slope upwinding for Godunov 0
SMPDE:stressRelaxation $stressRelaxation
*
* -- reduce interpolation width for godunov --
if( $pv eq "godunov" ){ $cmds = "reduce interpolation width\n 2"; }else{ $cmds="#"; }
$cmds
*
boundary conditions
  $bc
  * ---
*!  all=displacementBC , userDefinedBoundaryData
*!    ellipse deform
*!    .25 1.
*!  done
  * ---
done
*
displacement scale factor $dsf
dissipation $diss
order of dissipation $dissOrder
* SMPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad
SMPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad $ad
*
cfl $cfl
use conservative difference $cons
*
plot divergence 1
plot vorticity 1
initial conditions options...
* gaussianPulseInitialCondition
* Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)
close initial conditions options
# -- test user defined initial conditions:
    initial conditions options...
    OBIC:user defined...
      rotation
        $xc $yc $rate
      exit
    close initial conditions options
*
$checkErrors=0;
if( $ic eq "specialInitialCondition" ){ $checkErrors=1; }
check errors $checkErrors
plot errors $checkErrors
* -- trouble saving show file in parallel with check errors -- sequences have trouble
check errors 0
plot errors 0
# plotCauchy stress:
plot stress $plotStress
*
debug $debug
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
  OBPSF:frequency to flush 1
exit
***********************************
continue
*
erase
displacement
  displacement scale factor $dsf
exit
$go
