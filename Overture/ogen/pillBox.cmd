*
* BC: 1 = pec
*     2 = pec (end)
*     3 = symmetry
***************************************************************************
$orderOfAccuracy = "second order";
$interpolation="implicit";
*
* scale number of grid points in each direction by the following factor
* $factor=1; $name = "pillBox.hdf";
* $factor=2; $name = "pillBox2.hdf";
* $factor=4; $name = "pillBox4.hdf";
*
* -- fourth-order accurate ---
* $factor=1; $name = "pillBox1.order4.hdf";  $orderOfAccuracy = "fourth order"; $interpolation="explicit";
* $factor=2; $name = "pillBox2.order4.hdf";  $orderOfAccuracy = "fourth order"; $interpolation="explicit";
* $factor=4; $name = "pillBox4.order4.hdf";  $orderOfAccuracy = "fourth order"; $interpolation="explicit";
* $factor=6; $name = "pillBox6.order4.hdf";  $orderOfAccuracy = "fourth order"; $interpolation="explicit";
$factor=8; $name = "pillBox8.order4.hdf";  $orderOfAccuracy = "fourth order"; $interpolation="explicit";
*
* Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); \
}
*
***************************************************************************
* target grid spacing:
$ds=.05/$factor;
* width of boundary grids
* $width=.5/$factor; # =.25; 
* width .6 needed for factor=8
$width=.6/$factor; # =.25; 
**
create mappings
  smoothedPolygon
    vertices
      3
      -1. 0.
      0. 0. 
      0. -1.
    n-dist
      fixed normal distance
      $dn=$width; 
      $dn
    lines
      $nx=int(2./$ds+1.5);  $ny=int($width/$ds+2.5);
      $nx $ny 
    boundary conditions
     1 2 3 0
    share
      1 0 3 0
    mappingName
    corner
    exit
*
  body of revolution
    tangent of line to revolve about
      1 0 0
    choose a point on the line to revolve about
      0 1 0
    start/end angle
      270 360
    lines
      $nz=int(3.14*2.*.25/$ds+1.5);
      $nx $ny $nz
*
    boundary conditions
      2 1 1 0 3 3
    share
      2 3 0 0 1 4
    mappingName
      cornerRevolution
    exit
*
*  here is the larger radius cylinder
*
  cylinder
    orientation
    1 2 0
    centre for cylinder
      0 1 0
    bounds on the radial variable
      $deltaRadius=$width; 
      $outerRadius=2.; $innerRadius=$outerRadius-$deltaRadius;
      $innerRadius $outerRadius
    bounds on theta
      .25 .5 
    bounds on the axial variable
      0. 1.
    boundary conditions
      3 3 0 2 0 1
    share
      1 4 0 5 0 3
    lines
      $nx=int(3.14*$outerRadius*.25/$ds+1.5); $ny=int(1./$ds+1.5); $nz=int($width/$ds+2.5);
      * 39 17 5
      $nx $ny $nz
    mappingName
      cylinder
    exit
*
  $delta=$ds*5.; # make the boxes a bit bigger for explicit 4th-order interp
*
  box
    set corners
      $xa=-1.; $xb=0.+.5*$delta; $ya=$dn-$delta; $yb=1.; $za=0.; $zb=1.-$dn+$delta;
      $xa $xb $ya $yb $za $zb
    boundary conditions
      2 0 0 3 3 0 
    share
      2 0 0 1 4 0
    lines
      $nx=int(($xb-$xa)/$ds+1.5); $ny=int(($yb-$ya)/$ds+1.5); $nz=int(($zb-$za)/$ds+1.5);
      $nx $ny $nz 
    mappingName
     leftBox
    exit
*
  box
    set corners
      $xa=-.5*$delta; $xb=1.; $ya=-1.+$deltaRadius-$delta; $yb=1.; $za=0.; $zb=$innerRadius+$delta;
      $xa $xb $ya $yb $za $zb
    boundary conditions
      0 2 0 3 3 0 
    share
      0 5 0 1 4 0
    lines
      $nx=int(($xb-$xa)/$ds+1.5); $ny=int(($yb-$ya)/$ds+1.5); $nz=int(($zb-$za)/$ds+1.5);
      $nx $ny $nz 
    mappingName
      rightBox
    exit
*
  exit this menu
generate an overlapping grid
  leftBox
  rightBox
  cylinder
  cornerRevolution
  done choosing mappings
*
  change parameters
    ghost points
      all
      2 2 2 2 2 2
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType = "$interpolation for all grids";
      $interpType
  exit
*  pause
  compute overlap
* pause
exit
*
save an overlapping grid
  $name
  pillBox
exit


  change the plot
    colour boundaries by share value