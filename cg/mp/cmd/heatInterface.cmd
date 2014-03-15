*
*   Example showing the solution of two different heat-equations in two domains
*
$tFinal=1.; $tPlot=.1; $show = " "; $debug=0; $cfl=.5; $ghost=0;
$kappa1=.5; $ktc1=.5; $kappa2=.5; $ktc2=.5; $Tinflow=1.; 
$left="leftSquare"; $right="rightSquare"; $domain1="leftDomain"; $domain2="rightDomain";
*
* $grid="twoSquaresInterface1p.hdf"; $debug=0;
$grid="twoSquaresInterface1.hdf"; $debug=0;  $ghost=1;
* $grid="twoSquaresInterface2.hdf"; 
** $grid="innerOuter.hdf"; 
* $grid="innerOuter4.hdf"; 
*
* $grid="twoBoxesInterface1.hdf";  $kappa1=.5; $kappa2=.1; $left="leftBox"; $right="rightBox"; $tPlot=.1; 
*
$grid
*
* 
*  -------------Start domain 1 --------------
setup $domain1
 set solver Cgad
 solver name solidA
 solver parameters
* 
  advection diffusion
  continue
* 
  pde parameters
    kappa $kappa1
    thermal conductivity $ktc1
  done
* 
  forward Euler
* 
  turn off twilight
*
  boundary conditions
*   all=dirichletBoundaryCondition
    all=neumannBoundaryCondition
    $left(0,0)=dirichletBoundaryCondition, uniform(T=$Tinflow)
    $left(1,0)=interfaceBoundaryCondition
    outerSquare(0,1)=dirichletBoundaryCondition, uniform(T=$Tinflow)
    outerAnnulus(0,1)=interfaceBoundaryCondition
    done
*
  cfl $cfl
*
  initial conditions
  uniform flow
    T=0.
  exit
 continue
done
* -----------End domain 1 ------------------
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
    kappa $kappa2
    thermal conductivity $ktc2
  done
* 
  forward Euler
* 
  turn off twilight
*
  boundary conditions
*     all=dirichletBoundaryCondition
    all=neumannBoundaryCondition
    $right(0,0)=interfaceBoundaryCondition
    innerAnnulus(1,1)=interfaceBoundaryCondition
    done
*
  cfl $cfl
*
  initial conditions
  uniform flow
    T=0.
  exit
 continue
done
* -----------End domain 2 ------------------
continue
* -- set parameters for cgmp ---
  final time $tFinal
  times to plot $tPlot
  debug flag $debug
  forward Euler
  turn off twilight
  continue
*
continue


  contour
    min max 0 1
    ghost lines $ghost
    exit
    min max 0 1
    ghost lines $ghost
    exit

movie mode
finish


