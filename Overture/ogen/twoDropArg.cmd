#
# Two drops in a channel
#
# usage: ogen [noplot] twoDropArg -factor=<num> -order=[2/4/6/8] -interp=[e/i] -ml=<> 
#
#   ogen -noplot twoDropArg -interp=i -factor=2
#   ogen -noplot twoDropArg -factor=4
#   ogen -noplot twoDropArg -factor=8
#   ogen -noplot twoDropArg -factor=16
#
$prefix="twoDrop";  $rgd="var";
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-2.; $xb=2.; $ya=-2.; $yb=2.; 
$cx=0.; $cy=0.;  # center for the annulus
$blf=1;  # this means no stretching
$deltaRadius0=.3; # radius for rgd fixed
$xa=-1.; $xb=1.; $ya=-6.; $yb=1.;
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix,\
            "cx=f"=>\$cx,"cy=f"=>\$cy,"rgd=s"=> \$rgd );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
if( $rgd eq "fixed" ){ $prefix = $prefix . "Fixed"; }
$suffix = ".order$order"; 
if( $blf ne 1 ){ $suffix .= ".s$blf"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
$pi = 4.*atan2(1.,1.);
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
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
  # 21 71 
  boundary conditions
    1 2 3 4
  mappingName
   channel
exit
#
Annulus
  $nr = intmg( 7 );
  $deltaRad=($nr-2)*$ds; 
  $innerRad=.3; $outerRad=$innerRad+$deltaRad;
  inner and outer radii
    $innerRad $outerRad
  lines
    $nTheta = intmg( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
    $nTheta $nr
  centre for annulus
    -.25 -.75
  boundary conditions
    -1 -1 5 0
  mappingName
   drop
exit
#
#
Annulus
  inner and outer radii
    $innerRad $outerRad
  centre for annulus
    .25 .25     -> trouble: .6 .25 
  lines
    $nTheta $nr
  boundary conditions
    -1 -1 5 0
  mappingName
   drop2
exit
#
exit
generate an overlapping grid
    channel
    drop
    drop2
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
  compute overlap
#  pause
  exit
#
save an overlapping grid
$name
twoDrop
exit

