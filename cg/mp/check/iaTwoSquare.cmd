*
* Two domain example showing fluid flow and heat transfer 
*
$tFinal=.1; $tPlot=.05; $nu=.1; $kThermal=.1; $ktcFluid=.1; $kappa=1.; $a1=1.; $b1=1; 
$grid="twoSquaresInterface1.hdf";
*
$grid
* 
*
* ------- start new domain ----------
setup leftDomain
 set solver Cgins
 solver name fluid
 solver parameters
* 
  incompressible Navier Stokes
  continue
  pde parameters
    nu  $nu
    kThermal $kThermal
    thermal conductivity $ktcFluid
   done
* 
  forward Euler
*
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  OBTZ:degree in space 2
  OBTZ:degree in time 1
  boundary conditions
    all=dirichletBoundaryCondition
  done
  continue
done
* 
* ------- start new domain ----------
setup rightDomain
 set solver Cgad
 solver name solidB
 solver parameters
* 
  advection diffusion
  continue
* 
  pde parameters
    kappa $kappa
    thermal conductivity $kappa
    a $a1
    b $b1 
  done
* 
  forward Euler
*
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  OBTZ:degree in space 2
  OBTZ:degree in time 1
  boundary conditions
    all=dirichletBoundaryCondition
  done
  continue
done
continue
* 
* -- set parameters for cgmp ---
  final time $tFinal
  times to plot $tPlot
  forward Euler
  continue
* 
  movie mode
  finish

