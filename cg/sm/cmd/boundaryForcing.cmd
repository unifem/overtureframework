*
*  cgsm: Example showing the application of forcing functions on the boundary
*
* Usage: (not all options implemented yet)
*   
*  cgsm [-noplot] boundaryForcing -g=<name> -tf=<tFinal> -tp=<tPlot> -diss=<> -dissOrder=<> -order=<2/4> -debug=<num> ...
#                    -force=[traction|pressure|Gaussian] -bca=[symmetry|slipWall]
*                    -pv=[nc|c|g|h]  -bg=<backGround> -cons=[0/1] -dsf=<> -go=[run/halt/og]
#                    -tractionForce=<>
* 
*  -bc : boundary conditions: -bc=d : displacement, -bc=sf :stress-free, 
*        -bc=ellipseDeform : specified motion of the boundary
#  -bca : bc on adjacent faces 
*  -ic : initial conditions, ic=gaussianPulseForcing, ic=zero
*  -diss : coeff of artificial diffusion 
*  -go : run, halt, og=open graphics
*  -cons : 1= conservative difference 
*  -dsf : displacement scale factor
*  -pv : "pde-variation" : nc=non-conservative, c=conservative, g=godunov, h=hemp
#  -godunovType : 0=linear, 2=SVK, 3=rotated-linear
* 
* Examples:
*     cgsm boundaryForcing -g=square10 -pv=nc -diss=1. -dissOrder=4 -tp=.01 -tf=10. -x0=.5 -y0=.5 
*     cgsm boundaryForcing -g=square80 -cons=0 -diss=1. -dissOrder=4 -tp=.01 -tf=10. -x0=.5 -y0=.5 
*     cgsm boundaryForcing -g=sice3.order2.hdf -cons=0 -diss=1. -dissOrder=4 -tp=.01 -tf=10. -x0=0. -y0=0. 
*     cgsm boundaryForcing -g=sice3.order2.hdf -cons=1 -diss=1. -tp=.01 -tf=10. -x0=0. -y0=0. 
*     cgsm boundaryForcing -g=box20.hdf -cons=0 -diss=1. -tp=.01 -tf=10. -x0=.5 -y0=0. 
*     cgsm boundaryForcing -g=sib3.hdf -cons=0 -diss=1. -tp=.01 -tf=10. -x0=1.25 -y0=0. 
* 
*  cgsm boundaryForcing -g=square5.hdf -cons=1 -diss=0. -tp=.01 -tf=10. -x0=0.5 -y0=0.5 -debug=15
*
*  example with specified motion of the boundary:
*     cgsm boundaryForcing -g=sice3.order2.hdf -cons=1 -diss=1. -tp=.1 -tf=10. -bc=ellipseDeform -ic=zero
*  Test forced traction BC's
*    cgsm boundaryForcing -g=square5np.hdf -cons=0 -diss=1. -tp=.01 -ic=zero
*    cgsm boundaryForcing -g=squarenp20.order2.hdf -cons=0 -diss=10. -tf=100. -tp=.01 -dsf=1. lambda=2. -ic=zero
*    cgsm boundaryForcing -g=boxnp10.order2.hdf -bg=box -cons=0 -diss=10. -tf=100. -tp=.01 -dsf=1. -ic=zero
*
* -- godunov
*    cgsm boundaryForcing -g=square20 -pv=g -tp=.1 -tf=10.
*
* -- hemp
*    cgsm boundaryForcing -g=square20 -pv=h -ts=ie -tp=.1 -tf=10.
*    cgsm boundaryForcing -g=squarenp10.order2 -pv=h -ts=ie -tp=.1 -tf=10.  [ periodic in y] 
*
* parallel:
*     mpirun -np 2 $cgsmp boundaryForcing -g=square20 -cons=0 -diss=1. -dissOrder=4 -tp=.01 -tf=10. -x0=.5 -y0=.5 
*     mpirun-wdh -np 2 $cgsmp boundaryForcing -g=square20 -cons=0 -diss=1. -dissOrder=4 -tp=.01 -tf=10. -x0=.5 -y0=.5 
* totalview srun -a -N1 -n2 -ppdebug $cgsmp boundaryForcing -g=square20 -cons=0 -diss=1. -dissOrder=4 -tp=.01 -tf=10. -x0=.5 -y0=.5 
*     mpirun-wdh -np 2 $cgsmp boundaryForcing -g=box20.hdf -cons=0 -diss=1. -tp=.5 -tf=10. -x0=.5 -y0=0. 
* 
* --- set default values for parameters ---
* 
$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=2.0; $cl=1.0; $hgFlag=2; $hgVisc=1.0e-2;
*$Rg=8.314/27.; $yield=1.e10; $basePress=0.0; $c0=0.0; $cl=0.0; $hgFlag=0; $hgVisc=1.0e-2;
$tFinal=10.; $tPlot=.05; $backGround="square"; $cfl=.9; $bc="slip"; $ic="zero"; $ts="me"; 
$exponent=10.; $x0=.5; $y0=.5; $z0=.5; $dsf=.4; $show=" "; $bca=""; 
$noplot=""; $grid="rectangle80.ar10"; $mu=1.; $lambda=1.; $rho=1.; 
$debug = 0;  $tPlot=.1; $diss=0.; $dissOrder=2; $cons=1; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=.5; $fy=$fx; $fz=$fx; $ft=$fx;
$order = 2; $go="halt"; 
$force="Gaussian"; 
$tractionForce=-1.; 
# 
$godunovType=0; $godunovOrder=2; 
$ad2=0.; $ad2dt=0.; $ad4=0.; $ad4dt=0.;  
$stressRelaxation=4; $relaxAlpha=.5; $relaxDelta=.5;
$tangentialStressDissipation=.5; $tangentialStressDissipation1=.5; # new 
#
$slopeLimiter=0;  # slope limiter for Godunov method
$characteristicUpwinding=0; 
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet,"diss=f"=>\$diss,\
 "dissOrder=i"=>\$dissOrder,"tp=f"=>\$tPlot, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "cfl=f"=>\$cfl, "bg=s"=>\$backGround,"bc=s"=>\$bc,"ic=s"=>\$ic,"go=s"=>\$go,"noplot=s"=>\$noplot,"pv=s"=>\$pv,\
  "mu=f"=>\$mu,"lambda=f"=>\$lambda,"dtMax=f"=>\$dtMax, "cons=i"=>\$cons,"dsf=f"=>\$dsf,"x0=f"=>\$x0,"y0=f"=>\$y0,\
  "ts=s"=>\$ts,"rho=f"=>\$rho,"force=s"=>\$force,"bca=s"=>\$bca,"godunovType=i"=>\$godunovType,\
   "stressRelaxation=f"=>\$stressRelaxation,"relaxAlpha=f"=>\$relaxAlpha,"relaxDelta=f"=>\$relaxDelta, \
   "tangentialStressDissipation=f"=>\$tangentialStressDissipation,"tractionForce=f"=>\$tractionForce,\
   "slopeLimiter=i"=>\$slopeLimiter,"characteristicUpwinding=i"=>\$characteristicUpwinding,\
   "ad4=f"=>\$ad4,"ad4dt=f"=>\$ad4dt,"ad2=f"=>\$ad2,"ad2dt=f"=>\$ad2dt,"godunovOrder=i"=>\$godunovOrder );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="polynomial"; }else{ $tz="trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $model eq "ins" ){ $model = "incompressible Navier Stokes"; }else\
                     { $model = "incompressible Navier Stokes\n Boussinesq model"; }
* 
if( $pv eq "nc" ){ $pv = "non-conservative"; $cons=0; }
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
if( $bc eq "slip" ){ $bc = "all=slipWall"; }
if( $bc eq "ellipseDeform" ){ $bc = "all=displacementBC , userDefinedBoundaryData\n ellipse deform\n .25 1.\n done"; }
if( $ic eq "gaussianPulse" ){ $ic="gaussianPulseInitialCondition\n Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 \n"; }
if( $ic eq "zero" ){ $ic = "zeroInitialCondition"; }; 
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
* set-up stage: 
linear elasticity
$pv
 continue
* 
$ts
* 
* ----- trig IC's ----
* twilightZoneInitialCondition
* trigonometric
* TZ omega: 2 2 2 2 (fx,fy,fz,ft)
* -----------------------------
close forcing options
* 
final time $tFinal
times to plot $tPlot
# 
SMPDE:rho $rho
SMPDE:lambda $lambda
SMPDE:mu $mu 
# SVK and other nonlinear models:
SMPDE:PDE type for Godunov $godunovType
SMPDE:stressRelaxation $stressRelaxation
SMPDE:relaxAlpha $relaxAlpha
SMPDE:relaxDelta $relaxDelta
SMPDE:tangential stress dissipation $tangentialStressDissipation  $tangentialStressDissipation1
#
SMPDE:Godunov order of accuracy $godunovOrder
SMPDE:slope limiting for Godunov $slopeLimiter
SMPDE:slope upwinding for Godunov $characteristicUpwinding
#
SMPDE:artificial diffusion $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2 $ad2  $ad2 $ad2 $ad2
SMPDE:second-order dt dissipation $ad2dt $ad2dt $ad2dt $ad2dt $ad2dt $ad2dt $ad2dt $ad2dt $ad2dt $ad2dt  $ad2dt $ad2dt $ad2dt
SMPDE:fourth-order artificial diffusion $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4 $ad4  $ad4 $ad4 $ad4 
SMPDE:fourth-order dt dissipation $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt $ad4dt  $ad4dt $ad4dt $ad4dt
#
# these next are for the hemp code:
SMPDE:Rg $Rg
SMPDE:yield stress $yield
SMPDE:base pressure $basePress
SMPDE:c0 viscosity $c0
SMPDE:cl viscosity $cl
SMPDE:hg viscosity $hgVisc
*
boundary conditions
$bc
$cmd="#"; 
if( $bca eq "symmetry" ){ $cmd="bcNumber3=symmetry\n bcNumber4=symmetry\n"; }
if( $bca eq "slipWall" ){ $cmd="bcNumber3=slipWall\n bcNumber4=slipWall\n"; }
if( $bca eq "displacement" ){ $cmd="bcNumber1=tractionBC\n bcNumber3=displacementBC\n bcNumber4=displacementBC\n"; }
$cmd
* all=tractionBC
* all=slipWall
$backGround(1,0)=tractionBC, userDefinedBoundaryData
 # amp, alpha x0 y0 z0 t0, p
 $cmd="#"; 
 if( $force eq "Gaussian" ){ $cmd="Gaussian forcing\n -1. 50. 1. .5 0. 1. 3."; }
 if( $force eq "traction" ){ $cmd="traction forcing\n $tractionForce 0. 0."; }
 if( $force eq "pressure" ){ $cmd="pressure force\n 1."; }
 $cmd
 done
done  
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
if( $pv eq "hemp" ){ $ic="hempInitialCondition\n OBIC:Hemp initial condition option: default\n"; }
$ic 
* gaussianPulseInitialCondition
* Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0)
close initial conditions options
*
#*********************************
show file options...
  OBPSF:compressed
  OBPSF:open
    $show
 # OBPSF:frequency to save 
  OBPSF:frequency to flush 100
exit
#**********************************
debug $debug
*
continue
* 
$go

#
#-  Gaussian forcing
#-     -1. 50. 1. .5 0. 1. 3.
#   traction forcing
#      -1. 0. 0.
   done 
*-square(1,0)=tractionBC, userDefinedBoundaryData
*-  traction forcing
*-     0. -1. 0.
*-  done 
*:all=tractionBC, userDefinedBoundaryData
*:  traction forcing
*:     1. 0. 0.
*:  done
*:  traction forcing
*:     1. 0. 0.
*:  done
*:  traction forcing
*:     1. 0. 0.
*:  done
*:  traction forcing
*:     1. 0. 0.
*:  done
* ---
*   all=displacementBC , userDefinedBoundaryData
*     test deform
*   done
*     test deform
*   done
*     test deform
*   done
*     test deform
*   done
* --- 


erase
displacement
exit
