#
# Grid for a 2D deforming eye,
#
# Examples:
#    ogen -noplot realEyeGrid -order=2 -interp=e -factor=1 
#    ogen -noplot realEyeGrid -order=2 -interp=e -factor=2 
# 
#  -ml = number of (extra) multigrid levels to support
# 
# 
$prefix="realEyeGrid";  $rgd="var"; $angle=0.; $branch=0; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name="";
$blf=1;  # this means no stretching
$deltaRadius0=.3; # radius for rgd fixed
$xScale=100.; 
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
$ds=1./$factor/$xScale;
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
  $xa=-100/$xScale; $xb=115/$xScale; $ya=-35/$xScale; $yb=30/$xScale;
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
exit
#
#   - make the eye shape from text file  
#
$nr = intmg( 7 );
$nDist = ($nr-3)*$ds;
$nurbsDegree = 5;  # NOTE: this variable appears in the eye curve data file included below
nurbs
  set domain dimension
    1
  set range dimension
    2
  periodicity
    2
  enter points
   # the next file was created by eyeTest.C 
   include eyeCurveDataPoints.dat
 mappingName
  eyeBoundary
exit
# ----- hyperbolic grid -----
  hyperbolic
    backward
    distance to march $nDist
    lines to march $nr   
    $circumference=400./$xScale; # approximate circumference 
    $nTheta = int( $circumference/$ds );
    points on initial curve $nTheta
    ## warning: geometric stretching may affect order of accuracy if stretch factor is too large
    ## spacing: geometric
    ## geometric stretch factor 1.05
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
   $nurbsDegree=3;
   degree of nurbs $nurbsDegree
   evaluate as nurbs 1
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
  compute overlap
  change the plot
  plot branch cuts 1
  # pause
  exit
exit
# save an overlapping grid
save a grid (compressed)
$name
eyeGrid
exit
