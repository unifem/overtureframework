#
# Two domain verification test of fluid flow and heat transfer (TZ flow)
#          ia = cgIns + cgAd 
# Usage:
#      cgmp ia -g=<grid-name> -tf=<> -tp=<> -degreeX=[0|1|2..] -degreeT=[0|1|2..]
#
#  -degreeX=n, -degreeT=m : exact solution is a polynomial of this degree in x and t.
# 
# Examples:
#         cgmp ia -degreeX=1 -degreeT=1   [ exact 
#         cgmp ia -degreeX=2 -degreeT=1   [ exact 
#         cgmp ia -degreeX=2 -degreeT=2   [not exact
#         cgmp ia -g=twoSquaresInterfacee2.order2 -degreeX=2 -degreeT=1   [exact
#
$solver="yale";
$solver="choose best iterative solver"; $rtol=1.e-12; $atol=1.e-14; 
$nu=.1; $kThermal=.1; $kappa=.1; $thermalExpansivity=.1; $degreeX=2; $degreeT=1; $debug=0; 
$tFinal=.1; $tPlot=.05; 
$domain1="leftDomain"; $domain2="rightDomain";
# 
# -- set the solver:
$ksp="bcgs"; $pc="bjacobi"; $subksp="preonly"; $subpc="ilu"; $iluLevels=1; 
# $ksp="bcgs"; $pc="hypre"; 
# 
$grid="twoSquaresInterfacee1.order2.hdf"; $degreeX=1; $degreeT=1; $debug=0; $tPlot=.01; $debug=7; 
#
GetOptions( "g=s"=>\$grid,"tf=f"=>\$tFinal,"tp=f"=>\$tPlot,"degreeX=i"=>\$degreeX,"degreeT=i"=>\$degreeT );
#
$grid
# 
#
# ------- start new domain ----------
setup $domain1
 set solver Cgins
 solver name fluid
 solver parameters
  incompressible Navier Stokes
  Boussinesq model
  define real parameter thermalExpansivity $thermalExpansivity
  define real parameter adcBoussinesq 0.
  continue
# 
  forward Euler
#
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  OBTZ:degree in space $degreeX
  OBTZ:degree in time $degreeT
  pressure solver options
     $solver
 # yale
 # these tolerances are chosen for PETSc
#     define petscOption -ksp_type $ksp
#     define petscOption -pc_type $pc
#     define petscOption -sub_ksp_type $subksp
#     define petscOption -sub_pc_type $subpc
#     define petscOption -pc_factor_levels $iluLevels
#     define petscOption -sub_pc_factor_levels $iluLevels
# 
     relative tolerance
       $rtol
     absolute tolerance
       $atol
     debug 
       0
    exit
  pde parameters
    nu $nu
    kThermal $kThermal
    thermal conductivity $kThermal
    gravity
      0. -1. 0. 
  done
  boundary conditions
    all=dirichletBoundaryCondition
    bcNumber100=noSlipWall
    bcNumber100=heatFluxInterface
  done
  continue
done
# 
# ------- start new domain ----------
setup $domain2
 set solver Cgad
 solver name solid
 solver parameters
# 
  advection diffusion
  continue
# 
  pde parameters
    kappa $kappa
    thermal conductivity $kappa
    a 1. 
    b 1. 
  done
# 
  forward Euler
#
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  OBTZ:degree in space $degreeX
  OBTZ:degree in time $degreeT
  boundary conditions
    all=dirichletBoundaryCondition
    # old way: bcNumber100=interfaceBoundaryCondition
    bcNumber100=mixedBoundaryCondition, mixedDerivative(0.*t+1.*t.n=1.)
    bcNumber100=heatFluxInterface
  done
  continue
done
# 
continue
# -- set parameters for cgmp ---
  final time $tFinal
  times to plot $tPlot
  forward Euler
  continue
# 
continue


  movie mode
  finish

