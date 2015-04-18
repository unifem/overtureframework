#
# Grid for a 2D deforming eye,
#
# Usage: 
#     ogen [-noplot] deformingEyeGrid -factor=<num> -order=[2/4/6/8] -interp=[e/i]  -radX=<> -radY=<> -ml=<>  
# 
#  -radX, -radY : radii of the ellipse in the x and y directions
#  -ml = number of (extra) multigrid levels to support
# 
# Examples:
# 
#     ogen -noplot deformingEyeGrid -interp=e -factor=4
# Thin eye: 
#     ogen -noplot deformingEyeGrid.cmd -radY=.2 -factor=32
#     ogen -noplot deformingEyeGrid.cmd -radY=.15 -factor=32
# 
$prefix="deformingEyeGrid";  $rgd="var"; $angle=0.; $branch=0; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; 
$radX=1.; $radY=.5; # radii of the ellipse in the x and y directions
$cx=0.; $cy=0.;  # center for the ellipse
$blf=1;  # this means no stretching
$deltaRadius0=.3; # radius for rgd fixed
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd,"radX=f"=>\$radX,"radY=f"=>\$radY,"angle=f"=>\$angle,\
            "branch=i"=>\$branch );
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
  # Make background grid square to allow eye to open to a circle 
  $xa=-$radX; $xb=$radX; $ya=$xa; $yb=$xb;
  set corners
    $xa $xb $ya $yb
  lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
  boundary conditions
    0 0 0 0 
  mappingName
    backGround
# pause
exit
# ---
 $nr = intmg( 7 );
 $nDist = ($nr-3)*$ds;
 # Arclength: use Ramanujan's formula:
 $a = $radX+.5*$nDist; $b=$radY+.5*$nDist;
 $arcLength = $pi*( 3.*($a+$b)-sqrt( 10.*$a*$b + 3.*( $a*$a+$b*$b) ) );
 $nTheta = intmg( $arcLength/$ds + 1.5 );
#
# --- make the ellipse as a Nurbs
#
$npts=$nTheta; 
$nurbsDegree=5; # degree of nurbs
$pts="$npts $nurbsDegree";
for( $i=0; $i<$npts; $i++ ){ $theta=2.*$pi*$i/($npts-1)-$pi*.5; $x=$radX*cos($theta); $y=$radY*sin($theta); $pts .= "\n $x $y"; }
nurbs
  set domain dimension
    1
  set range dimension
    2
  periodicity
    2
  enter points
    $pts
  # open graphics
 # We could scale the geometry here 
 #  scale
 #   $scale $scale $scale
 # -- make the actual domain a bit smaller so that ghost points will lie on
 #    the original surface.
 # restrict the domain
 #  $delta=.025;
 #  $ra=$delta; $rb=1.-$delta;
 #  $ra $rb 
 mappingName
  eyeBoundary
# pause
exit
# ----- hyperbolic grid -----
  hyperbolic
    distance to march $nDist
    lines to march $nr   
    points on initial curve $nTheta
    backward
    spacing: geometric
    geometric stretch factor 1.1
    # increase volume smooths if ellipse is thin to handle sharp ends
    volume smooths 200
# 
   generate
   # open graphics
# 
    boundary conditions
     -1 -1 4 0
    share
       0  0 100 0
    mappingName
      eyeGrid
#
    degree of nurbs $nurbsDegree
   ## evaluate as nurbs 1
# pause
  exit
#
exit
generate an overlapping grid
    backGround
    eyeGrid
  done
  change parameters
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
# pause
   compute overlap
   # open graphics
  exit
#
# save an overlapping grid
save a grid (compressed)
$name
cic
exit

