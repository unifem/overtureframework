#
# cgins: command file for flow past a naca0012 airfoil
# 
#   cgins naca -g=naca0012.hdf -nu=1.e-8 
#
#  use either naca0012.hdf or naca.hype.hdf
#
# naca.hype
$tFinal=10.; $tPlot=1.; $debug=3; 
$grid="naca0012.hdf"; $nu=1.e-8; $ad21=1.; $ad22=1.; 
# read command line options:
GetOptions( "g=s"=>\$grid,"nu=f"=>\$nu );
#
$grid
#
  incompressible Navier Stokes
  exit
#
  show file options
    open
     naca.show
  exit
  turn off twilight zone 
  implicit
  choose grids for implicit
    all=implicit
    backGround=explicit
    done
  implicit factor
    0.75
#
  final time $tFinal.
  times to plot $tPlot
  debug $debug
#
  plot and always wait
 # no plotting
  pde parameters
 # the next value for nu is too small to have any effect.
    nu
      $nu
    # turn on secon-order non-linear artificial dissipation
    OBPDE:second-order artificial diffusion 1
    OBPDE:ad21,ad22 $ad21 , $ad22
  done
#  cfl
#    .75
  boundary conditions
    all=noSlipWall
    backGround(0,0)=inflowWithVelocityGiven, uniform(p=1.,u=1.)
    backGround(1,0)=outflow
    backGround(0,1)=slipWall
    backGround(1,1)=slipWall
    done
  initial conditions
  uniform flow
    p=1., u=1.
  done
  project initial conditions
  continue
