#
#     Solid shape in a channel
#
#
# usage: ogen [-noplot] shapedDiskGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i] -shape=[ellipse|starFish|cross] ...
#                -ml=<>  -rgd=[fixed|var] -radX=<> -radY=<> -angle=<> -xa=<> -xb=<> -ya=<> -yb=<> -cx=<> -cy=<>
# 
#  -blf : boundary-layer-factor : blf>1 : make grid lines near boundary this many times smaller
#  -ml = number of (extra) multigrid levels to support
#  -rgd : var=variable : decrease radial grid distance as grids are refined. fixed=fix radial grid distance
#  -radX, -radY : radii of the ellipse in the x and y directions
#  -angle : angle of rotation (degrees)
#  -xa, -xb, -ya, -yb : bounds on the back ground grid
#  -cx, -cy : center for the ellipse
# 
# Examples:
# 
#     ogen -noplot shapedDiskGrid -shape=ellipse -prefix=ellipticalGrid -interp=e -factor=4
#     ogen -noplot shapedDiskGrid -shape=ellipse -prefix=ellipticalGrid -interp=e -factor=8
#
#     ogen -noplot shapedDiskGrid -shape=ellipse -prefix=ellipticalGrid -interp=e -order=4 -factor=8
# 
#  -- star-fish 
#     ogen -noplot shapedDiskGrid -shape=starFish -prefix=starFishGrid -interp=e -order=2 -factor=64
#     ogen -noplot shapedDiskGrid -shape=starFish -prefix=starFishGrid -interp=e -order=2 -factor=128
#
#  --- cross:
#   ogen -noplot shapedDiskGrid -shape=cross -prefix=crossGrid -interp=e -order=2 -factor=16
#   ogen -noplot shapedDiskGrid -shape=cross -prefix=crossGrid -interp=i -order=4 -factor=16
#
$shape="ellipse"; 
$prefix="shapedDiskGrid";  $rgd="var"; $angle=0.; $branch=0; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-1.25; $yb=1.25; 
$radX=.7; $radY=.35; # radii of the ellipse in the x and y directions
$cx=0.; $cy=0.;  # center for the ellipse
$blf=1;  # this means no stretching
$deltaRadius0=.3; # radius for rgd fixed
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd,"radX=f"=>\$radX,"radY=f"=>\$radY,"angle=f"=>\$angle,\
            "branch=i"=>\$branch,"prefix=s"=> \$prefix,"shape=s"=> \$shape );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
if( $branch ne 0 ){ $prefix = $prefix . "Branch"; }
if( $angle ne 0 ){ $prefix = $prefix . "Angle$angle"; }
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
rectangle
  set corners
    $xa $xb $ya $yb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    1 2 -1 -1 
  mappingName
    backGround
exit
# some default parameters: 
$numberOfVolumeSmooths=50;
#
#  -- The curve for the shape is defined in an include file ---
#
if( $shape eq "ellipse" ){ $cmd="include ellipseCurve.h"; }
if( $shape eq "cross" ){ $cmd="include crossCurve.h"; }
if( $shape eq "starFish" ){ $cmd="include starFishCurve.h"; }
$cmd
# 
# -- Make a hyperbolic grid --
#
  $nr = intmg( 7 + $order/2 );
  hyperbolic
    forward
    $nDist=($nr-4)*$ds;
    distance to march $nDist
    $nrm=$nr-1; 
    lines to march $nrm
    $nTheta = int($arcLength/$ds+1.5);
    points on initial curve $nTheta
    uniform dissipation 0.05
    volume smooths $numberOfVolumeSmooths
    equidistribution 0 (in [0,1])
    #
    spacing: geometric
    geometric stretch factor 1.05 
    #
    generate
    boundary conditions
      -1 -1 7 0 0 0
    share 
       0 0 100 0 0 0
    name outerShape0
  exit
# --- inner domain
  hyperbolic
    backward
    distance to march $nDist
    lines to march $nrm
    points on initial curve $nTheta
    uniform dissipation 0.05
    volume smooths $numberOfVolumeSmooths
    equidistribution 0. (in [0,1])
    #
    spacing: geometric
    geometric stretch factor 1.05 
    #
    generate
    boundary conditions
      -1 -1 7 0 0 0
    share 
       0 0 100 0 0 0
    name innerShape0
  exit
#
# ------- inner background grid -----
#
$xai=-$radX;  $xbi=-$xai; $yai=-$radY; $ybi=-$yai;  # FIX ME FOR ROTATIONS
rectangle
  set corners
    $xai $xbi $yai $ybi
  lines
    $nx = intmg( ($xbi-$xai)/$ds +1.5 ); 
    $ny = intmg( ($ybi-$yai)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 0 0 0 
  mappingName
    innerBackGround
exit
#
# Convert to a nurbs and rotate
#
$numGhost=$ng+1; 
sub convertToNurbs\
{ local($old,$new,$angle)=@_; \
  $commands = "nurbs (surface)\n" . \
              "interpolate from mapping with options\n" . "$old\n" . "parameterize by index (uniform)\n" . \
              " number of ghost points to include\n $numGhost\n" . \
              "done\n" . \
              "rotate\n" . "$angle 1\n" . "0 0 0\n" . \
              "mappingName\n" . "$new\n" . "exit\n"; \
}
convertToNurbs("outerShape0","outerShape",$angle);
convertToNurbs("innerShape0","innerShape",$angle);
$commands
#
#
exit
#
#  --- generate the overlapping grid ---
#
generate an overlapping grid
    backGround
    outerShape
    innerBackGround
    innerShape
  done
  change parameters
    specify a domain
      innerDomain
      innerBackGround
      innerShape
    done
    specify a domain
      outerDomain
      backGround
      outerShape
    done
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      # $ngp = $ng+1;
      $ngp = $ng;
      $ng $ng $ng $ngp $ng $ng
  exit
  #  display intermediate results
  # open graphics
  # 
  compute overlap
#  plot
#   query a point 
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
shapedDiskGrid
exit
