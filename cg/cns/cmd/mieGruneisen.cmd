*
* Test of the Mie-Gruneisen EOS 
*
$cfl=.75;
$mu=0.; $kThermal=0.; $Prandtl=.72; $kappa=1.;
$tFinal=5.; $tPlot=.5; 
*
* Left and right states are from cg/cns/doc/shock.maple
*
*
*=======case: ( -Ec fix made )
* U= 2.00000, a= 0.50000, b= 0.50000, v0= 1.00000, gamma= 1.33333
*  [r1,u1,p1,E1,T1]=[ 1.00000, 0.00000, 0.58333, 1.75000, 0.58333]
*  [r2,u2,p2,E2,T2]=[ 3.26647, 1.38772, 3.35877,13.32886, 1.06077]
 $U=2.; 
 $a=.5; $b=.5; $v0=1.; $gamma=4./3.; 
 $r1=1.;      $u1=0;       $E1=1.75;     $T1=0.58333;
 $r2=3.26647; $u2=1.38772; $E2=13.32886; $T2=1.06077;
 $show = "mieGruneisenNonReacting.show";
*
* =========  case kappa=1.5 --> only T changes -- should make no difference
*  U= 2.00000, a= 0.50000, b= 0.50000, v0= 1.00000, gamma= 1.33333 kappa= 1.50000
*  [r1,u1,p1,E1,T1]=[ 1.00000, 0.00000, 0.58333, 1.75000, 0.38889]
*  [r2,u2,p2,E2,T2]=[ 3.26647, 1.38772, 3.35877,13.32886, 0.70718]
* $U=2.; 
* $a=.5; $b=.5; $v0=1.; $gamma=4./3.; 
* $r1=1.;      $u1=0;       $E1=1.75;     $T1=0.38889; 
* $r2=3.26647; $u2=1.38772; $E2=13.32886; $T2=0.70718;
* $show = "mieGruneisenNonReactingKappa1p5.show";
* === case: ideal
* U= 2.00000, a= 0.00000, b= 0.00000, v0= 1.00000, gamma= 1.40000
*  [r1,u1,p1,E1,T1]=[ 1.00000, 0.00000, 0.70000, 1.75000, 0.70000]
*  [r2,u2,p2,E2,T2]=[ 2.69663, 1.25833, 3.21667,10.17659, 1.19285]
*
* $U= 2.00000; $a= 0.00000; $b= 0.00000; $v0= 1.00000; $gamma= 1.4; 
* $r1=1.00000; $u1=0.00000; $p1=0.70000; $E1=1.75000;  $T1=0.70000;
* $r2=2.69663; $u2=1.25833; $p2=3.21667; $E2=10.17659; $T2=1.19285;
* $show = "mieGruneisenIdeal.show";
* 
*
* subtract out the shock speed
 $E1=$E1+.5*$r1*(-$u1*$u1 + ($u1-$U)*($u1-$U)); $u1=$u1-$U;
 $E2=$E2+.5*$r2*(-$u2*$u2 + ($u2-$U)*($u2-$U)); $u2=$u2-$U;
*
$mu=.0; $kThermal=$mu/$Prandtl; 
*
** channelFine
* channel.hdf
channelShort
* channelShortCoarse
*  
  compressible Navier Stokes (Godunov)  
  Mie-Gruneisen equation of state
**  one step
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
    open
      $show
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
      0.
    rate constant
      0.
   reciprocal activation energy
     1.
  done
  boundary conditions
    rectangle=slipWall
*    rectangle(0,0)=superSonicInflow uniform(r=$r2,u=$u2,T=$T2)
*    rectangle(1,0)=superSonicOutflow
*
    rectangle(1,0)=superSonicInflow uniform(r=$r1,u=$u1,T=$T1)
*    rectangle(0,0)=superSonicInflow uniform(r=$r2,u=$u2,T=$T2)
     rectangle(0,0)=superSonicOutflow
    done
*
    cfl $cfl
*
*  debug
*    3
  turn on adaptive grids
*
**  turn on user defined error estimator
*
  order of AMR interpolation
      2
  regrid frequency
    8 4 8
  error threshold
      .1 
  change error estimator parameters
    set scale factors     
      2 1 1 1 1 
    done
    exit
  change adaptive grid parameters
    refinement ratio
      4
    default number of refinement levels
      2 3 4 3 2 3 2 3 4 
    number of buffer zones
      2
  exit
*
*  allow user defined output 1
*
  initial conditions
    * x=.5
    step function
      x=.5
      r=$r2 u=$u2 T=$T2
      r=$r1 u=$u1 T=$T1
    continue
***************
  debug
    0 1 0 3
***************
   continue

movie mode
finish


  contour
   wire frame (toggle)
  exit this menu





  change the grid
    add a refinement
    rectangle
    set bounds
      .05 .45 0. 1.
    done
    done
   erase and exit
   erase and exit




