#
# Grid for a 2D dropping body in a box
# 
#
#    y=yb ---------------------------------------
#         |                                     |
#         |                                     |
#         |           ------------              | 
#         |           |          |              |
#         |           |          |  height      |  -- cy (center of body)
#         |           ------------              | 
#         |              width                  |
#         |                                     |
#     ya  ---------------------------------------
#        xa                |                    xb
#                          cx
#
# Examples:
#     ogen -noplot fallingBodyGrid -factor=4
# 
$prefix="fallingBodyGrid";  
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$xa =-1.; $xb=1.; $ya=-1.; $yb=1.; $angle=0.; 
$name=""; 
$width=1.; $height=.5; 
$cx=0.; $cy=0.;  # center for the body
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd,"angle=f"=>\$angle );
#
$xba=-.5*$width; $xbb=$xba+$width; $ybb=-$depth; $yba=$ybb-$height; # corners of embedded body
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
    $xa $xb $ya $yb 
  lines
    $nx = intmg( ($xb-$xa)/$ds+1.5);  
    $ny = intmg( ($yb-$ya)/$ds+1.5);
    $nx $ny 
  boundary conditions
     1 2 3 4 
  mappingName
   channel
exit
#
#  -- rectangular body ---
#
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
   $nStretch=4.; 
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
    rectangularBodyUnRotated
  exit
#
  rotate/scale/shift
    transform which mapping?
     rectangularBodyUnRotated
    rotate
     $angle
     0 0 0
    mappingName
     fallingBody
  exit
#
exit
generate an overlapping grid
    channel
    fallingBody
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
fallingBodyGrid
exit

