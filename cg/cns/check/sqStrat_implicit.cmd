*
* OverBlown command file for box in a gravitational field, implicit CNS
*
square20
$grid="square";
compressible Navier Stokes (implicit)
exit
turn off twilight
  final time .02
  times to plot .01
  plot and always wait
cfl 10 
implicit time step solver options
    fillin ratio
    40
choose best iterative solver
block size
4
    exit
implicit factor  .5
OBPDE:av2,av4 0.,1.
  pde parameters
  $gr =-2.;
  $grav = "0 $gr 0 0";
  $gamma=1.4; $Rg=1.;
    $Prandtl=.72;
    $Reynolds=1e2;
    $Mach = 8.451543e-01;
    $kThermal = $mu/$Prandtl;
    OBPDE:Rg (gas constant) $Rg
    OBPDE:Prandtl number $Prandtl
    OBPDE:Reynolds number $Reynolds
    OBPDE:Mach number $Mach
    OBPDE:gamma $gamma
    gravity
    $grav
  done
* states 
  $rho=1;
  $pwall=1;
  $twall=1;
  $rwall=$pwall/($twall*$Rg);
  $temp=$twall;
  $tmpb=$twall;
  $A = abs($gr)/($Rg*$temp);
*
* the following computation of $rho sets the amount of fluid
*  in the box such that $rho=1. at the ground when a steady state
*  is reached.
*
  $rho= $A>1e-15 ? $rwall*(1.-exp(-$A))/$A : 1;
  boundary conditions
  all=noSlipWall, uniform(T=$temp,r=$rho)
  $grid(0,1)=noSlipWall, userDefinedBoundaryData
      linear ramp in x
      3 $temp 0. 0.
      done
      done
  $grid(1,1)=noSlipWall, userDefinedBoundaryData
      linear ramp in x
      3 $temp 0. 0.
      done
      done
  done
  initial conditions
    uniform flow
      $tt = $temp;
      r=$rho u=0 v=0 T=$tt 
exit
    continue
movie mode
finish


