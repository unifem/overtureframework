*
$tFinal=.02; $tPlot=.001; $show = " "; $debug=0; $cfl=.9; $ghost=0; 
$degreeSpace1=2; $degreeTime1=1; $a1=0.; $b1=0.; 
$degreeSpace2=2; $degreeTime2=1; $a2=0.; $b2=0.; 
$kappa1=.1; $kappa2=.1;
*
$grid="square20.hdf"; $debug=3; $kappa1=1.; $tFinal=2.; $tPlot=.1;
* $grid="twoBoxesInterface1.hdf"; $debug=3; $kappa1=1.; $kappa2=.5;
* $grid="twoBoxesInterface2.hdf"; 
* $grid="innerOuter.hdf"; $kappa1=1.; $kappa2=.5;
* $grid="innerOuter3d.hdf"; 
* $grid="innerOuter3d2.hdf";  $degreeSpace1=0; $degreeSpace2=0;
* $grid="bib2.hdf"; $degreeSpace1=0; $degreeSpace2=0;
* $grid="boxsbs1.hdf"; $degreeSpace1=0; $degreeSpace2=0; $debug=31; $tPlot=.01;
* $grid="bib.hdf"; $degreeSpace1=0; $degreeSpace2=0; $debug=31; 
* $grid="cic2.hdf"; $degreeSpace1=0; $degreeSpace2=0;
*
$grid
* 
*  ------------------------------------------
  Cgad solidA
  advection diffusion
  define real parameter kappa $kappa1
  define real parameter nu0CD $kappa1
  define real parameter a0CD  $a1
  define real parameter b0CD  $b1
  define real parameter c0CD  0.
  continue
* 
* 
  forward Euler
*
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  OBTZ:degree in space $degreeSpace1
  OBTZ:degree in time $degreeTime1
  boundary conditions
    all=dirichletBoundaryCondition
    leftSquare(1,0)=interfaceBoundaryCondition
    leftBox(1,0)=interfaceBoundaryCondition
    outerAnnulus(0,1)=interfaceBoundaryCondition
  done
  debug $debug
  continue
*  ------------------------------------------
* -- set parameters for cgmp ---
  final time $tFinal
  times to plot $tPlot
  debug $debug
  continue
*
