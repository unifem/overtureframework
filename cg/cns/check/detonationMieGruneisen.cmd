*
* Test of the Mie-Gruneisen EOS with reactions
*    Solve for a detonation travelling wave profile 
*
$cfl=.9;
$mu=0.; $kThermal=0.; $Prandtl=.72;
$levels=3;  $tFinal=5.e-3; $tPlot=5.e-3; 
$eos = "Mie-Gruneisen equation of state";
$errTol=.0005; 
$grid="channelShort.hdf";
$amr = "turn on adaptive grids";
*
* results from profile.m
*
* ------ Case 0 (note gamma) ------
*  $x2=-2.104e-01; $r2=1.572560e+00; $u2=-2.023203e+00; $T2=2.936672e+00; $Y2=9.999900e-01;
*  $a=.0; $b=.0; $v0=1.; $gamma=1.4; 
*  $kappa=1.;  # Cp = Cv + kappa*R
*  $U=3.1816087; 
*  $r1=1.; $u1=-$U; $T1=.9325;
*  $eps=.075; $sigma=1.; $Q=4.; 
*  $dataFile="oneStepIdealProfile.data";  
**  $levels=2;  $showFile = "detonationMGKappa0p0-Level2.show";   $tPlot=.2;   $tFinal=4.; 
*   $levels=2;  $showFile = "detonationMGKappa0p0-Level2Fine.show";   $tPlot=.2;   $tFinal=4.; 
*   $levels=3;  $showFile = "detonationIdeal-Level3.show";  $eos="ideal gas law";  $tPlot=.2;  $tFinal=4.; 
*  $levels=1; $grid="channelShort64.hdf";  $showFile = "detonationIdealFine.show";  $eos="ideal gas law";  $tPlot=.2;  $tFinal=4.; $amr="turn off adaptive grids";
*    $levels=3;  $showFile = "detonationIdeal-Level3.show";  $eos="ideal gas law";  $tPlot=.2;  $tFinal=4.; 
*  $levels=3;  $showFile = "detonationMGKappa0p0-Level3.show";     $tPlot=.2;  $tFinal=4.; 
*
* ------ Case 1 ------
* --- $eps=.075; $sigma=1.; $Q=4.;  -> uCJ =  2.8663906  (2.8638483)  (old=2.85243)
*   $x2=-3.073e-01; $r2=1.572641e+00; $u2=-1.822660e+00; $T2=2.568932e+00; $Y2=9.999900e-01;
*    $a=.5; $b=.5; $v0=1.; $gamma=4./3.; 
*    $kappa=1.;  # Cp = Cv + kappa*R
*    $U=2.8663906; 
*    $r1=1.; $u1=-$U; $T1=.9325;
*    $eps=.075; $sigma=1.; $Q=4.; 
*    $dataFile="oneStepMieGruneisenProfile.data";  
*  * *  $levels=2;  $showFile = "detonationMGKappa1p0-Level2.show";
*    $levels=3;  $showFile = "detonationMGKappa1p0-Level3.show";     $tFinal=2.; 
* ------ Case 2: kappa=1.5 ------
* --- $eps=.075; $sigma=1.; $Q=4.;  -> uCJ =  3.0420251
   $x2=-1.080e+00; $r2=1.598631e+00; $u2=-1.910595e+00; $T2=2.088971e+00; $Y2=9.999900e-01;
   $a=.5; $b=.5; $v0=1.; $gamma=4./3.; 
   $kappa=1.5;  # Cp = Cv + kappa*R
   $U=3.0543360;    #   3.0420251;   # wrong: $U=3.03207; 
   $r1=1.; $u1=-$U; $T1=.9325;
   $eps=.075; $sigma=1.; $Q=4.; 
   $dataFile="oneStepMieGruneisenKappa1p5Profile.data";
   $levels=2; $showFile = "detonationMGKappa1p5-Level2.show"; 
*   $levels=3; $showFile = "detonationMGKappa1p5-Level3.show"; $tFinal=1.; 
* -------------------------------
*
* --- ideal gas case
* $a=.0; $b=.0; $v0=1.; $gamma=1.4; 
* $U=3.18161;
* $r1=1.; $u1=-$U; $T1=.9325;
* $eps=.075; $sigma=1.; $Q=4.; 
* $dataFile="oneStepIdealProfile.data";
* ----- end Ideal
*
$mu=.0; $kThermal=$mu/$Prandtl; $cfl=.75; 
*
* channelFine
* channel.hdf
*  [0,1]
* channelShort: [201x3]
* channelShort
* channelShort2: [401x3]
* channelShort2
* channelShort8: [1601x3]
* channelShort8
* channelShort
*  [-1,1] 
*  channelShortish.hdf
* channelShort2.hdf
* channelShortCoarse
*
  $grid
*
  compressible Navier Stokes (Godunov)  
  $eos
  one step
*
    define real parameter alphaMG  $a
    define real parameter betaMG   $b
    define real parameter V0MG     $v0
    define real parameter kappaMG  $kappa
* 
  exit
  turn off twilight
  final time $tFinal
  times to plot $tPlot
  plot and always wait
  * no plotting
  show file options
    compressed
     * open
     * $showFile
     * detonationMG-Level2.show
     * detonationMG-Level3.show
     * detonationMG-Level4.show
     * detonationIdeal-Level2.show
     * detonationIdeal-Level3.show
     * detonationIdeal-Level4.show
     * detonationMGKappa1p5-Level2.show
     * detonationMGKappa1p5-Level3.show
     * detonationMGKappa1p5-Level3Fine.show
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
*     rectangle(0,0)=superSonicInflow uniform(r=$r2,u=$u2,T=$T2,s=$Y2)
    rectangle(1,0)=superSonicInflow uniform(r=$r1,u=$u1,T=$T1)
    done
*
    cfl $cfl
*
*  debug
*    3
*  turn on adaptive grids
  $amr
*
* turn on user defined error estimator
*
  order of AMR interpolation
      2
  regrid frequency
    8 4 8
  error threshold
      * .025  .1   * .1 was used for 3 levels 
      $errTol
  change error estimator parameters
    set scale factors     
      2 1 1 1 1 
    done
    exit
  change adaptive grid parameters
    refinement ratio
      4
    default number of refinement levels
      $levels
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
**      1d profile from a data file perturbed
      $dataFile
      * a0*sin(2*Pi*f0*y)*exp(-beta*(x-x0)^2) 
      * a0, f0, x0, beta
**       .3 4. .25 30.
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







  change the grid
    add a refinement
    rectangle
    set bounds
      .05 .45 0. 1.
    done
    done
   erase and exit
   erase and exit




