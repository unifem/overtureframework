#
#  Supersonic flow around an expanding corner:
#   cgcns expansionCorner -g=[grid] -rho0=<> -u0=<> -p0=<> -cgrid=<> -slipWallBCOption=[0..4] -slopeLimiter=[0|1]
#
#  -cgrid : curved wall belongs to this grid 
#  -slopeLimiter : Godunov slope limiter, 1=on, 0=off
#  -slipWallBCOption : 0=default
#      1=slipWallPressureEntropySymmetry
#      2=slipWallTaylor
#      3=slipWallCharacteristic
#
#  cgcns expansionCorner -g=expansionCorner2.hdf -tp=.1 -tf=1. 
#  cgcns expansionCorner -g=expansionCorner1.hdf -tp=.1 -tf=1. -slipWallBCOption=0  [0,1,2, 
#  cgcns expansionCorner -g=expansionCorner1.hdf -tp=.1 -tf=1. -slipWallBCOption=1 [slipwall slipWallPressureEntropySymmetry OK
#  cgcns expansionCorner -g=expansionCorner1.hdf -tp=.1 -tf=1. -slipWallBCOption=2 [slipwall taylor OK
#  cgcns expansionCorner -g=expansionCorner1.hdf -tp=.1 -tf=1. -slipWallBCOption=3 [slipWallCharacteristic : trouble - why?
#
# Deforming diffuser flow: r=1.4, u=2., T=0.71428571428571 (=1./1.4)
#  cgcns expansionCorner.cmd -g=deformingDiffuserFluidFixedGrid8.order2 -slipWallBCOption=4 -tp=.05 -tf=.5 -cgrid=2 -go=halt
#  cgcns expansionCorner.cmd -g=deformingDiffuserFluidFixedGrid16.order2 -slipWallBCOption=4 -tp=.05 -tf=.5 -cgrid=2 -go=halt
#
#  cgcns expansionCorner -g=/home/schwend/cg/mp/cmd/Diffuser/deformingDiffuserFluidGrid4.order2 -tp=.1 -tf=1. -cgrid=1 -rho0=1.4 -u0=2. -T0=0.71428571428571
# 
#
# --- set default values for parameters ---
$grid="expansionCorner2.hdf"; $show = " "; $backGround="square"; $cnsVariation="godunov"; 
$tFinal=1.; $tPlot=.1; $cfl=.9; $debug=1; $tol=.2;  $dtMax=1.e10;
$cgrid=0; # grid for curved wall
$slopeLimiter=1;  # 1=use slope limiter, 0=do not 
$rho0=1.; $gamma=1.4; $p0=1.; $a0=sqrt($gamma*$p0/$rho0); $u0=1.5*$a0; $v0=0.; $T0=1.; 
$go="halt"; 
$ad=0.; $orderOfExtrapForOutflow=2; $orderOfExtrapForGhost2=2; $orderOfExtrapForInterpNeighbours=2; 
$slipWallBCOption=0; 
#* $slipWallBCOption=3; 
#* $slipWallBCOption=4; 
# 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"amr=i"=>\$amr,"l=i"=>\$nrl,"r=i"=>\$ratio,"tf=f"=>\$tFinal,"debug=i"=>\$debug,"ad=f"=>\$ad, \
            "tp=f"=>\$tPlot, "bg=s"=>\$backGround,"show=s"=>\$show,"go=s"=>\$go,"slipWallBCOption=i"=>\$slipWallBCOption,\
            "cnsVariation=s"=>\$cnsVariation,"cgrid=i"=>\$cgrid,"rho0=f"=>\$rho0,"u0=f"=>\$u0,"T0=f"=>\$T0,\
            "slopeLimiter=i"=>\$slopeLimiter );
# -------------------------------------------------------------------------------------------------
if( $amr eq "0" ){ $amr="turn off adaptive grids"; }else{ $amr="turn on adaptive grids"; }
if( $cnsVariation eq "godunov" ){ $pdeVariation="compressible Navier Stokes (Godunov)"; }
if( $cnsVariation eq "jameson" ){ $pdeVariation="compressible Navier Stokes (Jameson)"; }   #
if( $cnsVariation eq "nonconservative" ){ $pdeVariation="compressible Navier Stokes (non-conservative)";}  #
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
# 
#$method="compressible Navier Stokes (Godunov)";
#$methodnc="compressible Navier Stokes (non-conservative)";
#$methodj="compressible Navier Stokes (Jameson)";
#    
#$gridName="expansionCorner2.hdf"; $tPlot=.1; $tFinal=1.; $show = "expansionCorner2.show";
# $gridName="expansionCorner3.hdf"; $tPlot=.1; $tFinal=2.; 
# $gridName="expansionCorner4.hdf"; $tPlot=.5; $tFinal=2.; $show = "expansionCorner4.show";
# 
# $gridName="expansionCorner2.hdf"; $tPlot=.1; $tFinal=2.; $method=$methodj; 
# $gridName="expansionCorner3.hdf"; $tPlot=.1; $tFinal=1.; $method=$methodj; 
# $gridName="expansionCorner3.hdf"; $tPlot=.05; $tFinal=.2; $ad=5.; 
# $gridName="expansionCorner2.hdf"; $tPlot=.1;  $tFinal=2.; $method=$methodnc; $ad=2.; 
# $gridName="expansionCorner3.hdf"; $tPlot=.5; $tFinal=1.; $method=$methodnc; $ad=5.; 
# 
#
$grid
  $pdeVariation
#   compressible Navier Stokes (non-conservative)  
#*  compressible Navier Stokes (Godunov)  
#    compressible Navier Stokes (Jameson)  
#   one step
#   add extra variables
#     1
# *wdh* 051206:
#* define integer parameter oldVersion 1
#  -- turn off slope limiters:
  define integer parameter SlopeLimiter $slopeLimiter
  exit
  turn off twilight
#
#  do not use iterative implicit interpolation
#
  final time $tFinal 
#
  times to plot $tPlot
# no plotting
  plot and always wait
  show file options
    compressed
    open
      $show
 # expansionCorner2.show
    frequency to flush
      5
    exit
#
 reduce interpolation width
   2
#
#
  boundary conditions
 # all=noSlipWall uniform(T=.3572)
 # all=superSonicOutflow
 # all=outflow
 #   all=dirichletBoundaryCondition
#*    all=neumannBoundaryCondition
 # corner(0,0)=superSonicInflow uniform(r=2.6667,u=1.25,e=10.119,s=0)
 # rho=1, p=1, a=sqrt(1.4)=1.1832160, u=1.5*a
#-    corner(0,0)=superSonicInflow uniform(r=1.,u=1.7748239,T=1.,s=0)
#-    corner(0,1)=slipWall
#
#*    corner(1,0)=farField
#*    corner(1,1)=farField
#-   corner(1,0)=superSonicOutflow
#-   corner(1,1)=superSonicOutflow
 # -- new way:
  all=slipWall
  bcNumber1=superSonicInflow uniform(r=$rho0,u=$u0,T=$T0,s=0)
  bcNumber2=superSonicOutflow
  # top: 
  bcNumber4=superSonicOutflow
  bcNumber4=dirichletBoundaryCondition
 done
# ---NOTE: set extrapolation conditions
  boundary conditions...
   order of extrap for outflow $orderOfExtrapForOutflow (-1=default)
   order of extrap for 2nd ghost line $orderOfExtrapForGhost2
   # *wdh* 110103: 
   order of extrap for interp neighbours $orderOfExtrapForInterpNeighbours
  done
#********************************************************
#   choose the new slip wall BC
#      1=slipWallPressureEntropySymmetry
#      2=slipWallTaylor
#      3=slipWallCharacteristic
#*  OBPDE:slip wall boundary condition option 1
#   OBPDE:slip wall boundary condition option 2
  OBPDE:slip wall boundary condition option $slipWallBCOption
  OBPDE:artificial diffusion $ad $ad $ad $ad $ad $ad $ad
#********************************************************
# 
#* OBBC:order of extrap for 2nd ghost line 2
#
#****************************************************
# old way: OBTZ:supersonic flow in an expanding channel
# new way:
 OBTZ:user defined known solution
  supersonic flow in an expanding channel
  # rho0,u0,v0,p0
   $p0=$rho0*$T0; 
   $rho0 $u0 $v0 $p0 
  # 1. 1.77482 0. 1.
  # side,axis,grid for curved wall:
    0 1 $cgrid
  done
#****************************************************
#
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
  done
#  .7 ok, .8 not ok 
#  cfl .8 .7  .85  .75
  debug
    1
#
#****** We now use the exact solution as the default IC ******
#  initial conditions
#*****************************************************
#    uniform flow
#       r=1. u=1.7748239 T=1. s=0
#    continue
#****************************************************
#    OBIC:read from a show file
#      expansionCorner2.show
#      -1
#    continue
#*****************************************************
continue
# 
$go


movie mode
finish



#
  plot:rho-err
  contour
    ghost lines 0
    exit
open graphics
continue
#
movie mode
finish


