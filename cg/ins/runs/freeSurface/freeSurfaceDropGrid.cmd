#
# Grid for a two-dimensional drop with a free surface
#
# Usage:
#    ogen [-noplot] freeSurfaceDropGrid [options]
# 
# where options are
#     -amp=<f>          : amplitude of the free surface (amp)
#     -freq<f>          : frequency of free surface (freq)
#     -factor=<num>     : grid spacing is .1 divided by this factor
#     -interp=[e/i]     : implicit or explicit interpolation
#     -periodic=[n|p]   : periodic=p use periodic boundary conditions
#     -name=<string>    : over-ride the default name  
#     -nExtra           : add extra lines in the normal direction on the boundary fitted grids
#
# Examples:
#
#  ogen -noplot freeSurfaceDropGrid -interp=e -radx=1. -rady=1.25 -factor=2 -ml=1
#  ogen -noplot freeSurfaceDropGrid -interp=e -factor=4 -ml=1
#  ogen -noplot freeSurfaceDropGrid -interp=e -factor=8 -ml=2
#  ogen -noplot freeSurfaceDropGrid -interp=e -factor=16 -ml=3
#  ogen -noplot freeSurfaceDropGrid -interp=e -factor=32 -ml=3
#
#
#
$prefix = "freeSurfaceDropGrid"; 
$amp=.0; $freq=1.; $periodic="p"; 
$factor=1; $name="";  $ml=0; 
$interp="i"; $interpType = "implicit for all grids"; 
$order=2; $orderOfAccuracy = "second order"; $ng=2; 
$radx=1.; $rady=1.1;  
$xa=-1.5; $xb=1.5; $ya=-1.5; $yb=1.5;   # backGround grid
$nExtra=0; 
$cx=0.; $cy=0.; # center of the disk
$freeSurfaceShare=100; # share value for free surface
# 
# get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=>\$factor,"interp=s"=> \$interp,"case=s"=>\$case,\
           "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"nExtra=i"=>\$nExtra,"factor2=f"=>\$factor2,\
           "amp=f"=>\$amp,"freq=f"=>\$freq,"ml=i"=>\$ml,"periodic=s"=>\$periodic,"prefix=s"=> \$prefix,\
           "radx=f"=>\$radx,"rady=f"=>\$rady );
#
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
$suffix = ".order$order"; 
if( $periodic eq "p" ){ $suffix .= ".p"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
#
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
# 
$pi=4.*atan2(1.,1.);
#
# target grid spacing:
$ds0 = .1; 
$ds = $ds0/$factor;
# 
create mappings
#
  rectangle
    $nx=intmg( ($xb-$xa)/$ds+1.5 ); 
    $ny=intmg( ($yb-$ya)/$ds+1.5 ); 
    set corners
      $xa $xb $ya $yb 
    lines
      $nx $ny 
    boundary conditions
      0 0 0 0 
    share
      0 0 0 0 
    mappingName
      backGround
    exit
#
# Create a start curve for the free surface
#
# 
  spline
    $n=21*$factor; 
    enter spline points
      $n 
    $commands="";
    for( $i=0; $i<$n; $i++ ){ $theta=2.*$pi*$i/($n-1); $x=$radx*cos($theta)+$cx; $y=$rady*sin($theta)+$cy; \
                              $commands = $commands . "$x $y\n"; }
      $commands
    lines
      # add a few extra points as the boundary deforms it gets longer
      $stretchFactor=1.0; 
      $length = $stretchFactor*( ($xb-$xa) + $amp*$freq*2 ); # approx. arc length of the free surface
      $ns = intmg( $length/$ds+1.5 );
      $ns
     periodicity
       1
     $cmd
     # open graphics
    # pause
    exit
#  
  hyperbolic
    $nr = intmg( 9 );
    $dist = $ds*($nr - 4 );
    distance to march $dist 
    $linesToMarch = $nr-1;
    lines to march $linesToMarch
    points on initial curve $ns
#    if( $periodic eq "n" ){ $cmd="BC: left fix x, float y and z\n BC: right fix x, float y and z"; }else{ $cmd="#"; }
#     $cmd
    spacing: geometric
    geometric stretch factor 1.05
    backward
    generate
    boundary conditions
      if( $periodic eq "p" ){ $cmd="-1 -1 4 0"; }else{ $cmd="1 2 4 0"; }
      $cmd
    share
      if( $periodic eq "n" ){ $cmd="1 2 $freeSurfaceShare 0"; }else{ $cmd="0 0 $freeSurfaceShare 0"; }
      $cmd
    # -- set the order of data point interpolation: 
    # fourth order
    # second order
    use robust inverse
    name freeSurface
    # pause
    # open graphics
  exit
#
  exit this menu
#
generate an overlapping grid
  backGround
  freeSurface
 done choosing mappings
# 
  change parameters 
    order of accuracy
     $orderOfAccuracy
    interpolation type
      $interpType
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    exit 
#
  # open graphics
  compute overlap
#
exit
#
save an overlapping grid
  $name
  freeSurface
exit
