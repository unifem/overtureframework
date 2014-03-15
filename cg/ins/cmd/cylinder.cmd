#
# cgins command file for flow past a cylinder
# 
#  Usage:
#    cgins [-noplot] cylinder -g=<name> -nu=<> -tf=<> -tp=<> -show=<> -pressureGradient=<val>
#
# Examples:
#    cgins cylinder -g=cilc.hdf -tf=50. -tp=1. 
#    cgins cylinder -g=cilce4.order2.hdf -tf=50. -tp=1. -nu=.002
#
#  - order=4:
#    cgins cylinder -g=cilce2.order4 -tf=50. -tp=.1 -nu=.01 
#    cgins cylinder -g=cilce4.order4 -tf=50. -tp=.1 -nu=.002
#
#
$grid="cilc.hdf"; $tFinal=50.; $tPlot=1.; $nu=.01; $show=" "; $go="halt"; 
$restart=""; $pressureGradient=""; 
# ----------------------------- get command line arguments ---------------------------------------
GetOptions("g=s"=>\$grid,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot,"show=s"=>\$show,"nu=f"=>\$nu,\
           "restart=s"=>\$restart,"pressureGradient=f"=>\$pressureGradient,"go=s"=>\$go );
if( $go eq "halt" ){ $go = "break"; }
if( $go eq "og" ){ $go = "open graphics"; }
if( $go eq "run" || $go eq "go" ){ $go = "movie mode\n finish"; }
# 
# specify the overlapping grid to use:
$grid
# Specify the equations we solve:
  incompressible Navier Stokes
  exit
#
# Next specify the file to save the results in. 
# This file can be viewed with Overture/bin/plotStuff.
  show file options
    # OBPSF:show variable: p 0
    # OBPSF:show variable: u 0
    # OBPSF:show variable: v 0
     compressed
      open
       $show
    frequency to flush
      3
    exit
#   display parameters
  turn off twilight zone 
#
# choose implicit time stepping:
  implicit
# but integrate the square explicitly:
  choose grids for implicit
    all=implicit
    square=explicit
    done
  final time $tFinal
  times to plot $tPlot
  plot and always wait
#*  no plotting
  pde parameters
    nu
      $nu
#  OBPDE:check for inflow at outflow
#  OBPDE:expect inflow at outflow
# 
   OBPDE:second-order artificial diffusion 0
   OBPDE:ad21,ad22 .5,.5
   OBPDE:fourth-order artificial diffusion 0
   OBPDE:ad41,ad42 1,1
  done
#
#  OBPSF:show variable: vorticity 1
#  OBPSF:show variable: divergence 1
# 
# cfl
#   .25
# optinally add a forcing to the u equation
  $cmds = "user defined forcing\n constant forcing\n 1 $pressureGradient\n done\n  exit";
  if( $pressureGradient eq "" ){ $cmds="#"; }
  $cmds
#
  boundary conditions
    all=noSlipWall
    square(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)
##    square(0,0)=inflowWithVelocityGiven, parabolic(p=1.,u=1.,d=.5)
#*     square(1,0)=outflow,  pressure(1.*p+1.*p.n=0.)
#*    square(1,0)=outflow,  pressure(1.*p+0.*p.n=1.)
    square(1,0)=outflow, 
    square(0,1)=slipWall
    square(1,1)=slipWall
#    square(0,1)=outflow
#    square(1,1)=outflow
  done
if( $restart eq "" ){ $cmds = "uniform flow\n u=1., v=0., p=1."; }\
else{ $cmds = "OBIC:show file name $restart\n OBIC:solution number -1 \n OBIC:assign solution from show file"; }
# 
  initial conditions
    $cmds
  exit
  project initial conditions
#*  debug 3 
  continue
#
$go

 

