#
# Grid for a radial elastic piston FSI example (cgmp)
#
#                 inner           outer 
#              +-------------+---------------------+
#              ra            rb                    rc  --> r 
#
#   Notes: If ra=0 then the inner region has a background grid to remove the polar singularity 
#
# usage: ogen [noplot] radialElasticPiston -factor=<num> -order=[2/4/6/8] -interp=[e/i] 
# Options:
#    -ra,rb
# 
# examples:
#   ogen -noplot radialElasticPistonGrid -interp=e -factor=2
#   ogen -noplot radialElasticPistonGrid -interp=e -factor=4
#
#
$prefix="radialElasticPistonGrid";
$order=2; $factor=1; $interp="e"; # default values
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids"; $ml=0; 
$name=""; 
$ra=.35; $rb=.5; $rc=1.; 
# 
# get command line arguments
GetOptions( "order=i"=>\$order,"factor=f"=> \$factor,"ra=f"=> \$ra,"rb=f"=> \$rb,"rc=f"=> \$rc,\
            "interp=s"=> \$interp,"name=s"=> \$name,"per=i"=>\$per,"dsx=f"=>\$dsx,"prefix=s"=>\$prefix );
# 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
# 
$suffix = ".order$order"; 
if( $per eq 1 ){ $suffix .= "p"; }
if( $name eq "" ){$name = $prefix . "$interp$factor" . $suffix . ".hdf";}
# 
$ds=.1/$factor;
$pi = 4.*atan2(1.,1.);
# 
# -- convert a number so that it is a power of 2 plus 1 --
#    ml = number of multigrid levels 
$ml2 = 2**$ml; 
sub intmg{ local($n)=@_; $n = int(int($n+$ml2-2)/$ml2)*$ml2+1; return $n; }
sub max{ local($n,$m)=@_; if( $n>$m ){ return $n; }else{ return $m; } }
#
$bcInterface=100;  # bc for interfaces
$shareInterface=100;        # share value for interfaces
#
create mappings
#
# --- make a nurbs for a circle ----
#   **** Later we could make different curves for the interface ****
$radX=$rb; $radY=$rb;
include ellipseCurve.h
# 
# -- Make a hyperbolic grid --
#
  $nr = intmg( 5 + $order/2 );
  hyperbolic
    forward
    # Fixed radial distance
    $nDistInterface=.2;
    $nrm = intmg($nDistInterface/$ds+4.5);
    # $nDistInterface=($nr-3)*$ds;
    # $nrm=$nr-1; 
    distance to march $nDistInterface
    lines to march $nrm
    $nThetaInterface = intmg($arcLength/$ds+1.5);
    points on initial curve $nThetaInterface
    uniform dissipation 0.05
    volume smooths $numberOfVolumeSmooths
    equidistribution 0 (in [0,1])
    #
    # spacing: geometric
    # geometric stretch factor 1.05 
    #
    generate
    boundary conditions
      -1 -1 100 0 0 0
    share 
       0 0 100 0 0 0
    name fluidInterface
  exit
#
# --- inner domain as a hyperbolic mapping
  hyperbolic
    backward
    $nDist = $rb-$ra;
    $nrm = intmg($nDist/$ds+4.5);
    distance to march $nDist
    lines to march $nrm
    points on initial curve $nThetaInterface
    uniform dissipation 0.05
    volume smooths $numberOfVolumeSmooths
    equidistribution 0. (in [0,1])
    #
    spacing: geometric
    geometric stretch factor 1.05 
    #
    generate
    boundary conditions
      -1 -1 100 1 0 0
    share 
       0 0 100 0 0 0
    name innerRegionHype
  exit
#
# -- Outer annulus ----
#
Annulus
  inner and outer radii
    # inner radius needs to account for the interface moving inward, us ra instead of rb
    $innerRad=$ra + $nDistInterface -($order-2)*$ds; $outerRad=$rc; 
    $innerRad $outerRad
  lines
    $nTheta = intmg( 2.*$pi*($innerRad+$outerRad)*.5/$ds + 3.5 );
    $nr = intmg( ($outerRad-$innerRad)/$ds + 1.5 );
    $nTheta $nr
  boundary conditions
    -1 -1 0 4 
  mappingName
    outerRegion
exit
#
# -- Inner annulus ----
#
Annulus
  inner and outer radii
    $innerRad=$rb; $outerRad=$ra; # note: reverse order so interface is at r2=0 
    $innerRad $outerRad
  lines
    $nr = intmg( abs($outerRad-$innerRad)/$ds + 3.5 );
    $nThetaInterface $nr
  boundary conditions
    -1 -1 100 0
  share
     0  0 100 0
  mappingName
    innerRegion
 exit
#
#  --- Inner solid background grid ---
#
 rectangle
  $dsr=$ds*.8; # make spacing a litle smaller 
  $xb=$ra+2*$ds; $xa=-$xb; $ya=$xa; $yb=$xb; 
  set corners
    $xa $xb $ya $yb
  lines
    $nx = int( ($xb-$xa)/$dsr +1.5 ); 
    $ny = int( ($yb-$ya)/$dsr +1.5 ); 
    $nx $ny
  boundary conditions
    0 0 0 0 
  share 
    0 0 0 0 
  mappingName
    innerBackGround
  exit
#
exit this menu
#
generate an overlapping grid
  outerRegion
  fluidInterface
  #
  innerBackGround
  innerRegion
  done choosing mappings
  change parameters
    specify a domain
     solidDomain
     innerBackGround
     innerRegion
    done
    specify a domain
     fluidDomain
     outerRegion
     fluidInterface
    done
    # choose implicit or explicit interpolation
    interpolation type
      $interpType
    order of accuracy 
      $orderOfAccuracy
    ghost points
      all
      2 2 2 2 2 2
    exit
  # open graphics
  compute overlap
exit
#
# save an overlapping grid
save a grid (compressed)
$name
radialElasticPistonGrid
exit

