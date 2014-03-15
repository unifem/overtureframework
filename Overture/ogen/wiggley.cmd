*
* Non-orthogonal grid for testing
*
*
* usage: ogen [noplot] wiggley -factor=<num> -order=[2/4/6/8] 
* 
* examples:
*     ogen -noplot wiggley -order=2 -factor=1
*     ogen -noplot wiggley -order=2 -factor=2
*     ogen -noplot wiggley -order=2 -factor=4
* 
*     ogen -noplot wiggley -order=4 -factor=1
*     ogen -noplot wiggley -order=4 -factor=2
*     ogen -noplot wiggley -order=4 -factor=4
*
$norder=9; # order of nurbs -- was 5
$order=2; $factor=1; $interp="e"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"interp=s"=> \$interp);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "wiggley" . "$factor" . $suffix . ".hdf";}
*
* $grid = "wiggley1.hdf"; $order="fourth order"; $factor=1;
* $grid = "wiggley2.hdf"; $order="fourth order"; $factor=2;
* $grid = "wiggley3.hdf"; $order="fourth order"; $factor=4;
* $grid = "wiggley4.hdf"; $order="fourth order"; $factor=8;
* $grid = "wiggley5.hdf"; $order="fourth order"; $factor=16;
*
*
create mappings
*
* We use Nurbs since cubic splines are not smooth enough for 4th order methods
*
  nurbs (curve)
    enter points
     4 $norder
      0. 0. 
      .33  .025
      .66 -.025
      1. 0.
*     3 5 
*       0. 0. 
*       0.5 .06 
*       1.00 -.1
*
    mappingName
      bottom
    lines
      17
*  pause
    exit
*
  nurbs (curve)
    enter points
     3 $norder
      0.  0. 
      -.1 .5
      0.  1.
*     4 5 
*         0. 0. 
*         0.0105411 0.295335 0 
*         0.11931 0.601485 0 
*        0.15 1. 
*
    lines
    17
    mappingName
      left
    exit
*
  nurbs (curve)
    enter points
     3 $norder
      0. 1. 
      .5 .975
      1. 1.
*     4 5 
*         0.15 1. 
*         .6   .95 
*        .7   1.05
*         1.2  1.1
*
    lines
      17
    mappingName
      top
    exit
*
  nurbs (curve)
    enter points
     4 $norder 
      1.      0. 
      1.025  .33
      .975   .66
      1.      1.
*    enter points
*     4 5 
*         1.00 -.1
*         1.0  .3 
*         1.1  .6
*         1.2  1.1
*
    lines
    17
    mappingName
      right
    exit
*
*
  tfi 
    choose bottom curve (r_2=0) 
      bottom 
    choose top curve    (r_2=1) 
      top 
    choose left curve   (r_1=0)
      left
    choose right curve  (r_1=1)
      right
    lines
      $nx = int($factor*16+1.5); $ny=$nx;
       $nx $ny
    boundary conditions
      1 2 3 4 
    mappingName
      wiggley
* pause
    exit
  exit this menu
*
*
generate an overlapping grid
  wiggley
  done choosing mappings
  change parameters
    * choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
  compute overlap
* pause
  exit
save a grid
$name
wiggley
exit


