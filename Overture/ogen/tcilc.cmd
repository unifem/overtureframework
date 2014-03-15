#
# Two circles in a channel (taking arguments)
#
#
# usage: ogen [noplot] tcilc -factor=<num> -order=[2/4/6/8] -interp=[e/i] -blf=<num> -ml=<>
# 
#  -blf : boundary-layer-factor : blf>1 : make grid lines near boundary this many times smaller
#  -ml = number of (extra) multigrid levels to support
# 
# examples:
#
#  ogen -noplot tcilc -order=2 -interp=e -factor=2
#  ogen -noplot tcilc -order=2 -interp=e -factor=4
#  ogen -noplot tcilc -order=2 -interp=e -factor=8   [320K pts]
#
#  ogen -noplot tcilc -order=4 -interp=e -factor=2
#  ogen -noplot tcilc -order=4 -interp=e -factor=4
#  ogen -noplot tcilc -order=4 -interp=e -factor=8
#  ogen -noplot tcilc -order=4 -interp=e -factor=16  [1.2M pts]
#  ogen -noplot tcilc -order=4 -interp=e -factor=32
#
# multigrid:
#  ogen -noplot tcilc -order=2 -interp=e -ml=2 -factor=2
#  ogen -noplot tcilc -order=2 -interp=e -ml=3 -factor=4
#  ogen -noplot tcilc -order=2 -interp=e -ml=3 -factor=8
#  ogen -noplot tcilc -order=2 -interp=e -ml=3 -factor=16
#  ogen -noplot tcilc -order=2 -interp=e -ml=4 -factor=32  [5.8 M]
#  ogen -noplot tcilc -order=2 -interp=e -ml=4 -factor=64
# 
#  ogen -noplot tcilc -order=4 -interp=e -ml=2 -factor=2
#  ogen -noplot tcilc -order=4 -interp=e -ml=3 -factor=4
#  ogen -noplot tcilc -order=4 -interp=e -ml=3 -factor=8
#  ogen -noplot tcilc -order=4 -interp=e -ml=3 -factor=16  [1.2M pts]
#  ogen -noplot tcilc -order=4 -interp=e -ml=4 -factor=32  [5.5M]
#  ogen -noplot tcilc -order=4 -interp=e -ml=4 -factor=64  [22M]
#  ogen -noplot tcilc -order=4 -interp=e -ml=5 -factor=128 [90M]
#  ogen -noplot tcilc -order=4 -interp=e -ml=5 -factor=256 [360M pts]
#
$prefix="tcilc"; 
$order=2; $factor=1; $interp="i"; $ml=0; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
$name=""; $xa=-3.5; $xb=7.5; $ya=-2.5; $yb=2.5; 
$blf=5.;  # blf=1 : means no stretching
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,\
            "interp=s"=> \$interp,"name=s"=> \$name,"ml=i"=>\$ml,"blf=f"=> \$blf, "prefix=s"=> \$prefix);
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
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
*
$ds = .1/$factor; 
$pi=4.*atan2(1.,1.);
*
create mappings
  rectangle
    set corners
      $xa $xb $ya $yb
    lines
     $nx = intmg( ($xb-$xa)/$ds +1.5 ); 
     $ny = intmg( ($yb-$ya)/$ds +1.5 ); 
     $nx $ny
    boundary conditions
      1 1 1 1
    mappingName
     square
    exit
*
  Annulus
    inner radius
      .5
    # decrease the radius as the grids get finer -- but support number of requested MG levels 
    $nr = max( 16, $ml2*4 )+1;
    $nr = intmg( $nr ); 
    $radius = .5 + $ds*$nr/1.5;   # divide by 1.5 to account for stretching
    outer radius
      $radius
    centre for annulus
      -.6 .6
    lines
      $nTheta = intmg( 2.*$pi*$radius/$ds );
      $nTheta $nr
    boundary conditions
      -1 -1 1 0
    mappingName
      unstretched-annulus1
    exit
  * stretch the annulus *********
  *
  * Stretch coordinates
  stretch coordinates
    transform which mapping?
      unstretched-annulus1
    Stretch r2:itanh
      # $dx = .006/$factor;
      $dxMin = $ds/$blf; 
      STP:stretch r2 itanh: position and min dx 0 $dxMin
    stretch grid
*
    mappingName
      annulus1
    exit
  *
*
  Annulus
    inner radius
      .5
    outer radius
      $radius
    centre for annulus
      +.6 -.6
    lines
      $nTheta $nr
    boundary conditions
      -1 -1 1 0
    mappingName
      unstretched-annulus2
    exit
  * stretch the annulus *********
  *
  * Stretch coordinates
  stretch coordinates
    transform which mapping?
    unstretched-annulus2
    Stretch r2:itanh
      STP:stretch r2 itanh: position and min dx 0 $dxMin
    stretch grid
    mappingName
    annulus2
    exit
  *
  exit
  generate an overlapping grid
    square
    annulus1
    annulus2
    done
    change parameters
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    exit
    compute overlap
* pause
    exit
  save a grid (compressed)
    $name
    tcilc
  exit




