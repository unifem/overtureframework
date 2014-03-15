*
*  cgsm: run examples of the hemp solver
*
* Usage: (not all options implemented yet)
*   
*  cgsm [-noplot] hemp -g=<name> -tf=<tFinal> -tp=<tPlot> -debug=<num> ...
*                      -pv=[nc|c|g|h] -go=[run/halt/og] -ts=[me|fe|ie]
* 
*  -bc : boundary conditions: -bc=d : displacement, -bc=sf :stress-free, 
*        -bc=ellipseDeform : specified motion of the boundary
*  -ic : initial conditions, ic=gaussianPulse, ic=zero, ic=special
*  -diss : coeff of artificial diffusion 
*  -go : run, halt, og=open graphics
*  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
*  -ts : time-stepping method, me=modified-equation, fe=forward-Euler, ie=improved-Euler, ab=adams-bashforth
*
* Examples:
*     cgsm hemp -g=nonSquare10 -pv=h -tp=.01 -tf=10. -bc=sf
*     cgsm hemp -g=nonSquare20 -pv=h -tp=.05 -tf=10. 
*     valgrindebug ../bin/cgsm noplot hemp -g=nonSquare10 -pv=h -tp=.01 -tf=10.
*     cgsm hemp -g=square80 -pv=h -tp=.01 -tf=10. -bc=sf
*
*  -- try improved Euler time stepping: 
*     cgsm hemp -g=nonSquare10 -pv=h -tp=.01 -tf=10. -bc=sf -ts=ie
* 
* --- set default values for parameters ---
* 
$tFinal=10.; $tPlot=.0001; $backGround="square"; $cfl=.25; $bc="sf"; $pv="h"; $Rg=8.314/27.; $yield=1.e10; 
$apr=0.0; $bpr=0.0; $cpr=0.0; $dpr=0.4; $hgFlag=2;
$ic="hempInitialCondition";  $exponent=10.; $x0=.5; $y0=.5; $z0=.5; 
$noplot=""; $grid="/home/banks20/Grids/square80.hdf"; $mu=1.; $lambda=1.; $godunovOrder=2;  $ts="me"; 
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $bc="d"; $cons=1; 
$dsf=-1.;   # this means plot the displacement without adding onto the grid
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
$order = 2; $go="halt"; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "dissOrder=i"=>\$dissOrder,"tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bc=s"=>\$bc,"ic=s"=>\$ic,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"x0=f"=>\$x0,"y0=f"=>\$y0,\
  "pv=s"=>\$pv,"exponent=f"=>\$exponent,"godunovOrder=f"=>\$godunovOrder,"ts=s"=>\$ts );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $pv eq "nc" ){ $pv = "non-conservative"; }
if( $pv eq "c" ){ $pv = "conservative"; $cons=1; }
if( $pv eq "g" ){ $pv = "godunov"; }
if( $pv eq "h" ){ $pv = "hemp"; }
*
if( $ts eq "me" ){ $ts = "modifiedEquationTimeStepping"; }
if( $ts eq "fe" ){ $ts = "forwardEuler"; }
if( $ts eq "ie" ){ $ts = "improvedEuler"; }
if( $ts eq "ab" ){ $ts = "adamsBashforth2"; }
* 
if( $bc eq "d" ){ $bc = "all=displacementBC"; }
if( $bc eq "sf" ){ $bc = "all=tractionBC"; }
if( $bc eq "ellipseDeform" ){ $bc = "all=displacementBC , userDefinedBoundaryData\n ellipse deform\n .25 1.\n done"; }
if( $ic eq "gaussianPulse" ){ $ic="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 \n"; }
if( $ic eq "zero" ){ $ic = "zeroInitialCondition"; }; 
if( $ic eq "special" ){ $ic = "specialInitialCondition"; $dsf=.1; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
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
* ----- trig IC's ----
* twilightZoneInitialCondition
* trigonometric
* TZ omega: 2 2 2 2 (fx,fy,fz,ft)
* -----------------------------
close forcing options
* 
final time $tFinal
times to plot $tPlot
* 
SMPDE:Rg $Rg
SMPDE:yield stress $yield
SMPDE:lambda $lambda
SMPDE:mu $mu 
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:EOS polynomial $apr $bpr $cpr $dpr
SMPDE:hourglass control $hgFlag
*
boundary conditions
  $bc
*  square(0,0)=displacementBC
*  square(0,0)=userDefinedBounaryData
  square(0,0)=displacementBC , userDefinedBoundaryData
    piston
    0.1 0.0
  done
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
cfl $cfl
use conservative difference $cons
* 
* plot divergence 1
* plot vorticity 1
initial conditions options...
$ic 
* Here is where we can choose different initial conditions for hemp:
* (This string is then used in assignHempInitialConditions)
OBIC:Hemp initial condition option: default
* gaussianPulseInitialCondition
* Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)
close initial conditions options
*
$checkErrors=0;
if( $ic eq "specialInitialCondition" ){ $checkErrors=1; }
check errors $checkErrors
plot errors $checkErrors
*
debug $debug
*
continue
* 
erase
displacement
exit
$go 
