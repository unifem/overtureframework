#
# Shapes in a 2d box
#
#
# usage: ogen [noplot] shapesArg -factor=<num> -order=[2/4/6/8] -interp=[e/i]
# 
# examples:
#     ogen noplot shapesArg -order=2 -factor=1 
#     ogen noplot shapesArg -order=2 -interp=e -factor=2
# 
$order=2; $factor=1; $interp="i"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-1.25; $xb=1.25; $ya=-1.; $yb=1.; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $name eq "" ){$name = "shapes" . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.05/$factor;
#
create mappings
  rectangle
    mappingName
      backGround
    set corners
     # -1.25 1.25 -1. 1. 
      $xa $xb $ya $yb
    lines
    $nx = int( ($xb-$xa)/$ds +1.5 ); 
    $ny = int( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  exit
#
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
      $nDist=5.*$ds; 
      # -.15
      -$nDist
    n-stretch
      1. 2.0 0
    t-stretch
      0. 0.
      .15 10
      .15 10
      .15 10
      .15 10
    lines
      $length=4.; 
      $ns=int( $length/$ds + 1.5 );
      $nr = int( $nDist/$ds+3.5 );
      # 77 9  75 7
      $ns $nr 
    mappingName
      triangle
    exit
#
#
# failed if square is moved right by .25
  SmoothedPolygon
    vertices
      5
       -1. -.25 
       -1. -.75
       -.25 -.75
       -.25 -.25
       -1.  -.25
    boundary conditions
      -1 -1 1 0
    n-dist
    fixed normal distance
     # -.15
      -$nDist
    n-stretch
      1. 2.0 0
    t-stretch
      .15 10.
      .15 10
      .15 10
      .15 10
      .15 10
    lines
      # 77 9  75 7
      $length=4.; 
      $ns=int( $length/$ds + 1.5 );
      $nr = int( $nDist/$ds+3.5 );
      $ns $nr 
    mappingName
      square
    exit
#
  Annulus
    $deltaRad=$nDist;
    $innerRad=.25; $outerRad=$innerRad+$deltaRad;
    inner and outer
     # .25 .5
     $innerRad $outerRad
    centre
      -.5 .35
    lines
     # 37 9 33 7
     $ns = int( 2.*3.14*($innerRad+$outerRad)*.5/$ds+1.5);
     $nr = int( $nDist/$ds+3.5 );
     $ns $nr
    boundary conditions
      -1 -1 1 0
    mappingName
      annulus
  exit
#
  exit this menu
#
 generate an overlapping grid
    backGround
    triangle
    annulus
    square
  change parameters
 # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
#  display intermediate results
#  plot
  compute overlap
  exit
#
save an overlapping grid
$name
shapes
exit

