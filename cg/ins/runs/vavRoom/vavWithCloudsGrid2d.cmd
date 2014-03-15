#
# Grid for a 2d cross section of 3D VAV room : Purdue's VAV room
#
# usage: ogen [noplot] vavWithCloudsGrid2d -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<> -clouds=[0|1]
# 
# -clouds=1 :  add cloud grids
#
# NOTE: watch out at inlet/outlet : the background grid may retain a short section of wall where it shouldn't
#       if there is not enough overlap
#
# examples:
# 
# -- second-order
#  ogen -noplot vavWithCloudsGrid2d -interp=e -factor=1 -ml=1 
#  ogen -noplot vavWithCloudsGrid2d -interp=e -factor=2 -ml=1
#  ogen -noplot vavWithCloudsGrid2d -interp=e -factor=4 -ml=1
#  ogen -noplot vavWithCloudsGrid2d -interp=e -factor=8 -ml=2
#
# -- fourth-order
#  ogen -noplot vavWithCloudsGrid2d -interp=e -order=4 -factor=2 -ml=1 
#
# 
#
$order=2; $factor=1; $interp = "i";  $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; 
$name=""; $xa=-1.; $xb=1.; $ya=-1.; $yb=1.;		
$clouds=0; 
# 
$f2m = .3048; # feet to meters conversion (exact)
# $f2m = 1.; # work in units of feet
#
# -- Room dimensions:
# 
$xaRoom=0.; $xbRoom=32.*$f2m; $yaRoom=0.; $ybRoom=14.5*$f2m; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"clouds=i"=>\$clouds);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }else{ $interpType = "implicit for all grids"; }
# 
$prefix ="vavWithCloudsGrid2d";
if( $clouds eq 1 ){ $prefix .= "WithClouds"; }
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds0=.5*$f2m;
$ds=$ds0/$factor;
$dsNormal=$ds*.25; # make grid spacing in normal direction this amount
# 
$dw = $order+1; $iw=$order+1; 
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
sub min{ local($n,$m)=@_; if( $n<$m ){ return $n; }else{ return $m; } }
#
$Pi = 4.*atan2(1.,1.);
#
#
#
#
create mappings
#
$bcOutflow=12;   # outflow BC
#
#  Main room grid:
#
rectangle
  $xad=$xaRoom; $xbd=$xbRoom; $yad=$yaRoom; $ybd=$ybRoom;
  set corners
    $xad $xbd $yad $ybd
  lines
    $nx = intmg( ($xbd-$xad)/$ds +1.5 ); 
    $ny = intmg( ($ybd-$yad)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 3 4
  share
    1 2 3 4	 
  mappingName
    roomBackGround
  exit
#
#  Outlet grid (poor man's)
#
rectangle
  $nyOutlet = intmg( 9 ); # outlet grid extends this far into the room
#
#      # $xao=8.*$f2m; $xbo=12.*$f2m; $zao=8.*$f2m; $zbo=12.*$f2m; $yao=$ybRoom-$nyOutlet*$ds; $ybo=$ybRoom; 
#  $xao=12.5*$f2m; $xbo=16.*$f2m; $zao=11.*$f2m; $zbo=12.5*$f2m; $yao=$ybRoom-$nyOutlet*$ds; $ybo=$ybRoom; 
#
# -- For 2D
  $xao=$xbRoom-$nyOutlet*$ds; $xbo=$xbRoom; $yao=1.5*$f2m; $ybo=4.5*$f2m;
#
  set corners
    $xao $xbo $yao $ybo
  lines
    $nx = intmg( ($xbo-$xao)/$ds +1.5 ); 
    $ny = intmg( ($ybo-$yao)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 $bcOutflow 0 0 
  share
    0 4 0 0 
  mappingName
    outlet
  exit
  # clouds:
#
# Cloud grids 
#
$nr = intmg( 7 );
#
$cloudWidth=8.*$f2m; $cloudDepth=.5*$f2m;  
SmoothedPolygon
* start on a side so that the polygon is symmetric
  vertices 
    6
    $x0=0.;                $y0=0;
    $x1= $cloudWidth*.5; $y1=0.;
    $x2= $cloudWidth*.5; $y2=$cloudDepth;
    $x3=-$cloudWidth*.5; $y3=$cloudDepth;
    $x4=-$cloudWidth*.5; $y4=0.;
    $x5=0.;                $y5=0;
    $x0 $y0
    $x1 $y1
    $x2 $y2
    $x3 $y3
    $x4 $y4
    $x5 $y5
  n-stretch
   1. 1.0 0.
  n-dist
    fixed normal distance
    $nDist = -($nr-1)*$ds; 
    $nDist
  periodicity
    2
  lines
    # $stretchFactor=1.4; # add more lines in the tangential direction due to stretching at corners
    $stretchFactor=1.0; # add more lines in the tangential direction due to stretching at corners
    $length=2.4*($cloudWidth+$cloudDepth); # perimeter length 
    $nTheta = intmg( $stretchFactor*$length/$ds +1.5 ); 
    $nTheta $nr
  t-stretch
    0. 1.
    .2  15.
    .2  15.
    .2  15.
    .2  15.
    0. 1.
  boundary conditions
    -1 -1 7 0
  mappingName
    cloud-noStretch
exit
#
# optionally stretch the grid lines next to the surface
# 
 stretch coordinates 
  transform which mapping? 
    cloud-noStretch
  multigrid levels $ml
  # add extra resolution in the stretching direction: 
  # stretch resolution factor 2.
  # exponential to linear stretching: 
   Stretch r2:exp to linear
   STP:stretch r2 expl: position 0
   STP:stretch r2 expl: min dx, max dx $dsNormal $ds
  STRT:name cloud-stretched
  # open graphics
 exit
#
# Translate to the correct positions
#
  rotate/scale/shift
    transform which mapping?
    cloud-stretched
    shift 
      $xShift=$cloudWidth*.5 + 2.*$f2m; $yShift=10.*$f2m; 
      $xShift $yShift 0. 
    boundary conditions
      -1 -1 7 0
    share
       0  0 7 0    
    mappingName
      cloud1
    exit
#
  rotate/scale/shift
    transform which mapping?
    cloud-stretched
    shift 
      $xShift=$xShift + 10.*$f2m;
      $xShift $yShift 0. 
    boundary conditions
      -1 -1 8 0
    share
       0  0 8 0    
    mappingName
      cloud2
    exit
#
  rotate/scale/shift
    transform which mapping?
    cloud-stretched
    shift 
      $xShift=$xShift + 10.*$f2m;
      $xShift $yShift 0. 
    boundary conditions
      -1 -1 9 0
    share
       0  0 9 0    
    mappingName
      cloud3
    exit
#
#   -- diffuser grids
  reparameterize
    transform which mapping?
      cloud1
    set corners
      # The values [.05,.175]x[0,1] define the coordinates in the
      # unit square of a sub-region of the cloud where the diffuser sits:
      .05 .175 0. 1.
    boundary conditions
      -1 -1 10 0
    # lines: 21 x 17 for factor=4
    $nx = intmg( .8/$ds + 1.5);
    $ny = intmg( .6/$ds + 1.5);
    lines
      $nx $ny 
    mappingName
      diffuser1
    exit
#   -- diffuser grids
  reparameterize
    transform which mapping?
      cloud3
    set corners
     # .05 .175 0. 1.
     .825  .95  0. 1. 
    lines
      $nx $ny 
    boundary conditions
      -1 -1 11 0
    mappingName
      diffuser2
    exit
#
#
exit
#
#
# Make the overlapping grid
#
generate an overlapping grid
  roomBackGround
  outlet
  cloud1
  cloud2 
  cloud3
  diffuser1
  diffuser2
#
  done
  plot
  change parameters
 # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng 
  exit
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
vavWithCloudsGrid2d
exit

