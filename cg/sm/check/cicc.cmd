$tFinal=.3; $tPlot=.1; $backGround="square"; 
$degreeX=2; $degreeT=2; 
*
$grid="cic"; $degreeX=2; $degreeT=2; $cons=1; 
*
$grid
* -new: set-up stage: 
linear elasticity
conservative
 continue
* 
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
use conservative difference $cons
*
boundary conditions
  all=dirichlet
done
*
debug 2
check errors 1
plot errors 1
continue
movie mode
finish
