$grid = "inflow_outflow.str";
$grid = "inflow_outflow";
$grid = "rectangle.str";
$grid = "rectangle.reparam";
$grid = "rectangle";
$square = "rectangle";
$show = "wheel.show";
*  $gr =0;
*  $grav = "0 $gr 0 0";
$gamma=1.4; 
$Rg=1.;
$Prandtl=.72;
$Reynolds=1e3;
$temp = 1;
$Mach = sqrt(1./($gamma*$temp*$Rg));
$rmax = 1.1;
$rmin = .1;
$A2 = 1;
$rwall = 1;
$mass = (exp($A2)-exp($A2*$rmin*$rmin/($rmax*$rmax)))*$rwall*exp(-$A2)*$rmax*$rmax/($A2);
$rho = $mass/(($rmax*$rmax-$rmin*$rmin));
$omega = sqrt($A2*2.*$temp*$Rg/($rmax*$rmax));
$av4 = 0.;
$strick = 0.166667;
$dnewt = 1.;
$refact = 1;
$debug = 0;
$oges_db = 0;
  $solver = "iterative solver\n lu preconditioner";
*$solver = "direct\n";
*  $solver = "iterative solver\n";
$ksp = "gmres";
$pc = "bjacobi";
$pc = "lu";
*$pc = "ilu"; $iluLevels =4;
*
*
$grid
  steady-state compressible Navier Stokes (newton)
*  compressible Navier Stokes (implicit)
   axisymmetric flow with swirl 1
  exit
cylindrical axis is x axis
*
  show file options
    compressed
      open
      $show
    frequency to save
    20
    frequency to flush
     10
    exit
debug
$debug
turn off twilight
plot residuals 1
*
refactor frequency $refact
implicit factor $dnewt
OBPDE:av2,av4 0.,$av4
OBPDE:scoeff $strick
Oges::debug (od=)
$oges_db
implicit time step solver options
   choose best $solver
   block size
   5 
   relative tolerance
   1e-20
   absolute tolerance
   1e-4
  define petscOption -ksp_monitor stdout    
*  define petscOption -ksp_truemonitor stdout    
     define petscOption -ksp_type $ksp
     define petscOption -ksp_gmres_restart 200
     define petscOption -ksp_gmres_modifiedgramschmidt 1
     define petscOption -pc_type $pc
     define petscOption -pc_factor_levels $iluLevels
     define petscOption -pc_factor_shift
   maximum number of iterations
   600
exit
pde parameters
  OBPDE:Rg (gas constant) $Rg
  OBPDE:Prandtl number $Prandtl
  OBPDE:Reynolds number $Reynolds
  OBPDE:Mach number $Mach
  OBPDE:gamma $gamma
  gravity
  $grav
done
boundary conditions
* all=noSlipWall, uniform(u=0,v=0,T=$temp)
    $win = .1*$omega;
 $square(0,0)=noSlipWall userDefinedBoundaryData
  axisymmetric rotation 
  $omega 0. 0. $temp
  done
 $square(1,0)=noSlipWall userDefinedBoundaryData
  axisymmetric rotation 
  $omega 0. 0. $temp
  done
  $square(0,1)=slipWall
*# $square(0,1)=noSlipWall userDefinedBoundaryData
*# axisymmetric rotation
*# $omega 0 0 $temp
*# done
 $square(1,1)=noSlipWall userDefinedBoundaryData
 axisymmetric rotation
 $omega 0 0 $temp
 done
  done
done
initial conditions
uniform flow
r=$rho, T=$temp, u=0, v=0,w=$win
exit
continue
max iterations 1000
plot iterations 1



