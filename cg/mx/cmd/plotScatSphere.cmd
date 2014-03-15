*
*  plotStuff plotScatSphere.cmd
*
$show="scatSibx2.order4.show";
*
$show
* 
  DISPLAY COLOUR BAR:0 0
  DISPLAY AXES:0 0
  DISPLAY LABELS:0 0
* 
previous
contour
  plot the grid
    toggle grid 0 0
    plot block boundaries 0
    plot grid lines 0
    pick colour...
    PIC:brass
    grid colour 1 BRASS
    grid colour 2 BRASS
    close colour choices
    exit this menu
  component 1
  min max -0.8 .8
  bigger:0
  bigger:0
  smaller:0
  remove contour planes
    0 1
    done
  bigger:0
  smaller:0
  x-:0
  x-:0
  x+:0
  x+:0
  bigger:0
  hardcopy file name:0 scatSibx2-Ey.ps
  hardcopy save:0
  contour lines 0
  contour lines 1
  component 0
  min max -1. 1.
  hardcopy file name:0 scatSibx2-Ex.ps



  reset:0
  y+r:0
  y+r:0
  y+r:0
  reset:0
  y+r:0
  y+r:0
  y+r:0
  x+r:0
  x+r:0
  x+r:0
  component 2
  component 0
  component 1
  remove contour planes
    0
    done
  pick to delete contour planes
  delete contour plane 0
  reset:0
  component 0
  component 1
  component 2
  y+r:0
  y+r:0
  y+r:0
  x+r:0
  pick to add contour plane z
  add contour plane  0.00000e+00  0.00000e+00  1.00000e+00  1.32347e-02 -1.13392e+00  2.61456e-02 
  pick to add contour plane y
  add contour plane  0.00000e+00  1.00000e+00  0.00000e+00 -2.84421e-01 -1.17303e+00  5.10146e-02 
  x+r:0
  component 0
  add contour plane  0.00000e+00  1.00000e+00  0.00000e+00  1.76742e+00 -8.00099e-01  6.27769e-02 
  component 1
  component 2
  min max -.3 3.
  min max -0.3 .3
  pick to delete contour planes
  delete contour plane 2
  component 0
  min max -1. 1.
  contour lines 0
  contour lines 1
  shaded surfaces 0
  shaded surfaces 1
  component 1
  min max -.9 .6
  component 0
  reset min max
  min max -.7 .7
  min max -0.8 .8
  bigger:0
  bigger:0
  component 1
  reset:0
  set view:0 -0.0362538 -0.00604226 0 2.19497 1 0 0 0 1 0 0 0 1
  contour lines 0
  contour lines 1
  reset:0
  y+r:0
  set view:0 0.00302118 -0.0513595 0 1.79897 0.984808 0 -0.173648 0 1 0 0.173648 0 0.984808
  reset:0
  contour shift 0.2
  set view:0 -0.0966767 0.0060423 0 1.6802 1 0 0 0 1 0 0 0 1
  component 0
  component 1
  y+r:0
