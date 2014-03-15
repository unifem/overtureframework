$tFinal=1.; $tPlot=.1; $debug=0;
$grid="square20.hdf";
* 
$grid
*
*  -------------Start domain 1 --------------
  Cgad Solid
* 
  advection diffusion
  continue
* 
  forward Euler
* 
  OBTZ:polynomial
  OBTZ:twilight zone flow 1
  OBTZ:degree in space 2
  OBTZ:degree in time 1
* 
 continue
*  -------------End domain 1 ----------------
* -- set parameters for cgmp ---
  final time $tFinal
  times to plot $tPlot
*  turn off twilight
  debug flag $debug
  continue
*