***************************************************************
* cgcns: specified motion of a piston
*   Usage:
*      cgcns pistonSpecifiedMotion -g=<grid> -tp=<> -tf=<> -bcOption=<> -pp=<> -go=[go|halt|og]
* 
* Examples:
*  cgcns pistonSpecifiedMotion -g=plug4.hdf -bcOption=0
*  cgcns pistonSpecifiedMotion -g=plug16.hdf -bcOption=0
*    -> 0.200 1.65e-03 5.61e-04 0.00e+00 3.41e-04 1.40e+00 5.88e-03 1.29e+01 (      29,      29)
*  cgcns -noplot pistonSpecifiedMotion -g=plug32.hdf -bcOption=0 -tf=.2 -go=go
*    -> 0.200 7.06e-04 2.86e-04 0.00e+00 1.46e-04 1.40e+00 1.52e-03 2.30e+00 (      24,      24)
*  cgcns pistonSpecifiedMotion -g=plug16.hdf -bcOption=0 -rho0=.1 -p0=.1
*    -> 0.200 8.48e-05 4.80e-04 0.00e+00 3.43e-04 1.00e+00 5.00e-03 1.25e+01 (      29,      29)
*
*   cgcns -noplot pistonSpecifiedMotion -g=nonPlug16 -bcOption=4 -tp=.1 -tf=.2 -go=go [OK 
* 
*   cgcns -noplot pistonSpecifiedMotion -g=noPlug8 -bcOption=4 -tp=.1 -tf=.2 -go=go 
*   ->    0.200 3.06e-05 2.19e-05 0.00e+00 6.25e-06 1.40e+00 1.11e-02 2.14e-01 (      20,      20)
*   cgcns -noplot pistonSpecifiedMotion -g=plug8 -bcOption=4 -tp=.1 -tf=.2 -go=go 
*   ->    0.200 3.06e-05 2.19e-05 0.00e+00 6.25e-06 1.40e+00 1.11e-02 3.56e-01 (      22,      22)
*
*   cgcns pistonSpecifiedMotion -g=nonPlug8.hdf -ap=-1. -pp=4 -bcOption=4 -tp=.01 -tf=1. -go=halt
*   cgcns -noplot pistonSpecifiedMotion -g=nonPlug8.hdf -ap=-1. -pp=4 -bcOption=4 -tp=.01 -tf=1. -godOrder=1 -go=go >! psm8.out
*  
**************************************************************
*
$tFinal=1.; $tPlot=.1; $godOrder=2;  $dtMax=1e5; $checkForWallHeating=0; 
$cfl=.9; $debug=0; $bcOption=3; $ad=0.; $show=" "; $gridToMove="plug"; 
$show = " ";
$en="l1";
$rho0=1.4; $p0=1.; # added these 101020
$ad=0.; # Godunov linear dissipation
* 
*  motion: x(t) = ap*t^{pp}
  $pp = 4.;  # C2 solution
  $ap=-.5; $pp=3;  # C0 solution
* --- set default values for parameters ---
$grid="plug2.hdf"; $show = " "; $backGround="square"; $cnsVariation="godunov"; 
$ratio=2;  $nrl=2;  # refinement ratio and number of refinement levels
$x0=.5; $dtMax=1.e10; $nbz=2; 
$xStep="x=-1.5"; $go="halt"; 
* 
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=> \$nrl,"r=i"=> \$ratio, "tf=f"=>\$tFinal,"debug=i"=> \$debug,"cfl=f"=>\$cfl, \
            "tp=f"=>\$tPlot, "xStep=s"=>\$xStep, "bg=s"=>\$backGround,"show=s"=>\$show,"go=s"=>\$go,\
            "cnsVariation=s"=>\$cnsVariation,"bcOption=i"=> \$bcOption, "rho0=f"=>\$rho0,"p0=f"=>\$p0,\
            "gridToMove=s"=>\$gridToMove,"ap=f"=>\$ap,"pp=f"=>\$pp,"en=s"=>\$en,"godOrder=f"=>\$godOrder,\
            "dtMax=f"=>\$dtMax,"checkForWallHeating=i"=>\$checkForWallHeating,"ad=f"=>\$ad   );
* -------------------------------------------------------------------------------------------------
if( $cnsVariation eq "godunov" ){ $pdeVariation="compressible Navier Stokes (Godunov)"; }
if( $cnsVariation eq "jameson" ){ $pdeVariation="compressible Navier Stokes (Jameson)"; }   #
if( $cnsVariation eq "nonconservative" ){ $pdeVariation="compressible Navier Stokes (non-conservative)";}  #
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
# 
if( $en eq "max" ){ $errorNorm="OBTZ:maximum norm"; }
if( $en eq "l1" ){ $errorNorm="OBTZ:l1 norm"; }
if( $en eq "l2" ){ $errorNorm="OBTZ:l2 norm"; }
* 
$ap =$ap/$pp;  # piston velocity at time t=1 is equal to ap
* 
* 
* $gridName = "square10.hdf"; $show=" ";  $tPlot=.1; $debug=1; 
* $gridName = "plug2.hdf"; $tPlot=.1; $debug=1; $bcOption=4; 
* $gridName = "plug4.hdf"; $tPlot=.5; $debug=1; $bcOption=4; $ad=0.; $matlab="plug4LM.m"; 
* $gridName = "plug8.hdf"; $tPlot=.5; $debug=1; $bcOption=4; 
* $gridName = "plug16.hdf";  $tPlot=.5; $debug=1; $bcOption=4; 
*
*  --- single grid case ---
* $gridName = "noPlug1.hdf"; $show=" ";  $tPlot=.005; $debug=1;  $bcOption=4; 
*
*
*** $gridName = "noPlug4.hdf"; $show="noPlug4LM.show";  $tPlot=.1; $debug=1;  $bcOption=4;  $matlab="noPlug4LM.m"; 
* $gridName = "noPlug8.hdf"; $show="noPlug8LM.show";  $tPlot=.5; $debug=1;   $bcOption=4;  
* $gridName = "noPlug16.hdf"; $show="noPlug16LM.show";  $tPlot=.5; $debug=0;   $bcOption=4;
*
* -- for plotting the profiles at different times:
* $gridName = "noPlug16.hdf"; $show="noPlug16Movie.show";  $tPlot=.2; $debug=1;   $bcOption=4;  
* $gridName = "plug16.hdf"; $show="plug16Movie.show";  $tPlot=.2; $debug=1;   $bcOption=4;  
*
*  shock formation:
* $gridName = "noPlug4.hdf"; $show="noPlug4Shock.show";  $tPlot=.5; $debug=1;   $bcOption=4; $pp=2.; $ap=1./$pp; 
* $gridName = "noPlug16.hdf"; $show="noPlug16Shock.show";  $tPlot=.5; $debug=1;   $bcOption=4; $ap=1./$pp; 
*
$grid
*
*  Either Jameson or Godunov should work
*  compressible Navier Stokes (Jameson)
compressible Navier Stokes (Godunov)
* compressible Navier Stokes (non-conservative)
*   one step
*  -- turn off slope limiters:
**   define integer parameter SlopeLimiter 0 
  exit
* 
  turn off twilight
*
*  do not use iterative implicit interpolation
*
  final time (tf=)
   $tFinal
*
  times to plot (tp=)
    $tPlot
 no plotting
*plot and always wait
*
  show file options
    compressed
    open
      $show
    frequency to flush
      10
    exit
  * no plotting
* ----------- norm to use:
  $errorNorm
*****
*
* -- do this for now:
**  frequency for full grid gen update
**    1
  * There can be trouble if the grid moves too fast
  turn on moving grids
  specify grids to move
   user defined
      linear motion
       *  motion: x(t) = ap*t^{pp}
       $ap $pp 
      done
     * 
     * plug
     # $gridToMove
     * specify faces, specify body force and/or torque
   choose grids by share flag
      100
     done
  done
***************************
*  reduce interpolation width
*    2
*****
* 
*==  always use curvilinear BC version
* 
  boundary conditions
    * all=slipWall
    all=symmetry
    plug=symmetry
*     plug=superSonicOutflow
#    plug(0,0)=slipWall
     bcNumber100=slipWall
*     plug(1,0)=slipWall
*    square(0,0)=superSonicInflow uniform(r=2.6069,T=.943011,v=0.,u=0.694444,s=0.0)
*    square(0,0)=superSonicInflow uniform(r=1.4,T=.714286,v=0.,u=0.,s=0.0)
    done
*********************************************************
*   choose the new slip wall BC
*      1=slipWallPressureEntropySymmetry
*      2=slipWallTaylor
*      3=slipWallCharacteristic
  OBPDE:slip wall boundary condition option $bcOption
  OBPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad
*********************************************************
*********************************************************
 boundary conditions...
    order of extrap for 2nd ghost line 3
    order of extrap for interp neighbours 3
 done
*
*****************************************************
OBTZ:user defined known solution
  specified piston motion
    $ap $pp
    $rho0 $p0
  done
*****************************************************
*
  pde parameters
    mu
     0.0
    kThermal
     0.0
    heat release
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
   artificial diffusion $ad $ad $ad $ad $ad $ad $ad $ad
   check for wall heating $checkForWallHeating
  done
***************
  cfl
   $cfl
  dtMax $dtMax
* 
*  OBPDE:exact Riemann solver
* OBPDE:Roe Riemann solver
* OBPDE:HLL Riemann solver
  OBPDE:Godunov order of accuracy $godOrder
******************
 debug
   $debug
*
  initial conditions
    $T0=$p0/$rho0;
    OBIC:uniform state r=$rho0, u=0, v=0, w=0, T=$T0
    OBIC:assign uniform state
  continue
 continue
*
$go




    * This shock has speed 1.5
    step function
    x=-4.75
    T=.943011, v=0., u=.694444, r=2.6069
*    T=.714286, v=0., u=0., r=1.4
    $r0=1.4; $T0=1./$r0; 
    T=$T0, v=0., u=0., r=$r0
  continue
continue
*
*
contour
 * ghost lines 2
  x-r 60
  exit
* 
*

movie mode
finish


continue
continue


* Here we save the solution and errors to a matlab file
      contour
        line plots
          specify lines
          1 101
          -.125 .5 1.25 .5
            add u
            add T
            add p
            add rhoErr
            add uErr
            add TErr
            add x0
            save results to a matlab file
            $matlab


movie mode
finish


