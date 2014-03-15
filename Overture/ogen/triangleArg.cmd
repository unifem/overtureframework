*
*   triangle in a channel (taking arguments)
*
*
* usage: ogen [noplot] triangleArg -factor=<num> -order=[2/4/6/8] -interp=[e/i]
* 
* examples:
*     ogen noplot triangleArg -factor=2 -order=2
*     ogen noplot triangleArg -factor=2 -order=4
*
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-1.; $xb=2.; $ya=-1.5; $yb=1.5; 
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
if( $name eq "" ){$name = "triangle" . "$interp$factor" . $suffix . ".hdf";}
* 
$ds=.05/$factor;
*
*
create mappings
  rectangle
    mappingName
      backGround
   set corners
    $xa $xb $ya $yb
   lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  exit
  SmoothedPolygon
    vertices
      5
       .866025 0.
       .866025 .5
      0. 0 .
       .866025 -.5
       .866025 0.
    boundary conditions
      -1 -1 1 0
    n-dist
    fixed normal distance
      $nr=9+$order;
      $nDist=-($nr-3)*$ds;
      $nDist
    n-stretch
      1. 5.  0
    t-stretch
      0. 0.
      .15 25
      .15 25
      .15 25
      .15 25
    lines
      $nTheta = int( 3./$ds + 1.5 );
      $nTheta $nr
    mappingName
      triangle
    exit
exit this menu
generate an overlapping grid
  backGround
  triangle
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
  * pause
  compute overlap
exit
*
save an overlapping grid
$name
triangle
exit
