*
*  cgsm: pulse initial conditions
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
*     cgsm pulse -g=square40 -pv=nc -diss=1. -tp=.01 -tf=10. -x0=.5 -y0=.5 
*     cgsm pulse -g=square80 -pv=nc -diss=1. -dissOrder=4 -tp=.01 -tf=10. -x0=.5 -y0=.5 
*     cgsm pulse -g=sice3.order2.hdf -pv=nc -diss=1. -dissOrder=4 -tp=.01 -tf=10. -x0=0. -y0=0. 
*     cgsm pulse -g=sice3.order2.hdf -pv=c -diss=1. -tp=.01 -tf=10. -x0=0. -y0=0. 
*
*     cgsm pulse -g=cice2.order2.hdf -pv=nc -filter=1 -tp=.01 -tf=10. -x0=1. -y0=0. -bc=sf
*     cgsm pulse -g=cice2.order2.hdf -pv=nc -filter=1 -tp=.01 -tf=10. -x0=1. -y0=0. -bc=sf -lambda=100.  [ trouble]
*     cgsm pulse -g=cice2.order2.hdf -pv=c -filter=1 -tp=.01 -tf=10. -x0=1. -y0=0. -bc=sf -lambda=100.  [ OK ]
*     cgsm pulse -g=cice2.order2.hdf -pv=g -filter=0 -tp=.01 -tf=20. -x0=1. -y0=0. -bc=sf -lambda=100.  [ OK with ad -> now ok ad=0]
*     cgsm pulse -g=cice2.order2.hdf -pv=c -filter=1 -tp=5. -tf=200. -x0=1. -y0=0. -bc=sf -lambda=100. -diss=50. -dissOrder=4 [stable to t=200]
*   -- 3D: 
*     cgsm pulse -g=box20.hdf -pv=nc -diss=1. -tp=.01 -tf=10. -x0=.5 -y0=.5 -z0=.5
*     cgsm pulse -g=box20.hdf -pv=c -diss=1. -tp=.01 -tf=10. -x0=.5 -y0=.5 -z0=.5 -bc=sf 
*     cgsm pulse -g=box40.hdf -pv=c -diss=1. -tp=.01 -tf=10. -x0=.5 -y0=.5 -z0=.5 -bc=sf -dsf=.2 
*     cgsm pulse -g=sib3.hdf -pv=nc -diss=1. -tp=.01 -tf=10. -x0=1.25 -y0=0. 
* 
*  -- special solution: 1d eigenmode:
*     cgsm pulse -g=squarenp20.order2 -pv=nc -diss=1. -tp=.1 -tf=10. -ic=special
* 
*     cgsm pulse -g=square40p -pv=nc -diss=1. -tp=.01 -tf=10. -x0=.5 -y0=.5 
* 
*    -- invariant "scale-rotation" mode: 
*     cgsm pulse -g=square20.order2 -pv=nc -diss=1. -tp=.1 -tf=10. -ic=special -specialOption=invariant -bc=sf
*     cgsm pulse -g=square20.order2 -pv=c -diss=1. -tp=.1 -tf=10. -ic=special -specialOption=invariant -bc=sf
* 
*  cgsm pulse -g=square5.hdf -pv=c -diss=0. -tp=.01 -tf=10. -x0=0.5 -y0=0.5 -debug=15
*
*  Godunov examples:
*     cgsm pulse -g=square5 -pv=g -tp=.05 -tf=10. -x0=0.5 -y0=0.5
*     cgsm pulse -g=square40 -pv=g -tp=.05 -tf=10. -x0=0.5 -y0=0.5 -godunovOrder=1
*     cgsm pulse -g=square40 -pv=g -tp=.05 -tf=10. -x0=0.5 -y0=0.5 -godunovOrder=1 -bc=sf -cfl=.5   *note cfl* 
*     cgsm pulse -g=cice2.order2 -pv=g -tp=.05 -tf=10. -x0=1. -y0=0. -bc=sf -cfl=.75
* 
*  -- special solution: 1d eigenmode:
*     cgsm pulse -g=squarenp20.order2 -pv=g -tp=.01 -tf=10. -ic=special
* 
*  Hemp example:
*     cgsm pulse -g=sice3.order2.hdf -pv=h -tp=.01 -tf=10. -x0=0. -y0=0.
* 
*  example with specified motion of the boundary:
*     cgsm pulse -g=sice3.order2.hdf -pv=c -diss=1. -tp=.1 -tf=10. -bc=ellipseDeform -ic=zero
*
# -- SVK example:
#   cgsm pulse -g=square40 -model=nonlinear -pv=g -model=nonlinear -godunovType=2 -amp=.01 -dsf=10. -tp=.01 -tf=10. -x0=.5 -y0=.5 
#
* parallel:
*     mpirun -np 2 $cgsmp pulse -g=square20 -pv=nc -diss=1. -dissOrder=4 -tp=.01 -tf=10. -x0=.5 -y0=.5 
*     mpirun-wdh -np 2 $cgsmp pulse -g=square20 -pv=nc -diss=1. -dissOrder=4 -tp=.01 -tf=10. -x0=.5 -y0=.5 
* totalview srun -a -N1 -n2 -ppdebug $cgsmp pulse -g=square20 -pv=nc -diss=1. -dissOrder=4 -tp=.01 -tf=10. -x0=.5 -y0=.5 
*     mpirun-wdh -np 2 $cgsmp pulse -g=box20.hdf -pv=nc -diss=1. -tp=.5 -tf=10. -x0=.5 -y0=0. 
* 
* --- set default values for parameters ---
* 
$tFinal=10.; $tPlot=.05; $backGround="square"; $cfl=.9; $bc="sf"; $pv="nc";
$ic="gaussianPulse";  $amp=.05; $dsf=1.; $exponent=50.; $x0=.5; $y0=.5; $z0=.5; $specialOption="default"; 
$noplot=""; $grid="rectangle80.ar10"; $mu=1.; $lambda=1.; $godunovOrder=2;
$debug = 0;  $tPlot=.1;  $bc="d"; $cons=1; $dsf=0.4; 
$diss=0.; $dissOrder=2;  # for NC scheme
$tsdiss=.1; $tsdissdt=0.; # Tangential stress dissipation for Godunov
$filter=0; $filterFrequency=1; $filterOrder=6; $filterStages=2; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
$ad2=0.; # second-order art. diss for Godunov
$ad4=1.;$ad4dt=0.;  # fourth-order art. diss for Godunov
$order = 2; $go="halt"; $show=" "; $model="linear";
$godunovType=0; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
"tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bc=s"=>\$bc,"ic=s"=>\$ic,"go=s"=>\$go,"noplot=s"=>\$noplot,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"x0=f"=>\$x0,"y0=f"=>\$y0,"z0=f"=>\$z0,\
  "pv=s"=>\$pv,"exponent=f"=>\$exponent,"godunovOrder=f"=>\$godunovOrder,"specialOption=s"=>\$specialOption,\
  "dsf=f"=>\$dsf,"filter=i"=>\$filter,"filterFrequency=i"=>\$filterFrequency,"filterOrder=i"=>\$filterOrder,\
  "filterStages=i"=>\$filterStages,"ad2=f"=>\$ad2,"ad4=f"=>\$ad4,"ad4dt=f"=>\$ad4dt,"model=s"=>\$model,"godunovType=i"=>\$godunovType,\
  "amp=f"=>\$amp,"dsf=f"=>\$dsf,"tsdiss=f"=>\$tsdiss,"tsdissdt=f"=>\$tsdissdt  );
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
# dissipation $diss
# order of dissipation $dissOrder
SMPDE:artificial diffusion $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2
SMPDE:fourth-order artificial diffusion $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4
SMPDE:fourth-order dt dissipation $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt
SMPDE:tangential stress dissipation $tsdiss $tsdissdt
SMPDE:displacement dissipation $tsdiss
* 
cfl $cfl
use conservative difference $cons
* 
plot divergence 1
plot vorticity 1
initial conditions options...
#Special initial condition option: $specialOption
#$ic 
* gaussianPulseInitialCondition
* Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)
close initial conditions options
# -- test user defined initial conditions:
    initial conditions options...
    OBIC:user defined...
      pulse
        $x0 $y0 $z0 $amp $exponent
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
plot stress 0
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
erase
displacement
  displacement scale factor $dsf
exit
$go
