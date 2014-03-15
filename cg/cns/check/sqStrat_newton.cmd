* 
* OverBlown command file for box in a gravitational field, implicit CNS 
* 
square20
$grid="square";
  steady-state compressible Navier Stokes (newton) 
  exit 
debug
1
turn off twilight zone
max iterations 5
  plot and always wait 
  cfl 1 
* Oges::debug (od=) 
*63
* 3 
  implicit time step solver options 
    fillin ratio 
    40 
   choose best iterative solver 
    choose best direct solver 
*harwell
*PETSc
    exit 
  refactor frequency 1 
  implicit factor .9
  OBPDE:av2,av4 0.,1e-8
  pde parameters 
    $gr =2; 
    $grav = "0 -$gr 0 0"; 
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
all=noSlipWall, uniform(T=$temp,u=0,v=0)
*    $grid(0,0)=noSlipWall, userDefinedBoundaryData
*      linear ramp in x
*      1 0. 0. 0.
*      2 0. 0. 0.
*      3 $temp 0. 0.
*    done
*    done 
*    $grid(1,0)=noSlipWall, userDefinedBoundaryData
*      linear ramp in x
*      1 0. 0. 0.
*      2 0. 0. 0.
*      3 $temp 0. 0.
*    done
*    done 
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
      r=$rho, T=$temp, u=0, v=0
     exit
continue
plot iterations 1
movie mode
finish
