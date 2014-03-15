*
*   An interface between a fluid and a solid
*
* Examples:
*   cgmp fluidSolidInterface -g=innerOutere2.order2 -domain1=outerDomain -domain2=innerDomain
*
$tFinal=10.; $tPlot=.1; $show = " "; $debug=0; $cfl=.5; $ghost=0;
* 
$nu=.1; $kThermal=.1; $ktcFluid=.1; 
$kappa=.5; $ktcSolid=.5; $thermalExpansivity=.1; $Twall=10.;
* 
$domain1="leftSquare"; $domain2="rightSquare"; 
$left="leftSquare"; $right="rightSquare"; 
*
* $grid="twoSquaresInterface1p.hdf"; $debug=0;
* $grid="twoSquaresInterface1.hdf"; $debug=0;  $ghost=1;
* $grid="twoSquaresInterface2.hdf"; 
$grid="innerOuter2d.hdf"; $domain1="outerDomain"; $domain2="innerDomain"; 
* $grid="innerOuter4.hdf"; 
*
* $grid="twoBoxesInterface1.hdf";  $left="leftBox"; $right="rightBox"; $tPlot=.1; 
#
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot,"domain1=s"=>\$domain1,"domain2=s"=>\$domain2 );
*
$grid
*
* 
*  -------------Start domain 1 --------------
setup $domain1
 set solver Cgins
 solver name fluid
 solver parameters
* 
  incompressible Navier Stokes
  Boussinesq model
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0. 
  continue
* 
* 
  forward Euler
* 
  turn off twilight
*
  pde parameters
    nu  $nu
    kThermal $kThermal
    thermal conductivity $ktcFluid
    gravity
      0. -1. 0. 
   done
* 
  boundary conditions
    all=slipWall
    outerSquare(0,1)=noSlipWall, uniform(T=0.)
    # old: outerAnnulus(0,1)=interfaceBoundaryCondition
    bcNumber100=noSlipWall
    bcNumber100=heatFluxInterface
    done
*
  cfl $cfl
*
  initial conditions
  uniform flow
    u=0. v=0. T=0.
  exit
 continue
done
*  -------------End domain 1 ----------------
*  -------------Start domain 2 --------------
setup $domain2
 set solver Cgad
 solver name solidB
 solver parameters
* 
  advection diffusion
  continue
* 
  pde parameters
    kappa $kappa
    thermal conductivity $ktcSolid
  done
* 
  forward Euler
* 
  turn off twilight
*
  boundary conditions
*     all=dirichletBoundaryCondition
    all=neumannBoundaryCondition
    # old: $right(0,0)=interfaceBoundaryCondition
    # old: innerAnnulus(1,1)=interfaceBoundaryCondition
    bcNumber100=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=1.)
    bcNumber100=heatFluxInterface
    done
*
  cfl $cfl
*
  initial conditions
  uniform flow
    T=$Twall
  exit
 continue
done
* -----------End domain 2 ------------------
continue
* -- set parameters for cgmp ---
  final time $tFinal
  times to plot $tPlot
  turn off twilight
  forward Euler
  debug flag $debug
  continue
*
continue


  plot:fluid : T
  contour
    $Tmax=$Twall/5; 
    min max 0 $Tmax
    ghost lines $ghost
    exit
    min max 0 $Tmax
    ghost lines $ghost
    vertical scale factor 0.
    exit

movie mode
finish


