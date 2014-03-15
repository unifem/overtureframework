$tFinal=.3; $tPlot=.1; $backGround="square"; 
$degreeX=2; $degreeT=2; 
*
* $grid="square10"; $degreeX=2; $degreeT=2; 
* $grid="nonSquare10";
* $grid = "square10.order4"; $degreeX=4; 
$grid="sis"; $backGround="outer-square";
* $grid="box5"; $backGround="box"; 
* $grid="nonBox5"; $backGround="box"; 
*
$grid
* -new: set-up stage: 
linear elasticity
 continue
* 
modifiedEquationTimeStepping
*
OBTZ:polynomial
OBTZ:twilight zone flow 1
OBTZ:degree in space $degreeX
OBTZ:degree in time $degreeT
* 
final time $tFinal
times to plot $tPlot
*
boundary conditions
  all=displacementBC
done 
*
debug 2
check errors 1
* plot errors 1
continue
movie mode
finish

