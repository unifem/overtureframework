#
# One or more 3D stirring sticks in a channel
#
#
# usage: ogen [noplot] stir3d -factor=<num> -order=[2/4/6/8] -interp=[e/i] -blf=<num> -ml=<>  -rgd=[fixed|var] ...
#                             -xa=<> -xb=<> -ya=<> -yb=<> -angle=[degrees] -numStir=[1|2] -tStretch=<>
# 
#  -numStir : number of sticks (1 or 2)
#  -blf : boundary-layer-factor : blf>1 : make grid lines near boundary this many times smaller
#  -ml = number of (extra) multigrid levels to support
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -angle : offset initial angles by this many degrees
#  -tStretch : tangential stretching at the corners of the stick
# 
# examples:
#     ogen -noplot stir3d -order=2 -blf=4 -interp=e -factor=4
#     ogen -noplot stir3d -order=2 -blf=4 -interp=e -factor=8
# 
#     ogen -noplot stir3d -order=2 -blf=2 -interp=e -factor=4
#     ogen -noplot stir3d -order=2 -blf=2 -interp=e -factor=8
#
#  -- one stick:
#     ogen -noplot stir3d -xa=-1.5 -xb=1.5 -ya=-1.5 -yb=1.5 -order=2 -blf=4 -interp=e -tStretch=30. -numStir=1 -ml=1 -factor=2
#     ogen -noplot stir3d -xa=-1.5 -xb=1.5 -ya=-1.5 -yb=1.5 -order=2 -blf=4 -interp=e -tStretch=30. -numStir=1 -ml=1 -factor=3
# 
#     ogen -noplot stir3d -xa=-1.5 -xb=1.5 -ya=-1.5 -yb=1.5 -order=4 -blf=4 -interp=e -tStretch=40. -numStir=1 -factor=2
#     ogen -noplot stir3d -xa=-1.5 -xb=1.5 -ya=-1.5 -yb=1.5 -order=4 -blf=4 -interp=e -tStretch=40. -numStir=1 -factor=4
# 
#   -- multigrid
#     ogen -noplot stir3d -order=2 -blf=4 -interp=e -ml=1 -factor=1
#     ogen -noplot stir3d -order=2 -blf=4 -interp=e -ml=1 -factor=2
#     ogen -noplot stir3d -order=2 -blf=4 -interp=e -ml=1 -factor=4 
#     ogen -noplot stir3d -order=2 -blf=4 -interp=e -ml=2 -factor=8 
#     ogen -noplot stir3d -order=2 -blf=4 -interp=e -ml=2 -factor=16
#     ogen -noplot stir3d -order=2 -blf=4 -interp=e -ml=3 -factor=32
#     ogen -noplot stir3d -order=2 -blf=4 -interp=e -ml=3 -factor=64  [3.6M]
#     ogen -noplot stir3d -order=2 -blf=4 -interp=e -ml=3 -factor=128 [15M] 
# 
#   -- order=4
#     ogen -noplot stir3d -order=4 -blf=4 -interp=e -factor=3
#     ogen -noplot stir3d -order=4 -blf=4 -interp=e -ml=1 -factor=4 
#
# Initial angle: 
#     ogen -noplot stir3d -order=2 -blf=4 -interp=e -ml=2 -factor=8 -angle=54
# 
# rotated single stick: NOTE" blf=2 does not work so well with cgcns -- poor quality at corners
#    ogen -noplot stir3d -xa=-1.5 -xb=1.5 -ya=-1.5 -yb=1.5 -order=2 -blf=4 -interp=e -tStretch=40. -numStir=1 -factor=2 -angle=45 -name=stirRotated45e2.order2.hdf
#    ogen -noplot stir3d -xa=-1.5 -xb=1.5 -ya=-1.5 -yb=1.5 -order=2 -blf=4 -interp=e -tStretch=40. -numStir=1 -factor=2 -angle=90 -name=stirRotated90e2.order2.hdf
#
#   -- long channel 
#     ogen noplot stir3d -order=2 -interp=e -xa=-2. -xb=10. -ya=-3.0 -yb=3.0 -ml=2 -factor=2  -blf=4 -name="stirLongChannele2.hdf"
#
$prefix="stir3d";  $rgd="var"; $angle=0.; 
$numStir=2;  # 2 stirring sticks by default
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-1.; $xb=2.; $ya=-2.; $yb=1.; $za=0.; $zb=.25; 
$cx=0.; $cy=0.;  # center for the annulus
$blf=4;  # boundary layer spacing is this many times smaller
$deltaRadius0=.3; # radius for rgd fixed
$tStretch=20.; # $tStretch=6.; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"za=f"=>\$za,"zb=f"=>\$zb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd,"angle=f"=>\$angle,"numStir=i"=>\$numStir,"tStretch=f"=> \$tStretch );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $numStir eq 1 ){ $prefix = $prefix . "1"; }
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
$suffix = ".order$order"; 
if( $blf ne 1 ){ $suffix .= ".s$blf"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
$pi = 4.*atan2(1.,1.);
# 
$dw = $order+1; $iw=$order+1; 
# parallel ghost lines: for ogen we need at least:
#       .5*( iw -1 )   : implicit interpolation 
#       .5*( iw+dw-2 ) : explicit interpolation
$parallelGhost=($iw-1)/2;
if( $interp eq "e" ){  $parallelGhost=($iw+$dw-2)/2; }
if( $parallelGhost<1 ){ $parallelGhost=1; } 
minimum number of distributed ghost lines
  $parallelGhost
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#
create mappings
#
#
# Here is the inner box
#
Box
  set corners
    $xa $xb $ya $yb $za $zb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5);
    $ny = intmg( ($yb-$ya)/$ds +1.5);
    $nz = intmg( ($zb-$za)/$ds +1.5);
    $nx $ny $nz
  boundary conditions
    1 2 3 4 5 6
  share 
    0 0 0 0 5 6 
  mappingName
    backGround
  exit
#
$nr = intmg( 9 );
#
SmoothedPolygon
* start on a side so that the polygon is symmetric
  vertices 
    6
    -.1   .00
    -.1   .5
     .1   .5
     .1  -.5
    -.1  -.5
    -.1   .00
  n-stretch
   1. 1.0 0.
  n-dist
    fixed normal distance
    $nDist = ($nr-1)*$ds; 
    $nDist
  periodicity
    2
  lines
    # $stretchFactor=1.4; # add more lines in the tangential direction due to stretching at corners
    $stretchFactor=1.0; # add more lines in the tangential direction due to stretching at corners
    $length=2.4; # perimeter length 
    $nTheta = intmg( $stretchFactor*$length/$ds +1.5 ); 
    $nTheta $nr
  t-stretch
    0. 1.
    .2   $tStretch
    .2   $tStretch
    .2   $tStretch
    .2   $tStretch
    0. 1.
  boundary conditions
    -1 -1 7 0
  share 
     0  0 0 0
  mappingName
    stir-noStretch
exit
#
# optionally stretch the grid lines next to the cylinder
# 
 stretch coordinates 
  transform which mapping? 
    stir-noStretch
  multigrid levels $ml
  # add extra resolution in the stretching direction: 
  # stretch resolution factor 2.
  # exponential to linear stretching: 
   Stretch r2:exp to linear
   STP:stretch r2 expl: position 0
   $dxMin = $ds/$blf; 
   STP:stretch r2 expl: min dx, max dx $dxMin $ds
  STRT:name stir1-unrotated
  # open graphics
 exit
#
# Apply an initial rotation
#
  rotate/scale/shift
    transform which mapping?
    stir1-unrotated
    rotate
      $angle
      0 0 0
    mappingName
      stir1-orig
    exit
  # -- extrude the stirring stick to make 3D 
  sweep 
    choose reference mapping
      stir1-orig
    extrude
      $za $zb
    boundary conditions
      -1 -1 7 0 1 1
    share 
       0  0 0 0 5 6 
    lines
      $nz = intmg( ($zb-$za)/$ds + 1.5 );
      -1 -1 $nz 
    mappingName
      stir1_3d
   exit
#
# Make a second stir 
#
  rotate/scale/shift
    transform which mapping?
      stir1_3d
    rotate
      $theta=$angle+90.;
      $theta
      0 0 0
    shift
      0 -1.
    mappingName
      stir2_3d
    exit
#
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands = "nurbs (surface)\n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . "done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
#
# Convert to nurbs for faster evaluation (I hope)
#
convertToNurbs("stir1_3d","stir1",0.);
$commands
convertToNurbs("stir2_3d","stir2",0.);
$commands
#
exit
generate an overlapping grid
    backGround
    if( $numStir eq 1 ){ $gridNames ="stir1"; }else{ $gridNames = "stir1\n stir2"; }
    $gridNames
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
#  debug 
#    7
# open graphics
#  compute overlap
#  continue
#  continue
#
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
cic
exit
