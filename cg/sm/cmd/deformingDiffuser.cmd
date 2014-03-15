*
*  cgsm: deforming diffuser known solution
*
* Usage: (not all options implemented yet)
*   
*  cgsm [-noplot] pulse -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -dissOrder=<> -order=<2/4> -debug=<num> ...
*                       -bc=[d|sf|slip] -bg=<backGround> -pv=[nc|c|g|h] -godunovOrder=[1|2] ...
*                       -stressRelaxation=[0|2|4] -relaxAlpha=<> -relaxDelta=<> -go=[run/halt/og]
* 
*  -bc : boundary conditions: -bc=d : displacement, -bc=sf :stress-free, 
*        -bc=ellipseDeform : specified motion of the boundary
*  -ic : initial conditions, ic=gaussianPulse, ic=zero, ic=special
*  -diss : coeff of artificial diffusion 
*  -go : run, halt, og=open graphics
*  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
#  -stressRelaxation : turn on stress-strain relaxation, 2=2nd-order approx, 4=4th order
* 
* Examples:
*   cgsm deformingDiffuser -g=deformingDiffuserSolidGrid4.order2 -pv=nc -diss=10. -tp=.1 
*   cgsm deformingDiffuser -g=deformingDiffuserSolidGrid16.order2 -pv=nc -diss=10. -tp=.1 
*
* Godunov examples: (use stress relaxation for steady state)
*   cgsm deformingDiffuser -g=deformingDiffuserSolidGrid4.order2 -pv=g -stressRelaxation=4 -ad=10. -tp=.2 
*   cgsm deformingDiffuser -g=deformingDiffuserSolidGrid4.order2 -pv=g -stressRelaxation=4 -ad=0. -tp=.1 
*   cgsm deformingDiffuser -g=deformingDiffuserSolidGrid16.order2 -pv=g -stressRelaxation=4 -tp=.2 
* 
* --- set default values for parameters ---
* 
$tFinal=10.; $tPlot=.05; $backGround="square"; $cfl=.9; $bc="sf"; $pv="nc";
$ic="zero";  $exponent=10.; $x0=.5; $y0=.5; $z0=.5; $specialOption="default"; 
$noplot=""; $grid="rectangle80.ar10"; $rho=10.; $mu=10.; $lambda=10.; $godunovOrder=2;
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $bc="d"; $cons=1; $dsf=0.4; 
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
$ad=0.; # art. diss for Godunov
$order = 2; $go="halt"; $show=" ";
$stressRelaxation=0; $relaxAlpha=0.1; $relaxDelta=0.1; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "dissOrder=i"=>\$dissOrder,"tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bc=s"=>\$bc,"ic=s"=>\$ic,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0,\
  "pv=s"=>\$pv,"exponent=f"=>\$exponent,"godunovOrder=f"=>\$godunovOrder,"specialOption=s"=>\$specialOption,\
  "dsf=f"=>\$dsf,"filter=i"=>\$filter,"filterFrequency=i"=>\$filterFrequency,"filterOrder=i"=>\$filterOrder,\
  "filterStages=i"=>\$filterStages,"ad=f"=>\$ad,"stressRelaxation=f"=>\$stressRelaxation,"relaxAlpha=f"=>\$relaxAlpha,\
  "relaxDelta=f"=>\$relaxDelta );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $pv eq "nc" ){ $pv = "non-conservative"; $cons=0; }
if( $pv eq "c" ){ $pv = "conservative"; $cons=1; }
if( $pv eq "g" ){ $pv = "godunov"; }
if( $pv eq "h" ){ $pv = "hemp"; }
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
linear elasticity
$pv
 continue
* 
modifiedEquationTimeStepping
* 
SMPDE:rho $rho
SMPDE:lambda $lambda
SMPDE:mu $mu 
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:stressRelaxation $stressRelaxation
SMPDE:relaxAlpha $relaxAlpha
SMPDE:relaxDelta $relaxDelta
*
* ----- trig IC's ----
* twilightZoneInitialCondition
* trigonometric
* TZ omega: 2 2 2 2 (fx,fy,fz,ft)
* -----------------------------
#** 
  twilight zone options...
   OBTZ:known solution from a show file
   # show file name: /home/henshaw.0/Overture/ogshow/deformingDiffuserSolid.show
   # show file name: /home/henshaw.0/people/don/Petsc/displacementsTest101x41.show
   # show file name: /home/henshaw.0/people/don/Petsc/displacementsTest501x201.show
   show file name: /home/henshaw.0/people/don/Petsc/displacementsTest2001x801.show
   assign solution from show file
   done
close forcing options
* 
* 
apply filter $filter
if( $filter eq 1 ){ $cmds = "filter order $filterOrder\n filter frequency $filterFrequency\n filter iterations 1\n filter coefficient 1. \n  filter stages $filterStages\n explicit filter\n  exit"; }else{ $cmds = "#"; }
$cmds
* 
final time $tFinal
times to plot $tPlot
* -- reduce interpolation width for godunov --
if( $pv eq "godunov" ){ $cmds = "reduce interpolation width\n 2"; }else{ $cmds="#"; }
$cmds
*
boundary conditions
  all=displacementBC
  elasticStrip(0,0)=slipWall
  elasticStrip(1,0)=slipWall
  bcNumber100=tractionBC, userDefinedBoundaryData
    # pressure from data points
    # include deformingDiffuserSolidSurfacePressure.dat
    traction from data points
    # include deformingDiffuserSolidSurfaceTraction.dat
    # include /home/henshaw.0/people/don/Petsc/tractionTest101x41.dat
    # include /home/henshaw.0/people/don/Petsc/tractionTest501x201.dat
    include /home/henshaw.0/people/don/Petsc/tractionTest2001x801.dat
  done
#
  * ---
*!  all=displacementBC , userDefinedBoundaryData
*!    ellipse deform
*!    .25 1.
*!  done
  * --- 
done  
# -- pin the upper left corner 
# Diffuser: 
# u=-1.7248e-10
# v= -1.9634e-04
# s11=7.2563e-03
# s12=3.0073e-06
# s22=2.3228e-05   
pin corners or edges
  0 0 1 -1 1
  #     u1,      u2,       v1, v2,   s11,       s12,        s22,    
  -1.7248e-10 -1.9634e-04  0.   0. 7.2563e-03 3.0073e-06  2.3228e-05
done
#
displacement scale factor $dsf
dissipation $diss
order of dissipation $dissOrder
SMPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad $ad
* 
cfl $cfl
use conservative difference $cons
* 
plot divergence 1
plot vorticity 1
initial conditions options...
knownSolutionInitialCondition
* Special initial condition option: $specialOption
* $ic 
close initial conditions options
*
$checkErrors=1;
check errors $checkErrors
plot errors $checkErrors
* -- trouble saving show file in parallel with check errors -- sequences have trouble
* check errors 0
* plot errors 0
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
  OBPSF:frequency to flush 10
exit
***********************************
continue
* 
* erase
* displacement
* exit
$go
