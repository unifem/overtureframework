################################################################################
## cgins command file for flow in a periodic channel (2d or 3d)
## 
## 130118: kkc initial version
##
## Usage:
##    cgins [noplot] [nopause] -tf=<real> -tp=<real> -nu=<real>  -cfl=<real>
##                             -solver=[best|yale|mg] -ts=[im|pc|afs] 
##                             -pgf=<real> -show=<string> -xa=<real> -xb=<real> 
##                             -ya=<real> -yb=<real> -za=<real> -zb=<real> 
##                             -Nx=<int> -Ny=<int> -Nz=<int> -ystr=<real> 
##                             -order=[2|4]
##                             -ad2=<0|1> -ad2l=<real> -ad2n=<real> 
##                             -ad4=<0|1> -ad4l=<real> -ad4n=<real>
##                             -useWallModel=<0|1>
##                             -wallModel=<slipWall|logLaw|simpleLogLaw|wernerWengle|fixedUTau>
##                             -restart=<s> -restartSolution=<i>
##
## Parameters:
##    tf : final time (default=10)
##    tp : time to plot (default=0.1)
##    nu : viscosity (default 1e-3)
##    cfl: timestep stability parameter (default 0.9)
##    solver : linear solver to use 
##             (for both pressure and implicit time stepping)
##    ts : time stepping method 
##         (im: implicit, pc:predictor corrector, afs: approximate factorization)
##    show : name of show file (default is empty, no show files written)
##    u0 : velocity of the y=ya wall
##    u1 : velocity of the y=yb wall
##    up : magnitude of the Poiseuille flow
##    upp : magnitude of the Poiseuille flow perturbation
##    ax,ay : perturbation frequencies
##    pgf : pressure gradient is $nu*$up*$pgf
##    xa, xb: min and max x coordinate, this direction is always periodic
##    ya, yb: min and max y coordinate, this direction has walls at ya and yb
##    za, zb: min and max z coordinate, if specificed the problem is 3D, 
##            this direction is always periodic with no pressure gradient
##    Nx,Ny,Nz : nominal grid size in x, y and z, will be adjusted by stretching in y
##    ystr : stretching coefficient for the y direction, (default=7)
##    order : order of accuracy (default=2)
##    ml    : number of multigrid levels
##    ad2, ad2l, ad2n : ad2=1 activates 2nd order artificial dissipation 
##                      ad2l is the linear coefficient (default = 1)
##                      ad2n is the nonlinear coefficient (default = 1)
##    ad4, ad4l, ad4n : ad4=1 activates 4nd order artificial dissipation 
##                      ad4l is the linear coefficient (default = 1)
##                      ad4n is the nonlinear coefficient (default = 1)
##
##    bc : wallModel(default), noSlipWall or slipWall
##    wallModel : sets the wall model type (default=slipWall)
##    wallModelNoSlip : if set to 1 then the tangential velocity is forced to zero (default=0, i.e. slipWall with -wallModel=slipWall)
##    wallModelIncludeAD : if set to 1, then include the artificial dissipation in the wall shear stress (default=1)
##    wallModelParameters : set the parameters for either the simpleLogLaw or the fixedUTau models
##                          simpleLogLaw : specify a real value for the "roughness height"
##                          fixedUTau : specify a real value for the wall friction velocity
##    wallModelLinearLayerYPlus: specify the yplus for the transition between the inner (linear) and outer layers
##
## EXAMPLES:
##
## 2D serial: goes turbulent around t=170
## cgins periodicChannel.cmd -nu=1e-5 -Ny=81 -Nx=101 -order=4 -cfl=2 -ystr=14 -ad2=0 -ad4=1 -ad4l=0.25 -ad4n=0.25 -debug=1 -wallModelNoSlip=1 -wallModel=slipWall -Nz=41 -solver=mg -show=pChannel.2d.ns.show -tp=1 -tf=1000 
##
## mpirun -np 8 cgins -numberOfParallelGhost=4 periodicChannel.cmd -nu=1e-5 -Ny=81 -Nx=101 -order=4 -cfl=1 -ystr=14 -ad2=0 -ad4=1 -ad4l=0.25 -ad4n=0.25 -debug=1 -wallModelNoSlip=1 -wallModel=slipWall -za=0 -zb=1 -Nz=41 -solver=mg -show=pChannel.2d.ns.show noplot nopause -tp=1 -tf=1000
##
## SETUP DEFAULT PARAMETER VALUES
$debug = 0; $ogesDebug=0; $ogmgDebug=0; 
$pi=4.*atan2(1.,1.);
$tf = 10;
$tp = 0.1;
$nu = 1e-3;
$solver = "best"; $rtolp=1.e-8; $atolp=1.e-8; 
$ts = "afs";
$show = "";
$u0 = 0; $u1 = 0; $up = 1; $upp=0.1;
$ax=2.; $ay=2.;
$pgf = 8;
$xa = 0; $xb = 2*$pi; 
$ya = 0; $yb = 1; 
$za =-1; $zb =-1;
$Nx = 101; $Ny = 11; $Nz = 11;
$ystr = 7;
$order = 2;
$ml = 2;
$ad2 = 0; $ad2l = 1; $ad2n = 1;
$ad4 = 0; $ad4l = 1; $ad4n = 1;
$cfl = 0.9;
$slowStartSteps=-1; $slowStartCFL=.5; $slowStartRecomputeDt=50; $slowStartTime=-1.; $recomputeDt=10000;
$restart=""; $restartSolution=-1;
##
$useWallModel = 1;
$wallModel = "slipWall";
$wallModel_noSlipWall = 1;
$wallModel_includeAD = 0;
$wallModel_parameters = "#";
$wallModel_llyplus = 11;
$bc = "wallModel";
##
$nullVector="channel3d.101.81.order4.ml2.nullVector.hdf";
##
$cdv=1;  $cDt=.25;
#
$go="halt"; 
## GET THE COMMAND LINE ARGUMENTS
GetOptions("cfl=f"=>\$cfl,"ml=i"=>\$ml,"tf=f"=>\$tf,"tp=f"=>\$tp,"nu=f"=>\$nu,"solver=s"=>\$solver,"ts=s"=>\$ts,"show=s"=>\$show,"u0=f"=>\$u0,"u1=f"=>\$u1,"up=f"=>\$up,"pgf=f"=>\$pgf,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,"Nx=i"=>\$Nx,"Ny=i"=>\$Ny,"Nz=i"=>\$Nz,"ystr=f"=>\$ystr,"order=i"=>\$order,"ad2=i"=>\$ad2,"ad2l=f"=>\$ad2l,"ad2n=f"=>\$ad2n,"ad4=i"=>\$ad4,"ad4l=f"=>\$ad4l,"ad4n=f"=>\$ad4n,"wallModel=s"=>\$wallModel,"wallModelNoSlip=i"=>\$wallModel_noSlipWall,"wallModelIncludeAD=i"=>\$wallModel_includeAD,"wallModelParameters=f"=>\$wallModel_parameters,"useWallModel=i"=>\$useWallModel,"wallModelLinearLayerYPlus=f"=>\$wallModel_llyplus,"bc=s"=>\$bc,"upp=f"=>\$upp,"ax=f"=>\$ax, "ay=f"=>\$ay,"debug=i"=>\$debug,"nullVector=s"=>\$nullVector,"restart=s"=>\$restart,"restartSolution=i"=>\$restartSolution,"ogesDebug=i"=>\$ogesDebug,"ogmgDebug=i"=>\$ogmgDebug,"rtolp=f"=>\$rtolp,"atolp=f"=>\$atolp,"slowStartSteps=i"=>\$slowStartSteps,"slowStartRecomputeDt=i"=>\$slowStartRecomputeDt,"slowStartCFL=f"=>\$slowStartCFL,"go=s"=>\$go );
##
## SETUP SOME PARAMETERS BASED ON THE INPUTS
if( $solver eq "best" ){ $solver="choose best iterative solver"; }
if( $solver eq "mg" ){ $solver="multigrid"; }
$is3D = $za<$zb;
$newts = "*";
if( $ts eq "im" ){ $ts="implicit";       }
if( $ts eq "pc" ){ $ts="adams PC";       }
if( $ts eq "afs"){ $ts="approximate factorization"; $newts = "use new advanceSteps versions";}
if( ($wallModel ne "simpleLogLaw") && ($wallModel ne "fixedUTau") ){$wallModel_parameters = "#";}
#
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
$pressureGradient = $nu*$up*$pgf;
#
#
# GENERATE THE GRID (using either channel2d.ogen.cmd or channel3d.ogen.cmd)
$grid_script = $is3D ? "channel3d.ogen.cmd" : "channel2d.ogen.cmd";
ogen
include $grid_script
#
# SETUP CGINS
incompressible Navier Stokes
exit
#
turn off twilight zone
#
final time $tf
times to plot $tp
#
show file options
  compressed
  open
    $show
  frequency to flush
    5
  exit
#
$ts
$newts
choose grids for implicit
 all=implicit
done
 cfl $cfl
# 
  slow start cfl $slowStartCFL
  slow start steps $slowStartSteps
  slow start recompute dt $slowStartRecomputeDt
  slow start $slowStartTime   # (seconds)
#
maximum number of iterations for implicit interpolation
   10
#
pde parameters
  nu
  $nu
  OBPDE:second-order artificial diffusion $ad2
   OBPDE:ad21,ad22 $ad2l , $ad2n
  OBPDE:fourth-order artificial diffusion $ad4
   OBPDE:ad41,ad42 $ad4l , $ad4n
  OBPDE:use boundary dissipation in AF scheme 0
 OBPDE:divergence damping  $cdv
  OBPDE:cDt div damping $cDt
done
# 
user defined forcing
  constant forcing
 # add a forcing to the u equation
    1 $pressureGradient
  done
exit
#
pressure solver options
 $ogesSolver=$solver; $ogesRtol=$rtolp; $ogesAtol=$atolp; $ogesIluLevels=$iluLevels; $ogmgMaxIterations=10; 
 include $ENV{CG}/ins/cmd/ogesOptions.h
 maximum allowable increase in the residual
 1e10
 multigrid parameters
  problem is singular 1
  null vector option:readOrComputeAndSave
  null vector file name:$nullVector
 exit
exit
#
implicit time step solver options
 $ogesSolver=$solver; $ogesRtol=$rtol; $ogesAtol=$atol; 
 include $ENV{CG}/ins/cmd/ogesOptions.h
exit
#
boundary conditions
  bcNumber1=penaltyBoundaryCondition, penaltyWallFunctionBC
    noSlipWall $wallModel_noSlipWall
    includeAD $wallModel_includeAD
#    llyplus $wallModel_llyplus
    $wallModel
    $wallModel_parameters
  done
  bcNumber2=noSlipWall
  bcNumber3=slipWall
done
#
#
# initial conditions: uniform flow or restart from a solution in a show file 
# if( $restart eq "" ){ $cmds = "uniform flow\n u=1., v=0., w=0., p=1."; }\
#   else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number $restartSolution \n OBIC:assign solution from show file"; }
# 
#
# -- perturbed initial conditions:
# Couette-Poiseuille flow with a divergence free perturbation:
#   u = u0*(y-ya)(yb-y)/[.5*(yb-ya)]^2 + u1*(y-ya)/(yb-ya)
#     + u2*      sin(ax*pi*x/(yb-ya))*cos(ay*pi*(y-ya)/(yb-ya))
#   v =-u2*ax/ay*cos(ax*pi*x/(yb-ya))*sin(ay*pi*(y-ya)/(yb-ya))
# Enter u0,u1,u2, ax,ay, ya,yb
$cmds="OBIC:user defined...\n  couette profile\n  $up $u1 $upp $ax $ay $ya $yb\n  exit";
#
if( $restart ne "" ){ $cmds = "OBIC:show file name $restart\n use grid from show file 0\n OBIC:solution number $restartSolution \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
#
debug $debug
#
continue
#
$go


