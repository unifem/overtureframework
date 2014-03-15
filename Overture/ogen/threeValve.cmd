*
*  three valves
* 
* usage: ogen [noplot] threeValve -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot threeValve -factor=1 
*     ogen noplot threeValve -factor=2 
*     ogen noplot threeValve -factor=4 
*     ogen noplot threeValve -factor=8 
*
*  Boundary conditions:
*     Walls:                  1
*     cylinder inletOutlet:   2 
*     Valve1 inlet/oulet:     3
*     Valve2 inlet/oulet:     4
*     Valve3 inlet/oulet:     5
*
****
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "threeValve" . "$interp$factor" . $suffix . ".hdf";}
* 
$ds=(1./15.)/$factor;
$nr=9; # point in the radial direction for boundary fitted grids
$rDist=($nr-4)*$ds;   # radial distance for boundary fitted grids
*
* 
* Shift for the left valve (valve3)  to close: shift3=-.425
$shift3 = -.425;
***************************************************************************
* scale number of grid points in each direction by the following factor
* $factor=1; $name = "threeValve.hdf";
* $factor=2; $name = "threeValve2.hdf"; 
* printf(" factor=$factor\n");
*
* Define a subroutine to convert the number of grid points
sub getGridPoints\
{ local($n1,$n2)=@_; \
  $nx=int(($n1-1)*$factor+1.5); $ny=int(($n2-1)*$factor+1.5); \
}
*
***************************************************************************
*
create mappings
*    --valve head--
  SmoothedPolygon
    mappingName
    valve-head
    vertices
    7
    -.1 .3
    -.7 .3
    -1.1 0.
    0. 0.
    1.1 0.
    .7 .3
    .1 .3
    n-dist
    fixed normal distance
     *  -.2   * keep fixed for now
     -$rDist 
    t-stretch
    .15 15.
    .15 20.
    .15 50.
    .0  1.
    .15 50.
    .15 20
    .15 15.
    lines
*
    * getGridPoints(91,9);
    $arcLength=6.; 
    $nx = int( 6./$ds + 1.5 );
    $nx $nr 
*
*
    n-stretch
    1. 6. 0.
    sharpness
    30.
    30.
    30
    30.
    30
    30.
    30.
    boundary conditions
      1 1 1 0
    share
     1 1 2 0
    exit
  *
  * ------- --------
  rectangle
    mappingName
    unstretched-valve-shaft-left
*-    specify corners
*-      -.5 .4 -.1 1.5
*-    lines
*-*
*-    getGridPoints(9,21);
*-      $nx $ny 
    * warning - do not make too wide or will cut holes
    * in the valve-head
    $xa=-.1-$rDist; $xb=-.1; $ya=.2; $yb=1.5; 
    set corners
      $xa $xb $ya $yb
    lines
      $nx = int( ($xb-$xa)/$ds +1.5 ); 
      $ny = int( ($yb-$ya)/$ds +1.5 ); 
      $nx $ny
*
    boundary conditions
    0 1 0 0
    share
    0 1 0 0
    exit
  stretch coordinates
    mappingName
      valve-shaft-left
    stretch
    specify stretching along axis=0
      layers
      1
      1. 5. 1.
     exit
    exit
  exit
  *
  * --- ---
  rectangle
    mappingName
    unstretched-valve-shaft-right
*-    specify corners
*-      .1 .4 .5  1.5
*-    lines
*-*
*-    getGridPoints(9,21);
*-      $nx $ny 
    * warning - do not make too wide or will cut holes
    * in the valve-head
    $xa=.1; $xb=.1+$rDist; $ya=.2; $yb=1.5; 
    set corners
      $xa $xb $ya $yb
    lines
      $nx = int( ($xb-$xa)/$ds +1.5 ); 
      $ny = int( ($yb-$ya)/$ds +1.5 ); 
      $nx $ny
*
    boundary conditions
    1 0 0 0
    share
    1 0 0 0
    exit
  stretch coordinates
    mappingName
      valve-shaft-right
    stretch
    specify stretching along axis=0
      layers
      1
      1.  5. 0.
     exit
    exit
  exit
*
  SmoothedPolygon
    mappingName
      valve-seat-left
    vertices
      4
      -.7 1.5
      -.7 .8
      -1.1 .5
      -1.5 .5
    n-dist
    fixed normal distance
      *  .375
      $rDist
    t-stretch
      0. 50
      .15 30.
      .15 30.
      0. 50.
    n-stretch
      1.  5. 0.
    lines
*
    * getGridPoints(41,11);
     $nx = int( 2.7/$ds + 1.5 ); 
      $nx $nr 
*
    boundary conditions
      3 0 1 0    * 3 = valve1 inlet
    share
      3 0 4 0
    exit
*
  SmoothedPolygon
    mappingName
      valve-seat-right
    vertices
      4
       .7 1.5
       .7 .8
       1.1 .5
       1.5 .5
    n-dist
    fixed normal distance
      * -.375
      -$rDist
    t-stretch
      0. 50
      .15 30.
      .15 30.
      0. 50.
    n-stretch
      1.  5. 0.
    lines
*
    * getGridPoints(41,11);
    *   $nx $ny 
      $nx $nr 
*
    boundary conditions
      3 0 1 0   * 3 = valve1 inlet
    share
      3 0 4 0
    exit
*
  rectangle
*-     specify corners
*-       -.5 1.2 -.1 1.5
*-     lines
*- *
*-     getGridPoints(9,7);
*-       $nx $ny 
*- *      
   $xa=-.7+$rDist-$ds; $xb=-.1; $ya=1.; $yb=1.5; 
   set corners
     $xa $xb $ya $yb
   lines
     $nx = int( ($xb-$xa)/$ds +1.5 ); 
     $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
*  
    mappingName
      unstretched-inlet-left
    boundary conditions
      0 1 0 3    * 3 = valve1 inlet
    share
      0 1 0 3
    exit
  stretch coordinates
    mappingName
      inlet-left
    stretch
    specify stretching along axis=0
      layers
      1
      1. 5. 1.
     exit
    exit
  exit
*
  rectangle
*-     specify corners
*-        .1 1.2  .5 1.5
*-     lines
*- *
*-     getGridPoints(9,7);
*-       $nx $ny 
*- * 
   $xa=.1; $xb=.7-$rDist+$ds; $ya=1.; $yb=1.5; 
   set corners
     $xa $xb $ya $yb
   lines
     $nx = int( ($xb-$xa)/$ds +1.5 ); 
     $ny = int( ($yb-$ya)/$ds +1.5 ); 
     $nx $ny
* 
    mappingName
      unstretched-inlet-right
    boundary conditions
      1 0 0 3     * 3 = valve1 inlet
    share
      1 0 0 3
    exit
  stretch coordinates
    mappingName
      inlet-right
    stretch
    specify stretching along axis=0
      layers
      1
      1. 5. 0.
     exit
    exit
  exit
*
*  -------------- upper left corner ---------
* 
  rectangle
    mappingName
      upperLeftCorner 
    $xa=-2.; $xb=-1.5; $ya=0.; $yb=.5; 
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
*   
    boundary conditions
     1 0 0 1   
    share
      6 0 0 3
    exit
*  -------------- upper right corner ---------
* 
  rectangle
    mappingName
      upperRightCorner 
    $xa=1.5; $xb=2.0; $ya=0.; $yb=.5; 
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
*   
    boundary conditions
     0 1 0 1   
    share
      0 5 0 3
    exit
*  -------------- lower left corner ---------
* 
  rectangle
    mappingName
      lowerLeftCorner 
    $xa=-2.; $xb=-1.5; $ya=-3.5; $yb=-3.; 
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
*   
    boundary conditions
     1 0 2 0   
    share
      6 0 7 0 
    exit
*
*  -------------- lower right corner ---------
* 
  rectangle
    mappingName
      lowerRightCorner 
    $xa=1.5; $xb=2.0; $ya=-3.5; $yb=-3.; 
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
*   
    boundary conditions
     0 1 2 0 
    share
      0 5 7 0 
    exit
*
*  -------------- main cylinder grid ---------
* 
  rectangle
    mappingName
      cylinder
  $xa=-2.5; $xb=2.5; $ya=-3.5; $yb=1.; 
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
*   
    boundary conditions
     *  1 1 2 1     * cylinder inlet=2
     0 0 2 0 
    share
      0 0 7 0 
    exit
*
  stretch coordinates
    mappingName
      inlet-left
    stretch
    specify stretching along axis=0
      layers
      1
      1. 5. 1.
     exit
    exit
  exit
*
*  Now make a second valve
*
  rotate/scale/shift
    transform which mapping?
    valve-shaft-left
    rotate
      -90.
      0. -1.5
    mappingName
      valve-shaft-left2
    exit
*
  rotate/scale/shift
    transform which mapping?
    valve-shaft-right
    rotate
      -90.
      0. -1.5
    mappingName
      valve-shaft-right2
    exit
*
  rotate/scale/shift
    transform which mapping?
    inlet-left
    rotate
      -90.
      0. -1.5
    mappingName
      inlet-left2
    exit
*
  rotate/scale/shift
    transform which mapping?
    inlet-right
    rotate
      -90.
      0. -1.5
    mappingName
      inlet-right2
    exit
*
  rotate/scale/shift
    transform which mapping?
    valve-seat-left
    rotate
      -90.
      0. -1.5
    share
      5 0 4 0
    mappingName
      valve-seat-left2
    exit
*
  rotate/scale/shift
    transform which mapping?
    valve-seat-right
    rotate
      -90.
      0. -1.5
    share
      5 0 4 0
    mappingName
      valve-seat-right2
    exit
*
  rotate/scale/shift
    transform which mapping?
    valve-head
    rotate
      -90.
      0. -1.5
    mappingName
      valve-head2
    exit
*
*
*  Now make a third valve
*
  rotate/scale/shift
    transform which mapping?
    valve-shaft-left
    rotate
      90.
      0. -1.5
    shift
      $shift3
    mappingName
      valve-shaft-left3
    exit
*
  rotate/scale/shift
    transform which mapping?
    valve-shaft-right
    rotate
      90.
      0. -1.5
    shift
      $shift3
    mappingName
      valve-shaft-right3
    exit
*
  rotate/scale/shift
    transform which mapping?
    inlet-left
    rotate
      90.
      0. -1.5
    mappingName
      inlet-left3
    exit
*
  rotate/scale/shift
    transform which mapping?
    inlet-right
    rotate
      90.
      0. -1.5
    mappingName
      inlet-right3
    exit
*
  rotate/scale/shift
    transform which mapping?
    valve-seat-left
    rotate
      90.
      0. -1.5
    share
      6 0 4 0
    mappingName
      valve-seat-left3
    exit
*
  rotate/scale/shift
    transform which mapping?
    valve-seat-right
    rotate
      90.
      0. -1.5
    share
      6 0 4 0
    mappingName
      valve-seat-right3
    exit
*
  rotate/scale/shift
    transform which mapping?
    valve-head
    rotate
      90.
      0. -1.5
    shift
      $shift3
    mappingName
      valve-head3
    exit
*
  exit this menu
*
  generate an overlapping grid
    cylinder
    upperLeftCorner
    upperRightCorner
    lowerLeftCorner
    lowerRightCorner
    valve-shaft-left
    valve-shaft-right
    inlet-left
    inlet-right
    valve-seat-left
    valve-seat-right
    valve-head
*
    valve-shaft-left2
    valve-shaft-right2
    inlet-left2
    inlet-right2
    valve-seat-left2
    valve-seat-right2
    valve-head2
*
    valve-shaft-left3
    valve-shaft-right3
    inlet-left3
    inlet-right3
    valve-seat-left3
    valve-seat-right3
    valve-head3
    done
    change parameters
    *-   prevent hole cutting
    *-     cylinder
    *-       all
    *-     done
      ghost points
        all
        2 2 2 2 2 2
    exit
*      display intermediate results
**   pause
    * open graphics
    compute overlap
**    pause
  exit
*
save a grid (compressed)
$name
threeValve
exit


