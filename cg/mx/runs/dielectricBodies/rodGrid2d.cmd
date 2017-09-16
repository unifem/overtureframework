#
# Grid for a solid rod in a box
#
#
# usage: ogen [noplot] rodGrid2d -factor=<num> -order=[2/4/6/8] -interp=[e/i] -blf=<num> -ml=<>  -rgd=[fixed|var] ...
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
#                       periodic 
#               +-----------------------+
#               |                       |
#               |                       |
#               |        +----+         |
#               |        |    |         |
#               |        |    |         |
#               |        |    | height  |
#               |        |    |         |
#               |        |    |         |
#               |        +----+         |
#               |         width         |
#               |                       |
#               +-----------------------+
#                       periodic 
# 
# examples:
#     ogen -noplot rodGrid2d -order=2 -interp=e -factor=4
#     ogen -noplot rodGrid2d -order=2 -interp=e -factor=8 
#     ogen -noplot rodGrid2d -order=2 -interp=e -factor=16 
# order=4 
#     ogen -noplot rodGrid2d -order=4 -interp=e -factor=8 
#     ogen -noplot rodGrid2d -order=4 -interp=e -factor=16
#
#
$prefix="rodGrid2d";  $rgd="var"; $angle=0.; 
$height=1.5; $width=.4; 
$numStir=2;  # 2 stirring sticks by default
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.5; $xb=2.5; $ya=-1.5; $yb=1.5; 
$cx=0.; $cy=0.;  # center for the annulus
$deltaRadius0=.3; # radius for rgd fixed
$tStretch=1.; # stretch lines in tangential direction near corners
$sharp=30.; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml, "prefix=s"=> \$prefix,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd,"angle=f"=>\$angle,"numStir=i"=>\$numStir,\
            "height=f"=>\$height,"width=f"=>\$width,"tStretch=f"=> \$tStretch,"sharp=f"=>\$sharp );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $numStir eq 1 ){ $prefix = $prefix . "1"; }
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
$suffix = ".order$order"; 
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
# ------- outer background grid -----
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
#
$nr = intmg( 6 + $order );
$halfWidth=.5*$width;
$halfHeight=.5*$height; 
$stretchFactor=1.1; # add more lines in the tangential direction due to stretching at corners
$nstretch=1.5; 
#
#   --- outer grid on rod surface ----
SmoothedPolygon
  # start on a side so that the polygon is symmetric
  vertices 
    6
    -$halfWidth   .00
    -$halfWidth   $halfHeight
     $halfWidth   $halfHeight
     $halfWidth  -$halfHeight
    -$halfWidth  -$halfHeight
    -$halfWidth   .00
#
   curve or area (toggle)
  periodicity
    2
  lines
    $arcLength=2.*($height+$width); # perimeter length 
    $nTheta = intmg( $stretchFactor*$arcLength/$ds +1.5 ); 
    $nTheta $nr
  t-stretch
    0. 1.
    .1   $tStretch
    .1   $tStretch
    .1   $tStretch
    .1   $tStretch
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
    -1 -1 5 0
  share 
     0  0 100 0
  mappingName
    rodCurve
  # open graphics
exit
# 
# -- Make a hyperbolic grid --
#
  hyperbolic
    backward
    $nDist=($nr-5)*$ds;
    distance to march $nDist
    $nrm=$nr-1; 
    lines to march $nrm
    $nTheta = int($arcLength/$ds+1.5);
    points on initial curve $nTheta
    uniform dissipation 0.05
    $nvol=20*$factor; 
    volume smooths $nvol
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
    name outerRod0
  exit
# --- inner domain
  hyperbolic
    forward
    distance to march $nDist
    lines to march $nrm
    points on initial curve $nTheta
    uniform dissipation 0.05
    volume smooths $nvol
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
    name innerRod0
  exit
#
# ------- inner background grid -----
#
$xai=-$halfWidth;  $xbi=-$xai; $yai=-.5*$height; $ybi=-$yai;
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
#
# Convert to nurbs for faster evaluation (I hope)
#
$angle=0.; 
convertToNurbs("outerRod0","outerRod",$angle);
$commands
convertToNurbs("innerRod0","innerRod",$angle);
$commands
#
exit
#
#  --- generate the overlapping grid ---
#
generate an overlapping grid
    backGround
    outerRod
    innerBackGround
    innerRod
  done
  change parameters
    specify a domain
      innerDomain
      innerBackGround
      innerRod
    done
    specify a domain
      outerDomain
      backGround
      outerRod
    done
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
  #  open graphics
  compute overlap
#*  display computed geometry
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
rod
exit



