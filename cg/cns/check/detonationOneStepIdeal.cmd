*
*    Solve for a detonation travelling wave profile 
*
$cfl=.9;
$mu=0.; $kThermal=0.; $Prandtl=.72;
*
* results from ob/doc/shock.maple
*
* --- ideal gas case
 $a=.0; $b=.0; $v0=1.; $gamma=1.4; 
 $U=3.18161;
 $r1=1.; $u1=-$U; $T1=.9325;
 $eps=.075; $sigma=1.; $Q=4.; 
 $dataFile="oneStepIdealProfile.data";
* ----- end Ideal
*
$mu=.0; $kThermal=$mu/$Prandtl; $cfl=.75; 
*
* channelFine
* channel.hdf
*  [0,1]
* channelShort
channelShort
*  [-1,1] 
*  channelShortish.hdf
* channelShort2.hdf
* channelShortCoarse
  compressible Navier Stokes (Godunov)  
  Mie-Gruneisen equation of state
  one step
*
    define real parameter alphaMG  $a
    define real parameter betaMG   $b
    define real parameter V0MG     $v0
    define real parameter kappaMG  $kappa
* 
  exit
  turn off twilight
  final time .02
  times to plot .01
  plot and always wait
  * no plotting
  show file options
    compressed
     * open
     * detonationMG-Level2.show
    frequency to flush
      2
    exit
*
 reduce interpolation width
   2
*
*  variable time step PC
*
  OBPDE:gamma $gamma
  pde parameters
    mu
     $mu
    kThermal
     $kThermal
*    Mach number
*      1.
*    conservative Godunov
    heat release
      $Q
    rate constant
      $sigma
   reciprocal activation energy
     $eps
  done
  boundary conditions
    rectangle=slipWall
    rectangle(0,0)=superSonicOutflow
    rectangle(1,0)=superSonicInflow uniform(r=$r1,u=$u1,T=$T1)
    done
*
    cfl $cfl
*
*  debug
*    3
 turn on adaptive grids
*
* turn on user defined error estimator
*
  order of AMR interpolation
      2
  regrid frequency
    4 
  error threshold
      .025  .1   * .1 was used for 3 levels 
  change error estimator parameters
    set scale factors     
      2 1 1 1 1 
    done
    exit
  change adaptive grid parameters
    refinement ratio
      2
    default number of refinement levels
      2 
    number of buffer zones
      2
  exit
*
*  allow user defined output 1
*
***********************************************
  initial conditions
    user defined
     1d profile from a data file 
      $dataFile
    exit
  exit
********************************************
***************
  debug
    0 3
***************
   continue
*
movie mode
finish




