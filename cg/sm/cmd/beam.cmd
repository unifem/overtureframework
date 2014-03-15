*
*  cgsm -- Model the elastic waves in a beam
*
* Usage: (not all options implemented yet)
*   
*  cgsm [-noplot] beam -g=<name> -tz=<poly/trig> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
*                    -bcn=[d/sf] -diss=<> -order=<2/4> -debug=<num> -bg=<backGround> -cons=[0/1] -go=[run/halt/og]
* 
*  -diss : coeff of artificial diffusion 
*  -bcn : d=displacementBC, sf=stress-free
*  -go : run, halt, og=open graphics
*  -cons : 1= conservative difference 
* 
* Examples:
*     cgsm beam -g=rectangle80.ar10 -cons=1 -diss=0 -tp=.01 -tf=5.
*     cgsm beam -g=rectangle160.ar10 -cons=1 -diss=0 -tp=.01 -tf=5.
* 
* 
* --- set default values for parameters ---
* 
$tFinal=1.; $cfl=.9; 
$noplot=""; $backGround="rectangle"; $grid="rectangle80.ar10"; $mu=1.; $lambda=1.;
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $bcn="d"; $cons=1; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
$order = 2; $go="run"; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bcn=s"=>\$bcn,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
* 
if( $bcn eq "d" ){ $bcn = "bc: all=displacementBC"; }
if( $bcn eq "sf" ){ $bcn = "bc: all=displacementBC\n bc: $backGround(0,0)=tractionBC\n bc: $backGround(1,0)=tractionBC"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
* 
* $tFinal=10.; $tPlot=.05; $backGround="rectangle"; 
* $diss=0.; $cfl=.9;
* 
* $grid = "rectangle80.ar10"; $diss=10.; $tPlot=.2; $cfl=.5; 
*
$grid
* -new: set-up stage: 
linear elasticity
 continue
* 
modifiedEquationTimeStepping
* 
final time $tFinal
times to plot $tPlot
cfl $cfl
use conservative difference $cons
*
*
boundary conditions
  all=tractionBC
  * $backGround(0,0)=displacementBC
  * $backGround(1,0)=displacementBC
done  
* 
initial conditions options...
  parabolicInitialCondition
close initial conditions options
* 
displacement scale factor 0.5
dissipation $diss
continue
* 
erase
displacement
exit
