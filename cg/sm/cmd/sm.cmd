$tFinal=10.; $tPlot=.05; $backGround="square"; 
$diss=0.; $mu=1.; $lambda=1.; 
$exponent=10.; $x0=.5; $y0=.5; $z0=.5; 
* 
* $grid="square40"; $diss=1.;  $lambda=1.; 
* $grid="square80"; 
* $grid = "square20.order4"; $exponent=40.; 
* $grid = "square40.order4"; $exponent=60.; $diss=.0; 
* $grid="cic3"; $diss=1.;  $lambda=1.; 
* $grid="sice3.order2.hdf"; $diss=1.;  $lambda=20.; 
$grid="sice4.order2.hdf"; $x0=.0; $y0=.0;  $diss=1.;  $lambda=1.; 
* $grid="sib3"; $x0=1.25; $y0=.0; $z0=.0; 
* $grid="bis2e"; $x0=-.9; $y0=.0; $z0=.0; 
* $grid="box20"; $backGround="box"; 
* 
$grid 
* 
forcing options... 
gaussianPulseInitialCondition 
Gaussian pulse: 10 2 $exponent $x0 $y0 $z0 (beta,scale,exponent,x0,y0,z0) 
close forcing options 
tFinal $tFinal 
tPlot $tPlot 
* 
NFDTD 
* 
lambda $lambda 
mu $mu 
* 
* bc: $backGround=stressFree 
bc: all=stressFree 
* 
displacement scale factor 0.300000 
dissipation $diss 
* 
plot divergence 1 
plot vorticity 1 
* 
continue 
* 
plot:div(U)
continue
continue
continue
continue
continue
continue
continue
continue
continue
continue
continue
continue
continue
plot:vorz
continue
continue
continue
continue
continue
continue
continue
finish
