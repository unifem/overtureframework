* 
* cgmp example: solve advection-diffusion in two domains
*
*
$tFinal=.3; $tPlot=.1; $show = " "; $debug=0; $cfl=.9; $ghost=0; 
$degreeSpace1=2; $degreeTime1=1; $a1=0.; $b1=0.; 
$degreeSpace2=2; $degreeTime2=1; $a2=0.; $b2=0.; 
$kappa1=.1; $kappa2=.1;
*
* $grid="twoSquaresInterface1.hdf"; $debug=0; $kappa1=1.; $kappa2=.5;
* $grid="twoBoxesInterface1.hdf"; $kappa1=1.; $kappa2=.5; $tFinal=.1; $tPlot=.05;
$grid="twoBoxesInterfacei111.order2.hdf"; $kappa1=1.; $kappa2=.5; $tFinal=.1; $tPlot=.05;
* $grid="twoBoxesInterface2.hdf"; 
* $grid="innerOuter.hdf"; $kappa1=1.; $kappa2=.5;
* $grid="innerOuter3d.hdf"; 
* $grid="innerOuter3d2.hdf";  $degreeSpace1=0; $degreeSpace2=0;
*
$grid
* 
*  ------------------------------------------
setup leftDomain
 set solver Cgad
 solver name solidA
 solver parameters
* 
  advection diffusion
  continue
* 
  pde parameters
    kappa $kappa1
    thermal conductivity $kappa1
    a $a1
    b $b1 
  done
* 
  forward Euler
*
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  OBTZ:degree in space $degreeSpace1
  OBTZ:degree in time $degreeTime1
  boundary conditions
    all=dirichletBoundaryCondition
    * leftBox(1,0)=interfaceBoundaryCondition
    * leftBox(1,0)=interfaceBoundaryCondition
    * outerAnnulus(0,1)=interfaceBoundaryCondition
    * bcNumber100=interfaceBoundaryCondition
    bcNumber100=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)
    bcNumber100=heatFluxInterface
  done
  continue
done
*  ------------------------------------------
setup rightDomain
 set solver Cgad
 solver name solidB
 solver parameters
  advection diffusion
  continue
* 
  pde parameters
    kappa $kappa2
    thermal conductivity $kappa2
    a $a2
    b $b2 
  done
* 
  forward Euler
*
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  OBTZ:degree in space $degreeSpace2
  OBTZ:degree in time $degreeTime2
  boundary conditions
    all=dirichletBoundaryCondition
    * rightBox(0,0)=interfaceBoundaryCondition
    * rightBox(0,0)=interfaceBoundaryCondition
    * innerAnnulus(1,1)=interfaceBoundaryCondition
    * bcNumber100=interfaceBoundaryCondition
    bcNumber100=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=0.)
    bcNumber100=heatFluxInterface
  done
  continue
done
continue
*  ------------------------------------------
* -- set parameters for cgmp ---
  forward Euler
  final time $tFinal
  times to plot $tPlot
  debug $debug
  continue
*
movie mode
finish
