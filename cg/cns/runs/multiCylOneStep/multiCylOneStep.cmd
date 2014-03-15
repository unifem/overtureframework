***************************************************************
*  Detonation hitting a collection of cylinders
#
# NOTE: initial conditions in the profile file oneStepIdealProfile.data come from the 
#     matlab script $cg/runs/multiCylIOneStep/profile.m
* 
#    cgcns [-noplot] multiCylOneStep -g=<gridName> -amr=[0|1] -tf=<real> -tp=<real> -l=<int> -r=[2|4]
#
* Examples:
*   cgcns multiCylOneStep -g=multiCylIG2Big -amr=1 -tp=.05 -go=halt
*
*
* 070709 :
*     redo for David and the S&T brochure
*     original from /home/henshaw.0/res/OverBlown/cns/reactMove
*     /home/henshaw.0/Overture.v21n.d/bin/ogen noplot $ogen/multiCylIG.cmd 
*     /home/henshaw.0/cg.v21n.d/cns/bin/cgcns noplot multiCylOneStep >! multiCylOneStep.out &
**************************************************************
*
$tFinal=1.; $tPlot=.1; $flushFrequency=1; $orderOfExtrap=2; $interpWidth=2; $restart=""; 
$cfl=.9; $debug=0; $bcOption=4; $ad=0.; $go="halt"; 
$moveOn="turn on moving grids"; $moveOff="turn off moving grids"; $move=$moveOn;
$ratio=4; $nrl=2; $amrTol=.0005; 
$amrOff = "turn off adaptive grids";
$amrOn = "turn on adaptive grids";
$amr=0; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=>\$nrl,"r=i"=> \$ratio, "tf=f"=>\$tFinal,"debug=i"=>\$debug,"cfl=f"=>\$cfl, \
            "tp=f"=>\$tPlot, "xStep=s"=>\$xStep, "bg=s"=>\$backGround,"show=s"=>\$show,"go=s"=>\$go,\
            "restart=s"=>\$restart,"tol=f"=>\$to,"amr=i"=>\$amr,"$orderOfExtrap=i"=> \$orderOfExtrap );
# -------------------------------------------------------------------------------------------------
if( $amr eq 0 ){ $amr = "turn off adaptive grids"; }else{ $amr = "turn on adaptive grids"; }
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
*
* 
* multiCylIG2.hdf
* $grid="multiCylIG2Big.hdf"; $show="multiCylOneStep2.show"; 
* $grid="multiCylIG4Big.hdf"; $show="multiCylOneStep4.show"; 
## $grid="multiCylIG4Big.hdf"; $show="multiCylOneStep4-l2.show"; $amr=$amrOn; $flushFrequency=4; $tPlot=.005; 
* $grid="multiCylIG8Big.hdf"; $show="multiCylOneStep8-l2.show"; $amr=$amrOn; $flushFrequency=1; 
*
*
$grid
*
  compressible Navier Stokes (Godunov)
  one step
  exit
  turn off twilight
*
*  do not use iterative implicit interpolation
*
 final time $tFinal
 times to plot $tPlot
 cfl $cfl
*
* no plotting
#  plot and always wait
*
  show file options
    compressed
    open
      $show
    frequency to flush
      $flushFrequency
    exit
*
  detect collisions 1
  minimum separation for collisions 3.
*
*   $density = 2.;  
  $density = 1.;  
  $pi=3.141592653;
*
*****************************
$count=0;
sub moveAnnulus\
{ $count=$count+1; $aName="annulus" . "$count"; \
  $commands = \
  "rigid body\n" . \
  "density\n" . \
  "  $density\n" . \
  "done\n" . \
  "$aName\n" . \
  "done\n"; \
}
  $move
  specify grids to move
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
    moveAnnulus();
    $commands
  done
***************************
  reduce interpolation width
     $interpWidth
  maximum number of iterations for implicit interpolation
    10
*****
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
    order of extrap for 2nd ghost line $orderOfExtrap
    order of extrap for interp neighbours $orderOfExtrap
*   turn on limited extrapolation
    limited extrapolation
 done
*
*****************************************************
* --------------------------------------------------------
  $amr
  order of AMR interpolation
      2
  error threshold
     $amrTol
  regrid frequency
    $regrid=2*$ratio; 
    $regrid
  change error estimator parameters
    set scale factors
      1 10000 10000 1 10000 10000 10000
    done
    weight for first difference
      0.
    weight for second difference
      .03
    exit
    truncation error coefficient
      1.
   *  show amr error function
  change adaptive grid parameters
    refinement ratio
      $ratio
    default number of refinement levels
      $nrl
    number of buffer zones
      2
    grid efficiency
      .7
  exit
* --------------------------------------------------------
*
  OBPDE:mu 0.
  OBPDE:kThermal 0.
*  OBPDE:Rg (gas constant) 1.
*   --- these parameters need to match the values in ob/doc/profile.m 
  OBPDE:gamma 1.4
  OBPDE:heat release 4.0
  OBPDE:1/(activation Energy) 0.075
  OBPDE:rate constant 1
  OBPDE:artificial viscosity 1.5
***************
* OBPDE:exact Riemann solver
**  OBPDE:Roe Riemann solver
* OBPDE:HLL Riemann solver
**  OBPDE:Godunov order of accuracy 2
******************
  debug
    $debug
*
***********************************************
$uCJ=3.181609; 
if( $restart eq "" ){ $cmds = "user defined\n 1d profile from a data file with changes\n oneStepIdealProfile.data\n -.5 $uCJ\n  exit"; }\
  else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
#-     user defined
#-     1d profile from a data file with changes
#-       oneStepIdealProfile.data
#-       * xShift and uShift
#-       $uCJ=3.181609; 
#-       -.5 $uCJ
#-     exit
  exit
********************************************
*
  boundary conditions
    all=slipWall
    $uLeft=-2.023203+$uCJ; 
    backGround(0,0)=superSonicInflow uniform(r=1.572560,u=$uLeft,T=2.936672,s=1.)
    backGround(1,0)=superSonicOutflow
    done
*
continue
  contour
    vertical scale factor 0.
    exit
  grid
    plot grid lines 0
    plot non-physical boundaries 1
    colour boundaries by refinement level number
  exit this menu
*
$go



