*
* cgins: three moving valves
*
*  cgins [-noplot] threeValve -g=<name> -tf=<tFinal> -tp=<tPlot> -solver=<yale/best> -ts=<implicit> -debug=<num> ..,
*                     -nu=<num> -kThermal=<num> -bg=<backGround> -show=<name> -implicitFactor=<num> ...
*                     -moveOnly=[0|1]
* Examples:
*  cgins noplot threeValve -g=threeValvei1.order2.hdf -nu=.01 -show="threeValve.show" -go=go
* 
* --- set default values for parameters ---
* 
$cfl=.9; $tFinal=6.; $tPlot=.01; $go="halt";
$grid="threeValvei1.order2.hdf"; $nu=.01; $show="";  $moveOnly=0; $dtMax=.1; 
$solver="yale"; $rtol=1.e-4; $atol=1.e-6; $ogesDebug=0; $ad2=1; $ad22=2.; 
* 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"implicitFactor=f"=>\$implicitFactor, "model=s"=>\$model,"go=s"=>\$go,\
 "tp=f"=>\$tPlot, "solver=s"=>\$solver, "tz=s"=>\$tz, "show=s"=>\$show,"order=i"=>\$order,"debug=i"=>\$debug, \
 "ts=s"=>\$ts,"nu=f"=>\$nu,"kThermal=f"=>\$kThermal,"cfl=f"=>\$cfl, "bg=s"=>\$backGround, "noplot=s"=>\$noplot,\
  "rtol=f"=>\$rtol,"atol=f"=>\$atol,"ad2=i"=>\$ad2,"ad22=f"=>\$ad22,"moveOnly=i"=>\$moveOnly,"dtMax=f"=>\$dtMax );
* -------------------------------------------------------------------------------------------------
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
* 
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
* 
*
* specify the overlapping grid to use:
$grid
* Specify the equations we solve:
  incompressible Navier Stokes
*  simulate grid motion only $moveOnly 
  exit
*
  show file options
    compressed
    open
      $show
    * frequency to save 5
    frequency to flush
      10
  exit
*
  final time $tFinal
  times to plot $tPlot
  dtMax $dtMax
  maximum number of iterations for implicit interpolation
    10 
*
**
  pde parameters
    nu $nu 
    OBPDE:second-order artificial diffusion $ad2
    OBPDE:ad21,ad22  $ad22, $ad22
    done
**
  turn off twilight zone
  project initial conditions
*
  implicit
  choose grids for implicit
    all=implicit
    cylinder=explicit
   done
*
  turn on moving grids
  specify grids to move
    * Oscillation: x(t) = x(0) + tangent { [ 1-cos( (t-t0)*(omega *2*pi) ) ]*amplitude }
    * tangent
    * omega (oscillations/time)
    * amplitude
    * t0 - origin
    *
*    $omegaTop=.275;
*    $omegaRight=.5;
*    $omegaLeft=.35;
    $omegaTop=.55;
    $omegaRight=1.;
    $omegaLeft=.7;
    * ---- top valve ----
    oscillate
      0 1 0
      $omegaTop
       * .2125  .175  .15
       .2  .175  .15
      0.
     valve-shaft-left
     valve-shaft-right
     valve-head
    done
    * ---- right valve ----
    oscillate
      1 0 0
      $omegaRight
       .2   .175  .15
      0.
     valve-shaft-left2
     valve-shaft-right2
     valve-head2
    done
    * ---- left valve ----
    oscillate
      1 0 0
      $omegaLeft
       .2  .15
      0.
     valve-shaft-left3
     valve-shaft-right3
     valve-head3
    done
  done
  pressure solver options
     $solver
     relative tolerance
       $rtol
     absolute tolerance
       $atol
    exit
  implicit time step solver options
     $solver
     relative tolerance
       $rtol
     absolute tolerance
       $atol 
    exit
* 
* --------------------------------------
*  Boundary conditions:
*     Walls:                  1
*     cylinder inletOutlet:   2 
*     Valve1 inlet/oulet:     3
*     Valve2 inlet/oulet:     4
*     Valve3 inlet/oulet:     5
* --------------------------------------
  boundary conditions
    all=noSlipWall
*
*   bcNumber2=inflowWithVelocityGiven,       parabolic(d=.15,p=1,v=1.)
*   bcNumber3=outflow  
*
**   bcNumber2=outflow
**   bcNumber3=inflowWithPressureAndTangentialVelocityGiven, uniform(p=1.)
*
  done
*
 cfl $cfl 
*
  initial conditions
    uniform flow
      u=0., p=1.
   exit
 continue
*
$go



DISPLAY AXES:0 0
* DISPLAY LABELS:0 0
DISPLAY COLOUR BAR:0 0
set view:0 0.00462893 0.034687 0 1.26883 1 0 0 0 1 0 0 0 1
erase
streamlines
exit this menu
*
* pause
*


   movie and save
     threeValve
    finish



  boundary conditions
    all=noSlipWall
*    inlet-left(1,1)=inflowWithPressureAndTangentialVelocityGiven uniform(p=1.)
*    inlet-right(1,1)=inflowWithPressureAndTangentialVelocityGiven uniform(p=1.)
*    valve-seat-left(0,0)=inflowWithPressureAndTangentialVelocityGiven uniform(p=1.)
*   valve-seat-right(0,0)=inflowWithPressureAndTangentialVelocityGiven uniform(p=1.)
*
    inlet-left(1,1)=inflowWithVelocityGiven,       parabolic(d=.15,p=1,v=-1.)
    inlet-right(1,1)=inflowWithVelocityGiven,      parabolic(d=.15,p=1,v=-1.)
    valve-seat-left(0,0)=inflowWithVelocityGiven,  parabolic(d=.15,p=1,v=-1.)
    valve-seat-right(0,0)=inflowWithVelocityGiven, parabolic(d=.15,p=1,v=-1.)
*
*    inlet-left(1,1)=inflowWithVelocityGiven,       uniform(p=1,u=0.,v=-1.)
*    inlet-right(1,1)=inflowWithVelocityGiven,      uniform(p=1,u=0.,v=-1.)
*    valve-seat-left(0,0)=inflowWithVelocityGiven,  uniform(p=1,u=0.,v=-1.)
*    valve-seat-right(0,0)=inflowWithVelocityGiven, uniform(p=1,u=0.,v=-1.)
*
    inlet-left2(1,1)=inflowWithVelocityGiven,       parabolic(d=.15,p=1,u=-1.)
    inlet-right2(1,1)=inflowWithVelocityGiven,      parabolic(d=.15,p=1,u=-1.)
    valve-seat-left2(0,0)=inflowWithVelocityGiven,  parabolic(d=.15,p=1,u=-1.)
    valve-seat-right2(0,0)=inflowWithVelocityGiven, parabolic(d=.15,p=1,u=-1.)
*
    inlet-left3(1,1)=inflowWithVelocityGiven,       parabolic(d=.15,p=1,u=1.)
    inlet-right3(1,1)=inflowWithVelocityGiven,      parabolic(d=.15,p=1,u=1.)
    valve-seat-left3(0,0)=inflowWithVelocityGiven,  parabolic(d=.15,p=1,u=1.)
    valve-seat-right3(0,0)=inflowWithVelocityGiven, parabolic(d=.15,p=1,u=1.)
*
    cylinder(0,1)=outflow    
    done
