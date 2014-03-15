# This cgmp script sets up a conjugate heat transfer problem
# where a fluid flows between an moving isothermal wall and
# a conducting wall of finite thickness.  The geometry is
# given by:
#
#
# +================top_isoT_bc===========+ y = H, T=T_top, u=u_top
# 
# x-periodic   Fluid Domain                x-periodic
#
# +----------------interface_bc----------+ y = \alpha H
#
# x-periodic   Solid Domain                x-periodic
#
# +================bot_isoT_bc===========+ y = 0, T=T_bottom
# x = 0                                    x = L
#
# NOTES : 
#    - command line options are processed both in this file as well as flatPlates.ogen.cmd
#    - Usage:
#        cgmp [noplot] [nopause] --H=<height> --L=<length> --factor=<resolution factor> --accuracy=<accuracy> ...
#             --u_top=<top plate velocity> --T_top=<top temperature> --T_bot=<bottom temp> --alpha=<fraction of solid> ...
#             --name=<name for show files> -ts=<fe/be/im/pc> -coupled=[0|1] -nc=<num> -imp=<val>
#    - Example:
#        cgmp flatPlates --H=1 --L=5 --factor=2 --accuracy=4 --T_top=10 --u_top=1
#        sets the height (H) to 1, the length (L) to 5, the resolution factor to 2, accuracy to 4, etc...
# Examples:
#  cgmp flatPlates -H=1 -L=1 -factor=1 -T_top=2 -u_top=1 -ts=imp -imp=1. -nc=10 -tf=50 -tp=1. -dtMax=.1 -fluid_kappa=.02 -fluid_k=.02 -coupled=0 -iTol=1.e-3 -iOmega=.75
#  - add buouyancy : 
#  cgmp flatPlates -H=1 -L=1 -factor=1 -T_top=5 -u_top=0 -ts=imp -imp=1. -nc=10 -tf=50 -tp=1. -dtMax=.1 -solid_kappa=1. -solid_k=1. -fluid_kappa=.2 -fluid_k=.2 -coupled=0 -iTol=1.e-3 -iOmega=.75 -gravity="-1. 0 0." -debug=1 -show="flatPlates1.show"
# 
# - 3D example:
#  cgmp flatPlates -nd=3 -H=1 -L=1 -factor=1 -T_top=5 -u_top=0 -ts=imp -imp=1. -nc=10 -tf=10 -tp=1. -dtMax=.1 -solid_kappa=1. -solid_k=1. -fluid_kappa=.2 -fluid_k=.2 -coupled=0 -iTol=1.e-3 -iOmega=.75 -gravity="-1. 0 0." -debug=1 -show="flatPlates3d1.show"
#
# ----- TZ ------
#   -- explicit --
#   cgmp flatPlates -tz=poly -H=1 -L=1 -factor=1 -ts=pc -tf=50 -tp=1. -dtMax=.1 -fluid_kappa=.02 -fluid_k=.02 -coupled=1 -periodic=0 
#   -- implicit --
#   cgmp flatPlates -tz=poly -H=1 -L=1 -factor=1 -ts=imp -imp=.5 -nc=25 -tf=50 -tp=.1 -dtMax=.1 -fluid_kappa=.02 -fluid_k=.02 -coupled=0 -iTol=1.e-12 -iOmega=.75 -periodic=0 -debug=3 
# 
# kkc 080605: Initial version 
# set defaults for parameters: 
$u_top=1.; $T_bottom=1.; $T_top=2.; 
$nd = 2; # 2d by default 
$fluid_kappa = $fluid_k = .01; $fluid_nu=1e-1;
$solid_kappa = $solid_k = .1;
$show = " ";
$cfl = .9; $tFinal =10; $tPlot = .01; $dtMax=.1; 
$gravity = "0 0. 0.";
$tz="none"; $fx1=.5; $fx2=1.; $degreex1=2; $degreet1=2; $degreex2=2; $degreex1=2; 
$solver = "yale";
$go="halt"; $debug=0; 
$ts="pc"; $numberOfCorrections=1; $implicitFactor=.5; $coupled=1; $iTol=1.e-3; $iOmega=1.; 
$flatPlateIsPeriodic=1; 
# get command line options
* ----------------------------- get command line arguments ---------------------------------------
*  -- first get any commonly used options: (requires the env variable CG to be set)
$getCommonOptions = "$ENV{'CG'}/mp/cmd/getCommonOptions.h";
include $getCommonOptions
GetOptions("T_top=f"=>\$T_top,"T_bottom=f"=>\$T_bottom, "u_top=f"=>\$u_top, "T_bottom=f"=>\$T_bottom,"imp=f"=>\$implicitFactor,\
           "solid_kappa=f"=>\$solid_kappa,"solid_k=f"=>\$solid_k,"iOmega=f"=>\$iOmega,"iTol=f"=>\$iTol,\
           "fluid_nu=f"=>\$fluid_nu,"fluid_kappa=f"=>\$fluid_kappa,"fluid_k=f"=>\$fluid_k,\
           "solver=s"=>\$solver,"nd=i"=>\$nd,"degreex1=i"=>\$degreex1,"degreet1=i"=>\$degreet1,\
           "periodic=i"=>\$flatPlateIsPeriodic,\
           "degreex2=i"=>\$degreex2,"degreet2=i"=>\$degreet2,"gravity=s"=>\$gravity,"go=s"=>\$go);
if( $ts eq "fe" ){ $ts="forward Euler";  }
if( $ts eq "be" ){ $ts="backward Euler"; }
if( $ts eq "im" ){ $ts="implicit";       }
if( $ts eq "pc" ){ $ts="adams PC";       }
if( $ts eq "mid"){ $ts="midpoint";       }  
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
# generate the grid (and get boundary condition flags, accuracy options...)
ogen
if( $nd == 2 ){ $gridCommand = "include flatPlates.ogen.cmd"; }else{ $gridCommand = "include flatPlates3d.ogen.cmd"; }
$gridCommand
* include flatPlates.ogen.cmd
# setup CGMP solvers
$fbc = "all=noSlipWall\n";
$fbc .= "bcNumber$top_isoT_bc=noSlipWall, uniform(T=$T_top, u=$u_top, v=0)\n";
$fbc .= "bcNumber$interface_bc=noSlipWall, mixedDerivative(0.*t+1.*t.n=0.)\n  bcNumber$interface_bc=heatFluxInterface\n";
$fbc .= "bcNumber$bot_isoT_bc=dirichletBoundaryCondition, uniform(T=$T_bottom)";
# 
$sbc = "all=dirichletBoundaryCondition\n";
$sbc .= "bcNumber$top_isoT_bc=noSlipWall, uniform(T=$T_top, u=$u_top, v=0)\n";
$sbc .= "bcNumber$interface_bc=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)\n  bcNumber$interface_bc=heatFluxInterface\n";
$sbc .= "bcNumber$bot_isoT_bc=dirichletBoundaryCondition, uniform(T=$T_bottom)";
# SETUP THE DOMAINS
## fluid domain
$domainName = "fluid"; $solverName = "fluid_ins";
$kThermal = $fluid_kappa;  $ktc=$fluid_k; $nu = $fluid_nu; $T0=.5*($T_top+$T_bottom);
$bc = $fbc;
$degreeSpace=$degreex1; $degreeTime=$degreet1;
include insDomain.h
## solid domain
$domainName = "solid"; $solverName = "solid_ad";
$kappa=$solid_kappa; $ktc = $solid_k; $T0=.5*($T_top+$T_bottom);
$bc = $sbc;
$degreeSpace=$degreex2; $degreeTime=$degreet2;
include adDomain.h
#
continue
#
# cgmp parameters
#
  final time $tFinal
  times to plot $tPlot
  $ts
  cfl $cfl
  debug $debug
  $tz
  number of PC corrections $numberOfCorrections
  OBPDE:interface tolerance $iTol
  OBPDE:interface omega $iOmega
  OBPDE:solve coupled interface equations $coupled
*
  show file options
    compressed
      open
       $show
    frequency to save
    1
    frequency to flush
      10
    exit
  continue
continue
$go
