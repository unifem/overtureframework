* 
* Command file for amrh
*
*  usage: amrh -cmd=amrhtz  -g=<name> -l=<levels> -r=[ratio] -tf=<tFinal> -tp=<tPlot> -tz=<tzType> -ee=<errEst>
*                           -rad=<num> -rk=[2/4] -go=[run/halt]
*
* Examples:
*
*    amrh -cmd=amrhtz -g=sise2.order2.hdf -xc=-.4 -yc=-.4 -tf=.7 -l=2 -r=2 -go=halt
*    amrh -cmd=amrhtz -g=sise2.order4.hdf -xc=-.4 -yc=-.4 -tf=.7 -l=2 -r=2 -go=halt
* 
* srun -N1 -n1 -ppdebug amrh -noplot -cmd=amrhtz -g=rsis2e.hdf -xc=-.4 -yc=-.4 -tf=.7 -l=3 -r=2 
* srun -N1 -n4 -ppdebug amrh -noplot -cmd=amrhtz -g=rsis2e.hdf -xc=-.4 -yc=-.4 -tf=.7 -l=3 -r=2 
* srun -N1 -n4 -ppdebug amrh -noplot -cmd=amrhtz -g=rbib2e.order2.hdf -xc=-.25 -yc=-.25 -zc=-.25 -tf=.5 -l=2 -r=4 -tz=pulse -ee=noTop >! rbib2e.pulse.l2r4.N1n4.out &  
* srun -N1 -n8 -ppdebug amrh -noplot -cmd=amrhtz >! sibe2.pulse.l2r4.N1n8.out &
* 
* srun -N1 -n4 -ppdebug amrh -noplot -cmd=amrhtz -g=rbibe2.order2.hdf -xc=-.25 -yc=-.25 -zc=-.25 -tf=.5 -l=2 -r=4 -tz=pulse -ee=noTop >! rbib2e.pulse.l2r4.N1n4.out &
* 
* srun -N1 -n8 -ppdebug amrh -noplot -cmd=amrhtz -g=rbibe2.order2.hdf -xc=-.25 -yc=-.25 -zc=-.25 -tf=.5 -l=2 -r=4 -tz=pulse -ee=noTop >! rbib2e.pulse.l2r4.N1n8.out &
*
* srun -N1 -n4 -ppdebug amrh -noplot -cmd=amrhtz -g=sibe1.order2.hdf -rad=.35 -xc=-1. -yc=-1. -zc=-1. -tf=.5 -l=2 -r=2 -tz=pulse -ee=noTop >! sibe1.pulse.l2r2.N1n4.out &
* 
* srun -N1 -n8 -ppdebug amrh -noplot -cmd=amrhtz -g=sibe2.order2.hdf -rad=.35 -xc=-1. -yc=-1. -zc=-1. -tf=.5 -l=2 -r=4 -tz=pulse -ee=noTop >! sibe2.pulse.l2r4.N1n8.out &
* 
* 
* mpirun -np 1 amrh noplot -cmd=amrhtz
* mpirun-wdh -np 1 amrh -cmd=amrhtz
* srun -ppdebug -N1 -n2 amrh -noplot -cmd=amrhtz
* srun -ppdebug -N4 -n4 memcheck_all amrh -noplot -cmd=amrhtz
* srun -ppdebug -N4 -n4 memcheck_all amrh -cmd=amrhtz
*
* mpirun -np 1 amrh -et=.1 -cmd=amrhtz
* 
* 
*  ---- set defaults for all parameters ----
* 
$tFinal=1.; $tPlot=.1; $debug=0; $numberOfLevels=2; $ratio=2; $nbz=2; $nu=1.e-2; $tol=.01; 
$xc=.25; $yc=.25; $zc=0.; $rad=.15; 
* $tzType="use pulse function";
$tzType="use poly";
$noTopHat="do not use top hat for error estimator";
$errEst="use top hat for error estimator";
* RK time stepping is only exact for degree 1 in time
$degreeSpace=2; $degreeTime=1; 
$rk=2; # use 2nd-order Runge-Kutta
$vx=1.; $vy=1.; $vz=1.; 
$efficiency=.7; 
$go="run";
*
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "g=s"=>\$grid,"l=i"=> \$numberOfLevels,"r=i"=> \$ratio, "tFinal=f"=>\$tFinal,"tol=f"=>\$tol, \
            "tp=f"=>\$tPlot, "xc=f"=>\$xc, "yc=f"=>\$yc, "zc=f"=>\$zc, "tz=s"=>\$tzType, "ee=s"=>\$errEst,\
            "rad=f"=>\$rad, "noplot=s"=>\$noplot, "go=s"=>\$go, "cmd=s"=>\$cmd, "rk=s"=>\$rk );
* -------------------------------------------------------------------------------------------------
if( $errEst eq "noTop" ){ $errEst="do not use top hat for error estimator"; }
if( $tzType eq "pulse" ){ $tzType="use pulse function"; }
if( $rk eq "2" ){ $rk = "2nd order Runge Kutta"; }else{ $rk="4th order Runge Kutta"; }
if( $go eq "run" ){ $go = "movie mode\n finish"; }else{ $go="break"; }
*
* $grid="square20"; $tFinal=.3; 
* $grid="square20.hdf"; $xc=.2; $yc=.2;  $tFinal=.4; $debug=0; $numberOfLevels=2; $ratio=2; 
* $grid="square40.hdf"; $xc=.2; $yc=.2;  $tFinal=.6; $debug=1; $numberOfLevels=3; $ratio=2; 
* $grid="square40.hdf"; $xc=.2; $yc=.2;  $tFinal=.4; $debug=0; $numberOfLevels=3; $ratio=4; 
* $grid="square128.hdf"; $xc=.5; $yc=.5; $rad; $tPlot=.01; $tFinal=.02; $debug=0; $numberOfLevels=3; $ratio=4; $efficiency=.6; $nu=1.e-3; 
* $grid="square256.hdf"; $xc=.5; $yc=.5; $rad=.3; $tPlot=.01; $tFinal=.02; $debug=0; $numberOfLevels=3; $ratio=4; $efficiency=.5; $nu=1.e-3; 
* $grid="square512.hdf"; $xc=.5; $yc=.5;  $tPlot=.005; $tFinal=.005; $debug=0; $numberOfLevels=2; $ratio=2; $nu=1.e-3; 
* $grid="square1024.hdf"; $xc=.2; $yc=.2;  $tPlot=.00001; $tFinal=.00001; $debug=0; $numberOfLevels=3; $ratio=4; $nu=.5e-3; 
* $grid="square2048.hdf"; $xc=.2; $yc=.2;  $tPlot=.0001; $tFinal=.0001; $debug=0; $numberOfLevels=2; $ratio=2; $efficiency=.25; $nu=.25e-3; $debug=3; 
* $grid="sbs2.hdf"; $xc=.25; $yc=.5;  $tFinal=1.2; $debug=0; $numberOfLevels=3; $vy=0.;
* $grid="sbsn2.hdf"; $xc=.25; $yc=.5;  $tFinal=1.2; $debug=0; $numberOfLevels=3; $vy=0.;
* $grid="sise.hdf"; $xc=-.3; $yc=-.7;  $tFinal=.5; $debug=3; $numberOfLevels=2;
* $grid="sis3e.hdf"; $xc=-.7; $yc=-.7;  $tFinal=1.; $debug=0; $ratio=4; $numberOfLevels=3;
* $grid="sis3.hdf"; $xc=-.7; $yc=-.7; 
* --------- rotated square in a square ------------
* $grid="rsis2e.hdf"; $xc=-.4; $yc=-.4;  $tFinal=.7; $debug=0; $numberOfLevels=3;
* $grid="rsis2e.hdf"; $xc=-.7; $yc=-.7;  $tFinal=1.4; $debug=3; $numberOfLevels=2;
* $grid="rsis2e.hdf"; $xc=-.7; $yc=-.7;  $tFinal=1.2; $debug=0; $numberOfLevels=3; $ratio=4; $nu=1.e-3;
* $grid="rsis4e.hdf"; $xc=-.6; $yc=-.6;  $tFinal=1.; $numberOfLevels=3; $debug=1; $efficiency=.4; $ratio=4; $nu=1.e-3;  
* $grid="cice.hdf"; $xc=-1.6; $yc=-1.6; $tFinal=3; $tPlot=.1; $numberOfLevels=3; $ratio=2;
* $grid="cice.hdf"; $xc=-1.; $yc=-1.6; $tFinal=3; $numberOfLevels=3; 
* $ta=.226;  $grid="cice.hdf"; $xc=.8+$ta; $yc=.3+$ta; $tFinal=3; $numberOfLevels=2; $debug=3; 
* $grid="cic2e.hdf"; $xc=-.2; $yc=-.6; $tFinal=1; $numberOfLevels=2; $debug=0; 
* $grid="cic2e.hdf"; $xc=-1.2; $yc=-1.6; $tFinal=3; $numberOfLevels=2; $debug=0; 
* $grid="cic2e.hdf"; $xc=-1.2; $yc=-1.6; $tFinal=3; $numberOfLevels=3; $debug=0; 
* $grid="cic3e.hdf"; $xc=-1.2; $yc=-1.6; $tFinal=2; $tPlot=.2; $ratio=4; $numberOfLevels=3; 
* $grid="cic5e.hdf"; $rad=.4; $xc=-.75; $yc=-.75; $tFinal=.0001; $tPlot=.00001; $ratio=4; $numberOfLevels=3; $debug=3;
* $grid="cic6e.hdf"; $rad=.4; $xc=-.75; $yc=-.75; $tFinal=.001; $tPlot=.001; $ratio=4; $numberOfLevels=3; $debug=3;
*
* $grid="box20.hdf"; $xc=.25; $yc=.25; $zc=.25; $tFinal=.5; $numberOfLevels=2; 
* $grid="box40.hdf"; $xc=.25; $yc=.25; $zc=.25; $tFinal=.5; $numberOfLevels=3; 
*
* ---- box in a box 
* $grid="bib2e.hdf"; $xc=-.75; $yc=-.75; $zc=-.75; $tFinal=1.; $numberOfLevels=3; 
* $grid="bib2e.hdf"; $xc=-.5; $yc=-.5; $zc=-.75; $tFinal=.05; $numberOfLevels=2; $debug=3;
* $grid="sib2e.hdf"; $xc=-.75; $yc=-.75; $zc=-.75; $tPlot=.05; $tFinal=.3; $numberOfLevels=2; 
*
* -- rotated box in a box with poly-hat
*  srun -N1 -n2 -ppdebug amrh -noplot -cmd=amrhtz >! rbib2e.l2r2.N1n2.out &
* $grid="rbibe2.order2.hdf"; $xc=-.75; $yc=-.75; $zc=-.75; $tFinal=1.; $numberOfLevels=2;
* $grid="rbibe2.order2.hdf"; $xc=-.75; $yc=-.75; $zc=-.75; $tFinal=1.; $numberOfLevels=3;
* srun -N1 -n8 -ppdebug amrh -noplot -cmd=amrhtz >! rbib2e.l2r4.N1n8.out &
* $grid="rbibe2.order2.hdf"; $xc=-.75; $yc=-.75; $zc=-.75; $tFinal=1.; $numberOfLevels=2; $ratio=4; 
* $grid="rbibe2.order2.hdf"; $xc=-.5; $yc=-.5; $zc=-.5; $tFinal=.1; $numberOfLevels=3; $ratio=4; 
* $grid="rbibe4.order2.hdf"; $xc=-.5; $yc=-.5; $zc=-.5; $tFinal=.5; $numberOfLevels=2; $ratio=4; 
* $grid="rbibe4.order2.hdf"; $xc=-.5; $yc=-.5; $zc=-.5; $tFinal=.25; $numberOfLevels=3; $ratio=4; 
* -- rotated box in a box with pulse 
* $grid="rbibe2.order2.hdf"; $xc=-.75; $yc=-.75; $zc=-.75; $tFinal=1.; $numberOfLevels=2;
* $grid="rbibe2.order2.hdf"; $xc=-.75; $yc=-.75; $zc=-.75; $tFinal=.5; $tzType="use pulse function"; $errEst=$noTopHat;
* 
* srun -N1 -n4 -ppdebug amrh -noplot -cmd=amrhtz >! rbib2e.pulse.l2r4.N1n4.out &
* $grid="rbibe2.order2.hdf"; $xc=-.25; $yc=-.25; $zc=-.25; $tFinal=.5; $numberOfLevels=2; $ratio=4; $tzType="use pulse function"; $errEst=$noTopHat;
* srun -N1 -n2 -ppdebug amrh -noplot -cmd=amrhtz >! rbib2e.pulse.l3r2.N1n2.out &
* srun -N1 -n4 -ppdebug amrh -noplot -cmd=amrhtz >! rbib2e.pulse.l3r2.N1n4.out &
* $grid="rbibe2.order2.hdf"; $xc=-.25; $yc=-.25; $zc=-.25; $tFinal=.5; $numberOfLevels=3; $ratio=2; $tzType="use pulse function"; $errEst=$noTopHat;
* srun -N1 -n8 -ppdebug amrh -noplot -cmd=amrhtz >! rbib4e.pulse.l2r2.N1n8.out &
* $grid="rbibe4.order2.hdf"; $xc=-.25; $yc=-.25; $zc=-.25; $tFinal=.5; $numberOfLevels=2; $tzType="use pulse function"; $errEst=$noTopHat;
* srun -N2 -n16 -ppdebug amrh -noplot -cmd=amrhtz >! rbib8e.pulse.l1r2.N2n16.out &
* $grid="rbibe8.order2.hdf"; $xc=-.25; $yc=-.25; $zc=-.25; $tFinal=.5; $numberOfLevels=1; $tzType="use pulse function"; $errEst=$noTopHat;
* 
* -- sphere in a box --
* $grid="sibe2.order2.hdf"; $xc=-.65; $yc=-.65; $zc=-.65; $tFinal=.5; $numberOfLevels=2;
* 
* trouble here: 
* srun -N1 -n8 -ppdebug amrh -noplot -cmd=amrhtz >! sibe2.pulse.l2r4.N1n8.out &
** $grid="sibe2.order2.hdf"; $xc=-.65; $yc=-.65; $zc=-.65; $tFinal=.5; $numberOfLevels=2; $ratio=4; $tzType="use pulse function"; $errEst=$noTopHat;
* $grid="sibe4.order2.hdf"; $xc=-.65; $yc=-.65; $zc=-.65; $tFinal=.5; $numberOfLevels=2; $tzType="use pulse function"; $errEst=$noTopHat;
* 
* srun -N2 -n16 -ppdebug amrh -noplot -cmd=amrhtz >! sibe4.pulse.l1r2.N2n16.out &
* $grid="sibe4.order2.hdf"; $xc=-.65; $yc=-.65; $zc=-.65; $tFinal=.5; $numberOfLevels=1; $tzType="use pulse function"; $errEst=$noTopHat;
* srun -N1 -n8 -ppdebug amrh -noplot -cmd=amrhtz >! sibe4.pulse.l2r2.N1n8.out &
* $grid="sibe4.order2.hdf"; $xc=-.65; $yc=-.65; $zc=-.65; $tFinal=.5; $numberOfLevels=2; $tzType="use pulse function"; $errEst=$noTopHat;
* srun -N2 -n16 -ppdebug amrh -noplot -cmd=amrhtz >! sibe8.pulse.l1r2.N2n16.out &
* $grid="sibe8.order2.hdf"; $xc=-.65; $yc=-.65; $zc=-.65; $tFinal=.5; $numberOfLevels=1; $tzType="use pulse function"; $errEst=$noTopHat;
*
$grid
* 
$rk
* 
* turn off graphics
*
$errEst
top hat parameters
  $xc $yc $zc 
  $rad
  $vx $vy $vz
* turn off load balancer
change load balancer
  * sequential assignment
  * random assignment
exit
nu 
  $nu
error threshold
  $tol 
grid efficiency
   $efficiency
refinement ratio
  $ratio
number of buffer zones
  $nbz
regrid interval
  $regrid=$ratio*$nbz;
  $regrid
number of refinement levels
  $numberOfLevels
* regrid interval
*   10000
debug
  $debug
* ogen debug
*   3
*  
* --- TZ parameter ---
use twilight
$tzType
order in space
  $degreeSpace
order in time
  $degreeTime
* 
solve
final time
  $tFinal
plot time interval
  $tPlot
* to print errors at every step:
* plot interval
*  1
* 
grid
 plot shaded surfaces (3D) 0
exit
* 
$go


movie mode
exit


contour
 vertical scale factor 0.
exit

* 
grid
plot shaded surfaces (3D) 0
exit


erase
grid

movie mode
exit
exit

