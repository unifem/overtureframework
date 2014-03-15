*
* plotStuff plotQuarterSphere.cmd -show=<name>
*
* Examples:
*    plotStuff plotQuarterSphere.cmd -show=quarterSphere
*    plotStuff plotQuarterSphere.cmd -show=quarterSphere1el2r2.show
*    srun -N1 -n2 -ppdebug $ovp/bin/plotStuff plotQuarterSphere.cmd -show=qs2el2r2.show
*
$show="qs2el3r2.show";
* ----------------------------- get command line arguments ---------------------------------------
GetOptions( "show=s"=>\$show, "noplot=s"=>\$noplot );
* -------------------------------------------------------------------------------------------------
* $show="quarterSphere1el2r2.show";
* $show="quarterSphere2el2r2.show";
* $show="quarterSphere3el2r4.show";
*
$show
previous
*
  contour
    pick to delete contour planes
    delete contour plane 2
    delete contour plane 1
    delete contour plane 0
    pick to add contour plane z
    add contour plane  0.00000e+00  0.00000e+00  1.00000e+00   0. 0. 1.e-3
    pick to add contour plane y
    add contour plane  0.00000e+00  1.00000e+00  0.00000e+00   0. 0. 1.e-3
 pause
    exit
* 
* 
  grid
    plot shaded surfaces (3D) 0
    toggle grid 0 0
    toggle shaded surfaces 1 0
    toggle shaded surfaces 2 0
    toggle shaded surfaces 3 0
    set view:0 0 0 0 1 0.831533 0.16566 -0.530197 0.0313766 0.938962 0.342588 0.554588 -0.301509 0.77558
    plot block boundaries 0
   *  plot grid lines 0
 pause
  exit this menu
*
