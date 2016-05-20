#
# Grid for two rectangular bodies in a box
# 
#
#    y=yb ---------------------------------------
#         |                                      |
#         |                                      |
#         |           -------------              | 
#         |           |           |              |
#         |           |     + (cx1,cy1)          |
#         |           |   angle1  |  height1     |  
#         |           -------------              | 
#         |              width1                  |
#         |                                      |
#         |                                      |
#         |           -------------              | 
#         |           |           |              |
#         |           |     + (cx2,cy2)          |
#         |           |   angle2  |  height2     |  
#         |           -------------              | 
#         |              width2                  |
#         |                                      |
#         |                                      |
#         |                                      |
#     ya  ---------------------------------------
#        xa                                     xb
#                         
#
# Examples:
#    ogen -noplot twoFallingBodiesGrid -interp=e -order=2 -factor=2
#    ogen -noplot twoFallingBodiesGrid -interp=e -order=2 -factor=4
#    ogen -noplot twoFallingBodiesGrid -interp=e -order=2 -factor=8
#
#    ogen -noplot twoFallingBodiesGrid -interp=i -order=2 -factor=4
#    
# 
$prefix="twoFallingBodiesGrid";  
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$xa =-1.5; $xb=1.5; $ya=-2.; $yb=2; 
$name=""; 
$width1=1.; $height1=.4; 
$cx1=.0; $cy1= .35; $angle1=15; # center and angle for body 1
$width2=1.; $height2=.4; 
$cx2=.0; $cy2=-.35; $angle2=15; # center and angle for body 2
#
$blf=4;  # grid lines are this much finer near the boundary
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "width1=f"=>\$width1,"height1=f"=>\$height1,"cx1=f"=>\$cx1,"cy1=f"=>\$cy1,"angle1=f"=>\$angle1,\
            "width2=f"=>\$width2,"height2=f"=>\$height2,"cx2=f"=>\$cx2,"cy2=f"=>\$cy2,"angle2=f"=>\$angle2,\
            "rgd=s"=> \$rgd );
#
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
# 
$ds=.1/$factor;
$pi = 4.*atan2(1.,1.);
#
create mappings
#
rectangle
  set corners
    $yac=$ya;
    $xa $xb $yac $yb 
  lines
    $nx = intmg( ($xb-$xa )/$ds+1.5);  
    $ny = intmg( ($yb-$yac)/$ds+1.5);
    $nx $ny 
  boundary conditions
     1 2 3 4 
  share
    0 0 0 0
  mappingName
   channel
exit
#
#  ---------------- rectangular body 1 --------------------
#
$xba=$cx1-.5*$width1; $xbb=$xba+$width1; $ybb=$cy1+.5*$height1; $yba=$ybb-$height1; # corners of embedded body
$nr = 7+$order;
# $nr = 12+$order;
SmoothedPolygon
  # start on a side so that the polygon is symmetric
  vertices 
    $xbm=.5*($xba+$xbb); # mid-point on horizontal face
    $ybm=.5*($yba+$ybb); # mid-point on vertical face
    6
    # --- start curve on bottom in middle if a wide body ---
    $xbm   $yba
    $xbb   $yba
    $xbb   $ybb
    $xba   $ybb
    $xba   $yba
    $xbm   $yba
  n-stretch
   $nStretch=$blf; 
   1. $nStretch 0.
  n-dist
    fixed normal distance
    $nDist = ($nr-4)*$ds; 
    -$nDist
  periodicity
    2
  lines
    $stretchFactor=1.7; # add more lines in the tangential direction due to stretching at corners
    $length=2*( $xbb-$xba + $ybb-$yba ); # perimeter length 
    $nTheta = int( $stretchFactor*$length/$ds +1.5 ); 
    $nTheta $nr
  t-stretch
    0. 1.
    .2   $tStretch
    .2   $tStretch
    .2   $tStretch
    .2   $tStretch
    0. 1.
  # set sharpness of corners
  sharpness
    $sharp
    $sharp
    $sharp
    $sharp
    $sharp
    $sharp
  boundary conditions
    -1 -1 7 0
  share 
     0  0 0 0
  mappingName
    rectangularBodyUnRotated1
  exit
#
  rotate/scale/shift
    transform which mapping?
     rectangularBodyUnRotated1
    rotate
     $angle1
     $cx1 $cy1 0
    mappingName
     fallingBody1
  exit
#
#  ---------------- rectangular body 2 --------------------
#
$xba=$cx2-.5*$width2; $xbb=$xba+$width2; $ybb=$cy2+.5*$height2; $yba=$ybb-$height2; # corners of embedded body
$nr = 7+$order;
# $nr = 12+$order;
SmoothedPolygon
  # start on a side so that the polygon is symmetric
  vertices 
    $xbm=.5*($xba+$xbb); # mid-point on horizontal face
    $ybm=.5*($yba+$ybb); # mid-point on vertical face
    6
    # --- start curve on bottom in middle if a wide body ---
    $xbm   $yba
    $xbb   $yba
    $xbb   $ybb
    $xba   $ybb
    $xba   $yba
    $xbm   $yba
  n-stretch
   $nStretch=$blf; 
   1. $nStretch 0.
  n-dist
    fixed normal distance
    $nDist = ($nr-4)*$ds; 
    -$nDist
  periodicity
    2
  lines
    $stretchFactor=1.7; # add more lines in the tangential direction due to stretching at corners
    $length=2*( $xbb-$xba + $ybb-$yba ); # perimeter length 
    $nTheta = int( $stretchFactor*$length/$ds +1.5 ); 
    $nTheta $nr
  t-stretch
    0. 1.
    .2   $tStretch
    .2   $tStretch
    .2   $tStretch
    .2   $tStretch
    0. 1.
  # set sharpness of corners
  sharpness
    $sharp
    $sharp
    $sharp
    $sharp
    $sharp
    $sharp
  boundary conditions
    -1 -1 7 0
  share 
     0  0 0 0
  mappingName
    rectangularBodyUnRotated2
  exit
#
  rotate/scale/shift
    transform which mapping?
     rectangularBodyUnRotated2
    rotate
     $angle2
     $cx2 $cy2 0
    mappingName
     fallingBody2
  exit
#
exit
generate an overlapping grid
    channel
    fallingBody1
    fallingBody2
  done
  change parameters
 # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ngp = $ng+1;
      $ng $ng $ng $ngp $ng $ng
  exit
  #  display intermediate results
  # open graphics
  compute overlap
#  pause
  exit
#
save an overlapping grid
$name
twoFallingBodiesGrid
exit

