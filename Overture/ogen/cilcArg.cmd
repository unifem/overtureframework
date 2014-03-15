#
# Circle in a long channel with a stretched grid 
# Usage:
#    ogen [-noplot] cilcArg -factor=<> -interp=[e|i] -order=[2|4|6|8] -blf=<>
#
# -blf : grid spacing at cylinder will be this many times finer, e.g. blf=5 
#
# Examples:
#   ogen -noplot cilcArg -interp=e -factor=1 
#   ogen -noplot cilcArg -interp=i -factor=2
#   ogen -noplot cilcArg -interp=e -factor=2
#   ogen -noplot cilcArg -interp=e -factor=4
#   ogen -noplot cilcArg -interp=e -factor=8
#
# -order=4:
#   ogen -noplot cilcArg -interp=e -order=4 -factor=1 
#   ogen -noplot cilcArg -interp=e -order=4 -factor=2
#   ogen -noplot cilcArg -interp=e -order=4 -factor=4
#
# default values: 
$order=2; $ng=2; $factor=1; $interp="i"; $ml=0; # default values
$xa=-2.5; $xb=7.5; $ya=-2.5; $yb=2.5; 
$blf=5.;  # grid spacing at cylinder will be this many times finer
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"blf=f"=> \$blf );
# 
if( $order eq 2 ){ $orderOfAccuracy="second order"; $ng=2; }\
elsif( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "i" ){ $interpType = "implicit for all grids"; }\
elsif( $interp eq "e" ){ $interpType = "explicit for all grids"; }
#
$prefix="cilc";
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
#
$ds = .1/$factor; 
$pi=4.*atan2(1.,1.);
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
#
#
create mappings
  rectangle
    set corners
    $xa $xb $ya $yb
   lines
    $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
    $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
    $nx $ny
    boundary conditions
      1 2 3 4 
    mappingName
     square
    exit
  Annulus
    $nr = 9 + 2*($order-2); # fixed number of lines in the radial direction 
    $innerRad=.5; $outerRad=$innerRad + ($nr-2)*$ds;
    inner radius
      $innerRad
    outer radius
      $outerRad
    lines
      $nTheta = intmg( 2.*$pi*$outerRad/$ds + 1.5 );
      $nTheta $nr 
    boundary conditions
      -1 -1 5 0
    mappingName
      annulus-unstretched 
    exit
 # stretch the annulus *********
 #
 # Stretch coordinates
  stretch coordinates
    transform which mapping?
      annulus-unstretched
    STRT:multigrid levels $ml
    # Note: this stretching function will change the number of radial grid lines
    Stretch r2:exp to linear
    $dsMin = $ds/$blf;  # Make spacing near wall smaller by a factor of blf
    STP:stretch r2 expl: min dx, max dx $dsMin $ds
    mappingName
      annulus
    exit
 #
  exit
  generate an overlapping grid
    square
    annulus
    done
    change parameters
     # choose implicit or explicit interpolation
     interpolation type
       $interpType
     order of accuracy
       $orderOfAccuracy
     ghost points
       all
       $ng $ng $ng $ng $ng $ng
     exit
    compute overlap
    exit
  save an overlapping grid
  $name
  cilc
  exit

