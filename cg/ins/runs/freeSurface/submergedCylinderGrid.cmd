#
# Grid for a submerged cylinder with a free surface on top.
#
#  The initial free surface is defined by 
#           y = amp * .5 * [ cos( 2 * pi * freq * x ) - 1 ]
#
# Usage:
#    ogen [-noplot] submergedCylinderGrid [options]
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
#  ogen -noplot submergedCylinderGrid -interp=e -factor=1
#  ogen -noplot submergedCylinderGrid -interp=e -factor=2 -ml=1
#  ogen -noplot submergedCylinderGrid -interp=e -factor=4 -ml=1
#  ogen -noplot submergedCylinderGrid -interp=e -factor=8 -ml=2
#  ogen -noplot submergedCylinderGrid -interp=e -factor=16 -ml=3
#  ogen -noplot submergedCylinderGrid -interp=e -factor=32 -ml=3
#
# Longer domain 
#  ogen -noplot submergedCylinderGrid -interp=e -xa=-4 -xb=9 -factor=1
#  ogen -noplot submergedCylinderGrid -interp=e -xa=-4 -xb=9 -factor=2 -ml=1
#  ogen -noplot submergedCylinderGrid -interp=e -xa=-4 -xb=9 -factor=4 -ml=1
#  
#
$amp=.0; $freq=1.; $periodic="n"; 
$factor=1; $name="";  $ml=0; 
$interp="i"; $interpType = "implicit for all grids"; 
$order=2; $orderOfAccuracy = "second order"; $ng=2; 
$xa=-3.; $xb=8.; $ya=-4.; $yb=.5;   # make yb>0 to allow deformed surface to be higher
$nExtra=0; 
$cx=0.; $cy=-1.; # center of the cylinder
$freeSurfaceShare=100; # share value for free surface
# 
# get command line arguments
GetOptions("name=s"=> \$name,"order=i"=>\$order,"factor=f"=>\$factor,"interp=s"=> \$interp,"case=s"=>\$case,\
           "xa=f"=>\$xa,"xb=f"=>\$xb,"ya=f"=>\$ya,"yb=f"=>\$yb,"nExtra=i"=>\$nExtra,"factor2=f"=>\$factor2,\
           "amp=f"=>\$amp,"freq=f"=>\$freq,"ml=i"=>\$ml,"periodic=s"=>\$periodic );
#
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
$suffix = ".order$order"; 
if( $periodic eq "p" ){ $suffix .= ".p"; }
if( $ml ne 0 ){ $suffix .= ".ml$ml"; }
$prefix = "submergedCylinderGrid"; 
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
      if( $periodic eq "p" ){ $cmd="-1 -1 3 0"; }else{ $cmd="1 2 3 0"; }
      $cmd
    share
      if( $periodic eq "n" ){ $cmd="1 2 0 0"; }else{ $cmd="0 0 0 0"; }
      $cmd
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
    $x0=0.; $y0=0.;
    $commands="";
    for( $i=0; $i<$n; $i++ ){ $x=$xa+($xb-$xa)*$i/($n-1); $y=$amp*.5*(cos(2.*$pi*$freq*$x)-1.); \
                              $commands = $commands . "$x $y\n"; }
      $commands
    lines
      # add a few extra points as the boundary deforms it gets longer
      $stretchFactor=1.0; 
      $length = $stretchFactor*( ($xb-$xa) + $amp*$freq*2 ); # approx. arc length of the free surface
      $ns = intmg( $length/$ds+1.5 );
      $ns
     if( $periodic eq "p" ){ $cmd="periodicity\n 1"; }else{ $cmd="periodicity\n 1"; }
     $cmd
     # open graphics
    exit
#  
  hyperbolic
    $nr = intmg( 9 );
    $dist = $ds*($nr - 4 );
    distance to march $dist 
    $linesToMarch = $nr-1;
    lines to march $linesToMarch
    points on initial curve $ns
    if( $periodic eq "n" ){ $cmd="BC: left fix x, float y and z\n BC: right fix x, float y and z"; }else{ $cmd="#"; }
    $cmd
    # Note: equidistribution doesn't work on periodic curves?
    ## if( $periodic eq "n" ){ $cmd="equidistribution 0.4 (in [0,1])"; }else{ $cmd="#"; }
    ## $cmd 
    spacing: geometric
    geometric stretch factor 1.05
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
    # open graphics
  exit
#
#  --- grid for a cylinder 
#
  Annulus
    # Make sure there are at least 4 points on the coarsest MG level
    $nr = max( 5+ $ng + 2*($order-2), 2**($ml+2) );
    $nr = intmg( $nr );
    $innerRad=.5; $outerRad = $innerRad + ($nr-1)*$ds;
    center: $cx $cy
    inner and outer radii
      $innerRad $outerRad
    lines
      $nTheta = intmg( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 1.5 );
      $nTheta $nr
    boundary conditions
      -1 -1 5 0
    share
       0  0 0 0
    mappingName
     cylinder
  exit
#
  exit this menu
#
generate an overlapping grid
  backGround
  cylinder
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
