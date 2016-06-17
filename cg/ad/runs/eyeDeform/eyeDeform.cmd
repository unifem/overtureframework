#
# cgad: deforming eye example
#
# Usage:
#   
#  cgad [-noplot] eyeDeform -g=<name> -pde=[AD|TF] -tz=[poly|trig|none] -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
#         -go=<go/halt/og> -kappa=<value> -kapVar=[] -solver=<yale/best> -order=<2/4> -ts=[pc2|fe|im] ...
#         -gridToMove=<name> -a=<val> -b=<val> -ic=[tz|pulse] -bc=[d|n] -motion=[sinusoid|concentration|deformingEye|shift|rotate] ...
#         -moveGrids=[0|1]
# 
#   -ts : time-stepping, euler, adams2, implicit. 
# 
# Examples:
#
#
# --- set default values for parameters ---
# 
$pde="AD"; $tFinal=1.; $tPlot=.025; $cfl=.9; $kappa=.1; $kapVar="cons";  $kThermal=.1; $show = " "; 
$ts="pc2"; $noplot=""; $go="halt"; $a=1.; $b=1.; $c=0.;  
$debug = 0;  $tPlot=.1; $dtMax=.05; $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=2.; $fy=2.; $fz=1.; $ft=2.;
$order = 2; $ic="tz"; $bc="d"; $motion="sinusoid"; 
# 
$solver="yale"; $ogesDebug=0; $ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
$rtol=1.e-3; $atol=1.e-5;    # tolerances for the implicit solver
# $ksp="gmres"; 
#
$deformingGrid="share=100"; 
# $deformFrequency=2.; $deformAmplitude=1.; $deformationType="ice deform"; 
# $constantVolume=0; $volumePenalty=.5; 
$sbcl="d"; $sbcr="d"; # BC for surface, d=dirichlet, s=slide
#
$pi=atan2(1.,1.)*4.; 
$ampx=.0; $ampy=.0; $freqx=2.*$pi; $freqt=2.*$pi; 
$x0=.5; $y0=-.5; $z0=0.; $ampPulse=1.; $alphaPulse=40.;   # pulse parameters
$alpha=1.; $ue=.1;  # parameters for concentration dependent motion
$ampb=.1; # amplitude of the deforming eye motion
$eyeOption=0;  # 0=Gaussian, 1=ellipse
$bdfOrder=2; $implicitAdvection=0;
$evalGridAsNurbs=0; $nurbsDegree=3;
# 
$moveGrids=1; # 1=move
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "pde=s"=>\$pde, "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "kappa=f"=>\$kappa,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"moveGrids=i"=>\$moveGrids, \
 "ts=s"=>\$ts, "noplot=s"=>\$noplot, "go=s"=>\$go,"debug=i"=>\$debug,"a=f"=>\$a,"b=f"=>\$b,\
  "dg=s"=>\$deformingGrid,"dt=s"=>\$deformationType,"ampx=f"=>\$ampx,"ampy=f"=>\$ampy,"ic=s"=>\$ic,"bc=s"=>\$bc,\
  "motion=s"=>\$motion,"ampPulse=f"=>\$ampPulse,"ue=f"=>\$ue,"x0=f"=>\$x0,"y0=f"=>\$y0,"ampb=f"=>\$ampb,\
  "rtol=f"=>\$rtol,"atol=f"=>\$atol,"alphaPulse=f"=>\$alphaPulse,"eyeOption=i"=>\$eyeOption,"bdfOrder=i"=>\$bdfOrder,\
  "dtMax=f"=>\$dtMax,"implicitAdvection=i"=>\$implicitAdvection,"evalGridAsNurbs=i"=>\$evalGridAsNurbs,"nurbsDegree=i"=>\$nurbsDegree );
# -------------------------------------------------------------------------------------------------
if( $pde eq "AD" ){ $pdeName="advection diffusion"; }
if( $pde eq "TF" || $pde eq "thinFilm" ){ $pdeName="thin film equations"; }
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="turn on polynomial"; }
if( $tz eq "trig" ){ $tz="turn on trigonometric"; }
if( $tz eq "none" ){ $tz="turn off twilight zone"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $ts eq "pc2" ){ $ts = "adams PC"; }
if( $ts eq "fe" ){ $ts = "forward Euler"; }
if( $ts eq "im" ){ $ts = "implicit"; }
if( $ts eq "bdf" ){ $ts = "BDF"; }
if( $go eq "go" ){ $go = "movie mode\n finish"; }
if( $go eq "halt" ){ $go = "halt"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "savePlots" ){ $go = "#"; } # save plots
#
$grid
# 
  $pdeName
  if( $pdeName eq "thin film equations" ){ $cmd="number of components 2"; }else{ $cmd="#"; }
  $cmd
#
  exit
#
  show file options
 # compressed
    open
     $show
    frequency to flush
      5
    exit
# Twilight zone option: 
    $tz 
    frequencies (x,y,z,t)   $fx $fy $fz $ft
#
    $xPulse=-.5; $yPulse=-.15; $zPulse=0.;
    OBTZ:pulse center $xPulse $yPulse $zPulse
    OBTZ:pulse velocity 1 0 0
    OBTZ:pulse amplitude, exponent, power 1 60 1
#
#   On a translating grid: x(r,t) = a*r + b*t   --> Poly(x)*Poly(t) --> Poly(a*r+b*t)*Poly(t)
#     so TZ function is a higher degree in time. degreeSpace=1, degreeTime=1 should be exact
  degree in space $degreex
  degree in time $degreet
#
  final time $tFinal
  times to plot $tPlot
  dtMax $dtMax
  plot and always wait
 # no plotting
# 
  pde parameters
    kappa $kappa
    a $a
    b $b
    c $c
    # 
    OBPDE:variable diffusivity 0 
    OBPDE:variable advection 0
    treat advection implicitly $implicitAdvection
  done
#
#
  implicit time step solver options
   $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol;
   include $ENV{CG}/ins/cmd/ogesOptions.h
  exit
# 
  #  turn on moving grids
  use moving grids $moveGrids
  $xshift=1.; $yshift=0.; $zshift=0.; $speed=1.; 
  specify grids to move
    # translate
    #    $xshift $yshift $zshift
    #    $speed
     rotate
       # .5 .5 0.
        .0 .0 0.
        .5 0.
      $deformingGrid 
    done
  done
if( $move eq "rotate" ){ $cmd="turn on moving grids\n specify grids to move\n pause\nrotate\n 0. 0. 0 \n $rate 0.\n$gridToMove\n$gridToMove2\n done\n done"; }
# --- deforming grid: ----
#-   specify grids to move
#-     deforming body
#-       # evaluate the Hyperbolic grid as a Nurbs: 
#-       evaluate grid as Nurbs $evalGridAsNurbs
#-       nurbs degree: $degreeOfNurbs
#-       user defined deforming body
#-       user defined deforming surface
#-         $cmd="#"; 
#-         if( $motion eq "sinusoid" ){ $cmd="sinusoidal\n $ampx $ampy 0. $freqx $freqt"; }
#-         if( $motion eq "concentration" ){ $cmd="concentration motion\n $alpha $ue"; }
#-         $b0=.5;
#-         if( $motion eq "deformingEye" ){ $cmd="deforming eye\n $freqt $b0 $ampb $eyeOption"; }
#-         if( $motion eq "realDeformingEye" ){ $cmd="real deforming eye"; }
#-         $cmd
#-         exit
#-         #
#-         done
#-         if( $deformingGrid =~ /^share=/ ){ $deformingGrid =~ s/^share=//; \
#-                    $deformingGrid="choose grids by share flag\n $deformingGrid"; };
#-         $deformingGrid
#-      done
#-   done
#**************************
   $ts
   BDF order $bdfOrder
   choose grids for implicit
     all=implicit
#      all=explicit
#      stir=implicit
   done
# 
  debug $debug 
  boundary conditions
    $cmd="#"; 
    if( $bc eq "d" ){ $cmd="all=dirichletBoundaryCondition"; }
    if( $bc eq "n" ){ $cmd="all=neumannBoundaryCondition"; }
    $cmd
    # example of specifying user defined boundary values:
    # bcNumber4=neumannBoundaryCondition, userDefinedBoundaryData
   #   specified Neumann values
   #     # For now we set the RHS to the Neumann BC to the following value: 
   #     0. 
   #  done  
    # if( $tz ne "turn off twilight zone" ){ $cmd = "bcNumber4=dirichletBoundaryCondition"; }else{ $cmd="#"; }
    $cmd
  done
# 
  initial conditions
   $cmd="#";
   if( $ic eq "pulse" && $tz eq "turn off twilight zone" ){ $cmd ="OBIC:user defined...\n  pulse\n $x0 $y0 $z0 $ampPulse $alphaPulse\n exit"; }
   $cmd
  continue
# 
  continue
  contour
   wire frame (toggle)
  exit
 $go

# 
# -- plot results ---
line width scale factor:0 4
hardcopy vertical resolution:0 2048
hardcopy horizontal resolution:0 2048
# 
if( $motion eq "sinusoid" ){ $tsave=.2; }else{ $tsave=1; }
times to plot $tsave
contour
exit
$plotName = "cgadDeform_" . $motion . "0.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
pause
continue
$plotName = "cgadDeform_" . $motion . "1.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
pause
continue
$plotName = "cgadDeform_" . $motion. "2.ps"; 
hardcopy file name:0 $plotName
hardcopy save:0
pause
finish





