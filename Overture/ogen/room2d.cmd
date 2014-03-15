*
* Grid for a 2d room : flow and heat transfer
*
* usage: ogen [noplot] superseismicGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<>
* 
* NOTE: watch out at inlet/outlet : the background grid may retain a short section of wall where it shouldn't
*       if there is not enough overlap
*
* examples:
*     ogen noplot room2d -factor=1 
*     ogen noplot room2d -factor=2 
*     ogen noplot room2d -factor=4 
*     ogen noplot room2d -factor=8 
*     ogen noplot room2d -factor=16 
*     ogen noplot room2d -factor=32 
*
#  - MG levels:
#     ogen noplot room2d -factor=4 -ml=1
#     ogen noplot room2d -factor=8 -ml=2
#     ogen noplot room2d -factor=16 -ml=2
#     ogen noplot room2d -factor=32 -ml=3
#
#  - order=4:
*     ogen noplot room2d -order=4 -factor=4
*     ogen noplot room2d -order=4 -factor=8
#
#     ogen noplot room2d -order=4 -factor=4 -ml=2
#     ogen noplot room2d -order=4 -factor=8 -ml=2
#     ogen noplot room2d -order=4 -factor=16 -ml=3
*
$order=2; $factor=1; $interp="e";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-1.; $xb=1.; $ya=-1.; $yb=1.;
* 
* get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = "room2d" . "$interp$factor" . $suffix . ".hdf";}
* 
$ds=.1/$factor;
* 
$dw = $order+1; $iw=$order+1; 
*
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#
create mappings
#
  # nr = number of lines in normal directions to boundaries
  $nr = max( 7+ $ng + 2*($order-2), 2**($ml+2) );
  $nr = intmg( $nr );
#
# -- background grid for room
#
rectangle
  set corners
    $xa=0.; $xb=6.; $ya=0.; $yb=3.; 
    $xa $xb $ya $yb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 3 4 
  share 
    1 2 3 4 
  mappingName
    backGround
 exit
#
# -- ceiling partition 
#
 $width=.3; 
 $depth=1.5;
 smoothedPolygon
  vertices
    $x0=3.;          $y0=$yb;
    $x1=$x0;         $y1=$y0-$depth; 
    $x2=$x1+$width;  $y2=$y1;
    $x3=$x2;         $y3=$yb; 
    4
     $x0 $y0
     $x1 $y1
     $x2 $y2
     $x3 $y3
  sharpness
    60
    60
    60
    60
  t-stretch
    0 40
    .15 30
    .15 40
    0 40
  n-stretch
    .5 4. 0
  n-dist
    fixed normal distance
     $nDist = ($nr-3)*$ds; 
      -$nDist
   lines
      $length = $depth*2 + $width; 
      $ns = intmg( $length/$ds + 1.5 );
      $ns $nr
    boundary conditions
      4 4 1 0
    share
      4 4 0 0
    mappingName
      ceilingPartition
 exit
#
#  -- ceiling inlet
#
  $height=.25;  $width=.5; 
  $xi=.5; 
rectangle
  set corners
    $xai=$xi-$ds; $xbi=$xi+$width+$ds; $yai=$yb-($order+1)*$ds; $ybi=$yb+$height; 
    $xai $xbi $yai $ybi
  lines
    $nx = intmg( ($xbi-$xai)/$ds +1.5 ); 
    $ny = intmg( ($ybi-$yai)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 0 0 5 
  share 
    0 0 0 5
  mappingName
    inletBackGround
 exit
#
# -- inlet left side
#
 smoothedPolygon
  vertices
    $x0=$xi-.25;    $y0=$yb;
    $x1=$xi;        $y1=$yb;
    $x2=$xi;        $y2=$ybi;
    3
     $x0 $y0
     $x1 $y1
     $x2 $y2
  sharpness
    20
    20
    20
  t-stretch
    0 40
    .15 20
    0 40
  n-stretch
    .5 4. 0
  n-dist
    fixed normal distance
     $nDist = ($nr-3)*$ds; 
      -$nDist
   lines
      $length = $height*2.;
      $ns = intmg( 1.25*$length/$ds + 1.5 );
      $ns $nr
    boundary conditions
      0 5 1 0
    share
      0 5 4 0 
    mappingName
      inletLeftSide
 exit
#
# -- inlet right side
#
 smoothedPolygon
  vertices
    $x0=$xi+$width;     $y0=$ybi;
    $x1=$xi+$width;     $y1=$yb;
    $x2=$xi+$width+.25; $y2=$yb;
    3
     $x0 $y0
     $x1 $y1
     $x2 $y2
  sharpness
    20
    20
    20
  t-stretch
    0 40
    .15 20
    0 40
  n-stretch
    .5 4. 0
  n-dist
    fixed normal distance
     $nDist = ($nr-3)*$ds; 
      -$nDist
   lines
      $length = $height*2.;
      $ns = intmg( 1.25*$length/$ds + 1.5 );
      $ns $nr
    boundary conditions
      5 0 1 0
    share
      5 0 4 0 
    mappingName
      inletRightSide
 exit
#
#  -- ceiling outlet
#
  $height=.25;  $width=.5; 
  $xi=4.; 
rectangle
  set corners
    $xai=$xi-$ds; $xbi=$xi+$width+$ds; $yai=$yb-($order+1)*$ds; $ybi=$yb+$height; 
    $xai $xbi $yai $ybi
  lines
    $nx = intmg( ($xbi-$xai)/$ds +1.5 ); 
    $ny = intmg( ($ybi-$yai)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 0 0 6
  share 
    0 0 0 6
  mappingName
    outletBackGround
 exit
#
# -- outlet left side
#
 smoothedPolygon
  vertices
    $x0=$xi-.25;    $y0=$yb;
    $x1=$xi;        $y1=$yb;
    $x2=$xi;        $y2=$ybi;
    3
     $x0 $y0
     $x1 $y1
     $x2 $y2
  sharpness
    20
    20
    20
  t-stretch
    0 40
    .15 20
    0 40
  n-stretch
    .5 4. 0
  n-dist
    fixed normal distance
     $nDist = ($nr-3)*$ds; 
      -$nDist
   lines
      $length = $height*2.;
      $ns = intmg( 1.25*$length/$ds + 1.5 );
      $ns $nr
    boundary conditions
      0 6 1 0
    share
      0 6 4 0 
    mappingName
      outletLeftSide
 exit
#
# -- outlet right side
#
 smoothedPolygon
  vertices
    $x0=$xi+$width;     $y0=$ybi;
    $x1=$xi+$width;     $y1=$yb;
    $x2=$xi+$width+.25; $y2=$yb;
    3
     $x0 $y0
     $x1 $y1
     $x2 $y2
  sharpness
    20
    20
    20
  t-stretch
    0 40
    .15 20
    0 40
  n-stretch
    .5 4. 0
  n-dist
    fixed normal distance
     $nDist = ($nr-3)*$ds; 
      -$nDist
   lines
      $length = $height*2.;
      $ns = intmg( 1.25*$length/$ds + 1.5 );
      $ns $nr
    boundary conditions
      6 0 1 0
    share
      6 0 4 0 
    mappingName
      outletRightSide
 exit
#
#  Table
#
 $xat=.25; $yat=.75; $width=1.5; $depth=.1;  # (xat,yat) = lower left edge of the table
 smoothedPolygon
  vertices
    $xct=$xat+$width*.5; # center of table
    $x0=$xct;         $y0=$yat;
    $x1=$xat+$width;  $y1=$yat;
    $x2=$xat+$width;  $y2=$yat+$depth; 
    $x3=$xat;         $y3=$yat+$depth; 
    $x4=$xat;         $y4=$yat;
    $x5=$x0;          $y5=$y0;   
    6
     $x0 $y0
     $x1 $y1
     $x2 $y2
     $x3 $y3
     $x4 $y4
     $x5 $y5
  sharpness
    30
    60
    60
    60
    60
    30
  t-stretch
    0. 10
    .15 10
    .15 10
    .15 10
    .15 10
    0. 10.
  n-stretch
    .5 4. 0
  n-dist
    fixed normal distance
     $nDist = ($nr-3)*$ds; 
      -$nDist
   lines
      $length = ($width+$depth)*2.; 
      $ns = intmg( 1.25*$length/$ds + 1.5 );
      $ns $nr
    boundary conditions
      -1 -1 7 0
    share
      0 0 0 0 
    mappingName
      tableTop
 exit
#
# -- computer
#
 $xac=1.25;  $width=.2;  $height=.5; 
 smoothedPolygon
  vertices
    $x0=$xac;        $y0=$ya;
    $x1=$xac;        $y1=$ya+$height; 
    $x2=$xac+$width; $y2=$y1;
    $x3=$x2;         $y3=$ya; 
    4
     $x0 $y0
     $x1 $y1
     $x2 $y2
     $x3 $y3
  sharpness
    40
    40
    40
    40
  t-stretch
    0 40
    .15 30
    .15 40
    0 40
  n-stretch
    .5 4. 0
  n-dist
    fixed normal distance
     $nDist = ($nr-3)*$ds; 
      $nDist
   lines
      $length = 2*$height + $width;
      $ns = intmg( $length/$ds + 1.5 );
      $ns $nr
    boundary conditions
      3 3 8 0 
    share
      3 3 0 0
    mappingName
      computer
 exit
#
# Make the overlapping grid
#
exit
generate an overlapping grid
    backGround
    ceilingPartition
#
    inletBackGround
    inletLeftSide
    inletRightSide
#
    outletBackGround
    outletLeftSide
    outletRightSide
#
    tableTop
    computer
  done
  change parameters
    * prevent background from cutting holes in the ceiling inlet or putlet
    prevent hole cutting
      backGround
        inletBackGround
      backGround
        inletLeftSide
      backGround
        inletRightSide
      backGround
        outletBackGround
      backGround
        outletLeftSide
      backGround
        outletRightSide
    done
    * choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
  exit
*  display intermediate results
**plot
  compute overlap
**  display computed geometry
  exit
*
* save an overlapping grid
save a grid (compressed)
$name
room2d
exit