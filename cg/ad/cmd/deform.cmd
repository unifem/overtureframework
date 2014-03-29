*
* cgad: moving grid examples
*
*
* Usage:
*   
*  cgad [-noplot] deform -g=<name> -tz=<poly/trig> -degreex=<> -degreet=<> -tf=<tFinal> -tp=<tPlot> ...
*         -go=<go/halt/og> -kappa=<value> -kapVar=[] -solver=<yale/best> -order=<2/4> -ts=[pc2|fe|im] ...
*         -gridToMove=<name> -move=<shift/rotate> -ref=<fixed/rigid> -a=<val> -b=<val>
* 
*   -ts : time-stepping, euler, adams2, implicit. 
*   -ref : reference frame
* 
* Examples:
#    cgad deform -g=circleDeform -dg=ice -nu=.05 -tf=2. -tp=.02 -go=halt 
#    cgad deform -g=circleDeform -dg=ice -nu=.05 -tf=2. -tp=.02 -ts=im -go=halt 
#
*
* --- set default values for parameters ---
* 
$tFinal=1.; $tPlot=.025; $cfl=.9; $kappa=.1; $kapVar="cons";  $kThermal=.1; $show = " "; 
$ts="pc2"; $noplot=""; $go="halt"; $a=1.; $b=1.; $c=0.;  
$debug = 0;  $tPlot=.1; $maxIterations=100; $tol=1.e-16; $atol=1.e-16; 
$tz = "poly"; $degreex=2; $degreet=2; $fx=1.; $fy=1.; $fz=1.; $ft=1.;
$order = 2; $gridToMove="square"; $move="shift"; $refFrame="fixed"; 
* 
$solver="yale"; $ogesDebug=0; $ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=3;
* $ksp="gmres"; 
*
$deformingGrid="ice"; $deformFrequency=2.; $deformAmplitude=1.; $deformationType="ice deform"; 
$rhoe=10.; $te=1.; $ke=1.; $be=0.1; $ad2e=5.; # ellastic shell parameters
$constantVolume=0; $volumePenalty=.5; 
$sbcl="d"; $sbcr="d"; # BC for surface, d=dirichlet, s=slide
# 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"degreex=i"=>\$degreex, "degreet=i"=>\$degreet, "kappa=f"=>\$kappa,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"move=s"=>\$move,"ref=s"=>\$ref, \
 "ts=s"=>\$ts, "noplot=s"=>\$noplot, "go=s"=>\$go,"debug=i"=>\$debug,"gridToMove=s"=>\$gridToMove,"a=f"=>\$a,"b=f"=>\$b,\
  "dg=s"=>\$deformingGrid,"dt=s"=>\$deformationType,"da=f"=>\$deformAmplitude,"df=f"=>\$deformFrequency,\
  "rhoe=f"=>\$rhoe,"te=f"=>\$te,"ke=f"=>\$ke,"be=f"=>\$be,"ad2e=f"=>\$ad2e, "constantVolume=i"=>\$constantVolume,\
  "volumePenalty=f"=>\$volumePenalty, );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $tz eq "poly" ){ $tz="turn on polynomial"; }else{ $tz="turn on trigonometric"; }
if( $order eq "2" ){ $order = "second order accurate"; }else{ $order = "fourth order accurate"; }
if( $ts eq "pc2" ){ $ts = "adams PC"; }
if( $ts eq "fe" ){ $ts = "forward Euler"; }
if( $ts eq "im" ){ $ts = "implicit"; }
if( $go eq "go" ){ $go = "movie mode\n finish"; }
if( $go eq "halt" ){ $go = " "; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $move eq "shift" ){ $move="translate\n  1. 0. 0.\n 1."; }
if( $move eq "rotate" ){ $move="rotate\n  0. 0. 0.\n .5 .0"; }
if( $ref eq "fixed" ){ $ref="fixed reference frame"; }
if( $ref eq "rigid" ){ $ref="rigid body reference frame"; }
*
$grid
* 
  convection diffusion
  $ref 
  * fixed reference frame
  * rigid body reference frame
*
  exit
  show file options
    * compressed
    open
     $show
    frequency to flush
      5
    exit
**  turn off twilight zone
**  project initial conditions
*
*   On a translating grid: x(r,t) = a*r + b*t   --> Poly(x)*Poly(t) --> Poly(a*r+b*t)*Poly(t)
*     so TZ function is a higher degree in time. degreeSpace=1, degreeTime=1 should be exact
  degree in space $degreex
  degree in time $degreet
*
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  * no plotting
* 
  pde parameters
    kappa $kappa
    a $a
    b $b
    c $c
    # 
    OBPDE:variable diffusivity 1
    OBPDE:variable advection 0
  done
* 
*****************************
  turn on moving grids
  specify grids to move
      deforming body
        user defined deforming body
          $deformationType
          deformation frequency
            $deformFrequency
          deformation amplitude
            $deformAmplitude
          change hype parameters 0
          # -- elastic shell parameters
          # elastic shell parameters
          #   # te : surface tension, ke=spring restoring force, be=velocity damping, ad2e=art-diss
          #   $rhoe $te $ke $be $ad2e
          #
          elastic shell density: $rhoe
          elastic shell tension: $te
          elastic shell stiffness: $ke
          elastic shell damping: $be
          elastic shell dissipation: $ad2e
          volume penalty parameter: $volumePenalty
          constant volume $constantVolume
          # note: Dirichlet BC will be over-ridden if periodic
          if( $sbcl eq "d" ){ $sbcl="Dirichlet"; }else{  $sbcl="slide"; }
          if( $sbcr eq "d" ){ $sbcr="Dirichlet"; }else{  $sbcr="slide"; }
          BC left: $sbcl
          BC right: $sbcr
          BC bottom: $sbcl
          BC top: $sbcr
        #
        done
        if( $deformingGrid =~ /^share=/ ){ $deformingGrid =~ s/^share=//; \
                   $deformingGrid="choose grids by share flag\n $deformingGrid"; };
        $deformingGrid
        * north-pole
        * south-pole
        * choose grids by share flag
        *   100
     done
  done
***************************
#-   turn on moving grids
#- * 
#-   specify grids to move
#-     debug 
#-      $debug 
#-     $move
#-     $gridToMove
#-     done
#-   done
  * use implicit time stepping
   $ts
   choose grids for implicit
     all=implicit
*      all=explicit
*      stir=implicit
   done
* 
  debug $debug 
  boundary conditions
    all=dirichletBoundaryCondition
    done
*   initial conditions
*     uniform flow
*      p=1.
*   exit
* 
  continue



